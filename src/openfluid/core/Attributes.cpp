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
  @file Attributes.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/core/Attributes.hpp>
#include <openfluid/core/MapValue.hpp>
#include <openfluid/core/TreeValue.hpp>
#include <openfluid/core/NullValue.hpp>


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


bool Attributes::setValue(const AttributeName_t& aName, const Value& aValue)
{
  if (isAttributeExist(aName))
    return false;

  m_Data[aName].reset(aValue.clone());

  return true;
}


// =====================================================================
// =====================================================================


bool Attributes::setValue(const AttributeName_t& aName, const std::string& aValue)
{
  if (isAttributeExist(aName))
    return false;

  m_Data[aName].reset(new StringValue(aValue));

  return true;
}


// =====================================================================
// =====================================================================


bool Attributes::setValueFromRawString(const AttributeName_t& aName, const std::string& aValue)
{
  if (isAttributeExist(aName))
    return false;

  StringValue TmpStrValue(aValue);

  switch (TmpStrValue.guessTypeConversion())
  {
    case Value::DOUBLE :
    {
      double TmpVal;
      if (!TmpStrValue.toDouble(TmpVal))
        return false;
      m_Data[aName].reset(new DoubleValue(TmpVal));
      break;
    }


    case Value::INTEGER :
    {
      long TmpVal;
      if (!TmpStrValue.toInteger(TmpVal))
        return false;
      m_Data[aName].reset(new IntegerValue(TmpVal));
      break;
    }

    case Value::BOOLEAN :
    {
      bool TmpVal;
      if (!TmpStrValue.toBoolean(TmpVal))
        return false;
      m_Data[aName].reset(new BooleanValue(TmpVal));
      break;
    }

    case Value::STRING :
    {
      m_Data[aName].reset(new StringValue(aValue));
      break;
    }

    case Value::VECTOR :
    {
      VectorValue TmpVal;
      if (!TmpStrValue.toVectorValue(TmpVal))
        return false;
      m_Data[aName].reset(TmpVal.clone());
      break;
    }

    case Value::MATRIX :
    {
      MatrixValue TmpVal;
      if (!TmpStrValue.toMatrixValue(TmpVal))
        return false;
      m_Data[aName].reset(TmpVal.clone());
      break;
    }

    case Value::MAP :
    {
      MapValue TmpVal;
      if (!TmpStrValue.toMapValue(TmpVal))
        return false;
      m_Data[aName].reset(TmpVal.clone());
      break;
    }

    case Value::TREE :
    {
      TreeValue TmpVal;
      if (!TmpStrValue.toTreeValue(TmpVal))
        return false;
      m_Data[aName].reset(TmpVal.clone());
      break;
    }

    case Value::NULLL :
    {
      NullValue TmpVal;
      if (!TmpStrValue.toNullValue(TmpVal))
        return false;
      m_Data[aName].reset(TmpVal.clone());
      break;
    }

    default :
      return false;
  }

  return true;
}


// =====================================================================
// =====================================================================


bool Attributes::getValue(const AttributeName_t& aName, openfluid::core::StringValue& aValue) const
{
  // Don't use m_data[aName] because [] operator is not const

  AttributesMap_t::const_iterator it = m_Data.find(aName);

  if (it != m_Data.end())
  {
    aValue.set(it->second.get()->toString());

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


const openfluid::core::Value* Attributes::value(const AttributeName_t& aName) const
{
  AttributesMap_t::const_iterator it = m_Data.find(aName);

  if (it != m_Data.end())
    return it->second.get();

  return nullptr;
}


// =====================================================================
// =====================================================================


bool Attributes::getValue(const AttributeName_t& aName, std::string& aValue) const
{
  AttributesMap_t::const_iterator it = m_Data.find(aName);

  if (it != m_Data.end())
  {
    aValue = it->second.get()->toString();
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool Attributes::getValueAsDouble(const AttributeName_t& aName, double& aValue) const
{
  AttributesMap_t::const_iterator it = m_Data.find(aName);

  if (it != m_Data.end() && it->second.get()->isDoubleValue())
  {
    aValue = it->second.get()->asDoubleValue();
    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool Attributes::getValueAsLong(const AttributeName_t& aName, long& aValue) const
{
  AttributesMap_t::const_iterator it = m_Data.find(aName);

  if (it != m_Data.end() && it->second.get()->isIntegerValue())
  {
    aValue = it->second.get()->asIntegerValue();
    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool Attributes::isAttributeExist(const AttributeName_t& aName) const
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


bool Attributes::replaceValue(const AttributeName_t& aName, const StringValue& aValue)
{
  if(isAttributeExist(aName))
  {
    m_Data[aName].reset(new StringValue(aValue));

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool Attributes::replaceValue(const AttributeName_t& aName, const std::string& aValue)
{
  if(isAttributeExist(aName))
  {
    m_Data[aName].reset(new StringValue(aValue));

    return true;
  }

  return false;

}


// =====================================================================
// =====================================================================


bool Attributes::removeAttribute(const AttributeName_t& aName)
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
