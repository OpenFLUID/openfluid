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
  \file TimePrimitivesSim.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN


BEGIN_SIMULATOR_SIGNATURE("tests.primitives.time")

  DECLARE_NAME("");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");


  DECLARE_SCHEDULING_DEFAULT;

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class TimePrimitivesSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    bool m_IsFirstRunStep;

  public:


    TimePrimitivesSimulator() : PluggableSimulator(), m_IsFirstRunStep(true)
    {


    }


    // =====================================================================
    // =====================================================================


    ~TimePrimitivesSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::INITPARAMS)
        OPENFLUID_RaiseError("wrong stage");
    }

    // =====================================================================
    // =====================================================================


    void prepareData()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::PREPAREDATA)
        OPENFLUID_RaiseError("prepare data");

      if (OPENFLUID_GetBeginDate() != openfluid::core::DateTime(2000,1,1,0,0,0))
        OPENFLUID_RaiseError("wrong begin date");

      if (OPENFLUID_GetEndDate() != openfluid::core::DateTime(2000,1,1,6,0,0))
        OPENFLUID_RaiseError("wrong end date");

      if (OPENFLUID_GetSimulationDuration() != 21600)
        OPENFLUID_RaiseError("wrong simulation duration");

      if (OPENFLUID_GetDefaultDeltaT() != 3600)
        OPENFLUID_RaiseError("wrong default deltaT");

      if (OPENFLUID_GetSchedulingConstraint() != openfluid::base::SimulationStatus::SCHED_NONE)
        OPENFLUID_RaiseError("scheduling constraint");

    }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::CHECKCONSISTENCY)
        OPENFLUID_RaiseError("wrong stage");
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::INITIALIZERUN)
        OPENFLUID_RaiseError("wrong stage");

      if (OPENFLUID_GetCurrentDate() != OPENFLUID_GetBeginDate())
        OPENFLUID_RaiseError("wrong current date");

      if (OPENFLUID_GetSchedulingConstraint() != openfluid::base::SimulationStatus::SCHED_NONE)
        OPENFLUID_RaiseError("scheduling constraint");

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::RUNSTEP)
        OPENFLUID_RaiseError("wrong stage");


      if (m_IsFirstRunStep)
      {
        m_IsFirstRunStep = false;

        if (OPENFLUID_GetCurrentTimeIndex() != OPENFLUID_GetDefaultDeltaT())
          OPENFLUID_RaiseError("wrong time index");

        if (OPENFLUID_GetPreviousRunTimeIndex() != 0)
          OPENFLUID_RaiseError("wrong previous run time index");
      }
      else
      {
        if (OPENFLUID_GetPreviousRunTimeIndex() != (OPENFLUID_GetCurrentTimeIndex()-47))
          OPENFLUID_RaiseError("wrong previous run time index");

        if ((OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) % 47 != 0 )
          OPENFLUID_RaiseError("wrong time index");

      }

      if (OPENFLUID_GetCurrentDate() != (OPENFLUID_GetBeginDate()+OPENFLUID_GetCurrentTimeIndex()))
        OPENFLUID_RaiseError("wrong current date");

      if (OPENFLUID_GetSchedulingConstraint() != openfluid::base::SimulationStatus::SCHED_NONE)
        OPENFLUID_RaiseError("scheduling constraint");

      return Duration(47);
    }

    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::FINALIZERUN)
        OPENFLUID_RaiseError("wrong stage");
    }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(TimePrimitivesSimulator)

