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
  @file AttributesPrimitivesProdSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/core/MapValue.hpp>
#include <openfluid/tools/StringHelpers.hpp>


class AttributesPrimitivesProdSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


    AttributesPrimitivesProdSimulator() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~AttributesPrimitivesProdSimulator()
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
      openfluid::core::VectorValue aVector(3);
      aVector.set(0,1.1);
      aVector.set(1,1.2);
      aVector.set(2,1.3);
      openfluid::core::MatrixValue aMatrix(2,3);
      aMatrix.set(0,0,0.0);
      aMatrix.set(0,1,0.1);
      aMatrix.set(0,2,0.2);
      aMatrix.set(1,0,1.0);
      aMatrix.set(1,1,1.1);
      aMatrix.set(1,2,1.2);
      openfluid::core::MapValue aMap;
      aMap.setDouble("key1",2.1);
      aMap.setString("key2","a string");
      aMap.setBoolean("key3",true);


      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
      {
        double Dbl = (double)TU->getID() / 10;
        long Lg = TU->getID();
        bool Bl = (TU->getID()%2 == 0);
        std::string Str = openfluid::tools::format("ID %d",TU->getID());

        OPENFLUID_SetAttribute(TU,"indataDouble2",Dbl);
        OPENFLUID_SetAttribute(TU,"indataDouble3",openfluid::core::DoubleValue(Dbl));

        OPENFLUID_SetAttribute(TU,"indataLong2",Lg);
        OPENFLUID_SetAttribute(TU,"indataLong3",openfluid::core::IntegerValue(Lg));

        OPENFLUID_SetAttribute(TU,"indataString2",Str);
        OPENFLUID_SetAttribute(TU,"indataString3",openfluid::core::StringValue(Str));

        OPENFLUID_SetAttribute(TU,"indataBool2",Bl ? true : false);
        OPENFLUID_SetAttribute(TU,"indataBool3",openfluid::core::BooleanValue(Bl));

        openfluid::core::VectorValue TmpVV;
        openfluid::core::StringValue("[1.1,1.2,1.3]").toVectorValue(TmpVV);
        OPENFLUID_SetAttribute(TU,"indataVector2",TmpVV);
        OPENFLUID_SetAttribute(TU,"indataVector3",aVector);

        openfluid::core::MatrixValue TmpMV;
        openfluid::core::StringValue("[[0.0,1.0],[0.1,1.1],[0.2,1.2]]").toMatrixValue(TmpMV);
        OPENFLUID_SetAttribute(TU,"indataMatrix2",TmpMV);
        OPENFLUID_SetAttribute(TU,"indataMatrix3",aMatrix);

        openfluid::core::MapValue TmpMpV;
        openfluid::core::StringValue(R"({"key1":2.1,"key2":"a string","key3":true})").toMapValue(TmpMpV);
        OPENFLUID_SetAttribute(TU,"indataMap2",TmpMpV);
        OPENFLUID_SetAttribute(TU,"indataMap3",aMap);

        OPENFLUID_SetAttribute(TU,"indataNull2",openfluid::core::NullValue());
        OPENFLUID_SetAttribute(TU,"indataNull3",openfluid::core::NullValue());
      }

    }


    // =====================================================================
    // =====================================================================


    void checkConsistency() { }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      return Never();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {

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


DEFINE_SIMULATOR_CLASS(AttributesPrimitivesProdSimulator)

