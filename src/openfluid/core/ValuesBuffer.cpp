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
  m_Data.set_capacity(BufferSize);
}

// =====================================================================
// =====================================================================


ValuesBuffer::~ValuesBuffer()
{

}


// =====================================================================
// =====================================================================


ValuesBuffer::DataContainer_t::iterator ValuesBuffer::findAtIndex(const TimeIndex_t& anIndex)
{
  if (m_Data.empty()) return m_Data.end();

  if (anIndex < m_Data.front().m_Index || anIndex > m_Data.back().m_Index)
    return m_Data.end();

  if (anIndex == m_Data.front().m_Index)
    return m_Data.begin();

  if (anIndex == m_Data.back().m_Index)
  {
    ValuesBuffer::DataContainer_t::iterator TmpIt = m_Data.end();
    --TmpIt;
    return TmpIt;
  }


  DataContainer_t::iterator Itb = m_Data.begin();
  DataContainer_t::iterator Ite = m_Data.end();
  DataContainer_t::iterator It = Itb;

  while (It!=Ite)
  {
    if ((*It).m_Index == anIndex)
      return It;

    if ((*It).m_Index > anIndex)
      return m_Data.end();

    ++It;
  }

  return m_Data.end();

}


// =====================================================================
// =====================================================================


ValuesBuffer::DataContainer_t::const_iterator ValuesBuffer::findAtIndex(const TimeIndex_t& anIndex) const
{
  if (m_Data.empty()) return m_Data.end();

  if (anIndex < m_Data.front().m_Index || anIndex > m_Data.back().m_Index)
    return m_Data.end();

  if (anIndex == m_Data.front().m_Index)
    return m_Data.begin();

  if (anIndex == m_Data.back().m_Index)
  {
    ValuesBuffer::DataContainer_t::const_iterator TmpIt = m_Data.end();
    --TmpIt;
    return TmpIt;
  }

  DataContainer_t::const_iterator Itb = m_Data.begin();
  DataContainer_t::const_iterator Ite = m_Data.end();
  DataContainer_t::const_iterator It = Itb;

  while (It!=Ite)
  {
    if ((*It).m_Index == anIndex)
      return It;

    if ((*It).m_Index > anIndex)
      return m_Data.end();

    It++;
  }

  return m_Data.end();

}

// =====================================================================
// =====================================================================


bool ValuesBuffer::getValue(const TimeIndex_t& anIndex, Value* aValue) const
{
  DataContainer_t::const_iterator It = findAtIndex(anIndex);

  if (It != m_Data.end() && aValue->getType() == (*It).m_Value.get()->getType())
  {
    *aValue = *((*It).m_Value);

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


Value* ValuesBuffer::getValue(const TimeIndex_t& anIndex) const
{
  DataContainer_t::const_iterator It = findAtIndex(anIndex);

  if (It != m_Data.end())
  {
    return (*It).m_Value.get();
  }

  return (Value*)0;
}

// =====================================================================
// =====================================================================


Value* ValuesBuffer::getCurrentValue() const
{
  return m_Data.back().m_Value.get();
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::getCurrentValue(Value* aValue) const
{
  if(aValue->getType() == m_Data.back().m_Value->getType())
  {
    *aValue = *m_Data.back().m_Value;

    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::getLatestIndexedValue(IndexedValue& IndValue) const
{
  if(!m_Data.empty())
  {
    IndValue.m_Index = m_Data.back().m_Index;
    IndValue.m_Value.reset(m_Data.back().m_Value->clone());

    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::getLatestIndexedValues(const TimeIndex_t& anIndex, IndexedValueList& IndValueList) const
{
  IndValueList.clear();

  if(!m_Data.empty())
  {
    DataContainer_t::const_reverse_iterator rIt = m_Data.rbegin();
    DataContainer_t::const_reverse_iterator rIte = m_Data.rend();

    while (rIt != rIte && (*rIt).getIndex() >= anIndex)
    {
      IndValueList.push_front(*rIt);
      ++rIt;
    }

    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::getIndexedValues(const TimeIndex_t& aBeginIndex, const TimeIndex_t& anEndIndex,
                                    IndexedValueList& IndValueList) const
{
  IndValueList.clear();

  if(!m_Data.empty() && aBeginIndex <= anEndIndex)
  {
    DataContainer_t::const_reverse_iterator rIt = m_Data.rbegin();
    DataContainer_t::const_reverse_iterator rIte = m_Data.rend();

    while (rIt != rIte && (*rIt).getIndex() >= aBeginIndex)
    {
      if  ((*rIt).getIndex() <= anEndIndex) IndValueList.push_front(*rIt);
      ++rIt;
    }

    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================



TimeIndex_t ValuesBuffer::getCurrentIndex() const
{
  if (!m_Data.empty())
  {
    return m_Data.back().m_Index;
  }
  return -1;
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::modifyValue(const TimeIndex_t& anIndex, const Value& aValue)
{
  DataContainer_t::iterator It = findAtIndex(anIndex);

  if (It != m_Data.end())
  {
    (*It).m_Value.reset(aValue.clone());
    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::modifyCurrentValue(const Value& aValue)
{
  if (m_Data.empty()) return false;

  DataContainer_t::iterator It = m_Data.end();
  It--;
  (*It).m_Value.reset(aValue.clone());

  return true;
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::appendValue(const TimeIndex_t& anIndex, const openfluid::core::Value& aValue)
{
  if (!m_Data.empty() && anIndex <= m_Data.back().m_Index) return false;

  m_Data.push_back(IndexedValue(anIndex,aValue));

  return true;
}


// =====================================================================
// =====================================================================


void ValuesBuffer::displayStatus(std::ostream& OStream) const
{
  OStream << "-- ValuesBuffer status --" << std::endl;
  OStream << "   BufferSize : " << BufferSize << std::endl;
  OStream << "   Size : " << m_Data.size() << std::endl;
  OStream << "------------------------------" << std::endl;
}

// =====================================================================
// =====================================================================


void ValuesBuffer::displayContent(std::ostream& OStream) const
{
  OStream << "-- ValuesBuffer content --" << std::endl;

  DataContainer_t::const_iterator Itb = m_Data.begin();
  DataContainer_t::const_iterator Ite = m_Data.end();
  DataContainer_t::const_iterator It = Itb;

  while (It!=Ite)
  {
    OStream << "[" << (*It).m_Index << "|" << (*It).m_Value.get()->toString() << "]" << std::endl;
    It++;
  }

}

// =====================================================================
// =====================================================================


}
}
