/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file Attributes.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */


#include <openfluid/core/Attributes.hpp>

namespace openfluid { namespace core {

// =====================================================================
// =====================================================================


Attributes::Attributes()
{
}

// =====================================================================
// =====================================================================



Attributes::~Attributes()
{
}

// =====================================================================
// =====================================================================

bool Attributes::setValue(const AttributeName_t aName, const Value& aValue)
{
  if (isAttributeExist(aName))
    return false;

  m_Data[aName] = StringValue(aValue.toString());

  return true;
}


// =====================================================================
// =====================================================================


bool Attributes::setValue(const AttributeName_t aName, const std::string& aValue)
{
  if (isAttributeExist(aName))
    return false;

  m_Data[aName] = StringValue(aValue);

  return true;
}


// =====================================================================
// =====================================================================


bool Attributes::getValue(const AttributeName_t aName, openfluid::core::StringValue& aValue) const
{
  // Don't use m_data[aName] because [] operator is not const

  AttributesMap_t::const_iterator it = m_Data.find(aName);

  if(it != m_Data.end())
  {
    aValue.set(it->second.get());

    return true;
  }

  return false;
}

// =====================================================================
// =====================================================================

bool Attributes::getValue(const AttributeName_t aName, std::string& aValue) const
{
  AttributesMap_t::const_iterator it = m_Data.find(aName);

  if(it != m_Data.end())
  {
    aValue = it->second.get();

    return true;
  }

  return false;
}

// =====================================================================
// =====================================================================


bool Attributes::getValueAsDouble(const AttributeName_t aName, double& aValue) const
{
  AttributesMap_t::const_iterator it = m_Data.find(aName);

  if(it != m_Data.end())
    return it->second.toDouble(aValue);

  return false;
}

// =====================================================================
// =====================================================================


bool Attributes::getValueAsLong(const AttributeName_t aName, long& aValue) const
{
  AttributesMap_t::const_iterator it = m_Data.find(aName);

  if(it != m_Data.end())
    return it->second.toInteger(aValue);

  return false;
}

// =====================================================================
// =====================================================================


bool Attributes::isAttributeExist(const AttributeName_t aName) const
{
  return m_Data.find(aName) != m_Data.end();
}

// =====================================================================
// =====================================================================


std::vector<AttributeName_t> Attributes::getAttributesNames() const
{
  std::vector<AttributeName_t> TheNames;

  AttributesMap_t::const_iterator it;

  for (it = m_Data.begin(); it != m_Data.end(); ++it)
    TheNames.push_back(it->first);

  return TheNames;
}

// =====================================================================
// =====================================================================


bool Attributes::replaceValue(const AttributeName_t aName, const StringValue& aValue)
{
  if(isAttributeExist(aName))
  {
    m_Data[aName].set(aValue.get());

    return true;
  }

  return false;
}

// =====================================================================
// =====================================================================


bool Attributes::replaceValue(const AttributeName_t aName, const std::string& aValue)
{
  if(isAttributeExist(aName))
  {
    m_Data[aName].set(aValue);

    return true;
  }

  return false;

}


// =====================================================================
// =====================================================================


bool Attributes::removeAttribute(const AttributeName_t aName)
{
  if(isAttributeExist(aName))
  {
    m_Data.erase(aName);

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


void Attributes::clear()
{
  m_Data.clear();
}

// =====================================================================
// =====================================================================

} } // namespaces
