/**
  \file RainSources.cpp
  \brief implementation de ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "RainSources.h"
#include "ValueFactory.h"

#include <iostream>


// =====================================================================
// =====================================================================


namespace mhydasdk { namespace core {



RainSources::RainSources()
{
  m_RainSrcCollection.clear();

  m_FirstSerieConstraint = false;
  m_TimeStepConstraint = false;
  m_ConstrainedTimeStep = -1;

  m_ComputedTime = false;
}


// =====================================================================
// =====================================================================


RainSources::~RainSources()
{

}

// =====================================================================
// =====================================================================


bool RainSources::addRainSource(ChronDataSource *Source)
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

RainSourceMap RainSources::getRainSourceCollection()
{
  return m_RainSrcCollection;
}


// =====================================================================
// =====================================================================


ChronDataSource* RainSources::getRainSourceByID(cdsid_t ID)
{
  ChronDataSource* Source = NULL;

  if (m_RainSrcCollection.find(ID) != m_RainSrcCollection.end()) Source= m_RainSrcCollection[ID];

  return Source;

}



// =====================================================================
// =====================================================================


bool RainSources::computeTimeRange(rawtime_t &First, rawtime_t &Last, rawtime_t &FirstCommon, rawtime_t &LastCommon)
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



void RainSources::enableTimeStepConstraint(bool Enabled, int TimeStep)
{
  m_TimeStepConstraint = Enabled;
  m_ConstrainedTimeStep = TimeStep;

}


// =====================================================================
// =====================================================================


void RainSources::enableFirstSerieConstraint(bool Enabled)
{
  m_FirstSerieConstraint = Enabled;

}


// =====================================================================
// =====================================================================



DateTime RainSources::getStartingTime()
{
  return m_RainSrcCollection.begin()->second->getTimeSerie()->getFirstItem()->getDateTime();
}


// =====================================================================
// =====================================================================


DateTime RainSources::getEndingTime()
{
  return m_RainSrcCollection.begin()->second->getTimeSerie()->getLastItem()->getDateTime();
}


// =====================================================================
// =====================================================================

bool RainSources::ProcessRainSources(int TimeStep)
{
  

//  if (m_TimeStepConstraint && (m_ConstrainedTimeStep == TimeStep))
//  {
  
    RainSourceMap::iterator it;
    ChronDataSource *CDS;
       
    // pour tout les pluvios
    for (it = m_RainSrcCollection.begin(); it != m_RainSrcCollection.end(); ++it )
    {
      CDS = it->second;         
      
      CDS->Process();
    
    }

    return true;
//  }
//  else return false;
      
}


} } // namespace mhydasdk::core





