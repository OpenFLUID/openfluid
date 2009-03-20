/**
  \file LargeVectorProdFunc.cpp
  \brief Implements ...
*/


#include "LargeVectorProdFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(LargeVectorProdFunction);


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.largevector.prod"));
  DECLARE_SIGNATURE_NAME(("test function for large vector production"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

  DECLARE_PRODUCED_VAR("tests.largevector[]","TestUnits","vector for tests","");


END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


LargeVectorProdFunction::LargeVectorProdFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


LargeVectorProdFunction::~LargeVectorProdFunction()
{


}


// =====================================================================
// =====================================================================


bool LargeVectorProdFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{


  return true;
}

// =====================================================================
// =====================================================================


bool LargeVectorProdFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool LargeVectorProdFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool LargeVectorProdFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool LargeVectorProdFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  openfluid::core::Unit* TU;
  long VectorSize = 40000;
  openfluid::core::VectorValue TheVector;

  DECLARE_UNITS_ORDERED_LOOP(1);



  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

    TheVector =openfluid::core::VectorValue(VectorSize,double(TU->getID()));
    OPENFLUID_AppendVariable(TU,"tests.largevector",TheVector);


  END_LOOP


  return true;
}

// =====================================================================
// =====================================================================


bool LargeVectorProdFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

