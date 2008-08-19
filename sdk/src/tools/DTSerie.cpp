/**
  \file DTSerie.cpp
  \brief

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/



#include "DTSerie.h"
#include <math.h>

namespace openfluid { namespace tools {


DateTimeSerie::DateTimeSerie()
{
  m_IsLastAccessedSet = false;
}

// =====================================================================
// =====================================================================


DateTimeSerie::~DateTimeSerie()
{

}

// =====================================================================
// =====================================================================


bool DateTimeSerie::addValue(openfluid::core::DateTime DT, openfluid::core::ScalarValue Value)
{
  std::list<TimePair>::iterator it;

  TimePair TP;
  TP.DT = DT;
  TP.Value = Value;



  if (m_Data.size() == 0)
  {
    m_Data.push_back(TP);
    return true;
  }


  if (TP.DT > m_Data.back().DT)
  {
    m_Data.push_back(TP);
    return true;
  }


  for(it=m_Data.begin(); it!=m_Data.end(); ++it)
  {
    if (it->DT < TP.DT )
    {
      m_Data.insert(it,TP);
      return true;
    }
  }

  return false;

}



// =====================================================================
// =====================================================================


bool DateTimeSerie::getValue(openfluid::core::DateTime DT, openfluid::core::ScalarValue* Value)
{
  std::list<TimePair>::iterator it;

  if (m_Data.size() == 0) return false;


  // init the last visit accessor
  if (!m_IsLastAccessedSet)
  {
    m_LastAccessed = m_Data.begin();
    m_IsLastAccessedSet = true;
  }

  it = m_LastAccessed;

  if (it->DT == DT)
  {
    *Value = it->Value;
    m_LastAccessed = it;
    return true;
  }
  else
  {
    if (it->DT > DT)
    {
      std::list<TimePair>::reverse_iterator rit(it);


      while (rit != m_Data.rend())
      {
        if (rit->DT == DT)
        {
          *Value = rit->Value;
          return true;
        }

        if (rit->DT < DT) return false;

        ++rit;
      }
    }
    else
    {
      while (it!=m_Data.end())
      {

        if (it->DT == DT)
        {
          *Value = it->Value;
          m_LastAccessed = it;
          return true;
        }

        if (it->DT > DT) return false;

        ++it;
      }
    }
  }
  return false;

}


// =====================================================================
// =====================================================================


short DateTimeSerie::getNearestValues(openfluid::core::DateTime SearchedDT, TimePair* LowerPair, TimePair* UpperPair)
{
  /*
    returns 0 if serie begins after the searched value
    returns 2 if serie ends before the searched value
    returns 1 if the searched value contains the searched value
    returns 3 if error

  */


  std::list<TimePair>::iterator it;
  std::list<TimePair>::iterator previt;

  if (m_Data.front().DT > SearchedDT)
  {
    LowerPair = NULL;
    *UpperPair = m_Data.front();
    return 0;
  }


  if (m_Data.back().DT < SearchedDT)
  {


    *LowerPair = m_Data.back();
    UpperPair = NULL;
    return 0;
  }


  // not before the first, not after the last
  // TODO check the following algorithm

  previt = m_Data.begin();

  for(it=m_Data.begin(); it!=m_Data.end(); ++it)
  {
    if (it->DT == SearchedDT)
    {
      *LowerPair = *it;
      *UpperPair = *it;
      return 1;
    }

    if (it->DT > SearchedDT)
    {
      *UpperPair = *it;
      *LowerPair = *previt;
      return 1;
    }
    previt = it;

  }


  return 3;
}

// =====================================================================
// =====================================================================


void DateTimeSerie::clear()
{
  m_Data.clear();
}



// =====================================================================
// =====================================================================


int DateTimeSerie::getCount()
{
  return m_Data.size();
}

// =====================================================================
// =====================================================================


bool DateTimeSerie::createInterpolatedSerie(openfluid::core::DateTime Begin,openfluid::core::DateTime End,int TimeStep, DateTimeSerie* Serie)
{
  Serie->clear();

  openfluid::core::DateTime CurrentTime;
  openfluid::core::ScalarValue InterpValue;

  CurrentTime = Begin;


  while (CurrentTime < End)
  {

    if (getInterpolatedValue(CurrentTime, &InterpValue))
    {
      Serie->addValue(CurrentTime,InterpValue);
    }
    else
    {
      return false;
    }

    CurrentTime = CurrentTime + TimeStep;
  }

  return true;
}

// =====================================================================
// =====================================================================


bool DateTimeSerie::getInterpolatedValue(openfluid::core::DateTime SearchedDT, openfluid::core::ScalarValue* Value)
{
  TimePair LowerPair;
  TimePair UpperPair;

  short ReturnVal;


  ReturnVal = getNearestValues(SearchedDT,&LowerPair,&UpperPair);

  if ( ReturnVal == 1)
  {
    openfluid::core::ScalarValue y,y0,y1;
    long x,x1;

    if (LowerPair.DT == UpperPair.DT)
    {
      *Value = LowerPair.Value;
    }
    else
    {
//      x = wxTimeSpan(SearchedDT-LowerPair.DT).GetSeconds().ToLong();
      x = SearchedDT.diffInSeconds(LowerPair.DT);
      //x1 = wxTimeSpan(UpperPair.DT-LowerPair.DT).GetSeconds().ToLong();
      x1 = UpperPair.DT.diffInSeconds(LowerPair.DT);
      y0 = LowerPair.Value;
      y1 = UpperPair.Value;

      y = y0 + ( x * ( (y1-y0)/x1));

      *Value = y;
    }
  }
  else
  {
    return false;
  }


  return true;
}

// =====================================================================
// =====================================================================

bool DateTimeSerie::createIndexedSerie(IndexedSerie *ISerie)
{

  if (m_Data.size() == 0) return false;

  ISerie->Count = m_Data.size();
  ISerie->Values = new openfluid::core::ScalarValue[ISerie->Count];

  std::list<TimePair>::iterator it;

  int i = 0;
  for(it=m_Data.begin(); it!=m_Data.end(); ++it)
  {
    ISerie->Values[i] = it->Value;
    i++;
  }

}

} } // namespace openfluid::tools
