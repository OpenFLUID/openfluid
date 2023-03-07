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
  @file SpatialParsingParallelSim.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


// OpenFLUID:stylecheck:!incs


//! [spatial_parsing_parallel_include]
#include <openfluid/ware/ThreadedLoopMacros.hpp>
//! [spatial_parsing_parallel_include]
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


//! [spatial_parsing_parallel]
void computeA(openfluid::core::SpatialUnit* U)
{
 // compute something
 // can use/produce variables

 openfluid::core::DoubleValue TmpValue;

 OPENFLUID_GetVariable(U,"varA",TmpValue);
}


void computeB(openfluid::core::SpatialUnit* U, 
              const double Coeff)
{
 // compute something else, with extra args
 // can use/produce variables

 openfluid::core::DoubleValue TmpValue;

 OPENFLUID_GetVariable(U,"varA",TmpValue);
 OPENFLUID_AppendVariable(U,"varB",TmpValue*Coeff);
}


openfluid::base::SchedulingRequest runStep()
{
  APPLY_UNITS_ORDERED_LOOP_THREADED("SU",SnippetsSimulator::computeA);
  APPLY_UNITS_ORDERED_LOOP_THREADED("TU",SnippetsSimulator::computeB,2.5);
  
  APPLY_ALLUNITS_ORDERED_LOOP_THREADED(SnippetsSimulator::computeA);

  return DefaultDeltaT();
}
//! [spatial_parsing_parallel]


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};

