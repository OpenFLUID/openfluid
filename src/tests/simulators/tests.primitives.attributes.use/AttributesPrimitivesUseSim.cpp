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



#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/core.hpp>

#include <QString>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN


// =====================================================================
// =====================================================================


#define THIS_SIM_ID "tests.primitives.attributes.use"

BEGIN_SIMULATOR_SIGNATURE(THIS_SIM_ID)

  DECLARE_NAME("test simulator for attributes primitives");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");


  DECLARE_REQUIRED_ATTRIBUTE("indataDouble","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataDouble2","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataDouble3","TestUnits","","")

  DECLARE_REQUIRED_ATTRIBUTE("indataLong","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataLong2","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataLong3","TestUnits","","")

  DECLARE_REQUIRED_ATTRIBUTE("indataBool","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataBool2","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataBool3","TestUnits","","")

  DECLARE_REQUIRED_ATTRIBUTE("indataString","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataString2","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataString3","TestUnits","","")

  DECLARE_REQUIRED_ATTRIBUTE("indataVector","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataVector2","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataVector3","TestUnits","","")

  DECLARE_REQUIRED_ATTRIBUTE("indataMatrix","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataMatrix2","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataMatrix3","TestUnits","","")

  DECLARE_REQUIRED_ATTRIBUTE("indataMap","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataMap2","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataMap3","TestUnits","","")

  DECLARE_REQUIRED_ATTRIBUTE("indataNull2","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataNull3","TestUnits","","")

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class AttributesPrimitivesUseSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


    AttributesPrimitivesUseSimulator() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~AttributesPrimitivesUseSimulator()
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
        if (!OPENFLUID_IsAttributeExist(TU,"indataDouble"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataDouble)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataVector"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataDouble)");

        VarDoubleVal.set(0.0);
        OPENFLUID_GetAttribute(TU,"indataDouble",VarDoubleVal);
        if (!openfluid::tools::IsVeryClose(VarDoubleVal.get(),1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble wrongvalue) get with DoubleValue by reference");

        VarVectorVal.clear();
        OPENFLUID_GetAttribute(TU,"indataDouble",VarVectorVal);
        if (VarVectorVal.size() != 1)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble wrong size) get with VectorValue");
        if (!openfluid::tools::IsVeryClose(VarVectorVal[0],1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble wrongvalue) get with VectorValue");
      }

    }


    // =====================================================================
    // =====================================================================


    void checkConsistency() { }


    // =====================================================================
    // =====================================================================

    void checkAttributes()
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
        RefString = QString("ID %1").arg(TUID).toStdString();


        if (!OPENFLUID_IsAttributeExist(TU,"indataDouble"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataDouble)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataDouble2"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataDouble2)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataDouble3"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataDouble3)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataLong"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataLong)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataLong2"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataLong2)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataLong3"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataLong3)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataBool"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataBool)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataBool2"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataBool2)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataBool3"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataBool3)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataString"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataString)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataString2"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataString2)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataString3"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataString3)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataVector"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataVector)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataVector2"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataVector2)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataVector3"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataVector3)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataMatrix"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataMatrix)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataMatrix2"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataMatrix2)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataMatrix3"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataMatrix3)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataMap"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataMap)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataMap2"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataMap2)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataMap3"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataMap3)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataNull2"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataNull2)");

        if (!OPENFLUID_IsAttributeExist(TU,"indataNull3"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (indataNull3)");

        if (OPENFLUID_IsAttributeExist(TU,"wrongdata"))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_IsAttributeExist (wrongdata)");


        // double

        VarDouble = 0.0;
        OPENFLUID_GetAttribute(TU,"indataDouble",VarDouble);
        if (!openfluid::tools::IsVeryClose(VarDouble,1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble wrongvalue) get by reference");

        VarDoubleVal.set(0.0);
        OPENFLUID_GetAttribute(TU,"indataDouble",VarDoubleVal);
        if (!openfluid::tools::IsVeryClose(VarDoubleVal.get(),1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble wrongvalue) get with DoubleValue by reference");

        VarVectorVal.clear();
        OPENFLUID_GetAttribute(TU,"indataDouble",VarVectorVal);
        if (VarVectorVal.size() != 1)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble wrong size) get with VectorValue");
        if (!openfluid::tools::IsVeryClose(VarVectorVal[0],1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble wrongvalue) get with VectorValue");

        VarMatrixVal.clear();
        OPENFLUID_GetAttribute(TU,"indataDouble",VarMatrixVal);
        if (VarMatrixVal.size() != 1)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble wrong size) get with MatrixValue by reference");
        if (!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble wrongvalue) get with MatrixValue by reference");

        VarStringVal.set("");
        VarDouble = 0.0;
        OPENFLUID_GetAttribute(TU,"indataDouble",VarStringVal);
        if(!VarStringVal.toDouble(VarDouble))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble wrong type) get with StringValue by reference");
        if (!openfluid::tools::IsVeryClose(VarDouble,1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble wrongvalue) get with StringValue by reference");

        VarDouble = 0.0;
        OPENFLUID_GetAttribute(TU,"indataDouble2",VarDouble);
        if (!openfluid::tools::IsVeryClose(VarDouble,RefDouble))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble2 wrongvalue)");

        VarStringVal.set("");
        VarDouble = 0.0;
        OPENFLUID_GetAttribute(TU,"indataDouble2",VarStringVal);
        if(!VarStringVal.toDouble(VarDouble))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble2 wrong type) get with StringValue by reference");
        if (!openfluid::tools::IsVeryClose(VarDouble,RefDouble))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble2 wrongvalue) get with StringValue by reference");

        VarDouble = 0.0;
        OPENFLUID_GetAttribute(TU,"indataDouble3",VarDouble);
        if (!openfluid::tools::IsVeryClose(VarDouble,RefDouble))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble3 wrongvalue)");

        VarStringVal.set("");
        VarDouble = 0.0;
        OPENFLUID_GetAttribute(TU,"indataDouble3",VarStringVal);
        if(!VarStringVal.toDouble(VarDouble))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble3 wrong type) get with StringValue by reference");
        if (!openfluid::tools::IsVeryClose(VarDouble,RefDouble))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataDouble3 wrongvalue) get with StringValue by reference");


        // long

        VarLong = 0;
        OPENFLUID_GetAttribute(TU,"indataLong",VarLong);
        if (VarLong != 1)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataLong wrongvalue) get by reference");

        VarStringVal.set("");
        VarLong = 0;
        OPENFLUID_GetAttribute(TU,"indataLong",VarStringVal);
        if(!VarStringVal.toInteger(VarLong))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataLong wrong type) get with StringValue by reference");
        if (VarLong != 1)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataLong wrongvalue) get with StringValue by reference");

        VarLong = 0;
        OPENFLUID_GetAttribute(TU,"indataLong2",VarLong);
        if (VarLong != RefLong)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataLong2 wrongvalue)");

        VarStringVal.set("");
        VarLong = 0;
        OPENFLUID_GetAttribute(TU,"indataLong2",VarStringVal);
        if(!VarStringVal.toInteger(VarLong))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataLong2 wrong type) get with StringValue by reference");
        if (VarLong != RefLong)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataLong2 wrongvalue) get with StringValue by reference");

        VarLong = 0;
        OPENFLUID_GetAttribute(TU,"indataLong3",VarLong);
        if (VarLong != RefLong)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataLong3 wrongvalue)");

        VarStringVal.set("");
        VarLong = 0;
        OPENFLUID_GetAttribute(TU,"indataLong3",VarStringVal);
        if(!VarStringVal.toInteger(VarLong))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataLong3 wrong type) get with StringValue by reference");
        if (VarLong != RefLong)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataLong3 wrongvalue) get with StringValue by reference");


        // bool

        VarStringVal.set("");
        VarBool = false;
        OPENFLUID_GetAttribute(TU,"indataBool",VarStringVal);
        if(!VarStringVal.toBoolean(VarBool))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataBool2 wrong type) get with StringValue by reference");
        if (VarBool != RefBool)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataBool2 wrongvalue) get with StringValue by reference");

        VarStringVal.set("");
        VarBool = false;
        OPENFLUID_GetAttribute(TU,"indataBool2",VarStringVal);
        if(!VarStringVal.toBoolean(VarBool))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataBool2 wrong type) get with StringValue by reference");
        if (VarBool != RefBool)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataBool2 wrongvalue) get with StringValue by reference");

        VarStringVal.set("");
        VarBool = false;
        OPENFLUID_GetAttribute(TU,"indataBool3",VarStringVal);
        if(!VarStringVal.toBoolean(VarBool))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataBool3 wrong type) get with StringValue by reference");
        if (VarBool != RefBool)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataBool3 wrongvalue) get with StringValue by reference");


        //string
        std::string RefStr;
        openfluid::tools::ConvertValue(TU->getID(),&RefStr);
        RefStr = "CODE"+RefStr;
        VarString = "";
        OPENFLUID_GetAttribute(TU,"indataString",VarString);
        if (VarString != RefStr)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataString wrongvalue) get by reference");

        VarStringVal.set("");
        VarString = "";
        OPENFLUID_GetAttribute(TU,"indataString",VarStringVal);
        if (VarStringVal.get() != RefStr)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataString wrongvalue) get with StringValue by reference");

        VarString = "";
        OPENFLUID_GetAttribute(TU,"indataString2",VarString);
        if (VarString != RefString)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataString2 wrongvalue)");

        VarStringVal.set("");
        VarString = "";
        OPENFLUID_GetAttribute(TU,"indataString2",VarStringVal);
        if (VarStringVal.get() != RefString)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataString2 wrongvalue) get with StringValue by reference");

        VarString = "";
        OPENFLUID_GetAttribute(TU,"indataString3",VarString);
        if (VarString != RefString)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataString3 wrongvalue)");

        VarStringVal.set("");
        VarString = "";
        OPENFLUID_GetAttribute(TU,"indataString3",VarStringVal);
        if (VarStringVal.get() != RefString)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataString3 wrongvalue) get with StringValue by reference");


        // vector

        VarStringVal.set("");
        VarVectorVal.clear();
        OPENFLUID_GetAttribute(TU,"indataVector",VarStringVal);
        if(!VarStringVal.toVectorValue(";",VarVectorVal))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector wrong type)");
        if(VarVectorVal.size() != 3)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector wrong size)");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[0], TUID + 0.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector wrong value)");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[2], TUID + 0.3))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector wrong value)");

        VarStringVal.set("");
        VarMatrixVal.clear();
        OPENFLUID_GetAttribute(TU,"indataVector",VarStringVal);
        if(!VarStringVal.toMatrixValue(";","|",VarMatrixVal))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector wrong type) get with StringValue by reference");
        if(VarMatrixVal.getColsNbr() != 3)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector wrong cols nb) get with StringValue by reference");
        if(VarMatrixVal.getRowsNbr() != 1)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector wrong rows nb) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),TUID + 0.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(2,0),TUID + 0.3))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector wrong value) get with StringValue by reference");

        VarStringVal.set("");
        VarVectorVal.clear();
        OPENFLUID_GetAttribute(TU,"indataVector2",VarStringVal);
        if(!VarStringVal.toVectorValue(";",VarVectorVal))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector2 wrong type)");
        if(VarVectorVal.size() != 3)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector2 wrong size)");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[0], 1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector2 wrong value)");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[2], 1.3))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector2 wrong value)");

        VarMatrixVal.clear();
        OPENFLUID_GetAttribute(TU,"indataVector2",VarMatrixVal);
        if (VarMatrixVal.size() != 3)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector2 wrong size) get with MatrixValue by reference");
        if (!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector2 wrongvalue) get with MatrixValue by reference");
        if (!openfluid::tools::IsVeryClose(VarMatrixVal.at(2,0),1.3))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector2 wrongvalue) get with MatrixValue by reference");

        VarVectorVal.clear();
        OPENFLUID_GetAttribute(TU,"indataVector2",VarVectorVal);
        if(VarVectorVal.size() != 3)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector2 wrong size) get with VectorValue by reference");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[0], 1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector2 wrong value) get with VectorValue by reference");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[2], 1.3))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector2 wrong value) get with VectorValue by reference");

        VarStringVal.set("");
        VarVectorVal.clear();
        OPENFLUID_GetAttribute(TU,"indataVector3",VarStringVal);
        if(!VarStringVal.toVectorValue(";",VarVectorVal))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector3 wrong type) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarVectorVal.size(), 3))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector3 wrong size) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[0], 1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector3 wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarVectorVal[2], 1.3))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataVector3 wrong value) get with StringValue by reference");

        // matrix

        VarStringVal.set("");
        VarMatrixVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMatrix",VarStringVal);
        if(!VarStringVal.toMatrixValue(";","|",VarMatrixVal))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix wrong type) get with StringValue by reference");
        if(VarMatrixVal.getColsNbr() != 3)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix wrong cols nb) get with StringValue by reference");
        if(VarMatrixVal.getRowsNbr() != 2)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix wrong rows nb) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),TUID * 10))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,1),(TUID * 10)+1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(2,1),(TUID * 10)+1.2))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix wrong value) get with StringValue by reference");

        VarStringVal.set("");
        VarMatrixVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMatrix2",VarStringVal);
        if(!VarStringVal.toMatrixValue(";","|",VarMatrixVal))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix2 wrong type) get with StringValue by reference");
        if(VarMatrixVal.getColsNbr() != 2)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix2 wrong cols nb) get with StringValue by reference");
        if(VarMatrixVal.getRowsNbr() != 3)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix2 wrong rows nb) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),0.0))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix2 wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,2),0.2))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix2 wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(1,1),1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix2 wrong value) get with StringValue by reference");

        VarMatrixVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMatrix2",VarMatrixVal);
        if(VarMatrixVal.getColsNbr() != 2)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix2 wrong cols nb) get with MatrixValue by reference");
        if(VarMatrixVal.getRowsNbr() != 3)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix2 wrong rows nb) get with MatrixValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),0.0))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix2 wrongvalue) get with MatrixValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,2),0.2))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix2 wrongvalue) get with MatrixValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(1,1),1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix2 wrongvalue) get with MatrixValue by reference");

        VarMatrixVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMatrix3",VarMatrixVal);
        if(VarMatrixVal.getColsNbr() != 2)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong cols nb) get with MatrixValue by reference");
        if(VarMatrixVal.getRowsNbr() != 3)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong rows nb) get with MatrixValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),0.0))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong value) get with MatrixValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,2),0.2))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong value) get with MatrixValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(1,1),1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong value) get with MatrixValue by reference");

        VarStringVal.set("");
        VarMatrixVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMatrix3",VarStringVal);
        if(!VarStringVal.toMatrixValue(";","|",VarMatrixVal))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong type) get with StringValue by reference");
        if(VarMatrixVal.getColsNbr() != 2)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong cols nb) get with StringValue by reference");
        if(VarMatrixVal.getRowsNbr() != 3)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong rows nb) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),0.0))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,2),0.2))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong value) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(1,1),1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong value) get with StringValue by reference");

        // map

        VarStringVal.set("");
        VarMapVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMap",VarStringVal);
        if(!VarStringVal.toMapValue(";",VarMapVal))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap wrong type) get with StringValue by reference");
        if(VarMapVal.getSize() != 2)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap wrong size) get with StringValue by reference");
        VarDouble = 0.0;
        if(!VarMapVal["key1"].asStringValue().toDouble(VarDouble))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap key1 wrong type) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarDouble,1.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap key1 wrong value) get with StringValue by reference");
        if(VarMapVal.getString("key2") != "aString")
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap key2 wrong value) get with StringValue by reference");

        VarStringVal.set("");
        VarMapVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMap2",VarStringVal);
        if(!VarStringVal.toMapValue(";",VarMapVal))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap2 wrong type) get with StringValue by reference");
        if(VarMapVal.getSize() != 3)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap2 wrong size) get with StringValue by reference");
        VarDouble = 0.0;
        if(!VarMapVal["key1"].asStringValue().toDouble(VarDouble))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap2 key1 wrong type) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarDouble,2.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap2 key1 wrong value) get with StringValue by reference");
        if(VarMapVal.getString("key2") != "a string")
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap2 key2 wrong value) get with StringValue by reference");
        VarBool = false;
        if(!VarMapVal["key3"].asStringValue().toBoolean(VarBool))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap2 key3 wrong type) get with StringValue by reference");
        if(VarBool != true)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap2 key3 wrong value) get with StringValue by reference");

        VarStringVal.set("");
        VarMapVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMap3",VarStringVal);
        if(!VarStringVal.toMapValue(";",VarMapVal))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap3 wrong type) get with StringValue by reference");
        if(VarMapVal.getSize() != 3)
          VarDouble = 0.0;
        if(!VarMapVal["key1"].asStringValue().toDouble(VarDouble))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap3 key1 wrong type) get with StringValue by reference");
        if(!openfluid::tools::IsVeryClose(VarDouble,2.1))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap3 key1 wrong value) get with StringValue by reference");
        if(VarMapVal.getString("key2") != "a string")
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap3 key2 wrong value) get with StringValue by reference");
        VarBool = false;
        if(!VarMapVal["key3"].asStringValue().toBoolean(VarBool))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap3 key3 wrong type) get with StringValue by reference");
        if(VarBool != true)
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataMap3 key3 wrong value) get with StringValue by reference");


        // null

        VarStringVal.set("");
        OPENFLUID_GetAttribute(TU,"indataNull2",VarStringVal);
        if(!VarStringVal.toNullValue(VarNullVal))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataNull2 wrong type)");

        VarStringVal.set("");
        OPENFLUID_GetAttribute(TU,"indataNull3",VarStringVal);
        if(!VarStringVal.toNullValue(VarNullVal))
          OPENFLUID_RaiseError("tests.primitives.attributes.use","incorrect OPENFLUID_GetAttribute (indataNull3 wrong type)");
      }

    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      checkAttributes();

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {

      checkAttributes();

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

DEFINE_SIMULATOR_CLASS(AttributesPrimitivesUseSimulator)

