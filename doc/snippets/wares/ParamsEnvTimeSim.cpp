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
  @file ParamsEnvTimeSim.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>


class SnippetsSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    openfluid::core::DoubleValue m_MyParam;


  public:


    SnippetsSimulator() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~SnippetsSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


//! [params]
void initParams(const openfluid::ware::WareParams_t& Params)
{
  m_MyParam = 0; //default value set to the class member
  OPENFLUID_GetSimulatorParameter(Params,"myparam",m_MyParam);
}
//! [params]


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


//! [env]
openfluid::base::SchedulingRequest initializeRun()
{
  std::string InputDir;

  OPENFLUID_GetRunEnvironment("dir.input",InputDir);

  // the current input directory is now available through the InputDir local variable 

  return DefaultDeltaT();
}
//! [env]


    // =====================================================================
    // =====================================================================


//! [time]
openfluid::base::SchedulingRequest runStep()
{
  openfluid::core::Duration_t Duration = OPENFLUID_GetSimulationDuration();

  openfluid::core::TimeIndex_t CurrentIndex = OPENFLUID_GetCurrentTimeIndex();
  openfluid::core::DateTime CurrentDT = OPENFLUID_GetCurrentDate();  
  
  std::cout << Duration << std::endl;
  std::cout << CurrentIndex << std::endl;
  std::cout << CurrentDT.getAsISOString() << std::endl;

  return DefaultDeltaT();      
}
//! [time]


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};

