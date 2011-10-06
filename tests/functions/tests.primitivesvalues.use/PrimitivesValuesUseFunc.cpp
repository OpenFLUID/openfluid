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
  \file PrimitivesValuesUseFunc.cpp
  \brief Implements ...
*/


#include "PrimitivesValuesUseFunc.h"

#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/NullValue.hpp>

// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(PrimitivesValuesUseFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.primitivesvalues.use"));
  DECLARE_SIGNATURE_NAME(("test function for primitives values (use)"));
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


  DECLARE_REQUIRED_VAR("tests.double","TestUnits","double for tests","");
  DECLARE_REQUIRED_VAR("tests.integer","TestUnits","integer for tests","");
  DECLARE_REQUIRED_VAR("tests.bool","TestUnits","boolean for tests","");
  DECLARE_REQUIRED_VAR("tests.string","TestUnits","string for tests","");
  DECLARE_REQUIRED_VAR("tests.null","TestUnits","null for tests","");
  DECLARE_REQUIRED_VAR("tests.doubleval","TestUnits","double value for tests","");
  DECLARE_REQUIRED_VAR("tests.integerval","TestUnits","integer value for tests","");
  DECLARE_REQUIRED_VAR("tests.boolval","TestUnits","boolean value for tests","");
  DECLARE_REQUIRED_VAR("tests.stringval","TestUnits","string value for tests","");
  DECLARE_REQUIRED_VAR("tests.vector","TestUnits","vector for tests","");

  DECLARE_REQUIRED_INPUTDATA("indataA","TestUnits","input data for tests","")
  DECLARE_REQUIRED_INPUTDATA("indataB","TestUnits","input data for tests","")

  DECLARE_FUNCTION_PARAM ("strparam","=strvalue","-");
  DECLARE_FUNCTION_PARAM ("doubleparam","=0.1","-");
  DECLARE_FUNCTION_PARAM ("longparam","=10","-");
//  DECLARE_FUNCTION_PARAM ("strarrayparam","=strvalue1;strvalue2;strvalue3","-");
//  DECLARE_FUNCTION_PARAM ("doublearrayparam","=1.1;1.3;1.3;1.4","-");
//  DECLARE_FUNCTION_PARAM ("longarrayparam","=11;12;13;14;15","-");

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


PrimitivesValuesUseFunction::PrimitivesValuesUseFunction()
                : PluggableFunction()
{

}


// =====================================================================
// =====================================================================


PrimitivesValuesUseFunction::~PrimitivesValuesUseFunction()
{


}


// =====================================================================
// =====================================================================


bool PrimitivesValuesUseFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{

//  std::vector<long> LongArrayParam;
//  std::vector<double> DoubleArrayParam;
//  std::vector<std::string> StrArrayParam;


  // ====== String param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"strparam",&m_StrParam))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strparam)");

  if (OPENFLUID_GetFunctionParameter(Params,"wrongstrparam",&m_StrParam))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wrongstrparam)");



  // ====== Double param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"doubleparam",&m_DoubleParam))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doubleparam)");

  if (OPENFLUID_GetFunctionParameter(Params,"wrongdoubleparam",&m_DoubleParam))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wrongdoubleparam)");



  // ====== Long param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"longparam",&m_LongParam))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longparam)");

  if (OPENFLUID_GetFunctionParameter(Params,"wronglongparam",&m_LongParam))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wronglongparam)");


//
//  // ====== String array param ======
//
//  if (!OPENFLUID_GetFunctionParameter(Params,"strarrayparam",&StrArrayParam))
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam)");
//
//  if (StrArrayParam.size() != 3)
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam, size)");
//
//  if (StrArrayParam[1] != "strvalue2")
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam, value)");
//
//  if (OPENFLUID_GetFunctionParameter(Params,"wrongstrarrayparam",&StrArrayParam))
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wrongstrarrayparam)");
//
//
//
//  // ====== Double array param ======
//
//  if (!OPENFLUID_GetFunctionParameter(Params,"doublearrayparam",&DoubleArrayParam))
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam)");
//
//  if (DoubleArrayParam.size() != 4)
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, size)");
//
//  if (!openfluid::tools::IsCloseEnough(DoubleArrayParam[2],1.3,0.00001))
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, value)");
//
//  if (openfluid::tools::IsCloseEnough(DoubleArrayParam[3],1.3,0.00001))
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, wrongvalue)");
//
//  if (OPENFLUID_GetFunctionParameter(Params,"wrongdoublearrayparam",&DoubleArrayParam))
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wrongdoublearrayparam)");
//
//
//
//  // ====== Long array param ======
//
//  if (!OPENFLUID_GetFunctionParameter(Params,"longarrayparam",&LongArrayParam))
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam)");
//
//  if (LongArrayParam.size() != 5)
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, size)");
//
//  if (LongArrayParam[3] != 14)
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, value)");
//
//  if (LongArrayParam[4] == 14)
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, wrongvalue)");
//
//
//  if (OPENFLUID_GetFunctionParameter(Params,"wronglongarrayparam",&LongArrayParam))
//    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wronglongarrayparam)");



  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesValuesUseFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesValuesUseFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesValuesUseFunction::initializeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
{


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesValuesUseFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  openfluid::core::Unit* TU;
  unsigned int TUID;
  unsigned int CurStep;

  double DoubleRef;
  long LongRef;
  bool BoolRef;
  std::string StringRef;
  unsigned long VectorSizeRef = 40;

  double TheGetDouble;
  long TheGetLong;
  bool TheGetBool;
  std::string TheGetString;
  openfluid::core::DoubleValue TheGetDoubleVal;
  openfluid::core::IntegerValue TheGetLongVal;
  openfluid::core::BooleanValue TheGetBoolVal;
  openfluid::core::StringValue TheGetStringVal;
  openfluid::core::VectorValue TheGetVectorVal;

  double TheDouble;
  long TheLong;
  bool TheBool;
  std::string TheString;
  openfluid::core::NullValue TheNull;
  long VectorSize = 5;
  openfluid::core::VectorValue TheVector;

  double TheDoublePre;
  long TheLongPre;
  bool TheBoolPre;
  std::string TheStringPre;

  double TheInputDouble;
  std::string TheInputStr;
  openfluid::core::EventsCollection TheEvents;
  openfluid::core::Event Ev;

  DECLARE_UNITS_ORDERED_LOOP(1);


  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)


  // ====== Variables ======

  TUID = TU->getID();
  CurStep = SimStatus->getCurrentStep();

  DoubleRef = TUID/10;
  LongRef = TUID;
  BoolRef = (TUID%2 == 0);
  StringRef = Glib::ustring::compose("ID %1",TUID);

  TheDouble = TUID*CurStep*m_DoubleParam;
  TheLong = TUID*CurStep*m_LongParam;
  TheBool = ((TUID*CurStep)%2 == 0);
  TheString = Glib::ustring::compose("%1 %2x%3",m_StrParam,TUID,CurStep);

  TheDoublePre = TUID*(CurStep-1)*m_DoubleParam;
  TheLongPre = TUID*(CurStep-1)*m_LongParam;
  TheBoolPre = ((TUID*(CurStep-1))%2 == 0);
  TheStringPre = Glib::ustring::compose("%1 %2x%3",m_StrParam,TUID,(CurStep-1));

  // double

  OPENFLUID_GetVariable(TU,"tests.double",CurStep,&TheGetDouble);
  if (!openfluid::tools::IsCloseEnough(TheGetDouble,DoubleRef,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value (tests.double)");

  OPENFLUID_SetVariable(TU,"tests.double",CurStep,TheDouble);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.double",CurStep-1,&TheGetDouble);
    if (!openfluid::tools::IsCloseEnough(TheGetDouble,TheDoublePre,0.00001))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value at t-1 (tests.double)");
  }

  OPENFLUID_GetVariable(TU,"tests.double",CurStep,&TheGetDouble);

  if (!openfluid::tools::IsCloseEnough(TheGetDouble,TheDouble,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value after update (tests.double)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.double"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.double)");

  // double value

  OPENFLUID_GetVariable(TU,"tests.doubleval",CurStep,&TheGetDoubleVal);
  if (TheGetDoubleVal.get() != DoubleRef)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value (tests.doubleval)");

  OPENFLUID_SetVariable(TU,"tests.doubleval",CurStep,TheDouble);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.doubleval",CurStep-1,&TheGetDoubleVal);
    if (!openfluid::tools::IsCloseEnough(TheGetDoubleVal.get(),TheDoublePre,0.00001))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value at t-1 (tests.doubleval)");
  }

  OPENFLUID_GetVariable(TU,"tests.doubleval",CurStep,&TheGetDoubleVal);

  if (!openfluid::tools::IsCloseEnough(TheGetDoubleVal.get(),TheDouble,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value after update (tests.doubleval)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.doubleval"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.doubleval)");

  // long

  OPENFLUID_GetVariable(TU,"tests.integer",CurStep,&TheGetLong);
  if (TheGetLong != LongRef)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value (tests.integer)");

  OPENFLUID_SetVariable(TU,"tests.integer",CurStep,TheLong);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.integer",CurStep-1,&TheGetLong);
    if (TheGetLong != TheLongPre)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value at t-1 (tests.integer)");
  }

  OPENFLUID_GetVariable(TU,"tests.integer",CurStep,&TheGetLong);

  if (TheGetLong != TheLong)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value after update (tests.integer)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.integer"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.integer)");

  // long value

  OPENFLUID_GetVariable(TU,"tests.integerval",CurStep,&TheGetLongVal);
  if (TheGetLongVal.get() != LongRef)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value (tests.integerval)");

  OPENFLUID_SetVariable(TU,"tests.integerval",CurStep,TheLong);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.integerval",CurStep-1,&TheGetLongVal);
    if (TheGetLongVal.get() != TheLongPre)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value at t-1 (tests.integerval)");
  }

  OPENFLUID_GetVariable(TU,"tests.integerval",CurStep,&TheGetLongVal);

  if (TheGetLongVal.get() != TheLong)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value after update (tests.integerval)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.integerval"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.integerval)");

  // bool

  OPENFLUID_GetVariable(TU,"tests.bool",CurStep,&TheGetBool);
  if (TheGetBool != BoolRef)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value (tests.bool)");

  OPENFLUID_SetVariable(TU,"tests.bool",CurStep,TheBool);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.bool",CurStep-1,&TheGetBool);
    if (TheGetBool != TheBoolPre)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value at t-1 (tests.bool)");
  }

  OPENFLUID_GetVariable(TU,"tests.bool",CurStep,&TheGetBool);

  if (TheGetBool != TheBool)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value after update (tests.bool)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.bool"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.bool)");

  // bool value

  OPENFLUID_GetVariable(TU,"tests.boolval",CurStep,&TheGetBoolVal);
  if (TheGetBoolVal.get() != BoolRef)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value (tests.boolval)");

  OPENFLUID_SetVariable(TU,"tests.boolval",CurStep,TheBool);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.boolval",CurStep-1,&TheGetBoolVal);
    if (TheGetBoolVal.get() != TheBoolPre)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value at t-1 (tests.boolval)");
  }

  OPENFLUID_GetVariable(TU,"tests.boolval",CurStep,&TheGetBoolVal);

  if (TheGetBoolVal.get() != TheBool)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value after update (tests.boolval)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.boolval"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.boolval)");

  // string

  OPENFLUID_GetVariable(TU,"tests.string",CurStep,&TheGetString);
  if (TheGetString != StringRef)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect string value (tests.string)");

  OPENFLUID_SetVariable(TU,"tests.string",CurStep,TheString);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.string",CurStep-1,&TheGetString);
    if (TheGetString != TheStringPre)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect string value at t-1 (tests.string)");
  }

  OPENFLUID_GetVariable(TU,"tests.string",CurStep,&TheGetString);

  if (TheGetString != TheString)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect string value after update (tests.string)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.string"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.string)");

  // null value

  OPENFLUID_GetVariable(TU,"tests.null",CurStep,&TheNull);
  if (TheNull.getType() != openfluid::core::Value::NULLL)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect null value (tests.null)");

  // vector value

  OPENFLUID_GetVariable(TU,"tests.vector",CurStep,&TheGetVectorVal);
  if (TheGetVectorVal.getSize() != VectorSizeRef)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector size");

  openfluid::core::VectorValue NewVect(VectorSize,TheDouble);
  OPENFLUID_SetVariable(TU,"tests.vector",CurStep,NewVect);

  OPENFLUID_GetVariable(TU,"tests.vector",CurStep,&TheGetVectorVal);
  if (TheGetVectorVal.getSize() != VectorSize)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector size after update");

  if (!openfluid::tools::IsCloseEnough(TheGetVectorVal[0],TheDouble,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector value at index 0 after update");

  if (!openfluid::tools::IsCloseEnough(TheGetVectorVal[4],TheDouble,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector value at index 4 after update");


  if (CurStep > 0)
  {
    OPENFLUID_GetVariable(TU,"tests.vector",CurStep-1,&TheGetVectorVal);

    if (TheGetVectorVal.getSize() != VectorSize)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector size at t-1");

    if (!openfluid::tools::IsCloseEnough(TheGetVectorVal[0],TheDoublePre,0.00001))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector value at index 0 at t-1");

    if (!openfluid::tools::IsCloseEnough(TheGetVectorVal[4],TheDoublePre,0.00001))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector value at index 4 at t-1");

  }

  if (!OPENFLUID_IsVariableExist(TU,"tests.vector"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.vector)");



  // ====== Input data ======

  if (!OPENFLUID_IsInputDataExist(TU,"indataA"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataA)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataB"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataB)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataC"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataC)");

  if (OPENFLUID_IsInputDataExist(TU,"wrongdata"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (wrongdata)");


  TheInputDouble = 0.0;
  OPENFLUID_GetInputData(TU,"indataA",&TheInputDouble);
  if (!openfluid::tools::IsVeryClose(TheInputDouble,1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA wrongvalue)");


  TheInputStr = "";
  OPENFLUID_GetInputData(TU,"indataB",&TheInputStr);
  std::string RefStr;
  openfluid::tools::ConvertValue(TU->getID(),&RefStr);
  RefStr = "CODE"+RefStr;
  if (TheInputStr != RefStr)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataB wrongvalue)");



  // ====== Events ======

  TheEvents.clear();

  OPENFLUID_GetEvents(TU,SimStatus->getCurrentTime(),SimStatus->getCurrentTime() + SimStatus->getTimeStep(),&TheEvents);

  if (TheEvents.getCount() > 2)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetEvents");


  Ev = openfluid::core::Event(openfluid::core::DateTime(SimStatus->getCurrentTime()+(SimStatus->getTimeStep()*2)));
  OPENFLUID_AppendEvent(TU,Ev);


  END_LOOP


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesValuesUseFunction::finalizeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
{
  return true;
}

