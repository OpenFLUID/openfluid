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
  @file IntegerValue.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CORE_INTEGERVALUE_HPP__
#define __OPENFLUID_CORE_INTEGERVALUE_HPP__


#include <openfluid/core/SimpleValue.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

/**
  IntegerValue is a container for a signed long integer value.\n

  @see Value

  <I>Example : declaration</I>
  @snippet misc/values.cpp integer_decl

  <I>Example : getting the contained value</I>
  @snippet misc/values.cpp integer_get

  <I>Example : setting the contained value</I>
  @snippet misc/values.cpp integer_set

  <I>Example : conversion from string</I>
  @snippet misc/values.cpp integer_fromstr

  <I>Example : conversion to string</I>
  @snippet misc/values.cpp integer_tostr

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Types", "Values"],
    "title" : "IntegerValue",
    "text" : "openfluid::core::IntegerValue %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
class OPENFLUID_API IntegerValue : public SimpleValue
{
  private:

    long m_Value;

  public:

    /**
      Default constructor
    */
    IntegerValue() : SimpleValue(), m_Value(0)
    { }

    /**
      Copy constructor
    */
    IntegerValue(const IntegerValue& Val) : SimpleValue(), m_Value(Val.m_Value)
    { }

    /**
      Copy constructor
    */
    IntegerValue(IntegerValue&& Val) : SimpleValue(), m_Value(std::move(Val.m_Value))
    { }

    /**
      Constructor from plain old type
    */
    IntegerValue(const long& POD) : SimpleValue(), m_Value(POD)
    { }

    IntegerValue& operator=(const Value& Other) override;

    IntegerValue& operator=(Value&& Other) override;

    IntegerValue& operator=(const IntegerValue& Other) = default;

    IntegerValue& operator=(IntegerValue&& Other) = default;

    virtual ~IntegerValue() = default;

    /**
    * Cast operator
    */
    operator long() const
    {
      return m_Value;
    }

    inline Type getType() const override
    {
      return Value::INTEGER;
    }

    Value* clone() const override
    {
      return new IntegerValue(*this);
    };

    bool convert(Value& Val) const override;

    /**
      Returns the integer value as a plain old type
      @return the integer value
    */
    inline long get() const
    {
      return m_Value;
    }

    /**
      Sets the plain old type long integer value
      @param[in] Val the long integer value
    */
    inline void set(const long& Val)
    {
      m_Value = Val;
    }

    void writeToStream(std::ostream& OutStm) const override;

    void writeQuotedToStream(std::ostream& OutStm) const override
    {
      writeToStream(OutStm);
    }

};


} }  // namespaces


#endif /* __OPENFLUID_CORE_INTEGERVALUE_HPP__ */
