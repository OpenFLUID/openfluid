/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file PrimitivesProdFunc.cpp
  \brief Implements ...
*/


#include "PrimitivesProdFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(PrimitivesProdFunction)


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

    // vector value

    if (!OPENFLUID_IsVariableExist(TU,"tests.vector"))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.vector)");

    if (OPENFLUID_IsVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep) before append");

    if (!OPENFLUID_IsVectorVariableExist(TU,"tests.vector"))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVectorVariableExist (tests.vector)");

    if (OPENFLUID_IsVectorVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVectorVariableExist (tests.vector, timestep) before append");

    TheVector = openfluid::core::VectorValue(VectorSize,double(TU->getID()));
    OPENFLUID_AppendVariable(TU,"tests.vector",TheVector);

    if (!OPENFLUID_IsVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep+1) after append");

    if (!OPENFLUID_IsVectorVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVectorVariableExist (tests.scalar, timestep) after append");

    if (OPENFLUID_IsVectorVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVectorVariableExist (tests.scalar, timestep+1) after append");


    // scalar value

    if (!OPENFLUID_IsVariableExist(TU,"tests.scalar"))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.scalar)");

    if (OPENFLUID_IsVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep) before append");

    if (!OPENFLUID_IsScalarVariableExist(TU,"tests.scalar"))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsScalarVariableExist (tests.scalar)");

    if (OPENFLUID_IsScalarVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsScalarVariableExist (tests.scalar, timestep) before append");

    OPENFLUID_AppendVariable(TU,"tests.scalar",double(TU->getID()));

    if (!OPENFLUID_IsVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep+1) after append");

    if (!OPENFLUID_IsScalarVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsScalarVariableExist (tests.scalar, timestep) after append");

    if (OPENFLUID_IsScalarVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsScalarVariableExist (tests.scalar, timestep+1) after append");


  END_LOOP


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesProdFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

