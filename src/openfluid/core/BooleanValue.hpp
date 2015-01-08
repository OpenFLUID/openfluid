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
  @file BooleanValue.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_CORE_BOOLEANVALUE_HPP__
#define __OPENFLUID_CORE_BOOLEANVALUE_HPP__

#include <openfluid/core/SimpleValue.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

/**
BooleanValue is a container for a true/false value.\n

\see Value

\n

<I>Example : declaration</I>
@code
  // declaration of a BooleanValue, initialized to false by default
  openfluid::core::BooleanValue Val1;

  // declaration of a BooleanValue, initialized to true
  openfluid::core::BooleanValue Val2(true);
@endcode


<I>Example : getting the contained value</I>
@code
  bool Tmp1;

  // using the get method
  Tmp1 = Val1.get();

  // or using the cast operator
  Tmp1 = Val1;
@endcode


<I>Example : setting the contained value</I>
@code
  // using the set method
  Val1.set(true);
@endcode


<I>Example : conversion from string</I>
@code
  openfluid::core::StringValue StringVal("true");

  // to BooleanValue
  Val1 = StringVal.toBooleanValue();

  // to bool
  bool BoolVal = StringVal.toBoolean();
@endcode


<I>Example : conversion to string</I>
@code
  std::string StdStrVal = Val1.toString();
@endcode
*/
class OPENFLUID_API BooleanValue : public SimpleValue
{
  private:

    bool m_Value;

  public:

    /**
      Default constructor
    */
    BooleanValue() : m_Value(false) {};

    /**
      Copy constructor
    */
    BooleanValue(const BooleanValue& Val) : SimpleValue(Val), m_Value(Val.m_Value) {};

    /**
      Constructor from plain old type
    */
    BooleanValue(const bool& POD) : SimpleValue(), m_Value(POD) {};

    Value& operator =(const Value& Other);

    /**
    * Cast operator
    */
    operator bool() const { return m_Value; };

    virtual ~BooleanValue() {};

    inline Type getType() const { return Value::BOOLEAN; };

    Value* clone() const { return new BooleanValue(*this); };

    /**
      Returns the boolean value as plain old type
      @return the boolean value
    */
    inline bool get() { return m_Value; };

    /**
      Sets the plain old type boolean value
      @param[in] Val the boolean value
    */
    inline void set(const bool& Val) { m_Value = Val; };

    void writeToStream(std::ostream& OutStm) const;

};


} }  // namespaces



#endif /* __OPENFLUID_CORE_BOOLEANVALUE_HPP__ */
