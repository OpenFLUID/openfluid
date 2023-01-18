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
This is \LaTeX doc\\
\vspace{1em}
enclosed between {\tt sim2doc} tags
</sim2doc>
*/


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.cmdline.sim-ui-migration")

  // Informations
  DECLARE_NAME("Simulator 2.1.x with paramsUI for migration")
  DECLARE_DESCRIPTION("This is a simulator 2.1.x with parameterization UI for migration")
  DECLARE_VERSION("2022-09")

  DECLARE_AUTHOR("Nomen nescio","nn@bar.org")

  DECLARE_DOMAIN("testing")

  DECLARE_USED_EXTRAFILE("observed.csv")
  DECLARE_REQUIRED_EXTRAFILE("db.json")

  DECLARE_USED_PARAMETER("pA","parameter A","kg")
  DECLARE_REQUIRED_PARAMETER("prX","parameter X","")
  DECLARE_REQUIRED_PARAMETER("prY","parameter Y","m/s")

  DECLARE_USED_ATTRIBUTE("attr1","TUA","","")
  DECLARE_REQUIRED_ATTRIBUTE("attr2","TUA","","")
  DECLARE_REQUIRED_ATTRIBUTE("attr2","TUB","length","m")

  DECLARE_USED_VARIABLE("varA","TUA","variable A","m")
  DECLARE_USED_VARIABLE("genvarA","TUA","variable A","m")
  DECLARE_REQUIRED_VARIABLE("varA","TUB","variable A","m")
  DECLARE_REQUIRED_VARIABLE("genvarA","TUB","variable A","m")
  DECLARE_REQUIRED_VARIABLE("genvarB","TUB","variable A","m")
  DECLARE_UPDATED_VARIABLE("varB","TUA","","")
  DECLARE_PRODUCED_VARIABLE("varI","TUA","variable I","m")
  DECLARE_PRODUCED_VARIABLE("varJ","TUA","variable J","kg/m3")

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


/**

*/
class SimulatorWithParamsUI : public openfluid::ware::PluggableSimulator
{
  private:

  
  public:

  
    SimulatorWithParamsUI(): PluggableSimulator()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~SimulatorWithParamsUI()
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


DEFINE_SIMULATOR_CLASS(SimulatorWithParamsUI);


DEFINE_WARE_LINKUID(WARE_LINKUID)

