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
  @file DoubleValue.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_CORE_DOUBLEVALUE_HPP__
#define __OPENFLUID_CORE_DOUBLEVALUE_HPP__

#include <openfluid/core/SimpleValue.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {
/**
DoubleValue is a container for a signed double precision floating point value.\n

\see Value

\n

<I>Example : declaration</I>
@code
  // declaration of a DoubleValue, initialized to 0.0 by default
  openfluid::core::DoubleValue Val1;

  // declaration of a DoubleValue, initialized to 1.357
  openfluid::core::DoubleValue Val2(1.357);
@endcode


<I>Example : getting the contained value</I>
@code
  double Tmp1;

  // using the get method
  Tmp1 = Val1.get();

  // or using the cast operator
  Tmp1 = Val1;
@endcode


<I>Example : setting the contained value</I>
@code
  // using the set method
  Val1.set(101.99);
@endcode


<I>Example : conversion from string</I>
@code
  openfluid::core::StringValue StringVal("57.33");

  // to DoubleValue
  Val1 = StringVal.toDoubleValue();

  // to double
  double DblVal = StringVal.toDouble();
@endcode


<I>Example : conversion to string</I>
@code
  std::string StdStrVal = Val1.toString();
@endcode
*/
class OPENFLUID_API DoubleValue : public SimpleValue
{
  private:

    double m_Value;

  public:

    /**
      Default constructor
    */
    DoubleValue() : SimpleValue(), m_Value(0.0)
    { }

    /**
      Copy constructor
    */
    DoubleValue(const DoubleValue& Val) : SimpleValue(), m_Value(Val.m_Value)
    { }

    /**
      Constructor from plain old type
    */
    DoubleValue(const double& POD) : SimpleValue(), m_Value(POD)
    { }

    virtual ~DoubleValue()
    { }

    Value& operator =(const Value& Other);

    /**
      Cast operator
     */
    operator double() const
    { return m_Value; }

    inline Type getType() const
    { return Value::DOUBLE; }

    Value* clone() const
    { return new DoubleValue(*this); }

    bool convert(Value& Val) const;

    /**
      Returns the double value as plain old type
      @return the double value
    */
    inline double get() const
    { return m_Value; }

    /**
      Sets the plain old type double value
      @param[in] Val the double value
    */
    inline void set(const double& Val)
    { m_Value = Val; }

    void writeToStream(std::ostream& OutStm) const;

    void writeQuotedToStream(std::ostream& OutStm) const
    { writeToStream(OutStm); }

};


} }  // namespaces


#endif /* __OPENFLUID_CORE_DOUBLEVALUE_HPP__ */
