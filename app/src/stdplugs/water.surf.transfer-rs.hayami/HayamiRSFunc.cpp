/**
  \file HayamiRSFunc.h
  \brief header of HayamiRSFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
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

  DECLARE_SIGNATURE_ID(wxT("water.surf.transfer-rs.hayami"));
  DECLARE_SIGNATURE_NAME(wxT("water transfer on reach segments using hayami propagation method"));
  DECLARE_SIGNATURE_DESCRIPTION(wxT(""));
  DECLARE_SIGNATURE_DOMAIN(wxT("hydrology"));
  
  DECLARE_SIGNATURE_STATUS(mhydasdk::base::BETA);
  
  DECLARE_SIGNATURE_SDKVERSION;
  
  DECLARE_SIGNATURE_AUTHORNAME(wxT("Moussa R., Fabre J.-C., Louchart X."));
  DECLARE_SIGNATURE_AUTHOREMAIL(wxT("moussa@supagro.inra.fr, fabrejc@supagro.inra.fr, louchart@supagro.inra.fr"));  
  
  
  DECLARE_RS_PRODUCED_VAR("water.surf.Q.downstream-rs",wxT("output volume at the outlet of the ditch"),wxT("m3/s"));
  DECLARE_RS_PRODUCED_VAR("water.surf.H.level-rs",wxT("water height at the outlet of the ditch"),wxT("m"));
  
  DECLARE_RS_REQUIRED_PROPERTY("nmanning",wxT("Manning roughness coefficient"),wxT("s/m^(-1/3)"));
  
  DECLARE_SU_USED_VAR("water.surf.Q.downstream-su",wxT("output volume at the outlet of the SUs"),wxT("m3/s"));
  
  DECLARE_FUNCTION_PARAM("maxsteps",wxT("maximum hayami kernel steps"),wxT(""));
  DECLARE_FUNCTION_PARAM("meancel",wxT("wave mean celerity on RSs"),wxT("m/s"));  
  DECLARE_FUNCTION_PARAM("meansigma",wxT("mean diffusivity on RSs"),wxT("m2/s"));  
  DECLARE_FUNCTION_PARAM("calibstep",wxT("calibration step for height-discharge relation"),wxT("m"));    
  DECLARE_FUNCTION_PARAM("rsbuffer",wxT("buffer upon reach for water heigh over reach height"),wxT("m"));  



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


bool HayamiRSFunction::initParams(mhydasdk::core::ParamsMap Params)
{
  MHYDAS_GetFunctionParam(Params,wxT("maxsteps"),&m_MaxSteps);  
  MHYDAS_GetFunctionParam(Params,wxT("meancel"),&m_MeanCelerity);
  MHYDAS_GetFunctionParam(Params,wxT("meansigma"),&m_MeanSigma);
  MHYDAS_GetFunctionParam(Params,wxT("calibstep"),&m_CalibrationStep);
  MHYDAS_GetFunctionParam(Params,wxT("rsbuffer"),&m_RSBuffer);
  return true;
}

// =====================================================================
// =====================================================================


bool HayamiRSFunction::prepareData()
{
  // On verifie s'il existe des SU pour recuperer leur debit
   
  m_UseUpSUOutput = false;   
  if (mp_CoreData->getSpatialData()->getSUsCollection()->size() > 0)
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


bool HayamiRSFunction::initializeRun(const mhydasdk::base::SimulationInfo* SimInfo)
{
  mhydasdk::core::ReachSegment* RS;
  float Cel, Sigma;
  mhydasdk::core::HOID ID;
  mhydasdk::core::MHYDASScalarValue TmpValue;
  DECLARE_RS_ORDERED_LOOP;
 
 
  BEGIN_RS_ORDERED_LOOP(RS)
    ID = RS->getID();  
    
    m_Input[ID] = new mhydasdk::core::SerieOfMHYDASScalarValue();
    m_HeightDischarge[ID] = new mhydasdk::core::SerieOfMHYDASScalarValue();
    m_CurrentInputSum[ID] = 0;
           
    m_MeanSlope = m_MeanSlope + RS->getUsrSlope();
    MHYDAS_GetDistributedProperty(RS,wxT("nmanning"),&TmpValue);
    m_MeanManning = m_MeanManning + TmpValue;    
  END_LOOP

  m_MeanSlope = m_MeanSlope / mp_CoreData->getSpatialData()->getRSsCollection()->size();
  m_MeanManning = m_MeanManning / mp_CoreData->getSpatialData()->getRSsCollection()->size(); 

  BEGIN_RS_ORDERED_LOOP(RS)
    MHYDAS_GetDistributedProperty(RS,wxT("nmanning"),&TmpValue);
    Cel = m_MeanCelerity * (m_MeanManning / TmpValue) * (sqrt((RS->getUsrSlope() / m_MeanSlope)));
    Sigma = m_MeanSigma * (TmpValue / m_MeanManning) * (m_MeanSlope / RS->getUsrSlope());      
    m_RSKernel[RS->getID()] = t_HayamiKernel();
    ComputeHayamiKernel(Cel, Sigma,RS->getUsrLength(),m_MaxSteps,SimInfo->getTimeStep(), &m_RSKernel[RS->getID()]);        
  END_LOOP

  
  // calcul de la relation hauteur débit
  
  double HydrauRadius, Speed, Q, Section, CurrentHeight;
  int i, StepsNbr;
  
  BEGIN_RS_ORDERED_LOOP(RS)

    MHYDAS_GetDistributedProperty(RS,wxT("nmanning"),&TmpValue);    
    
    StepsNbr = int(ceil((RS->getUsrHeight() + m_RSBuffer) / m_CalibrationStep));
    for (i=0;i<StepsNbr;i++)
    {
      CurrentHeight = i * m_CalibrationStep;
      
      Section = RS->getUsrWidth() * CurrentHeight;
      HydrauRadius = Section / ((2 * CurrentHeight) + RS->getUsrWidth());
      Speed = (1/TmpValue) * pow(HydrauRadius,0.6666) * pow(RS->getUsrSlope(),0.5);   
      Q = Speed * Section;
    
      m_HeightDischarge[RS->getID()]->push_back(Q);
      
    }
        
    
  END_LOOP
  
  
  return true;
}



// =====================================================================
// =====================================================================


bool HayamiRSFunction::runStep(const mhydasdk::base::SimulationStatus* SimStatus)
{
  int ID;
  int CurrentStep;
  int TimeStep;
  float UpSrcSUsOutputsSum;  
  float UpLatSUsOutputsSum;  
  float UpRSsOutputsSum;  
  mhydasdk::core::MHYDASScalarValue QOutput;
  mhydasdk::core::MHYDASScalarValue QInput;
  mhydasdk::core::MHYDASScalarValue TmpValue;

  
  mhydasdk::core::ReachSegment* RS;
  mhydasdk::core::ReachSegment* UpRS;
  mhydasdk::core::SurfaceUnit* UpSU;

  list<mhydasdk::core::SurfaceUnit*>::iterator UpSUiter;
  list<mhydasdk::core::SurfaceUnit*>* UpSUsList;
  list<mhydasdk::core::ReachSegment*>::iterator UpRSiter;
  list<mhydasdk::core::ReachSegment*>* UpRSsList;  
  
 
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
      UpSUsList = RS->getSrcUpstreamSUs();
      
      for(UpSUiter=UpSUsList->begin(); UpSUiter != UpSUsList->end(); UpSUiter++)
      {                
        UpSU = *UpSUiter;
        
        MHYDAS_GetDistributedVarValue(UpSU,wxT("water.surf.Q.downstream-su"),CurrentStep,&TmpValue);
        UpSrcSUsOutputsSum = UpSrcSUsOutputsSum + TmpValue; // / UpSU->getUsrArea();
      }  
    }
  

    // 1.b calcul du debit provenant des SU laterales

    
    UpLatSUsOutputsSum = 0;
    if (m_UseUpSUOutput)
    {      
      UpSUsList = RS->getLatUpstreamSUs();
      
      for(UpSUiter=UpSUsList->begin(); UpSUiter != UpSUsList->end(); UpSUiter++)
      {                
        UpSU = *UpSUiter;
        
        MHYDAS_GetDistributedVarValue(UpSU,wxT("water.surf.Q.downstream-su"),CurrentStep,&TmpValue);        
        UpLatSUsOutputsSum = UpLatSUsOutputsSum + TmpValue;// / UpSU->getUsrArea();                
        
      }  
    }


    // 2.a calcul des debits provenant des RS amont
      
    
    UpRSsOutputsSum = 0;

    UpRSsList = RS->getUpstreamReaches();
      
    for(UpRSiter=UpRSsList->begin(); UpRSiter != UpRSsList->end(); UpRSiter++) \
    {                
      UpRS = *UpRSiter;
      MHYDAS_GetDistributedVarValue(UpRS,wxT("water.surf.Q.downstream-rs"),CurrentStep,&TmpValue);
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

    
    MHYDAS_AppendDistributedVarValue(RS,wxT("water.surf.Q.downstream-rs"),QOutput);

    
    if (!computeWaterHeightFromDischarge(ID,QOutput,&TmpValue))
    {      
      MHYDAS_RaiseWarning(wxT("water.surf.transfer-rs.hayami"),SimStatus->getCurrentStep(),wxT("cannot compute water height on RS ") + wxString::Format(wxT("%d"),ID));
    }
        
    
    MHYDAS_AppendDistributedVarValue(RS,wxT("water.surf.H.level-rs"),TmpValue);    
    
  END_LOOP

  return true;
}


// =====================================================================
// =====================================================================


bool HayamiRSFunction::finalizeRun(const mhydasdk::base::SimulationInfo* SimInfo)
{

  return true;
}

// =====================================================================
// =====================================================================

bool HayamiRSFunction::computeWaterHeightFromDischarge(mhydasdk::core::HOID ID, mhydasdk::core::MHYDASScalarValue Discharge, mhydasdk::core::MHYDASScalarValue *Height)
{
    

  if (isnan(Discharge)) return false;
  
  if (Discharge < 0) return false;

    
  
  if (Discharge == 0) *Height = 0;
  else
  {
   
    
    int i;
    float Q1, Q2, H1, H2;
    
    mhydasdk::core::MHYDASVectorValue* HeightDischarge = m_HeightDischarge[ID]; 

   
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
