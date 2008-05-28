/**
  \file RainSUFromFilesFunc.cpp
  \brief Implements ...
*/


#include "RainSUFromFilesFunc.h"


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
  DECLARE_SU_PRODUCED_VAR("water.atm-surf.I.rain",wxT("m/s"),wxT("rain on each SU"));

  // Required extra files
  DECLARE_REQUIRED_EXTRAFILE(wxT("raindistriSU.dat"));
  DECLARE_REQUIRED_EXTRAFILE(wxT("rainsources.xml"));
END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


RainSUFromFilesFunction::RainSUFromFilesFunction()
                : PluggableFunction()
{


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


  return true;
}

// =====================================================================
// =====================================================================


bool RainSUFromFilesFunction::runStep(const mhydasdk::base::SimulationStatus* SimStatus)
{
  mhydasdk::core::SurfaceUnit* SU;
  DECLARE_SU_ORDERED_LOOP;
  
  BEGIN_SU_ORDERED_LOOP(SU)
  
    MHYDAS_AppendDistributedVarValue(SU,wxT("water.atm-surf.I.rain"),0);
  
  END_LOOP
  

  
  
  return true;
}

// =====================================================================
// =====================================================================


bool RainSUFromFilesFunction::finalizeRun(const mhydasdk::base::SimulationInfo* SimInfo)
{


  return true;
}

