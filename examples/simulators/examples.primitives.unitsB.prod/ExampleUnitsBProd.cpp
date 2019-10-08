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
  @file ExampleUnitsBProd.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("examples.primitives.unitsB.prod")

  DECLARE_NAME("Example of data production on units of class B");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION(openfluid::config::VERSION_FULL);

  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("examples");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("Jean-Christophe Fabre","jean-christophe.fabre@inra.fr");

  DECLARE_USED_VARIABLE("var2","unitsA","the variable 2","");
  DECLARE_USED_VARIABLE("var3","unitsA","the variable 3","");
  
  DECLARE_PRODUCED_VARIABLE("var5","unitsB","the variable 5","");
  

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


/**

*/
class ExampleUnitsBProduction : public openfluid::ware::PluggableSimulator
{
  private:

  
  public:

  
    ExampleUnitsBProduction() : PluggableSimulator()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~ExampleUnitsBProduction()
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
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    void checkConsistency()
    {
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest initializeRun()
    {
      openfluid::core::SpatialUnit *B;

      OPENFLUID_UNITS_ORDERED_LOOP("unitsB",B)
      {
        OPENFLUID_InitializeVariable(B,"var5",0.0);
      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest runStep()
    {

      openfluid::core::SpatialUnit *FromA, *FromB, *B;
      openfluid::core::UnitsPtrList_t *FromAList, *FromBList;
      openfluid::core::DoubleValue Value5, AuxValue;

      unsigned int CurrentTimeIndex = OPENFLUID_GetCurrentTimeIndex();

      OPENFLUID_UNITS_ORDERED_LOOP("unitsB",B)
      {

        Value5 = 0.0;

        FromAList = B->fromSpatialUnits("unitsA");

        if (FromAList != nullptr)
        {
          OPENFLUID_UNITSLIST_LOOP(FromAList,FromA)
          {

            if (OPENFLUID_IsVariableExist(FromA,"var2",CurrentTimeIndex))
            {
              OPENFLUID_GetVariable(FromA,"var2",CurrentTimeIndex,AuxValue);
              Value5 = Value5 + AuxValue;
            }
            else
            {
              OPENFLUID_LogWarning("var2 is not present, ignored");
            }

            if (OPENFLUID_IsVariableExist(FromA,"var3",CurrentTimeIndex))
            {
              OPENFLUID_GetVariable(FromA,"var3",CurrentTimeIndex,AuxValue);
              Value5 = Value5 + AuxValue;
            }
            else
            {
              OPENFLUID_LogWarning("var3 is not present, ignored");
            }

          }
        }

        if (OPENFLUID_GetCurrentTimeIndex()>0)
        {
          FromBList = B->fromSpatialUnits("unitsB");

          if (FromBList != nullptr)
          {
            OPENFLUID_UNITSLIST_LOOP(FromBList,FromB)
            {
              OPENFLUID_GetVariable(FromB,"var5",CurrentTimeIndex-OPENFLUID_GetDefaultDeltaT(),AuxValue);
              Value5 = Value5 + AuxValue;

            }
          }
        }

        OPENFLUID_AppendVariable(B,"var5",Value5);

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


DEFINE_SIMULATOR_CLASS(ExampleUnitsBProduction);

