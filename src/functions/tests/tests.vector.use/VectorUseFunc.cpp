/**
  \file VectorUseFunc.cpp
  \brief Implements ...
*/


#include "VectorUseFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(VectorUseFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.vector.use"));
  DECLARE_SIGNATURE_NAME(("test function for vector use"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

  DECLARE_REQUIRED_VAR("tests.vector[]","TestUnits","vector for tests","");

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


VectorUseFunction::VectorUseFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


VectorUseFunction::~VectorUseFunction()
{


}


// =====================================================================
// =====================================================================


bool VectorUseFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{


  return true;
}

// =====================================================================
// =====================================================================


bool VectorUseFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool VectorUseFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool VectorUseFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool VectorUseFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

  openfluid::core::Unit* TU;
  unsigned long VectorSize = 40;
  openfluid::core::VectorValue TheVector;

  DECLARE_UNITS_ORDERED_LOOP(1);



  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

    OPENFLUID_GetVariable(TU,"tests.vector",SimStatus->getCurrentStep(),&TheVector);

    if (TheVector.getSize() != VectorSize)
      throw openfluid::base::OFException("incorrect vector size");

  END_LOOP

  return true;
}

// =====================================================================
// =====================================================================


bool VectorUseFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

