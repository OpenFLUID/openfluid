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
  @file VariablesSim.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>


BEGIN_SIMULATOR_SIGNATURE("help.snippets.variables")

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


//! [variables]
openfluid::base::SchedulingRequest runStep()
{
  openfluid::core::DoubleValue TmpValue;
  openfluid::core::SpatialUnit* SU;

  OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
  {
    OPENFLUID_GetVariable(SU,"MyVar",TmpValue);
    TmpValue = TmpValue * 2;
    OPENFLUID_AppendVariable(SU,"MyVarX2",TmpValue);
  }
  
  return DefaultDeltaT();
}
//! [variables]


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(SnippetsSimulator)

