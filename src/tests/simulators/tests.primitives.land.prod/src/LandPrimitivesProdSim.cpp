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
  @file LandPrimitivesProdSim.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/ware/PluggableSimulator.hpp>


class LandPrimitivesProdSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


    LandPrimitivesProdSimulator() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~LandPrimitivesProdSimulator()
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
      openfluid::core::SpatialUnit* U;

      OPENFLUID_UNITS_ORDERED_LOOP("TU",U)
      {
        OPENFLUID_InitializeVariable(U,"tests.var1",0.0);
      }

      OPENFLUID_UNITS_ORDERED_LOOP("MU",U)
      {
        OPENFLUID_InitializeVariable(U,"tests.var2",0.0);
      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      openfluid::core::SpatialUnit* U;

      OPENFLUID_UNITS_ORDERED_LOOP("TU",U)
      {
        OPENFLUID_AppendVariable(U,"tests.var1",0.0);
      }

      OPENFLUID_UNITS_ORDERED_LOOP("MU",U)
      {
        OPENFLUID_AppendVariable(U,"tests.var2",0.0);
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


DEFINE_SIMULATOR_CLASS(LandPrimitivesProdSimulator)

