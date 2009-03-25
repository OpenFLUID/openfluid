/**
  \file PrimitivesProdFunc.cpp
  \brief Implements ...
*/


#include "PrimitivesProdFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(PrimitivesProdFunction);


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(wxT("tests.primitives.prod"));
  DECLARE_SIGNATURE_NAME(wxT("test function for primitives (production)"));
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


PrimitivesProdFunction::PrimitivesProdFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


PrimitivesProdFunction::~PrimitivesProdFunction()
{


}


// =====================================================================
// =====================================================================


bool PrimitivesProdFunction::initParams(openfluid::core::ParamsMap Params)
{


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesProdFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesProdFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesProdFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesProdFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesProdFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

