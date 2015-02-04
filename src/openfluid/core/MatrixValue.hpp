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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_CORE_MATRIXVALUE_HPP__
#define __OPENFLUID_CORE_MATRIXVALUE_HPP__

#include <openfluid/core/CompoundValue.hpp>
#include <openfluid/core/Matrix.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

/**
  MatrixValue is a container for a 2D matrix of signed double precision floating point values.\n

\see Value
\see Matrix

\n

<I>Example : declaration</I>
@code
  // declaration of a MatrixValue, empty by default
  openfluid::core::MatrixValue Val1;

  // declaration of a MatrixValue, of 3 columns by 7 rows, with values initialized to 0.0
  openfluid::core::MatrixValue Val2(3,7);

  // declaration of a MatrixValue, of 3 columns by 7 rows, with values initialized to 1.99
  openfluid::core::MatrixValue Val3(3,7,1.99);
@endcode


<I>Example : getting the contained values</I>
@code
  double Tmp1;

  Tmp1 = Val1.get(2,3);
@endcode


<I>Example : getting all values as a c-style array of double, rows-first indexed</I>
@code
  double DblArrayVal[];

  DblArrayVal = Val1.getData();
@endcode


<I>Example : setting the contained values</I>
@code
  // using the set method
  Val1.set(0,0,101.99);
@endcode


<I>Example : conversion from string</I>
@code
  openfluid::core::StringValue StringVal;
  openfluid::core::MatrixValue Val2;

  // to MatrixValue, using two string values separators
  StringVal.set("3;5;2.8;6;17.999923|1;1;1;1;1|2.11;2.12;2.13;2.14;2.15");
  StringVal.toMatrixValue(";","|",Val2);

  // to MatrixValue, using a single string values separator and row length
  StringVal.set("3;5;2.8;6;17.999923;1;1;1;1;1;2.11;2.12;2.13;2.14;2.15");
  StringVal.toMatrixValue(";",5,Val2);
@endcode


<I>Example : conversion to string</I>
@code
  std::string StdStrVal = Val1.toString();
@endcode
*/
class OPENFLUID_API MatrixValue : public CompoundValue, public Matrix<double>
{

  public:

    /**
      Default constructor
    */
    MatrixValue() : CompoundValue(), Matrix<double>() {};

    /**
      Copy constructor
    */
    MatrixValue(const MatrixValue& Val) :
      CompoundValue(static_cast<const CompoundValue&>(Val)),
      Matrix<double>(static_cast<const Matrix<double>& >(Val)) {};

    /**
      Constructor, creates a Matrix containing ColsNbr x RowsNbr elements
    */
    MatrixValue(unsigned long ColsNbr,unsigned long RowsNbr) : CompoundValue(), Matrix<double>(ColsNbr,RowsNbr) {};

    /**
      Constructor, creates a Matrix containing ColsNbr x RowsNbr elements, initialized with value InitValue
    */
    MatrixValue(unsigned long ColsNbr, unsigned long RowsNbr, double InitValue) :
      CompoundValue(), Matrix<double>(ColsNbr,RowsNbr,InitValue) {};

    Value& operator =(const Value& Other);

    virtual ~MatrixValue() {};

    inline Type getType() const { return Value::MATRIX; };

    Value* clone() const { return new MatrixValue(*this); };

    void writeToStream(std::ostream& OutStm) const;

};


} }  // namespaces



#endif /* __OPENFLUID_CORE_MATRIXVALUE_HPP__ */
