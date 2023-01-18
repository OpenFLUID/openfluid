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
  @file Simulator.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/

/*
<sim2doc>

</sim2doc>
*/


#include <openfluid/ware/PluggableSimulator.hpp>

#include "Other.hpp"


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.cmdline.sim-migration")

  // Informations
  DECLARE_NAME("Simulator 2.1.x for migration")
  DECLARE_DESCRIPTION("This is a simulator 2.1.x for migration")
  DECLARE_VERSION("2022-09")
  DECLARE_STATUS(openfluid::ware::BETA)
  DECLARE_SDKVERSION

  DECLARE_AUTHOR("John Doe","john.doe@foo.org")
  DECLARE_AUTHOR("Jane Doe","jane.doe@foo.org")

  DECLARE_DOMAIN("testing")
  DECLARE_PROCESS("migration")
  DECLARE_METHOD("conversion")

  DECLARE_USED_PARAMETER("param1","parameter for nothing","x/x")

  DECLARE_PRODUCED_ATTRIBUTE("attr1","TUA","attribute 1","")
 
  DECLARE_PRODUCED_VARIABLE("varA","TUA","variable A","m")
  DECLARE_PRODUCED_VARIABLE("varA","TUB","variable A","m")
  DECLARE_PRODUCED_VARIABLE("varB","TUA","","")  
  
  DECLARE_USED_EVENTS("TUA")
  
  DECLARE_UPDATED_UNITSGRAPH("update of the spatial units graph")
  DECLARE_UPDATED_UNITSCLASS("TUA","modification of from-to connections")
  
  DECLARE_SCHEDULING_DEFAULT

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


/**

*/
class Simulator : public openfluid::ware::PluggableSimulator
{
  private:

  
  public:

  
    Simulator(): PluggableSimulator()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~Simulator()
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
      SomeFunction();
      
      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest runStep()
    {

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


DEFINE_SIMULATOR_CLASS(Simulator);


DEFINE_WARE_LINKUID(WARE_LINKUID)
