/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file EventsColl.h
  \brief Header of distributed event collection management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __EVENTSCOLL_H__
#define __EVENTSCOLL_H__

#include "Event.h"
#include <list>


namespace openfluid { namespace core {

class Event;


/**
  \brief Class defining a collection of discrete events
*/
class EventsCollection
{
  private:
    std::list<Event*> m_Events;

  public:
    EventsCollection();

    virtual ~EventsCollection();


    /**
      Inserts an event in the event collection, ordered by date
    */
    bool addEvent(Event* Ev);

    /**
      Returns an event collection extracted from the current event collection, taking into account a time period
      If some events are already in the given collection, they are not deleted. Events matching the period are appended
      at the end of the given collection
    */
    bool getEventsBetween(const DateTime BeginDate, const DateTime EndDate, EventsCollection *Events);

    /**
      Returns the event collection as a list
    */
    std::list<Event*>* getEventsList() { return &m_Events; };

    /**
      Returns number of events in the event collection
    */
    int getCount() const { return m_Events.size(); };

    /**
      Clears the event collection
    */
    void clear() { m_Events.clear(); };

    void println();

};


} } // namespace openfluid::core



#endif /* __EVENTSCOLL_H__*/
