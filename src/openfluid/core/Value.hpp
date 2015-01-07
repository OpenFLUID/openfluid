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
  @file Value.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_CORE_VALUE_HPP__
#define __OPENFLUID_CORE_VALUE_HPP__

#include <iostream>

#include <openfluid/dllexport.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace core {

class NullValue;
class BooleanValue;
class DoubleValue;
class IntegerValue;
class StringValue;

class VectorValue;
class MatrixValue;
class MapValue;


class OPENFLUID_API Value
{
  public:

    enum Type { NONE, BOOLEAN, INTEGER, DOUBLE, STRING, VECTOR, MATRIX, MAP, NULLL };

    /**
      Default constructor
    */
    Value() {};

    /**
      Copy constructor
    */
    Value(const Value& /*Val*/) {};

    /**
     * Assignment operator
     */
    virtual Value& operator =(const Value& /*Other*/) { return *this; }

    virtual ~Value() {};

    virtual Type getType() const = 0;

    virtual Value* clone() const { throw openfluid::base::FrameworkException("Value::clone","Value is not cloneable"); };

    inline virtual bool isSimple() const = 0;

    inline virtual bool isCompound() const = 0;

    virtual void writeToStream(std::ostream& OutStm) const = 0;

    friend std::ostream& operator<<(std::ostream& OutStm, const Value& Val)
      { Val.writeToStream(OutStm); return OutStm; };

    /**
      Returns true if the Value is a DoubleValue
    */
    inline bool isDoubleValue() const { return getType() == Value::DOUBLE; };

    const DoubleValue& asDoubleValue() const;

    DoubleValue& asDoubleValue();

    /**
      Returns true if the Value is an IntegerValue
    */
    inline bool isIntegerValue() const { return getType() == Value::INTEGER; };

    const IntegerValue& asIntegerValue() const;

    IntegerValue& asIntegerValue();

    /**
      Returns true if the Value is a BooleanValue
    */
    inline bool isBooleanValue() const { return getType() == Value::BOOLEAN; };

    const BooleanValue& asBooleanValue() const;

    BooleanValue& asBooleanValue();

    /**
      Returns true if the Value is a StringValue
    */
    inline bool isStringValue() const { return getType() == Value::STRING; };

    const StringValue& asStringValue() const;

    StringValue& asStringValue();

    /**
      Returns true if the Value is a NullValue
    */
    inline bool isNullValue() const { return getType() == Value::NULLL; };

    const NullValue& asNullValue() const;

    NullValue& asNullValue();

    /**
      Returns true if the Value is a VectorValue
    */
    inline bool isVectorValue() const { return getType() == Value::VECTOR; };

    const VectorValue& asVectorValue() const;

    VectorValue& asVectorValue();

    /**
      Returns true if the Value is a MatrixValue
    */
    inline bool isMatrixValue() const { return getType() == Value::MATRIX; };

    const MatrixValue& asMatrixValue() const;

    MatrixValue& asMatrixValue();

    /**
      Returns true if the Value is a MapValue
    */
    inline bool isMapValue() const { return getType() == Value::MAP; };

    const MapValue& asMapValue() const;

    MapValue& asMapValue();

    /**
      Returns the contained value as a string
    */
    std::string toString() const;

    static bool getValueTypeFromString(const std::string ValueTypeString, Value::Type& ValueType);

    static std::string getStringFromValueType(const Value::Type ValueType);

};


} }  // namespaces


#endif /* __OPENFLUID_CORE_VALUE_HPP__ */
