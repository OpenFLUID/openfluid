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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_CORE_BOOLEANVALUE_HPP__
#define __OPENFLUID_CORE_BOOLEANVALUE_HPP__


#include <openfluid/core/SimpleValue.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {


/**
  BooleanValue is a container for a true/false value.\n

  @see Value

  <I>Example : declaration</I>
  @snippet misc/values.cpp boolean_decl

  <I>Example : getting the contained value</I>
  @snippet misc/values.cpp boolean_get

  <I>Example : setting the contained value</I>
  @snippet misc/values.cpp boolean_set

  <I>Example : conversion from string</I>
  @snippet misc/values.cpp boolean_fromstr

  <I>Example : conversion to string</I>
  @snippet misc/values.cpp boolean_tostr

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "Variables"],
    "title" : "Boolean value",
    "text" : "openfluid::core::BooleanValue %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
class OPENFLUID_API BooleanValue : public SimpleValue
{
  private:

    bool m_Value;

  public:

    /**
      Default constructor
    */
    BooleanValue() : SimpleValue(), m_Value(false)
    { }

    /**
      Copy constructor
    */
    BooleanValue(const BooleanValue& Val) : SimpleValue(), m_Value(Val.m_Value)
    { }

    /**
      Constructor from plain old type
    */
    BooleanValue(const bool& POD) : SimpleValue(), m_Value(POD)
    { }

    virtual ~BooleanValue()
    { }

    Value& operator =(const Value& Other);

    /**
    * Cast operator
    */
    operator bool() const
    {
      return m_Value;
    }

    inline Type getType() const
    {
      return Value::BOOLEAN;
    }

    Value* clone() const
    { return new BooleanValue(*this); }

    bool convert(Value& Val) const;

    /**
      Returns the boolean value as plain old type
      @return the boolean value
    */
    inline bool get() const
    {
      return m_Value;
    }

    /**
      Sets the plain old type boolean value
      @param[in] Val the boolean value
    */
    inline void set(const bool& Val)
    {
      m_Value = Val;
    }

    void writeToStream(std::ostream& OutStm) const;

    void writeQuotedToStream(std::ostream& OutStm) const
    {
      writeToStream(OutStm);
    }

};


} }  // namespaces


#endif /* __OPENFLUID_CORE_BOOLEANVALUE_HPP__ */
