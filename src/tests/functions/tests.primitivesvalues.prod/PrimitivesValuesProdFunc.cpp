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


#include "PrimitivesValuesProdFunc.h"

#include <glibmm/ustring.h>

#include <openfluid/core.hpp>


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(PrimitivesValuesProdFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.primitivesvalues.prod"));
  DECLARE_SIGNATURE_NAME(("test function for primitives values (production)"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

  DECLARE_PRODUCED_VAR("tests.double","TestUnits","double for tests","");
  DECLARE_PRODUCED_VAR("tests.integer","TestUnits","integer for tests","");
  DECLARE_PRODUCED_VAR("tests.bool","TestUnits","boolean for tests","");
  DECLARE_PRODUCED_VAR("tests.string","TestUnits","string for tests","");
  DECLARE_PRODUCED_VAR("tests.doubleval","TestUnits","double value for tests","");
  DECLARE_PRODUCED_VAR("tests.integerval","TestUnits","integer value for tests","");
  DECLARE_PRODUCED_VAR("tests.boolval","TestUnits","boolean value for tests","");
  DECLARE_PRODUCED_VAR("tests.stringval","TestUnits","string value for tests","");
  DECLARE_PRODUCED_VAR("tests.null","TestUnits","null for tests","");
  DECLARE_PRODUCED_VAR("tests.vector","TestUnits","vector for tests","");
  DECLARE_PRODUCED_VAR("tests.matrix","TestUnits","matrix for tests","");
  DECLARE_PRODUCED_VAR("tests.map","TestUnits","map for tests","");

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


PrimitivesValuesProdFunction::PrimitivesValuesProdFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


PrimitivesValuesProdFunction::~PrimitivesValuesProdFunction()
{


}


// =====================================================================
// =====================================================================


bool PrimitivesValuesProdFunction::initParams(openfluid::core::FuncParamsMap_t /*Params*/)
{


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesValuesProdFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesValuesProdFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesValuesProdFunction::initializeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
{


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesValuesProdFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  openfluid::core::Unit* TU;
  unsigned int TUID;

  double TheDouble;
  long TheLong;
  bool TheBool;
  std::string TheString;
  openfluid::core::NullValue TheNull;
  long VectorSize = 40;
  openfluid::core::VectorValue TheVector;
  long MatrixColsNb = 4;
  long MatrixRowsNb = 3;
  openfluid::core::MatrixValue TheMatrix;
  openfluid::core::MapValue TheMap;


  OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
  {

    TUID = TU->getID();

    TheDouble = (double)TUID/10;
    TheLong = TUID;
    TheBool = (TUID%2 == 0);
    TheString = Glib::ustring::compose("ID %1",TUID);

    // double

    if (!OPENFLUID_IsVariableExist(TU,"tests.double"))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.double)");

    if (OPENFLUID_IsVariableExist(TU,"tests.double",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.double, timestep) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.double",SimStatus->getCurrentStep(), openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.double, timestep, DOUBLE) before append");

    OPENFLUID_AppendVariable(TU,"tests.double",TheDouble);

    if (!OPENFLUID_IsVariableExist(TU,"tests.double",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.double, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.double",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.double, timestep+1) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.double",SimStatus->getCurrentStep(), openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.double, timestep, DOUBLE) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.double",SimStatus->getCurrentStep()+1, openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.double, timestep+1, DOUBLE) after append");

    // integer

    if (!OPENFLUID_IsVariableExist(TU,"tests.integer"))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integer)");

    if (OPENFLUID_IsVariableExist(TU,"tests.integer",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integer, timestep) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.integer",SimStatus->getCurrentStep(), openfluid::core::Value::INTEGER))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integer, timestep, INTEGER) before append");

    OPENFLUID_AppendVariable(TU,"tests.integer",TheLong);

    if (!OPENFLUID_IsVariableExist(TU,"tests.integer",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integer, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.integer",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integer, timestep+1) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.integer",SimStatus->getCurrentStep(), openfluid::core::Value::INTEGER))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integer, timestep, INTEGER) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.integer",SimStatus->getCurrentStep()+1, openfluid::core::Value::INTEGER))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integer, timestep+1, INTEGER) after append");

    // boolean

    if (!OPENFLUID_IsVariableExist(TU,"tests.bool"))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.bool)");

    if (OPENFLUID_IsVariableExist(TU,"tests.bool",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.bool, timestep) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.bool",SimStatus->getCurrentStep(), openfluid::core::Value::BOOLEAN))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.bool, timestep, BOOLEAN) before append");

    OPENFLUID_AppendVariable(TU,"tests.bool",TheBool);

    if (!OPENFLUID_IsVariableExist(TU,"tests.bool",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.bool, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.bool",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.bool, timestep+1) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.bool",SimStatus->getCurrentStep(), openfluid::core::Value::BOOLEAN))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.bool, timestep, BOOLEAN) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.bool",SimStatus->getCurrentStep()+1, openfluid::core::Value::BOOLEAN))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.bool, timestep+1, BOOLEAN) after append");

    // string

    if (!OPENFLUID_IsVariableExist(TU,"tests.string"))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.string)");

    if (OPENFLUID_IsVariableExist(TU,"tests.string",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.string, timestep) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.string",SimStatus->getCurrentStep(), openfluid::core::Value::STRING))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.string, timestep, STRING) before append");

    OPENFLUID_AppendVariable(TU,"tests.string",TheString);

    if (!OPENFLUID_IsVariableExist(TU,"tests.string",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.string, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.string",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.string, timestep+1) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.string",SimStatus->getCurrentStep(), openfluid::core::Value::STRING))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.string, timestep, STRING) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.string",SimStatus->getCurrentStep()+1, openfluid::core::Value::STRING))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.string, timestep+1, STRING) after append");

    // double value

    if (!OPENFLUID_IsVariableExist(TU,"tests.doubleval"))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.doubleval)");

    if (OPENFLUID_IsVariableExist(TU,"tests.doubleval",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.doubleval, timestep) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.doubleval",SimStatus->getCurrentStep(), openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.doubleval, timestep, DOUBLE) before append");

    OPENFLUID_AppendVariable(TU,"tests.doubleval",openfluid::core::DoubleValue(TheDouble));

    if (!OPENFLUID_IsVariableExist(TU,"tests.doubleval",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.doubleval, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.doubleval",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.doubleval, timestep+1) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.doubleval",SimStatus->getCurrentStep(), openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.doubleval, timestep, DOUBLE) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.doubleval",SimStatus->getCurrentStep()+1, openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.doubleval, timestep+1, DOUBLE) after append");

    // integer value

    if (!OPENFLUID_IsVariableExist(TU,"tests.integerval"))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integerval)");

    if (OPENFLUID_IsVariableExist(TU,"tests.integerval",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integerval, timestep) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.integerval",SimStatus->getCurrentStep(), openfluid::core::Value::INTEGER))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integerval, timestep, INTEGER) before append");

    OPENFLUID_AppendVariable(TU,"tests.integerval",openfluid::core::IntegerValue(TheLong));

    if (!OPENFLUID_IsVariableExist(TU,"tests.integerval",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integerval, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.integerval",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integerval, timestep+1) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.integerval",SimStatus->getCurrentStep(), openfluid::core::Value::INTEGER))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integerval, timestep, INTEGER) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.integerval",SimStatus->getCurrentStep()+1, openfluid::core::Value::INTEGER))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.integerval, timestep+1, INTEGER) after append");

    // boolean value

    if (!OPENFLUID_IsVariableExist(TU,"tests.boolval"))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.boolval)");

    if (OPENFLUID_IsVariableExist(TU,"tests.boolval",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.boolval, timestep) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.boolval",SimStatus->getCurrentStep(), openfluid::core::Value::BOOLEAN))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.boolval, timestep, BOOLEAN) before append");

    OPENFLUID_AppendVariable(TU,"tests.boolval",openfluid::core::BooleanValue(TheBool));

    if (!OPENFLUID_IsVariableExist(TU,"tests.boolval",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.boolval, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.boolval",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.boolval, timestep+1) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.boolval",SimStatus->getCurrentStep(), openfluid::core::Value::BOOLEAN))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.boolval, timestep, BOOLEAN) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.boolval",SimStatus->getCurrentStep()+1, openfluid::core::Value::BOOLEAN))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.boolval, timestep+1, BOOLEAN) after append");

    // string value

    if (!OPENFLUID_IsVariableExist(TU,"tests.stringval"))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.stringval)");

    if (OPENFLUID_IsVariableExist(TU,"tests.stringval",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.stringval, timestep) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.stringval",SimStatus->getCurrentStep(), openfluid::core::Value::STRING))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.stringval, timestep, STRING) before append");

    OPENFLUID_AppendVariable(TU,"tests.stringval",openfluid::core::StringValue(TheString));

    if (!OPENFLUID_IsVariableExist(TU,"tests.stringval",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.stringval, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.stringval",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.stringval, timestep+1) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.stringval",SimStatus->getCurrentStep(), openfluid::core::Value::STRING))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.stringval, timestep, STRING) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.stringval",SimStatus->getCurrentStep()+1, openfluid::core::Value::STRING))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.stringval, timestep+1, STRING) after append");

    // null value

    if (!OPENFLUID_IsVariableExist(TU,"tests.null"))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.null)");

    if (OPENFLUID_IsVariableExist(TU,"tests.null",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.null, timestep) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.null",SimStatus->getCurrentStep(), openfluid::core::Value::NULLL))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.null, timestep, NULLL) before append");

    OPENFLUID_AppendVariable(TU,"tests.null",TheNull);

    if (!OPENFLUID_IsVariableExist(TU,"tests.null",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.null, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.null",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.null, timestep+1) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.null",SimStatus->getCurrentStep(), openfluid::core::Value::NULLL))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.null, timestep, NULLL) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.null",SimStatus->getCurrentStep()+1, openfluid::core::Value::NULLL))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.null, timestep+1, NULLL) after append");

    // vector value

    if (!OPENFLUID_IsVariableExist(TU,"tests.vector"))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.vector)");

    if (OPENFLUID_IsVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.vector",SimStatus->getCurrentStep(), openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep, VECTOR) before append");

    TheVector = openfluid::core::VectorValue(VectorSize,TheDouble);
    OPENFLUID_AppendVariable(TU,"tests.vector",TheVector);

    if (!OPENFLUID_IsVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep+1) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.vector",SimStatus->getCurrentStep(), openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep, VECTOR) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.vector",SimStatus->getCurrentStep()+1, openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep+1, VECTOR) after append");

    // matrix value

    if (!OPENFLUID_IsVariableExist(TU,"tests.matrix"))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.matrix)");

    if (OPENFLUID_IsVariableExist(TU,"tests.matrix",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.matrix, timestep) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.matrix",SimStatus->getCurrentStep(), openfluid::core::Value::MATRIX))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.matrix, timestep, MATRIX) before append");

    TheMatrix = openfluid::core::MatrixValue(MatrixColsNb,MatrixRowsNb,TheDouble);
    OPENFLUID_AppendVariable(TU,"tests.matrix",TheMatrix);

    if (!OPENFLUID_IsVariableExist(TU,"tests.matrix",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.matrix, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.matrix",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.matrix, timestep+1) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.matrix",SimStatus->getCurrentStep(), openfluid::core::Value::MATRIX))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.matrix, timestep, MATRIX) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.matrix",SimStatus->getCurrentStep()+1, openfluid::core::Value::MATRIX))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.matrix, timestep+1, MATRIX) after append");

    // map value

    if (!OPENFLUID_IsVariableExist(TU,"tests.map"))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.map)");

    if (OPENFLUID_IsVariableExist(TU,"tests.map",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.map, timestep) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.map",SimStatus->getCurrentStep(), openfluid::core::Value::MAP))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.map, timestep, MAP) before append");

    TheMap = openfluid::core::MapValue();
    TheMap.setString("key1",TheString);
    TheMap.setDouble("key2",TheDouble);
    OPENFLUID_AppendVariable(TU,"tests.map",TheMap);

    if (!OPENFLUID_IsVariableExist(TU,"tests.map",SimStatus->getCurrentStep()))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.map, timestep) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.map",SimStatus->getCurrentStep()+1))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.map, timestep+1) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.map",SimStatus->getCurrentStep(), openfluid::core::Value::MAP))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.map, timestep, MAP) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.map",SimStatus->getCurrentStep()+1, openfluid::core::Value::MAP))
      OPENFLUID_RaiseError("tests.primitivesvalues.prod","incorrect OPENFLUID_IsVariableExist (tests.map, timestep+1, MAP) after append");


  }


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesValuesProdFunction::finalizeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
{
  return true;
}

