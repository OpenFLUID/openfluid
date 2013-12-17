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

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __EVENTSCOLL_H__
#define __EVENTSCOLL_H__

#include <openfluid/core/Event.hpp>
#include <openfluid/dllexport.hpp>

#include <list>


namespace openfluid { namespace core {

class DLLEXPORT Event;

typedef std::list<Event> EventsList_t;

/**
  @brief Class defining a collection of discrete events
*/
class DLLEXPORT EventsCollection
{
  private:

    EventsList_t m_Events;

  public:

    EventsCollection();

    virtual ~EventsCollection();


    /**
      Inserts an event in the event collection, ordered by date
      @deprecated
    */
    bool addEvent(const Event* Ev) OPENFLUID_DEPRECATED;

    /**
      Inserts an event in the event collection, ordered by date
    */
    bool addEvent(const Event& Ev);

    /**
      Returns an event collection extracted from the current event collection, taking into account a time period
      If some events are already in the given collection, they are not deleted. Events matching the period are appended
      at the end of the given collection
      @deprecated
    */
    bool getEventsBetween(const DateTime BeginDate, const DateTime EndDate, EventsCollection *Events) const OPENFLUID_DEPRECATED;

    /**
      Returns an event collection extracted from the current event collection, taking into account a time period
      If some events are already in the given collection, they are not deleted. Events matching the period are appended
      at the end of the given collection
    */
    bool getEventsBetween(const DateTime BeginDate, const DateTime EndDate, EventsCollection& Events) const;

    /**
      Returns the event collection as a list
    */
    inline EventsList_t* getEventsList() { return &m_Events; };

    /**
      Returns number of events in the event collection
    */
    inline int getCount() const { return m_Events.size(); };

    /**
      Clears the event collection
    */
    void clear() { m_Events.clear(); };

    void println() const;
};


} } // namespace openfluid::core



#endif /* __EVENTSCOLL_H__*/
