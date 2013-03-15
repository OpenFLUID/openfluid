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
  \file SimulationStatus.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/base/SimulationStatus.hpp>
#include <openfluid/base/OFException.hpp>


namespace openfluid { namespace base {


// =====================================================================
// =====================================================================

SimulationStatus::SimulationStatus(const openfluid::core::DateTime& Begin,
                                   const openfluid::core::DateTime& End,
                                   const openfluid::core::Duration_t DeltaT)
: m_BeginDate(Begin), m_EndDate(End), m_CurrentDate(Begin),
  m_CurrentTimeIndex(0), m_DefaultDeltaT(DeltaT),m_CurrentStage(PRE)
{
  m_Duration = openfluid::core::Duration_t(End.diffInSeconds(Begin));
}


// =====================================================================
// =====================================================================


void SimulationStatus::setCurrentTimeIndex(const openfluid::core::TimeIndex_t& Index)
{
  if (m_CurrentStage != RUNSTEP)
    throw OFException("OpenFLUID framework","SimulationStatus::setCurrentTimeIndex()","Setting a time index is allowed during RUNSTEP stage only");

  if (Index > m_Duration)
    throw OFException("OpenFLUID framework","SimulationStatus::setCurrentTimeIndex()","Setting a time index greater than simulation duration is not allowed");

  if (Index < m_CurrentTimeIndex)
    throw OFException("OpenFLUID framework","SimulationStatus::setCurrentTimeIndex()","Setting a time index lesser than current time index is not allowed");

  m_CurrentTimeIndex = Index;
  m_CurrentDate = m_BeginDate+m_CurrentTimeIndex;
}


// =====================================================================
// =====================================================================


void SimulationStatus::setCurrentStage(const SimulationStage& Stage)
{
  if (Stage < m_CurrentStage)
    throw OFException("OpenFLUID framework","SimulationStatus::setCurrentStage()","Setting a simulation stage previous to the current stage is not allowed");

  m_CurrentStage = Stage;
}



}  }  // namespaces
