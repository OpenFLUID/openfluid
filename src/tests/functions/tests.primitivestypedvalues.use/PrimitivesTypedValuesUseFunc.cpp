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
  \file PrimitivesTypedValuesUseFunc.cpp
  \brief Implements ...
*/


#include "PrimitivesTypedValuesUseFunc.h"

#include <glibmm/ustring.h>

#include <openfluid/core.hpp>

// =====================================================================
// =====================================================================


DEFINE_FUNCTION_CLASS(PrimitivesTypedValuesUseFunction)


// =====================================================================
// =====================================================================


BEGIN_FUNCTION_SIGNATURE
  DECLARE_SIGNATURE_ID(("tests.primitivestypedvalues.use"));
  DECLARE_SIGNATURE_NAME(("test function for primitives typed values (use)"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));


  DECLARE_USED_EVENTS("TestUnits");


  DECLARE_REQUIRED_VAR("tests.none","TestUnits","untyped for tests","");
  DECLARE_REQUIRED_VAR("tests.double[double]","TestUnits","double for tests","");
  DECLARE_REQUIRED_VAR("tests.integer[integer]","TestUnits","integer for tests","");
  DECLARE_REQUIRED_VAR("tests.bool[boolean]","TestUnits","boolean for tests","");
  DECLARE_REQUIRED_VAR("tests.string[string]","TestUnits","string for tests","");
  DECLARE_REQUIRED_VAR("tests.vector[vector]","TestUnits","vector for tests","");
  DECLARE_REQUIRED_VAR("tests.oldvector[]","TestUnits","old style vector for tests","");
  DECLARE_REQUIRED_VAR("tests.matrix[matrix]","TestUnits","matrix for tests","");
  DECLARE_REQUIRED_VAR("tests.map[map]","TestUnits","map for tests","");


END_FUNCTION_SIGNATURE


// =====================================================================
// =====================================================================


PrimitivesTypedValuesUseFunction::PrimitivesTypedValuesUseFunction()
                : PluggableFunction()
{

}


// =====================================================================
// =====================================================================


PrimitivesTypedValuesUseFunction::~PrimitivesTypedValuesUseFunction()
{


}


// =====================================================================
// =====================================================================


bool PrimitivesTypedValuesUseFunction::initParams(openfluid::core::FuncParamsMap_t /*Params*/)
{
  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesTypedValuesUseFunction::prepareData()
{
  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesTypedValuesUseFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesTypedValuesUseFunction::initializeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
{


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesTypedValuesUseFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  openfluid::core::Unit* TU;
  unsigned int TUID;
  unsigned int CurStep;

  double RefDouble;
  long RefLong;
  bool RefBool;
  std::string RefString;
  unsigned long RefVectorSize = 40;
  unsigned long RefMatrixColsNb = 4;
  unsigned long RefMatrixRowsNb = 3;

  double VarDouble;
  long VarLong;
  bool VarBool;
  std::string VarString;
  openfluid::core::VectorValue VarVectorVal;
  openfluid::core::MatrixValue VarMatrixVal;
  openfluid::core::MapValue VarMapVal;

  double NewDouble;
  long NewLong;
  bool NewBool;
  std::string NewString;
  unsigned long NewVectorSize = 5;
  unsigned long NewMatrixColsNb = RefMatrixColsNb + 1;
  unsigned long NewMatrixRowsNb = RefMatrixRowsNb + 1;


  OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
  {


    TUID = TU->getID();
    CurStep = SimStatus->getCurrentStep();

    RefDouble = (double)TUID/10;
    RefLong = TUID;
    RefBool = (TUID%2 == 0);
    RefString = Glib::ustring::compose("ID %1",TUID);

    NewDouble = TUID*CurStep*0.1;
    NewLong = TUID*CurStep*10;
    NewBool = ((TUID*CurStep)%2 == 0);
    NewString = Glib::ustring::compose("%1 %2x%3","strvalue",TUID,CurStep);



    // double

    OPENFLUID_GetVariable(TU,"tests.double",CurStep,VarDouble);
    if (!openfluid::tools::IsCloseEnough(VarDouble,RefDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect double value (tests.double) get by reference");

    OPENFLUID_SetVariable(TU,"tests.double",CurStep,NewDouble);

    OPENFLUID_GetVariable(TU,"tests.double",CurStep,VarDouble);
    if (!openfluid::tools::IsCloseEnough(VarDouble,NewDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect double value after update (tests.double)");

    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.double",openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsTypedVariableExist (tests.double, DOUBLE)");


    // long

    OPENFLUID_GetVariable(TU,"tests.integer",CurStep,VarLong);
    if (VarLong != RefLong)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect long value (tests.integer) get by reference");

    OPENFLUID_SetVariable(TU,"tests.integer",CurStep,NewLong);

    OPENFLUID_GetVariable(TU,"tests.integer",CurStep,VarLong);
    if (VarLong != NewLong)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect long value after update (tests.integer)");

    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.integer",openfluid::core::Value::INTEGER))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsTypedVariableExist (tests.integer, INTEGER)");


    // bool

    OPENFLUID_GetVariable(TU,"tests.bool",CurStep,VarBool);
    if (VarBool != RefBool)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect bool value (tests.bool) get by reference");

    OPENFLUID_SetVariable(TU,"tests.bool",CurStep,NewBool);

    OPENFLUID_GetVariable(TU,"tests.bool",CurStep,VarBool);
    if (VarBool != NewBool)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect bool value after update (tests.bool)");

    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.bool",openfluid::core::Value::BOOLEAN))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsTypedVariableExist (tests.bool, BOOLEAN)");


    // string

    OPENFLUID_GetVariable(TU,"tests.string",CurStep,VarString);
    if (VarString != RefString)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect string value (tests.string) get by reference");

    OPENFLUID_SetVariable(TU,"tests.string",CurStep,NewString);

    OPENFLUID_GetVariable(TU,"tests.string",CurStep,VarString);
    if (VarString != NewString)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect string value after update (tests.string)");

    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.string",openfluid::core::Value::STRING))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsTypedVariableExist (tests.string, STRING)");


    // vector

    OPENFLUID_GetVariable(TU,"tests.vector",CurStep,VarVectorVal);
    if (VarVectorVal.getSize() != RefVectorSize)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect vector size get by reference");

    openfluid::core::VectorValue NewVect(NewVectorSize,NewDouble);
    OPENFLUID_SetVariable(TU,"tests.vector",CurStep,NewVect);

    OPENFLUID_GetVariable(TU,"tests.vector",CurStep,VarVectorVal);
    if (VarVectorVal.getSize() != NewVectorSize)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect vector size after update");
    if (!openfluid::tools::IsCloseEnough(VarVectorVal[0],NewDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect vector value at index 0 after update");
    if (!openfluid::tools::IsCloseEnough(VarVectorVal[4],NewDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect vector value at index 4 after update");

    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.vector",openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, VECTOR)");


    // old style vector

    VarVectorVal.clear();
    OPENFLUID_GetVariable(TU,"tests.oldvector",CurStep,VarVectorVal);
    if (VarVectorVal.getSize() != RefVectorSize)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect oldvector size get by reference");

    OPENFLUID_SetVariable(TU,"tests.oldvector",CurStep,NewVect);

    OPENFLUID_GetVariable(TU,"tests.oldvector",CurStep,VarVectorVal);
    if (VarVectorVal.getSize() != NewVectorSize)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect oldvector size after update");
    if (!openfluid::tools::IsCloseEnough(VarVectorVal[0],NewDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect oldvector value at index 0 after update");
    if (!openfluid::tools::IsCloseEnough(VarVectorVal[4],NewDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect oldvector value at index 4 after update");

    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.oldvector",openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsTypedVariableExist (tests.oldvector, VECTOR)");


    // matrix

    OPENFLUID_GetVariable(TU,"tests.matrix",CurStep,VarMatrixVal);
    if (VarMatrixVal.getColsNbr() != RefMatrixColsNb)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect matrix cols nb get by reference");
    if (VarMatrixVal.getRowsNbr() != RefMatrixRowsNb)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect matrix rows nb get by reference");

    openfluid::core::MatrixValue NewMatrix(NewMatrixColsNb,NewMatrixRowsNb,NewDouble);
    OPENFLUID_SetVariable(TU,"tests.matrix",CurStep,NewMatrix);

    OPENFLUID_GetVariable(TU,"tests.matrix",CurStep,VarMatrixVal);
    if (VarMatrixVal.getColsNbr() != NewMatrixColsNb)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect matrix cols nb after update");
    if (VarMatrixVal.getRowsNbr() != NewMatrixRowsNb)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect matrix rows nb after update");
    if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(0,0),NewDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect matrix value at index 0,0 after update");
    if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(4,3),NewDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect matrix value at index 4,3 after update");

    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.matrix",openfluid::core::Value::MATRIX))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsTypedVariableExist (tests.matrix, MATRIX)");


    // map

    OPENFLUID_GetVariable(TU,"tests.map",CurStep,VarMapVal);
    if (VarMapVal.getSize() != 2)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect map size get by reference");

    openfluid::core::MapValue NewMap;
    NewMap.setString("key1",NewString);
    NewMap.setDouble("key2",NewDouble);
    NewMap.setBoolean("key3",NewBool);
    OPENFLUID_SetVariable(TU,"tests.map",CurStep,NewMap);

    OPENFLUID_GetVariable(TU,"tests.map",CurStep,VarMapVal);
    if (VarMapVal.getSize() != 3)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect map size after update");
    if (VarMapVal.getString("key1") != NewString)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect map value at key key1 after update");
    if (!openfluid::tools::IsCloseEnough(VarMapVal.getDouble("key2"),NewDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect map value at key key2 after update");
    if (VarMapVal.getBoolean("key3") != NewBool)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect map value at key key3 after update");

    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.map",openfluid::core::Value::MAP))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsTypedVariableExist (tests.map, MAP)");


    // none

    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.none",openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsTypedVariableExist (tests.none, NONE)");


    if (!OPENFLUID_IsVariableExist(TU,"tests.none",CurStep,openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsVariableExist (tests.none, DOUBLE)");

    VarDouble = 0.0;
    OPENFLUID_GetVariable(TU,"tests.none",CurStep,VarDouble);
    if (!openfluid::tools::IsCloseEnough(VarDouble,RefDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect double value (tests.none)");


    OPENFLUID_SetVariable(TU,"tests.none",CurStep,NewLong);

    if (!OPENFLUID_IsVariableExist(TU,"tests.none",CurStep,openfluid::core::Value::INTEGER))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsVariableExist (tests.none, INTEGER)");

    VarLong = 0;
    OPENFLUID_GetVariable(TU,"tests.none",CurStep,VarLong);
    if (VarLong != NewLong)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect long value (tests.none)");


    OPENFLUID_SetVariable(TU,"tests.none",CurStep,openfluid::core::NullValue());

    if (!OPENFLUID_IsVariableExist(TU,"tests.none",CurStep,openfluid::core::Value::NULLL))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsVariableExist (tests.none, NULL)");


    OPENFLUID_SetVariable(TU,"tests.none",CurStep,NewBool);

    if (!OPENFLUID_IsVariableExist(TU,"tests.none",CurStep,openfluid::core::Value::BOOLEAN))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsVariableExist (tests.none, BOOLEAN)");

    VarBool = false;
    OPENFLUID_GetVariable(TU,"tests.none",CurStep,VarBool);
    if (VarBool != NewBool)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect boolean value (tests.none)");


    OPENFLUID_SetVariable(TU,"tests.none",CurStep,NewString);

    if (!OPENFLUID_IsVariableExist(TU,"tests.none",CurStep,openfluid::core::Value::STRING))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsVariableExist (tests.none, STRING)");

    VarString= "";
    OPENFLUID_GetVariable(TU,"tests.none",CurStep,VarString);
    if (VarString != NewString)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect string value (tests.none)");


    OPENFLUID_SetVariable(TU,"tests.none",CurStep,NewVect);

    if (!OPENFLUID_IsVariableExist(TU,"tests.none",CurStep,openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsVariableExist (tests.none, VECTOR)");

    VarVectorVal.clear();
    OPENFLUID_GetVariable(TU,"tests.none",CurStep,VarVectorVal);
    if (VarVectorVal.size() != NewVect.size())
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect vector size value (tests.none)");
    if (!openfluid::tools::IsCloseEnough(VarVectorVal[4],NewDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect vector value at index 4 (tests.none)");


    OPENFLUID_SetVariable(TU,"tests.none",CurStep,NewMatrix);

    if (!OPENFLUID_IsVariableExist(TU,"tests.none",CurStep,openfluid::core::Value::MATRIX))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsVariableExist (tests.none, MATRIX)");

    VarMatrixVal.clear();
    OPENFLUID_GetVariable(TU,"tests.none",CurStep,VarMatrixVal);
    if (VarMatrixVal.size() != NewMatrix.size())
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect matrix size value (tests.none)");
    if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(4,3),NewDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect matrix value at index 4,3 (tests.none)");


    OPENFLUID_SetVariable(TU,"tests.none",CurStep,NewMap);

    if (!OPENFLUID_IsVariableExist(TU,"tests.none",CurStep,openfluid::core::Value::MAP))
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect OPENFLUID_IsVariableExist (tests.none, MAP)");

    VarMapVal.clear();
    OPENFLUID_GetVariable(TU,"tests.none",CurStep,VarMapVal);
    if (VarMapVal.size() != NewMap.size())
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect map size value (tests.none)");
    if (VarMapVal.getBoolean("key3") != NewBool)
      OPENFLUID_RaiseError("tests.primitivestypedvalues.use","incorrect map value at key key3 (tests.none)");


  }


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesTypedValuesUseFunction::finalizeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
{
  return true;
}

