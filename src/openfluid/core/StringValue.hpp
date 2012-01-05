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
  \file StringValue.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __STRINGVALUE_HPP___
#define __STRINGVALUE_HPP___


#include <string>

#include <openfluid/core/SimpleValue.hpp>

#include <openfluid/dllexport.hpp>



namespace openfluid { namespace core {
/**
StringValue is a container for a std::string value, with methods for conversion to other containers derived from Value.\n

\see Value

\n

<I>Example : declaration</I>
@code
  // declaration of a StringValue, initialized to an empty string by default
  openfluid::core::StringValue Val1;

  // declaration of a StringValue, initialized to "hello world"
  openfluid::core::StringValue Val2("hello world");
@endcode


<I>Example : getting the contained value</I>
@code
  std::string Tmp1;

  // using the get method
  Tmp1 = Val1.get();

  // or using the cast operator
  Tmp1 = Val1;
@endcode


<I>Example : setting the contained value</I>
@code
  // using the set method
  Val1.set("Have a nice day");
@endcode
*/
class DLLEXPORT StringValue : public SimpleValue
{
  private:

    std::string m_Value;

    static bool convertStringToDouble(const std::string& Str, double& Dbl);


  public:

    /**
      Default constructor
    */
    StringValue() : m_Value("") {};

    /**
      Copy constructor
    */
    StringValue(const StringValue& Val) : SimpleValue(Val), m_Value(Val.m_Value) {};

    /**
      Constructor from plain old type
    */
    StringValue(const std::string& POD) : SimpleValue(), m_Value(POD) {};

    Value& operator =(const Value& Other);

    /**
     * Cast operator
     */
    operator std::string() const { return m_Value; };

    virtual ~StringValue() {};

    inline Type getType() const { return Value::STRING; };

    Value* clone() const { return new StringValue(*this); };

    /**
      Returns the string value as plain old type
      @return the string value
    */
    inline std::string& get() { return m_Value; };

    /**
      Returns the string value as a const plain old type
      @return the string value
    */
    inline const std::string& get() const { return m_Value; };

    /**
      Sets the string value
      @param[in] Val the string value
    */
    inline void set(const std::string& Val) { m_Value = Val; };

    void writeToStream(std::ostream& OutStm) const;

    /**
      Returns the size of the string
      @return size of the string
    */
    inline unsigned long getSize() const { return m_Value.size(); };

    /**
      Returns the size of the string
      @return size of the string
    */
    unsigned long size() const { return m_Value.size(); };

    /**
      Converts the contained string to a double value (if possible)
      @return bool true if the conversion is correct, false otherwise
    */
    bool toDouble(double& Val) const;

    /**
      Converts the contained string to a DoubleValue (if possible)
      @param[out] Val the converted value
      @return bool true if the conversion is correct, false otherwise
    */
    bool toDoubleValue(DoubleValue& Val) const;

    /**
      Converts the contained string to a boolean value (if possible)
      @param[out] Val the converted value
      @return bool true if the conversion is correct, false otherwise
    */
    bool toBoolean(bool& Val) const;

    /**
      Converts the contained string to a BooleanValue (if possible)
      @param[out] Val the converted value
      @return bool true if the conversion is correct, false otherwise
    */
    bool toBooleanValue(BooleanValue& Val) const;

    /**
      Converts the contained string to a long value (if possible)
      @param[out] Val the converted value
      @return bool true if the conversion is correct, false otherwise
    */
    bool toInteger(long& Val) const;

    /**
      Converts the contained string to an IntegerValue (if possible)
      @param[out] Val the converted value
      @return bool true if the conversion is correct, false otherwise
    */
    bool toIntegerValue(IntegerValue& Val) const;

    /**
      Converts the contained string to a NullValue (if possible)
      @param[out] Val the converted value
      @return bool true if the conversion is correct, false otherwise
    */
    bool toNullValue(NullValue& Val) const;

    /**
      Converts the contained string to a VectorValue value (if possible)
      @param[in] Sep the separator used to split the string into vector items
      @param[out] Val the converted value
      @return bool true if the conversion is correct, false otherwise
    */
    bool toVectorValue(const std::string& Sep, VectorValue& Val) const;

    /**
      Converts the contained string to a MatrixValue value (if possible)
      @param[in] ColSep the column separator used to split the string columns
      @param[in] RowSep the row separator used to split the string rows
      @param[out] Val the converted value
      @return bool true if the conversion is correct, false otherwise
    */
    bool toMatrixValue(const std::string& ColSep, const std::string& RowSep, MatrixValue& Val) const;

    /**
      Converts the contained string to a MatrixValue value (if possible)
      @param[in] Sep the separator used to split the string
      @param[in] RowLength the size of a row
      @param[out] Val the converted value
      @return bool true if the conversion is correct, false otherwise
    */
    bool toMatrixValue(const std::string& Sep, const unsigned int& RowLength, MatrixValue& Val) const;

    /**
      Converts the contained string to a MapValue value (if possible)
      @param[in] Sep the separator used to split the string into map items
      @param[out] Val the converted value
      @return bool true if the conversion is correct, false otherwise
    */
    bool toMapValue(const std::string& Sep, MapValue& Val) const;

};


} }  // namespaces



#endif /* __STRINGVALUE_HPP___ */
