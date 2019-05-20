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
  @file SignaturePartsSim.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>


BEGIN_SIMULATOR_SIGNATURE("help.snippets.signature-parts")

//! [signature_params]
DECLARE_REQUIRED_PARAMETER("meanspeed","mean speed to use","m/s")
//! [signature_params]

//! [signature_attributes]
DECLARE_REQUIRED_ATTRIBUTE("area","TU","area of the Test Units","m")
DECLARE_USED_ATTRIBUTE("landuse","OU","landuse of the Other Units","")
//! [signature_attributes]

//! [signature_variables]
DECLARE_REQUIRED_VARIABLE("varA[double]","TU","","m")
DECLARE_USED_VARIABLE("varB","OU","simple var on Other Units","kg")
DECLARE_PRODUCED_VARIABLE("VarB[vector]","TU","vectorized var on Test Units","kg")
DECLARE_UPDATED_VARIABLE("VarC","TU","","")
//! [signature_variables]

//! [signature_events]
DECLARE_USED_EVENTS("TU")
//! [signature_events]

//! [signature_extrafiles]
DECLARE_USED_EXTRAFILE("fileA.dat")
DECLARE_REQUIRED_EXTRAFILE("geo/zone.shp")
//! [signature_extrafiles]

//! [signature_spatial]
DECLARE_UPDATED_UNITSGRAPH("update of the spatial graph for ...")
DECLARE_UPDATED_UNITSCLASS("TU","")
//! [signature_spatial]

END_SIMULATOR_SIGNATURE


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

