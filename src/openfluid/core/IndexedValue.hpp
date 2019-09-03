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
  @file IndexedValue.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CORE_INDEXEDVALUE_HPP__
#define __OPENFLUID_CORE_INDEXEDVALUE_HPP__


#include <list>
#include <memory>

#include <openfluid/core/Value.hpp>
#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/DateTime.hpp>


namespace openfluid { namespace core {

/**
  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "Variables"],
    "title" : "Time indexed value",
    "text" : "openfluid::core::IndexedValue %%SEL_START%%IndexedVal%%SEL_END%%"
  }
  @endcond
*/
class IndexedValue
{
  friend class ValuesBuffer;

  private:

    TimeIndex_t m_Index;

    std::shared_ptr<Value> m_Value;


  public:

    /**
      Default constructor
    */
    IndexedValue() : m_Index(0),m_Value(new NullValue())
    {

    }

    /**
      Constructor from a time index and a value
    */
    IndexedValue(const TimeIndex_t& Ind, const Value& Val) : m_Index(Ind),m_Value(Val.clone())
    {

    }

    /**
      Copy constructor
    */
    IndexedValue(const IndexedValue& IndValue) : m_Index(IndValue.m_Index),m_Value(IndValue.m_Value.get()->clone())
    { }

    /**
      Returns the time index of the indexed value
      @return the time index
    */
    inline TimeIndex_t getIndex() const
    {
      return m_Index;
    }

    /**
      Returns a pointer to the value of the indexed value
      @return a pointer to the value
    */
    inline Value* value() const
    {
      return m_Value.get();
    }

    /**
      Returns a pointer to the value of the indexed value
      @return a pointer to the value
    */
    inline Value* value()
    {
      return m_Value.get();
    }

    /**
      @deprecated Since version 2.1.0. Use openfluid::core::IndexedValue::value() const instead
    */
    [[deprecated]] inline Value* getValue() const
    {
      return m_Value.get();
    }

    /**
      @deprecated Since version 2.1.0. Use openfluid::core::IndexedValue::value() instead
    */
    [[deprecated]] inline Value* getValue()
    {
      return m_Value.get();
    }


    /**
      Clears the content of the indexed value. The time index is set to 0,
      and the value is set to an openfluid::core::NullValue.
    */
    inline void clear()
    {
      m_Index = 0;
      m_Value.reset(new NullValue());
    }

};


/**
  Indexed value list, ordered from oldest (front) to more recent (back)

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "Variables"],
    "title" : "List of time indexed value",
    "text" : "openfluid::core::IndexedValueList %%SEL_START%%IndexedValList%%SEL_END%%"
  }
  @endcond
*/
typedef std::list<IndexedValue> IndexedValueList;


} } // namespaces


#endif /* __OPENFLUID_CORE_INDEXEDVALUE_HPP__ */
