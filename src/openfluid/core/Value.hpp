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
  \file Value.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __VALUE_HPP___
#define __VALUE_HPP___

#include <iostream>

namespace openfluid { namespace core {

class NullValue;
class BooleanValue;
class DoubleValue;
class IntegerValue;
class StringValue;

class VectorValue;
class MatrixValue;
class ArrayValue;
class MapValue;


class Value
{
  public:

    enum Type { BOOLEAN, INTEGER, DOUBLE, STRING, VECTOR, MATRIX, ARRAY, MAP, NULLL };

    /**
      Default constructor
    */
    Value() {};

    /**
      Copy constructor
    */
    Value(const Value& /*Val*/) {};

    virtual ~Value() {};

    virtual Type getType() const = 0;

    inline virtual bool isSimple() const = 0;

    inline virtual bool isCompound() const = 0;

    virtual void writeToStream(std::ostream& OutStm) const = 0;

    friend std::ostream& operator<<(std::ostream& OutStm, const Value& Val)
      { Val.writeToStream(OutStm); return OutStm; };


    inline bool isDoubleValue() const { return getType() == Value::DOUBLE; };

    const DoubleValue& asDoubleValue() const;

    DoubleValue& asDoubleValue();


    inline bool isIntegerValue() const { return getType() == Value::INTEGER; };

    const IntegerValue& asIntegerValue() const;

    IntegerValue& asIntegerValue();


    inline bool isBooleanValue() const { return getType() == Value::BOOLEAN; };

    const BooleanValue& asBooleanValue() const;

    BooleanValue& asBooleanValue();


    inline bool isStringValue() const { return getType() == Value::STRING; };

    const StringValue& asStringValue() const;

    StringValue& asStringValue();


    inline bool isNullValue() const { return getType() == Value::NULLL; };

    const NullValue& asNullValue() const;

    NullValue& asNullValue();


    inline bool isVectorValue() const { return getType() == Value::VECTOR; };

    const VectorValue& asVectorValue() const;

    VectorValue& asVectorValue();


    inline bool isMatrixValue() const { return getType() == Value::MATRIX; };

    const MatrixValue& asMatrixValue() const;

    MatrixValue& asMatrixValue();


    inline bool isArrayValue() const { return getType() == Value::ARRAY; };

    const VectorValue& asArrayValue() const;

    VectorValue& asArrayValue();


    inline bool isMapValue() const { return getType() == Value::MAP; };

    const VectorValue& asMapValue() const;

    VectorValue& asMapValue();


    std::string toString() const;

};


} }  // namespaces


#endif /* __VALUE_HPP___ */
