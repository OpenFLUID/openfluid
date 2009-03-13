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
  DECLARE_SIGNATURE_ID(("water.atm-surf.rain-su.files"));
  DECLARE_SIGNATURE_NAME(("Spatial distribution and time interpolation of rain on SU, from files as data sources"));
  DECLARE_SIGNATURE_DESCRIPTION(("This function interpolates rain data at the given model time step, then distribute it on the SUs of the spatial domain. The rain sources are given through a rainsources.xml file. The distribution of rain sources is given through a SUraindistri.dat file, which is a list of couples [UNIT_ID RAINSOURCE_ID]. A threshold can be set to replace by zero very small values (near null), as a function parameter. The resulting data of the time interpolation and spatial distribution can be used by other simulation functions as a variable called water.atm-surf.H.rain."));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::STABLE);

  DECLARE_SIGNATURE_DOMAIN(("hydrology"));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME(("Jean-Christophe Fabre"));
  DECLARE_SIGNATURE_AUTHOREMAIL(("fabrejc@supagro.inra.fr"));

  // Produced variables
  DECLARE_SU_PRODUCED_VAR("water.atm-surf.H.rain",("rainfall height on each SU by time step"),("m"));

  // Function parameters
  DECLARE_FUNCTION_PARAM("threshold",("rainfall heights lower than or equal to this value are replaced by zero"),("m"));


  // Required extra files
  DECLARE_REQUIRED_EXTRAFILE(("SUraindistri.dat"));
  DECLARE_REQUIRED_EXTRAFILE(("rainsources.xml"));
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


bool RainSUFromFilesFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{

  OPENFLUID_GetFunctionParameter(Params,("threshold"),&m_Threshold);

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


bool RainSUFromFilesFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  std::string InputDir;

  OPENFLUID_GetRunEnvironment(("dir.input"),&InputDir);

  m_DataPool.setConfig(InputDir, ("rainsources.xml"),("SUraindistri.dat"),openfluid::tools::SERIEPREPCS_CUMULATE,SimInfo->getStartTime(),SimInfo->getEndTime(),SimInfo->getTimeStep());

  m_DataPool.loadAndPrepareData();


  return true;
}

// =====================================================================
// =====================================================================


bool RainSUFromFilesFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  openfluid::core::Unit* SU;
  openfluid::core::ScalarValue Value,ValueNext,MSValue;
  DECLARE_SU_ORDERED_LOOP;

  BEGIN_SU_ORDERED_LOOP(SU)

    Value = 0;
    ValueNext = 0;

    if (m_DataPool.getValue(SU->getID(),SimStatus->getCurrentStep(),&Value) && m_DataPool.getValue(SU->getID(),SimStatus->getCurrentStep()+1,&ValueNext))
    {
      MSValue = (ValueNext-Value)/1000;

      if (isnan(MSValue) || MSValue < m_Threshold) MSValue = 0;

      OPENFLUID_AppendVariable(SU,("water.atm-surf.H.rain"),MSValue);
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


bool RainSUFromFilesFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

