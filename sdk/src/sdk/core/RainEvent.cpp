/**
  \file RainEvent.cpp
  \brief implémentation de ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "RainEvent.h"
#include "ValueFactory.h"

#include <iostream>


// =====================================================================
// =====================================================================


namespace mhydasdk { namespace core {



RainEvent::RainEvent()
{
  m_RainSrcCollection.clear();

  m_FirstSerieConstraint = false;
  m_TimeStepConstraint = false;
  m_ConstrainedTimeStep = -1;

  m_ComputedTime = false;
}


// =====================================================================
// =====================================================================


RainEvent::~RainEvent()
{

}

// =====================================================================
// =====================================================================


bool RainEvent::addRainSource(ChronDataSource *Source)
{
  bool OKToAdd = true;
  int ReturnCode = 0;

  // TimeStep constraint process
  if (m_TimeStepConstraint)
  {
    OKToAdd = (m_ConstrainedTimeStep == Source->getTimeSerie()->computeTimeStep());
    if (!OKToAdd) ReturnCode = -1;
  }


  // First serie constraint process
  if (OKToAdd && m_FirstSerieConstraint)
  {
    if (m_RainSrcCollection.size() > 0)
    {
      rawtime_t BeginTime = m_RainSrcCollection.begin()->second->getTimeSerie()->getFirstItem()->getRawTime();
      rawtime_t EndTime = m_RainSrcCollection.begin()->second->getTimeSerie()->getLastItem()->getRawTime();
      int TimeStep = m_RainSrcCollection.begin()->second->getTimeSerie()->computeTimeStep();

      OKToAdd = (Source->getTimeSerie()->isMatching(BeginTime, EndTime, TimeStep));
      if (!OKToAdd) ReturnCode = -2;

    }
  }


  // adds the gauge if constraints are satisfied
  if (OKToAdd)
  {

    m_RainSrcCollection[Source->getID()] = Source;

    m_ComputedTime = false;

  }



  return (ReturnCode == 0);

}

// =====================================================================
// =====================================================================

RainSourceMap RainEvent::getRainSourceCollection()
{
  return m_RainSrcCollection;
}


// =====================================================================
// =====================================================================


ChronDataSource* RainEvent::getRainSourceByID(cdsid_t ID)
{
  ChronDataSource* Source = NULL;

  if (m_RainSrcCollection.find(ID) != m_RainSrcCollection.end()) Source= m_RainSrcCollection[ID];

  return Source;

}



// =====================================================================
// =====================================================================


bool RainEvent::computeTimeRange(rawtime_t &First, rawtime_t &Last, rawtime_t &FirstCommon, rawtime_t &LastCommon)
{

  bool ReturnedValue = true;

  if (m_RainSrcCollection.size() > 0)
  {
    if (!m_ComputedTime)
    {
      bool FoundFirst = false;

      RainSourceMap::iterator RainSrcit;
      for(RainSrcit = m_RainSrcCollection.begin(); RainSrcit != m_RainSrcCollection.end(); ++RainSrcit )
      {
        ChronDataSource *Source = RainSrcit->second;
        ReturnedValue = true;

        if (!FoundFirst)
        {
          m_FirstTime = Source->getTimeSerie()->getFirstItem()->getRawTime();
          m_LastTime = Source->getTimeSerie()->getLastItem()->getRawTime();

          m_FirstCommonTime =  m_FirstTime;
          m_LastCommonTime = m_LastTime;

          FoundFirst = true;

        }
        else
        {

          ValueFactory<rawtime_t> Factory(1);

          m_FirstTime = Factory.getMin(m_FirstTime,Source->getTimeSerie()->getFirstItem()->getRawTime());
          m_LastTime = Factory.getMax(m_LastTime,Source->getTimeSerie()->getLastItem()->getRawTime());

          m_FirstCommonTime = Factory.getMax(m_FirstCommonTime,Source->getTimeSerie()->getFirstItem()->getRawTime());
          m_LastCommonTime = Factory.getMin(m_LastCommonTime,Source->getTimeSerie()->getLastItem()->getRawTime());

        }
      }
      m_ComputedTime = true;
    }
  }
  else
  {
    ReturnedValue = false;
  }

  First = m_FirstTime;
  Last = m_LastTime;
  FirstCommon = m_FirstCommonTime;
  LastCommon = m_LastCommonTime;

  return ReturnedValue;

}


// =====================================================================
// =====================================================================



void RainEvent::enableTimeStepConstraint(bool Enabled, int TimeStep)
{
  m_TimeStepConstraint = Enabled;
  m_ConstrainedTimeStep = TimeStep;

}


// =====================================================================
// =====================================================================


void RainEvent::enableFirstSerieConstraint(bool Enabled)
{
  m_FirstSerieConstraint = Enabled;

}


// =====================================================================
// =====================================================================



DateTime RainEvent::getEventStartingTime()
{
  return m_RainSrcCollection.begin()->second->getTimeSerie()->getFirstItem()->getDateTime();
}


// =====================================================================
// =====================================================================


DateTime RainEvent::getEventEndingTime()
{
  return m_RainSrcCollection.begin()->second->getTimeSerie()->getLastItem()->getDateTime();
}



} } // namespace mhydasdk::core





