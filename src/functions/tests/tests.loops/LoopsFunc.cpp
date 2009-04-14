/**
  \file LoopsFunc.cpp
  \brief Implements ...
*/


#include "LoopsFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(LoopsFunction);


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.loops"));
  DECLARE_SIGNATURE_NAME(("test function for units loops"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));
END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


LoopsFunction::LoopsFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


LoopsFunction::~LoopsFunction()
{


}


// =====================================================================
// =====================================================================


bool LoopsFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{


  return true;
}

// =====================================================================
// =====================================================================


bool LoopsFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool LoopsFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool LoopsFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool LoopsFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  openfluid::core::Unit *TU;
//  DECLARE_UNITS_ORDERED_LOOP(25)
  DECLARE_UNITS_ORDERED_LOOP(1)
  DECLARE_UNITS_ORDERED_LOOP(2)
  unsigned int CountInside;


  // TODO complete tests

  // ===== loop inside loop =====

  CountInside = 0;
  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

    BEGIN_UNITS_ORDERED_LOOP(2,"TestUnits",TU)
      CountInside++;
    END_LOOP

  END_LOOP

  if (CountInside != 144)
    OPENFLUID_RaiseError("tests.loops","runStep()","wrong units for while loop inside loop");


  // ===== unit class =====

  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)
    if (TU->getClass() != "TestUnits")
      OPENFLUID_RaiseError("tests.loops","runStep()","wrong units class");
  END_LOOP



  return true;
}

// =====================================================================
// =====================================================================


bool LoopsFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

