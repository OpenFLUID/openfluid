/**
  \file EventCollection.h
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __EVENTCOLLECTION_H__
#define __EVENTCOLLECTION_H__

#include "DistributedEvent.h"
#include <list>
#include <wx/datetime.h>
 

namespace mhydasdk { namespace core {

class DistributedEvent;

class EventCollection
{
  private:
    std::list<DistributedEvent*> m_Events;
  
  public:
    EventCollection();
    
    virtual ~EventCollection();
    
    
    /**
      Insert an event in the event collection, ordered by date 
    */
    bool addEvent(DistributedEvent* Ev);
    
    bool getEventsBetween(const wxDateTime BeginDate, const wxDateTime EndDate, EventCollection *Events);
    
    std::list<DistributedEvent*> getEventsList() { return m_Events; };
    
    int getCount() const { return m_Events.size(); };
    
    void clear() { m_Events.clear(); };
    
    void println();
    
};


} } // namespace mhydasdk::core



#endif /* __EVENTCOLLECTION_H__*/
