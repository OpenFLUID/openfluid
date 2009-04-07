/**
  \file HayamiRSFunc.h
  \brief header of HayamiRSFunction

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "HayamiRSGUFunc.h"
#include <math.h>
#include "openfluid-tools.h"

#include <iostream>

// =====================================================================
// =====================================================================

DEFINE_FUNCTION_HOOK(HayamiRSFunction)


// =====================================================================
// =====================================================================

BEGIN_SIGNATURE_HOOK;
  DECLARE_SIGNATURE_ID(("water.surf.transfer-rs-exchange-gu.hayami-tank"));
  DECLARE_SIGNATURE_NAME(("water transfer on reach segments using hayami propagation method; surface water/groundwater exchanges + exchanges betwen groundwater units"));
  DECLARE_SIGNATURE_DESCRIPTION(("WARNING: UNIQUE BUFFER WITHOUT TAKING INTO ACCOUNT THE HEIGHT OF THE REACHES"));
  DECLARE_SIGNATURE_DOMAIN(("hydrology"));

  DECLARE_SIGNATURE_STATUS(openfluid::base::BETA);

  DECLARE_SIGNATURE_SDKVERSION;

  DECLARE_SIGNATURE_AUTHORNAME(("Moussa. R, Dages C., Louchart X., Fabre J.-C."));
  DECLARE_SIGNATURE_AUTHOREMAIL(("moussa@supagro.inra.fr, dages@supagro.inra.fr, louchart@supagro.inra.fr, fabrejc@supagro.inra.fr"));



  // Produced variables
  DECLARE_RS_PRODUCED_VAR("water.surf.Q.downstream-rs",("Output volume at the outlet of the ditch"),("m3/s"));
  DECLARE_RS_PRODUCED_VAR("water.surf.H.level-rs",("Water height at the outlet of the ditch"),("m"));
  DECLARE_RS_PRODUCED_VAR("water.surf-sz.Q.exchange-rs-gu",("water flux exchanged betwwen RS and GU unit"),("m3/s"));

  DECLARE_GU_PRODUCED_VAR("water.sz.Q.output",("water flux output of the GU"),("m3/s"));
  DECLARE_GU_PRODUCED_VAR("water.sz-surf.Q.exfiltration",("exfiltration from GU to RS"),("m3/s/m2"));
  DECLARE_GU_PRODUCED_VAR("water.sz.H.watertable",("watertable depth, positive value from soil surface"),("m"));

  // Required variables
  DECLARE_SU_REQUIRED_VAR("water.surf.H.infiltration",(""),("m"));
  DECLARE_SU_USED_VAR("water.surf.Q.downstream-su",("output volume at the outlet of the SUs"),("m3/s"));


  // Required initial conditions
  DECLARE_SU_REQUIRED_INICOND("thetains",(""),("m3/m3"));
  DECLARE_GU_REQUIRED_INICOND("iniwatertable",("initial watertable depth, positive value from soil surface"),("m"));

  // Required properties
  DECLARE_SU_REQUIRED_PROPERTY("thetasat",(""),("m3/m3"));
  DECLARE_RS_REQUIRED_PROPERTY("nmanning",("Manning roughness coefficient"),(""));

  DECLARE_REQUIRED_INPUTDATA("area","SU","","m2");

  DECLARE_REQUIRED_INPUTDATA("length","RS","","m");
  DECLARE_REQUIRED_INPUTDATA("width","RS","","m");
  DECLARE_REQUIRED_INPUTDATA("height","RS","","m");
  DECLARE_REQUIRED_INPUTDATA("slope","RS","","m/m");

  DECLARE_REQUIRED_INPUTDATA("area","GU","","m2");

  // Function parameters
  DECLARE_FUNCTION_PARAM("maxsteps",("maximum hayami kernel steps"),(""));
  DECLARE_FUNCTION_PARAM("meancel",("wave mean celerity on RSs"),("m/s"));
  DECLARE_FUNCTION_PARAM("meansigma",("mean diffusivity on RSs"),("m2/s"));
  DECLARE_FUNCTION_PARAM("calibstep",("calibration step for height-discharge relation"),("m"));
  DECLARE_FUNCTION_PARAM("rsbuffer",("buffer upon reach for water heigh over reach height"),("m"));

  DECLARE_FUNCTION_PARAM("coeffinfiltration",("multiplicative exchange coefficient when watertable level is under surface level"),(""));
  DECLARE_FUNCTION_PARAM("coeffdrainage",("multiplicative exchange coefficient when watertable level is above surface level"),(""));
  DECLARE_FUNCTION_PARAM("coeffgw",("multiplicative exchange coefficient between GUs"),(""));

END_SIGNATURE_HOOK


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

  m_CoeffInfiltration = 1;
  m_CoeffDrainage = 1;
  m_CoeffGW = 1;

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
  OPENFLUID_GetFunctionParameter(Params,("coeffinfiltration"),&m_CoeffInfiltration);
  OPENFLUID_GetFunctionParameter(Params,("coeffdrainage"),&m_CoeffDrainage);
  OPENFLUID_GetFunctionParameter(Params,("coeffgw"),&m_CoeffGW);
  return true;
}

// =====================================================================
// =====================================================================


bool HayamiRSFunction::prepareData()
{
  // On verifie s'il existe des SU pour recuperer leur debit

  unsigned int SUCount = 0;

  m_UseUpSUOutput = false;

  OPENFLUID_GetUnitsCount("SU",&SUCount);

  if (SUCount > 0)
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
  std::list<openfluid::core::Unit*>* SUList;
  std::list<openfluid::core::Unit*>::iterator Iter;
  openfluid::core::Unit *GU;
  openfluid::core::Unit *SU;
  openfluid::core::Unit *RS;

  openfluid::core::ScalarValue RSheight, RSwidth, RSslope, RSmanning, RSlength;
  openfluid::core::ScalarValue SUslope,SUarea,SUthetaini, SUthetasat;
  openfluid::core::ScalarValue GUarea;

  float Cel, Sigma;
  //openfluid::core::ScalarValue TmpValue;
  int ID;


  DECLARE_GU_ORDERED_LOOP;

  DECLARE_RS_ORDERED_LOOP;


  BEGIN_RS_ORDERED_LOOP(RS)
  ID = RS->getID();

  m_Input[ID] = new openfluid::core::SerieOfScalarValue();
  m_HeightDischarge[ID] = new openfluid::core::SerieOfScalarValue();
  m_CurrentInputSum[ID] = 0;

  OPENFLUID_GetInputData(RS,"slope",&RSslope);
  m_MeanSlope = m_MeanSlope + RSslope;
  OPENFLUID_GetInputData(RS,("nmanning"),&RSmanning);
  m_MeanManning = m_MeanManning + RSmanning;
  //m_MeanManning = m_MeanManning + RS->getProperties()->find(("nmanning"))->second;
  END_LOOP

  m_MeanSlope = m_MeanSlope / mp_CoreData->getUnits("RS")->getList()->size();
  m_MeanManning = m_MeanManning / mp_CoreData->getUnits("RS")->getList()->size();

  BEGIN_RS_ORDERED_LOOP(RS)
    OPENFLUID_GetInputData(RS,"nmanning",&RSmanning);
    OPENFLUID_GetInputData(RS,"slope",&RSslope);
    OPENFLUID_GetInputData(RS,"length",&RSlength);

  //    Cel = m_MeanCelerity * (m_MeanManning / RS->getProperties()->find(("nmanning"))->second) * (sqrt((RS->getUsrSlope() / m_MeanSlope)));
  //    Sigma = m_MeanSigma * (RS->getProperties()->find(("nmanning"))->second / m_MeanManning) * (m_MeanSlope / RS->getUsrSlope());
    Cel = m_MeanCelerity * (m_MeanManning / RSmanning) * (sqrt((RSslope / m_MeanSlope)));
    Sigma = m_MeanSigma * (RSmanning / m_MeanManning) * (m_MeanSlope / RSslope);
    ComputeHayamiKernel(Cel, Sigma,RSlength,m_MaxSteps,SimInfo->getTimeStep(),&m_RSKernel[RS->getID()]);
  END_LOOP


  // calcul de la relation hauteur débit

  double HydrauRadius, Speed, Q, Section, CurrentHeight;
  int i, StepsNbr;

  BEGIN_RS_ORDERED_LOOP(RS)

  OPENFLUID_GetInputData(RS,("nmanning"),&RSmanning);
  OPENFLUID_GetInputData(RS,"slope",&RSslope);
  OPENFLUID_GetInputData(RS,"width",&RSwidth);

  // StepsNbr = int(ceil((RS->getUsrHeight() + m_RSBuffer) / m_CalibrationStep));
  StepsNbr = int(ceil((m_RSBuffer) / m_CalibrationStep));

  //std::cerr << " ATTENTION VERSION AVEC BUFFER UNIQUE SANS TENIR COMPTE HAUTEUR FOSSE " <<std::endl;

  // TODO ATTENTION VERSION AVEC BUFFER UNIQUE SANS TENIR COMPTE HAUTEUR FOSSE



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

  BEGIN_GU_ORDERED_LOOP(GU)
  /* computes ThetaSat and ThetaIni for each GU from ThetaSat and ThetaIniNS of all SUs that exchange with GU
   * ThetaSat and ThetaIni are the average of ThetaSat and ThetaIniNS of SUs weighted by the area of each SU
   */

  SUList = GU->getFromUnits("SU");

  m_ThetaIni[GU->getID()] = 0;
  m_ThetaSat[GU->getID()] = 0;

  for (Iter=SUList->begin(); Iter != SUList->end(); Iter++)
  {
    SU = *Iter;
    OPENFLUID_GetInputData(SU,("thetains"),&SUthetaini);
    OPENFLUID_GetInputData(SU,("thetasat"),&SUthetasat);
    OPENFLUID_GetInputData(SU,("area"),&SUarea);
    m_ThetaIni[GU->getID()] = m_ThetaIni[GU->getID()] + SUthetaini*SUarea;
    m_ThetaSat[GU->getID()] = m_ThetaSat[GU->getID()] + SUthetasat*SUarea;
  }

  // the final sum is divided by the area of the GU
  OPENFLUID_GetInputData(GU,("area"),&GUarea);
  m_ThetaIni[GU->getID()] = m_ThetaIni[GU->getID()] / GUarea;
  m_ThetaSat[GU->getID()] = m_ThetaSat[GU->getID()] / GUarea;

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
  int i;
  float UpSrcSUsOutputsSum;
  float UpLatSUsOutputsSum;
  float UpRSsOutputsSum;
  openfluid::core::ScalarValue QOutput;
  float QInput;
  openfluid::core::ScalarValue TmpValue;
  float TmpGUValue;
  bool m_UseUpGUExchangersgu;

  openfluid::core::ScalarValue WaterTable, WaterTableDown,QguOutput;
  float  InputVol,ExchangeSurface,  OutputVol;
  openfluid::core::ScalarValue TmpExfiltration, Exfiltration;
  openfluid::core::ScalarValue  SUInfiltration; // for SU
  openfluid::core::ScalarValue TmpQExchange;
  float RSHeight,WaterHeight ; // for RS


  openfluid::core::Unit* RS;
  openfluid::core::Unit* UpRS;
  openfluid::core::Unit* UpSU;
  openfluid::core::Unit *GU, *GUex, *GUdown,*GURS;
  openfluid::core::Unit *SU;

  std::list<openfluid::core::Unit*>* GUList;
  std::list<openfluid::core::Unit*>::iterator IterGW;
  std::list<openfluid::core::Unit*>* SUList;
  std::list<openfluid::core::Unit*>::iterator IterSU;
  std::list<openfluid::core::Unit*>::iterator UpSUiter;
  std::list<openfluid::core::Unit*>* UpSUsList;
  std::list<openfluid::core::Unit*>::iterator UpRSiter;
  std::list<openfluid::core::Unit*>* UpRSsList;
  std::list<openfluid::core::Unit*>* RSList;
  std::list<openfluid::core::Unit*>::iterator IterRS;

  openfluid::core::ScalarValue RSheight, RSwidth, RSslope, RSmanning, RSlength;
  openfluid::core::ScalarValue SUslope,SUarea,SUthetaini, SUthetasat;
  openfluid::core::ScalarValue GUarea;



  TimeStep = SimStatus->getTimeStep();
  CurrentStep = SimStatus->getCurrentStep();

  DECLARE_RS_ORDERED_LOOP;
  DECLARE_GU_ORDERED_LOOP;

  if (CurrentStep == 0)
  {
    //exchange between GUs
    BEGIN_GU_ORDERED_LOOP(GU)
    // water level initializing
    ID = GU->getID();

    OPENFLUID_GetInputData(GU,("iniwatertable"),&WaterTable);

    OPENFLUID_AppendVariable(GU,("water.sz.Q.output"),0);
    OPENFLUID_AppendVariable(GU,("water.sz-surf.Q.exfiltration"),0);

    // Computes recharge from SUs
    InputVol = 0;

    SUList = GU->getFromUnits("SU");

    for (IterSU=SUList->begin(); IterSU != SUList->end(); IterSU++)
    {
      SU = *IterSU;
      OPENFLUID_GetInputData(SU,"area",&SUarea);
      OPENFLUID_GetVariable(SU,("water.surf.H.infiltration"),SimStatus->getCurrentStep(),&SUInfiltration);
      InputVol = InputVol + SUInfiltration * SUarea;
    }
    // Updates WaterTable according to the recharge
    OPENFLUID_GetInputData(GU,"area",&GUarea);
    WaterTable = WaterTable - (InputVol / (GUarea*(m_ThetaSat[GU->getID()] - m_ThetaIni[GU->getID()])));
    OPENFLUID_AppendVariable(GU,("water.sz.H.watertable"),WaterTable);

    END_LOOP


    BEGIN_RS_ORDERED_LOOP(RS)

    ID = RS->getID();
    TmpQExchange = 0;
    OPENFLUID_AppendVariable(RS,("water.surf-sz.Q.exchange-rs-gu"),TmpQExchange);

    END_LOOP

  }




  BEGIN_RS_ORDERED_LOOP(RS)

  ID = RS->getID();

  QOutput = 0;

  // 1.a calcul du debit provenant des SU sources qui se jettent dans les noeuds sources

  // TODO comment on traite différence entrée latéral / source

  UpSrcSUsOutputsSum = 0;
  if (m_UseUpSUOutput)
  {
    UpSUsList = RS->getFromUnits("SU");

    for(UpSUiter=UpSUsList->begin(); UpSUiter != UpSUsList->end(); UpSUiter++)
    {
      UpSU = *UpSUiter;
      OPENFLUID_GetVariable(UpSU,("water.surf.Q.downstream-su"),CurrentStep,&TmpValue);
      UpSrcSUsOutputsSum = UpSrcSUsOutputsSum + TmpValue; // / UpSU->getUsrArea();

      if (CurrentStep != 0)
      {
        GUex = mp_CoreData->getUnit("GU",UpSU->getToUnits("GU")->front()->getID());
        OPENFLUID_GetVariable(GUex,("water.sz-surf.Q.exfiltration"),CurrentStep-1,&TmpExfiltration);

        OPENFLUID_GetInputData(UpSU,"area",&SUarea);
        UpSrcSUsOutputsSum = UpSrcSUsOutputsSum + TmpExfiltration* SUarea;
      }
    }
  }


  // TODO comment on traite différence entrée latéral / source
  // 1.b calcul du debit provenant des SU laterales

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

      if (CurrentStep != 0)
      {
        GUex = mp_CoreData->getSpatialData()->getGUByID(UpSU->getGUExchangeID());
        OPENFLUID_GetVariable(GUex,("water.sz-surf.Q.exfiltration"),CurrentStep-1,&TmpExfiltration);
        UpLatSUsOutputsSum = UpLatSUsOutputsSum + TmpExfiltration* UpSU->getUsrArea();
      }

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


  // OPENFLUID_AppendDistributedVarValue(RS,("qoutput"),QOutput);

  //3. Premier calcul de la hauteur d'eau

  //if (!computeWaterHeightFromDischarge(ID,QOutput,&TmpValue)) std::cerr << "ça dépasse ID: " << ID <<std::endl;
  if (!computeWaterHeightFromDischarge(ID,QOutput,&WaterHeight))
  {
    std::string IDStr;
    openfluid::tools::ConvertValue(ID,&IDStr);
    OPENFLUID_RaiseWarning(("water.surf.transfer-rs-exchange-gu.hayami-tank"),CurrentStep,("water height is over reach height + buffer on RS ") + IDStr);
  }


  //OPENFLUID_AppendDistributedVarValue(RS,("waterheight"),WaterHeight);


  // Echanges nappes-rivières
  //   4.a. Calcul du débit échangé entre la rivière et l'aquifère


  // TODO à vérifier
  GURS = RS->getToUnits("GU")->front();

  //OPENFLUID_GetDistributedVarValue(RS,("waterheight"),CurrentStep,&WaterHeight);
  OPENFLUID_GetVariable(GURS,("water.sz.H.watertable"),CurrentStep-1,&WaterTable);
  OPENFLUID_GetInputData(RS,"length",&RSlength);
  OPENFLUID_GetInputData(RS,"width",&RSwidth);
  OPENFLUID_GetInputData(RS,"height",&RSheight);


  ExchangeSurface = RSlength * ((2 * WaterHeight) + RSwidth);

  /*  mp_ExecMsgs->setError(("groundwater"),SimStatus->getCurrentStep(),("watertable not present"));
          return false;
   */

  // computes water flux between RS and GU (qexchangersgu)
  RSHeight = RSheight;

  TmpQExchange = 0;

  if (WaterTable <= RSHeight)
  {
    TmpQExchange = m_CoeffDrainage* (RSHeight - WaterHeight - WaterTable) * ExchangeSurface;
  }
  else
  {
    TmpQExchange = -m_CoeffInfiltration * WaterHeight * ExchangeSurface;
  }

  // 4.b. Calcul du bilan à l'échelle du troncon nTR puis calcul de la nouvelle hauteur d'eau
  if ((-TmpQExchange) > QOutput)
  {
    TmpQExchange = -QOutput;
    QOutput = 0;
  }
  else
  {
    QOutput = QOutput + TmpQExchange;
  }


  if (!computeWaterHeightFromDischarge(ID,QOutput,&WaterHeight))
  {
    std::string IDStr;
    openfluid::tools::ConvertValue(ID,&IDStr);
    OPENFLUID_RaiseWarning(("water.surf.transfer-rs-exchange-gu.hayami-tank"),SimStatus->getCurrentStep(),("overflow on RS") + IDStr);
  }

  OPENFLUID_AppendVariable(RS,("water.surf.Q.downstream-rs"),QOutput);

  OPENFLUID_AppendVariable(RS,("water.surf.H.level-rs"),WaterHeight);
  if (CurrentStep != 0)   OPENFLUID_AppendVariable(RS,("water.surf-sz.Q.exchange-rs-gu"),TmpQExchange);


  END_LOOP

  if (CurrentStep != 0)
  {
    /* =======================================================================
     * exchange between GUs
     * ======================================================================= */
    BEGIN_GU_ORDERED_LOOP(GU)


    // Computes recharge from SUs
    InputVol = 0;
    OutputVol = 0;

    SUList = GU->getFromUnits("SU");

    for (IterSU=SUList->begin(); IterSU != SUList->end(); IterSU++)
    {
      SU = *IterSU;
      OPENFLUID_GetVariable(SU,("water.surf.H.infiltration"),CurrentStep,&SUInfiltration);
      OPENFLUID_GetInputData(SU,"area",&SUarea);
      InputVol = InputVol + (SUInfiltration * SUarea);

    }

    // Add to the recharge upGUs fluxes
    // TODO a vérifier
    GUList = GU->getFromUnits("GU");
    for (IterGW=GUList->begin(); IterGW != GUList->end(); IterGW++)
    {
      GUex = *IterGW;
      OPENFLUID_GetVariable(GUex,("water.sz.Q.output"),CurrentStep-1,&QguOutput);
      InputVol = InputVol + QguOutput*TimeStep;
    }

    // computes the Output Volume in relation to the RSs
    RSList = GU->getFromUnits("RS");
    for (IterRS=RSList->begin(); IterRS != RSList->end(); IterRS++)
    {
      RS = *IterRS;



      OPENFLUID_GetVariable(RS,("qexchangersgu"),CurrentStep,&TmpQExchange);
      OutputVol = OutputVol + TmpQExchange*TimeStep;


    }

    //updates the Output Volume with the downGU
    // get WaterTable(t-1)
    OPENFLUID_GetVariable(GU,("water.sz.H.watertable"),CurrentStep-1,&WaterTable);

    QguOutput = 0;

    if (GU->getToUnits("GU")->size() == 1)
    {
      GUdown = GU->getToUnits("GU")->front();
      OPENFLUID_GetVariable(GUdown,("water.sz.H.watertable"),CurrentStep-1,&WaterTableDown); // Watertable on downGU
      QguOutput = m_CoeffGW * (WaterTableDown - WaterTable);
      OutputVol = OutputVol + QguOutput*TimeStep;


    }
    else
    {
      QguOutput = 0;
    }






    OPENFLUID_AppendVariable(GU,("water.sz.Q.output"),QguOutput);

    // Updates WaterTable(t) according to the Input, Output volumes, and WaterTable(t-1)
    OPENFLUID_GetInputData(GU,"area",&GUarea);
    WaterTable = WaterTable - (InputVol - OutputVol) / (GUarea*(m_ThetaSat[GU->getID()] - m_ThetaIni[GU->getID()]));




    // Computes exfiltration if necessary, and updates WaterTable
    if (WaterTable < 0)
    {
      Exfiltration = -WaterTable * (m_ThetaSat[GU->getID()] - m_ThetaIni[GU->getID()]) / SimStatus->getTimeStep();
      WaterTable = 0;
    }
    else
    {
      Exfiltration = 0;
    }

    // TODO Exfiltration à rajouter au ruissellement des SU


    // Append final WaterTable(t) and Exfiltration(t)
    OPENFLUID_AppendVariable(GU,("water.sz.H.watertable"),WaterTable);
    OPENFLUID_AppendVariable(GU,("water.sz-surf.Q.exfiltration"),Exfiltration);


    END_LOOP
  }
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

bool HayamiRSFunction::computeWaterHeightFromDischarge(int ID, float Discharge, float *Height)
{

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
