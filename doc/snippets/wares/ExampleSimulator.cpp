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
  @file ExampleSimulator.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


// OpenFLUID:stylecheck:!incs


//! [example_full]
#include <openfluid/ware/PluggableSimulator.hpp>


class ExampleSimulator : public openfluid::ware::PluggableSimulator
{
  private:  


  public:


    ExampleSimulator(): PluggableSimulator()
    {
      // Here is source code for constructor
    }


    // =====================================================================
    // =====================================================================


    ~ExampleSimulator()
    {
      // Here is source code for destructor
    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {
      // Here is source code for processing simulator parameters
    }


    // =====================================================================
    // =====================================================================


    void prepareData()
    {
      // Here is source code for data preparation
    }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {  
      // Here is source code for specific consistency checking
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {
      // Here is source code for initialization

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      // Here is source code for each time step

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {
      // Here is source code for finalization
    }

};


DEFINE_SIMULATOR_CLASS(ExampleSimulator)
//! [example_full]

