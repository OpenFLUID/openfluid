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
 \file ValuesBuffer.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <openfluid/core/ValuesBuffer.hpp>
#include <openfluid/core/StringValue.hpp>

#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>

namespace openfluid {
namespace core {

// =====================================================================
// =====================================================================


ValuesBuffer::ValuesBuffer()
{

  m_NextStep = 0;

  m_Data.set_capacity(BufferSize);
  m_Data.clear();

}

// =====================================================================
// =====================================================================


ValuesBuffer::~ValuesBuffer()
{

}

// =====================================================================
// =====================================================================


bool ValuesBuffer::TranslateStepNbrToIndex(const unsigned int& StepNbr,
    unsigned int& Index) const
{
  if (m_NextStep < BufferSize)
  {
    if (StepNbr < m_NextStep)
    {
      Index = StepNbr;
      return true;
    }
  }
  else
  {
    int StepsDelta = m_NextStep - StepNbr - 1;

    if (StepsDelta < (int) BufferSize && StepsDelta >= 0)
    {
      Index = BufferSize - StepsDelta - 1;
      return true;
    }
  }

  return false;
}

// =====================================================================
// =====================================================================


bool ValuesBuffer::getValue(const unsigned int StepNbr, Value* aValue) const
{
  unsigned int Index;

  if (TranslateStepNbrToIndex(StepNbr, Index))
  {
    *aValue = *m_Data[Index];

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


Value* ValuesBuffer::getValue(const unsigned int StepNbr) const
{
  unsigned int Index;

  if (TranslateStepNbrToIndex(StepNbr, Index))
  {
    return m_Data[Index].get();
  }

  return (Value*)0;
}

// =====================================================================
// =====================================================================


Value* ValuesBuffer::getCurrentValue() const
{
  return m_Data.back().get();
}

// =====================================================================
// =====================================================================


bool ValuesBuffer::modifyValue(const unsigned int StepNbr, const Value& aValue)
{
  unsigned int Index;

  if (TranslateStepNbrToIndex(StepNbr, Index))
  {
    m_Data[Index].reset(aValue.clone());
    return true;
  }
  return false;
}

// =====================================================================
// =====================================================================


bool ValuesBuffer::appendValue(const openfluid::core::Value& aValue)
{
  m_Data.push_back(boost::shared_ptr<Value>(aValue.clone()));
  m_NextStep++;

  return true;
}

// =====================================================================
// =====================================================================


unsigned int ValuesBuffer::getNextStep() const
{
  return m_NextStep;
}

// =====================================================================
// =====================================================================


void ValuesBuffer::displayStatus(std::ostream& OStream)
{
  OStream << "-- ValuesBuffer status --" << std::endl;
  OStream << "   BufferSize : " << BufferSize << std::endl;
  OStream << "   Size : " << m_Data.size() << std::endl;
  //  OStream << "   Element size : " << sizeof(T) << std::endl;
  OStream << "   Current storage step : " << m_NextStep - 1 << std::endl;
  OStream << "------------------------------" << std::endl;
}

// =====================================================================
// =====================================================================

}
}
