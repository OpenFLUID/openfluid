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
  @file AlternateSim.cpp

  @author Armel THONI <armel.thoni@inra.fr>
 */


#include <openfluid/debug.hpp>
#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.na.alternate")

  DECLARE_NAME("test simulator for missing value check with value generated every DeltaT*2");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_STATUS(openfluid::ware::STABLE);

  DECLARE_PRODUCED_VARIABLE("na.alternate","TU","Saved variable one step out of two","");

  DECLARE_SCHEDULING_DEFAULT;

END_SIMULATOR_SIGNATURE

/**

*/
class AlternateSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


  AlternateSimulator() : PluggableSimulator()
  {

  }


  // =====================================================================
  // =====================================================================


  ~AlternateSimulator()
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
      OPENFLUID_InitializeVariable(TU,"na.alternate",20.20);
    }
    return DefaultDeltaT();
  }


  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest runStep()
  {
      
    static bool Register = false;
    
    if (Register)
    {
      openfluid::core::SpatialUnit* TU;

      OPENFLUID_UNITS_ORDERED_LOOP("TU",TU)
      {
        OPENFLUID_AppendVariable(TU,"na.alternate",22.22);
      }
    }
    Register = !Register;
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


DEFINE_SIMULATOR_CLASS(AlternateSimulator)

