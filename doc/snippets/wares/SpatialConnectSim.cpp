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
  @file SpatialConnectSim.cpp

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


//! [spatial_connect]
void prepareData()
{

 /*
      TU.1         TU.2
        |            |
        -->  TU.22 <--
               |
               --> TU.18
                     |
          TU.52 --> OU.5 <-- OU.13
                     |
                     --> OU.25

       VU1 <-> VU2

   with:
   TU1, TU2, TU22, TU18 are children of VU1
   TU52, OU5, OU13, OU25 are children of VU2
*/

  OPENFLUID_AddUnit("VU",1,1);
  OPENFLUID_AddUnit("VU",2,2);
  OPENFLUID_AddUnit("TU",1,1);
  OPENFLUID_AddUnit("TU",2,1);
  OPENFLUID_AddUnit("TU",22,2);
  OPENFLUID_AddUnit("TU",18,3);
  OPENFLUID_AddUnit("TU",52,1);
  OPENFLUID_AddUnit("OU",5,4);
  OPENFLUID_AddUnit("OU",13,1);
  OPENFLUID_AddUnit("OU",25,5);

  OPENFLUID_AddFromToConnection("VU",1,"VU",2);
  OPENFLUID_AddFromToConnection("VU",2,"VU",1);
  OPENFLUID_AddFromToConnection("TU",1,"TU",22);
  OPENFLUID_AddFromToConnection("TU",2,"TU",22);
  OPENFLUID_AddFromToConnection("TU",22,"TU",18);
  OPENFLUID_AddFromToConnection("TU",18,"OU",5);
  OPENFLUID_AddFromToConnection("TU",52,"OU",5);
  OPENFLUID_AddFromToConnection("OU",13,"OU",5);
  OPENFLUID_AddFromToConnection("OU",5,"OU",25);

  OPENFLUID_AddChildParentConnection("TU",1,"VU",1);
  OPENFLUID_AddChildParentConnection("TU",2,"VU",1);
  OPENFLUID_AddChildParentConnection("TU",22,"VU",1);
  OPENFLUID_AddChildParentConnection("TU",18,"VU",1);
  OPENFLUID_AddChildParentConnection("TU",52,"VU",2);
  OPENFLUID_AddChildParentConnection("OU",5,"VU",2);
  OPENFLUID_AddChildParentConnection("OU",13,"VU",2);
  OPENFLUID_AddChildParentConnection("OU",25,"VU",2);
}
//! [spatial_connect]


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

