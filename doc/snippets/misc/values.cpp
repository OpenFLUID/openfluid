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
  @file values.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/MapValue.hpp>
#include <openfluid/core/StringValue.hpp>


void snippet_Values()
{

{
//! [boolean_decl]
// declaration of a BooleanValue, initialized to false by default
openfluid::core::BooleanValue Val1;

// declaration of a BooleanValue, initialized to true
openfluid::core::BooleanValue Val2(true);
//! [boolean_decl]


//! [boolean_get]
bool Tmp1;

// using the get method
Tmp1 = Val1.get();

// or using the cast operator
Tmp1 = Val1;
//! [boolean_get]

if (Tmp1)
{
  
} // to avoid "unused var" message from compiler


//! [boolean_set]
// using the set method
Val1.set(true);
//! [boolean_set]


//! [boolean_fromstr]
openfluid::core::StringValue StringVal("true");

// to BooleanValue
StringVal.toBooleanValue(Val1);

// to bool
bool BoolVal; 
StringVal.toBoolean(BoolVal);
//! [boolean_fromstr]


//! [boolean_tostr]
std::string StdStrVal = Val1.toString();
//! [boolean_tostr]
}


//////////


{
//! [double_decl]
// declaration of a DoubleValue, initialized to 0.0 by default
openfluid::core::DoubleValue Val1;

// declaration of a DoubleValue, initialized to 1.357
openfluid::core::DoubleValue Val2(1.357);
//! [double_decl]


//! [double_get]
double Tmp1;

// using the get method
Tmp1 = Val1.get();

// or using the cast operator
Tmp1 = Val1;
//! [double_get]

if (Tmp1)
{
  
} // to avoid "unused var" message from compiler


//! [double_set]
// using the set method
Val1.set(101.99);
//! [double_set]


//! [double_fromstr]
openfluid::core::StringValue StringVal("57.33");

// to DoubleValue
StringVal.toDoubleValue(Val1);

// to double
double DblVal;
StringVal.toDouble(DblVal);
//! [double_fromstr]


//! [double_tostr]
std::string StdStrVal = Val1.toString();
//! [double_tostr]
}


//////////


{
//! [integer_decl]
// declaration of an IntegerValue, initialized to 0 by default
openfluid::core::IntegerValue Val1;

// declaration of an IntegerValue, initialized to 35
openfluid::core::IntegerValue Val2(35);
//! [integer_decl]


//! [integer_get]
long Tmp1;

// using the get method
Tmp1 = Val1.get();

// or using the cast operator
Tmp1 = Val1;
//! [integer_get]

if (Tmp1)
{
  
} // to avoid "unused var" message from compiler


//! [integer_set]
// using the set method
Val1.set(-10199);
//! [integer_set]


//! [integer_fromstr]
openfluid::core::StringValue StringVal("57");

// to IntegerValue
StringVal.toIntegerValue(Val1);

// to long
long LongVal;
StringVal.toInteger(LongVal);
//! [integer_fromstr]


//! [integer_tostr]
std::string StdStrVal = Val1.toString();
//! [integer_tostr]
}


//////////


{
//! [map_decl]
// declaration of a MapValue, empty by default
openfluid::core::MapValue Val1;
//! [map_decl]


//! [map_set]
// using the generic set method (notice the new operator)
Val1.set("myvalue1",new openfluid::core::DoubleValue(18.05));

// using a specific set method
Val1.setDouble("myvalue2",openfluid::core::DoubleValue(0.005));

// using a specific set method
Val1.setMatrixValue("myvalue3",openfluid::core::MatrixValue(3,3,1.99));
//! [map_set]


//! [map_get]
openfluid::core::DoubleValue Tmp1;
double DblTmp1;

// using the generic at method
Tmp1 = Val1.at("myvalue1").asDoubleValue();

// using specific get methods
Tmp1 = Val1.getDouble("myvalue1");
DblTmp1 = Val1.getDouble("myvalue1");

// or using the [] operator
Tmp1 = Val1["myvalue1"].asDoubleValue();
//! [map_get]

if (DblTmp1)
{
  
} // to avoid "unused var" message from compiler


//! [map_test]
// testing if a key exist
Val1.isKeyExist("myvalue1"); // true in this case;

// testing if a key exist and the contained value type
Val1.isKeyExist("myvalue2") && Val1["myvalue2"].getType() == openfluid::core::Value::BOOLEAN; // false in this case
//! [map_test]


//! [map_fromstr]
openfluid::core::StringValue StringVal;
openfluid::core::MapValue Val2;

// to MapValue, using a string values separator
StringVal.set("{\"myvalue1\":toto,\"myvalue2\"=12.56,\"myvalue3\"=17,\"myvalue3\"=false");
StringVal.toMapValue(Val2);

// all values are stored as strings, that can be converted to other types
openfluid::core::IntegerValue TmpInt;
Val2.at("myvalue3").asStringValue().toIntegerValue(TmpInt);
//! [map_fromstr]


//! [map_tostr]
std::string StdStrVal = Val1.toString();
//! [map_tostr]
}


//////////


{
//! [matrix_decl]
// declaration of a MatrixValue, empty by default
openfluid::core::MatrixValue Val1;

// declaration of a MatrixValue, of 3 columns by 7 rows, with values initialized to 0.0
openfluid::core::MatrixValue Val2(3,7);

// declaration of a MatrixValue, of 3 columns by 7 rows, with values initialized to 1.99
openfluid::core::MatrixValue Val3(3,7,1.99);
//! [matrix_decl]


//! [matrix_get]
double Tmp1;

Tmp1 = Val1.get(2,3);
//! [matrix_get]

if (Tmp1)
{
  
} // to avoid "unused var" message from compiler


//! [matrix_get_carray]
double* DblArrayVal;

DblArrayVal = Val1.data();
//! [matrix_get_carray]

if (DblArrayVal)
{
  
} // to avoid "unused var" message from compiler


//! [matrix_set]
// using the set method
Val1.set(0,0,101.99);
//! [matrix_set]


//! [matrix_fromstr]
openfluid::core::StringValue StringVal;

// to MatrixValue, using two string values separators
StringVal.set("[[3,5,2.8,6,17.999923],[1,1,1,1,1],[2.11,2.12,2.13,2.14,2.15]]");
StringVal.toMatrixValue(Val2);

// to MatrixValue, using a single string values separator and row length
StringVal.set("[3,5,2.8,6,17.999923,1,1,1,1,1,2.11,2.12,2.13,2.14,2.15]");
StringVal.toMatrixValue(5,Val2);
//! [matrix_fromstr]


//! [matrix_tostr]
std::string StdStrVal = Val1.toString();
//! [matrix_tostr]
}


///////////


{
//! [string_decl]
// declaration of a StringValue, initialized to an empty string by default
openfluid::core::StringValue Val1;

// declaration of a StringValue, initialized to "hello world"
openfluid::core::StringValue Val2("hello world");
//! [string_decl]


//! [string_get]
std::string Tmp1;

// using the get method
Tmp1 = Val1.get();

// or using the cast operator
Tmp1 = Val1;
//! [string_get]

if (Tmp1.empty())
{
  
} // to avoid "unused var" message from compiler


//! [string_set]
// using the set method
Val1.set("Have a nice day");
//! [string_set]
}


//////////


{
//! [vector_decl]
// declaration of a VectorValue, empty by default
openfluid::core::VectorValue Val1;

// declaration of a VectorValue of 7 elements, with values initialized to 0.0
openfluid::core::VectorValue Val2(7);

// declaration of a VectorValue of 7 elements, with values initialized to 1.99
openfluid::core::VectorValue Val3(7,1.99);
//! [vector_decl]


//! [vector_get]
double Tmp1;

// using the get method
Tmp1 = Val1.get(2);

// or using the [] operator
Tmp1 = Val1[2];
//! [vector_get]

if (Tmp1)
{
  
} // to avoid "unused var" message from compiler


//! [vector_get_carray]
double* DblArrayVal;

DblArrayVal = Val1.data();
//! [vector_get_carray]

if (DblArrayVal)
{
  
} // to avoid "unused var" message from compiler


//! [vector_set]
// using the set method
Val1.set(0,101.99);

// or using the [] operator
Val1[0] = 101.99;
//! [vector_set]


//! [vector_fromstr]
openfluid::core::StringValue StringVal;


// to VectorValue, using a string values separator
StringVal.set("[3,5,2.8,6,17.999923,1,1,1,1,1,2.11,2.12,2.13,2.14,2.15]");
StringVal.toVectorValue(Val2);
//! [vector_fromstr]


//! [vector_tostr]
std::string StdStrVal = Val1.toString();
//! [vector_tostr]
}

}
