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
  \file MapValue.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include <openfluid/core/MapValue.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace core {


// =====================================================================
// =====================================================================


MapValue::MapValue(const MapValue& Val)
: CompoundValue()
{
  m_Value.clear();

  for (Map_t::const_iterator it=Val.m_Value.begin();it!=Val.m_Value.end();++it)
  {
    m_Value[(*it).first].reset((*(*it).second).clone());
  }
};


// =====================================================================
// =====================================================================


Value& MapValue::operator =(const Value& Other)
{
  if (this == &Other) return *this;

  const MapValue* CastedValue = dynamic_cast<const MapValue*> (&Other);

  if (CastedValue)
  {
    m_Value = CastedValue->m_Value;
  }

  return *this;
}


// =====================================================================
// =====================================================================


MapValue::~MapValue()
{
  clear();
};


// =====================================================================
// =====================================================================


void MapValue::writeToStream(std::ostream& OutStm) const
{

  if (m_Value.empty())
  {
    OutStm << "empty";
  }
  else
  {
    Map_t::const_iterator it;

    for (it=m_Value.begin(); it!=m_Value.end(); ++it)
    {
      if (it != m_Value.begin()) OutStm << m_StreamSeparators[0];
      OutStm << (*it).first << "=" << (*(*it).second);
    }
  }

}


// =====================================================================
// =====================================================================


void MapValue::set(const std::string& Key, Value* Element)
{
  m_Value[Key].reset(Element);
}


// =====================================================================
// =====================================================================


Value& MapValue::operator[](const std::string& Key)
{
  if (!isKeyExist(Key))
    throw openfluid::base::FrameworkException("MapValue::operator[]","Requested key " + Key + " does not exist");

  return (*(m_Value[Key]));
}


// =====================================================================
// =====================================================================


Value& MapValue::get(const std::string& Key)
{
  if (!isKeyExist(Key))
    throw openfluid::base::FrameworkException("MapValue::get","Requested key " + Key + " does not exist");

  return (*(m_Value[Key]));
}


// =====================================================================
// =====================================================================


bool MapValue::remove(const std::string& Key)
{
  return (m_Value.erase(Key) == 1);
}


// =====================================================================
// =====================================================================


void MapValue::clear()
{
  m_Value.clear();
}

} }  // namespaces



