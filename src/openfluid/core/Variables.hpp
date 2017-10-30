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
  @file Variables.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/

#ifndef __OPENFLUID_CORE_VARIABLES_HPP__
#define __OPENFLUID_CORE_VARIABLES_HPP__

#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/core/ValuesBuffer.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {


class OPENFLUID_API Variables
{
  private:

    typedef std::map<VariableName_t, std::pair<ValuesBuffer,Value::Type> > VariablesMap_t;

    VariablesMap_t m_Data;


  public:

    Variables();

    ~Variables();

    bool createVariable(const VariableName_t& aName);

    bool createVariable(const VariableName_t& aName, const Value::Type& aType);

    bool modifyValue(const VariableName_t& aName, const TimeIndex_t& anIndex,
        const Value& aValue);

    bool modifyCurrentValue(const VariableName_t& aName, const Value& aValue);

    bool appendValue(const VariableName_t& aName, const TimeIndex_t& anIndex, const Value& aValue);

    bool getValue(const VariableName_t& aName, const TimeIndex_t& anIndex,Value* aValue) const;

    const Value* value(const VariableName_t& aName, const TimeIndex_t& anIndex) const;

    const Value* currentValue(const VariableName_t& aName) const;

    bool getCurrentValue(const VariableName_t& aName, Value* aValue) const;

    bool getLatestIndexedValue(const VariableName_t& aName, IndexedValue& IndValue) const;

    bool getLatestIndexedValues(const VariableName_t& aName, const TimeIndex_t& anIndex,
                                IndexedValueList& IndValueList) const;

    bool getIndexedValues(const VariableName_t& aName,
                          const TimeIndex_t& aBeginIndex, const TimeIndex_t& anEndIndex,
                          IndexedValueList& IndValueList) const;

    bool getCurrentValueIfIndex(const VariableName_t& aName, const TimeIndex_t& Index, Value* aValue) const;

    Value* currentValueIfIndex(const VariableName_t& aName, const TimeIndex_t& Index) const;

    bool isVariableExist(const VariableName_t& aName) const;

    bool isVariableExist(const VariableName_t& aName, const TimeIndex_t& anIndex) const;

    bool isVariableExist(const VariableName_t& aName, const TimeIndex_t& anIndex,
        Value::Type ValueType) const;

    bool isTypedVariableExist(const VariableName_t& aName, const Value::Type& VarType) const;

    bool isTypedVariableExist(const VariableName_t& aName, const TimeIndex_t& anIndex,
        const Value::Type& VarType) const;

    std::vector<VariableName_t> getVariablesNames() const;

    int getVariableValuesCount(const VariableName_t& aName) const;

    bool checkAllVariablesCount(unsigned int Count, VariableName_t& ErrorVarName) const;

    void clear();

    void displayContent(const VariableName_t& aName, std::ostream& OStream) const;

};


} } // namespaces


#endif /* __VARIABLES_H_ */
