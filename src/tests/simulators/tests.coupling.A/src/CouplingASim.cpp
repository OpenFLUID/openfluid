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
  @file CouplingASim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/tools/MiscHelpers.hpp>


class CouplingASimulator : public openfluid::ware::PluggableSimulator
{
  private:

    std::list<openfluid::base::SchedulingRequest> m_DeltaTList;

  public:


  CouplingASimulator() : PluggableSimulator()
  {
    m_DeltaTList.push_back(Duration(190));
    m_DeltaTList.push_back(Duration(180));
  }


  // =====================================================================
  // =====================================================================


  ~CouplingASimulator()
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

    OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
    {
      OPENFLUID_InitializeVariable(TU,"varA",(long)0);
    }


    openfluid::base::SchedulingRequest DT = m_DeltaTList.front();
    m_DeltaTList.pop_front();

    openfluid::tools::millisleep(5);

    return DT;
  }


  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest runStep()
  {
    openfluid::core::SpatialUnit* TU;

    OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
    {
      OPENFLUID_AppendVariable(TU,"varA",(long)OPENFLUID_GetCurrentTimeIndex());
    }


    if (m_DeltaTList.empty())
    {
      return Never();
    }

    openfluid::base::SchedulingRequest DT = m_DeltaTList.front();
    m_DeltaTList.pop_front();

    if (DT.RequestType == openfluid::base::SchedulingRequest::DURATION)
    {
      openfluid::tools::microsleep(10000/DT.Duration);
    }


    return DT;
  }


  // =====================================================================
  // =====================================================================


  void finalizeRun()
  {

  }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(CouplingASimulator)

