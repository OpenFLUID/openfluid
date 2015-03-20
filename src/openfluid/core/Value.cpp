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
  @file Value.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/core/Value.hpp>

#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/StringValue.hpp>

#include <openfluid/core/VectorValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/MapValue.hpp>
#include <openfluid/core/TreeValue.hpp>


namespace openfluid { namespace core {


const DoubleValue& Value::asDoubleValue() const
{
  if (!isDoubleValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a DoubleValue");

  return static_cast<const DoubleValue&>(*this);
}


// =====================================================================
// =====================================================================


DoubleValue& Value::asDoubleValue()
{
  if (!isDoubleValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a DoubleValue");

  return static_cast<DoubleValue&>(*this);
}


// =====================================================================
// =====================================================================


const IntegerValue& Value::asIntegerValue() const
{
  if (!isIntegerValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not an IntegerValue");

  return static_cast<const IntegerValue&>(*this);
}


// =====================================================================
// =====================================================================


IntegerValue& Value::asIntegerValue()
{
  if (!isIntegerValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not an IntegerValue");

  return static_cast<IntegerValue&>(*this);
}


// =====================================================================
// =====================================================================


const NullValue& Value::asNullValue() const
{
  if (!isNullValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a NullValue");

  return static_cast<const NullValue&>(*this);
}


// =====================================================================
// =====================================================================

NullValue& Value::asNullValue()
{
  if (!isNullValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a NullValue");

  return static_cast<NullValue&>(*this);
}


// =====================================================================
// =====================================================================


const BooleanValue& Value::asBooleanValue() const
{
  if (!isBooleanValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a BooleanValue");

  return static_cast<const BooleanValue&>(*this);
}


// =====================================================================
// =====================================================================

BooleanValue& Value::asBooleanValue()
{
  if (!isBooleanValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a BooleanValue");

  return static_cast<BooleanValue&>(*this);
}


// =====================================================================
// =====================================================================


const StringValue& Value::asStringValue() const
{
  if (!isStringValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a StringValue");

  return static_cast<const StringValue&>(*this);
}


// =====================================================================
// =====================================================================

StringValue& Value::asStringValue()
{
  if (!isStringValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a StringValue");

  return static_cast<StringValue&>(*this);
}


// =====================================================================
// =====================================================================


const VectorValue& Value::asVectorValue() const
{
  if (!isVectorValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a VectorValue");

  return static_cast<const VectorValue&>(*this);
}


// =====================================================================
// =====================================================================

VectorValue& Value::asVectorValue()
{
  if (!isVectorValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a VectorValue");

  return static_cast<VectorValue&>(*this);
}


// =====================================================================
// =====================================================================


const MatrixValue& Value::asMatrixValue() const
{
  if (!isMatrixValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a MatrixValue");

  return static_cast<const MatrixValue&>(*this);
}


// =====================================================================
// =====================================================================


MatrixValue& Value::asMatrixValue()
{
  if (!isMatrixValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a MatrixValue");

  return static_cast<MatrixValue&>(*this);
}


// =====================================================================
// =====================================================================


const MapValue& Value::asMapValue() const
{
  if (!isMapValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a MapValue");

  return static_cast<const MapValue&>(*this);

}


// =====================================================================
// =====================================================================


MapValue& Value::asMapValue()
{
  if (!isMapValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a MapValue");

  return static_cast<MapValue&>(*this);
}


// =====================================================================
// =====================================================================


const TreeValue& Value::asTreeValue() const
{
  if (!isTreeValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a TreeValue");

  return static_cast<const TreeValue&>(*this);
}


// =====================================================================
// =====================================================================


TreeValue& Value::asTreeValue()
{
  if (!isTreeValue())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not a TreeValue");

  return static_cast<TreeValue&>(*this);
}


// =====================================================================
// =====================================================================


std::string Value::toString() const
{
  std::ostringstream OSS;
  writeToStream(OSS);
  return OSS.str();
}


// =====================================================================
// =====================================================================

bool Value::getValueTypeFromString(const std::string& TypeName, Value::Type& ValueType)
{
  if (TypeName == "double")
  {
    ValueType = openfluid::core::Value::DOUBLE;
    return true;
  }
  else if (TypeName == "integer")
  {
    ValueType = openfluid::core::Value::INTEGER;
    return true;
  }
  else if (TypeName == "boolean")
  {
    ValueType = openfluid::core::Value::BOOLEAN;
    return true;
  }
  else if(TypeName == "vector")
  {
    ValueType = openfluid::core::Value::VECTOR;
    return true;
  }
  else if (TypeName == "matrix")
  {
    ValueType = openfluid::core::Value::MATRIX;
    return true;
  }
  else if (TypeName == "map")
  {
    ValueType = openfluid::core::Value::MAP;
    return true;
  }
  else if (TypeName == "tree")
  {
    ValueType = openfluid::core::Value::TREE;
    return true;
  }
  else if (TypeName == "string")
  {
    ValueType = openfluid::core::Value::STRING;
    return true;
  }
  else if (TypeName == "null")
  {
    ValueType = openfluid::core::Value::NULLL;
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


std::string Value::getStringFromValueType(const Value::Type ValueType)
{
  switch (ValueType)
  {
    case openfluid::core::Value::NONE:
      return "(none)";
    case openfluid::core::Value::DOUBLE:
      return "double";
    case openfluid::core::Value::INTEGER:
      return "integer";
    case openfluid::core::Value::BOOLEAN:
      return "boolean";
    case openfluid::core::Value::VECTOR:
      return "vector";
    case openfluid::core::Value::MATRIX:
      return "matrix";
    case openfluid::core::Value::MAP:
      return "map";
    case openfluid::core::Value::TREE:
      return "tree";
    case openfluid::core::Value::STRING:
      return "string";
    case openfluid::core::Value::NULLL:
      return "null";
    default:
      return "";
  }
}


} }  // namespaces

