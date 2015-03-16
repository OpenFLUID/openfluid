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
  @file ValuesBuffer.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#ifndef __OPENFLUID_CORE_VALUESBUFFER_HPP__
#define __OPENFLUID_CORE_VALUESBUFFER_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/core/ValuesBufferProperties.hpp>
#include <openfluid/core/IndexedValue.hpp>


namespace openfluid { namespace core {



class OPENFLUID_API ValuesBuffer: public ValuesBufferProperties
{

  private:

    class PrivateImpl;
    PrivateImpl* m_PImpl;


  public:

    ValuesBuffer();

    ~ValuesBuffer();

    bool getValue(const TimeIndex_t& anIndex, Value* aValue) const;

    Value* value(const TimeIndex_t& anIndex) const;

    Value* currentValue() const;

    TimeIndex_t getCurrentIndex() const;

    bool isValueExist(const TimeIndex_t& anIndex) const;

    bool getCurrentValue(Value* aValue) const;

    bool getLatestIndexedValue(IndexedValue& IndValue) const;

    bool getLatestIndexedValues(const TimeIndex_t& anIndex, IndexedValueList& IndValueList) const;

    bool getIndexedValues(const TimeIndex_t& aBeginIndex, const TimeIndex_t& anEndIndex,
                                IndexedValueList& IndValueList) const;

    bool modifyValue(const TimeIndex_t& anIndex, const Value& aValue);

    bool modifyCurrentValue(const Value& aValue);

    bool appendValue(const TimeIndex_t& anIndex, const Value& aValue);

    unsigned int getValuesCount() const;

    void displayStatus(std::ostream& OStream) const;

    void displayContent(std::ostream& OStream) const;


};

}  } // namespaces


#endif /* __OPENFLUID_CORE_VALUESBUFFER_HPP__ */
