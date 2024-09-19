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
  @file WareMain.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>


/**

*/
class ExampleUnitsAUpdate : public openfluid::ware::PluggableSimulator
{
  private:
    double m_Mult;
  
  public:

  
    ExampleUnitsAUpdate() : PluggableSimulator(),
      m_Mult(1.0)
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
      OPENFLUID_GetWareParameter(Params,"gmult",m_Mult);
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
          OPENFLUID_LogWarning("var2 not present, init to value 1.0");
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

