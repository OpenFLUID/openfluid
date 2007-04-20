/**
  \file HayamiRSFunc.h
  \brief header of HayamiRSFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "HayamiRSFunc.h"
#include <math.h>

#include <iostream>

HayamiRSFunction::HayamiRSFunction(mhydasdk::core::CoreRepository *CoreData)
                : Function(CoreData)
{
  RS_VARIABLE_TO_CREATE("qoutput");

  RS_PROPERTY_TO_CHECK("nmanning");



  m_MaxSteps = 100;    
  m_MeanCelerity = 0.49;    
  m_MeanSigma = 500;
  m_MeanSlope = 0;    
  m_MeanManning = 0;        

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
  //if (Params.find(wxT("maxsteps")) != Params.end()) m_MaxSteps = (int)(Params[wxT("maxsteps")]);
  MHYDAS_GetFunctionParam(Params,wxT("maxsteps"),&m_MaxSteps);  
  //if (Params.find(wxT("meancel")) != Params.end()) m_MeanCelerity = Params[wxT("meancel")];      
  MHYDAS_GetFunctionParam(Params,wxT("meancel"),&m_MeanCelerity);
  // if (Params.find(wxT("meansigma")) != Params.end()) m_MeanSigma = Params[wxT("meansigma")];
  MHYDAS_GetFunctionParam(Params,wxT("meansigma"),&m_MeanSigma);
  return true;
}


// =====================================================================
// =====================================================================


bool HayamiRSFunction::initializeRun(mhydasdk::base::SimulationInfo* SimInfo)
{
  mhydasdk::core::ReachSegment* RS;
  float Cel, Sigma;
  int ID;
  float TmpValue;
  DECLARE_RS_ORDERED_LOOP;
 
 
  BEGIN_RS_ORDERED_LOOP(RS)
    ID = RS->getID();  
    
    m_Input[ID] = new mhydasdk::core::VectorOfDouble();
    m_CurrentInputSum[ID] = 0;
           
    m_MeanSlope = m_MeanSlope + RS->getUsrSlope();
    MHYDAS_GetDistributedProperty(RS,wxT("nmanning"),&TmpValue);
    m_MeanManning = m_MeanManning + TmpValue;
    //m_MeanManning = m_MeanManning + RS->getProperties()->find(wxT("nmanning"))->second;    
  END_LOOP

  m_MeanSlope = m_MeanSlope / mp_CoreData->getSpatialData()->getRSsCollection()->size();
  m_MeanManning = m_MeanManning / mp_CoreData->getSpatialData()->getRSsCollection()->size(); 

  BEGIN_RS_ORDERED_LOOP(RS)
    MHYDAS_GetDistributedProperty(RS,wxT("nmanning"),&TmpValue);
//    Cel = m_MeanCelerity * (m_MeanManning / RS->getProperties()->find(wxT("nmanning"))->second) * (sqrt((RS->getUsrSlope() / m_MeanSlope)));
//    Sigma = m_MeanSigma * (RS->getProperties()->find(wxT("nmanning"))->second / m_MeanManning) * (m_MeanSlope / RS->getUsrSlope());      
    Cel = m_MeanCelerity * (m_MeanManning / TmpValue) * (sqrt((RS->getUsrSlope() / m_MeanSlope)));
    Sigma = m_MeanSigma * (TmpValue / m_MeanManning) * (m_MeanSlope / RS->getUsrSlope());      
    m_RSKernel[RS->getID()] = ComputeHayamiKernel(Cel, Sigma,RS->getUsrLength(),m_MaxSteps,SimInfo->getTimeStep());        
  END_LOOP



  return true;
}


// =====================================================================
// =====================================================================


bool HayamiRSFunction::checkConsistency()
{
  // On verifie s'il existe des SU pour r�cup�rer leur d�bit
  m_UseUpSUOutput = false; 
  if (mp_CoreData->getSpatialData()->getSUsCollection()->size() > 0)
  {
    SU_VARIABLE_TO_CHECK("qoutput");
    m_UseUpSUOutput = true;
  } 

  return Function::checkConsistency();
}


// =====================================================================
// =====================================================================


bool HayamiRSFunction::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{
  int ID;
  int CurrentStep;
  int TimeStep;
  int i;
  float UpSrcSUsOutputsSum;  
  float UpLatSUsOutputsSum;  
  float UpRSsOutputsSum;  
  float QOutput;
  float QInput;
  float TmpValue;

  
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
    
    // 1.a calcul du d�bit provenant des SU sources qui se jettent dans les noeuds sources    
    
    UpSrcSUsOutputsSum = 0;
    if (m_UseUpSUOutput)
    {      
      UpSUsList = RS->getSrcUpstreamSUs();
      
      for(UpSUiter=UpSUsList->begin(); UpSUiter != UpSUsList->end(); UpSUiter++)
      {                
        UpSU = *UpSUiter;
        MHYDAS_GetDistributedVarValue(UpSU,wxT("qoutput"),CurrentStep,&TmpValue);
        UpSrcSUsOutputsSum = UpSrcSUsOutputsSum + TmpValue / UpSU->getUsrArea();
      }  
    }
  

    // 1.b calcul du d�bit provenant des SU lat�rales

    UpLatSUsOutputsSum = 0;
    if (m_UseUpSUOutput)
    {      
      UpSUsList = RS->getLatUpstreamSUs();
      
      for(UpSUiter=UpSUsList->begin(); UpSUiter != UpSUsList->end(); UpSUiter++)
      {                
        UpSU = *UpSUiter;
        //std::cerr << GET_SIMVAR_VALUE(UpSU,"qoutput",CurrentStep) << std::endl;
        MHYDAS_GetDistributedVarValue(UpSU,wxT("qoutput"),CurrentStep,&TmpValue);
        UpLatSUsOutputsSum = UpLatSUsOutputsSum + TmpValue / UpSU->getUsrArea();                
        
      }  
    }

    //std::cerr << CurrentStep << "," << ID << " -> " << UpSrcSUsOutputsSum << " " << UpLatSUsOutputsSum << std::endl;

    // 2.a calcul des d�bits provenant des RS amont
    
    UpRSsOutputsSum = 0;

    UpRSsList = RS->getUpstreamReaches();
      
    for(UpRSiter=UpRSsList->begin(); UpRSiter != UpRSsList->end(); UpRSiter++) \
    {                
      UpRS = *UpRSiter;
        //std::cerr << GET_SIMVAR_VALUE(UpSU,"qoutput",CurrentStep) << std::endl;
      MHYDAS_GetDistributedVarValue(UpRS,wxT("qoutput"),CurrentStep,&TmpValue);
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
        
    MHYDAS_AppendDistributedVarValue(RS,wxT("qoutput"),QOutput);

  END_LOOP

  return true;
}


// =====================================================================
// =====================================================================


bool HayamiRSFunction::finalizeRun(mhydasdk::base::SimulationInfo* SimInfo)
{

  return true;
}


