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
  \file WrongProdSim.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN

// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.wrongprod")

  DECLARE_NAME("test simulator for wrong production");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");


  DECLARE_PRODUCED_VAR("tests.wrongvector[vector]","TestUnits","wrong vector for tests","");
  DECLARE_PRODUCED_VAR("tests.wrongscalar[double]","TestUnits","wrong scalar for tests","");
  DECLARE_PRODUCED_VAR("tests.scalarok[double]","TestUnits","scalar OK for tests","");

  DECLARE_SCHEDULING_DEFAULT;

END_SIMULATOR_SIGNATURE

/**

*/
class WrongProductionSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


  WrongProductionSimulator() : PluggableSimulator()
  {


  }


  // =====================================================================
  // =====================================================================


  ~WrongProductionSimulator()
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
    return DefaultDeltaT();
  }

  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest runStep()
  {
    openfluid::core::Unit* TU;


    OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
    {
      OPENFLUID_AppendVariable(TU,"tests.scalarok",double(TU->getID()));
    }


    return DefaultDeltaT();
  }

  // =====================================================================
  // =====================================================================


  void finalizeRun()
  {  }

};

// =====================================================================
// =====================================================================

DEFINE_SIMULATOR_CLASS(WrongProductionSimulator)

