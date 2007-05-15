/**
  \file TimeSerie.cpp
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include "TimeSerie.h"
#include <algorithm>

#include <iostream>


namespace mhydasdk { namespace core {


// =====================================================================
// =====================================================================


struct SortByDate
{
  bool operator ()(TimeSerieItem* Time1, TimeSerieItem* Time2) const
  {
    bool ReturnValue = false;


    if (Time1->getYear() < Time2->getYear())
    {
      ReturnValue = true;
    }
    else
    {
      if (Time1->getYear() == Time2->getYear())
      {
        if (Time1->getMonth() < Time2->getMonth())
        {
          ReturnValue = true;
        }
        else
        {
          if (Time1->getMonth() == Time2->getMonth())
          {
            if (Time1->getDay() < Time2->getDay())
            {
              ReturnValue = true;
            }
            else
            {
              if (Time1->getDay() == Time2->getDay())
              {
                int Seconds1 = (Time1->getHour()*3600)+(Time1->getMinute()*60)+(Time1->getSecond());
                int Seconds2 = (Time2->getHour()*3600)+(Time2->getMinute()*60)+(Time2->getSecond());

                ReturnValue = (Seconds1 <= Seconds2);
              }
            }
          }
        }
      }
    }
    return ReturnValue;
  }

};



// =====================================================================
// =====================================================================



TimeSerie::TimeSerie(std::string Unit)
{
  m_Serie = new vector<TimeSerieItem*>();
  m_Unit = Unit;
}

TimeSerie::~TimeSerie()
{
/*
  if (m_Serie != NULL)
  {
    const int SerieCount = m_Serie->size();
    TimeSerieItem* Item;

    // destruction de chaque objet du vecteur
    for (int i=0; i<SerieCount;i++)
    {
      Item = m_Serie->at(i);
      if (Item != NULL) delete Item;
    }

    delete m_Serie;
  }
*/
}


// =====================================================================
// =====================================================================


void TimeSerie::sort()
{
  std::sort(m_Serie->begin(),m_Serie->end(),SortByDate());
}

// =====================================================================
// =====================================================================

void TimeSerie::addValue(TimeSerieItem *Item)
{
  m_Serie->push_back(Item);
}

// =====================================================================
// =====================================================================

std::string TimeSerie::getUnit() const
{
  return m_Unit;
}

// =====================================================================
// =====================================================================

vector<TimeSerieItem*> *TimeSerie::getItemsCollection() const
{
  return m_Serie;
}

// =====================================================================
// =====================================================================

TimeSerieItem* TimeSerie::getFirstItem() const
{
  return m_Serie->front();
}

// =====================================================================
// =====================================================================

TimeSerieItem* TimeSerie::getLastItem() const
{
  return m_Serie->back();
}

// =====================================================================
// =====================================================================


int TimeSerie::computeTimeStep()
{

  /** \internal

    if there is only one item, we consider that there is no possible time step and -1 is returned.
    if there is 2 or more items,
    - we calculate the step between the first and the second element,
    then we compare it with all others steps (item_n minus item_n-1)
    - if there are all the same this step is returned, if not -1 is returned

  */

  int Count = m_Serie->size();
  int TimeStepToTry;

  if (Count < 2)
  {
    return -1;
  }
  else
  {
    int i;
    TimeStepToTry = m_Serie->at(1)->getRawTime() - m_Serie->at(0)->getRawTime();

    for (i=2;i<Count;i++)
    {
      if ((m_Serie->at(i)->getRawTime() - m_Serie->at(i-1)->getRawTime()) != TimeStepToTry)
      {
      /*
        std::cerr << "�a chie entre les lignes " << i-1 << " et " << i << "; Time step de " << TimeStepToTry << std::endl;
//        std::cerr << m_Serie->at(i-1)->getRawTime() << " et " << m_Serie->at(i)->getRawTime() << std::endl;
//        std::cerr << m_Serie->at(i-1)->getTimeAsString() << " et " << m_Serie->at(i)->getTimeAsString() << std::endl;
        std::cerr << "DateTime() " << m_Serie->at(i-1)->getDateTime().getRawTime() << " et " << m_Serie->at(i)->getDateTime().getRawTime() << std::endl;
        std::cerr << "DateTime() " << m_Serie->at(i-1)->getDateTime().asString() << " et " << m_Serie->at(i)->getDateTime().asString() << std::endl;
       */
        return false;
      }
    }
  }

  return TimeStepToTry;


}


// =====================================================================
// =====================================================================



bool TimeSerie::isSorted()
{
  int Count = m_Serie->size();

  if (Count < 2)
  {
    return true;
  }
  else
  {
    int i;

    for (i=1;i<Count;i++)
    {
      if (m_Serie->at(i-1)->getRawTime() >= m_Serie->at(i)->getRawTime()) return false;
    }
  }

  return true;

}



// =====================================================================
// =====================================================================

bool TimeSerie::isMatching(rawtime_t RawBeginTime, rawtime_t RawEndTime, int TimeStep)
{

  return (m_Serie->front()->getRawTime() == RawBeginTime) &&
         (m_Serie->back()->getRawTime() == RawEndTime) &&
         (computeTimeStep() == TimeStep);

}

// =====================================================================
// =====================================================================


bool TimeSerie::multiplyValuesByFactor(float Factor)
{
  
  int Count = m_Serie->size();
  
  for (int i=0; i<Count;i++)
  {
    m_Serie->at(i)->setValue(m_Serie->at(i)->getValue()*Factor);
  }
}


} } // namespace mhydasdk::core

