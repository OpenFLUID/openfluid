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
  \file PrimitivesValuesProdFunc.cpp
  \brief Implements ...
*/

#include <glibmm/ustring.h>

#include "PrimitivesTypedValuesProdFunc.h"

#include <openfluid/core.hpp>

// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(PrimitivesTypedValuesProdFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.primitivestypedvalues.prod"));
  DECLARE_SIGNATURE_NAME(("test function for primitives typed values (production)"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

  DECLARE_PRODUCED_VAR("tests.none","TestUnits","untyped for tests","");
  DECLARE_PRODUCED_VAR("tests.double[double]","TestUnits","double for tests","");
  DECLARE_PRODUCED_VAR("tests.integer[integer]","TestUnits","integer for tests","");
  DECLARE_PRODUCED_VAR("tests.bool[boolean]","TestUnits","boolean for tests","");
  DECLARE_PRODUCED_VAR("tests.string[string]","TestUnits","string for tests","");
  DECLARE_PRODUCED_VAR("tests.vector[vector]","TestUnits","vector for tests","");
  DECLARE_PRODUCED_VAR("tests.oldvector[]","TestUnits","old style vector for tests","");
  DECLARE_PRODUCED_VAR("tests.matrix[matrix]","TestUnits","matrix for tests","");
  DECLARE_PRODUCED_VAR("tests.map[map]","TestUnits","map for tests","");

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


PrimitivesTypedValuesProdFunction::PrimitivesTypedValuesProdFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


PrimitivesTypedValuesProdFunction::~PrimitivesTypedValuesProdFunction()
{


}


// =====================================================================
// =====================================================================


bool PrimitivesTypedValuesProdFunction::initParams(openfluid::core::FuncParamsMap_t /*Params*/)
{


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesTypedValuesProdFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesTypedValuesProdFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesTypedValuesProdFunction::initializeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
{


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesTypedValuesProdFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  openfluid::core::Unit* TU;
  unsigned int TUID;

  double TheDouble;
  long TheLong;
  bool TheBool;
  std::string TheString;
  long VectorSize = 40;
  openfluid::core::VectorValue TheVector;
  long MatrixColsNb = 4;
  long MatrixRowsNb = 3;
  openfluid::core::MatrixValue TheMatrix;
  openfluid::core::MapValue TheMap;

  DECLARE_UNITS_ORDERED_LOOP(1);


  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)


  TUID = TU->getID();

  TheDouble = (double)TUID/10;
  TheLong = TUID;
  TheBool = (TUID%2 == 0);
  TheString = Glib::ustring::compose("ID %1",TUID);

  // none

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.none", openfluid::core::Value::NONE))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.none, NONE)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.none", openfluid::core::Value::DOUBLE))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.none, DOUBLE)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.none",SimStatus->getCurrentStep(), openfluid::core::Value::NONE))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.none, timestep, NONE) before append");

  OPENFLUID_AppendVariable(TU,"tests.none",TheDouble);

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.none",SimStatus->getCurrentStep(), openfluid::core::Value::NONE))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.none, timestep, NONE) after append");

  // double

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.double", openfluid::core::Value::DOUBLE))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.double, DOUBLE)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.double", openfluid::core::Value::NONE))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.double, NONE)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.double",SimStatus->getCurrentStep(), openfluid::core::Value::DOUBLE))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.double, timestep, DOUBLE) before append");

  OPENFLUID_AppendVariable(TU,"tests.double",TheDouble);

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.double",SimStatus->getCurrentStep(), openfluid::core::Value::DOUBLE))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.double, timestep, DOUBLE) after append");

  // integer

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.integer", openfluid::core::Value::INTEGER))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.integer, INTEGER)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.integer", openfluid::core::Value::NONE))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.integer, NONE)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.integer",SimStatus->getCurrentStep(), openfluid::core::Value::INTEGER))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.integer, timestep, INTEGER) before append");

  OPENFLUID_AppendVariable(TU,"tests.integer",TheLong);

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.integer",SimStatus->getCurrentStep(), openfluid::core::Value::INTEGER))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.integer, timestep, INTEGER) after append");

  // boolean

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.bool", openfluid::core::Value::BOOLEAN))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.bool, BOOLEAN)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.bool", openfluid::core::Value::NONE))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.bool, NONE)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.bool",SimStatus->getCurrentStep(), openfluid::core::Value::BOOLEAN))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.bool, timestep, BOOLEAN) before append");

  OPENFLUID_AppendVariable(TU,"tests.bool",TheBool);

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.bool",SimStatus->getCurrentStep(), openfluid::core::Value::BOOLEAN))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.bool, timestep, BOOLEAN) after append");

  // string

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.string", openfluid::core::Value::STRING))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.string, STRING)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.string", openfluid::core::Value::NONE))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.string, NONE)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.string",SimStatus->getCurrentStep(), openfluid::core::Value::STRING))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.string, timestep, STRING) before append");

  OPENFLUID_AppendVariable(TU,"tests.string",TheString);

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.string",SimStatus->getCurrentStep(), openfluid::core::Value::STRING))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.string, timestep, STRING) after append");

  // vector

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.vector", openfluid::core::Value::VECTOR))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, VECTOR)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.vector", openfluid::core::Value::NONE))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, NONE)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.vector",SimStatus->getCurrentStep(), openfluid::core::Value::VECTOR))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, timestep, VECTOR) before append");

  TheVector = openfluid::core::VectorValue(VectorSize,TheDouble);
  OPENFLUID_AppendVariable(TU,"tests.vector",TheVector);

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.vector",SimStatus->getCurrentStep(), openfluid::core::Value::VECTOR))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, timestep, VECTOR) after append");

  // old style vector

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.oldvector", openfluid::core::Value::VECTOR))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.oldvector, VECTOR)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.oldvector", openfluid::core::Value::NONE))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.oldvector, NONE)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.oldvector",SimStatus->getCurrentStep(), openfluid::core::Value::VECTOR))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.oldvector, timestep, VECTOR) before append");

  TheVector = openfluid::core::VectorValue(VectorSize,TheDouble);
  OPENFLUID_AppendVariable(TU,"tests.oldvector",TheVector);

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.oldvector",SimStatus->getCurrentStep(), openfluid::core::Value::VECTOR))
    OPENFLUID_RaiseError("tests.primitivestypedvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.oldvector, timestep, VECTOR) after append");

  // matrix value

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.matrix", openfluid::core::Value::MATRIX))
    OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.matrix, MATRIX)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.matrix", openfluid::core::Value::NONE))
    OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.matrix, NONE)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.matrix",SimStatus->getCurrentStep(), openfluid::core::Value::MATRIX))
    OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.matrix, timestep, MATRIX) before append");

  TheMatrix = openfluid::core::MatrixValue(MatrixColsNb,MatrixRowsNb,TheDouble);
  OPENFLUID_AppendVariable(TU,"tests.matrix",TheMatrix);

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.matrix",SimStatus->getCurrentStep(), openfluid::core::Value::MATRIX))
    OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.matrix, timestep, MATRIX) after append");

  // map value

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.map", openfluid::core::Value::MAP))
    OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.map, MAP)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.map", openfluid::core::Value::NONE))
    OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.map, NONE)");

  if (OPENFLUID_IsTypedVariableExist(TU,"tests.map",SimStatus->getCurrentStep(), openfluid::core::Value::MAP))
    OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.map, timestep, MAP) before append");

  TheMap = openfluid::core::MapValue();
  TheMap.setString("key1",TheString);
  TheMap.setDouble("key2",TheDouble);
  OPENFLUID_AppendVariable(TU,"tests.map",TheMap);

  if (!OPENFLUID_IsTypedVariableExist(TU,"tests.map",SimStatus->getCurrentStep(), openfluid::core::Value::MAP))
    OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.map, timestep, MAP) after append");



  END_LOOP


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesTypedValuesProdFunction::finalizeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
{
  return true;
}

