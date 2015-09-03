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
  @file ValuesBuffer.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
 */


#include <boost/circular_buffer.hpp>


#include <openfluid/core/ValuesBuffer.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>

#include <iostream>


namespace openfluid { namespace core {


class ValuesBuffer::PrivateImpl
{
  public:

    typedef  boost::circular_buffer<IndexedValue> DataContainer_t;

    DataContainer_t m_Data;

    DataContainer_t::iterator findAtIndex(const TimeIndex_t& anIndex)
    {
      if (m_Data.empty()) return m_Data.end();

      if (anIndex < m_Data.front().m_Index || anIndex > m_Data.back().m_Index)
        return m_Data.end();

      if (anIndex == m_Data.front().m_Index)
        return m_Data.begin();

      if (anIndex == m_Data.back().m_Index)
      {
        ValuesBuffer::PrivateImpl::DataContainer_t::iterator TmpIt = m_Data.end();
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

    DataContainer_t::const_iterator findAtIndex(const TimeIndex_t& anIndex) const
    {
      if (m_Data.empty()) return m_Data.end();

      if (anIndex < m_Data.front().m_Index || anIndex > m_Data.back().m_Index)
        return m_Data.end();

      if (anIndex == m_Data.front().m_Index)
        return m_Data.begin();

      if (anIndex == m_Data.back().m_Index)
      {
        ValuesBuffer::PrivateImpl::DataContainer_t::const_iterator TmpIt = m_Data.end();
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

        ++It;
      }

      return m_Data.end();

    }
};


// =====================================================================
// =====================================================================


ValuesBuffer::ValuesBuffer():
    m_PImpl(new PrivateImpl)
{
  m_PImpl->m_Data.set_capacity(BufferSize);
}


// =====================================================================
// =====================================================================


ValuesBuffer::~ValuesBuffer()
{
  delete m_PImpl;
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::getValue(const TimeIndex_t& anIndex, Value* aValue) const
{
  PrivateImpl::DataContainer_t::const_iterator It = m_PImpl->findAtIndex(anIndex);

  if (It != m_PImpl->m_Data.end() && aValue->getType() == (*It).m_Value.get()->getType())
  {
    *aValue = *((*It).m_Value);

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


Value* ValuesBuffer::value(const TimeIndex_t& anIndex) const
{
  PrivateImpl::DataContainer_t::const_iterator It = m_PImpl->findAtIndex(anIndex);

  if (It != m_PImpl->m_Data.end())
  {
    return (*It).m_Value.get();
  }

  return (Value*)0;
}


// =====================================================================
// =====================================================================


Value* ValuesBuffer::currentValue() const
{
  return m_PImpl->m_Data.back().m_Value.get();
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::getCurrentValue(Value* aValue) const
{
  if(aValue->getType() == m_PImpl->m_Data.back().m_Value->getType())
  {
    *aValue = *(m_PImpl->m_Data.back()).m_Value;

    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::getLatestIndexedValue(IndexedValue& IndValue) const
{
  if(!m_PImpl->m_Data.empty())
  {
    IndValue.m_Index = m_PImpl->m_Data.back().m_Index;
    IndValue.m_Value.reset(m_PImpl->m_Data.back().m_Value->clone());

    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::getLatestIndexedValues(const TimeIndex_t& anIndex, IndexedValueList& IndValueList) const
{
  IndValueList.clear();

  if(!m_PImpl->m_Data.empty())
  {
    PrivateImpl::DataContainer_t::const_reverse_iterator rIt = m_PImpl->m_Data.rbegin();
    PrivateImpl::DataContainer_t::const_reverse_iterator rIte = m_PImpl->m_Data.rend();

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

  if(!m_PImpl->m_Data.empty() && aBeginIndex <= anEndIndex)
  {
    PrivateImpl::DataContainer_t::const_reverse_iterator rIt = m_PImpl->m_Data.rbegin();
    PrivateImpl::DataContainer_t::const_reverse_iterator rIte = m_PImpl->m_Data.rend();

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
  if (!m_PImpl->m_Data.empty())
  {
    return m_PImpl->m_Data.back().m_Index;
  }
  return -1;
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::isValueExist(const TimeIndex_t& anIndex) const
{
  return (!m_PImpl->m_Data.empty() && m_PImpl->findAtIndex(anIndex) != m_PImpl->m_Data.end());
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::modifyValue(const TimeIndex_t& anIndex, const Value& aValue)
{
  PrivateImpl::DataContainer_t::iterator It = m_PImpl->findAtIndex(anIndex);

  if (It != m_PImpl->m_Data.end())
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
  if (m_PImpl->m_Data.empty()) return false;

  PrivateImpl::DataContainer_t::iterator It = m_PImpl->m_Data.end();
  --It;
  (*It).m_Value.reset(aValue.clone());

  return true;
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::appendValue(const TimeIndex_t& anIndex, const openfluid::core::Value& aValue)
{
  if (!m_PImpl->m_Data.empty() && anIndex <= m_PImpl->m_Data.back().m_Index) return false;

  m_PImpl->m_Data.push_back(IndexedValue(anIndex,aValue));

  return true;
}


// =====================================================================
// =====================================================================


unsigned int ValuesBuffer::getValuesCount() const
{
  return m_PImpl->m_Data.size();
}


// =====================================================================
// =====================================================================


void ValuesBuffer::displayStatus(std::ostream& OStream) const
{
  OStream << "-- ValuesBuffer status --" << std::endl;
  OStream << "   BufferSize : " << BufferSize << std::endl;
  OStream << "   Size : " << m_PImpl->m_Data.size() << std::endl;
  OStream << "------------------------------" << std::endl;
}

// =====================================================================
// =====================================================================


void ValuesBuffer::displayContent(std::ostream& OStream) const
{
  OStream << "-- ValuesBuffer content --" << std::endl;

  PrivateImpl::DataContainer_t::const_iterator Itb = m_PImpl->m_Data.begin();
  PrivateImpl::DataContainer_t::const_iterator Ite = m_PImpl->m_Data.end();
  PrivateImpl::DataContainer_t::const_iterator It = Itb;

  while (It!=Ite)
  {
    OStream << "[" << (*It).m_Index << "|" << (*It).m_Value.get()->toString() << "]" << std::endl;
    ++It;
  }

}

// =====================================================================
// =====================================================================


} } // namespaces
