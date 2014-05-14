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
  \file IntegerValue.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __INTEGERVALUE_HPP___
#define __INTEGERVALUE_HPP___



#include <openfluid/core/SimpleValue.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

/**
IntegerValue is a container for a signed long integer value.\n

\see Value

\n

<I>Example : declaration</I>
@code
  // declaration of an IntegerValue, initialized to 0 by default
  openfluid::core::IntegerValue Val1;

  // declaration of an IntegerValue, initialized to 35
  openfluid::core::IntegerValue Val2(35);
@endcode


<I>Example : getting the contained value</I>
@code
  long Tmp1;

  // using the get method
  Tmp1 = Val1.get();

  // or using the cast operator
  Tmp1 = Val1;
@endcode


<I>Example : setting the contained value</I>
@code
  // using the set method
  Val1.set(-10199);
@endcode


<I>Example : conversion from string</I>
@code
  openfluid::core::StringValue StringVal("57");

  // to IntegerValue
  Val1 = StringVal.toIntegerValue();

  // to long
  long LongVal = StringVal.toInteger();
@endcode


<I>Example : conversion to string</I>
@code
  std::string StdStrVal = Val1.toString();
@endcode
*/
class DLLEXPORT IntegerValue : public SimpleValue
{
  private:

    long m_Value;

  public:

    /**
      Default constructor
    */
    IntegerValue() : m_Value(0) {};

    /**
      Copy constructor
    */
    IntegerValue(const IntegerValue& Val) : SimpleValue(Val), m_Value(Val.m_Value) {};

    /**
      Constructor from plain old type
    */
    IntegerValue(const long& POD) : SimpleValue(), m_Value(POD) {};

    Value& operator =(const Value& Other);

    /**
    * Cast operator
    */
    operator long() const { return m_Value; };

    virtual ~IntegerValue() {};

    inline Type getType() const { return Value::INTEGER; };

    Value* clone() const { return new IntegerValue(*this); };

    /**
      Returns the integer value as plain old type
      @return the integer value
    */
    inline long& get() { return m_Value; };

    /**
      Returns the integer value as a const plain old type
      @return the integer value
    */
    inline const long& get() const { return m_Value; };

    /**
      Sets the plain old type long integer value
      @param[in] Val the long integer value
    */
    inline void set(const long& Val) { m_Value = Val; };

    void writeToStream(std::ostream& OutStm) const;

};


} }  // namespaces


#endif /* __INTEGERVALUE_HPP___ */
