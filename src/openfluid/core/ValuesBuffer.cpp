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
#ifdef bcb
  m_Data.set_capacity(BufferSize);
#endif
}

// =====================================================================
// =====================================================================


ValuesBuffer::~ValuesBuffer()
{

}

// =====================================================================
// =====================================================================


void ValuesBuffer::runGarbageCollector()
{
#ifndef bcb
  while (m_Data.size() > BufferSize) m_Data.pop_front();
#endif
}


// =====================================================================
// =====================================================================

ValuesBuffer::DataContainer_t::iterator ValuesBuffer::findAtIndex(const TimeIndex_t& anIndex)
{
  if (m_Data.empty()) return m_Data.end();

  if (anIndex < m_Data.front().Index || anIndex > m_Data.back().Index)
    return m_Data.end();

  if (anIndex == m_Data.front().Index)
    return m_Data.begin();

  if (anIndex == m_Data.back().Index)
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
    if ((*It).Index == anIndex)
      return It;

    if ((*It).Index > anIndex)
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

  if (anIndex < m_Data.front().Index || anIndex > m_Data.back().Index)
    return m_Data.end();

  if (anIndex == m_Data.front().Index)
    return m_Data.begin();

  if (anIndex == m_Data.back().Index)
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
    if ((*It).Index == anIndex)
      return It;

    if ((*It).Index > anIndex)
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

  if (It != m_Data.end() && aValue->getType() == (*It).Data.get()->getType())
  {
    *aValue = *((*It).Data);

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
    return (*It).Data.get();
  }

  return (Value*)0;
}

// =====================================================================
// =====================================================================


Value* ValuesBuffer::getCurrentValue() const
{
  return m_Data.back().Data.get();
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::getCurrentValue(Value* aValue) const
{
  if(aValue->getType() == m_Data.back().Data->getType())
  {
    *aValue = *m_Data.back().Data;

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
    return m_Data.back().Index;
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
    (*It).Data.reset(aValue.clone());
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
  (*It).Data.reset(aValue.clone());

  return true;
}


// =====================================================================
// =====================================================================


bool ValuesBuffer::appendValue(const TimeIndex_t& anIndex, const openfluid::core::Value& aValue)
{
  if (!m_Data.empty() && anIndex <= m_Data.back().Index) return false;

  m_Data.push_back(IndexedValue(anIndex,aValue));
  runGarbageCollector();

  return true;
}


// =====================================================================
// =====================================================================


void ValuesBuffer::displayStatus(std::ostream& OStream) const
{
  OStream << "-- ValuesBuffer status --" << std::endl;
  OStream << "   BufferSize : " << BufferSize << std::endl;
  OStream << "   Size : " << m_Data.size() << std::endl;
  //  OStream << "   Element size : " << sizeof(T) << std::endl;
//  OStream << "   Current storage step : " << m_NextStep - 1 << std::endl;
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
    OStream << "[" << (*It).Index << "|" << (*It).Data.get()->toString() << "]" << std::endl;
    It++;
  }

}

// =====================================================================
// =====================================================================


}
}
