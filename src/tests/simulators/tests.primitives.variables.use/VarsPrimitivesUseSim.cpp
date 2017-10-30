/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/

/**
  @file VarsPrimitivesUseSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QString>

#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/core/MapValue.hpp>
#include <openfluid/core/TreeValue.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>


// =====================================================================
// =====================================================================


#define THIS_SIM_ID "tests.primitives.variables.use"

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


  DECLARE_USED_PARAMETER("strparam","=strvalue","-");
  DECLARE_USED_PARAMETER("doubleparam","=0.1","-");
  DECLARE_USED_PARAMETER("longparam","=10","-");

  DECLARE_REQUIRED_VARIABLE("tests.double","TestUnits","double for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.integer","TestUnits","integer for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.bool","TestUnits","boolean for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.string","TestUnits","string for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.null","TestUnits","null for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.doubleval","TestUnits","double value for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.integerval","TestUnits","integer value for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.boolval","TestUnits","boolean value for tests","");
  DECLARE_REQUIRED_VAR("tests.stringval","TestUnits","string value for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.vector","TestUnits","vector for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.matrix","TestUnits","matrix for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.map","TestUnits","map for tests","");

  DECLARE_REQUIRED_VARIABLE("tests.typed.none","TestUnits","untyped for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.double[double]","TestUnits","double for tests","");
  DECLARE_REQUIRED_VAR("tests.typed.integer[integer]","TestUnits","integer for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.bool[boolean]","TestUnits","boolean for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.string[string]","TestUnits","string for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.vector[vector]","TestUnits","vector for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.oldvector[]","TestUnits","old style vector for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.matrix[matrix]","TestUnits","matrix for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.map[map]","TestUnits","map for tests","");

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class VarsPrimitivesUseSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    double m_ParamDouble;

    long m_ParamLong;

    std::string m_ParamString;

    unsigned long m_ExpectedValCount;

  public:


    VarsPrimitivesUseSimulator() : PluggableSimulator(),
    m_ParamDouble(0.1), m_ParamLong(10), m_ParamString("strvalue"), m_ExpectedValCount(0)
    {


    }


    // =====================================================================
    // =====================================================================


    ~VarsPrimitivesUseSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      OPENFLUID_GetSimulatorParameter(Params,"strparam",m_ParamString);
      OPENFLUID_GetSimulatorParameter(Params,"doubleparam",m_ParamDouble);
      OPENFLUID_GetSimulatorParameter(Params,"longparam",m_ParamLong);

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

      m_ExpectedValCount++;

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {

      // untyped

      {
        openfluid::core::SpatialUnit* TU;
        unsigned int TUID;
        unsigned int CurrIndex;

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
        openfluid::core::DoubleValue VarDoubleVal;
        openfluid::core::IntegerValue VarLongVal;
        openfluid::core::BooleanValue VarBoolVal;
        openfluid::core::StringValue VarStringVal;
        openfluid::core::VectorValue VarVectorVal;
        openfluid::core::MatrixValue VarMatrixVal;
        openfluid::core::MapValue VarMapVal;
        openfluid::core::TreeValue VarTreeVal;
        openfluid::core::NullValue VarNullVal;

        openfluid::core::IndexedValue IndValue;
        openfluid::core::IndexedValueList IndValueList;

        double PreDouble;
        long PreLong;
        bool PreBool;
        std::string PreString;

        double NewDouble;
        long NewLong;
        bool NewBool;
        std::string NewString;
        unsigned long NewVectorSize = 5;
        unsigned long NewMatrixColsNb = RefMatrixColsNb + 1;
        unsigned long NewMatrixRowsNb = RefMatrixRowsNb + 1;

/*        openfluid::core::EventsCollection TheEvents;
        openfluid::core::Event Ev;*/

        m_ExpectedValCount++;


        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {


          TUID = TU->getID();
          CurrIndex = OPENFLUID_GetCurrentTimeIndex();

          RefDouble = (double)TUID/10;
          RefLong = TUID;
          RefBool = (TUID%2 == 0);
          RefString = QString("ID %1").arg(TUID).toStdString();

          PreDouble = TUID*(CurrIndex-OPENFLUID_GetDefaultDeltaT())*m_ParamDouble;
          PreLong = TUID*(CurrIndex-OPENFLUID_GetDefaultDeltaT())*m_ParamLong;
          PreBool = ((TUID*(CurrIndex-OPENFLUID_GetDefaultDeltaT()))%2 == 0);
          PreString = QString("%1 %2x%3")
                      .arg(QString::fromStdString(m_ParamString))
                      .arg(TUID)
                      .arg(CurrIndex-OPENFLUID_GetDefaultDeltaT())
                      .toStdString();

          NewDouble = TUID*CurrIndex*m_ParamDouble;
          NewLong = TUID*CurrIndex*m_ParamLong;
          NewBool = ((TUID*CurrIndex)%2 == 0);
          NewString = QString("%1 %2x%3")
                      .arg(QString::fromStdString(m_ParamString))
                      .arg(TUID).arg(CurrIndex)
                      .toStdString();


          // ====== Variables ======


          // double

          VarDouble = 0.0;
          OPENFLUID_GetVariable(TU,"tests.double",CurrIndex,VarDouble);
          if (!openfluid::scientific::isCloseEnough(VarDouble,RefDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.double, by reference)");

          VarDouble = OPENFLUID_GetVariable(TU,"tests.double",CurrIndex)->asDoubleValue();
          if (!openfluid::scientific::isCloseEnough(VarDouble,RefDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.double, by return)");

          VarDouble = 0.0;
          OPENFLUID_GetVariable(TU,"tests.double",VarDouble);
          if (!openfluid::scientific::isCloseEnough(VarDouble,RefDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.double, by reference, without index)");

          VarDouble = OPENFLUID_GetVariable(TU,"tests.double")->asDoubleValue();
          if (!openfluid::scientific::isCloseEnough(VarDouble,RefDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.double, by return, without index)");

          OPENFLUID_GetLatestVariable(TU,"tests.double",IndValue);
          if (IndValue.getIndex() != OPENFLUID_GetCurrentTimeIndex())
            OPENFLUID_RaiseError("incorrect time index (tests.double, by reference, latest variable)");

          if (!openfluid::scientific::isCloseEnough<double>(IndValue.value()->asDoubleValue(),RefDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.double, by reference, latest variable");

          IndValue.clear();
          IndValue = OPENFLUID_GetLatestVariable(TU,"tests.double");
          if (IndValue.getIndex() != OPENFLUID_GetCurrentTimeIndex())
            OPENFLUID_RaiseError("incorrect time index (tests.double, by return, latest variable)");

          if (!openfluid::scientific::isCloseEnough<double>(IndValue.value()->asDoubleValue(),RefDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.double, by return, latest variable");


          VarDouble = 0.0;
          OPENFLUID_GetLatestVariables(TU,"tests.double",OPENFLUID_GetCurrentTimeIndex()-1,IndValueList);
          if (!openfluid::scientific::isCloseEnough<double>(
                IndValueList.back().value()->asDoubleValue(),RefDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.double) get by reference");


          VarDouble = 0.0;
          IndValueList.clear();
          IndValueList = OPENFLUID_GetLatestVariables(TU,"tests.double",OPENFLUID_GetCurrentTimeIndex()-1);
          if (!openfluid::scientific::isCloseEnough<double>(
                IndValueList.back().value()->asDoubleValue(),RefDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.double) get by reference");


          OPENFLUID_SetVariable(TU,"tests.double",NewDouble);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.double",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarDouble);
            if (!openfluid::scientific::isCloseEnough(VarDouble,PreDouble,0.00001))
              OPENFLUID_RaiseError("incorrect double value at t-1 (tests.double)");

            VarDouble = OPENFLUID_GetVariable(TU,"tests.double",CurrIndex-OPENFLUID_GetDefaultDeltaT())
                        ->asDoubleValue();
            if (!openfluid::scientific::isCloseEnough(VarDouble,PreDouble,0.00001))
              OPENFLUID_RaiseError("incorrect double value at t-1 (tests.double)");
          }

          OPENFLUID_GetVariable(TU,"tests.double",CurrIndex,VarDouble);

          if (!openfluid::scientific::isCloseEnough(VarDouble,NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value after update (tests.double)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.double"))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.double)");


          // double value

          OPENFLUID_GetVariable(TU,"tests.doubleval",VarDoubleVal);
          if (VarDoubleVal.get() != RefDouble)
            OPENFLUID_RaiseError("incorrect double value (tests.doubleval, by reference, without index)");

          VarDoubleVal= OPENFLUID_GetVariable(TU,"tests.doubleval")->asDoubleValue();
          if (VarDoubleVal.get() != RefDouble)
            OPENFLUID_RaiseError("incorrect double value (tests.doubleval, by return, without index");

          VarDoubleVal.set(0.0);
          OPENFLUID_GetVariable(TU,"tests.doubleval",CurrIndex,VarDoubleVal);
          if (VarDoubleVal.get() != RefDouble)
            OPENFLUID_RaiseError("incorrect double value (tests.doubleval, by reference)");

          VarDoubleVal.set(0.0);
          VarDoubleVal = OPENFLUID_GetVariable(TU,"tests.doubleval",CurrIndex)->asDoubleValue();
          if (VarDoubleVal.get() != RefDouble)
            OPENFLUID_RaiseError("incorrect double value (tests.doubleval, by return)");


          OPENFLUID_SetVariable(TU,"tests.doubleval",NewDouble);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.doubleval",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarDoubleVal);
            if (!openfluid::scientific::isCloseEnough(VarDoubleVal.get(),PreDouble,0.00001))
              OPENFLUID_RaiseError("incorrect double value at t-1 (tests.doubleval)");
          }

          OPENFLUID_GetVariable(TU,"tests.doubleval",CurrIndex,VarDoubleVal);

          if (!openfluid::scientific::isCloseEnough(VarDoubleVal.get(),NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value after update (tests.doubleval)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.doubleval"))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.doubleval)");


          // long

          VarLong = 0;
          OPENFLUID_GetVariable(TU,"tests.integer",CurrIndex,VarLong);
          if (VarLong != RefLong)
            OPENFLUID_RaiseError("incorrect long value (tests.integer) get by reference");

          VarLong = OPENFLUID_GetVariable(TU,"tests.integer",CurrIndex)->asIntegerValue();
          if (VarLong != RefLong)
            OPENFLUID_RaiseError("incorrect long value (tests.integer, by return)");

          VarLong = OPENFLUID_GetVariable(TU,"tests.integer")->asIntegerValue();
          if (VarLong != RefLong)
            OPENFLUID_RaiseError("incorrect long value (tests.integer, by return, without time index)");

          OPENFLUID_SetVariable(TU,"tests.integer",NewLong);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.integer",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarLong);
            if (VarLong != PreLong)
              OPENFLUID_RaiseError("incorrect long value at t-1 (tests.integer)");
          }

          OPENFLUID_GetVariable(TU,"tests.integer",CurrIndex,VarLong);

          if (VarLong != NewLong)
            OPENFLUID_RaiseError("incorrect long value after update (tests.integer)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.integer"))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.integer)");


          // long value

          VarLongVal.set(0);
          OPENFLUID_GetVariable(TU,"tests.integerval",CurrIndex,VarLongVal);
          if (VarLongVal.get() != RefLong)
            OPENFLUID_RaiseError("incorrect long value (tests.integerval, by reference)");


          VarLongVal.set(0);
          VarLongVal = OPENFLUID_GetVariable(TU,"tests.integerval",CurrIndex)->asIntegerValue();
          if (VarLongVal != RefLong)
            OPENFLUID_RaiseError("incorrect long value (tests.integerval, by return)");

          VarLongVal.set(0);
          VarLongVal = OPENFLUID_GetVariable(TU,"tests.integerval")->asIntegerValue();
          if (VarLongVal != RefLong)
            OPENFLUID_RaiseError("incorrect long value (tests.integerval, by return, without time index)");


          OPENFLUID_SetVariable(TU,"tests.integerval",NewLong);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.integerval",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarLongVal);
            if (VarLongVal.get() != PreLong)
              OPENFLUID_RaiseError("incorrect long value at t-1 (tests.integerval)");
          }

          OPENFLUID_GetVariable(TU,"tests.integerval",CurrIndex,VarLongVal);

          if (VarLongVal.get() != NewLong)
            OPENFLUID_RaiseError("incorrect long value after update (tests.integerval)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.integerval"))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.integerval)");

          // values count since beginning of simulation
          OPENFLUID_GetVariables(TU,"tests.integer",0,CurrIndex-1,IndValueList);
          if (IndValueList.size() != (m_ExpectedValCount-1))
            OPENFLUID_RaiseError(
                                 "incorrect long value list between t0 and current time index -1 (tests.integer) "
                                 "by variables");


          IndValueList.clear();
          IndValueList = OPENFLUID_GetVariables(TU,"tests.integer",0,CurrIndex-1);
          if (IndValueList.size() != (m_ExpectedValCount-1))
            OPENFLUID_RaiseError(
                                 "incorrect long value list between t0 and current time index -1 (tests.integer) "
                                 "by return");


          OPENFLUID_GetLatestVariables(TU,"tests.integer",0,IndValueList);
          if (IndValueList.size() != m_ExpectedValCount)
            OPENFLUID_RaiseError("incorrect long value list since t0 (tests.integer) by latest variables");

          OPENFLUID_GetVariables(TU,"tests.integer",0,CurrIndex,IndValueList);
          if (IndValueList.size() != m_ExpectedValCount)
            OPENFLUID_RaiseError("incorrect long value list between t0 and current time index (tests.integer) "
                                 "by variables");


          // bool

          VarBool = false;
          OPENFLUID_GetVariable(TU,"tests.bool",CurrIndex,VarBool);
          if (VarBool != RefBool)
            OPENFLUID_RaiseError("incorrect bool value (tests.bool, by reference)");


          VarBool = OPENFLUID_GetVariable(TU,"tests.bool",CurrIndex)->asBooleanValue();
          if (VarBool != RefBool)
            OPENFLUID_RaiseError("incorrect bool value (tests.bool, by return)");

          VarBool = OPENFLUID_GetVariable(TU,"tests.bool")->asBooleanValue();
          if (VarBool != RefBool)
            OPENFLUID_RaiseError("incorrect bool value (tests.bool, by return, without time index)");


          OPENFLUID_SetVariable(TU,"tests.bool",NewBool);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.bool",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarBool);
            if (VarBool != PreBool)
              OPENFLUID_RaiseError("incorrect bool value at t-1 (tests.bool)");
          }

          OPENFLUID_GetVariable(TU,"tests.bool",CurrIndex,VarBool);
          if (VarBool != NewBool)
            OPENFLUID_RaiseError("incorrect bool value after update (tests.bool)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.bool"))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.bool)");


          // bool value

          VarBoolVal.set(false);
          OPENFLUID_GetVariable(TU,"tests.boolval",CurrIndex,VarBoolVal);
          if (VarBoolVal.get() != RefBool)
            OPENFLUID_RaiseError("incorrect bool value (tests.boolval) get by reference");

          VarBoolVal.set(false);
          VarBoolVal = OPENFLUID_GetVariable(TU,"tests.boolval",CurrIndex)->asBooleanValue();
          if (VarBoolVal.get() != RefBool)
            OPENFLUID_RaiseError("incorrect bool value (tests.boolval, by return");

          VarBoolVal.set(false);
          VarBoolVal = OPENFLUID_GetVariable(TU,"tests.boolval")->asBooleanValue();
          if (VarBoolVal.get() != RefBool)
            OPENFLUID_RaiseError("incorrect bool value (tests.boolval, by return, without time index");


          OPENFLUID_SetVariable(TU,"tests.boolval",NewBool);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.boolval",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarBoolVal);
            if (VarBoolVal.get() != PreBool)
              OPENFLUID_RaiseError("incorrect bool value at t-1 (tests.boolval)");
          }

          OPENFLUID_GetVariable(TU,"tests.boolval",CurrIndex,VarBoolVal);

          if (VarBoolVal.get() != NewBool)
            OPENFLUID_RaiseError("incorrect bool value after update (tests.boolval)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.boolval"))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.boolval)");


          // string

          VarString = "";
          OPENFLUID_GetVariable(TU,"tests.string",CurrIndex,VarString);
          if (VarString != RefString)
            OPENFLUID_RaiseError("incorrect string value (tests.string) get by reference");

          VarString = OPENFLUID_GetVariable(TU,"tests.string",CurrIndex)->asStringValue();
          if (VarString != RefString)
            OPENFLUID_RaiseError("incorrect string value (tests.string, by return");

          VarString = OPENFLUID_GetVariable(TU,"tests.string")->asStringValue();
          if (VarString != RefString)
            OPENFLUID_RaiseError("incorrect string value (tests.string, by return, without time index");

          OPENFLUID_SetVariable(TU,"tests.string",NewString);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.string",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarString);
            if (VarString != PreString)
              OPENFLUID_RaiseError("incorrect string value at t-1 (tests.string)");
          }

          OPENFLUID_GetVariable(TU,"tests.string",CurrIndex,VarString);

          if (VarString != NewString)
            OPENFLUID_RaiseError("incorrect string value after update (tests.string)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.string"))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.string)");


          // null value

          OPENFLUID_GetVariable(TU,"tests.null",CurrIndex,VarNullVal);
          if (VarNullVal.getType() != openfluid::core::Value::NULLL)
            OPENFLUID_RaiseError("incorrect null value (tests.null) get by reference");

          VarNullVal = OPENFLUID_GetVariable(TU,"tests.null",CurrIndex)->asNullValue();


          // vector value

          VarVectorVal.clear();
          OPENFLUID_GetVariable(TU,"tests.vector",CurrIndex,VarVectorVal);
          if (VarVectorVal.getSize() != RefVectorSize)
            OPENFLUID_RaiseError("incorrect vector size get by reference");


          VarVectorVal.clear();
          VarVectorVal = OPENFLUID_GetVariable(TU,"tests.vector",CurrIndex)->asVectorValue();
          if (VarVectorVal.getSize() != RefVectorSize)
            OPENFLUID_RaiseError("incorrect vector size, by return");

          VarVectorVal.clear();
          VarVectorVal = OPENFLUID_GetVariable(TU,"tests.vector")->asVectorValue();
          if (VarVectorVal.getSize() != RefVectorSize)
            OPENFLUID_RaiseError("incorrect vector size, by return, without time index");


          openfluid::core::VectorValue NewVect(NewVectorSize,NewDouble);
          OPENFLUID_SetVariable(TU,"tests.vector",NewVect);

          OPENFLUID_GetVariable(TU,"tests.vector",CurrIndex,VarVectorVal);
          if (VarVectorVal.getSize() != NewVectorSize)
            OPENFLUID_RaiseError("incorrect vector size after update");

          if (!openfluid::scientific::isCloseEnough(VarVectorVal[0],NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect vector value at index 0 after update");

          if (!openfluid::scientific::isCloseEnough(VarVectorVal[4],NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect vector value at index 4 after update");


          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.vector",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarVectorVal);

            if (VarVectorVal.getSize() != NewVectorSize)
              OPENFLUID_RaiseError("incorrect vector size at t-1");

            if (!openfluid::scientific::isCloseEnough(VarVectorVal[0],PreDouble,0.00001))
              OPENFLUID_RaiseError("incorrect vector value at index 0 at t-1");

            if (!openfluid::scientific::isCloseEnough(VarVectorVal[4],PreDouble,0.00001))
              OPENFLUID_RaiseError("incorrect vector value at index 4 at t-1");

          }

          if (!OPENFLUID_IsVariableExist(TU,"tests.vector"))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.vector)");


          // matrix value

          VarMatrixVal.clear();
          OPENFLUID_GetVariable(TU,"tests.matrix",CurrIndex,VarMatrixVal);
          if (VarMatrixVal.getColsNbr() != RefMatrixColsNb)
            OPENFLUID_RaiseError("incorrect matrix cols nb get by reference");
          if (VarMatrixVal.getRowsNbr() != RefMatrixRowsNb)
            OPENFLUID_RaiseError("incorrect matrix rows nb get by reference");


          VarMatrixVal.clear();
          VarMatrixVal = OPENFLUID_GetVariable(TU,"tests.matrix",CurrIndex)->asMatrixValue();
          if (VarMatrixVal.getColsNbr() != RefMatrixColsNb)
            OPENFLUID_RaiseError("incorrect matrix cols nb get by return");
          if (VarMatrixVal.getRowsNbr() != RefMatrixRowsNb)
            OPENFLUID_RaiseError("incorrect matrix rows nb get by return");

          VarMatrixVal.clear();
          VarMatrixVal = OPENFLUID_GetVariable(TU,"tests.matrix")->asMatrixValue();
          if (VarMatrixVal.getColsNbr() != RefMatrixColsNb)
            OPENFLUID_RaiseError("incorrect matrix cols nb get by return, without time index");
          if (VarMatrixVal.getRowsNbr() != RefMatrixRowsNb)
            OPENFLUID_RaiseError("incorrect matrix rows nb get by return, without time index");

          VarMatrixVal.clear();
          OPENFLUID_GetVariable(TU,"tests.matrix",VarMatrixVal);
          if (VarMatrixVal.getColsNbr() != RefMatrixColsNb)
            OPENFLUID_RaiseError("incorrect matrix cols nb get by reference without index");
          if (VarMatrixVal.getRowsNbr() != RefMatrixRowsNb)
            OPENFLUID_RaiseError("incorrect matrix rows nb get by reference without index");

          openfluid::core::MatrixValue NewMatrix(NewMatrixColsNb,NewMatrixRowsNb,NewDouble);
          OPENFLUID_SetVariable(TU,"tests.matrix",NewMatrix);

          OPENFLUID_GetVariable(TU,"tests.matrix",CurrIndex,VarMatrixVal);
          if (VarMatrixVal.getColsNbr() != NewMatrixColsNb)
            OPENFLUID_RaiseError("incorrect matrix cols nb after update");
          if (VarMatrixVal.getRowsNbr() != NewMatrixRowsNb)
            OPENFLUID_RaiseError("incorrect matrix rows nb after update");

          if (!openfluid::scientific::isCloseEnough(VarMatrixVal.get(0,0),NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect matrix value at index 0,0 after update");

          if (!openfluid::scientific::isCloseEnough(VarMatrixVal.get(4,3),NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect matrix value at index 4,3 after update");


          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.matrix",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarMatrixVal);

            if (VarMatrixVal.getSize() != NewMatrixRowsNb*NewMatrixColsNb)
              OPENFLUID_RaiseError("incorrect matrix size at t-1");

            if (!openfluid::scientific::isCloseEnough(VarMatrixVal.get(0,0),PreDouble,0.00001))
              OPENFLUID_RaiseError("incorrect matrix value at index 0,0 at t-1");

            if (!openfluid::scientific::isCloseEnough(VarMatrixVal.get(4,3),PreDouble,0.00001))
              OPENFLUID_RaiseError("incorrect matrix value at index 4,3 at t-1");

          }

          if (!OPENFLUID_IsVariableExist(TU,"tests.matrix"))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.matrix)");


          // map value

          VarMapVal.clear();
          OPENFLUID_GetVariable(TU,"tests.map",CurrIndex,VarMapVal);
          if (VarMapVal.getSize() != 2)
            OPENFLUID_RaiseError("incorrect map size get by reference");

          VarMapVal.clear();
          VarMapVal = OPENFLUID_GetVariable(TU,"tests.map",CurrIndex)->asMapValue();
          if (VarMapVal.getSize() != 2)
            OPENFLUID_RaiseError("incorrect map size get by return");

           VarMapVal.clear();
           VarMapVal = OPENFLUID_GetVariable(TU,"tests.map")->asMapValue();
           if (VarMapVal.getSize() != 2)
             OPENFLUID_RaiseError("incorrect map size get by return, without time index");

          openfluid::core::MapValue NewMap;
          NewMap.setString("key1",NewString);
          NewMap.setDouble("key2",NewDouble);
          NewMap.setBoolean("key3",NewBool);
          OPENFLUID_SetVariable(TU,"tests.map",NewMap);

          OPENFLUID_GetVariable(TU,"tests.map",CurrIndex,VarMapVal);
          if (VarMapVal.getSize() != 3)
            OPENFLUID_RaiseError("incorrect map size after update");

          if (VarMapVal.getString("key1") != NewString)
            OPENFLUID_RaiseError("incorrect map value at key key1 after update");

          if (!openfluid::scientific::isCloseEnough(VarMapVal.getDouble("key2"),NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect map value at key key2 after update");

          if (VarMapVal.getBoolean("key3") != NewBool)
            OPENFLUID_RaiseError("incorrect map value at key key3 after update");


          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.map",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarMapVal);

            if (VarMapVal.getSize() != 3)
              OPENFLUID_RaiseError("incorrect map size at t-1");

            if (VarMapVal.getString("key1") != PreString)
              OPENFLUID_RaiseError("incorrect map value at key key1 at t-1");

            if (!openfluid::scientific::isCloseEnough(VarMapVal.getDouble("key2"),PreDouble,0.00001))
              OPENFLUID_RaiseError("incorrect map value at key key2 at t-1");

            if (VarMapVal.getBoolean("key3") != PreBool)
              OPENFLUID_RaiseError("incorrect map value at key key3 at t-1");

          }

          if (!OPENFLUID_IsVariableExist(TU,"tests.map"))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.map)");


          // tree value

          VarTreeVal.clear();
          OPENFLUID_GetVariable(TU,"tests.tree",CurrIndex,VarTreeVal);
          if (VarTreeVal.size() != 8)
            OPENFLUID_RaiseError("incorrect tree size get by reference");

          VarTreeVal.clear();
          VarTreeVal = OPENFLUID_GetVariable(TU,"tests.tree",CurrIndex)->asTreeValue();
          if (VarTreeVal.size() != 8)
            OPENFLUID_RaiseError("incorrect tree size get by return");

          VarTreeVal.clear();
          VarTreeVal = OPENFLUID_GetVariable(TU,"tests.tree")->asTreeValue();
          if (VarTreeVal.size() != 8)
            OPENFLUID_RaiseError("incorrect tree size get by return, without time index");

          /* Note: the tree is created using this:
             TheTree = openfluid::core::TreeValue();
             TheTree.addChild("x").addChild("x1").addChild("x2",2);
             TheTree.child("x").child("x1").addChild("x22",22);
             TheTree.addChild("y").addChild("y1").addChild("y2",OPENFLUID_GetCurrentTimeIndex());
           */

          if (!openfluid::scientific::isCloseEnough(VarTreeVal.child("y").child("y1").child("y2").getValue(),
                                                    double(OPENFLUID_GetCurrentTimeIndex()),0.01))
            OPENFLUID_RaiseError("incorrect tree value at y.y1.y2");

          openfluid::core::TreeValue NewTree;
          NewTree.addChild("a").addChild("b",2.0).addChild("c",3.0);
          OPENFLUID_SetVariable(TU,"tests.tree",NewTree);
          OPENFLUID_GetVariable(TU,"tests.tree",CurrIndex,VarTreeVal);

          if (VarTreeVal.size() != 4)
            OPENFLUID_RaiseError("incorrect tree size get by reference");

          if (VarTreeVal.child("a").hasValue())
            OPENFLUID_RaiseError("incorrect tree value existence at a");

          if (!openfluid::scientific::isCloseEnough(VarTreeVal.child("a").child("b").getValue(),
                                                    2.0,0.01))
            OPENFLUID_RaiseError("incorrect tree value at b");

          if (!openfluid::scientific::isCloseEnough(VarTreeVal.child("a").child("b").getChildValue("c",0.0),
                                                    3.0,0.01))
            OPENFLUID_RaiseError("incorrect tree value at c");

          VarTreeVal.clear();
          if (VarTreeVal.size() != 1)
            OPENFLUID_RaiseError("incorrect tree after clear");

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.tree",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarTreeVal);
            if (VarTreeVal.size() != 4)
              OPENFLUID_RaiseError("incorrect tree size at t-1");
          }

          if (!OPENFLUID_IsVariableExist(TU,"tests.tree"))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.tree)");
        }
      }



      // typed
      {
        openfluid::core::SpatialUnit* TU;
        unsigned int TUID;
        unsigned int CurrIndex;

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
        openfluid::core::TreeValue VarTreeVal;

        openfluid::core::IndexedValue IndValue;
        openfluid::core::IndexedValueList IndValueList;

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
          CurrIndex = OPENFLUID_GetCurrentTimeIndex();

          RefDouble = (double)TUID/10;
          RefLong = TUID;
          RefBool = (TUID%2 == 0);
          RefString = QString("ID %1").arg(TUID).toStdString();

          NewDouble = TUID*CurrIndex*0.1;
          NewLong = TUID*CurrIndex*10;
          NewBool = ((TUID*CurrIndex)%2 == 0);
          NewString = QString("%1 %2x%3").arg("strvalue").arg(TUID).arg(CurrIndex).toStdString();


          // double

          OPENFLUID_GetVariable(TU,"tests.typed.double",CurrIndex,VarDouble);
          if (!openfluid::scientific::isCloseEnough(VarDouble,RefDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.double) get by reference");

          VarDouble = OPENFLUID_GetVariable(TU,"tests.typed.double",CurrIndex)->asDoubleValue();
          if (!openfluid::scientific::isCloseEnough(VarDouble,RefDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.double, by return)");

          OPENFLUID_GetLatestVariable(TU,"tests.typed.double",IndValue);
          if (!openfluid::scientific::isCloseEnough<double>(IndValue.value()->asDoubleValue(),RefDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.double) get by latest variable");


          OPENFLUID_SetVariable(TU,"tests.typed.double",NewDouble);

          OPENFLUID_GetVariable(TU,"tests.typed.double",CurrIndex,VarDouble);
          if (!openfluid::scientific::isCloseEnough(VarDouble,NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value after update (tests.double)");

          OPENFLUID_GetLatestVariables(TU,"tests.typed.double",OPENFLUID_GetCurrentTimeIndex()-1,IndValueList);
          if (!openfluid::scientific::isCloseEnough<double>(
                IndValueList.back().value()->asDoubleValue(),NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.double) get by latest variable");


          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.double",openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsTypedVariableExist (tests.double, DOUBLE)");


          // long

          OPENFLUID_GetVariable(TU,"tests.typed.integer",CurrIndex,VarLong);
          if (VarLong != RefLong)
            OPENFLUID_RaiseError("incorrect long value (tests.integer) get by reference");

          OPENFLUID_SetVariable(TU,"tests.typed.integer",NewLong);

          OPENFLUID_GetVariable(TU,"tests.typed.integer",CurrIndex,VarLong);
          if (VarLong != NewLong)
            OPENFLUID_RaiseError("incorrect long value after update (tests.integer)");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.integer",openfluid::core::Value::INTEGER))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsTypedVariableExist (tests.integer, INTEGER)");


          // bool

          OPENFLUID_GetVariable(TU,"tests.typed.bool",CurrIndex,VarBool);
          if (VarBool != RefBool)
            OPENFLUID_RaiseError("incorrect bool value (tests.bool) get by reference");

          OPENFLUID_SetVariable(TU,"tests.typed.bool",NewBool);

          OPENFLUID_GetVariable(TU,"tests.typed.bool",CurrIndex,VarBool);
          if (VarBool != NewBool)
            OPENFLUID_RaiseError("incorrect bool value after update (tests.bool)");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.bool",openfluid::core::Value::BOOLEAN))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsTypedVariableExist (tests.bool, BOOLEAN)");


          // string

          OPENFLUID_GetVariable(TU,"tests.typed.string",CurrIndex,VarString);
          if (VarString != RefString)
            OPENFLUID_RaiseError("incorrect string value (tests.string) get by reference");

          OPENFLUID_SetVariable(TU,"tests.typed.string",NewString);

          OPENFLUID_GetVariable(TU,"tests.typed.string",CurrIndex,VarString);
          if (VarString != NewString)
            OPENFLUID_RaiseError("incorrect string value after update (tests.string)");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.string",openfluid::core::Value::STRING))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsTypedVariableExist (tests.string, STRING)");


          // vector

          OPENFLUID_GetVariable(TU,"tests.typed.vector",CurrIndex,VarVectorVal);
          if (VarVectorVal.getSize() != RefVectorSize)
            OPENFLUID_RaiseError("incorrect vector size get by reference");

          OPENFLUID_GetVariable(TU,"tests.typed.vector",VarVectorVal);
          if (VarVectorVal.getSize() != RefVectorSize)
            OPENFLUID_RaiseError("incorrect vector size get by reference");


          OPENFLUID_GetLatestVariable(TU,"tests.typed.vector",IndValue);
          if (IndValue.value()->asVectorValue().getSize() != RefVectorSize)
            OPENFLUID_RaiseError("incorrect vector size get by latest variable");


          openfluid::core::VectorValue NewVect(NewVectorSize,NewDouble);
          OPENFLUID_SetVariable(TU,"tests.typed.vector",NewVect);

          OPENFLUID_GetVariable(TU,"tests.typed.vector",CurrIndex,VarVectorVal);
          if (VarVectorVal.getSize() != NewVectorSize)
            OPENFLUID_RaiseError("incorrect vector size after update");
          if (!openfluid::scientific::isCloseEnough(VarVectorVal[0],NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect vector value at index 0 after update");
          if (!openfluid::scientific::isCloseEnough(VarVectorVal[4],NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect vector value at index 4 after update");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.vector",openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsTypedVariableExist (tests.vector, VECTOR)");


          // old style vector

          VarVectorVal.clear();
          OPENFLUID_GetVariable(TU,"tests.typed.oldvector",CurrIndex,VarVectorVal);
          if (VarVectorVal.getSize() != RefVectorSize)
            OPENFLUID_RaiseError("incorrect oldvector size get by reference");

          OPENFLUID_SetVariable(TU,"tests.typed.oldvector",NewVect);

          OPENFLUID_GetVariable(TU,"tests.typed.oldvector",CurrIndex,VarVectorVal);
          if (VarVectorVal.getSize() != NewVectorSize)
            OPENFLUID_RaiseError("incorrect oldvector size after update");
          if (!openfluid::scientific::isCloseEnough(VarVectorVal[0],NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect oldvector value at index 0 after update");
          if (!openfluid::scientific::isCloseEnough(VarVectorVal[4],NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect oldvector value at index 4 after update");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.oldvector",openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsTypedVariableExist (tests.oldvector, VECTOR)");


          // matrix

          OPENFLUID_GetVariable(TU,"tests.typed.matrix",CurrIndex,VarMatrixVal);
          if (VarMatrixVal.getColsNbr() != RefMatrixColsNb)
            OPENFLUID_RaiseError("incorrect matrix cols nb get by reference");
          if (VarMatrixVal.getRowsNbr() != RefMatrixRowsNb)
            OPENFLUID_RaiseError("incorrect matrix rows nb get by reference");

          VarMatrixVal = OPENFLUID_GetVariable(TU,"tests.typed.matrix",CurrIndex)->asMatrixValue();
          if (VarMatrixVal.getColsNbr() != RefMatrixColsNb)
            OPENFLUID_RaiseError("incorrect matrix cols nb get by return");
          if (VarMatrixVal.getRowsNbr() != RefMatrixRowsNb)
            OPENFLUID_RaiseError("incorrect matrix rows nb get by return");


          openfluid::core::MatrixValue NewMatrix(NewMatrixColsNb,NewMatrixRowsNb,NewDouble);
          OPENFLUID_SetVariable(TU,"tests.typed.matrix",NewMatrix);

          OPENFLUID_GetVariable(TU,"tests.typed.matrix",CurrIndex,VarMatrixVal);
          if (VarMatrixVal.getColsNbr() != NewMatrixColsNb)
            OPENFLUID_RaiseError("incorrect matrix cols nb after update");
          if (VarMatrixVal.getRowsNbr() != NewMatrixRowsNb)
            OPENFLUID_RaiseError("incorrect matrix rows nb after update");
          if (!openfluid::scientific::isCloseEnough(VarMatrixVal.get(0,0),NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect matrix value at index 0,0 after update");
          if (!openfluid::scientific::isCloseEnough(VarMatrixVal.get(4,3),NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect matrix value at index 4,3 after update");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.matrix",openfluid::core::Value::MATRIX))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsTypedVariableExist (tests.matrix, MATRIX)");


          // map

          OPENFLUID_GetVariable(TU,"tests.typed.map",CurrIndex,VarMapVal);
          if (VarMapVal.getSize() != 2)
            OPENFLUID_RaiseError("incorrect map size get by reference");

          openfluid::core::MapValue NewMap;
          NewMap.setString("key1",NewString);
          NewMap.setDouble("key2",NewDouble);
          NewMap.setBoolean("key3",NewBool);
          NewMap.setVectorValue("key4",openfluid::core::VectorValue(4,4.44));
          NewMap.setMatrixValue("key5",openfluid::core::MatrixValue(5,5,5.55));
          OPENFLUID_SetVariable(TU,"tests.typed.map",NewMap);

          OPENFLUID_GetVariable(TU,"tests.typed.map",CurrIndex,VarMapVal);
          if (VarMapVal.getSize() != 5)
            OPENFLUID_RaiseError("incorrect map size after update");
          if (VarMapVal.getString("key1") != NewString)
            OPENFLUID_RaiseError("incorrect map value at key key1 after update");
          if (!openfluid::scientific::isCloseEnough(VarMapVal.getDouble("key2"),NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect map value at key key2 after update");
          if (VarMapVal.getBoolean("key3") != NewBool)
            OPENFLUID_RaiseError("incorrect map value at key key3 after update");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.map",openfluid::core::Value::MAP))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsTypedVariableExist (tests.map, MAP)");



          // tree value

          VarTreeVal.clear();
          OPENFLUID_GetVariable(TU,"tests.typed.tree",CurrIndex,VarTreeVal);
          if (VarTreeVal.size() != 8)
            OPENFLUID_RaiseError("incorrect typed tree size get by reference");

          /* Note: the tree is created using this:
             TheTree = openfluid::core::TreeValue();
             TheTree.addChild("x").addChild("x1").addChild("x2",2);
             TheTree.child("x").child("x1").addChild("x22",22);
             TheTree.addChild("y").addChild("y1").addChild("y2",OPENFLUID_GetCurrentTimeIndex());
           */

          if (!openfluid::scientific::isCloseEnough(VarTreeVal.child("y").child("y1").child("y2").getValue(),
                                                    double(OPENFLUID_GetCurrentTimeIndex()),0.01))
            OPENFLUID_RaiseError("incorrect typed tree value at y.y1.y2");

          openfluid::core::TreeValue NewTree;
          NewTree.addChild("a").addChild("b",20.0).addChild("c",30.0).addChild("d",40.0);
          OPENFLUID_SetVariable(TU,"tests.typed.tree",NewTree);
          OPENFLUID_GetVariable(TU,"tests.typed.tree",CurrIndex,VarTreeVal);

          if (VarTreeVal.size() != 5)
            OPENFLUID_RaiseError("incorrect typed tree size get by reference");

          if (VarTreeVal.child("a").hasValue())
            OPENFLUID_RaiseError("incorrect typed tree value existence at a");

          if (!openfluid::scientific::isCloseEnough(VarTreeVal.child("a").child("b").getValue(),
                                                    20.0,0.01))
            OPENFLUID_RaiseError("incorrect typed tree value at b");

          if (!openfluid::scientific::isCloseEnough(VarTreeVal.child("a").child("b").getChildValue("c",0.0),
                                                    30.0,0.01))
            OPENFLUID_RaiseError("incorrect typed tree value at c");

          VarTreeVal.clear();
          if (VarTreeVal.size() != 1)
            OPENFLUID_RaiseError("incorrect typed tree after clear");

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.typed.tree",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarTreeVal);
            if (VarTreeVal.size() != 5)
              OPENFLUID_RaiseError("incorrect typed tree size at t-1");
          }

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.tree"))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.typed.tree)");


          // none

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.none",openfluid::core::Value::NONE))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsTypedVariableExist (tests.none, NONE)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.none, DOUBLE)");

          VarDouble = 0.0;
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarDouble);
          if (!openfluid::scientific::isCloseEnough(VarDouble,RefDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.none, get by reference)");

          VarDouble = OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex)->asDoubleValue();
          if (!openfluid::scientific::isCloseEnough(VarDouble,RefDouble,0.00001))
            OPENFLUID_RaiseError("incorrect double value (tests.none, get by return)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",NewLong);

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::INTEGER))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.none, INTEGER)");

          VarLong = 0;
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarLong);
          if (VarLong != NewLong)
            OPENFLUID_RaiseError("incorrect long value (tests.none)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",openfluid::core::NullValue());

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::NULLL))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.none, nullptr)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",NewBool);

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::BOOLEAN))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.none, BOOLEAN)");

          VarBool = false;
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarBool);
          if (VarBool != NewBool)
            OPENFLUID_RaiseError("incorrect boolean value (tests.none)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",NewString);

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::STRING))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.none, STRING)");

          VarString= "";
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarString);
          if (VarString != NewString)
            OPENFLUID_RaiseError("incorrect string value (tests.none)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",NewVect);

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.none, VECTOR)");

          VarVectorVal.clear();
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarVectorVal);
          if (VarVectorVal.size() != NewVect.size())
            OPENFLUID_RaiseError("incorrect vector size value (tests.none)");
          if (!openfluid::scientific::isCloseEnough(VarVectorVal[4],NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect vector value at index 4 (tests.none)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",NewMatrix);

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::MATRIX))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.none, MATRIX)");

          VarMatrixVal.clear();
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarMatrixVal);
          if (VarMatrixVal.size() != NewMatrix.size())
            OPENFLUID_RaiseError("incorrect matrix size value (tests.none)");
          if (!openfluid::scientific::isCloseEnough(VarMatrixVal.get(4,3),NewDouble,0.00001))
            OPENFLUID_RaiseError("incorrect matrix value at index 4,3 (tests.none)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",NewMap);

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::MAP))
            OPENFLUID_RaiseError("incorrect OPENFLUID_IsVariableExist (tests.none, MAP)");

          VarMapVal.clear();
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarMapVal);
          if (VarMapVal.size() != NewMap.size())
            OPENFLUID_RaiseError("incorrect map size value (tests.none)");
          if (VarMapVal.getBoolean("key3") != NewBool)
            OPENFLUID_RaiseError("incorrect map value at key key3 (tests.none)");
        }
      }


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

DEFINE_SIMULATOR_CLASS(VarsPrimitivesUseSimulator)

