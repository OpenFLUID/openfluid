/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file Event.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/core/Event.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>
#include <openfluid/tools/MiscHelpers.hpp>


namespace openfluid { namespace core {


Event::Event():
  m_Date(DateTime())
{

}


// =====================================================================
// =====================================================================


Event::Event(const DateTime& Date):
  m_Date(Date)
{

}


// =====================================================================
// =====================================================================


bool Event::isInfoExist(const std::string& Key) const
{
  return m_Infos.count(Key);
}


// =====================================================================
// =====================================================================


bool Event::isInfoEqual(const std::string& Key, const std::string& Value) const
{
  return (isInfoExist(Key) && m_Infos.at(Key).data() == Value);
}


// =====================================================================
// =====================================================================


bool Event::isInfoEqual(const std::string& Key, const long Value) const
{
  long TmpValue;

  return (getInfoAsLong(Key,TmpValue) && TmpValue == Value);
}


// =====================================================================
// =====================================================================


bool Event::isInfoEqual(const std::string& Key, const double Value) const
{
  double TmpValue;

  return (getInfoAsDouble(Key,TmpValue) &&
          openfluid::scientific::isCloseEnough(TmpValue,Value,0.00001));
}


// =====================================================================
// =====================================================================


bool Event::isInfoEqual(const std::string& Key, const double* Value) const
{
  return isInfoEqual(Key,*Value);
}


// =====================================================================
// =====================================================================


bool Event::isInfoEqual(const std::string& Key, const DoubleValue& Value) const
{
  DoubleValue TmpValue;

  return (getInfoAsDoubleValue(Key,TmpValue) &&
          openfluid::scientific::isCloseEnough(TmpValue.get(),Value.get(),0.00001));
}


// =====================================================================
// =====================================================================


bool Event::getInfoAsString(const std::string& Key, std::string* Info) const
{
  return getInfoAsString(Key,*Info);
}


// =====================================================================
// =====================================================================


bool Event::getInfoAsString(const std::string& Key, std::string& Info) const
{
  if(isInfoExist(Key))
  {
    Info = m_Infos.at(Key).data();

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool Event::getInfoAsLong(const std::string& Key, long *Info) const
{
  return getInfoAsLong(Key,*Info);
}


// =====================================================================
// =====================================================================


bool Event::getInfoAsLong(const std::string& Key, long& Info) const
{
  return (isInfoExist(Key) && m_Infos.at(Key).toInteger(Info));
}


// =====================================================================
// =====================================================================


bool Event::getInfoAsDouble(const std::string& Key, double *Info) const
{
  return getInfoAsDouble(Key,*Info);
}


// =====================================================================
// =====================================================================


bool Event::getInfoAsDouble(const std::string& Key, double& Info) const
{
  return (isInfoExist(Key) && m_Infos.at(Key).toDouble(Info));
}


// =====================================================================
// =====================================================================


bool Event::getInfoAsDoubleValue(const std::string& Key, DoubleValue& Info) const
{
  return (isInfoExist(Key) && m_Infos.at(Key).toDoubleValue(Info));
}


// =====================================================================
// =====================================================================


bool Event::addInfo(const std::string& Key, const std::string& Info)
{
  if (!isInfoExist(Key))
  {
    m_Infos[Key] = openfluid::core::StringValue(Info);

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


void Event::println() const
{
  std::cout << "Date : " << m_Date.getAsISOString() << std::endl;
  std::cout << "Infos : " << std::endl;

  for (const auto&  Ev : m_Infos)
  {
    std::cout << " - " << Ev.first << " = " << Ev.second.data() << std::endl;
  }
}


} }  // namespaces
