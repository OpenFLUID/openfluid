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


#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/core.hpp>

#include <QString>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN


// =====================================================================
// =====================================================================


#define THIS_SIM_ID "tests.primitives.variables.prod"

BEGIN_SIMULATOR_SIGNATURE(THIS_SIM_ID)

  DECLARE_NAME("test simulator for variables primitives");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");



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

  DECLARE_PRODUCED_VAR("tests.typed.none","TestUnits","untyped for tests","");
  DECLARE_PRODUCED_VAR("tests.typed.double[double]","TestUnits","double for tests","");
  DECLARE_PRODUCED_VAR("tests.typed.integer[integer]","TestUnits","integer for tests","");
  DECLARE_PRODUCED_VAR("tests.typed.bool[boolean]","TestUnits","boolean for tests","");
  DECLARE_PRODUCED_VAR("tests.typed.string[string]","TestUnits","string for tests","");
  DECLARE_PRODUCED_VAR("tests.typed.vector[vector]","TestUnits","vector for tests","");
  DECLARE_PRODUCED_VAR("tests.typed.oldvector[]","TestUnits","old style vector for tests","");
  DECLARE_PRODUCED_VAR("tests.typed.matrix[matrix]","TestUnits","matrix for tests","");
  DECLARE_PRODUCED_VAR("tests.typed.map[map]","TestUnits","map for tests","");

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class VarsPrimitivesProdSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    unsigned long int m_ProductionCounter;

  public:


    VarsPrimitivesProdSimulator() : PluggableSimulator(), m_ProductionCounter(0)
    {


    }


    // =====================================================================
    // =====================================================================


    ~VarsPrimitivesProdSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {

    }


    // =====================================================================
    // =====================================================================


    void prepareData() { }


    // =====================================================================
    // =====================================================================


    void checkConsistency() { }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      // untyped
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
          TheString = QString("ID %1").arg(TU->getID()).toStdString();

          OPENFLUID_InitializeVariable(TU,"tests.double",TheDouble);
          OPENFLUID_InitializeVariable(TU,"tests.integer",TheLong);
          OPENFLUID_InitializeVariable(TU,"tests.bool",TheBool);
          OPENFLUID_InitializeVariable(TU,"tests.string",TheString);

          OPENFLUID_InitializeVariable(TU,"tests.doubleval",openfluid::core::DoubleValue(TheDouble));
          OPENFLUID_InitializeVariable(TU,"tests.integerval",openfluid::core::IntegerValue(TheLong));
          OPENFLUID_InitializeVariable(TU,"tests.boolval",openfluid::core::BooleanValue(TheBool));
          OPENFLUID_InitializeVariable(TU,"tests.stringval",openfluid::core::StringValue(TheString));

          OPENFLUID_InitializeVariable(TU,"tests.null",TheNull);

          TheVector = openfluid::core::VectorValue(VectorSize,TheDouble);
          OPENFLUID_InitializeVariable(TU,"tests.vector",TheVector);

          TheMatrix = openfluid::core::MatrixValue(MatrixColsNb,MatrixRowsNb,TheDouble);
          OPENFLUID_InitializeVariable(TU,"tests.matrix",TheMatrix);

          TheMap = openfluid::core::MapValue();
          TheMap.setString("key1",TheString);
          TheMap.setDouble("key2",TheDouble);
          OPENFLUID_InitializeVariable(TU,"tests.map",TheMap);

        }
      }

      // typed

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


        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {


          TUID = TU->getID();

          TheDouble = (double)TUID/10;
          TheLong = TUID;
          TheBool = (TUID%2 == 0);
          TheString = QString("ID %1").arg(TU->getID()).toStdString();


          OPENFLUID_InitializeVariable(TU,"tests.typed.none",TheDouble);

          OPENFLUID_InitializeVariable(TU,"tests.typed.double",TheDouble);

          OPENFLUID_InitializeVariable(TU,"tests.typed.integer",TheLong);

          OPENFLUID_InitializeVariable(TU,"tests.typed.bool",TheBool);

          OPENFLUID_InitializeVariable(TU,"tests.typed.string",TheString);

          TheVector = openfluid::core::VectorValue(VectorSize,TheDouble);
          OPENFLUID_InitializeVariable(TU,"tests.typed.vector",TheVector);

          TheVector = openfluid::core::VectorValue(VectorSize,TheDouble);
          OPENFLUID_InitializeVariable(TU,"tests.typed.oldvector",TheVector);

          TheMatrix = openfluid::core::MatrixValue(MatrixColsNb,MatrixRowsNb,TheDouble);
          OPENFLUID_InitializeVariable(TU,"tests.typed.matrix",TheMatrix);

          TheMap = openfluid::core::MapValue();
          TheMap.setString("key1",TheString);
          TheMap.setDouble("key2",TheDouble);
          OPENFLUID_InitializeVariable(TU,"tests.typed.map",TheMap);

        }
      }

      m_ProductionCounter++;

      return DefaultDeltaT();

    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
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
        openfluid::core::IndexedValue IndValue;
        openfluid::core::IndexedValueList IndValueList;



        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {

          TUID = TU->getID();

          TheDouble = (double)TUID/10;
          TheLong = TUID;
          TheBool = (TUID%2 == 0);
          TheString = QString("ID %1").arg(TUID).toStdString();

          // double

          if (!OPENFLUID_IsVariableExist(TU,"tests.double"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.double)");

          if (OPENFLUID_IsVariableExist(TU,"tests.double",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.double, timestep) before append");

          if (OPENFLUID_IsVariableExist(TU,"tests.double",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.double, timestep, DOUBLE) before append");

          OPENFLUID_GetLatestVariable(TU,"tests.double",IndValue);
          if (IndValue.getIndex() != OPENFLUID_GetPreviousRunTimeIndex())
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_GetLatestVariable time index (tests.double, DOUBLE) before append");

          if (!IndValue.getValue()->isDoubleValue())
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_GetLatestVariable value (tests.double, DOUBLE) before append");

          OPENFLUID_GetLatestVariables(TU,"tests.double",0,IndValueList);
          if (IndValueList.size() != m_ProductionCounter)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_GetLatestVariables list size (tests.double, DOUBLE, index=0) before append");

          OPENFLUID_GetLatestVariables(TU,"tests.double",OPENFLUID_GetPreviousRunTimeIndex()+1,IndValueList);
          if (IndValueList.size() != 0)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_GetLatestVariables list size (tests.double, DOUBLE, index=previous+1) before append");

          OPENFLUID_GetVariables(TU,"tests.double",OPENFLUID_GetPreviousRunTimeIndex(),OPENFLUID_GetPreviousRunTimeIndex()+1,IndValueList);
          if (IndValueList.size() != 1)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_GetVariables list size (tests.double, DOUBLE, bindex=previous, eindex=previous+1) before append");

          OPENFLUID_GetVariables(TU,"tests.double",0,OPENFLUID_GetCurrentTimeIndex(),IndValueList);
          if (IndValueList.size() != m_ProductionCounter)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_GetVariables list size (tests.double, DOUBLE, bindex=0, eindex=current) before append");


          OPENFLUID_AppendVariable(TU,"tests.double",TheDouble);

          if (!OPENFLUID_IsVariableExist(TU,"tests.double",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.double, timestep) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.double",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.double, timestep+1) after append");

          if (!OPENFLUID_IsVariableExist(TU,"tests.double",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.double, timestep, DOUBLE) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.double",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(), openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.double, timestep+1, DOUBLE) after append");

          // integer

          if (!OPENFLUID_IsVariableExist(TU,"tests.integer"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integer)");

          if (OPENFLUID_IsVariableExist(TU,"tests.integer",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integer, timestep) before append");

          if (OPENFLUID_IsVariableExist(TU,"tests.integer",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::INTEGER))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integer, timestep, INTEGER) before append");

          OPENFLUID_AppendVariable(TU,"tests.integer",TheLong);

          if (!OPENFLUID_IsVariableExist(TU,"tests.integer",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integer, timestep) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.integer",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integer, timestep+1) after append");

          if (!OPENFLUID_IsVariableExist(TU,"tests.integer",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::INTEGER))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integer, timestep, INTEGER) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.integer",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(), openfluid::core::Value::INTEGER))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integer, timestep+1, INTEGER) after append");

          // boolean

          if (!OPENFLUID_IsVariableExist(TU,"tests.bool"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.bool)");

          if (OPENFLUID_IsVariableExist(TU,"tests.bool",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.bool, timestep) before append");

          if (OPENFLUID_IsVariableExist(TU,"tests.bool",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::BOOLEAN))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.bool, timestep, BOOLEAN) before append");

          OPENFLUID_AppendVariable(TU,"tests.bool",TheBool);

          if (!OPENFLUID_IsVariableExist(TU,"tests.bool",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.bool, timestep) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.bool",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.bool, timestep+1) after append");

          if (!OPENFLUID_IsVariableExist(TU,"tests.bool",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::BOOLEAN))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.bool, timestep, BOOLEAN) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.bool",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(), openfluid::core::Value::BOOLEAN))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.bool, timestep+1, BOOLEAN) after append");

          // string

          if (!OPENFLUID_IsVariableExist(TU,"tests.string"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.string)");

          if (OPENFLUID_IsVariableExist(TU,"tests.string",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.string, timestep) before append");

          if (OPENFLUID_IsVariableExist(TU,"tests.string",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::STRING))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.string, timestep, STRING) before append");

          OPENFLUID_AppendVariable(TU,"tests.string",TheString);

          if (!OPENFLUID_IsVariableExist(TU,"tests.string",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.string, timestep) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.string",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.string, timestep+1) after append");

          if (!OPENFLUID_IsVariableExist(TU,"tests.string",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::STRING))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.string, timestep, STRING) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.string",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(), openfluid::core::Value::STRING))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.string, timestep+1, STRING) after append");

          // double value

          if (!OPENFLUID_IsVariableExist(TU,"tests.doubleval"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.doubleval)");

          if (OPENFLUID_IsVariableExist(TU,"tests.doubleval",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.doubleval, timestep) before append");

          if (OPENFLUID_IsVariableExist(TU,"tests.doubleval",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.doubleval, timestep, DOUBLE) before append");

          OPENFLUID_AppendVariable(TU,"tests.doubleval",openfluid::core::DoubleValue(TheDouble));

          if (!OPENFLUID_IsVariableExist(TU,"tests.doubleval",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.doubleval, timestep) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.doubleval",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.doubleval, timestep+1) after append");

          if (!OPENFLUID_IsVariableExist(TU,"tests.doubleval",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.doubleval, timestep, DOUBLE) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.doubleval",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(), openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.doubleval, timestep+1, DOUBLE) after append");

          // integer value

          if (!OPENFLUID_IsVariableExist(TU,"tests.integerval"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integerval)");

          if (OPENFLUID_IsVariableExist(TU,"tests.integerval",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integerval, timestep) before append");

          if (OPENFLUID_IsVariableExist(TU,"tests.integerval",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::INTEGER))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integerval, timestep, INTEGER) before append");

          OPENFLUID_AppendVariable(TU,"tests.integerval",openfluid::core::IntegerValue(TheLong));

          if (!OPENFLUID_IsVariableExist(TU,"tests.integerval",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integerval, timestep) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.integerval",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integerval, timestep+1) after append");

          if (!OPENFLUID_IsVariableExist(TU,"tests.integerval",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::INTEGER))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integerval, timestep, INTEGER) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.integerval",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(), openfluid::core::Value::INTEGER))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integerval, timestep+1, INTEGER) after append");

          // boolean value

          if (!OPENFLUID_IsVariableExist(TU,"tests.boolval"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.boolval)");

          if (OPENFLUID_IsVariableExist(TU,"tests.boolval",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.boolval, timestep) before append");

          if (OPENFLUID_IsVariableExist(TU,"tests.boolval",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::BOOLEAN))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.boolval, timestep, BOOLEAN) before append");

          OPENFLUID_AppendVariable(TU,"tests.boolval",openfluid::core::BooleanValue(TheBool));

          if (!OPENFLUID_IsVariableExist(TU,"tests.boolval",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.boolval, timestep) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.boolval",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.boolval, timestep+1) after append");

          if (!OPENFLUID_IsVariableExist(TU,"tests.boolval",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::BOOLEAN))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.boolval, timestep, BOOLEAN) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.boolval",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(), openfluid::core::Value::BOOLEAN))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.boolval, timestep+1, BOOLEAN) after append");

          // string value

          if (!OPENFLUID_IsVariableExist(TU,"tests.stringval"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.stringval)");

          if (OPENFLUID_IsVariableExist(TU,"tests.stringval",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.stringval, timestep) before append");

          if (OPENFLUID_IsVariableExist(TU,"tests.stringval",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::STRING))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.stringval, timestep, STRING) before append");

          OPENFLUID_AppendVariable(TU,"tests.stringval",openfluid::core::StringValue(TheString));

          if (!OPENFLUID_IsVariableExist(TU,"tests.stringval",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.stringval, timestep) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.stringval",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.stringval, timestep+1) after append");

          if (!OPENFLUID_IsVariableExist(TU,"tests.stringval",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::STRING))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.stringval, timestep, STRING) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.stringval",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(), openfluid::core::Value::STRING))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.stringval, timestep+1, STRING) after append");

          // null value

          if (!OPENFLUID_IsVariableExist(TU,"tests.null"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.null)");

          if (OPENFLUID_IsVariableExist(TU,"tests.null",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.null, timestep) before append");

          if (OPENFLUID_IsVariableExist(TU,"tests.null",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::NULLL))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.null, timestep, NULLL) before append");

          OPENFLUID_AppendVariable(TU,"tests.null",TheNull);

          if (!OPENFLUID_IsVariableExist(TU,"tests.null",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.null, timestep) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.null",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.null, timestep+1) after append");

          if (!OPENFLUID_IsVariableExist(TU,"tests.null",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::NULLL))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.null, timestep, NULLL) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.null",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(), openfluid::core::Value::NULLL))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.null, timestep+1, NULLL) after append");

          // vector value

          if (!OPENFLUID_IsVariableExist(TU,"tests.vector"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.vector)");

          if (OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.vector, timestep) before append");

          if (OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.vector, timestep, VECTOR) before append");

          TheVector = openfluid::core::VectorValue(VectorSize,TheDouble);
          OPENFLUID_AppendVariable(TU,"tests.vector",TheVector);

          if (!OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.vector, timestep) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.vector, timestep+1) after append");

          if (!OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.vector, timestep, VECTOR) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.vector",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(), openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.vector, timestep+1, VECTOR) after append");

          // matrix value

          if (!OPENFLUID_IsVariableExist(TU,"tests.matrix"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.matrix)");

          if (OPENFLUID_IsVariableExist(TU,"tests.matrix",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.matrix, timestep) before append");

          if (OPENFLUID_IsVariableExist(TU,"tests.matrix",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::MATRIX))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.matrix, timestep, MATRIX) before append");

          TheMatrix = openfluid::core::MatrixValue(MatrixColsNb,MatrixRowsNb,TheDouble);
          OPENFLUID_AppendVariable(TU,"tests.matrix",TheMatrix);

          if (!OPENFLUID_IsVariableExist(TU,"tests.matrix",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.matrix, timestep) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.matrix",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.matrix, timestep+1) after append");

          if (!OPENFLUID_IsVariableExist(TU,"tests.matrix",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::MATRIX))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.matrix, timestep, MATRIX) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.matrix",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(), openfluid::core::Value::MATRIX))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.matrix, timestep+1, MATRIX) after append");

          // map value

          if (!OPENFLUID_IsVariableExist(TU,"tests.map"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.map)");

          if (OPENFLUID_IsVariableExist(TU,"tests.map",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.map, timestep) before append");

          if (OPENFLUID_IsVariableExist(TU,"tests.map",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::MAP))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.map, timestep, MAP) before append");

          TheMap = openfluid::core::MapValue();
          TheMap.setString("key1",TheString);
          TheMap.setDouble("key2",TheDouble);
          OPENFLUID_AppendVariable(TU,"tests.map",TheMap);

          if (!OPENFLUID_IsVariableExist(TU,"tests.map",OPENFLUID_GetCurrentTimeIndex()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.map, timestep) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.map",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT()))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.map, timestep+1) after append");

          if (!OPENFLUID_IsVariableExist(TU,"tests.map",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::MAP))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.map, timestep, MAP) after append");

          if (OPENFLUID_IsVariableExist(TU,"tests.map",OPENFLUID_GetCurrentTimeIndex()+OPENFLUID_GetDefaultDeltaT(), openfluid::core::Value::MAP))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.map, timestep+1, MAP) after append");


        }
      }


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


        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {


          TUID = TU->getID();

          TheDouble = (double)TUID/10;
          TheLong = TUID;
          TheBool = (TUID%2 == 0);
          TheString = QString("ID %1").arg(TUID).toStdString();

          // none

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.none", openfluid::core::Value::NONE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.none, NONE)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.none", openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.none, DOUBLE)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.none",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::NONE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.none, timestep, NONE) before append");

          OPENFLUID_AppendVariable(TU,"tests.typed.none",TheDouble);

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.none",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::NONE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.none, timestep, NONE) after append");

          // double

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.double", openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.double, DOUBLE)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.double", openfluid::core::Value::NONE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.double, NONE)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.double",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.double, timestep, DOUBLE) before append");

          OPENFLUID_AppendVariable(TU,"tests.typed.double",TheDouble);

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.double",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.double, timestep, DOUBLE) after append");

          // integer

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.integer", openfluid::core::Value::INTEGER))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.integer, INTEGER)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.integer", openfluid::core::Value::NONE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.integer, NONE)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.integer",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::INTEGER))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.integer, timestep, INTEGER) before append");

          OPENFLUID_AppendVariable(TU,"tests.typed.integer",TheLong);

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.integer",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::INTEGER))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.integer, timestep, INTEGER) after append");

          // boolean

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.bool", openfluid::core::Value::BOOLEAN))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.bool, BOOLEAN)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.bool", openfluid::core::Value::NONE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.bool, NONE)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.bool",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::BOOLEAN))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.bool, timestep, BOOLEAN) before append");

          OPENFLUID_AppendVariable(TU,"tests.typed.bool",TheBool);

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.bool",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::BOOLEAN))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.bool, timestep, BOOLEAN) after append");

          // string

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.string", openfluid::core::Value::STRING))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.string, STRING)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.string", openfluid::core::Value::NONE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.string, NONE)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.string",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::STRING))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.string, timestep, STRING) before append");

          OPENFLUID_AppendVariable(TU,"tests.typed.string",TheString);

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.string",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::STRING))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.string, timestep, STRING) after append");

          // vector

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.vector", openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.vector, VECTOR)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.vector", openfluid::core::Value::NONE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.vector, NONE)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.vector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.vector, timestep, VECTOR) before append");

          TheVector = openfluid::core::VectorValue(VectorSize,TheDouble);
          OPENFLUID_AppendVariable(TU,"tests.typed.vector",TheVector);

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.vector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.vector, timestep, VECTOR) after append");

          // old style vector

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.oldvector", openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.oldvector, VECTOR)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.oldvector", openfluid::core::Value::NONE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.oldvector, NONE)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.oldvector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.oldvector, timestep, VECTOR) before append");

          TheVector = openfluid::core::VectorValue(VectorSize,TheDouble);
          OPENFLUID_AppendVariable(TU,"tests.typed.oldvector",TheVector);

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.oldvector",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.oldvector, timestep, VECTOR) after append");

          // matrix value

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.matrix", openfluid::core::Value::MATRIX))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.matrix, MATRIX)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.matrix", openfluid::core::Value::NONE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.matrix, NONE)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.matrix",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::MATRIX))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.matrix, timestep, MATRIX) before append");

          TheMatrix = openfluid::core::MatrixValue(MatrixColsNb,MatrixRowsNb,TheDouble);
          OPENFLUID_AppendVariable(TU,"tests.typed.matrix",TheMatrix);

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.matrix",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::MATRIX))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.matrix, timestep, MATRIX) after append");

          // map value

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.map", openfluid::core::Value::MAP))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.map, MAP)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.map", openfluid::core::Value::NONE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.map, NONE)");

          if (OPENFLUID_IsTypedVariableExist(TU,"tests.typed.map",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::MAP))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.map, timestep, MAP) before append");

          TheMap = openfluid::core::MapValue();
          TheMap.setString("key1",TheString);
          TheMap.setDouble("key2",TheDouble);
          OPENFLUID_AppendVariable(TU,"tests.typed.map",TheMap);

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.map",OPENFLUID_GetCurrentTimeIndex(), openfluid::core::Value::MAP))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.map, timestep, MAP) after append");
        }

      }

      m_ProductionCounter++;

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};

// =====================================================================
// =====================================================================

DEFINE_SIMULATOR_CLASS(VarsPrimitivesProdSimulator)

