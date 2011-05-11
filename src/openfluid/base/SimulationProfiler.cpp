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
  \file SimulationProfiler.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/base/SimulationProfiler.hpp>


namespace openfluid { namespace base {


SimulationProfiler* SimulationProfiler::mp_Singleton = NULL;


// =====================================================================
// =====================================================================


SimulationProfiler::SimulationProfiler()
: m_IsProfilingEnabled(false)
{

}

// =====================================================================
// =====================================================================


SimulationProfiler* SimulationProfiler::getInstance()
{
  if (mp_Singleton == NULL)
    mp_Singleton = new SimulationProfiler();
  return mp_Singleton;
}


// =====================================================================
// =====================================================================


void SimulationProfiler::addDuration(const openfluid::base::FuncID_t& FuncID,
                                     TimeProfilePart ProfilePart,
                                     const boost::posix_time::time_duration& Duration)
{
  if (!m_IsProfilingEnabled) return;

  if (m_ModelTimeProfile.find(FuncID) == m_ModelTimeProfile.end())
  {
    m_ModelTimeProfile[FuncID][INITPARAMS] = boost::posix_time::time_duration(0,0,0,0);
    m_ModelTimeProfile[FuncID][PREPAREDATA] = boost::posix_time::time_duration(0,0,0,0);
    m_ModelTimeProfile[FuncID][CHECKCONSISTENCY] = boost::posix_time::time_duration(0,0,0,0);
    m_ModelTimeProfile[FuncID][INITIALIZERUN] = boost::posix_time::time_duration(0,0,0,0);
    m_ModelTimeProfile[FuncID][RUNSTEP] = boost::posix_time::time_duration(0,0,0,0);
    m_ModelTimeProfile[FuncID][FINALIZERUN] = boost::posix_time::time_duration(0,0,0,0);
  }

  m_ModelTimeProfile[FuncID][ProfilePart] = m_ModelTimeProfile[FuncID][ProfilePart] + Duration;

  if (ProfilePart == RUNSTEP) m_RunStepTimeProfile[FuncID].push_back(Duration);

}


} } //namespaces
