/**
  \file HayamiSUFunc.h
  \brief header of HayamiSUFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "HayamiSUFunc.h"
#include <iostream>
#include "math.h"

#include "HayamiTools.h"



HayamiSUFunction::HayamiSUFunction(mhydasdk::core::CoreRepository *CoreData)
                : Function(CoreData)
{
  SU_VARIABLE_TO_CREATE("qoutput");
  
  SU_VARIABLE_TO_CHECK("runoff");
   
  SU_PROPERTY_TO_CHECK("nmanning");


  m_MaxSteps = 100;    
  m_MeanCelerity = 0.045;    
  m_MeanSigma = 500;
  m_MeanSlope = 0;    
  m_MeanManning = 0;        

}


// =====================================================================
// =====================================================================


HayamiSUFunction::~HayamiSUFunction()
{

}


// =====================================================================
// =====================================================================


bool HayamiSUFunction::initParams(mhydasdk::core::ParamsMap Params)
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


bool HayamiSUFunction::initializeRun(mhydasdk::base::SimulationInfo* SimInfo)
{
  mhydasdk::core::SurfaceUnit* SU;
  float Cel, Sigma;
  int ID;
  float TmpValue;

  DECLARE_SU_ORDERED_LOOP;
 
 
  BEGIN_SU_ORDERED_LOOP(SU)
    ID = SU->getID();
    
    m_Input[ID] = new mhydasdk::core::VectorOfDouble();
    m_CurrentInputSum[ID] = 0;
  
    m_MeanSlope = m_MeanSlope + SU->getUsrSlope();
    MHYDAS_GetDistributedProperty(SU,wxT("nmanning"),&TmpValue);
    m_MeanManning = m_MeanManning + TmpValue;
    //m_MeanManning = m_MeanManning + SU->getProperties()->find(wxT("nmanning"))->second;    
  END_LOOP

  m_MeanSlope = m_MeanSlope / mp_CoreData->getSpatialData()->getSUsCollection()->size();
  m_MeanManning = m_MeanManning / mp_CoreData->getSpatialData()->getSUsCollection()->size(); 

  BEGIN_SU_ORDERED_LOOP(SU)
    MHYDAS_GetDistributedProperty(SU,wxT("nmanning"),&TmpValue);
    // Cel = m_MeanCelerity * (m_MeanManning / SU->getProperties()->find(wxT("nmanning"))->second) * (sqrt((SU->getUsrSlope() / m_MeanSlope)));
    // Sigma = m_MeanSigma * (SU->getProperties()->find(wxT("nmanning"))->second / m_MeanManning) * (m_MeanSlope / SU->getUsrSlope());      
    Cel = m_MeanCelerity * (m_MeanManning / TmpValue) * (sqrt((SU->getUsrSlope() / m_MeanSlope)));
    Sigma = m_MeanSigma * (TmpValue / m_MeanManning) * (m_MeanSlope / SU->getUsrSlope());    
    m_SUKernel[SU->getID()] = ComputeHayamiKernel(Cel, Sigma,SU->getDownstreamDistance(),m_MaxSteps,SimInfo->getTimeStep());    
  END_LOOP


  return true;
}


// =====================================================================
// =====================================================================


bool HayamiSUFunction::checkConsistency()
{

  return Function::checkConsistency();
}


// =====================================================================
// =====================================================================


bool HayamiSUFunction::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{

  int ID;
  int CurrentStep;
  int TimeStep;
  float QOutput;
  float QInput;
  float TmpValue;

  
  mhydasdk::core::SurfaceUnit* SU;

  TimeStep = SimStatus->getTimeStep();
  CurrentStep = SimStatus->getCurrentStep();
  
  DECLARE_SU_ORDERED_LOOP;
  BEGIN_SU_ORDERED_LOOP(SU)

    ID = SU->getID();
    MHYDAS_GetDistributedVarValue(SU,wxT("runoff"),CurrentStep,&TmpValue);
    //QInput = GET_SIMVAR_VALUE(SU,"runoff",CurrentStep) * SU->getUsrArea() / TimeStep;
    QInput = TmpValue * SU->getUsrArea() / TimeStep;
    m_CurrentInputSum[ID] = m_CurrentInputSum[ID] + QInput;
    m_Input[ID]->push_back(QInput);
    
    QOutput = 0;
    if (m_CurrentInputSum[ID] > 0)
    {    
      QOutput = DoHayamiPropagation(m_SUKernel[ID], CurrentStep, m_Input[ID], m_MaxSteps, TimeStep);
    }  
        
    //APPEND_SIMVAR_VALUE(SU,"qoutput",QOutput);
    MHYDAS_AppendDistributedVarValue(SU,wxT("qoutput"),QOutput);

  END_LOOP
  

  return true;
  
}


// =====================================================================
// =====================================================================


bool HayamiSUFunction::finalizeRun(mhydasdk::base::SimulationInfo* SimInfo)
{

  return true;
}


