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

#ifndef __ATTRIBUTES_H__
#define __ATTRIBUTES_H__

#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/core/Value.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/core/IntegerValue.hpp>


namespace openfluid {
namespace core {

class OPENFLUID_API Attributes
{
  private:

    typedef std::map<AttributeName_t, StringValue> AttributesMap_t;
    AttributesMap_t m_Data;

  public:

    Attributes();

    ~Attributes();

    bool setValue(const AttributeName_t aName, const Value& aValue);

    bool setValue(const AttributeName_t aName, const std::string& aValue);

    bool getValue(const AttributeName_t aName, openfluid::core::StringValue& aValue) const;

    bool getValue(const AttributeName_t aName, std::string& aValue) const;

    bool getValueAsDouble(const AttributeName_t aName, double& aValue) const;

    bool getValueAsLong(const AttributeName_t aName, long& aValue) const;

    bool isAttributeExist(const AttributeName_t aName) const;

    std::vector<AttributeName_t> getAttributesNames() const;

    bool replaceValue(const AttributeName_t aName, const StringValue& aValue);

    bool replaceValue(const AttributeName_t aName, const std::string& aValue);

    bool removeAttribute(const AttributeName_t aName);

    void clear();

};


} } // namespaces


#endif /* __ATTRIBUTES_H__ */
