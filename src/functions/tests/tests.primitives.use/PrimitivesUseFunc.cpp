/**
  \file PrimitivesUseFunc.cpp
  \brief Implements ...
*/


#include "PrimitivesUseFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(PrimitivesUseFunction);


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(wxT("tests.primitives.use"));
  DECLARE_SIGNATURE_NAME(wxT("test function for primitives (use)"));
  DECLARE_SIGNATURE_DESCRIPTION(wxT(""));

  DECLARE_SIGNATURE_VERSION(wxT("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN(wxT(""));
  DECLARE_SIGNATURE_PROCESS(wxT(""));
  DECLARE_SIGNATURE_METHOD(wxT(""));
  DECLARE_SIGNATURE_AUTHORNAME(wxT(""));
  DECLARE_SIGNATURE_AUTHOREMAIL(wxT(""));
END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


PrimitivesUseFunction::PrimitivesUseFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


PrimitivesUseFunction::~PrimitivesUseFunction()
{


}


// =====================================================================
// =====================================================================


bool PrimitivesUseFunction::initParams(openfluid::core::ParamsMap Params)
{


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesUseFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesUseFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesUseFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesUseFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesUseFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

