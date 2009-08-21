/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file EventCollection.h
  \brief Header of distributed event collection management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __EVENTCOLLECTION_H__
#define __EVENTCOLLECTION_H__

#include "DistributedEvent.h"
#include <list>


namespace openfluid { namespace core {

class DistributedEvent;


/**
  \brief Class defining a collection of discrete events
*/
class EventCollection
{
  private:
    std::list<DistributedEvent*> m_Events;

  public:
    EventCollection();

    virtual ~EventCollection();


    /**
      Inserts an event in the event collection, ordered by date
    */
    bool addEvent(DistributedEvent* Ev);

    /**
      Returns an event collection extracted from the current event collection, taking into account a time period
    */
    bool getEventsBetween(const DateTime BeginDate, const DateTime EndDate, EventCollection *Events);

    /**
      Returns the event collection as a list
    */
    std::list<DistributedEvent*>* getEventsList() { return &m_Events; };

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



#endif /* __EVENTCOLLECTION_H__*/
