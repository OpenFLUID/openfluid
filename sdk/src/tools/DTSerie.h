/**
  \file DTSerie.h
  \brief 
  
  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __DTSERIE_H__
#define __DTSERIE_H__


#include <openfluid-core.h>
#include <list>

namespace openfluid { namespace tools {

struct TimePair
{
  wxDateTime DT;
  openfluid::core::ScalarValue Value;  
};



struct IndexedSerie
{
  int Count;  
  openfluid::core::ScalarValue* Values;
  
  IndexedSerie()
  {
    Count = 0;
  }
};


/**
  Class for management of a time ordered serie of data (ScalarValue)
*/
class DateTimeSerie
{
  private:
    std::list<TimePair> m_Data;

    std::list<TimePair>::iterator m_LastAccessed;
    bool m_IsLastAccessedSet;
    
  public:

    DateTimeSerie();

    virtual ~DateTimeSerie();
    
    bool addValue(wxDateTime DT, openfluid::core::ScalarValue Value);
    
    bool getValue(wxDateTime DT, openfluid::core::ScalarValue* Value);    

    short getNearestValues(wxDateTime SearchedDT, TimePair* LowerPair, TimePair* UpperPair);    
    
    bool getInterpolatedValue(wxDateTime SearchedDT, openfluid::core::ScalarValue* Value);

    void clear();
    
    bool createInterpolatedSerie(wxDateTime Begin,wxDateTime End,int TimeStep,DateTimeSerie *Serie);
    
    bool createIndexedSerie(IndexedSerie *ISerie);
    
    int getCount();
    
};


} } // namespace openfluid::tools

#endif /*DTSERIE_H_*/
