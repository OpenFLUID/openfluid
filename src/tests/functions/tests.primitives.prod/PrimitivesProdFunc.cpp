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
  \file PrimitivesProdFunc.cpp
  \brief Implements ...
*/


#include "PrimitivesProdFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_CLASS(PrimitivesProdFunction)


// =====================================================================
// =====================================================================


BEGIN_FUNCTION_SIGNATURE("tests.primitives.prod")

  DECLARE_SIGNATURE_NAME("test function for primitives (production)");
  DECLARE_SIGNATURE_DESCRIPTION("");

  DECLARE_SIGNATURE_VERSION("1.0");
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN("");
  DECLARE_SIGNATURE_PROCESS("");
  DECLARE_SIGNATURE_METHOD("");
  DECLARE_SIGNATURE_AUTHORNAME("");
  DECLARE_SIGNATURE_AUTHOREMAIL("");

  DECLARE_PRODUCED_VAR("tests.vector[]","TestUnits","vector for tests","");
  DECLARE_PRODUCED_VAR("tests.scalar","TestUnits","scalar for tests","");

  DECLARE_SCHEDULING_DEFAULT();

END_FUNCTION_SIGNATURE


// =====================================================================
// =====================================================================


PrimitivesProdFunction::PrimitivesProdFunction()
                : PluggableFunction()
{
  std::cout << "PrimitivesProdFunction : constructor" << std::endl;
}


// =====================================================================
// =====================================================================


PrimitivesProdFunction::~PrimitivesProdFunction()
{
  std::cout << "PrimitivesProdFunction : destructor" << std::endl;
}


// =====================================================================
// =====================================================================


void PrimitivesProdFunction::initParams(const openfluid::ware::WareParams_t& /*Params*/)
{

}


// =====================================================================
// =====================================================================


void PrimitivesProdFunction::prepareData()
{

}


// =====================================================================
// =====================================================================


void PrimitivesProdFunction::checkConsistency()
{

}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest PrimitivesProdFunction::initializeRun()
{
  long VectorSize = 40;
  openfluid::core::Unit* TU;

  OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
  {
    OPENFLUID_InitializeVariable(TU,"tests.scalar",double(TU->getID())*0);
    OPENFLUID_InitializeVariable(TU,"tests.vector",openfluid::core::VectorValue(VectorSize,double(TU->getID())));
  }

  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest PrimitivesProdFunction::runStep()
{
  openfluid::core::Unit* TU;
  long VectorSize = 40;
  openfluid::core::VectorValue TheVector;


  OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
  {
    // vector value

    if (!OPENFLUID_IsVariableExist(TU,"tests.vector"))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.vector)");
    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.vector",openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, VECTOR)");

    if (OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep) before append");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(),openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, timestep, VECTOR) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep, VECTOR) before append");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(),openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, timestep, VECTOR) before append");

    TheVector = openfluid::core::VectorValue(VectorSize,double(TU->getID()));
    OPENFLUID_AppendVariable(TU,"tests.vector",TheVector);

    if (!OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep) after append");
    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, timestep, VECTOR) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()+1))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep+1) after append");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()+1, openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, timestep+1, VECTOR) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep, VECTOR) after append");
    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, timestep, VECTOR) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()+1, openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.vector, timestep+1, VECTOR) after append");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()+1, openfluid::core::Value::VECTOR))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.vector, timestep+1, VECTOR) after append");


    // scalar value

    if (!OPENFLUID_IsVariableExist(TU,"tests.scalar"))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.scalar)");
    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.scalar",openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.scalar, NONE)");

    if (OPENFLUID_IsVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex()))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep) before append");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex(),openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.scalar, timestep, NONE) before append");

    if (OPENFLUID_IsVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep, DOUBLE) before append");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.scalar, timestep, NONE) before append");

    OPENFLUID_AppendVariable(TU,"tests.scalar",double(TU->getID()));

    if (!OPENFLUID_IsVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex()))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep) after append");
    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex(),openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.scalar, timestep, NONE) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex()+1))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep+1) after append");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex()+1,openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.scalar, timestep+1, NONE) after append");

    if (!OPENFLUID_IsVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep, DOUBLE) after append");
    if (!OPENFLUID_IsTypedVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.scalar, timestep, NONE) after append");

    if (OPENFLUID_IsVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex()+1, openfluid::core::Value::DOUBLE))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsVariableExist (tests.scalar, timestep+1, DOUBLE) after append");
    if (OPENFLUID_IsTypedVariableExist(TU,"tests.scalar",OPENFLUID_GetCurrentTimeIndex()+1, openfluid::core::Value::NONE))
      OPENFLUID_RaiseError("tests.primitives.prod","incorrect OPENFLUID_IsTypedVariableExist (tests.scalar, timestep+1, NONE) after append");


  }


  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


void PrimitivesProdFunction::finalizeRun()
{

}

