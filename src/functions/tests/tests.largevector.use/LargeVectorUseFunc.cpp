/**
  \file LargeVectorUseFunc.cpp
  \brief Implements ...
*/


#include "LargeVectorUseFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(LargeVectorUseFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.largevector.use"));
  DECLARE_SIGNATURE_NAME(("test function for large vector use"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

  DECLARE_REQUIRED_VAR("tests.largevector[]","TestUnits","vector for tests","");

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


LargeVectorUseFunction::LargeVectorUseFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


LargeVectorUseFunction::~LargeVectorUseFunction()
{


}


// =====================================================================
// =====================================================================


bool LargeVectorUseFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{


  return true;
}

// =====================================================================
// =====================================================================


bool LargeVectorUseFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool LargeVectorUseFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool LargeVectorUseFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool LargeVectorUseFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  openfluid::core::Unit* TU;
  unsigned long VectorSize = 40000;
  openfluid::core::VectorValue TheVector;

  DECLARE_UNITS_ORDERED_LOOP(1);



  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

    OPENFLUID_GetVariable(TU,"tests.largevector",SimStatus->getCurrentStep(),&TheVector);

    if (TheVector.getSize() != VectorSize)
      throw openfluid::base::OFException("incorrect vector size");

  END_LOOP


  return true;
}

// =====================================================================
// =====================================================================


bool LargeVectorUseFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

