/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file PrimitivesUseFunc.cpp
  \brief Implements ...
*/


#include "PrimitivesUseFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_CLASS(PrimitivesUseFunction)


// =====================================================================
// =====================================================================


BEGIN_FUNCTION_SIGNATURE("tests.primitives.use")

  DECLARE_SIGNATURE_NAME("test function for primitives (use)");
  DECLARE_SIGNATURE_DESCRIPTION("");

  DECLARE_SIGNATURE_VERSION("1.0");
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN("");
  DECLARE_SIGNATURE_PROCESS("");
  DECLARE_SIGNATURE_METHOD("");
  DECLARE_SIGNATURE_AUTHORNAME("");
  DECLARE_SIGNATURE_AUTHOREMAIL("");


  DECLARE_USED_EVENTS("TestUnits");

  DECLARE_REQUIRED_VAR("tests.vector[]","TestUnits","vector for tests","");
  DECLARE_REQUIRED_VAR("tests.scalar","TestUnits","scalar for tests","");

  DECLARE_REQUIRED_INPUTDATA("indataA","TestUnits","input data for tests","")
  DECLARE_REQUIRED_INPUTDATA("indataB","TestUnits","input data for tests","")

  DECLARE_FUNCTION_PARAM ("strparam","=strvalue","-");
  DECLARE_FUNCTION_PARAM ("doubleparam","=1.1","-");
  DECLARE_FUNCTION_PARAM ("longparam","=11","-");
  DECLARE_FUNCTION_PARAM ("strarrayparam","=strvalue1;strvalue2;strvalue3","-");
  DECLARE_FUNCTION_PARAM ("doublearrayparam","=1.1;1.3;1.3;1.4","-");
  DECLARE_FUNCTION_PARAM ("longarrayparam","=11;12;13;14;15","-");

END_FUNCTION_SIGNATURE


// =====================================================================
// =====================================================================


PrimitivesUseFunction::PrimitivesUseFunction()
                : PluggableFunction()
{

//std::cout << "PrimitiveUseFunc constructor" << std::endl;
}


// =====================================================================
// =====================================================================


PrimitivesUseFunction::~PrimitivesUseFunction()
{


}


// =====================================================================
// =====================================================================


void PrimitivesUseFunction::initParams(const openfluid::ware::WareParams_t& Params)
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
}

// =====================================================================
// =====================================================================


void PrimitivesUseFunction::prepareData()
{

}


// =====================================================================
// =====================================================================


void PrimitivesUseFunction::checkConsistency()
{

}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest PrimitivesUseFunction::initializeRun()
{
  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest PrimitivesUseFunction::runStep()
{
//  std::cout << "PrimitiveUseFunc runStep begin" << std::endl;
  openfluid::core::Unit* TU;
  unsigned long VectorSize = 40;
  unsigned long NewVectorSize = 5;
  openfluid::core::VectorValue TheVector;
  openfluid::core::DoubleValue TheScalar;
  double TheInputDouble;
  std::string TheInputStr;
  openfluid::core::EventsCollection TheEvents;
  openfluid::core::Event Ev;
  std::string RunEnvStr;
  bool RunEnvBool;
  unsigned int UnitsCount;


  std::cout << std::endl;

//  std::cout << "PrimitiveUseFunc runStep Units..." << std::endl;
  // ===== Units =====

  if (!OPENFLUID_IsUnitClassExist("TestUnits"))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsUnitClassExist (TestUnits)");

  if (!OPENFLUID_IsUnitClassExist("ParentTestUnits"))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsUnitClassExist (ParentTestUnits)");

  if (OPENFLUID_IsUnitClassExist("unknown"))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsUnitClassExist (unknown)");

  if (!OPENFLUID_GetUnitsCount("TestUnits",UnitsCount) || UnitsCount != 12)
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_getUnitsCount or count value (TestUnits)");

  if (!OPENFLUID_GetUnitsCount("ParentTestUnits",UnitsCount)|| UnitsCount != 2)
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_getUnitsCount (ParentTestUnits)");

  if (OPENFLUID_GetUnitsCount("unknown",UnitsCount))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_getUnitsCount (unknown)");

//  std::cout << "PrimitiveUseFunc runStep Environt..." << std::endl;
  // ===== Run environment =====

  RunEnvStr.clear();

  if (!OPENFLUID_GetRunEnvironment("dir.input",&RunEnvStr))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (dir.input)");

  std::cout << "dir.input = " << RunEnvStr << std::endl;

  if (!OPENFLUID_GetRunEnvironment("dir.output",&RunEnvStr))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (dir.output)");

  std::cout << "dir.output = " << RunEnvStr << std::endl;

  if (OPENFLUID_GetRunEnvironment("wrong.str",&RunEnvStr))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (wrong.str)");

  RunEnvBool = false;
  if (OPENFLUID_GetRunEnvironment("mode.verbose",&RunEnvBool))
   OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (mode.verbose)");

  if (RunEnvBool) OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (mode.verbose is true)");

  if (OPENFLUID_GetRunEnvironment("wrong.bool",&RunEnvBool))
    OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (wrong.bool)");


//  std::cout << "*** PrimitiveUseFunc runStep Start Loop" << std::endl;
  OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
  {

//  std::cout << "PrimitiveUseFunc runStep child/parents..." << std::endl;
//  std::cout << "TU->getID() : " << TU->getID() << std::endl;
//  std::cout << "TU->getParentUnits(\"ParentTestUnits\")->size() : " << TU->getParentUnits("ParentTestUnits")->size() << std::endl;
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


//    std::cout << "PrimitiveUseFunc runStep scalar..." << std::endl;
    // ====== Scalar ======

    OPENFLUID_GetVariable(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex(),&TheScalar);
    if ((openfluid::core::UnitID_t)TheScalar != TU->getID())
      OPENFLUID_RaiseError("tests.primitives.use","incorrect scalar value (tests.scalar)");

    OPENFLUID_SetVariable(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex(),(double)(TU->getID()*OPENFLUID_GetCurrentTimeIndex()));

    if (OPENFLUID_GetCurrentTimeIndex() > 0 )
    {
      OPENFLUID_GetVariable(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT(),&TheScalar);
      if (!openfluid::tools::IsCloseEnough(TheScalar,(double)(TU->getID()*(OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT())),0.00001))
        OPENFLUID_RaiseError("tests.primitives.use","incorrect scalar value at t-1 (tests.scalar)");
    }


    OPENFLUID_GetVariable(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex(),&TheScalar);

    if (!openfluid::tools::IsCloseEnough(TheScalar,(double)(TU->getID()*OPENFLUID_GetCurrentTimeIndex()),0.00001))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect scalar value after update (tests.scalar)");


    if (!OPENFLUID_IsVariableExist(TU,"tests.scalar"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.scalar)");
    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.scalar",openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsTypedVariableExist (tests.scalar, NONE)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongscalar"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongscalar)");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.wrongscalar",openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsTypedVariableExist (tests.wrongscalar, NONE)");


    if (!OPENFLUID_IsVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep)");
    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex(),openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsTypedVariableExist (tests.scalar, timestep, NONE)");


    if (OPENFLUID_IsVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex()+1))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep+1)");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(),openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsTypedVariableExist (tests.scalar, timestep+1, NONE)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongscalar",OPENFLUID_GetCurrentTimeIndex()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongscalar, timestep)");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.wrongscalar",OPENFLUID_GetCurrentTimeIndex(),openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongscalar, timestep, NONE)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongscalar",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongscalar, timestep+1)");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.wrongscalar",OPENFLUID_GetCurrentTimeIndex()+1,openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsTypedVariableExist (tests.wrongscalar, timestep+1, NONE)");


    if (!OPENFLUID_IsVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep, DOUBLE)");
    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep, NONE)");


    if (OPENFLUID_IsVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex()+(2000*OPENFLUID_GetDefaultDeltaT()), openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep+2000, DOUBLE)");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex()+(2000*OPENFLUID_GetDefaultDeltaT()), openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsTypedVariableExist (tests.scalar, timestep+2000, NONE)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongscalar",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongscalar, timestep, DOUBLE)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongscalar",OPENFLUID_GetCurrentTimeIndex()+(1920*OPENFLUID_GetDefaultDeltaT()), openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongscalar, timestep+1920, DOUBLE)");


    if (OPENFLUID_IsVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep, VECTOR)");


//    std::cout << "PrimitiveUseFunc runStep Vector..." << std::endl;
    // ====== Vector ======

    OPENFLUID_GetVariable(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(),&TheVector);
    if (TheVector.getSize() != VectorSize)
      OPENFLUID_RaiseError("tests.primitives.use","incorrect vector size");

    openfluid::core::VectorValue NewVect(NewVectorSize,OPENFLUID_GetCurrentTimeIndex());
    OPENFLUID_SetVariable(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(),NewVect);

    OPENFLUID_GetVariable(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(),&TheVector);
    if (TheVector.getSize() != NewVectorSize)
      OPENFLUID_RaiseError("tests.primitives.use","incorrect vector size after update");

    if (!openfluid::tools::IsCloseEnough(TheVector[0],(double)OPENFLUID_GetCurrentTimeIndex(),0.00001))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect vector value at index 0 after update");

    if (!openfluid::tools::IsCloseEnough(TheVector[4],(double)OPENFLUID_GetCurrentTimeIndex(),0.00001))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect vector value at index 4 after update");


    if (OPENFLUID_GetCurrentTimeIndex() > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
    {
      OPENFLUID_GetVariable(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT(),&TheVector);

      if (TheVector.getSize() != NewVectorSize)
        OPENFLUID_RaiseError("tests.primitives.use","incorrect vector size at t-1");

      if (!openfluid::tools::IsCloseEnough(TheVector[0],(double)(OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()),0.00001))
        OPENFLUID_RaiseError("tests.primitives.use","incorrect vector value at index 0 at t-1");

      if (!openfluid::tools::IsCloseEnough(TheVector[4],(double)(OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()),0.00001))
        OPENFLUID_RaiseError("tests.primitives.use","incorrect vector value at index 4 at t-1");

    }



    if (!OPENFLUID_IsVariableExist(TU,"tests.vector"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.vector)");
    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.vector",openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, VECTOR)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongvector"))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongvector)");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.wrongvector",openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsTypedVariableExist (tests.wrongvector, VECTOR)");


    if (!OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep)");
    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(),openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, timestep, VECTOR)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongvector",OPENFLUID_GetCurrentTimeIndex()))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongvector, timestep)");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.wrongvector",OPENFLUID_GetCurrentTimeIndex(),openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsTypedVariableExist (tests.wrongvector, timestep, VECTOR)");


    if (!OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep, VECTOR)");
    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, timestep, VECTOR)");

    if (OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(), openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep+1, VECTOR)");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()+1, openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, timestep+1, VECTOR)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongvector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongvector, timestep, VECTOR)");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.wrongvector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsTypedVariableExist (tests.wrongvector, timestep, VECTOR)");


    if (OPENFLUID_IsVariableExist(TU,"tests.wrongvector",OPENFLUID_GetCurrentTimeIndex()+(21*OPENFLUID_GetDefaultDeltaT()), openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.wrongvector, timestep+21, VECTOR)");

    if (OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep, DOUBLE)");

//    std::cout << "PrimitiveUseFunc runStep InputData..." << std::endl;
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


//    std::cout << "PrimitiveUseFunc runStep Events..." << std::endl;
    // ====== Events ======

    TheEvents.clear();

    OPENFLUID_GetEvents(TU,OPENFLUID_GetCurrentDate(),OPENFLUID_GetCurrentDate() + OPENFLUID_GetDefaultDeltaT(),&TheEvents);

    if (TheEvents.getCount() > 2)
      OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetEvents");


    Ev = openfluid::core::Event(openfluid::core::DateTime(OPENFLUID_GetCurrentDate()+(OPENFLUID_GetDefaultDeltaT()*2)));
    OPENFLUID_AppendEvent(TU,Ev);


  }
//  std::cout << "*** PrimitiveUseFunc runStep End Loop" << std::endl;


  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


void PrimitivesUseFunction::finalizeRun()
{ }

