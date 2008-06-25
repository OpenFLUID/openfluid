/**
  \file RainSUFromFilesFunc.cpp
  \brief Implements ...
*/


#include "RainSUFromFilesFunc.h"
#include <iostream>
#include <math.h>

// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(RainSUFromFilesFunction);


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(wxT("water.atm-surf.rain-su.files"));
  DECLARE_SIGNATURE_NAME(wxT("Rain on SU from files"));
  DECLARE_SIGNATURE_DESCRIPTION(wxT(""));

  DECLARE_SIGNATURE_VERSION(wxT("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(mhydasdk::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN(wxT("hydrology"));
  DECLARE_SIGNATURE_PROCESS(wxT(""));
  DECLARE_SIGNATURE_METHOD(wxT(""));
  DECLARE_SIGNATURE_AUTHORNAME(wxT("Jean-Christophe Fabre"));
  DECLARE_SIGNATURE_AUTHOREMAIL(wxT("fabrejc@supagro.inra.fr"));

  // Produced variables
  DECLARE_SU_PRODUCED_VAR("water.atm-surf.I.rain",wxT("m/s"),wxT("rain intensity on each SU by time step"));

  // Required extra files
  DECLARE_REQUIRED_EXTRAFILE(wxT("SUraindistri.dat"));
  DECLARE_REQUIRED_EXTRAFILE(wxT("rainsources.xml"));
END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


RainSUFromFilesFunction::RainSUFromFilesFunction()
                : PluggableFunction()
{

  m_Threshold = 0.00001;

}


// =====================================================================
// =====================================================================


RainSUFromFilesFunction::~RainSUFromFilesFunction()
{


}


// =====================================================================
// =====================================================================


bool RainSUFromFilesFunction::initParams(mhydasdk::core::ParamsMap Params)
{

  MHYDAS_GetFunctionParam(Params,wxT("threshold"),&m_Threshold);  
  
  return true;
}

// =====================================================================
// =====================================================================


bool RainSUFromFilesFunction::prepareData()
{

  
  return true;
}


// =====================================================================
// =====================================================================


bool RainSUFromFilesFunction::checkConsistency()
{

  
  
  return true;
}


// =====================================================================
// =====================================================================


bool RainSUFromFilesFunction::initializeRun(const mhydasdk::base::SimulationInfo* SimInfo)
{

  wxString InputDir;
  
  MHYDAS_GetEnvironmentInputDir(&InputDir);  

  m_DataPool.setConfig(InputDir, wxT("rainsources.xml"),wxT("SUraindistri.dat"),mhydasdk::tools::SERIEPREPCS_CUMULATE,SimInfo->getStartTime(),SimInfo->getEndTime(),SimInfo->getTimeStep());
  
  if (!m_DataPool.loadAndPrepareData())
  {
    MHYDAS_RaiseError(wxT("water.atm-surf.rain-su.filesbeta"),m_DataPool.getErrorMessage());
    return false;
  }
     
  
  return true;
}

// =====================================================================
// =====================================================================


bool RainSUFromFilesFunction::runStep(const mhydasdk::base::SimulationStatus* SimStatus)
{
  mhydasdk::core::SurfaceUnit* SU;
  mhydasdk::core::MHYDASScalarValue Value,ValueNext,MSValue;
  DECLARE_SU_ORDERED_LOOP;

  BEGIN_SU_ORDERED_LOOP(SU)


    Value = 0;
    ValueNext = 0;
    
    if (m_DataPool.getValue(SU->getID(),SimStatus->getCurrentStep(),&Value) && m_DataPool.getValue(SU->getID(),SimStatus->getCurrentStep()+1,&ValueNext))    
//    if (m_DataPool.getValue(SU->getID(),SimStatus->getCurrentTime()-wxTimeSpan(0,0,SimStatus->getTimeStep(),0),&Value) && m_DataPool.getValue(SU->getID(),SimStatus->getCurrentTime(),&ValueNext))
    {
      MSValue = (ValueNext-Value)/1000;
      
      if (isnan(MSValue) || MSValue < m_Threshold) MSValue = 0;
      
//      std::cerr << "MSValue " << MSValue << std::endl;
      MHYDAS_AppendDistributedVarValue(SU,wxT("water.atm-surf.I.rain"),MSValue);
      
    }
    else
    {
      return false;
    }


    

  END_LOOP

  return true;
}

// =====================================================================
// =====================================================================


bool RainSUFromFilesFunction::finalizeRun(const mhydasdk::base::SimulationInfo* SimInfo)
{


  return true;
}

