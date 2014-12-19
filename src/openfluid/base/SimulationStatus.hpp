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
  \file SimulationStatus.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BASE_SIMULATIONSTATUS_HPP__
#define __OPENFLUID_BASE_SIMULATIONSTATUS_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/core/DateTime.hpp>


namespace openfluid { namespace base {


// =====================================================================
// =====================================================================


class OPENFLUID_API SimulationStatus
{
  public:

    enum SimulationStage { PRE, INITPARAMS, PREPAREDATA, CHECKCONSISTENCY, INITIALIZERUN, RUNSTEP, FINALIZERUN, POST, UNKNOWN };

    enum SchedulingConstraint { SCHED_NONE, SCHED_DTCHECKED, SCHED_DTFORCED };

  private:

    openfluid::core::DateTime m_BeginDate;

    openfluid::core::DateTime m_EndDate;

    openfluid::core::Duration_t m_Duration;

    openfluid::core::DateTime m_CurrentDate;

    openfluid::core::TimeIndex_t m_CurrentTimeIndex;

    openfluid::core::Duration_t m_DefaultDeltaT;

    SchedulingConstraint m_SchedConstraint;

    SimulationStage m_CurrentStage;

  public:

    SimulationStatus(const openfluid::core::DateTime& Begin,
                     const openfluid::core::DateTime& End,
                     const openfluid::core::Duration_t DeltaT,
                     const SchedulingConstraint& SConst = SCHED_NONE);

    ~SimulationStatus() {}

    inline openfluid::core::DateTime getBeginDate() const { return m_BeginDate; }

    inline openfluid::core::DateTime getEndDate() const { return m_EndDate; }

    inline openfluid::core::DateTime getCurrentDate() const { return m_CurrentDate; }

    inline openfluid::core::Duration_t getDefaultDeltaT() const { return m_DefaultDeltaT; }

    inline openfluid::core::Duration_t getSimulationDuration() const { return m_Duration; }

    inline openfluid::core::TimeIndex_t getCurrentTimeIndex() const { return m_CurrentTimeIndex; }

    inline SchedulingConstraint getSchedulingConstraint() const { return m_SchedConstraint; }

    inline bool isFirstTimeIndex() const { return m_CurrentTimeIndex == 0; }

    void setCurrentTimeIndex(const openfluid::core::TimeIndex_t& Index);

    inline SimulationStage getCurrentStage() const { return m_CurrentStage; }

    void setCurrentStage(const SimulationStage& Stage);

};

}  }  // namespaces


#endif /* __OPENFLUID_BASE_SIMULATIONSTATUS_HPP__ */
