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
  @file ExampleUnitsAUp.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>



// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN;


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("examples.primitives.unitsA.up")

  DECLARE_NAME("Example of data update on units of class A");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION(openfluid::config::FULL_VERSION);

  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("examples");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("Jean-Christophe Fabre","jean-christophe.fabre@supagro.inra.fr");

  DECLARE_SIMULATOR_PARAM("gmult","multiply coefficient","");

  DECLARE_REQUIRED_VAR("var1","unitsA","the variable 1","");
  DECLARE_REQUIRED_VAR("var10","unitsA","the variable 1","");
  DECLARE_REQUIRED_VAR("var11","unitsA","the variable 1","");
  
  DECLARE_PRODUCED_VAR("var3","unitsA","the variable 3","");
  
  DECLARE_UPDATED_VAR("var2","unitsA","the variable 2","");

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


/**

*/
class ExampleUnitsAUpdate : public openfluid::ware::PluggableSimulator
{
  private:
    double m_Mult;
  
  public:

  
    ExampleUnitsAUpdate() : PluggableSimulator()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~ExampleUnitsAUpdate()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      m_Mult = 1.0;
      OPENFLUID_GetSimulatorParameter(Params,"gmult",m_Mult);
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
      openfluid::core::SpatialUnit* A;

      OPENFLUID_UNITS_ORDERED_LOOP("unitsA",A)
      {
        OPENFLUID_InitializeVariable(A,"var3",0.0);
      }


      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest runStep()
    {
      openfluid::core::SpatialUnit* A;
      openfluid::core::DoubleValue Value1, Value2, Value10, Value11;

      unsigned int CurrentTimeIndex = OPENFLUID_GetCurrentTimeIndex();

      OPENFLUID_UNITS_ORDERED_LOOP("unitsA",A)
      {

        OPENFLUID_GetVariable(A,"var1",CurrentTimeIndex,Value1);

        OPENFLUID_GetVariable(A,"var10",CurrentTimeIndex,Value10);
        OPENFLUID_GetVariable(A,"var11",CurrentTimeIndex,Value11);


        if (OPENFLUID_IsVariableExist(A,"var2",CurrentTimeIndex,openfluid::core::Value::DOUBLE))
        {
          OPENFLUID_GetVariable(A,"var2",CurrentTimeIndex,Value2);
          Value2 = Value2 * m_Mult;
          OPENFLUID_SetVariable(A,"var2",Value2);
        }
        else
        {
          OPENFLUID_RaiseWarning("examples.primitives.unitsA.up","var2 not present, init to value 1.0");
          Value2 = 1.0;
          OPENFLUID_AppendVariable(A,"var2",Value2);
        }

        OPENFLUID_AppendVariable(A,"var3",Value1+0.3);

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


DEFINE_SIMULATOR_CLASS(ExampleUnitsAUpdate);

