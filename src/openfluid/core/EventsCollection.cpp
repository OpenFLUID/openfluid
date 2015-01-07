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
  @brief Implements distributed event collection management class

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/

#include <openfluid/core/EventsCollection.hpp>
#include <iostream>

namespace openfluid { namespace core
{



EventsCollection::EventsCollection()
{
}


// =====================================================================
// =====================================================================


EventsCollection::~EventsCollection()
{
}


// =====================================================================
// =====================================================================


bool EventsCollection::addEvent(const Event* Ev)
{
  return addEvent(*Ev);
}


// =====================================================================
// =====================================================================


bool EventsCollection::addEvent(const Event& Ev)
{


  // empty list
  if (m_Events.size() == 0)
  {
    m_Events.push_back(Ev);
  }
  else
  {
    // event date is before first collection item
    if (m_Events.front().getDateTime() >= Ev.getDateTime())
    {
      m_Events.push_front(Ev);
    }
    else
    {
      // event date is after last collection item
      if (m_Events.back().getDateTime() <= Ev.getDateTime())
      {
        m_Events.push_back(Ev);
      }
      else
      {
        // event has to be inserted somewhere in the collection
        EventsList_t::iterator DEiter;

        for(DEiter=m_Events.begin(); DEiter != m_Events.end(); ++DEiter)
        {
          if ((*DEiter).getDateTime() >= Ev.getDateTime())
          {
            m_Events.insert(--DEiter,Ev);
            return true;
          }
        }

      }
    }
  }

  return true;
}


// =====================================================================
// =====================================================================


bool EventsCollection::getEventsBetween(const DateTime BeginDate, const DateTime EndDate,
    EventsCollection *Events) const
{
  return getEventsBetween(BeginDate,EndDate,*Events);
}


// =====================================================================
// =====================================================================


bool EventsCollection::getEventsBetween(const DateTime BeginDate, const DateTime EndDate,
    EventsCollection& Events) const
{

  EventsList_t::const_iterator DEiter;

  for(DEiter=m_Events.begin(); DEiter != m_Events.end(); ++DEiter)
  {
    if ((*DEiter).getDateTime().isBetween(BeginDate,EndDate))
    {
      Events.addEvent(*DEiter);
    }
  }

  return true;
}


// =====================================================================
// =====================================================================


void EventsCollection::println() const
{
  EventsList_t::const_iterator DEiter;

    for(DEiter=m_Events.begin(); DEiter != m_Events.end(); ++DEiter)
    {
      (*DEiter).println();
    }
}




} }  // namespaces


