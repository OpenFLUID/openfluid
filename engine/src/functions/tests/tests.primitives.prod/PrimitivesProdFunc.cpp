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
  DECLARE_SIGNATURE_ID(("tests.primitives.prod"));
  DECLARE_SIGNATURE_NAME(("test function for primitives (production)"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

  DECLARE_PRODUCED_VAR("tests.vector[]","TestUnits","vector for tests","");
  DECLARE_PRODUCED_VAR("tests.scalar","TestUnits","scalar for tests","");

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


bool PrimitivesProdFunction::initParams(openfluid::core::FuncParamsMap_t Params)
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
  openfluid::core::Unit* TU;
  long VectorSize = 40;
  openfluid::core::VectorValue TheVector;


  DECLARE_UNITS_ORDERED_LOOP(1);

  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

    TheVector = openfluid::core::VectorValue(VectorSize,double(TU->getID()));
    OPENFLUID_AppendVariable(TU,"tests.vector",TheVector);
    OPENFLUID_AppendVariable(TU,"tests.scalar",double(TU->getID()));

  END_LOOP


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesProdFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

