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
 @file Variables.cpp

 @author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <openfluid/core/Variables.hpp>

namespace openfluid {
namespace core {

// =====================================================================
// =====================================================================


Variables::Variables()
{

}

// =====================================================================
// =====================================================================


Variables::~Variables()
{

}

// =====================================================================
// =====================================================================


bool Variables::createVariable(const VariableName_t& aName)
{
  if (!isVariableExist(aName))
  {
    m_Data[aName].second = Value::NONE;
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool Variables::createVariable(const VariableName_t& aName, const Value::Type& aType)
{
  if (!isVariableExist(aName))
  {
    m_Data[aName].second = aType;
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================

/**
 * The existing Variable must be untyped (NONE), otherwise the expecting Value must be
 * either a NullValue or the same type than the existing Variable.
 */
bool Variables::modifyValue(const VariableName_t& aName, const TimeIndex_t& anIndex,
    const Value& aValue)
{
  if (isVariableExist(aName, anIndex)
      && (m_Data[aName].second == openfluid::core::Value::NONE
          || aValue.getType() == openfluid::core::Value::NULLL
          || m_Data[aName].second == aValue.getType()))
    return m_Data[aName].first.modifyValue(anIndex, aValue);

  return false;
}

// =====================================================================
// =====================================================================

/**
 * The existing Variable must be untyped (NONE), otherwise the expecting Value must be
 * either a NullValue or the same type than the existing Variable.
 */
bool Variables::modifyCurrentValue(const VariableName_t& aName, const Value& aValue)
{
  if (isVariableExist(aName)
      && (m_Data[aName].second == openfluid::core::Value::NONE
          || aValue.getType() == openfluid::core::Value::NULLL
          || m_Data[aName].second == aValue.getType()))
    return m_Data[aName].first.modifyCurrentValue(aValue);

  return false;
}


// =====================================================================
// =====================================================================

/**
 * The existing Variable must be untyped (NONE), otherwise the expecting Value must be
 * either a NullValue or the same type than the existing Variable.
 */
bool Variables::appendValue(const VariableName_t& aName, const TimeIndex_t& anIndex, const Value& aValue)
{
  if (isVariableExist(aName)
      && (m_Data[aName].second == openfluid::core::Value::NONE
          || aValue.getType() == openfluid::core::Value::NULLL
          || m_Data[aName].second == aValue.getType()))
    return m_Data[aName].first.appendValue(anIndex,aValue);

  return false;
}


// =====================================================================
// =====================================================================


bool Variables::getValue(const VariableName_t& aName, const TimeIndex_t& anIndex,
    Value* aValue) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  return (it != m_Data.end() && it->second.first.getValue(anIndex, aValue));
}

// =====================================================================
// =====================================================================

Value* Variables::value(const VariableName_t& aName, const TimeIndex_t& anIndex) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  if (it != m_Data.end())
    return it->second.first.value(anIndex);

  return (Value*) 0;
}

// =====================================================================
// =====================================================================


Value* Variables::currentValue(const VariableName_t& aName) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  if (it != m_Data.end())
    return it->second.first.currentValue();

  return (Value*) 0;
}


// =====================================================================
// =====================================================================


bool Variables::getCurrentValue(const VariableName_t& aName, Value* aValue) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  return (it != m_Data.end() && it->second.first.getCurrentValue(aValue));
}


// =====================================================================
// =====================================================================


bool Variables::getLatestIndexedValue(const VariableName_t& aName, IndexedValue& IndValue) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  return (it != m_Data.end() && it->second.first.getLatestIndexedValue(IndValue));
}


// =====================================================================
// =====================================================================


bool Variables::getLatestIndexedValues(const VariableName_t& aName, const TimeIndex_t& anIndex, IndexedValueList& IndValueList) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  return (it != m_Data.end() && it->second.first.getLatestIndexedValues(anIndex,IndValueList));
}


// =====================================================================
// =====================================================================


bool Variables::getIndexedValues(const VariableName_t& aName,
                                 const TimeIndex_t& aBeginIndex, const TimeIndex_t& anEndIndex,
                                 IndexedValueList& IndValueList) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  return (it != m_Data.end() && it->second.first.getIndexedValues(aBeginIndex,anEndIndex,IndValueList));

}


// =====================================================================
// =====================================================================


Value* Variables::currentValueIfIndex(const VariableName_t& aName, const TimeIndex_t& Index) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  if (it != m_Data.end() && it->second.first.getCurrentIndex() == Index)
    return it->second.first.currentValue();

  return (Value*) 0;
}


// =====================================================================
// =====================================================================

bool Variables::getCurrentValueIfIndex(const VariableName_t& aName, const TimeIndex_t& Index, Value* aValue) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  return (it != m_Data.end() && it->second.first.getCurrentIndex() == Index && it->second.first.getCurrentValue(aValue));
}


// =====================================================================
// =====================================================================


bool Variables::isVariableExist(const VariableName_t& aName) const
{
  return m_Data.find(aName) != m_Data.end();
}

// =====================================================================
// =====================================================================


bool Variables::isVariableExist(const VariableName_t& aName,
                                const TimeIndex_t& anIndex) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  return (it != m_Data.end() && it->second.first.isValueExist(anIndex));
}

// =====================================================================
// =====================================================================


bool Variables::isVariableExist(const VariableName_t& aName, const TimeIndex_t& anIndex,
    Value::Type ValueType) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  return (it != m_Data.end() && it->second.first.isValueExist(anIndex) && it->second.first.value(anIndex)->getType() == ValueType);
}

// =====================================================================
// =====================================================================


bool Variables::isTypedVariableExist(const VariableName_t& aName, const Value::Type& VarType) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  return (it != m_Data.end() && it->second.second == VarType);
}

// =====================================================================
// =====================================================================


bool Variables::isTypedVariableExist(const VariableName_t& aName,
                                     const TimeIndex_t& anIndex, const Value::Type& VarType) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  return (it != m_Data.end() && it->second.first.isValueExist(anIndex) && it->second.second == VarType);
}

// =====================================================================
// =====================================================================


std::vector<VariableName_t> Variables::getVariablesNames() const
{
  std::vector<VariableName_t> TheNames;

  for (VariablesMap_t::const_iterator it = m_Data.begin(); it != m_Data.end(); ++it)
    TheNames.push_back(it->first);

  return TheNames;
}

// =====================================================================
// =====================================================================


int Variables::getVariableValuesCount(const VariableName_t& aName) const
{

  VariablesMap_t::const_iterator it = m_Data.find(aName);

  if (it == m_Data.end())
    return -1;

  return it->second.first.getValuesCount();
}


// =====================================================================
// =====================================================================


bool Variables::isAllVariablesCount(unsigned int Count) const
{
  for (VariablesMap_t::const_iterator it = m_Data.begin(); it != m_Data.end(); ++it)
  {
    if (it->second.first.getValuesCount() != Count)
      return false;
  }

  return true;
}

// =====================================================================
// =====================================================================


void Variables::clear()
{
  m_Data.clear();
}


// =====================================================================
// =====================================================================


void Variables::displayContent(const VariableName_t& aName, std::ostream& OStream) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  if (it != m_Data.end())
  {
    OStream << "Variable " << aName << std::endl;
    it->second.first.displayContent(OStream);
  }
}

} } //namespaces
