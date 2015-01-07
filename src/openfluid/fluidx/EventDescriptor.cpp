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
  @file
  @brief Implements ...

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/fluidx/EventDescriptor.hpp>

namespace openfluid { namespace fluidx {


EventID_t EventDescriptor::m_NextID = 0;


// =====================================================================
// =====================================================================


EventDescriptor::EventDescriptor()
: m_Event()
{
  m_ID = assignID();
}

// =====================================================================
// =====================================================================


EventDescriptor::EventDescriptor(const EventDescriptor& EvDesc)
{
  m_ID = EvDesc.m_ID;

  m_UnitClass = EvDesc.m_UnitClass;
  m_UnitID = EvDesc.m_UnitID;
  m_Event = EvDesc.m_Event;
}


// =====================================================================
// =====================================================================


EventDescriptor::~EventDescriptor()
{

}


// =====================================================================
// =====================================================================


EventID_t EventDescriptor::assignID()
{
  m_NextID++;
  return m_NextID;
}



} } // namespaces
