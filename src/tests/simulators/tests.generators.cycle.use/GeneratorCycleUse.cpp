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
  @file GeneratorCycleUse.cpp

  @author Armel THONI <armel.thoni@inrae.fr>
*/


#include <stdlib.h>  

#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.generators.cycle.use")

  // Informations
  DECLARE_NAME("")
  DECLARE_DESCRIPTION("")
  DECLARE_VERSION("")
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL)
  
  DECLARE_REQUIRED_VARIABLE("var.rand.prod[double]","TU","randomly generated floating value","SI unit")
  DECLARE_PRODUCED_VARIABLE("var.indirect[double]","TU","second hand variable","SI unit")

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


/**

*/
class GeneratorCycleUseSim : public openfluid::ware::PluggableSimulator
{
  private:

  
  public:

  
    GeneratorCycleUseSim(): PluggableSimulator()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~GeneratorCycleUseSim()
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
        OPENFLUID_InitializeVariable(TU,"var.indirect",1.0);
      }
      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest runStep()
    {
      openfluid::core::SpatialUnit* TU;
      openfluid::core::DoubleValue TmpValue;
      OPENFLUID_UNITS_ORDERED_LOOP("TU",TU)
      {
        // checks that the value injected from the generator can be understood and used
        OPENFLUID_GetVariable(TU,"var.rand.prod",TmpValue);
        OPENFLUID_AppendVariable(TU,"var.indirect",TmpValue*2);
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


DEFINE_SIMULATOR_CLASS(GeneratorCycleUseSim);
