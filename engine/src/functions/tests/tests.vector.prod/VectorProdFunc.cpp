/**
  \file VectorProdFunc.cpp
  \brief Implements ...
*/


#include "VectorProdFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(VectorProdFunction);


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.vector.prod"));
  DECLARE_SIGNATURE_NAME(("test function for vector production"));
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

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


VectorProdFunction::VectorProdFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


VectorProdFunction::~VectorProdFunction()
{


}


// =====================================================================
// =====================================================================


bool VectorProdFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{


  return true;
}

// =====================================================================
// =====================================================================


bool VectorProdFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool VectorProdFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool VectorProdFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool VectorProdFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  openfluid::core::Unit* TU;
  long VectorSize = 40;
  openfluid::core::VectorValue TheVector;

  DECLARE_UNITS_ORDERED_LOOP(1);



  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

    TheVector =openfluid::core::VectorValue(VectorSize,double(TU->getID()));
    OPENFLUID_AppendVariable(TU,"tests.vector",TheVector);


  END_LOOP

  return true;
}

// =====================================================================
// =====================================================================


bool VectorProdFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

