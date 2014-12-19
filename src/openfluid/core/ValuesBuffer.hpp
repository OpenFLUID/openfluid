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
 @file

 @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#ifndef __OPENFLUID_CORE_VALUESBUFFER_HPP__
#define __OPENFLUID_CORE_VALUESBUFFER_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/core/ValuesBufferProperties.hpp>
#include <openfluid/core/Value.hpp>
#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/DateTime.hpp>
#include <boost/circular_buffer.hpp>

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <list>

namespace openfluid { namespace core {


class IndexedValue
{
  friend class ValuesBuffer;

  private:

    TimeIndex_t m_Index;

    boost::shared_ptr<Value> m_Value;


  public:

    /**
      Default constructor
    */
    IndexedValue():
      m_Index(0),m_Value(boost::shared_ptr<Value>(new NullValue())) {};

    /**
      Constructor from a time index and a value
    */
    IndexedValue(const TimeIndex_t& Ind, const Value& Val):
      m_Index(Ind),m_Value(boost::shared_ptr<Value>(Val.clone())) {};

    /**
      Copy constructor
    */
    IndexedValue(const IndexedValue& IndValue):
          m_Index(IndValue.m_Index),m_Value(boost::shared_ptr<Value>(IndValue.m_Value.get()->clone())) {};

    /**
      Returns the time index of the indexed value
      @return the time index
    */
    inline TimeIndex_t getIndex() const { return m_Index; };

    /**
      Returns a pointer to the value of the indexed value
      @return a pointer to the value
    */
    inline Value* getValue() const { return m_Value.get(); };

    /**
      Returns a pointer to the value of the indexed value
      @return a pointer to the value
    */
    inline Value* getValue() { return m_Value.get(); };

    /**
      Clears the content of the indexed value. The time index is set to 0,
      and the value is set to an openfluid::core::NullValue.
    */
    inline void clear() { m_Index = 0; m_Value.reset(new NullValue()); };

};


/**
  Indexed value list, ordered from oldest (front) to more recent (back)
*/
typedef std::list<IndexedValue> IndexedValueList;


// =====================================================================
// =====================================================================


class OPENFLUID_API ValuesBuffer: public ValuesBufferProperties
{

  public:

    typedef  boost::circular_buffer<IndexedValue> DataContainer_t;


  private:

    DataContainer_t m_Data;

    DataContainer_t::iterator findAtIndex(const TimeIndex_t& anIndex);

    DataContainer_t::const_iterator findAtIndex(const TimeIndex_t& anIndex) const;

  public:

    ValuesBuffer();

    ~ValuesBuffer();

    bool getValue(const TimeIndex_t& anIndex, Value* aValue) const;

    Value* getValue(const TimeIndex_t& anIndex) const;

    Value* getCurrentValue() const;

    TimeIndex_t getCurrentIndex() const;

    bool isValueExist(const TimeIndex_t& anIndex) const
    {
      return (!m_Data.empty() && findAtIndex(anIndex) != m_Data.end());
    }


    bool getCurrentValue(Value* aValue) const;

    bool getLatestIndexedValue(IndexedValue& IndValue) const;

    bool getLatestIndexedValues(const TimeIndex_t& anIndex, IndexedValueList& IndValueList) const;

    bool getIndexedValues(const TimeIndex_t& aBeginIndex, const TimeIndex_t& anEndIndex,
                                IndexedValueList& IndValueList) const;

    bool modifyValue(const TimeIndex_t& anIndex, const Value& aValue);

    bool modifyCurrentValue(const Value& aValue);

    bool appendValue(const TimeIndex_t& anIndex, const Value& aValue);

    unsigned int getValuesCount() const
    {
      return m_Data.size();
    }

    void displayStatus(std::ostream& OStream) const;

    void displayContent(std::ostream& OStream) const;


};

}
} // namespaces


#endif /* __OPENFLUID_CORE_VALUESBUFFER_HPP__ */
