/**
  \file RainRSFromFilesFunc.cpp
  \brief Implements ...
*/


#include "RainRSFromFilesFunc.h"
#include <math.h>

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
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN(wxT("hydrology"));
  DECLARE_SIGNATURE_PROCESS(wxT(""));
  DECLARE_SIGNATURE_METHOD(wxT(""));
  DECLARE_SIGNATURE_AUTHORNAME(wxT("Jean-Christophe Fabre"));
  DECLARE_SIGNATURE_AUTHOREMAIL(wxT("fabrejc@supagro.inra.fr"));

  // Produced variables
  DECLARE_RS_PRODUCED_VAR("water.atm-surf.H.rain",wxT("rainfall height on each RS by time step"),wxT("m"));

  // Required extra files
  DECLARE_REQUIRED_EXTRAFILE(wxT("RSraindistri.dat"));
  DECLARE_REQUIRED_EXTRAFILE(wxT("rainsources.xml"));
END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


RainRSFromFilesFunction::RainRSFromFilesFunction()
                : PluggableFunction()
{
  m_Threshold = 0.00001;


}


// =====================================================================
// =====================================================================


RainRSFromFilesFunction::~RainRSFromFilesFunction()
{


}


// =====================================================================
// =====================================================================


bool RainRSFromFilesFunction::initParams(openfluid::core::ParamsMap Params)
{

  OPENFLUID_GetFunctionParameter(Params,wxT("threshold"),&m_Threshold);

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


bool RainRSFromFilesFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  wxString InputDir;

  OPENFLUID_GetRunEnvironment(wxT("dir.input"),&InputDir);


  m_DataPool.setConfig(InputDir, wxT("rainsources.xml"),wxT("RSraindistri.dat"),openfluid::tools::SERIEPREPCS_CUMULATE,SimInfo->getStartTime(),SimInfo->getEndTime(),SimInfo->getTimeStep());

  m_DataPool.loadAndPrepareData();

  return true;
}

// =====================================================================
// =====================================================================


bool RainRSFromFilesFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

  openfluid::core::ReachSegment* RS;
  openfluid::core::ScalarValue Value,ValueNext,MSValue;

  DECLARE_RS_ORDERED_LOOP;

  BEGIN_RS_ORDERED_LOOP(RS)

    Value = 0;
    ValueNext = 0;

    if (m_DataPool.getValue(RS->getID(),SimStatus->getCurrentStep(),&Value) && m_DataPool.getValue(RS->getID(),SimStatus->getCurrentStep()+1,&ValueNext))
    {
      MSValue = (ValueNext-Value)/1000;

      if (isnan(MSValue) || MSValue < m_Threshold) MSValue = 0;

      OPENFLUID_AppendVariable(RS,wxT("water.atm-surf.H.rain"),MSValue);
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


bool RainRSFromFilesFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

