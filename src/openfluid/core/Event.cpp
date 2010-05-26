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
  \file Event.cpp
  \brief implements distributed event definition class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include <openfluid/core/Event.hpp>
#include <openfluid/tools.hpp>
#include <iostream>

namespace openfluid { namespace core {

Event::Event()
{
  m_Infos.clear();
  m_Date = DateTime();
}

// =====================================================================
// =====================================================================


Event::Event(DateTime Date)
{
  m_Infos.clear();
  m_Date = Date;
}

// =====================================================================
// =====================================================================


Event::~Event()
{

}


// =====================================================================
// =====================================================================


bool Event::isInfoExist(std::string Key)
{
  return !(m_Infos.find(Key) == m_Infos.end());
}

// =====================================================================
// =====================================================================


bool Event::isInfoEqual(std::string Key, std::string Value)
{
  std::string TmpValue;

  return (getInfoAsString(Key,&TmpValue) && (TmpValue == Value));

}

// =====================================================================
// =====================================================================


bool Event::isInfoEqual(std::string Key, long Value)
{
 long TmpValue;

  return (getInfoAsLong(Key,&TmpValue) && (TmpValue == Value));

}

// =====================================================================
// =====================================================================


bool Event::isInfoEqual(std::string Key, double Value)
{
  double TmpValue;

  return (getInfoAsDouble(Key,&TmpValue) && (openfluid::tools::IsCloseEnough(TmpValue,Value,0.00001)));

}

// =====================================================================
// =====================================================================


bool Event::isInfoEqual(std::string Key, ScalarValue *Value)
{
  ScalarValue TmpValue;

  return (getInfoAsScalarValue(Key,&TmpValue) && (openfluid::tools::IsCloseEnough(TmpValue,*Value,0.00001)));

}


// =====================================================================
// =====================================================================


bool Event::getInfoAsString(std::string Key, std::string *Info)
{

  if (m_Infos.find(Key) == m_Infos.end())
  {
    return false;
  }
  else *Info = m_Infos[Key];

  return true;
}

// =====================================================================
// =====================================================================


bool Event::getInfoAsLong(std::string Key, long *Info)
{

  std::string InfoStr;

  return ((getInfoAsString(Key,&InfoStr)) && (openfluid::tools::ConvertString(InfoStr,Info)));
}

// =====================================================================
// =====================================================================


bool Event::getInfoAsDouble(std::string Key, double *Info)
{
  std::string InfoStr;

  return ((getInfoAsString(Key,&InfoStr)) && (openfluid::tools::ConvertString(InfoStr,Info)));
}

// =====================================================================
// =====================================================================


bool Event::getInfoAsScalarValue(std::string Key, ScalarValue *Info)
{
  return getInfoAsDouble(Key,Info);
}


// =====================================================================
// =====================================================================

bool Event::addInfo(std::string Key, std::string Info)
{
  if (m_Infos.find(Key) != m_Infos.end())
  {
    return false;
  }
  else m_Infos[Key] = Info;

  return true;
}

// =====================================================================
// =====================================================================


void Event::println()
{
  std::cerr << "Date : " << m_Date.getAsISOString() << std::endl;
  std::cerr << "Infos : " <<std::endl;

  EventInfosMap_t::iterator it;
  for (it = m_Infos.begin();it != m_Infos.end();++it)
  {
    std::cerr << " - " << it->first << " = " << it->second << std::endl;
  }
}



} }
