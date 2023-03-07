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
  @file InfosDebugSim.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>


class SnippetsSimulator : public openfluid::ware::PluggableSimulator
{
  private:

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


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {

    }


    // =====================================================================
    // =====================================================================


    void prepareData()
    {
      openfluid::core::SpatialUnit* TU;

        OPENFLUID_UNITS_ORDERED_LOOP("TU",TU)
        {

//! [macro_logwarning]
OPENFLUID_LogWarning("This is a logged warning message for " << "TestUnits#" << TU->getID());
//! [macro_logwarning]

//! [macro_displaywarning]
OPENFLUID_DisplayWarning("This is a displayed warning message for " << "TestUnits#" << TU->getID());
//! [macro_displaywarning]

//! [macro_logdisplaywarning]
OPENFLUID_LogAndDisplayWarning("This is a logged and displayed warning message for " <<
                               "TestUnits#" << TU->getID());
//! [macro_logdisplaywarning]


//! [macro_loginfo]
OPENFLUID_LogInfo("This is a logged information message for " << "TestUnits#" << TU->getID());
//! [macro_loginfo]

//! [macro_displayinfo]
OPENFLUID_DisplayInfo("This is a displayed information message for " << "TestUnits#" << TU->getID());
//! [macro_displayinfo]

//! [macro_logdisplayinfo]
OPENFLUID_LogAndDisplayInfo("This is a logged and displayed information message for " <<
                            "TestUnits#" << TU->getID());
//! [macro_logdisplayinfo]


//! [macro_logdebug]
OPENFLUID_LogDebug("This is a logged debug message for " << "TestUnits#" << TU->getID());
//! [macro_logdebug]

//! [macro_displaydebug]
OPENFLUID_DisplayDebug("This is a displayed debug message for " << "TestUnits#" << TU->getID());
//! [macro_displaydebug]

//! [macro_logdisplaydebug]
OPENFLUID_LogAndDisplayDebug("This is a logged and displayed debug message for " << 
                             "TestUnits#" << TU->getID());
//! [macro_logdisplaydebug]

        }
    }


    // =====================================================================
    // =====================================================================


//! [error]
void checkConsistency()
{
  double TmpValue;
  openfluid::core::SpatialUnit* SU;
    
  OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
  {
    OPENFLUID_GetAttribute(SU,"MyAttr",TmpValue);
    
    if (TmpValue <= 0)
    {
      OPENFLUID_RaiseError("Wrong value for the MyProp attribute on SU");
    }    
  }
}
//! [error]


    // =====================================================================
    // =====================================================================


//! [infowarning]
openfluid::base::SchedulingRequest initializeRun()
{
  openfluid::core::SpatialUnit* TU;

  OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
  {
    OPENFLUID_LogInfo("TestUnits #" << TU->getID());
    OPENFLUID_DisplayInfo("TestUnits #" << TU->getID());

    OPENFLUID_LogWarning("This is a warning message for " << "TestUnits #" << TU->getID());
  }

  return DefaultDeltaT();
}
//! [infowarning]


    // =====================================================================
    // =====================================================================


//! [debug]
openfluid::base::SchedulingRequest runStep()
{
  OPENFLUID_LogDebug("Entering runStep at time index " << OPENFLUID_GetCurrentTimeIndex());

  return DefaultDeltaT();
}
//! [debug]


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};

