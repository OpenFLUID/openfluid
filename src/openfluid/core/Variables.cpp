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
 \file Variables.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
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


bool Variables::createVariable(const VariableName_t aName)
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


bool Variables::createVariable(const VariableName_t aName, const Value::Type aType)
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
bool Variables::modifyValue(const VariableName_t aName, const TimeStep_t aStep,
    const Value& aValue)
{
  if (isVariableExist(aName, aStep)
      && (m_Data[aName].second == openfluid::core::Value::NONE
          || aValue.getType() == openfluid::core::Value::NULLL
          || m_Data[aName].second == aValue.getType()))
    return m_Data[aName].first.modifyValue(aStep, aValue);

  return false;
}

// =====================================================================
// =====================================================================

/**
 * The existing Variable must be untyped (NONE), otherwise the expecting Value must be
 * either a NullValue or the same type than the existing Variable.
 */
bool Variables::appendValue(const VariableName_t aName, const Value& aValue)
{
  if (isVariableExist(aName)
      && (m_Data[aName].second == openfluid::core::Value::NONE
          || aValue.getType() == openfluid::core::Value::NULLL
          || m_Data[aName].second == aValue.getType()))
    return m_Data[aName].first.appendValue(aValue);

  return false;
}

// =====================================================================
// =====================================================================


bool Variables::getValue(const VariableName_t aName, const TimeStep_t aStep,
    Value* aValue) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  return (it != m_Data.end() && it->second.first.getValue(aStep, aValue));
}

// =====================================================================
// =====================================================================

Value* Variables::getValue(const VariableName_t aName, const TimeStep_t aStep) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  if (it != m_Data.end())
    return it->second.first.getValue(aStep);

  return (Value*) 0;
}

// =====================================================================
// =====================================================================


Value* Variables::getCurrentValue(const VariableName_t aName) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  if (it != m_Data.end())
    return it->second.first.getCurrentValue();

  return (Value*) 0;
}

// =====================================================================
// =====================================================================


bool Variables::getCurrentValue(const VariableName_t aName, Value* aValue) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  return (it != m_Data.end() && it->second.first.getCurrentValue(aValue));
}

// =====================================================================
// =====================================================================


bool Variables::isVariableExist(const VariableName_t aName) const
{
  return m_Data.find(aName) != m_Data.end();
}

// =====================================================================
// =====================================================================


bool Variables::isVariableExist(const VariableName_t aName,
    const TimeStep_t aStep) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  // the variable exist if the required step is strictly lesser than the variable storage next step
  return (it != m_Data.end() && aStep < it->second.first.getNextStep());
}

// =====================================================================
// =====================================================================


bool Variables::isVariableExist(const VariableName_t aName, const TimeStep_t aStep,
    Value::Type ValueType) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  // the variable exist if the required step is strictly lesser than the variable storage next step
  return (it != m_Data.end() && aStep < it->second.first.getNextStep() && it->second.first.getValue(aStep)->getType() == ValueType);
}

// =====================================================================
// =====================================================================


bool Variables::isTypedVariableExist(const VariableName_t aName, const Value::Type VarType) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  return (it != m_Data.end() && it->second.second == VarType);
}

// =====================================================================
// =====================================================================


bool Variables::isTypedVariableExist(const VariableName_t aName,
    const TimeStep_t aStep, Value::Type VarType) const
{
  VariablesMap_t::const_iterator it = m_Data.find(aName);

  // the variable exist if the required step is strictly lesser than the variable storage next step
  return (it != m_Data.end() && aStep < it->second.first.getNextStep() && it->second.second == VarType);
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


unsigned int Variables::getVariableValuesCount(const VariableName_t aName) const
{

  VariablesMap_t::const_iterator it = m_Data.find(aName);

  if (it == m_Data.end())
    return -1;

  return it->second.first.getNextStep();
}

// =====================================================================
// =====================================================================


bool Variables::isAllVariablesCount(unsigned int Count) const
{
  for (VariablesMap_t::const_iterator it = m_Data.begin(); it != m_Data.end(); ++it)
  {
    if (it->second.first.getNextStep() != Count)
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

}
}
