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
  @file MemoryBufferSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.variable.memorylimit.use")

  DECLARE_NAME("test simulator for variable time production");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");


  DECLARE_USED_VARIABLE("varA","TestUnits","double for tests","");

END_SIMULATOR_SIGNATURE

/**

*/
class MemoryBufferSimumator : public openfluid::ware::PluggableSimulator
{

  private:
    int m_Step = 0;

  public:


  MemoryBufferSimumator() : PluggableSimulator()
  {


  }


  // =====================================================================
  // =====================================================================


  ~MemoryBufferSimumator()
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
    return DefaultDeltaT();
  }


  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest runStep()
  {
    openfluid::core::SpatialUnit* TU;
    if (m_Step == 4)
    {
      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
      {
        for (int i=0;i<6;i++)
        {
          if(OPENFLUID_IsVariableExist(TU,"varA", 
               ( OPENFLUID_GetCurrentTimeIndex() - ( OPENFLUID_GetDefaultDeltaT() * i ) ), 
               openfluid::core::Value::DOUBLE ) )
          {
            OPENFLUID_LogAndDisplayInfo("VARIABLE EXISTS for deltaT x " << i);
          }
          else
          {
            OPENFLUID_LogAndDisplayInfo("VARIABLE DOES NOT EXISTS for deltaT x " << i);
          }
        }
      }
    }
    m_Step++;
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


DEFINE_SIMULATOR_CLASS(MemoryBufferSimumator)

