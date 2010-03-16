/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file PrimitivesUseFunc.cpp
  \brief Implements ...
*/


#include "PrimitivesUseFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(PrimitivesUseFunction)


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


  DECLARE_USED_EVENTS("TestUnits");

  DECLARE_REQUIRED_VAR("tests.vector[]","TestUnits","vector for tests","");
  DECLARE_REQUIRED_VAR("tests.scalar","TestUnits","scalar for tests","");

  DECLARE_REQUIRED_INPUTDATA("indataA","TestUnits","input data for tests","")
  DECLARE_REQUIRED_INPUTDATA("indataB","TestUnits","input data for tests","")

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

  long LongParam;
  double DoubleParam;
  std::string StrParam;

  std::vector<long> LongArrayParam;
  std::vector<double> DoubleArrayParam;
  std::vector<std::string> StrArrayParam;


  // ====== String param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"strparam",&StrParam))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (strparam)");

  if (OPENFLUID_GetFunctionParameter(Params,"wrongstrparam",&StrParam))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (wrongstrparam)");



  // ====== Double param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"doubleparam",&DoubleParam))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (doubleparam)");

  if (OPENFLUID_GetFunctionParameter(Params,"wrongdoubleparam",&DoubleParam))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (wrongdoubleparam)");



  // ====== Long param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"longparam",&LongParam))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (longparam)");

  if (OPENFLUID_GetFunctionParameter(Params,"wronglongparam",&LongParam))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (wronglongparam)");



  // ====== String array param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"strarrayparam",&StrArrayParam))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam)");

  if (StrArrayParam.size() != 3)
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam, size)");

  if (StrArrayParam[1] != "strvalue2")
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam, value)");

  if (OPENFLUID_GetFunctionParameter(Params,"wrongstrarrayparam",&StrArrayParam))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (wrongstrarrayparam)");



  // ====== Double array param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"doublearrayparam",&DoubleArrayParam))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam)");

  if (DoubleArrayParam.size() != 4)
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, size)");

  if (!openfluid::tools::IsCloseEnough(DoubleArrayParam[2],1.3,0.00001))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, value)");

  if (openfluid::tools::IsCloseEnough(DoubleArrayParam[3],1.3,0.00001))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, wrongvalue)");

  if (OPENFLUID_GetFunctionParameter(Params,"wrongdoublearrayparam",&DoubleArrayParam))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (wrongdoublearrayparam)");



  // ====== Long array param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"longarrayparam",&LongArrayParam))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam)");

  if (LongArrayParam.size() != 5)
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, size)");

  if (LongArrayParam[3] != 14)
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, value)");

  if (LongArrayParam[4] == 14)
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, wrongvalue)");


  if (OPENFLUID_GetFunctionParameter(Params,"wronglongarrayparam",&LongArrayParam))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetFunctionParameter (wronglongarrayparam)");



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
  unsigned long VectorSize = 40;
  unsigned long NewVectorSize = 5;
  openfluid::core::VectorValue TheVector;
  openfluid::core::ScalarValue TheScalar;
  double TheInputDouble;
  std::string TheInputStr;
  openfluid::core::EventsCollection TheEvents;
  openfluid::core::Event Ev;
  DECLARE_UNITS_ORDERED_LOOP(1);
  std::string RunEnvStr;
  bool RunEnvBool;
  unsigned int UnitsCount;



  // ===== Units =====

  if (!OPENFLUID_IsUnitClassExist("TestUnits"))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsUnitClassExist (TestUnits)");

  if (!OPENFLUID_IsUnitClassExist("ParentTestUnits"))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsUnitClassExist (ParentTestUnits)");

  if (OPENFLUID_IsUnitClassExist("unknown"))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsUnitClassExist (unknown)");

  if (!OPENFLUID_GetUnitsCount("TestUnits",&UnitsCount) || UnitsCount != 12)
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_getUnitsCount or count value (TestUnits)");

  if (!OPENFLUID_GetUnitsCount("ParentTestUnits",&UnitsCount)|| UnitsCount != 2)
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_getUnitsCount (ParentTestUnits)");

  if (OPENFLUID_GetUnitsCount("unknown",&UnitsCount))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_getUnitsCount (unknown)");


  // ===== Run environment =====

  if (!OPENFLUID_GetRunEnvironment("dir.input",&RunEnvStr))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (dir.input)");

  if (OPENFLUID_GetRunEnvironment("wrong.str",&RunEnvStr))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (wrong.str)");

  RunEnvBool = true;
  if (!OPENFLUID_GetRunEnvironment("mode.verbose",&RunEnvBool))
   OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (mode.verbose)");

  if (RunEnvBool) OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (mode.verbose is true)");

  if (OPENFLUID_GetRunEnvironment("wrong.bool",&RunEnvBool))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (wrong.bool)");



  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)


    // ====== Units child/Parents ======
    if (TU->getID() < 10)
    {
      if (!(TU->getParentUnits("ParentTestUnits")->size() == 1))
        OPENFLUID_RaiseError("tests.primitives.use","incorrect parent count for TestUnits ID < 10");

      if (TU->getID() % 2 == 0)
      {
        if (!(TU->getParentUnits("ParentTestUnits")->front()->getID() == 2))
          OPENFLUID_RaiseError("tests.primitives.use","incorrect parent for even TestUnits ID");
      }
      else
      {
        if (!(TU->getParentUnits("ParentTestUnits")->front()->getID() == 1))
          OPENFLUID_RaiseError("tests.primitives.use","incorrect parent for odd TestUnits ID");
      }
    }



    // ====== Scalar ======

    OPENFLUID_GetVariable(TU,"tests.scalar",SimStatus->getCurrentStep(),&TheScalar);
    if ((openfluid::core::UnitID_t)TheScalar != TU->getID())
      OPENFLUID_RaiseError("tests.primitives.use","incorrect scalar value (tests.scalar)");

    OPENFLUID_SetVariable(TU,"tests.scalar",SimStatus->getCurrentStep(),(double)(TU->getID()*SimStatus->getCurrentStep()));

    if (SimStatus->getCurrentStep() > 0 )
    {
      OPENFLUID_GetVariable(TU,"tests.scalar",SimStatus->getCurrentStep()-1,&TheScalar);
      if (!openfluid::tools::IsCloseEnough(TheScalar,(double)(TU->getID()*(SimStatus->getCurrentStep()-1)),0.00001))
        OPENFLUID_RaiseError("tests.primitives.use","incorrect scalar value at t-1 (tests.scalar)");
    }


    OPENFLUID_GetVariable(TU,"tests.scalar",SimStatus->getCurrentStep(),&TheScalar);

    if (!openfluid::tools::IsCloseEnough(TheScalar,(double)(TU->getID()*SimStatus->getCurrentStep()),0.00001))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect scalar value after update (tests.scalar)");


    if (!OPENFLUID_IsVariableExist(TU,"tests.scalar"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.scalar)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongscalar"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongscalar)");


    if (!OPENFLUID_IsVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep)");


    if (OPENFLUID_IsVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep+1)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongscalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongscalar, timestep)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongscalar",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongscalar, timestep+1)");


    if (!OPENFLUID_IsScalarVariableExist(TU,"tests.scalar"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.scalar)");


    if (OPENFLUID_IsScalarVariableExist(TU,"tests.wrongscalar"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.wrongscalar)");


    if (!OPENFLUID_IsScalarVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.scalar, timestep)");


    if (OPENFLUID_IsScalarVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()+2000))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.scalar, timestep+2000)");


    if (OPENFLUID_IsScalarVariableExist(TU,"tests.wrongscalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.wrongscalar, timestep)");


    if (OPENFLUID_IsScalarVariableExist(TU,"tests.wrongscalar",SimStatus->getCurrentStep()+1920))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.wrongscalar, timestep+1920)");


    if (OPENFLUID_IsVectorVariableExist(TU,"tests.scalar",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVectorVariableExist (tests.scalar, timestep)");

    if (OPENFLUID_IsVectorVariableExist(TU,"tests.scalar"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVectorVariableExist (tests.scalar)");



    // ====== Vector ======

    OPENFLUID_GetVariable(TU,"tests.vector",SimStatus->getCurrentStep(),&TheVector);
    if (TheVector.getSize() != VectorSize)
      OPENFLUID_RaiseError("tests.primitives.use","incorrect vector size");

    OPENFLUID_SetVariable(TU,"tests.vector",SimStatus->getCurrentStep(),openfluid::core::VectorValue(NewVectorSize,SimStatus->getCurrentStep()));

    OPENFLUID_GetVariable(TU,"tests.vector",SimStatus->getCurrentStep(),&TheVector);
    if (TheVector.getSize() != NewVectorSize)
      OPENFLUID_RaiseError("tests.primitives.use","incorrect vector size after update");

    if (!openfluid::tools::IsCloseEnough(TheVector[0],(double)SimStatus->getCurrentStep(),0.00001))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect vector value at index 0 after update");

    if (!openfluid::tools::IsCloseEnough(TheVector[4],(double)SimStatus->getCurrentStep(),0.00001))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect vector value at index 4 after update");


    if (SimStatus->getCurrentStep() > 0)
    {
      OPENFLUID_GetVariable(TU,"tests.vector",SimStatus->getCurrentStep()-1,&TheVector);

      if (TheVector.getSize() != NewVectorSize)
        OPENFLUID_RaiseError("tests.primitives.use","incorrect vector size at t-1");

      if (!openfluid::tools::IsCloseEnough(TheVector[0],(double)(SimStatus->getCurrentStep()-1),0.00001))
        OPENFLUID_RaiseError("tests.primitives.use","incorrect vector value at index 0 at t-1");

      if (!openfluid::tools::IsCloseEnough(TheVector[4],(double)(SimStatus->getCurrentStep()-1),0.00001))
        OPENFLUID_RaiseError("tests.primitives.use","incorrect vector value at index 4 at t-1");

    }



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


    if (OPENFLUID_IsVectorVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVectorVariableExist (tests.vector, timestep+1)");


    if (OPENFLUID_IsVectorVariableExist(TU,"tests.wrongvector",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVectorVariableExist (tests.wrongvector, timestep)");


    if (OPENFLUID_IsVectorVariableExist(TU,"tests.wrongvector",SimStatus->getCurrentStep()+21))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVectorVariableExist (tests.wrongvector, timestep+21)");


    if (OPENFLUID_IsScalarVariableExist(TU,"tests.vector"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.vector)");


    if (OPENFLUID_IsScalarVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsScalarVariableExist (tests.vector, timestep)");


    // ====== Input data ======

    if (!OPENFLUID_IsInputDataExist(TU,"indataA"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsInputDataExist (indataA)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataB"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsInputDataExist (indataB)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataC"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsInputDataExist (indataC)");

    if (OPENFLUID_IsInputDataExist(TU,"wrongdata"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsInputDataExist (wrongdata)");


    TheInputDouble = 0.0;
    OPENFLUID_GetInputData(TU,"indataA",&TheInputDouble);
    if (!openfluid::tools::IsVeryClose(TheInputDouble,1.1))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetInputData (indataA wrongvalue)");


    TheInputStr = "";
    OPENFLUID_GetInputData(TU,"indataB",&TheInputStr);
    std::string RefStr;
    openfluid::tools::ConvertValue(TU->getID(),&RefStr);
    RefStr = "CODE"+RefStr;
    if (TheInputStr != RefStr)
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetInputData (indataB wrongvalue)");



    // ====== Events ======

    TheEvents.clear();

    OPENFLUID_GetEvents(TU,SimStatus->getCurrentTime(),SimStatus->getCurrentTime() + SimStatus->getTimeStep(),&TheEvents);

    if (TheEvents.getCount() > 0)
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetEvents");


    Ev = openfluid::core::Event(openfluid::core::DateTime(SimStatus->getCurrentTime()+(SimStatus->getTimeStep()*2)));
    OPENFLUID_AppendEvent(TU,Ev);


  END_LOOP



  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesUseFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

