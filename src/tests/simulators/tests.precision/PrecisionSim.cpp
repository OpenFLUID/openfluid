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
  @file PrecisionSim.cpp

  @author Armel THONI <armel.thoni@inra.fr>
 */


#include <openfluid/debug.hpp>
#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.precision")

  DECLARE_NAME("test simulator to check precision on various numbers and settings");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_STATUS(openfluid::ware::STABLE);


  DECLARE_PRODUCED_VARIABLE("prec.bigger","TU","Basic variable with growing value" ,"");
  DECLARE_PRODUCED_VARIABLE("prec.smaller","TU","Basic variable with decreasing value" ,"");

  DECLARE_SCHEDULING_DEFAULT;

END_SIMULATOR_SIGNATURE

/**

*/
class PrecisionSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


  PrecisionSimulator() : PluggableSimulator()
  {


  }


  // =====================================================================
  // =====================================================================


  ~PrecisionSimulator()
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

    openfluid::core::SpatialUnit* TU;

    OPENFLUID_UNITS_ORDERED_LOOP("TU",TU)
    {
      OPENFLUID_InitializeVariable(TU,"prec.bigger",0.123456789);
      OPENFLUID_InitializeVariable(TU,"prec.smaller",0.123456789);
    }
    return DefaultDeltaT();
  }


  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest runStep()
  {
    openfluid::core::SpatialUnit* TU;

    OPENFLUID_UNITS_ORDERED_LOOP("TU",TU)
    {
      double NewValue = OPENFLUID_GetLatestVariable(TU, "prec.bigger").value()->asDoubleValue()*10;
      OPENFLUID_AppendVariable(TU,"prec.bigger", NewValue);
      NewValue = OPENFLUID_GetLatestVariable(TU, "prec.smaller").value()->asDoubleValue()/10;
      OPENFLUID_AppendVariable(TU,"prec.smaller", NewValue);
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


DEFINE_SIMULATOR_CLASS(PrecisionSimulator)

