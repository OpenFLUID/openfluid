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
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/



#include <openfluid/base/SimStatus.hpp>
#include <iostream>

namespace openfluid { namespace base {


SimulationInfo::SimulationInfo(openfluid::core::DateTime StartTime,
                               openfluid::core::DateTime EndTime,
                               int TimeStep)

{


  openfluid::core::rawtime_t DeltaTime;

  m_StartTime = StartTime;
  m_EndTime = EndTime;

  m_TimeStep = TimeStep;

  DeltaTime = EndTime.diffInSeconds(StartTime);
  m_StepsCount = int(DeltaTime / TimeStep);
  if ((DeltaTime % TimeStep) != 0) m_StepsCount++;





}

// =====================================================================
// =====================================================================

SimulationInfo::~SimulationInfo()
{

}



// =====================================================================
// =====================================================================



SimulationStatus::SimulationStatus(openfluid::core::DateTime StartTime,
                                   openfluid::core::DateTime EndTime,
                                   int TimeStep)
                : SimulationInfo(StartTime,EndTime,TimeStep)

{

  m_CurrentStep = 0;
  m_CurrentTime = m_StartTime;

  m_IsFirstStep = true;

  m_IsLastStep = false;
  if (m_StepsCount == 1) m_IsLastStep = true;


}

// =====================================================================
// =====================================================================

SimulationStatus::~SimulationStatus()
{

}

// =====================================================================
// =====================================================================



bool SimulationStatus::switchToNextStep()
{
  openfluid::core::DateTime NextTime(m_CurrentTime + m_TimeStep);

  if (NextTime < m_EndTime)
  {
    m_CurrentStep++;

    m_CurrentTime = NextTime;

    m_IsFirstStep = (m_CurrentStep == 0);
    m_IsLastStep = (m_CurrentStep == (m_StepsCount-1));

    return true;
  }
  else return false;

}


} } // namespace openfluid::base

