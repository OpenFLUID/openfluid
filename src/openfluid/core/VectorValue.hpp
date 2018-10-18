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
  @file VectorValue.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CORE_VECTORVALUE_HPP__
#define __OPENFLUID_CORE_VECTORVALUE_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/core/CompoundValue.hpp>
#include <openfluid/core/Vector.hpp>


namespace openfluid { namespace core {

/**
  VectorValue is a container for a 1D vector of signed double precision floating point values.\n

\see Value
\see Vector

\n

<I>Example : declaration</I>
@code
  // declaration of a VectorValue, empty by default
  openfluid::core::VectorValue Val1;

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
  StringVal.set("[3,5,2.8,6,17.999923,1,1,1,1,1,2.11,2.12,2.13,2.14,2.15]");
  StringVal.toVectorValue(Val2);
@endcode


<I>Example : conversion to string</I>
@code
  std::string StdStrVal = Val1.toString();
@endcode
*/
class OPENFLUID_API VectorValue : public CompoundValue, public Vector<double>
{

  public:

    /**
      Default constructor
    */
    VectorValue() : CompoundValue(), Vector<double>()
    { }

    /**
      Copy constructor
    */
    VectorValue(const VectorValue& Val) :
      CompoundValue(),
      Vector<double>(static_cast<const Vector<double>& >(Val))
    { }

    /**
      Constructor, creates a vector containing Size elements
    */
    VectorValue(unsigned long Size) : CompoundValue(), Vector<double>(Size)
    {

    }

    /**
      Constructor, creates a vector containing Size elements, initialized with value InitValue
    */
    VectorValue(unsigned long Size, double InitValue) : CompoundValue(), Vector<double>(Size,InitValue)
    {

    }

    /**
      Constructor, creates a vector of size Size, containing Data
    */
    VectorValue(double* Data, unsigned long Size) : CompoundValue(), Vector<double>(Data,Size)
    {

    }

    virtual ~VectorValue()
    {

    }

    Value& operator =(const Value& Other);

    inline Type getType() const
    {
      return Value::VECTOR;
    }

    Value* clone() const
    {
      return new VectorValue(*this);
    }

    void writeToStream(std::ostream& OutStm) const;

    void writeQuotedToStream(std::ostream& OutStm) const
    {
      writeToStream(OutStm);
    }

};


} }  // namespaces


#endif /* __OPENFLUID_CORE_VECTORVALUE_HPP__ */
