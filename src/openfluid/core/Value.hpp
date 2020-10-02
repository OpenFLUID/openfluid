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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_CORE_VALUE_HPP__
#define __OPENFLUID_CORE_VALUE_HPP__


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
class TreeValue;


class OPENFLUID_API Value
{
  public:

    enum Type { NONE, BOOLEAN, INTEGER, DOUBLE, STRING, VECTOR, MATRIX, MAP, TREE, NULLL };

    Value() = default; 

    Value(const Value&) = default; 

    Value(Value&&) = default; 

    /**
      Assignment operator
    */
    virtual Value& operator=(const Value&)
    {
      return *this;
    }

    /**
      Move operator
    */
    virtual Value& operator=(Value&&)
    {
      return *this;
    }

    virtual ~Value() = default; 

    virtual Type getType() const = 0;

    virtual Value* clone() const
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Value is not cloneable");
    }

    virtual bool convert(Value&) const
    {
      return false;
    }

    virtual bool isSimple() const = 0;

    virtual bool isCompound() const = 0;

    virtual void writeToStream(std::ostream& OutStm) const = 0;

    virtual void writeQuotedToStream(std::ostream& OutStm) const = 0;

    friend std::ostream& operator<<(std::ostream& OutStm, const Value& Val)
    {
      Val.writeToStream(OutStm); 
      return OutStm;
    }

    /**
      Returns true if the Value is a DoubleValue
    */
    inline bool isDoubleValue() const
    {
      return getType() == Value::DOUBLE;
    }

    /**
      Returns the value as a constant DoubleValue if the value is of the DoubleValue type
      @return the value as a constant DoubleValue
      @throw openfluid::base::FrameworkException if the value is not of the DoubleValue type
    */
    const DoubleValue& asDoubleValue() const;

    /**
      Returns the value as a DoubleValue if the value is of the DoubleValue type
      @return the value as a DoubleValue
      @throw openfluid::base::FrameworkException if the value is not of the DoubleValue type
    */
    DoubleValue& asDoubleValue();

    /**
      Returns true if the value is an IntegerValue
    */
    inline bool isIntegerValue() const
    {
      return getType() == Value::INTEGER;
    }

    /**
      Returns the value as a constant IntegerValue if the value is of the IntegerValue type
      @return the value as a constant IntegerValue
      @throw openfluid::base::FrameworkException if the value is not of the IntegerValue type
    */
    const IntegerValue& asIntegerValue() const;

    /**
      Returns the value as a IntegerValue if the value is of the IntegerValue type
      @return the value as a IntegerValue
      @throw openfluid::base::FrameworkException if the value is not of the IntegerValue type
    */
    IntegerValue& asIntegerValue();

    /**
      Returns true if the value is a BooleanValue
    */
    inline bool isBooleanValue() const
    {
      return getType() == Value::BOOLEAN;
    }

    /**
      Returns the value as a constant BooleanValue if the value is of the BooleanValue type
      @return the value as a constant BooleanValue
      @throw openfluid::base::FrameworkException if the value is not of the BooleanValue type
    */
    const BooleanValue& asBooleanValue() const;

    /**
      Returns the value as a BooleanValue if the value is of the BooleanValue type
      @return the value as a BooleanValue
      @throw openfluid::base::FrameworkException if the value is not of the BooleanValue type
    */
    BooleanValue& asBooleanValue();

    /**
      Returns true if the value is a StringValue
    */
    inline bool isStringValue() const
    {
      return getType() == Value::STRING;
    }

    /**
      Returns the value as a constant StringValue if the value is of the StringValue type
      @return the value as a constant StringValue
      @throw openfluid::base::FrameworkException if the value is not of the StringValue type
    */
    const StringValue& asStringValue() const;

    /**
      Returns the value as a StringValue if the value is of the StringValue type
      @return the value as a StringValue
      @throw openfluid::base::FrameworkException if the value is not of the StringValue type
    */
    StringValue& asStringValue();

    /**
      Returns true if the value is a NullValue
    */
    inline bool isNullValue() const
    {
      return getType() == Value::NULLL;
    }

    /**
      Returns the value as a constant NullValue if the value is of the NullValue type
      @return the value as a constant NullValue
      @throw openfluid::base::FrameworkException if the value is not of the NullValue type
    */
    const NullValue& asNullValue() const;

    /**
      Returns the value as a NullValue if the value is of the NullValue type
      @return the value as a NullValue
      @throw openfluid::base::FrameworkException if the value is not of the NullValue type
    */
    NullValue& asNullValue();

    /**
      Returns true if the value is a VectorValue
    */
    inline bool isVectorValue() const
    {
      return getType() == Value::VECTOR;
    }

    /**
      Returns the value as a constant VectorValue if the value is of the VectorValue type
      @return the value as a constant VectorValue
      @throw openfluid::base::FrameworkException if the value is not of the VectorValue type
    */
    const VectorValue& asVectorValue() const;

    /**
      Returns the value as a VectorValue if the value is of the VectorValue type
      @return the value as a VectorValue
      @throw openfluid::base::FrameworkException if the value is not of the VectorValue type
    */
    VectorValue& asVectorValue();

    /**
      Returns true if the value is a MatrixValue
    */
    inline bool isMatrixValue() const
    {
      return getType() == Value::MATRIX;
    }

    /**
      Returns the value as a constant MatrixValue if the value is of the MatrixValue type
      @return the value as a constant MatrixValue
      @throw openfluid::base::FrameworkException if the value is not of the MatrixValue type
    */
    const MatrixValue& asMatrixValue() const;

    /**
      Returns the value as a MatrixValue if the value is of the MatrixValue type
      @return the value as a MatrixValue
      @throw openfluid::base::FrameworkException if the value is not of the MatrixValue type
    */
    MatrixValue& asMatrixValue();

    /**
      Returns true if the value is a MapValue
    */
    inline bool isMapValue() const
    {
      return getType() == Value::MAP;
    }

    /**
      Returns the value as a constant MapValue if the value is of the MapValue type
      @return the value as a constant MapValue
      @throw openfluid::base::FrameworkException if the value is not of the MapValue type
    */
    const MapValue& asMapValue() const;

    /**
      Returns the value as a MapValue if the value is of the MapValue type
      @return the value as a MapValue
      @throw openfluid::base::FrameworkException if the value is not of the MapValue type
    */
    MapValue& asMapValue();

    /**
      Returns true if the value is a TreeValue
    */
    inline bool isTreeValue() const
    {
      return getType() == Value::TREE;
    }

    /**
      Returns the value as a constant TreeValue if the value is of the TreeValue type
      @return the value as a constant TreeValue
      @throw openfluid::base::FrameworkException if the value is not of the TreeValue type
    */
    const TreeValue& asTreeValue() const;

    /**
      Returns the value as a TreeValue if the value is of the TreeValue type
      @return the value as a TreeValue
      @throw openfluid::base::FrameworkException if the value is not of the TreeValue type
    */
    TreeValue& asTreeValue();

    /**
      Returns the value as a string
    */
    std::string toString() const;

    /**
      Gets the value type ciorresponding to the name of the type
      @param[in] ValueTypeString The type name as a string
      @param[out] ValueType The value type
      @return false if the type name does not exist or is misspelled, true otherwise
    */
    static bool getValueTypeFromString(const std::string& ValueTypeString, Value::Type& ValueType);

    /**
      Returns the name of the type corresponding to the given type
      @param[in] ValueType The value type
      @return the type name
    */
    static std::string getStringFromValueType(const Value::Type ValueType);

};


} }  // namespaces


#endif /* __OPENFLUID_CORE_VALUE_HPP__ */
