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


#ifndef __VARIABLES_HPP__
#define __VARIABLES_HPP__


#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/core/ValuesBuffer.hpp>
#include <openfluid/dllexport.hpp>

namespace openfluid { namespace core {


template <class T>
class DLLEXPORT Variables
{
  private :

    typedef std::map<VariableName_t,ValuesBuffer<T> > VariablesMap_t;
    VariablesMap_t m_Data;


  public :

    Variables();

    ~Variables();

    bool createVariable(const VariableName_t aName);

    bool modifyValue(const VariableName_t aName, const TimeStep_t aStep, const T aValue);

    bool appendValue(const VariableName_t aName, const T aValue);

    bool getValue(const VariableName_t aName, const TimeStep_t aStep, T *aValue) const;

    bool getCurrentValue(const VariableName_t aName, T *aValue) const;

    bool isVariableExist(const VariableName_t aName) const;

    bool isVariableExist(const VariableName_t aName, const TimeStep_t aStep) const;

    std::vector<VariableName_t> getVariablesNames() const;

    unsigned int getVariableValuesCount(const VariableName_t aName) const;

    bool isAllVariablesCount(unsigned int Count) const;

    void clear();

};


// =====================================================================
// =====================================================================


template <class T>
Variables<T>::Variables()
{

}


// =====================================================================
// =====================================================================


template <class T>
Variables<T>::~Variables()
{

}

// =====================================================================
// =====================================================================


template <class T>
bool Variables<T>::createVariable(const VariableName_t aName)
{
  if (!isVariableExist(aName))
  {
    m_Data[aName];
    return true;
  }
  return false;
}

// =====================================================================
// =====================================================================

template <class T>
bool Variables<T>::modifyValue(const VariableName_t aName, const TimeStep_t aStep, const T aValue)
{
  if (isVariableExist(aName,aStep))
  {
    return m_Data[aName].modifyValue(aStep,aValue);
  }
  return false;
}


// =====================================================================
// =====================================================================


template <class T>
bool Variables<T>::appendValue(const VariableName_t aName, const T aValue)
{
  if (isVariableExist(aName))
  {
     return m_Data[aName].appendValue(aValue);
  }
  return false;
}

// =====================================================================
// =====================================================================


template <class T>
bool Variables<T>::getValue(const VariableName_t aName, const TimeStep_t aStep, T* aValue) const
{

  typename VariablesMap_t::const_iterator it = m_Data.find(aName);

  if (it != m_Data.end())
  {
    return it->second.getValue(aStep,aValue);
  }

  return false;
}


// =====================================================================
// =====================================================================


template <class T>
bool Variables<T>::getCurrentValue(const VariableName_t aName, T* aValue) const
{

  typename VariablesMap_t::const_iterator it = m_Data.find(aName);

  if (it != m_Data.end())
  {
    return it->second.getCurrentValue(aValue);
  }

  return false;
}

// =====================================================================
// =====================================================================


template <class T>
bool Variables<T>::isVariableExist(const VariableName_t aName) const
{

  return m_Data.find(aName) != m_Data.end();
}

// =====================================================================
// =====================================================================

template <class T>
bool Variables<T>::isVariableExist(const VariableName_t aName, const TimeStep_t aStep) const
{
  typename VariablesMap_t::const_iterator it;

  it = m_Data.find(aName);

  if (it != m_Data.end())
  {
    // the variable exist if the required step is strictly lesser than the variable storage next step
    return (aStep < it->second.getNextStep());
  }
  return false;

}

// =====================================================================
// =====================================================================

template <class T>
std::vector<VariableName_t> Variables<T>::getVariablesNames() const
{
  std::vector<VariableName_t> TheNames;

  typename VariablesMap_t::const_iterator it;

  for (it = m_Data.begin() ; it != m_Data.end() ; ++it)
  {
    TheNames.push_back(it->first);
  }

  return TheNames;

}


// =====================================================================
// =====================================================================


template <class T>
unsigned int Variables<T>::getVariableValuesCount(const VariableName_t aName) const
{

  typename VariablesMap_t::const_iterator it;

  it = m_Data.find(aName);

  if (it == m_Data.end()) return (-1);

  else return it->second.getNextStep();
}


// =====================================================================
// =====================================================================


template <class T>
bool Variables<T>::isAllVariablesCount(unsigned int Count) const
{
  typename VariablesMap_t::const_iterator it;

  for (it = m_Data.begin();it != m_Data.end();++it)
  {
    if (it->second.getNextStep() != Count) return false;
  }
  return true;
}


// =====================================================================
// =====================================================================


template <class T>
void Variables<T>::clear()
{
    m_Data.clear();
}


} } // namespaces




#endif /* __VARIABLES_H_ */
