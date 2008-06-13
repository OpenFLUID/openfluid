/**
  \file DTSerie.h
  \brief 
  
  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __DTSERIE_H__
#define __DTSERIE_H__


#include <mhydasdk-core.h>
#include <list>

namespace mhydasdk { namespace tools {

struct TimePair
{
  wxDateTime DT;
  mhydasdk::core::MHYDASScalarValue Value;  
};



struct IndexedSerie
{
  int Count;  
  mhydasdk::core::MHYDASScalarValue* Values;
  
  IndexedSerie()
  {
    Count = 0;
  }
};


/**
  Class for management of a time ordered serie of data (MHYDASScalarValue)
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
    
    bool addValue(wxDateTime DT, mhydasdk::core::MHYDASScalarValue Value);
    
    bool getValue(wxDateTime DT, mhydasdk::core::MHYDASScalarValue* Value);    

    short getNearestValues(wxDateTime SearchedDT, TimePair* LowerPair, TimePair* UpperPair);    
    
    bool getInterpolatedValue(wxDateTime SearchedDT, mhydasdk::core::MHYDASScalarValue* Value);

    void clear();
    
    bool createInterpolatedSerie(wxDateTime Begin,wxDateTime End,int TimeStep,DateTimeSerie *Serie);
    
    bool createIndexedSerie(IndexedSerie *ISerie);
    
    int getCount();
    
};


} } // namespace mhydasdk::tools

#endif /*DTSERIE_H_*/
