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


#include <openfluid/ware/PluggableFunction.hpp>
#include <openfluid/core.hpp>

#include <glibmm/ustring.h>



// =====================================================================
// =====================================================================


DECLARE_FUNCTION_PLUGIN


// =====================================================================
// =====================================================================


#define THIS_SIM_ID "tests.primitives.variables.use"

BEGIN_FUNCTION_SIGNATURE(THIS_SIM_ID)

  DECLARE_SIGNATURE_NAME("test function for variables primitives");
  DECLARE_SIGNATURE_DESCRIPTION("");

  DECLARE_SIGNATURE_VERSION("1.0");
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN("");
  DECLARE_SIGNATURE_PROCESS("");
  DECLARE_SIGNATURE_METHOD("");
  DECLARE_SIGNATURE_AUTHORNAME("");
  DECLARE_SIGNATURE_AUTHOREMAIL("");

  DECLARE_FUNCTION_PARAM("strparam","=strvalue","-");
  DECLARE_FUNCTION_PARAM("doubleparam","=0.1","-");
  DECLARE_FUNCTION_PARAM("longparam","=10","-");

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
  DECLARE_REQUIRED_VAR("tests.matrix","TestUnits","matrix for tests","");
  DECLARE_REQUIRED_VAR("tests.map","TestUnits","map for tests","");

  DECLARE_REQUIRED_VAR("tests.typed.none","TestUnits","untyped for tests","");
  DECLARE_REQUIRED_VAR("tests.typed.double[double]","TestUnits","double for tests","");
  DECLARE_REQUIRED_VAR("tests.typed.integer[integer]","TestUnits","integer for tests","");
  DECLARE_REQUIRED_VAR("tests.typed.bool[boolean]","TestUnits","boolean for tests","");
  DECLARE_REQUIRED_VAR("tests.typed.string[string]","TestUnits","string for tests","");
  DECLARE_REQUIRED_VAR("tests.typed.vector[vector]","TestUnits","vector for tests","");
  DECLARE_REQUIRED_VAR("tests.typed.oldvector[]","TestUnits","old style vector for tests","");
  DECLARE_REQUIRED_VAR("tests.typed.matrix[matrix]","TestUnits","matrix for tests","");
  DECLARE_REQUIRED_VAR("tests.typed.map[map]","TestUnits","map for tests","");

END_FUNCTION_SIGNATURE


// =====================================================================
// =====================================================================


class VarsPrimitivesUseFunction : public openfluid::ware::PluggableFunction
{
  private:

    double m_ParamDouble;

    long m_ParamLong;

    std::string m_ParamString;

    unsigned long m_ExpectedValCount;

  public:


    VarsPrimitivesUseFunction() : PluggableFunction(),
    m_ParamDouble(0.1), m_ParamLong(10), m_ParamString("strvalue"), m_ExpectedValCount(0)
    {


    }


    // =====================================================================
    // =====================================================================


    ~VarsPrimitivesUseFunction()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      OPENFLUID_GetFunctionParameter(Params,"strparam",m_ParamString);
      OPENFLUID_GetFunctionParameter(Params,"doubleparam",m_ParamDouble);
      OPENFLUID_GetFunctionParameter(Params,"longparam",m_ParamLong);

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
        openfluid::core::Unit* TU;
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
          RefString = Glib::ustring::compose("ID %1",TUID);

          PreDouble = TUID*(CurrIndex-OPENFLUID_GetDefaultDeltaT())*m_ParamDouble;
          PreLong = TUID*(CurrIndex-OPENFLUID_GetDefaultDeltaT())*m_ParamLong;
          PreBool = ((TUID*(CurrIndex-OPENFLUID_GetDefaultDeltaT()))%2 == 0);
          PreString = Glib::ustring::compose("%1 %2x%3",m_ParamString,TUID,(CurrIndex-OPENFLUID_GetDefaultDeltaT()));

          NewDouble = TUID*CurrIndex*m_ParamDouble;
          NewLong = TUID*CurrIndex*m_ParamLong;
          NewBool = ((TUID*CurrIndex)%2 == 0);
          NewString = Glib::ustring::compose("%1 %2x%3",m_ParamString,TUID,CurrIndex);


          // ====== Variables ======


          // double

          OPENFLUID_GetVariable(TU,"tests.double",CurrIndex,VarDouble);
          if (!openfluid::tools::IsCloseEnough(VarDouble,RefDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value (tests.double)");

          VarDouble = 0.0;
          OPENFLUID_GetVariable(TU,"tests.double",VarDouble);
          if (!openfluid::tools::IsCloseEnough(VarDouble,RefDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value (tests.double) get by reference without index");

          VarDouble = 0.0;
          OPENFLUID_GetLatestVariable(TU,"tests.double",IndValue);
          if (IndValue.getIndex() != OPENFLUID_GetCurrentTimeIndex())
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect time index (tests.double) get by latest variable");

          if (!openfluid::tools::IsCloseEnough(IndValue.getValue()->asDoubleValue(),RefDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value (tests.double) get by latest variable");

          VarDouble = 0.0;
          OPENFLUID_GetLatestVariables(TU,"tests.double",OPENFLUID_GetCurrentTimeIndex()-1,IndValueList);
          if (!openfluid::tools::IsCloseEnough(IndValueList.back().getValue()->asDoubleValue(),RefDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value (tests.double) get by reference");


          OPENFLUID_SetVariable(TU,"tests.double",NewDouble);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.double",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarDouble);
            if (!openfluid::tools::IsCloseEnough(VarDouble,PreDouble,0.00001))
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value at t-1 (tests.double)");
          }

          OPENFLUID_GetVariable(TU,"tests.double",CurrIndex,VarDouble);

          if (!openfluid::tools::IsCloseEnough(VarDouble,NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value after update (tests.double)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.double"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.double)");


          // double value

          OPENFLUID_GetVariable(TU,"tests.doubleval",VarDoubleVal);
          if (VarDoubleVal.get() != RefDouble)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value (tests.doubleval) get by reference without index");

          VarDoubleVal.set(0.0);
          OPENFLUID_GetVariable(TU,"tests.doubleval",CurrIndex,VarDoubleVal);
          if (VarDoubleVal.get() != RefDouble)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value (tests.doubleval) get by reference");

          OPENFLUID_SetVariable(TU,"tests.doubleval",NewDouble);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.doubleval",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarDoubleVal);
            if (!openfluid::tools::IsCloseEnough(VarDoubleVal.get(),PreDouble,0.00001))
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value at t-1 (tests.doubleval)");
          }

          OPENFLUID_GetVariable(TU,"tests.doubleval",CurrIndex,VarDoubleVal);

          if (!openfluid::tools::IsCloseEnough(VarDoubleVal.get(),NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value after update (tests.doubleval)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.doubleval"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.doubleval)");


          // long

          VarLong = 0;
          OPENFLUID_GetVariable(TU,"tests.integer",CurrIndex,VarLong);
          if (VarLong != RefLong)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect long value (tests.integer) get by reference");

          OPENFLUID_SetVariable(TU,"tests.integer",NewLong);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.integer",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarLong);
            if (VarLong != PreLong)
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect long value at t-1 (tests.integer)");
          }

          OPENFLUID_GetVariable(TU,"tests.integer",CurrIndex,VarLong);

          if (VarLong != NewLong)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect long value after update (tests.integer)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.integer"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integer)");


          // long value

          VarLongVal.set(0);
          OPENFLUID_GetVariable(TU,"tests.integerval",CurrIndex,VarLongVal);
          if (VarLongVal.get() != RefLong)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect long value (tests.integerval) get by reference");
          OPENFLUID_SetVariable(TU,"tests.integerval",NewLong);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.integerval",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarLongVal);
            if (VarLongVal.get() != PreLong)
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect long value at t-1 (tests.integerval)");
          }

          OPENFLUID_GetVariable(TU,"tests.integerval",CurrIndex,VarLongVal);

          if (VarLongVal.get() != NewLong)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect long value after update (tests.integerval)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.integerval"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.integerval)");

          // values count since beginning of simulation
          OPENFLUID_GetVariables(TU,"tests.integer",0,CurrIndex-1,IndValueList);
          if (IndValueList.size() != (m_ExpectedValCount-1))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect long value list between t0 and current time index -1 (tests.integer) by variables");

          OPENFLUID_GetLatestVariables(TU,"tests.integer",0,IndValueList);
          if (IndValueList.size() != m_ExpectedValCount)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect long value list since t0 (tests.integer) by latest variables");

          OPENFLUID_GetVariables(TU,"tests.integer",0,CurrIndex,IndValueList);
          if (IndValueList.size() != m_ExpectedValCount)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect long value list between t0 and current time index (tests.integer) by variables");


          // bool

          VarBool = false;
          OPENFLUID_GetVariable(TU,"tests.bool",CurrIndex,VarBool);
          if (VarBool != RefBool)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect bool value (tests.bool) get by reference");

          OPENFLUID_SetVariable(TU,"tests.bool",NewBool);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.bool",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarBool);
            if (VarBool != PreBool)
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect bool value at t-1 (tests.bool)");
          }

          OPENFLUID_GetVariable(TU,"tests.bool",CurrIndex,VarBool);
          if (VarBool != NewBool)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect bool value after update (tests.bool)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.bool"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.bool)");


          // bool value

          VarBoolVal.set(false);
          OPENFLUID_GetVariable(TU,"tests.boolval",CurrIndex,VarBoolVal);
          if (VarBoolVal.get() != RefBool)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect bool value (tests.boolval) get by reference");

          OPENFLUID_SetVariable(TU,"tests.boolval",NewBool);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.boolval",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarBoolVal);
            if (VarBoolVal.get() != PreBool)
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect bool value at t-1 (tests.boolval)");
          }

          OPENFLUID_GetVariable(TU,"tests.boolval",CurrIndex,VarBoolVal);

          if (VarBoolVal.get() != NewBool)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect bool value after update (tests.boolval)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.boolval"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.boolval)");


          // string

          VarString = "";
          OPENFLUID_GetVariable(TU,"tests.string",CurrIndex,VarString);
          if (VarString != RefString)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect string value (tests.string) get by reference");

          OPENFLUID_SetVariable(TU,"tests.string",NewString);

          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.string",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarString);
            if (VarString != PreString)
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect string value at t-1 (tests.string)");
          }

          OPENFLUID_GetVariable(TU,"tests.string",CurrIndex,VarString);

          if (VarString != NewString)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect string value after update (tests.string)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.string"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.string)");


          // null value

          OPENFLUID_GetVariable(TU,"tests.null",CurrIndex,VarNullVal);
          if (VarNullVal.getType() != openfluid::core::Value::NULLL)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect null value (tests.null) get by reference");


          // vector value

          VarVectorVal.clear();
          OPENFLUID_GetVariable(TU,"tests.vector",CurrIndex,VarVectorVal);
          if (VarVectorVal.getSize() != RefVectorSize)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector size get by reference");

          openfluid::core::VectorValue NewVect(NewVectorSize,NewDouble);
          OPENFLUID_SetVariable(TU,"tests.vector",NewVect);

          OPENFLUID_GetVariable(TU,"tests.vector",CurrIndex,VarVectorVal);
          if (VarVectorVal.getSize() != NewVectorSize)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector size after update");

          if (!openfluid::tools::IsCloseEnough(VarVectorVal[0],NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector value at index 0 after update");

          if (!openfluid::tools::IsCloseEnough(VarVectorVal[4],NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector value at index 4 after update");


          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.vector",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarVectorVal);

            if (VarVectorVal.getSize() != NewVectorSize)
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector size at t-1");

            if (!openfluid::tools::IsCloseEnough(VarVectorVal[0],PreDouble,0.00001))
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector value at index 0 at t-1");

            if (!openfluid::tools::IsCloseEnough(VarVectorVal[4],PreDouble,0.00001))
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector value at index 4 at t-1");

          }

          if (!OPENFLUID_IsVariableExist(TU,"tests.vector"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.vector)");


          // matrix value

          VarMatrixVal.clear();
          OPENFLUID_GetVariable(TU,"tests.matrix",CurrIndex,VarMatrixVal);
          if (VarMatrixVal.getColsNbr() != RefMatrixColsNb)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix cols nb get by reference");
          if (VarMatrixVal.getRowsNbr() != RefMatrixRowsNb)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix rows nb get by reference");

          VarMatrixVal.clear();
          OPENFLUID_GetVariable(TU,"tests.matrix",VarMatrixVal);
          if (VarMatrixVal.getColsNbr() != RefMatrixColsNb)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix cols nb get by reference without index");
          if (VarMatrixVal.getRowsNbr() != RefMatrixRowsNb)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix rows nb get by reference without index");

          openfluid::core::MatrixValue NewMatrix(NewMatrixColsNb,NewMatrixRowsNb,NewDouble);
          OPENFLUID_SetVariable(TU,"tests.matrix",NewMatrix);

          OPENFLUID_GetVariable(TU,"tests.matrix",CurrIndex,VarMatrixVal);
          if (VarMatrixVal.getColsNbr() != NewMatrixColsNb)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix cols nb after update");
          if (VarMatrixVal.getRowsNbr() != NewMatrixRowsNb)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix rows nb after update");

          if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(0,0),NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix value at index 0,0 after update");

          if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(4,3),NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix value at index 4,3 after update");


          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.matrix",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarMatrixVal);

            if (VarMatrixVal.getSize() != NewMatrixRowsNb*NewMatrixColsNb)
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix size at t-1");

            if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(0,0),PreDouble,0.00001))
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix value at index 0,0 at t-1");

            if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(4,3),PreDouble,0.00001))
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix value at index 4,3 at t-1");

          }

          if (!OPENFLUID_IsVariableExist(TU,"tests.matrix"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.matrix)");


          // map value

          VarMapVal.clear();
          OPENFLUID_GetVariable(TU,"tests.map",CurrIndex,VarMapVal);
          if (VarMapVal.getSize() != 2)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map size get by reference");

          openfluid::core::MapValue NewMap;
          NewMap.setString("key1",NewString);
          NewMap.setDouble("key2",NewDouble);
          NewMap.setBoolean("key3",NewBool);
          OPENFLUID_SetVariable(TU,"tests.map",NewMap);

          OPENFLUID_GetVariable(TU,"tests.map",CurrIndex,VarMapVal);
          if (VarMapVal.getSize() != 3)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map size after update");

          if (VarMapVal.getString("key1") != NewString)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map value at key key1 after update");

          if (!openfluid::tools::IsCloseEnough(VarMapVal.getDouble("key2"),NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map value at key key2 after update");

          if (VarMapVal.getBoolean("key3") != NewBool)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map value at key key3 after update");


          if (CurrIndex > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
          {
            OPENFLUID_GetVariable(TU,"tests.map",CurrIndex-OPENFLUID_GetDefaultDeltaT(),VarMapVal);

            if (VarMapVal.getSize() != 3)
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map size at t-1");

            if (VarMapVal.getString("key1") != PreString)
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map value at key key1 at t-1");

            if (!openfluid::tools::IsCloseEnough(VarMapVal.getDouble("key2"),PreDouble,0.00001))
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map value at key key2 at t-1");

            if (VarMapVal.getBoolean("key3") != PreBool)
              OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map value at key key3 at t-1");

          }

          if (!OPENFLUID_IsVariableExist(TU,"tests.map"))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.map)");
        }
      }



      // typed
      {
        openfluid::core::Unit* TU;
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
          RefString = Glib::ustring::compose("ID %1",TUID);

          NewDouble = TUID*CurrIndex*0.1;
          NewLong = TUID*CurrIndex*10;
          NewBool = ((TUID*CurrIndex)%2 == 0);
          NewString = Glib::ustring::compose("%1 %2x%3","strvalue",TUID,CurrIndex);



          // double

          OPENFLUID_GetVariable(TU,"tests.typed.double",CurrIndex,VarDouble);
          if (!openfluid::tools::IsCloseEnough(VarDouble,RefDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value (tests.double) get by reference");

          OPENFLUID_GetLatestVariable(TU,"tests.typed.double",IndValue);
          if (!openfluid::tools::IsCloseEnough(IndValue.getValue()->asDoubleValue(),RefDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value (tests.double) get by latest variable");


          OPENFLUID_SetVariable(TU,"tests.typed.double",NewDouble);

          OPENFLUID_GetVariable(TU,"tests.typed.double",CurrIndex,VarDouble);
          if (!openfluid::tools::IsCloseEnough(VarDouble,NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value after update (tests.double)");

          OPENFLUID_GetLatestVariables(TU,"tests.typed.double",OPENFLUID_GetCurrentTimeIndex()-1,IndValueList);
          if (!openfluid::tools::IsCloseEnough(IndValueList.back().getValue()->asDoubleValue(),NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value (tests.double) get by latest variable");


          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.double",openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.double, DOUBLE)");


          // long

          OPENFLUID_GetVariable(TU,"tests.typed.integer",CurrIndex,VarLong);
          if (VarLong != RefLong)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect long value (tests.integer) get by reference");

          OPENFLUID_SetVariable(TU,"tests.typed.integer",NewLong);

          OPENFLUID_GetVariable(TU,"tests.typed.integer",CurrIndex,VarLong);
          if (VarLong != NewLong)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect long value after update (tests.integer)");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.integer",openfluid::core::Value::INTEGER))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.integer, INTEGER)");


          // bool

          OPENFLUID_GetVariable(TU,"tests.typed.bool",CurrIndex,VarBool);
          if (VarBool != RefBool)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect bool value (tests.bool) get by reference");

          OPENFLUID_SetVariable(TU,"tests.typed.bool",NewBool);

          OPENFLUID_GetVariable(TU,"tests.typed.bool",CurrIndex,VarBool);
          if (VarBool != NewBool)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect bool value after update (tests.bool)");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.bool",openfluid::core::Value::BOOLEAN))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.bool, BOOLEAN)");


          // string

          OPENFLUID_GetVariable(TU,"tests.typed.string",CurrIndex,VarString);
          if (VarString != RefString)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect string value (tests.string) get by reference");

          OPENFLUID_SetVariable(TU,"tests.typed.string",NewString);

          OPENFLUID_GetVariable(TU,"tests.typed.string",CurrIndex,VarString);
          if (VarString != NewString)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect string value after update (tests.string)");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.string",openfluid::core::Value::STRING))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.string, STRING)");


          // vector

          OPENFLUID_GetVariable(TU,"tests.typed.vector",CurrIndex,VarVectorVal);
          if (VarVectorVal.getSize() != RefVectorSize)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector size get by reference");

          OPENFLUID_GetVariable(TU,"tests.typed.vector",VarVectorVal);
          if (VarVectorVal.getSize() != RefVectorSize)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector size get by reference");


          OPENFLUID_GetLatestVariable(TU,"tests.typed.vector",IndValue);
          if (IndValue.getValue()->asVectorValue().getSize() != RefVectorSize)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector size get by latest variable");


          openfluid::core::VectorValue NewVect(NewVectorSize,NewDouble);
          OPENFLUID_SetVariable(TU,"tests.typed.vector",NewVect);

          OPENFLUID_GetVariable(TU,"tests.typed.vector",CurrIndex,VarVectorVal);
          if (VarVectorVal.getSize() != NewVectorSize)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector size after update");
          if (!openfluid::tools::IsCloseEnough(VarVectorVal[0],NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector value at index 0 after update");
          if (!openfluid::tools::IsCloseEnough(VarVectorVal[4],NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector value at index 4 after update");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.vector",openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.vector, VECTOR)");


          // old style vector

          VarVectorVal.clear();
          OPENFLUID_GetVariable(TU,"tests.typed.oldvector",CurrIndex,VarVectorVal);
          if (VarVectorVal.getSize() != RefVectorSize)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect oldvector size get by reference");

          OPENFLUID_SetVariable(TU,"tests.typed.oldvector",NewVect);

          OPENFLUID_GetVariable(TU,"tests.typed.oldvector",CurrIndex,VarVectorVal);
          if (VarVectorVal.getSize() != NewVectorSize)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect oldvector size after update");
          if (!openfluid::tools::IsCloseEnough(VarVectorVal[0],NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect oldvector value at index 0 after update");
          if (!openfluid::tools::IsCloseEnough(VarVectorVal[4],NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect oldvector value at index 4 after update");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.oldvector",openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.oldvector, VECTOR)");


          // matrix

          OPENFLUID_GetVariable(TU,"tests.typed.matrix",CurrIndex,VarMatrixVal);
          if (VarMatrixVal.getColsNbr() != RefMatrixColsNb)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix cols nb get by reference");
          if (VarMatrixVal.getRowsNbr() != RefMatrixRowsNb)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix rows nb get by reference");

          openfluid::core::MatrixValue NewMatrix(NewMatrixColsNb,NewMatrixRowsNb,NewDouble);
          OPENFLUID_SetVariable(TU,"tests.typed.matrix",NewMatrix);

          OPENFLUID_GetVariable(TU,"tests.typed.matrix",CurrIndex,VarMatrixVal);
          if (VarMatrixVal.getColsNbr() != NewMatrixColsNb)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix cols nb after update");
          if (VarMatrixVal.getRowsNbr() != NewMatrixRowsNb)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix rows nb after update");
          if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(0,0),NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix value at index 0,0 after update");
          if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(4,3),NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix value at index 4,3 after update");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.matrix",openfluid::core::Value::MATRIX))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.matrix, MATRIX)");


          // map

          OPENFLUID_GetVariable(TU,"tests.typed.map",CurrIndex,VarMapVal);
          if (VarMapVal.getSize() != 2)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map size get by reference");

          openfluid::core::MapValue NewMap;
          NewMap.setString("key1",NewString);
          NewMap.setDouble("key2",NewDouble);
          NewMap.setBoolean("key3",NewBool);
          OPENFLUID_SetVariable(TU,"tests.typed.map",NewMap);

          OPENFLUID_GetVariable(TU,"tests.typed.map",CurrIndex,VarMapVal);
          if (VarMapVal.getSize() != 3)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map size after update");
          if (VarMapVal.getString("key1") != NewString)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map value at key key1 after update");
          if (!openfluid::tools::IsCloseEnough(VarMapVal.getDouble("key2"),NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map value at key key2 after update");
          if (VarMapVal.getBoolean("key3") != NewBool)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map value at key key3 after update");

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.map",openfluid::core::Value::MAP))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.map, MAP)");


          // none

          if (!OPENFLUID_IsTypedVariableExist(TU,"tests.typed.none",openfluid::core::Value::NONE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsTypedVariableExist (tests.none, NONE)");


          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::DOUBLE))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.none, DOUBLE)");

          VarDouble = 0.0;
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarDouble);
          if (!openfluid::tools::IsCloseEnough(VarDouble,RefDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect double value (tests.none)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",NewLong);

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::INTEGER))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.none, INTEGER)");

          VarLong = 0;
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarLong);
          if (VarLong != NewLong)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect long value (tests.none)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",openfluid::core::NullValue());

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::NULLL))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.none, NULL)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",NewBool);

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::BOOLEAN))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.none, BOOLEAN)");

          VarBool = false;
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarBool);
          if (VarBool != NewBool)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect boolean value (tests.none)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",NewString);

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::STRING))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.none, STRING)");

          VarString= "";
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarString);
          if (VarString != NewString)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect string value (tests.none)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",NewVect);

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::VECTOR))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.none, VECTOR)");

          VarVectorVal.clear();
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarVectorVal);
          if (VarVectorVal.size() != NewVect.size())
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector size value (tests.none)");
          if (!openfluid::tools::IsCloseEnough(VarVectorVal[4],NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect vector value at index 4 (tests.none)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",NewMatrix);

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::MATRIX))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.none, MATRIX)");

          VarMatrixVal.clear();
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarMatrixVal);
          if (VarMatrixVal.size() != NewMatrix.size())
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix size value (tests.none)");
          if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(4,3),NewDouble,0.00001))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect matrix value at index 4,3 (tests.none)");


          OPENFLUID_SetVariable(TU,"tests.typed.none",NewMap);

          if (!OPENFLUID_IsVariableExist(TU,"tests.typed.none",CurrIndex,openfluid::core::Value::MAP))
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect OPENFLUID_IsVariableExist (tests.none, MAP)");

          VarMapVal.clear();
          OPENFLUID_GetVariable(TU,"tests.typed.none",CurrIndex,VarMapVal);
          if (VarMapVal.size() != NewMap.size())
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map size value (tests.none)");
          if (VarMapVal.getBoolean("key3") != NewBool)
            OPENFLUID_RaiseError(THIS_SIM_ID,"incorrect map value at key key3 (tests.none)");
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

DEFINE_FUNCTION_CLASS(VarsPrimitivesUseFunction)

