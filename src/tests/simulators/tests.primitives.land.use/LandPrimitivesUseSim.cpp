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
  @file LandPrimitivesUseSim.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN


BEGIN_SIMULATOR_SIGNATURE("tests.primitives.land.use")

  DECLARE_NAME("");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("10.07");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");

  DECLARE_REQUIRED_VAR("tests.var1","TU","","");
  DECLARE_REQUIRED_VAR("tests.var2","MU","","");

  DECLARE_REQUIRED_ATTRIBUTE("attr1","TU","","");

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class LandPrimitivesUseSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


    LandPrimitivesUseSimulator() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~LandPrimitivesUseSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {  }


    // =====================================================================
    // =====================================================================


    void prepareData()
    {  }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {  }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {
      openfluid::core::SpatialUnit* U;
      openfluid::core::DoubleValue TmpValue;

      OPENFLUID_UNITS_ORDERED_LOOP("TU",U)
      {
        OPENFLUID_GetVariable(U,"tests.var1",TmpValue);
      }

      OPENFLUID_UNITS_ORDERED_LOOP("MU",U)
      {
        OPENFLUID_GetVariable(U,"tests.var2",TmpValue);
      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      openfluid::core::SpatialUnit* U;
      openfluid::core::DoubleValue TmpValue;

      OPENFLUID_UNITS_ORDERED_LOOP("TU",U)
      {
        OPENFLUID_GetVariable(U,"tests.var1",TmpValue);
      }

      OPENFLUID_UNITS_ORDERED_LOOP("MU",U)
      {
        OPENFLUID_GetVariable(U,"tests.var2",TmpValue);
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


DEFINE_SIMULATOR_CLASS(LandPrimitivesUseSimulator)

