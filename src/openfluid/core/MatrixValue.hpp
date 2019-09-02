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
  @file MatrixValue.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CORE_MATRIXVALUE_HPP__
#define __OPENFLUID_CORE_MATRIXVALUE_HPP__


#include <openfluid/core/CompoundValue.hpp>
#include <openfluid/core/Matrix.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

/**
  MatrixValue is a container for a 2D matrix of signed double precision floating point values.\n

  @see Value
  @see Matrix

  <I>Example : declaration</I>
  @snippet misc/values.cpp matrix_decl

  <I>Example : getting the contained values</I>
  @snippet misc/values.cpp matrix_get

  <I>Example : getting all values as a c-style array of double, rows-first indexed</I>
  @snippet misc/values.cpp matrix_get_carray

  <I>Example : setting the contained values</I>
  @snippet misc/values.cpp matrix_set

  <I>Example : conversion from string</I>
  @snippet misc/values.cpp matrix_fromstr

  <I>Example : conversion to string</I>
  @snippet misc/values.cpp matrix_tostr

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "Values"],
    "title" : "Matrix of double values",
    "text" : "openfluid::core::MatrixValue %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
class OPENFLUID_API MatrixValue : public CompoundValue, public Matrix<double>
{

  public:

    /**
      Default constructor
    */
    MatrixValue() : CompoundValue(), Matrix<double>()
    { }

    /**
      Copy constructor
    */
    MatrixValue(const MatrixValue& Val) : CompoundValue(),
                                          Matrix<double>(static_cast<const Matrix<double>& >(Val))
    { }

    /**
      Constructor, creates a Matrix containing ColsNbr x RowsNbr elements
    */
    MatrixValue(unsigned long ColsNbr,unsigned long RowsNbr) : CompoundValue(), Matrix<double>(ColsNbr,RowsNbr)
    {

    }

    /**
      Constructor, creates a Matrix containing ColsNbr x RowsNbr elements, initialized with value InitValue
    */
    MatrixValue(unsigned long ColsNbr, unsigned long RowsNbr, double InitValue) :
      CompoundValue(), Matrix<double>(ColsNbr,RowsNbr,InitValue)
    {

    }

    virtual ~MatrixValue()
    {

    }

    Value& operator =(const Value& Other);

    inline Type getType() const
    {
      return Value::MATRIX;
    };

    Value* clone() const
    {
      return new MatrixValue(*this);
    }

    void writeToStream(std::ostream& OutStm) const;

    void writeQuotedToStream(std::ostream& OutStm) const
    {
      writeToStream(OutStm);
    }

};


} }  // namespaces


#endif /* __OPENFLUID_CORE_MATRIXVALUE_HPP__ */
