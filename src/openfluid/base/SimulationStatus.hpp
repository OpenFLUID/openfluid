/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


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


#ifndef __SIMULATIONSTATUS_HPP__
#define __SIMULATIONSTATUS_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/core/DateTime.hpp>


namespace openfluid { namespace base {


// =====================================================================
// =====================================================================


class DLLEXPORT SimulationStatus
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


#endif /* __SIMULATIONSTATUS_HPP__ */
