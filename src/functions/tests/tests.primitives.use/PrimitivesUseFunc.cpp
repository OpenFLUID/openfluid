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
  DECLARE_SIGNATURE_ID(("tests.primitives.use"));
  DECLARE_SIGNATURE_NAME(("test function for primitives (use)"));
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


bool PrimitivesUseFunction::initParams(openfluid::core::FuncParamsMap_t Params)
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
  openfluid::core::Unit* TU;
  long VectorSize = 40;
  openfluid::core::VectorValue TheVector;
  openfluid::core::ScalarValue TheScalar;


  DECLARE_UNITS_ORDERED_LOOP(1);

  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

    // TODO complete this


    // ====== Scalar ======

    OPENFLUID_GetVariable(TU,"tests.scalar",SimStatus->getCurrentStep(),&TheScalar);


    if (!OPENFLUID_IsVariableExist(TU,"tests.scalar"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.scalar)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongscalar"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongscalar)");


    if (!OPENFLUID_IsVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongscalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongscalar, timestep)");


    if (!OPENFLUID_IsScalarVariableExist(TU,"tests.scalar"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.scalar)");


    if (OPENFLUID_IsScalarVariableExist(TU,"tests.wrongscalar"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.wrongscalar)");


    if (!OPENFLUID_IsScalarVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.scalar, timestep)");


    if (OPENFLUID_IsScalarVariableExist(TU,"tests.wrongscalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.wrongscalar, timestep)");


    if (OPENFLUID_IsVectorVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVectorVariableExist (tests.scalar, timestep)");

    if (OPENFLUID_IsVectorVariableExist(TU,"tests.scalar"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVectorVariableExist (tests.scalar)");



    // ====== Vector ======

    OPENFLUID_GetVariable(TU,"tests.vector",SimStatus->getCurrentStep(),&TheVector);
    if (TheVector.getSize() != VectorSize)
      OPENFLUID_RaiseError("tests.primitives.use","incorrect vector size");


    if (!OPENFLUID_IsVariableExist(TU,"tests.vector"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.vector)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongvector"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongvector)");


    if (!OPENFLUID_IsVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongvector",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongvector, timestep)");


    if (!OPENFLUID_IsVectorVariableExist(TU,"tests.vector"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVectorVariableExist (tests.vector)");


    if (OPENFLUID_IsVectorVariableExist(TU,"tests.wrongvector"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVectorVariableExist (tests.wrongvector)");


    if (!OPENFLUID_IsVectorVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVectorVariableExist (tests.vector, timestep)");


    if (OPENFLUID_IsVectorVariableExist(TU,"tests.wrongvector",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVectorVariableExist (tests.wrongvector, timestep)");


    if (OPENFLUID_IsScalarVariableExist(TU,"tests.vector"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.vector)");


    if (OPENFLUID_IsScalarVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.vector, timestep)");






  END_LOOP



  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesUseFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

