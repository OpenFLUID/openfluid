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
  \file VectorValue.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __VECTORVALUE_HPP___
#define __VECTORVALUE_HPP___

#include <openfluid/core/CompoundValue.hpp>
#include <openfluid/core/Vector.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

/**
  VectorValue is a container for a 1D vector of signed double precision floating point values.\n

\see Value
\see Vector

\n

<I>Example : declaration</I>
@code
  // declaration of a VectorValue, empty by default
  openfluid::core::VectorValue Val1();

  // declaration of a VectorValue of 7 elements, with values initialized to 0.0
  openfluid::core::VectorValue Val2(7);

  // declaration of a VectorValue of 7 elements, with values initialized to 1.99
  openfluid::core::VectorValue Val3(7,1.99);
@endcode


<I>Example : getting the contained values</I>
@code
  double Tmp1;
  // using the get method
  Tmp1 = Val1.get(2);

  // or using the [] operator
  Tmp1 = Val1[2];
@endcode


<I>Example : getting all values as a c-style array of double</I>
@code
  double DblArrayVal[];

  DblArrayVal = Val1.getData();
@endcode


<I>Example : setting the contained values</I>
@code
  // using the set method
  Val1.set(0,101.99);

  // or using the [] operator
  Val1[0] = 101.99;
@endcode


<I>Example : conversion from string</I>
@code
  openfluid::core::StringValue StringVal;
  openfluid::core::VectorValue Val2;

  // to VectorValue, using a string values separator
  StringVal.set("3;5;2.8;6;17.999923;1;1;1;1;1;2.11;2.12;2.13;2.14;2.15");
  StringVal.toVectorValue(";",Val2);
@endcode


<I>Example : conversion to string</I>
@code
  std::string StdStrVal = Val1.toString();
@endcode
*/
class DLLEXPORT VectorValue : public CompoundValue, public Vector<double>
{

  public:

    /**
      Default constructor
    */
    VectorValue() : CompoundValue(), Vector<double>() {};

    /**
      Copy constructor
    */
    VectorValue(const VectorValue& Val) : CompoundValue(static_cast<const CompoundValue&>(Val)), Vector<double>(static_cast<const Vector<double>& >(Val)) {};

    /**
      Constructor, creates a vector containing Size elements
    */
    VectorValue(unsigned long Size) : CompoundValue(), Vector<double>(Size) {};

    /**
      Constructor, creates a vector containing Size elements, initialized with value InitValue
    */
    VectorValue(unsigned long Size, double InitValue) : CompoundValue(), Vector<double>(Size,InitValue) {};

    /**
      Constructor, creates a vector of size Size, containing Data
    */
    VectorValue(double* Data, unsigned long Size) : CompoundValue(), Vector<double>(Data,Size) {};

    Value& operator =(const Value& Other);

    virtual ~VectorValue() {};

    inline Type getType() const { return Value::VECTOR; };

    Value* clone() const { return new VectorValue(*this); };

    void writeToStream(std::ostream& OutStm) const;

};


} }  // namespaces



#endif /* __VECTORVALUE_HPP___ */
