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


#ifndef __SSERIEVALUES_H__
#define __SSERIEVALUES_H__


#include <vector>
#include <iostream>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

class DLLEXPORT StepsReservation
{
  protected :
    static unsigned int Reserved;

  public :
    static unsigned int getReservation() { return Reserved; };

    static void setReservation(unsigned int aReservation) { Reserved = aReservation; };


};


template <class T>
class DLLEXPORT StepSerieOfValues : public StepsReservation
{

  private:

    unsigned int m_BaseStepIndex;
    std::vector<T> m_Data;

    unsigned int m_NextStep;

  public:


    StepSerieOfValues();

    ~StepSerieOfValues();

    bool deleteValues(const unsigned int UntilStepNbr);

    bool getValue(const unsigned int StepNbr,T* Value) const;

    bool modifyValue(const unsigned int StepNbr, T Value);

    bool appendValue(const T Value);

    unsigned int getNextStep() const;

    void displayStatus();

};


// =====================================================================
// =====================================================================

template <class T>
StepSerieOfValues<T>::StepSerieOfValues()
{

  m_BaseStepIndex = 0;
  m_NextStep = 0;

  m_Data.clear();
  m_Data.reserve(Reserved);
  m_Data.clear();

}

// =====================================================================
// =====================================================================

template <class T>
StepSerieOfValues<T>::~StepSerieOfValues()
{

}

// =====================================================================
// =====================================================================


template <class T>
bool StepSerieOfValues<T>::deleteValues(const unsigned int UntilStepNbr)
{
  // compute the real index of UntilStepNbr in m_Data
  unsigned int RealEnd = UntilStepNbr - m_BaseStepIndex +1;

  if (RealEnd < 0) return false;

  m_Data.erase(m_Data.begin(),m_Data.begin()+RealEnd);
  m_Data.reserve(Reserved);

  // compute the new BaseIndex
  m_BaseStepIndex = UntilStepNbr+1;

  return true;
}


// =====================================================================
// =====================================================================

template <class T>
bool StepSerieOfValues<T>::getValue(const unsigned int StepNbr,T* Value) const
{

  if (StepNbr < m_BaseStepIndex || StepNbr-m_BaseStepIndex >= m_Data.size()) return false;

  *Value = m_Data[StepNbr-m_BaseStepIndex];

  return true;
}


// =====================================================================
// =====================================================================

template <class T>
bool StepSerieOfValues<T>::modifyValue(const unsigned int StepNbr, T Value)
{
  if (StepNbr < m_BaseStepIndex) return false;

  m_Data[StepNbr-m_BaseStepIndex] = Value;

  return true;
}


// =====================================================================
// =====================================================================

template <class T>
bool StepSerieOfValues<T>::appendValue(const T Value)
{

  m_Data.push_back(Value);
  m_NextStep++;

  return true;
}


// =====================================================================
// =====================================================================


template <class T>
unsigned int StepSerieOfValues<T>::getNextStep() const
{
  return m_NextStep;
}


// =====================================================================
// =====================================================================


template <class T>
void StepSerieOfValues<T>::displayStatus()
{
  std::cout << "-- StepSerieOfValues status --"<< std::endl;
  std::cout << "   Reserved : " << Reserved << std::endl;
  std::cout << "   Size : " << m_Data.size() << std::endl;
  std::cout << "   Element size : " << sizeof(T) << std::endl;
  std::cout << "   BaseStepIndex : " << m_BaseStepIndex << std::endl;
  std::cout << "   Current storage step : " << m_NextStep-1 << std::endl;
  std::cout << "------------------------------"<< std::endl;
}


// =====================================================================
// =====================================================================


} } // namespaces



#endif /* __SSERIEVALUES_H__ */
