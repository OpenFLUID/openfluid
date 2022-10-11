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
  @file AttributesPrimitivesUseSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QString>

#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/core/MapValue.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>


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
      openfluid::core::SpatialUnit* TU;

      openfluid::core::DoubleValue VarDoubleVal;
      openfluid::core::VectorValue VarVectorVal;

      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
      {
        if (!OPENFLUID_IsAttributeExist(TU,"indataDouble"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataDouble)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataVector"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataVector)");
        }

        VarDoubleVal.set(0.0);
        OPENFLUID_GetAttribute(TU,"indataDouble",VarDoubleVal);
        if (!openfluid::scientific::isVeryClose(VarDoubleVal.get(),1.1))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataDouble wrongvalue) "
                               "get with DoubleValue by reference");
        }
      }

    }


    // =====================================================================
    // =====================================================================


    void checkConsistency() { }


    // =====================================================================
    // =====================================================================


    void checkAttributes()
    {

      openfluid::core::SpatialUnit* TU;
      unsigned int TUID;

      double RefDouble;
      long RefLong;
      bool RefBool;
      std::string RefString;

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

        RefDouble = (double)TUID/10;
        RefLong = TUID;
        RefBool = (TUID%2 == 0);
        RefString = QString("ID %1").arg(TUID).toStdString();


        if (!OPENFLUID_IsAttributeExist(TU,"indataDouble"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataDouble)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataDouble2"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataDouble2)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataDouble3"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataDouble3)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataLong"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataLong)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataLong2"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataLong2)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataLong3"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataLong3)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataBool"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataBool)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataBool2"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataBool2)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataBool3"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataBool3)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataString"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataString)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataString2"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataString2)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataString3"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataString3)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataVector"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataVector)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataVector2"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataVector2)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataVector3"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataVector3)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataMatrix"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataMatrix)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataMatrix2"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataMatrix2)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataMatrix3"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataMatrix3)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataMap"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataMap)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataMap2"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataMap2)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataMap3"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataMap3)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataNull2"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataNull2)");
        }

        if (!OPENFLUID_IsAttributeExist(TU,"indataNull3"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (indataNull3)");
        }

        if (OPENFLUID_IsAttributeExist(TU,"wrongdata"))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_IsAttributeExist (wrongdata)");
        }


        // double

        VarDouble = 0.0;
        OPENFLUID_GetAttribute(TU,"indataDouble",VarDouble);
        if (!openfluid::scientific::isVeryClose(VarDouble,1.1))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataDouble wrongvalue) get by reference");
        }

        VarDoubleVal.set(0.0);
        OPENFLUID_GetAttribute(TU,"indataDouble",VarDoubleVal);
        if (!openfluid::scientific::isVeryClose(VarDoubleVal.get(),1.1))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataDouble wrongvalue) "
                               "get with DoubleValue by reference");
        }

        VarDouble = 0.0;
        OPENFLUID_GetAttribute(TU,"indataDouble2",VarDouble);
        if (!openfluid::scientific::isVeryClose(VarDouble,RefDouble))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataDouble2 wrongvalue)");
        }


        VarDouble = 0.0;
        OPENFLUID_GetAttribute(TU,"indataDouble3",VarDouble);
        if (!openfluid::scientific::isVeryClose(VarDouble,RefDouble))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataDouble3 wrongvalue)");
        }


        // long

        VarLong = 0;
        OPENFLUID_GetAttribute(TU,"indataLong",VarLong);
        if (VarLong != 1)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataLong wrongvalue) get by reference");
        }

        VarLong = 0;
        OPENFLUID_GetAttribute(TU,"indataLong2",VarLong);
        if (VarLong != RefLong)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataLong2 wrongvalue)");
        }


        VarLong = 0;
        OPENFLUID_GetAttribute(TU,"indataLong3",VarLong);
        if (VarLong != RefLong)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataLong3 wrongvalue)");
        }

        VarDoubleVal = 0.0;
        OPENFLUID_GetAttribute(TU,"indataLong3",VarDoubleVal);
        if (!openfluid::scientific::isVeryClose(VarDoubleVal.get(),RefLong))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataLong3 conversion to double)");
        }


        // bool

        VarBoolVal.set(false);
        OPENFLUID_GetAttribute(TU,"indataBool",VarBoolVal);
        if (VarBoolVal != RefBool)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataBool2 wrongvalue) "
                               "get with StringValue by reference");
        }

        VarBoolVal.set(false);
        VarBool = false;
        OPENFLUID_GetAttribute(TU,"indataBool2",VarBoolVal);

        if (VarBoolVal != RefBool)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataBool2 wrongvalue) "
                               "get with StringValue by reference");
        }

        VarLongVal = -1;
        OPENFLUID_GetAttribute(TU,"indataBool2",VarLongVal);
        if (VarLongVal != RefBool)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataBool2 wrong conversion to int)");
        }

        VarDoubleVal = -1.0;
        OPENFLUID_GetAttribute(TU,"indataBool2",VarDoubleVal);
        if (!openfluid::scientific::isVeryClose(VarDoubleVal.get(),RefBool))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataBool2 wrong conversion to double)");
        }

        //string
        std::string RefStr = std::to_string(TU->getID());
        RefStr = "CODE"+RefStr;
        VarString = "";
        OPENFLUID_GetAttribute(TU,"indataString",VarString);
        if (VarString != RefStr)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataString wrongvalue) get by reference");
        }

        VarStringVal.set("");
        VarString = "";
        OPENFLUID_GetAttribute(TU,"indataString",VarStringVal);
        if (VarStringVal.get() != RefStr)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataString wrongvalue) "
                               "get with StringValue by reference");
        }

        VarString = "";
        OPENFLUID_GetAttribute(TU,"indataString2",VarString);
        if (VarString != RefString)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataString2 wrongvalue)");
        }

        VarStringVal.set("");
        VarString = "";
        OPENFLUID_GetAttribute(TU,"indataString2",VarStringVal);
        if (VarStringVal.get() != RefString)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataString2 wrongvalue) "
                               "get with StringValue by reference");
        }

        VarString = "";
        OPENFLUID_GetAttribute(TU,"indataString3",VarString);
        if (VarString != RefString)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataString3 wrongvalue)");
        }

        VarStringVal.set("");
        VarString = "";
        OPENFLUID_GetAttribute(TU,"indataString3",VarStringVal);
        if (VarStringVal.get() != RefString)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataString3 wrongvalue) "
                               "get with StringValue by reference");
        }


        // vector

        VarVectorVal.clear();
        OPENFLUID_GetAttribute(TU,"indataVector",VarVectorVal);
        if(VarVectorVal.size() != 3)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataVector wrong size)");
        }
        if(!openfluid::scientific::isVeryClose(VarVectorVal[0], TUID + 0.1))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataVector wrong value)");
        }
        if(!openfluid::scientific::isVeryClose(VarVectorVal[2], TUID + 0.3))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataVector wrong value)");
        }


        VarVectorVal.clear();
        OPENFLUID_GetAttribute(TU,"indataVector2",VarVectorVal);
        if(VarVectorVal.size() != 3)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataVector2 wrong size)");
        }
        if(!openfluid::scientific::isVeryClose(VarVectorVal[0], 1.1))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataVector2 wrong value)");
        }
        if(!openfluid::scientific::isVeryClose(VarVectorVal[2], 1.3))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataVector2 wrong value)");
        }


        VarVectorVal.clear();
        OPENFLUID_GetAttribute(TU,"indataVector3",VarVectorVal);
        if(VarVectorVal.size() != 3)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataVector3 wrong size) "
                               "get with StringValue by reference");
        }
        if(!openfluid::scientific::isVeryClose(VarVectorVal[0], 1.1))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataVector3 wrong value) "
                               "get with StringValue by reference");
        }
        if(!openfluid::scientific::isVeryClose(VarVectorVal[2], 1.3))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataVector3 wrong value) "
                               "get with StringValue by reference");
        }

        // matrix

        VarMatrixVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMatrix",VarMatrixVal);
        if(VarMatrixVal.getColsNbr() != 3)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix wrong cols nb) "
                               "get with StringValue by reference");
        }
        if(VarMatrixVal.getRowsNbr() != 2)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix wrong rows nb) "
                               "get with StringValue by reference");
        }
        if(!openfluid::scientific::isVeryClose<double>(VarMatrixVal.at(0,0),TUID * 10))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix wrong value) "
                               "get with StringValue by reference");
        }
        if(!openfluid::scientific::isVeryClose<double>(VarMatrixVal.at(0,1),(TUID * 10)+1))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix wrong value) "
                               "get with StringValue by reference");
        }
        if(!openfluid::scientific::isVeryClose(VarMatrixVal.at(2,1),(TUID * 10)+1.2))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix wrong value) "
                               "get with StringValue by reference");
        }

        VarMatrixVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMatrix2",VarMatrixVal);
        if(VarMatrixVal.getColsNbr() != 2)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix2 wrong cols nb) "
                               "get with StringValue by reference");
        }
        if(VarMatrixVal.getRowsNbr() != 3)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix2 wrong rows nb) "
                               "get with StringValue by reference");
        }
        if(!openfluid::scientific::isVeryClose(VarMatrixVal.at(0,0),0.0))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix2 wrong value) "
                               "get with StringValue by reference");
        }
        if(!openfluid::scientific::isVeryClose(VarMatrixVal.at(0,2),0.2))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix2 wrong value) "
                               "get with StringValue by reference");
        }
        if(!openfluid::scientific::isVeryClose(VarMatrixVal.at(1,1),1.1))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix2 wrong value) "
                               "get with StringValue by reference");
        }


        VarMatrixVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMatrix3",VarMatrixVal);
        if(VarMatrixVal.getColsNbr() != 2)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong cols nb) "
                               "get with MatrixValue by reference");
        }
        if(VarMatrixVal.getRowsNbr() != 3)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong rows nb) "
                               "get with MatrixValue by reference");
        }
        if(!openfluid::scientific::isVeryClose(VarMatrixVal.at(0,0),0.0))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong value) "
                               "get with MatrixValue by reference");
        }
        if(!openfluid::scientific::isVeryClose(VarMatrixVal.at(0,2),0.2))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong value) "
                               "get with MatrixValue by reference");
        }
        if(!openfluid::scientific::isVeryClose(VarMatrixVal.at(1,1),1.1))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMatrix3 wrong value) "
                               "get with MatrixValue by reference");
        }


        // map

        VarMapVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMap",VarMapVal);
        if(VarMapVal.getSize() != 2)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMap wrong size) "
                               "get with StringValue by reference");
        }
        VarDouble =  VarMapVal["key1"].asDoubleValue();
        if(!openfluid::scientific::isVeryClose(VarDouble,1.1))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMap key1 wrong value) "
                               "get with StringValue by reference");
        }
        if(VarMapVal.getString("key2") != "aString")
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMap key2 wrong value) "
                               "get with StringValue by reference");
        }

        VarMapVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMap2",VarMapVal);
        if(VarMapVal.getSize() != 3)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMap2 wrong size) "
                               "get with StringValue by reference");
        }
        VarDouble = VarMapVal["key1"].asDoubleValue();
        if(!openfluid::scientific::isVeryClose(VarDouble,2.1))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMap2 key1 wrong value) "
                               "get with StringValue by reference");
        }
        if(VarMapVal.getString("key2") != "a string")
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMap2 key2 wrong value) "
                               "get with StringValue by reference");
        }
        VarBool = VarMapVal["key3"].asBooleanValue();
        if(VarBool != true)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMap2 key3 wrong value) "
                               "get with StringValue by reference");
        }

        VarMapVal.clear();
        OPENFLUID_GetAttribute(TU,"indataMap3",VarMapVal);
        if(VarMapVal.getSize() != 3)
        {
          VarDouble = 0.0;
        }
        VarDouble = VarMapVal["key1"].asDoubleValue();
        if(!openfluid::scientific::isVeryClose(VarDouble,2.1))
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMap3 key1 wrong value) "
                               "get with StringValue by reference");
        }
        if(VarMapVal.getString("key2") != "a string")
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMap3 key2 wrong value) "
                               "get with StringValue by reference");
        }
        VarBool = VarMapVal["key3"].asBooleanValue();
        if(VarBool != true)
        {
          OPENFLUID_RaiseError("incorrect OPENFLUID_GetAttribute (indataMap3 key3 wrong value) "
                               "get with StringValue by reference");
        }


        // null

        OPENFLUID_GetAttribute(TU,"indataNull2",VarNullVal);

        OPENFLUID_GetAttribute(TU,"indataNull3",VarNullVal);
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

