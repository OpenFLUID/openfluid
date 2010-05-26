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


#ifndef __INPUTDATA_H__
#define __INPUTDATA_H__

#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {


template <class T>
class DLLEXPORT InputData
{
  private :

    typedef std::map<InputDataName_t,T> InputDataMap_t;
    InputDataMap_t m_Data;

  public :

    InputData();

    ~InputData();

    bool setValue(const InputDataName_t aName,const T aValue);

    bool getValue(const InputDataName_t aName, T *aValue);

    bool getValueAsDouble(const InputDataName_t aName, double *aValue);

    bool getValueAsLong(const InputDataName_t aName, long *aValue);

    bool isDataExist(const InputDataName_t aName) const;

    std::vector<InputDataName_t> getInputDataNames() const;


};


// =====================================================================
// =====================================================================


template <class T>
InputData<T>::InputData()
{

}


// =====================================================================
// =====================================================================


template <class T>
InputData<T>::~InputData()
{

}


// =====================================================================
// =====================================================================

template <class T>
bool InputData<T>::setValue(const InputDataName_t aName,const T aValue)
{

  if (isDataExist(aName)) return false;
  else
  {
    m_Data[aName] = aValue;
  }
  return true;
}


// =====================================================================
// =====================================================================


template <class T>
bool InputData<T>::getValue(const InputDataName_t aName, T *aValue)
{
  typename InputDataMap_t::iterator it = m_Data.find(aName);

  if (it != m_Data.end())
  {
    *aValue = it->second;
    return true;
  }
  else return false;
}

// =====================================================================
// =====================================================================


template <class T>
bool InputData<T>::getValueAsDouble(const InputDataName_t aName, double *aValue)
{
  T TValue;
  typename InputDataMap_t::iterator it = m_Data.find(aName);

  if (it != m_Data.end())
  {
    TValue = it->second;

    std::istringstream iss(TValue);
    return !(iss >> (*aValue)).fail();
  }
  else return false;
}


// =====================================================================
// =====================================================================


template <class T>
bool InputData<T>::getValueAsLong(const InputDataName_t aName, long *aValue)
{
  T TValue;
  typename InputDataMap_t::iterator it = m_Data.find(aName);

  if (it != m_Data.end())
  {
    TValue = it->second;

    std::istringstream iss(TValue);
    return !(iss >> (*aValue)).fail();
  }
  else return false;
}


// =====================================================================
// =====================================================================


template <class T>
bool InputData<T>::isDataExist(const InputDataName_t aName) const
{
  return m_Data.find(aName) != m_Data.end();
}

// =====================================================================
// =====================================================================

template <class T>
std::vector<InputDataName_t> InputData<T>::getInputDataNames() const
{
  std::vector<InputDataName_t> TheNames;

  typename InputDataMap_t::const_iterator it;

  for (it = m_Data.begin() ; it != m_Data.end() ; ++it)
  {
    TheNames.push_back(it->first);
  }

  return TheNames;

}

} } // namespaces



#endif /* __INPUTDATA_H__ */
