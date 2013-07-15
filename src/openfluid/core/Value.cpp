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
  \file Value.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
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


namespace openfluid { namespace core {


const DoubleValue& Value::asDoubleValue() const
{
  if (!isDoubleValue())
    throw openfluid::base::FrameworkException("Value::asDoubleValue","Value is not a DoubleValue");

  return static_cast<const DoubleValue&>(*this);
}


// =====================================================================
// =====================================================================


DoubleValue& Value::asDoubleValue()
{
  if (!isDoubleValue())
    throw openfluid::base::FrameworkException("Value::asDoubleValue","Value is not a DoubleValue");

  return static_cast<DoubleValue&>(*this);
}


// =====================================================================
// =====================================================================


const IntegerValue& Value::asIntegerValue() const
{
  if (!isIntegerValue())
    throw openfluid::base::FrameworkException("Value::asIntegerValue","Value is not an IntegerValue");

  return static_cast<const IntegerValue&>(*this);
}


// =====================================================================
// =====================================================================


IntegerValue& Value::asIntegerValue()
{
  if (!isIntegerValue())
    throw openfluid::base::FrameworkException("Value::asIntegerValue","Value is not an IntegerValue");

  return static_cast<IntegerValue&>(*this);
}


// =====================================================================
// =====================================================================


const NullValue& Value::asNullValue() const
{
  if (!isNullValue())
    throw openfluid::base::FrameworkException("Value::asNullValue","Value is not a NullValue");

  return static_cast<const NullValue&>(*this);
}


// =====================================================================
// =====================================================================

NullValue& Value::asNullValue()
{
  if (!isNullValue())
    throw openfluid::base::FrameworkException("Value::asNullValue","Value is not a NullValue");

  return static_cast<NullValue&>(*this);
}


// =====================================================================
// =====================================================================


const BooleanValue& Value::asBooleanValue() const
{
  if (!isBooleanValue())
    throw openfluid::base::FrameworkException("Value::asBooleanValue","Value is not a BooleanValue");

  return static_cast<const BooleanValue&>(*this);
}


// =====================================================================
// =====================================================================

BooleanValue& Value::asBooleanValue()
{
  if (!isBooleanValue())
    throw openfluid::base::FrameworkException("Value::asBooleanValue","Value is not a BooleanValue");

  return static_cast<BooleanValue&>(*this);
}


// =====================================================================
// =====================================================================


const StringValue& Value::asStringValue() const
{
  if (!isStringValue())
    throw openfluid::base::FrameworkException("Value::asStringValue","Value is not a StringValue");

  return static_cast<const StringValue&>(*this);
}


// =====================================================================
// =====================================================================

StringValue& Value::asStringValue()
{
  if (!isStringValue())
    throw openfluid::base::FrameworkException("Value::asStringValue","Value is not a StringValue");

  return static_cast<StringValue&>(*this);
}


// =====================================================================
// =====================================================================


const VectorValue& Value::asVectorValue() const
{
  if (!isVectorValue())
    throw openfluid::base::FrameworkException("Value::asVectorValue","Value is not a VectorValue");

  return static_cast<const VectorValue&>(*this);
}


// =====================================================================
// =====================================================================

VectorValue& Value::asVectorValue()
{
  if (!isVectorValue())
    throw openfluid::base::FrameworkException("Value::asVectorValue","Value is not a VectorValue");

  return static_cast<VectorValue&>(*this);
}


// =====================================================================
// =====================================================================


const MatrixValue& Value::asMatrixValue() const
{
  if (!isMatrixValue())
    throw openfluid::base::FrameworkException("Value::asMatrixValue","Value is not a MatrixValue");

  return static_cast<const MatrixValue&>(*this);
}


// =====================================================================
// =====================================================================


MatrixValue& Value::asMatrixValue()
{
  if (!isMatrixValue())
    throw openfluid::base::FrameworkException("Value::asMatrixValue","Value is not a MatrixValue");

  return static_cast<MatrixValue&>(*this);
}


// =====================================================================
// =====================================================================


const MapValue& Value::asMapValue() const
{
  if (!isMapValue())
    throw openfluid::base::FrameworkException("Value::asMapValue","Value is not a MapValue");

  return static_cast<const MapValue&>(*this);

}


// =====================================================================
// =====================================================================


MapValue& Value::asMapValue()
{
  if (!isMapValue())
    throw openfluid::base::FrameworkException("Value::asMapValue","Value is not a MapValue");

  return static_cast<MapValue&>(*this);
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

bool Value::getValueTypeFromString(const std::string TypeName, Value::Type& ValueType)
{
  if(TypeName == "double") { ValueType = openfluid::core::Value::DOUBLE; return true; }
  if(TypeName == "integer") { ValueType = openfluid::core::Value::INTEGER; return true; }
  if(TypeName == "boolean") { ValueType = openfluid::core::Value::BOOLEAN; return true; }
  if(TypeName == "vector") { ValueType = openfluid::core::Value::VECTOR; return true; }
  if(TypeName == "matrix") { ValueType = openfluid::core::Value::MATRIX; return true; }
  if(TypeName == "map") { ValueType = openfluid::core::Value::MAP; return true; }
  if(TypeName == "string") { ValueType = openfluid::core::Value::STRING; return true; }

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
    case openfluid::core::Value::STRING:
      return "string";
    default:
      return "";
  }
}


} }  // namespaces

