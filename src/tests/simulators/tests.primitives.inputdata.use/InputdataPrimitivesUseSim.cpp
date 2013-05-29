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

#include <glibmm/ustring.h>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN


// =====================================================================
// =====================================================================


#define THIS_SIM_ID "tests.primitives.inputdata.use"

BEGIN_SIMULATOR_SIGNATURE(THIS_SIM_ID)

  DECLARE_NAME("test simulator for inputdata primitives");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");


  DECLARE_REQUIRED_INPUTDATA("indataDouble","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataDouble2","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataDouble3","TestUnits","","")

  DECLARE_REQUIRED_INPUTDATA("indataLong","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataLong2","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataLong3","TestUnits","","")

  DECLARE_REQUIRED_INPUTDATA("indataBool","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataBool2","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataBool3","TestUnits","","")

  DECLARE_REQUIRED_INPUTDATA("indataString","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataString2","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataString3","TestUnits","","")

  DECLARE_REQUIRED_INPUTDATA("indataVector","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataVector2","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataVector3","TestUnits","","")

  DECLARE_REQUIRED_INPUTDATA("indataMatrix","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataMatrix2","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataMatrix3","TestUnits","","")

  DECLARE_REQUIRED_INPUTDATA("indataMap","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataMap2","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataMap3","TestUnits","","")

  DECLARE_REQUIRED_INPUTDATA("indataNull2","TestUnits","","")
  DECLARE_REQUIRED_INPUTDATA("indataNull3","TestUnits","","")

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class InputdataPrimitivesUseSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


    InputdataPrimitivesUseSimulator() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~InputdataPrimitivesUseSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {

    }


    // =====================================================================
    // =====================================================================


    void prepareData()
    {
      openfluid::core::Unit* TU;

      openfluid::core::DoubleValue VarDoubleVal;
      openfluid::core::VectorValue VarVectorVal;

      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
      {
        if (!OPENFLUID_IsInputDataExist(TU,"indataDouble"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataDouble)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataVector"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataDouble)");

        VarDoubleVal.set(0.0);
        OPENFLUID_GetInputData(TU,"indataDouble",VarDoubleVal);
        if (!openfluid::tools::IsVeryClose(VarDoubleVal.get(),1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue) get with DoubleValue by reference");

        VarVectorVal.clear();
        OPENFLUID_GetInputData(TU,"indataDouble",VarVectorVal);
        if (VarVectorVal.size() != 1)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrong size) get with VectorValue");
        if (!openfluid::tools::IsVeryClose(VarVectorVal[0],1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue) get with VectorValue");
      }

    }


    // =====================================================================
    // =====================================================================


    void checkConsistency() { }


    // =====================================================================
    // =====================================================================

    void checkInputdata()
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


      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
      {

        TUID = TU->getID();
        CurrIndex = OPENFLUID_GetCurrentTimeIndex();

        RefDouble = (double)TUID/10;
        RefLong = TUID;
        RefBool = (TUID%2 == 0);
        RefString = Glib::ustring::compose("ID %1",TUID);


        if (!OPENFLUID_IsInputDataExist(TU,"indataDouble"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataDouble)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataDouble2"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataDouble2)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataDouble3"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataDouble3)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataLong"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataLong)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataLong2"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataLong2)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataLong3"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataLong3)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataBool"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataBool)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataBool2"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataBool2)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataBool3"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataBool3)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataString"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataString)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataString2"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataString2)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataString3"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataString3)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataVector"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataVector)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataVector2"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataVector2)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataVector3"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataVector3)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataMatrix"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataMatrix)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataMatrix2"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataMatrix2)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataMatrix3"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataMatrix3)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataMap"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataMap)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataMap2"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataMap2)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataMap3"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataMap3)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataNull2"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataNull2)");

        if (!OPENFLUID_IsInputDataExist(TU,"indataNull3"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataNull3)");

        if (OPENFLUID_IsInputDataExist(TU,"wrongdata"))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (wrongdata)");


        // double

        VarDouble = 0.0;
        OPENFLUID_GetInputData(TU,"indataDouble",VarDouble);
        if (!openfluid::tools::IsVeryClose(VarDouble,1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue) get by reference");

        VarDoubleVal.set(0.0);
        OPENFLUID_GetInputData(TU,"indataDouble",VarDoubleVal);
        if (!openfluid::tools::IsVeryClose(VarDoubleVal.get(),1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue) get with DoubleValue by reference");

        VarVectorVal.clear();
        OPENFLUID_GetInputData(TU,"indataDouble",VarVectorVal);
        if (VarVectorVal.size() != 1)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrong size) get with VectorValue");
        if (!openfluid::tools::IsVeryClose(VarVectorVal[0],1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue) get with VectorValue");

        VarMatrixVal.clear();
        OPENFLUID_GetInputData(TU,"indataDouble",VarMatrixVal);
        if (VarMatrixVal.size() != 1)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrong size) get with MatrixValue by reference");
        if (!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue) get with MatrixValue by reference");

        VarStringVal.set("");
        VarDouble = 0.0;
        OPENFLUID_GetInputData(TU,"indataDouble",VarStringVal);
        if(!VarStringVal.toDouble(VarDouble))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrong type) get with StringValue by reference");
        if (!openfluid::tools::IsVeryClose(VarDouble,1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue) get with StringValue by reference");

        VarDouble = 0.0;
        OPENFLUID_GetInputData(TU,"indataDouble2",VarDouble);
        if (!openfluid::tools::IsVeryClose(VarDouble,RefDouble))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble2 wrongvalue)");

        VarStringVal.set("");
        VarDouble = 0.0;
        OPENFLUID_GetInputData(TU,"indataDouble2",VarStringVal);
        if(!VarStringVal.toDouble(VarDouble))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble2 wrong type) get with StringValue by reference");
        if (!openfluid::tools::IsVeryClose(VarDouble,RefDouble))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble2 wrongvalue) get with StringValue by reference");

        VarDouble = 0.0;
        OPENFLUID_GetInputData(TU,"indataDouble3",VarDouble);
        if (!openfluid::tools::IsVeryClose(VarDouble,RefDouble))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble3 wrongvalue)");

        VarStringVal.set("");
        VarDouble = 0.0;
        OPENFLUID_GetInputData(TU,"indataDouble3",VarStringVal);
        if(!VarStringVal.toDouble(VarDouble))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble3 wrong type) get with StringValue by reference");
        if (!openfluid::tools::IsVeryClose(VarDouble,RefDouble))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble3 wrongvalue) get with StringValue by reference");


        // long

        VarLong = 0;
        OPENFLUID_GetInputData(TU,"indataLong",VarLong);
        if (VarLong != 1)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong wrongvalue) get by reference");

        VarStringVal.set("");
        VarLong = 0;
        OPENFLUID_GetInputData(TU,"indataLong",VarStringVal);
        if(!VarStringVal.toInteger(VarLong))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong wrong type) get with StringValue by reference");
        if (VarLong != 1)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong wrongvalue) get with StringValue by reference");

        VarLong = 0;
        OPENFLUID_GetInputData(TU,"indataLong2",VarLong);
        if (VarLong != RefLong)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong2 wrongvalue)");

        VarStringVal.set("");
        VarLong = 0;
        OPENFLUID_GetInputData(TU,"indataLong2",VarStringVal);
        if(!VarStringVal.toInteger(VarLong))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong2 wrong type) get with StringValue by reference");
        if (VarLong != RefLong)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong2 wrongvalue) get with StringValue by reference");

        VarLong = 0;
        OPENFLUID_GetInputData(TU,"indataLong3",VarLong);
        if (VarLong != RefLong)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong3 wrongvalue)");

        VarStringVal.set("");
        VarLong = 0;
        OPENFLUID_GetInputData(TU,"indataLong3",VarStringVal);
        if(!VarStringVal.toInteger(VarLong))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong3 wrong type) get with StringValue by reference");
        if (VarLong != RefLong)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong3 wrongvalue) get with StringValue by reference");


        // bool

        VarStringVal.set("");
        VarBool = false;
        OPENFLUID_GetInputData(TU,"indataBool",VarStringVal);
        if(!VarStringVal.toBoolean(VarBool))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataBool2 wrong type) get with StringValue by reference");
        if (VarBool != RefBool)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataBool2 wrongvalue) get with StringValue by reference");

        VarStringVal.set("");
        VarBool = false;
        OPENFLUID_GetInputData(TU,"indataBool2",VarStringVal);
        if(!VarStringVal.toBoolean(VarBool))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataBool2 wrong type) get with StringValue by reference");
        if (VarBool != RefBool)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataBool2 wrongvalue) get with StringValue by reference");

        VarStringVal.set("");
        VarBool = false;
        OPENFLUID_GetInputData(TU,"indataBool3",VarStringVal);
        if(!VarStringVal.toBoolean(VarBool))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataBool3 wrong type) get with StringValue by reference");
        if (VarBool != RefBool)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataBool3 wrongvalue) get with StringValue by reference");


        //string
        std::string RefStr;
        openfluid::tools::ConvertValue(TU->getID(),&RefStr);
        RefStr = "CODE"+RefStr;
        VarString = "";
        OPENFLUID_GetInputData(TU,"indataString",VarString);
        if (VarString != RefStr)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataString wrongvalue) get by reference");

        VarStringVal.set("");
        VarString = "";
        OPENFLUID_GetInputData(TU,"indataString",VarStringVal);
        if (VarStringVal.get() != RefStr)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataString wrongvalue) get with StringValue by reference");

        VarString = "";
        OPENFLUID_GetInputData(TU,"indataString2",VarString);
        if (VarString != RefString)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataString2 wrongvalue)");

        VarStringVal.set("");
        VarString = "";
        OPENFLUID_GetInputData(TU,"indataString2",VarStringVal);
        if (VarStringVal.get() != RefString)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataString2 wrongvalue) get with StringValue by reference");

        VarString = "";
        OPENFLUID_GetInputData(TU,"indataString3",VarString);
        if (VarString != RefString)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataString3 wrongvalue)");

        VarStringVal.set("");
        VarString = "";
        OPENFLUID_GetInputData(TU,"indataString3",VarStringVal);
        if (VarStringVal.get() != RefString)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataString3 wrongvalue) get with StringValue by reference");


        // vector

        VarStringVal.set("");
        VarVectorVal.clear();
        OPENFLUID_GetInputData(TU,"indataVector",VarStringVal);
        if(!VarStringVal.toVectorValue(";",VarVectorVal))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong type)");
        if(VarVectorVal.size() != 3)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong size)");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[0], TUID + 0.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong value)");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[2], TUID + 0.3))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong value)");

        VarStringVal.set("");
        VarMatrixVal.clear();
        OPENFLUID_GetInputData(TU,"indataVector",VarStringVal);
        if(!VarStringVal.toMatrixValue(";","|",VarMatrixVal))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong type) get with StringValue by reference");
        if(VarMatrixVal.getColsNbr() != 3)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong cols nb) get with StringValue by reference");
        if(VarMatrixVal.getRowsNbr() != 1)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong rows nb) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),TUID + 0.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(2,0),TUID + 0.3))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong value) get with StringValue by reference");

        VarStringVal.set("");
        VarVectorVal.clear();
        OPENFLUID_GetInputData(TU,"indataVector2",VarStringVal);
        if(!VarStringVal.toVectorValue(";",VarVectorVal))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong type)");
        if(VarVectorVal.size() != 3)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong size)");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[0], 1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong value)");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[2], 1.3))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong value)");

        VarMatrixVal.clear();
        OPENFLUID_GetInputData(TU,"indataVector2",VarMatrixVal);
        if (VarMatrixVal.size() != 3)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong size) get with MatrixValue by reference");
        if (!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrongvalue) get with MatrixValue by reference");
        if (!openfluid::tools::IsVeryClose(VarMatrixVal.at(2,0),1.3))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrongvalue) get with MatrixValue by reference");

        VarVectorVal.clear();
        OPENFLUID_GetInputData(TU,"indataVector2",VarVectorVal);
        if(VarVectorVal.size() != 3)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong size) get with VectorValue by reference");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[0], 1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong value) get with VectorValue by reference");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[2], 1.3))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong value) get with VectorValue by reference");

        VarStringVal.set("");
        VarVectorVal.clear();
        OPENFLUID_GetInputData(TU,"indataVector3",VarStringVal);
        if(!VarStringVal.toVectorValue(";",VarVectorVal))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector3 wrong type) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarVectorVal.size(), 3))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector3 wrong size) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[0], 1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector3 wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[2], 1.3))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector3 wrong value) get with StringValue by reference");

        // matrix

        VarStringVal.set("");
        VarMatrixVal.clear();
        OPENFLUID_GetInputData(TU,"indataMatrix",VarStringVal);
        if(!VarStringVal.toMatrixValue(";","|",VarMatrixVal))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix wrong type) get with StringValue by reference");
        if(VarMatrixVal.getColsNbr() != 3)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix wrong cols nb) get with StringValue by reference");
        if(VarMatrixVal.getRowsNbr() != 2)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix wrong rows nb) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),TUID * 10))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,1),(TUID * 10)+1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(2,1),(TUID * 10)+1.2))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix wrong value) get with StringValue by reference");

        VarStringVal.set("");
        VarMatrixVal.clear();
        OPENFLUID_GetInputData(TU,"indataMatrix2",VarStringVal);
        if(!VarStringVal.toMatrixValue(";","|",VarMatrixVal))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong type) get with StringValue by reference");
        if(VarMatrixVal.getColsNbr() != 2)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong cols nb) get with StringValue by reference");
        if(VarMatrixVal.getRowsNbr() != 3)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong rows nb) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),0.0))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,2),0.2))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(1,1),1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong value) get with StringValue by reference");

        VarMatrixVal.clear();
        OPENFLUID_GetInputData(TU,"indataMatrix2",VarMatrixVal);
        if(VarMatrixVal.getColsNbr() != 2)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong cols nb) get with MatrixValue by reference");
        if(VarMatrixVal.getRowsNbr() != 3)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong rows nb) get with MatrixValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),0.0))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrongvalue) get with MatrixValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,2),0.2))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrongvalue) get with MatrixValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(1,1),1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrongvalue) get with MatrixValue by reference");

        VarMatrixVal.clear();
        OPENFLUID_GetInputData(TU,"indataMatrix3",VarMatrixVal);
        if(VarMatrixVal.getColsNbr() != 2)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong cols nb) get with MatrixValue by reference");
        if(VarMatrixVal.getRowsNbr() != 3)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong rows nb) get with MatrixValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),0.0))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong value) get with MatrixValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,2),0.2))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong value) get with MatrixValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(1,1),1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong value) get with MatrixValue by reference");

        VarStringVal.set("");
        VarMatrixVal.clear();
        OPENFLUID_GetInputData(TU,"indataMatrix3",VarStringVal);
        if(!VarStringVal.toMatrixValue(";","|",VarMatrixVal))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong type) get with StringValue by reference");
        if(VarMatrixVal.getColsNbr() != 2)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong cols nb) get with StringValue by reference");
        if(VarMatrixVal.getRowsNbr() != 3)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong rows nb) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),0.0))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,2),0.2))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(1,1),1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong value) get with StringValue by reference");

        // map

        VarStringVal.set("");
        VarMapVal.clear();
        OPENFLUID_GetInputData(TU,"indataMap",VarStringVal);
        if(!VarStringVal.toMapValue(";",VarMapVal))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap wrong type) get with StringValue by reference");
        if(VarMapVal.getSize() != 2)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap wrong size) get with StringValue by reference");
        VarDouble = 0.0;
        if(!VarMapVal["key1"].asStringValue().toDouble(VarDouble))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap key1 wrong type) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarDouble,1.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap key1 wrong value) get with StringValue by reference");
        if(VarMapVal.getString("key2") != "aString")
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap key2 wrong value) get with StringValue by reference");

        VarStringVal.set("");
        VarMapVal.clear();
        OPENFLUID_GetInputData(TU,"indataMap2",VarStringVal);
        if(!VarStringVal.toMapValue(";",VarMapVal))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 wrong type) get with StringValue by reference");
        if(VarMapVal.getSize() != 3)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 wrong size) get with StringValue by reference");
        VarDouble = 0.0;
        if(!VarMapVal["key1"].asStringValue().toDouble(VarDouble))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 key1 wrong type) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarDouble,2.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 key1 wrong value) get with StringValue by reference");
        if(VarMapVal.getString("key2") != "a string")
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 key2 wrong value) get with StringValue by reference");
        VarBool = false;
        if(!VarMapVal["key3"].asStringValue().toBoolean(VarBool))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 key3 wrong type) get with StringValue by reference");
        if(VarBool != true)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 key3 wrong value) get with StringValue by reference");

        VarStringVal.set("");
        VarMapVal.clear();
        OPENFLUID_GetInputData(TU,"indataMap3",VarStringVal);
        if(!VarStringVal.toMapValue(";",VarMapVal))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap3 wrong type) get with StringValue by reference");
        if(VarMapVal.getSize() != 3)
          VarDouble = 0.0;
        if(!VarMapVal["key1"].asStringValue().toDouble(VarDouble))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap3 key1 wrong type) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarDouble,2.1))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap3 key1 wrong value) get with StringValue by reference");
        if(VarMapVal.getString("key2") != "a string")
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap3 key2 wrong value) get with StringValue by reference");
        VarBool = false;
        if(!VarMapVal["key3"].asStringValue().toBoolean(VarBool))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap3 key3 wrong type) get with StringValue by reference");
        if(VarBool != true)
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap3 key3 wrong value) get with StringValue by reference");


        // null

        VarStringVal.set("");
        OPENFLUID_GetInputData(TU,"indataNull2",VarStringVal);
        if(!VarStringVal.toNullValue(VarNullVal))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataNull2 wrong type)");

        VarStringVal.set("");
        OPENFLUID_GetInputData(TU,"indataNull3",VarStringVal);
        if(!VarStringVal.toNullValue(VarNullVal))
          OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataNull3 wrong type)");
      }

    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      checkInputdata();

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {

      checkInputdata();

      return Never();
    }


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};

// =====================================================================
// =====================================================================

DEFINE_SIMULATOR_CLASS(InputdataPrimitivesUseSimulator)

