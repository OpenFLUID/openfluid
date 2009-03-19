/**
  \file HayamiRSFunc.h
  \brief header of HayamiRSFunction

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "HayamiRSFunc.h"
#include <math.h>

#include <iostream>

// =====================================================================
// =====================================================================

DEFINE_FUNCTION_HOOK(HayamiRSFunction)

// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK

  DECLARE_SIGNATURE_ID(("water.surf.transfer-rs.hayami"));
  DECLARE_SIGNATURE_NAME(("water transfer on reach segments using hayami propagation method"));
  DECLARE_SIGNATURE_DESCRIPTION((""));
  DECLARE_SIGNATURE_DOMAIN(("hydrology"));

  DECLARE_SIGNATURE_STATUS(openfluid::base::BETA);

  DECLARE_SIGNATURE_SDKVERSION;

  DECLARE_SIGNATURE_AUTHORNAME(("Moussa R., Fabre J.-C., Louchart X."));
  DECLARE_SIGNATURE_AUTHOREMAIL(("moussa@supagro.inra.fr, fabrejc@supagro.inra.fr, louchart@supagro.inra.fr"));


  DECLARE_RS_PRODUCED_VAR("water.surf.Q.downstream-rs",("output volume at the outlet of the ditch"),("m3/s"));
  DECLARE_RS_PRODUCED_VAR("water.surf.H.level-rs",("water height at the outlet of the ditch"),("m"));

  DECLARE_RS_REQUIRED_PROPERTY("nmanning",("Manning roughness coefficient"),("s/m^(-1/3)"));

  DECLARE_REQUIRED_INPUTDATA("area","SU","","m2");

  DECLARE_REQUIRED_INPUTDATA("length","RS","","m");
  DECLARE_REQUIRED_INPUTDATA("width","RS","","m");
  DECLARE_REQUIRED_INPUTDATA("height","RS","","m");
  DECLARE_REQUIRED_INPUTDATA("slope","RS","","m/m");


  DECLARE_SU_USED_VAR("water.surf.Q.downstream-su",("output volume at the outlet of the SUs"),("m3/s"));

  DECLARE_FUNCTION_PARAM("maxsteps",("maximum hayami kernel steps"),(""));
  DECLARE_FUNCTION_PARAM("meancel",("wave mean celerity on RSs"),("m/s"));
  DECLARE_FUNCTION_PARAM("meansigma",("mean diffusivity on RSs"),("m2/s"));
  DECLARE_FUNCTION_PARAM("calibstep",("calibration step for height-discharge relation"),("m"));
  DECLARE_FUNCTION_PARAM("rsbuffer",("buffer upon reach for water heigh over reach height"),("m"));



END_SIGNATURE_HOOK

// =====================================================================
// =====================================================================


HayamiRSFunction::HayamiRSFunction()
                : PluggableFunction()
{



  // default values
  m_MaxSteps = 100;
  m_MeanCelerity = 0.49;
  m_MeanSigma = 500;
  m_MeanSlope = 0;
  m_MeanManning = 0;
  m_CalibrationStep = 0.01;
  m_RSBuffer = 0;
}


// =====================================================================
// =====================================================================


HayamiRSFunction::~HayamiRSFunction()
{

}


// =====================================================================
// =====================================================================


bool HayamiRSFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{
  OPENFLUID_GetFunctionParameter(Params,("maxsteps"),&m_MaxSteps);
  OPENFLUID_GetFunctionParameter(Params,("meancel"),&m_MeanCelerity);
  OPENFLUID_GetFunctionParameter(Params,("meansigma"),&m_MeanSigma);
  OPENFLUID_GetFunctionParameter(Params,("calibstep"),&m_CalibrationStep);
  OPENFLUID_GetFunctionParameter(Params,("rsbuffer"),&m_RSBuffer);
  return true;
}

// =====================================================================
// =====================================================================


bool HayamiRSFunction::prepareData()
{
  // On verifie s'il existe des SU pour recuperer leur debit

  m_UseUpSUOutput = false;
  if (mp_CoreData->getUnits("SU")->getList()->size() > 0)
  {
    m_UseUpSUOutput = true;
  }

  return true;
}

// =====================================================================
// =====================================================================


bool HayamiRSFunction::checkConsistency()
{

  return true;
}


// =====================================================================
// =====================================================================


bool HayamiRSFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{
  openfluid::core::Unit* RS;
  float Cel, Sigma;
  openfluid::core::UnitID_t ID;
  openfluid::core::ScalarValue RSmanning,RSslope,RSlength,RSheight,RSwidth;
  DECLARE_RS_ORDERED_LOOP;


  BEGIN_RS_ORDERED_LOOP(RS)
    ID = RS->getID();

    m_Input[ID] = new openfluid::core::SerieOfScalarValue();
    m_HeightDischarge[ID] = new openfluid::core::SerieOfScalarValue();
    m_CurrentInputSum[ID] = 0;

    OPENFLUID_GetInputData(RS,("nmanning"),&RSmanning);
    OPENFLUID_GetInputData(RS,("slope"),&RSslope);
    m_MeanSlope = m_MeanSlope + RSslope;
    m_MeanManning = m_MeanManning + RSmanning;
  END_LOOP

  m_MeanSlope = m_MeanSlope / mp_CoreData->getUnits("RS")->getList()->size();
  m_MeanManning = m_MeanManning / mp_CoreData->getUnits("RS")->getList()->size();

  BEGIN_RS_ORDERED_LOOP(RS)
    OPENFLUID_GetInputData(RS,("nmanning"),&RSmanning);
    OPENFLUID_GetInputData(RS,("slope"),&RSslope);
    OPENFLUID_GetInputData(RS,("length"),&RSlength);
    Cel = m_MeanCelerity * (m_MeanManning / RSmanning) * (sqrt((RSslope / m_MeanSlope)));
    Sigma = m_MeanSigma * (RSmanning/ m_MeanManning) * (m_MeanSlope / RSslope);
    m_RSKernel[RS->getID()] = t_HayamiKernel();
    ComputeHayamiKernel(Cel, Sigma,RSlength,m_MaxSteps,SimInfo->getTimeStep(), &m_RSKernel[RS->getID()]);
  END_LOOP


  // calcul de la relation hauteur débit

  double HydrauRadius, Speed, Q, Section, CurrentHeight;
  int i, StepsNbr;

  BEGIN_RS_ORDERED_LOOP(RS)

    OPENFLUID_GetInputData(RS,("nmanning"),&RSmanning);
    OPENFLUID_GetInputData(RS,("height"),&RSheight);
    OPENFLUID_GetInputData(RS,("slope"),&RSslope);
    OPENFLUID_GetInputData(RS,("length"),&RSlength);
    OPENFLUID_GetInputData(RS,("width"),&RSwidth);



    StepsNbr = int(ceil((RSheight + m_RSBuffer) / m_CalibrationStep));
    for (i=0;i<StepsNbr;i++)
    {
      CurrentHeight = i * m_CalibrationStep;

      Section = RSwidth * CurrentHeight;
      HydrauRadius = Section / ((2 * CurrentHeight) + RSwidth);
      Speed = (1/RSmanning) * pow(HydrauRadius,0.6666) * pow(RSslope,0.5);
      Q = Speed * Section;

      m_HeightDischarge[RS->getID()]->push_back(Q);

    }


  END_LOOP


  return true;
}



// =====================================================================
// =====================================================================


bool HayamiRSFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  int ID;
  int CurrentStep;
  int TimeStep;
  float UpSrcSUsOutputsSum;
  float UpLatSUsOutputsSum;
  float UpRSsOutputsSum;
  openfluid::core::ScalarValue QOutput;
  openfluid::core::ScalarValue QInput;
  openfluid::core::ScalarValue TmpValue;


  openfluid::core::Unit* RS;
  openfluid::core::Unit* UpRS;
  openfluid::core::Unit* UpSU;

  std::list<openfluid::core::Unit*>::iterator UpSUiter;
  std::list<openfluid::core::Unit*>* UpSUsList;
  std::list<openfluid::core::Unit*>::iterator UpRSiter;
  std::list<openfluid::core::Unit*>* UpRSsList;


  TimeStep = SimStatus->getTimeStep();
  CurrentStep = SimStatus->getCurrentStep();

  DECLARE_RS_ORDERED_LOOP;
  BEGIN_RS_ORDERED_LOOP(RS)

    ID = RS->getID();

    QOutput = 0;


    // 1.a calcul du debit provenant des SU sources qui se jettent dans les noeuds sources


    UpSrcSUsOutputsSum = 0;
    if (m_UseUpSUOutput)
    {
      UpSUsList = RS->getFromUnits("SU");

      for(UpSUiter=UpSUsList->begin(); UpSUiter != UpSUsList->end(); UpSUiter++)
      {
        UpSU = *UpSUiter;

        OPENFLUID_GetVariable(UpSU,("water.surf.Q.downstream-su"),CurrentStep,&TmpValue);
        UpSrcSUsOutputsSum = UpSrcSUsOutputsSum + TmpValue; // / UpSU->getUsrArea();
      }
    }


    // 1.b calcul du debit provenant des SU laterales

   //TODO à vérifier ce qu'on en fait

    UpLatSUsOutputsSum = 0;
    /*
    if (m_UseUpSUOutput)
    {
      UpSUsList = RS->getLatUpstreamSUs();

      for(UpSUiter=UpSUsList->begin(); UpSUiter != UpSUsList->end(); UpSUiter++)
      {
        UpSU = *UpSUiter;

        OPENFLUID_GetVariable(UpSU,("water.surf.Q.downstream-su"),CurrentStep,&TmpValue);
        UpLatSUsOutputsSum = UpLatSUsOutputsSum + TmpValue;// / UpSU->getUsrArea();

      }
    }*/


    // 2.a calcul des debits provenant des RS amont


    UpRSsOutputsSum = 0;

    UpRSsList = RS->getFromUnits("RS");

    for(UpRSiter=UpRSsList->begin(); UpRSiter != UpRSsList->end(); UpRSiter++) \
    {
      UpRS = *UpRSiter;
      OPENFLUID_GetVariable(UpRS,("water.surf.Q.downstream-rs"),CurrentStep,&TmpValue);
      UpRSsOutputsSum = UpRSsOutputsSum + TmpValue;
    }


    // 2.b propagation via Hayami



    QInput = UpRSsOutputsSum + UpSrcSUsOutputsSum;
    m_CurrentInputSum[ID] = m_CurrentInputSum[ID] + QInput;
    m_Input[ID]->push_back(QInput);

    QOutput = 0;
    if (m_CurrentInputSum[ID] > 0)
    {
      QOutput = DoHayamiPropagation(m_RSKernel[ID], CurrentStep, m_Input[ID], m_MaxSteps, TimeStep);
    }

    QOutput = QOutput + UpLatSUsOutputsSum;


    OPENFLUID_AppendVariable(RS,("water.surf.Q.downstream-rs"),QOutput);


    if (!computeWaterHeightFromDischarge(ID,QOutput,&TmpValue))
    {

      std::string IDStr;
      openfluid::tools::ConvertValue(ID,&IDStr);
      OPENFLUID_RaiseWarning(("water.surf.transfer-rs.hayami"),SimStatus->getCurrentStep(),("cannot compute water height on RS ") + IDStr);
    }


    OPENFLUID_AppendVariable(RS,("water.surf.H.level-rs"),TmpValue);

  END_LOOP

  return true;
}


// =====================================================================
// =====================================================================


bool HayamiRSFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  return true;
}

// =====================================================================
// =====================================================================

bool HayamiRSFunction::computeWaterHeightFromDischarge(openfluid::core::UnitID_t ID, openfluid::core::ScalarValue Discharge, openfluid::core::ScalarValue *Height)
{


  if (isnan(Discharge)) return false;

  if (Discharge < 0) return false;



  if (Discharge == 0) *Height = 0;
  else
  {


    int i;
    float Q1, Q2, H1, H2;

    openfluid::core::SerieOfScalarValue* HeightDischarge = m_HeightDischarge[ID];


    // on determine par boucle le premier débit de la relation H/D supérieur au débit recherché
    i = 0;

    while ((i < HeightDischarge->size()) && (HeightDischarge->at(i) < Discharge))
    {
      i++;
    }


    if (i == HeightDischarge->size())
    {
      *Height = (i-1) * m_CalibrationStep;

      return false;
    }
    else
    {

      Q1 = HeightDischarge->at(i-1);


      Q2 = HeightDischarge->at(i);


      H1 = (i-1) * m_CalibrationStep;
      H2 = i * m_CalibrationStep;


      // risque de division par 0 si Q1 == Q2 !! à revoir, comment fait-on?
      *Height = H1 + ((Discharge-Q1) * (H2-H1) / (Q2-Q1));


    }

  }

  return true;

}
