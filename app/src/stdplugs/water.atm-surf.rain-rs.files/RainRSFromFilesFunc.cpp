/**
  \file RainRSFromFilesFunc.cpp
  \brief Implements ...
*/


#include "RainRSFromFilesFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(RainRSFromFilesFunction);


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(wxT("water.atm-surf.rain-rs.files"));
  DECLARE_SIGNATURE_NAME(wxT("Rain on RS from files"));
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
  DECLARE_RS_PRODUCED_VAR("water.atm-surf.I.rain",wxT(""),wxT("m/s"));

  // Required extra files
  DECLARE_REQUIRED_EXTRAFILE(wxT("raindistriRS.dat"));
  DECLARE_REQUIRED_EXTRAFILE(wxT("rainsources.xml"));
END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


RainRSFromFilesFunction::RainRSFromFilesFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


RainRSFromFilesFunction::~RainRSFromFilesFunction()
{


}


// =====================================================================
// =====================================================================


bool RainRSFromFilesFunction::initParams(mhydasdk::core::ParamsMap Params)
{


  return true;
}

// =====================================================================
// =====================================================================


bool RainRSFromFilesFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool RainRSFromFilesFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool RainRSFromFilesFunction::initializeRun(const mhydasdk::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool RainRSFromFilesFunction::runStep(const mhydasdk::base::SimulationStatus* SimStatus)
{
  
  mhydasdk::core::ReachSegment* RS;
  DECLARE_RS_ORDERED_LOOP;
  
  BEGIN_RS_ORDERED_LOOP(RS)
  
    MHYDAS_AppendDistributedVarValue(RS,wxT("water.atm-surf.I.rain"),0);
  
  END_LOOP


  return true;
}

// =====================================================================
// =====================================================================


bool RainRSFromFilesFunction::finalizeRun(const mhydasdk::base::SimulationInfo* SimInfo)
{


  return true;
}

