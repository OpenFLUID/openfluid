/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


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



  // check the following algorithm?

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
      x = SearchedDT.diffInSeconds(LowerPair.DT);
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

  return true;
}

} } // namespace openfluid::tools
