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
  @file Sim2DocSim.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>


//! [sim2doc]
/*
<sim2doc>
This part of the documentation will be included in the 
It can be formatted using \LaTeX and is fully compatible with all \LaTeX commands,
including tables, scientific formulae, figures, and many more.
</sim2doc>
*/

BEGIN_SIMULATOR_SIGNATURE("help.snippets.sim2doc")

  DECLARE_NAME("Example simulator");
  DECLARE_DESCRIPTION("This simulator is an example");
  DECLARE_VERSION("13.05");
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);
  DECLARE_AUTHOR("John","john@foobar.org");
  DECLARE_AUTHOR("Dave","dave@foobar.org");
  DECLARE_AUTHOR("Mike","mike@foobar.org");
  
  DECLARE_REQUIRED_PARAMETER("meanspeed","mean speed to use","m/s")
  
  DECLARE_REQUIRED_ATTRIBUTE("area","TU","area of the Test Units","m")
  DECLARE_USED_ATTRIBUTE("landuse","OU","landuse of the Other Units","")
  
  DECLARE_REQUIRED_VARIABLE("varA[double]","TU","","m")
  DECLARE_USED_VARIABLE("varB","OU","simple var on Other Units","kg")
  DECLARE_PRODUCED_VARIABLE("VarB[vector]","TU","vectorized var on Test Units","kg")
  DECLARE_UPDATED_VARIABLE("VarC","TU","","")
  
  DECLARE_USED_EVENTS("TU")
  
END_SIMULATOR_SIGNATURE
//! [sim2doc]


// =====================================================================
// =====================================================================


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


DEFINE_SIMULATOR_CLASS(SnippetsSimulator)

