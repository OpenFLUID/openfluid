/**
  \file HayamiSUFunc.h
  \brief header of HayamiSUFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "HayamiSUFunc.h"
#include <iostream>
#include "math.h"

#include "HayamiTools.h"

// =====================================================================
// =====================================================================

DEFINE_FUNCTION_HOOK(HayamiSUFunction);



// =====================================================================
// =====================================================================

BEGIN_SIGNATURE_HOOK

  DECLARE_SIGNATURE_ID(wxT("water.surf.transfer-su.hayami"));  
  DECLARE_SIGNATURE_NAME(wxT("water transfer on surface units using hayami propagation method"));
  DECLARE_SIGNATURE_DESCRIPTION(wxT(""));
  DECLARE_SIGNATURE_DOMAIN(wxT("hydrology"));
  
  DECLARE_SIGNATURE_STATUS(mhydasdk::base::BETA);  
  
  DECLARE_SIGNATURE_SDKVERSION;

  DECLARE_SIGNATURE_AUTHORNAME(wxT("Moussa R., Fabre J.-C."));
  DECLARE_SIGNATURE_AUTHOREMAIL(wxT("moussa@supagro.inra.fr, fabrejc@supagro.inra.fr"));  

  DECLARE_SU_PRODUCED_VAR("water.surf.Q.downstream-su",wxT("output volume at the outlet of the SUs"),wxT("m3/s"));  

  DECLARE_SU_REQUIRED_VAR("water.surf.H.runoff",wxT("runoff on the surface of the unit"),wxT("m/s"));  
  
  DECLARE_SU_REQUIRED_PROPERTY("nmanning",wxT("Manning roughness coefficient"),wxT("s/m^(-1/3)"));
 
  
  DECLARE_FUNCTION_PARAM("maxsteps",wxT("maximum hayami kernel steps"),wxT(""));
  DECLARE_FUNCTION_PARAM("meancel",wxT("wave mean celerity"),wxT("m/s"));  
  DECLARE_FUNCTION_PARAM("meansigma",wxT("mean diffusivity on SUs"),wxT("m2/s"));  

END_SIGNATURE_HOOK

// =====================================================================
// =====================================================================



HayamiSUFunction::HayamiSUFunction()
                : PluggableFunction()
{

  
  // default values
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
// =============================================)========================


bool HayamiSUFunction::initParams(mhydasdk::core::ParamsMap Params)
{

  MHYDAS_GetFunctionParam(Params,wxT("maxsteps"),&m_MaxSteps);        
  MHYDAS_GetFunctionParam(Params,wxT("meancel"),&m_MeanCelerity);
  MHYDAS_GetFunctionParam(Params,wxT("meansigma"),&m_MeanSigma);
  
  return true;
}


// =====================================================================
// =====================================================================

bool HayamiSUFunction::prepareData()
{
  
  return true;  
}


// =====================================================================
// =====================================================================



bool HayamiSUFunction::checkConsistency()
{
  
  return true;  
}

// =====================================================================
// =====================================================================


bool HayamiSUFunction::initializeRun(mhydasdk::base::SimulationInfo* SimInfo)
{
  mhydasdk::core::SurfaceUnit* SU;
  float Cel, Sigma;
  mhydasdk::core::HOID ID;
  mhydasdk::core::PropertyValue TmpValue;
  
  DECLARE_SU_ORDERED_LOOP;
 
  
  BEGIN_SU_ORDERED_LOOP(SU)
    ID = SU->getID();
    
    m_Input[ID] = new mhydasdk::core::VectorOfMHYDASValue();
    m_CurrentInputSum[ID] = 0;
  
    m_MeanSlope = m_MeanSlope + SU->getUsrSlope();
    MHYDAS_GetDistributedProperty(SU,wxT("nmanning"),&TmpValue);
    m_MeanManning = m_MeanManning + TmpValue;
  END_LOOP  
  
  m_MeanSlope = m_MeanSlope / mp_CoreData->getSpatialData()->getSUsCollection()->size();
  m_MeanManning = m_MeanManning / mp_CoreData->getSpatialData()->getSUsCollection()->size(); 

  BEGIN_SU_ORDERED_LOOP(SU)
    MHYDAS_GetDistributedProperty(SU,wxT("nmanning"),&TmpValue);
    Cel = m_MeanCelerity * (m_MeanManning / TmpValue) * (sqrt((SU->getUsrSlope() / m_MeanSlope)));
    Sigma = m_MeanSigma * (TmpValue / m_MeanManning) * (m_MeanSlope / SU->getUsrSlope());    
    
    m_SUKernel[SU->getID()] = t_HayamiKernel();         
    
    ComputeHayamiKernel(Cel, Sigma,SU->getDownstreamDistance(),m_MaxSteps,SimInfo->getTimeStep(), &m_SUKernel[SU->getID()]);
    
  END_LOOP


  return true;
}



// =====================================================================
// =====================================================================


bool HayamiSUFunction::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{

  int ID;
  int CurrentStep;
  int TimeStep;
  mhydasdk::core::MHYDASValue QOutput;
  mhydasdk::core::MHYDASValue QInput;
  mhydasdk::core::MHYDASValue TmpValue;

  
  mhydasdk::core::SurfaceUnit* SU;

  TimeStep = SimStatus->getTimeStep();
  CurrentStep = SimStatus->getCurrentStep();
  
  DECLARE_SU_ORDERED_LOOP;
  BEGIN_SU_ORDERED_LOOP(SU)

    ID = SU->getID();
  
    MHYDAS_GetDistributedVarValue(SU,wxT("water.surf.H.runoff"),CurrentStep,&TmpValue);

    QInput = TmpValue * SU->getUsrArea() / TimeStep;
    m_CurrentInputSum[ID] = m_CurrentInputSum[ID] + QInput;
    m_Input[ID]->push_back(QInput);
    
    QOutput = 0;
    if (m_CurrentInputSum[ID] > 0)
    {    
      QOutput = DoHayamiPropagation(m_SUKernel[ID], CurrentStep, m_Input[ID], m_MaxSteps, TimeStep);
    }  
        

    MHYDAS_AppendDistributedVarValue(SU,wxT("water.surf.Q.downstream-su"),QOutput);

  END_LOOP
  
  

  return true;
  
}


// =====================================================================
// =====================================================================


bool HayamiSUFunction::finalizeRun(mhydasdk::base::SimulationInfo* SimInfo)
{

  return true;
}


