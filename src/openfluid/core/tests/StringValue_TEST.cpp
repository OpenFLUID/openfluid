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
  @file StringValue_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_stringvalue


#include <boost/test/unit_test.hpp>

#include <openfluid/core/StringValue.hpp>
#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/MapValue.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_string)
{
  std::cout << "======== check_string ========" << std::endl;

  openfluid::core::StringValue Val1;

  std::cout << Val1 << std::endl;
  BOOST_REQUIRE_EQUAL(Val1.isStringValue(),true);
  BOOST_REQUIRE_EQUAL(Val1.isSimple(),true);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),false);
  BOOST_REQUIRE_EQUAL(Val1.get().size(),0);

  Val1.set("ABCDE");
  BOOST_REQUIRE_EQUAL(Val1.guessTypeConversion(),openfluid::core::Value::STRING);
  std::cout << Val1 << std::endl;
  BOOST_REQUIRE_EQUAL(Val1.get(),"ABCDE");
  BOOST_REQUIRE_EQUAL(Val1.get().size(),5);

  openfluid::core::StringValue Val2 = Val1;
  std::cout << Val2 << std::endl;
  BOOST_REQUIRE_EQUAL(Val2.get(),"ABCDE");
  BOOST_REQUIRE_EQUAL(Val2.get().size(),5);

  Val2.set("EGDC");
  std::cout << Val2 << std::endl;
  BOOST_REQUIRE_EQUAL(Val2.get(),"EGDC");
  BOOST_REQUIRE_EQUAL(Val2.size(),4);
  BOOST_REQUIRE_EQUAL(Val1.get(),"ABCDE");
  BOOST_REQUIRE_EQUAL(Val1.getSize(),5);


  BOOST_REQUIRE_EQUAL(Val2.toString(),"EGDC");


  // check other constructors
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("char*").get(),"char*");
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue(42).get(),"42");
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue(false).get(),"false");
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue(true).get(),"true");
  double TmpDbl;
  openfluid::core::StringValue(99.9).toDouble(TmpDbl);
  BOOST_REQUIRE_CLOSE(TmpDbl,99.9,0.001);

  openfluid::core::StringValue Val3;

  openfluid::core::NullValue NullV;
  openfluid::core::DoubleValue DoubleV;
  openfluid::core::IntegerValue IntV;
  openfluid::core::BooleanValue BoolV;
  openfluid::core::VectorValue VectV;
  openfluid::core::MatrixValue MatV;
  openfluid::core::MapValue MapV;

  // to null
  Val3.set("null");
  BOOST_REQUIRE_EQUAL(Val3.guessTypeConversion(),openfluid::core::Value::NULLL);
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(!Val3.toVectorValue(VectV));
  BOOST_REQUIRE(!Val3.toMatrixValue(MatV));
  BOOST_REQUIRE(!Val3.toMapValue(MapV));
  BOOST_REQUIRE_EQUAL(NullV.toString(),"null");
  std::cout << NullV << std::endl;

  // to double
  Val3.set("2.79");
  BOOST_REQUIRE_EQUAL(Val3.guessTypeConversion(),openfluid::core::Value::DOUBLE);
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(Val3.toVectorValue(VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(MatV));
  BOOST_REQUIRE(!Val3.toMapValue(MapV));
  BOOST_REQUIRE_CLOSE(DoubleV.get(),2.79,0.000001);
  BOOST_REQUIRE_EQUAL(VectV.getSize(),1);
  std::cout << DoubleV << std::endl;

  // to int
  Val3.set("17");
  BOOST_REQUIRE_EQUAL(Val3.guessTypeConversion(),openfluid::core::Value::INTEGER);
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(Val3.toVectorValue(VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(MatV));
  BOOST_REQUIRE(!Val3.toMapValue(MapV));
  BOOST_REQUIRE_EQUAL(IntV.get(),17);
  std::cout << IntV << std::endl;


  // to bool
  Val3.set("true");
  BOOST_REQUIRE_EQUAL(Val3.guessTypeConversion(),openfluid::core::Value::BOOLEAN);
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(Val3.toVectorValue(VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(MatV));
  BOOST_REQUIRE(!Val3.toMapValue(MapV));
  BOOST_REQUIRE_EQUAL(BoolV.get(),true);
  BOOST_REQUIRE_EQUAL(IntV.get(),1);
  BOOST_REQUIRE_EQUAL(VectV.size(),1);
  std::cout << BoolV << std::endl;

  Val3.set("0");
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(Val3.toVectorValue(VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(MatV));
  BOOST_REQUIRE(!Val3.toMapValue(MapV));
  BOOST_REQUIRE_EQUAL(BoolV.get(),false);
  std::cout << BoolV << std::endl;

  // to vector
  Val3.set("[3,5,2.8,6,17.999923]");
  BOOST_REQUIRE_EQUAL(Val3.guessTypeConversion(),openfluid::core::Value::VECTOR);
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(Val3.toVectorValue(VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(MatV));
  BOOST_REQUIRE(!Val3.toMapValue(MapV));
  BOOST_REQUIRE_EQUAL(VectV.getSize(),5);
  BOOST_REQUIRE_CLOSE(VectV.get(1),5.000,0.00001);
  BOOST_REQUIRE_CLOSE(VectV.get(4),17.999923,0.0000001);
  std::cout << VectV << std::endl;
  BOOST_REQUIRE_EQUAL(MatV.getSize(),5);
  BOOST_REQUIRE_CLOSE(MatV.get(1,0),5.000,0.00001);
  BOOST_REQUIRE_CLOSE(MatV.get(4,0),17.999923,0.0000001);


  // to vector, from deprecated style
  Val3.set("3;5;2.8;6;17.999923");
  BOOST_REQUIRE_EQUAL(Val3.guessTypeConversion(),openfluid::core::Value::STRING);
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(Val3.toVectorValue(VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(MatV));
  BOOST_REQUIRE(!Val3.toMapValue(MapV));
  BOOST_REQUIRE_EQUAL(VectV.getSize(),5);
  BOOST_REQUIRE_CLOSE(VectV.get(1),5.000,0.00001);
  BOOST_REQUIRE_CLOSE(VectV.get(4),17.999923,0.0000001);
  BOOST_REQUIRE_EQUAL(MatV.getColsNbr(),5);
  BOOST_REQUIRE_EQUAL(MatV.getRowsNbr(),1);
  std::cout << VectV << std::endl;

  // to matrix
  Val3.set("[[3,5,2.8,6,17.999923],[1,1,1,1,1],[2.11,2.12,2.13,2.14,2.15]]");
  BOOST_REQUIRE_EQUAL(Val3.guessTypeConversion(),openfluid::core::Value::MATRIX);
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(!Val3.toVectorValue(VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(MatV));
  BOOST_REQUIRE(!Val3.toMapValue(MapV));
  BOOST_REQUIRE_EQUAL(MatV.getColsNbr(),5);
  BOOST_REQUIRE_EQUAL(MatV.getRowsNbr(),3);
  BOOST_REQUIRE_CLOSE(MatV.get(0,0),3.000,0.00001);
  BOOST_REQUIRE_CLOSE(MatV.get(4,2),2.15,0.00001);
  BOOST_REQUIRE_CLOSE(MatV.get(2,1),1.00,0.00001);
  std::cout << MatV << std::endl;

  Val3.set("[3,5,2.8,6,17.999923,1,1,1,1,1,2.11,2.12,2.13,2.14,2.15]");
  BOOST_REQUIRE_EQUAL(Val3.guessTypeConversion(),openfluid::core::Value::VECTOR);
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(Val3.toVectorValue(VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(5,MatV));
  BOOST_REQUIRE(!Val3.toMapValue(MapV));
  BOOST_REQUIRE_EQUAL(VectV.getSize(),15);
  BOOST_REQUIRE_EQUAL(MatV.getColsNbr(),5);
  BOOST_REQUIRE_EQUAL(MatV.getRowsNbr(),3);
  BOOST_REQUIRE_CLOSE(MatV.get(0,0),3.000,0.00001);
  BOOST_REQUIRE_CLOSE(MatV.get(4,2),2.15,0.00001);
  BOOST_REQUIRE_CLOSE(MatV.get(2,1),1.00,0.00001);
  std::cout << MatV << std::endl;

  // to map
  Val3.set(R"({"str":"toto","dbl":12.56,"int":17,"bool":false})");
  BOOST_REQUIRE_EQUAL(Val3.guessTypeConversion(),openfluid::core::Value::MAP);
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(!Val3.toVectorValue(VectV));
  BOOST_REQUIRE(!Val3.toMatrixValue(MatV));
  BOOST_REQUIRE(Val3.toMapValue(MapV));
  BOOST_REQUIRE_EQUAL(MapV.getSize(),4);
  BOOST_REQUIRE_EQUAL(MapV.getString("str"),"toto");
  BOOST_REQUIRE_CLOSE(MapV.getDouble("dbl"),12.560,0.000001);
  BOOST_REQUIRE_EQUAL(MapV.getInteger("int"),17);
  BOOST_REQUIRE_EQUAL(MapV.getBoolean("bool"),false);
  std::cout << MapV << std::endl;

  // to map, from deprecated style
  Val3.set("str=toto;dbl=12.56;int=17;bool=false");
  BOOST_REQUIRE_EQUAL(Val3.guessTypeConversion(),openfluid::core::Value::STRING);
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(!Val3.toVectorValue(VectV));
  BOOST_REQUIRE(!Val3.toMatrixValue(MatV));
  BOOST_REQUIRE(Val3.toMapValue(MapV));
  BOOST_REQUIRE_EQUAL(MapV.getSize(),4);
  BOOST_REQUIRE_EQUAL(MapV.getString("str"),"toto");
  BOOST_REQUIRE(MapV.at("dbl").asStringValue().toDoubleValue(DoubleV));
  BOOST_REQUIRE_CLOSE(DoubleV.get(),12.560,0.000001);
  BOOST_REQUIRE_EQUAL(MapV.getString("dbl"),"12.56");
  BOOST_REQUIRE(MapV.at("int").asStringValue().toIntegerValue(IntV));
  BOOST_REQUIRE_EQUAL(IntV.get(),17);
  BOOST_REQUIRE_EQUAL(MapV.getString("int"),"17");
  BOOST_REQUIRE(MapV.at("bool").asStringValue().toBooleanValue(BoolV));
  BOOST_REQUIRE_EQUAL(BoolV.get(),false);
  BOOST_REQUIRE_EQUAL(MapV.getString("bool"),"false");
  std::cout << MapV << std::endl;


  // to map, with complex nested values
  Val3.set(
    R"({"key1":2.25,"key2":"string","key3":true,"key4":[1,2,3.3],"key5":[[1.1,1.2,1.3],[2.1,2.2,2.3]],"key6":)"
      R"({"key11":2.25,"key22":"string","key33":true,"key44":[1,2,3.3],"key55":[[1.1,1.2,1.3],[2.1,2.2,2.3]],"key66":)"
        R"({"key111":2.25,"key222":"string","key333":true,"key444":[1,2,3.3],"key555":[[1.1,1.2,1.3],[2.1,2.2,2.3]]}}})"
          );

  BOOST_REQUIRE_EQUAL(Val3.guessTypeConversion(),openfluid::core::Value::MAP);
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(!Val3.toVectorValue(VectV));
  BOOST_REQUIRE(!Val3.toMatrixValue(MatV));
  BOOST_REQUIRE(Val3.toMapValue(MapV));
  BOOST_REQUIRE_CLOSE(MapV.getDouble("key1"),2.25,0.000001);
  BOOST_REQUIRE_EQUAL(MapV.getString("key2"),"string");
  BOOST_REQUIRE(MapV.getBoolean("key3"));

  openfluid::core::VectorValue MapVKey4 = MapV.getVectorValue("key4");
  BOOST_REQUIRE_EQUAL(MapVKey4.size(),3);
  BOOST_REQUIRE_CLOSE(MapVKey4.get(1),2.0,0.000001);
  BOOST_REQUIRE_CLOSE(MapVKey4.get(2),3.3,0.000001);

  openfluid::core::MatrixValue MapVKey5 = MapV.getMatrixValue("key5");
  BOOST_REQUIRE_EQUAL(MapVKey5.size(),6);
  BOOST_REQUIRE_EQUAL(MapVKey5.getColsNbr(),3);
  BOOST_REQUIRE_EQUAL(MapVKey5.getRowsNbr(),2);
  BOOST_REQUIRE_CLOSE(MapVKey5.get(1,1),2.2,0.000001);
  BOOST_REQUIRE_CLOSE(MapVKey5.get(2,1),2.3,0.000001);
  BOOST_REQUIRE_CLOSE(MapVKey5.get(0,0),1.1,0.000001);

  openfluid::core::MapValue MapVKey6 = MapV.getMapValue("key6");
  BOOST_REQUIRE_EQUAL(MapVKey6.size(),6);
  BOOST_REQUIRE(MapVKey6.isKeyExist("key11"));
  BOOST_REQUIRE(MapVKey6.isKeyExist("key55"));
  BOOST_REQUIRE(!MapVKey6.isKeyExist("key2"));
  BOOST_REQUIRE(!MapVKey6.isKeyExist("key333"));
  BOOST_REQUIRE_EQUAL(MapVKey6.getMatrixValue("key55").getColsNbr(),3);
  BOOST_REQUIRE_EQUAL(MapVKey6.getMapValue("key66").getSize(),5);
  BOOST_REQUIRE(MapVKey6.getMapValue("key66").isKeyExist("key555"));
  BOOST_REQUIRE_CLOSE(MapVKey6.getMapValue("key66").getMatrixValue("key555").get(1,0),1.2,0.00001);

  std::cout << MapV << std::endl;


  openfluid::core::StringValue SourceStr("my cheschire cat is blue");

  BOOST_REQUIRE_EQUAL(SourceStr.replaceAll("ch","bl"),2);
  std::cout << SourceStr << std::endl;

  BOOST_REQUIRE_EQUAL(SourceStr.replaceAll("bl","ch"),3);
  std::cout << SourceStr << std::endl;

  BOOST_REQUIRE_EQUAL(SourceStr.toString(),"my cheschire cat is chue");

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_guessconversion)
{
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("").guessTypeConversion(),openfluid::core::Value::NONE);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue(" ").guessTypeConversion(),openfluid::core::Value::STRING);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("aaaa").guessTypeConversion(),openfluid::core::Value::STRING);

  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("true").guessTypeConversion(),openfluid::core::Value::BOOLEAN);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("false").guessTypeConversion(),openfluid::core::Value::BOOLEAN);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("True").guessTypeConversion(),openfluid::core::Value::STRING);

  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("1").guessTypeConversion(),openfluid::core::Value::INTEGER);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("+1").guessTypeConversion(),openfluid::core::Value::INTEGER);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("-1").guessTypeConversion(),openfluid::core::Value::INTEGER);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("-1E+18").guessTypeConversion(),openfluid::core::Value::DOUBLE);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("-1E18").guessTypeConversion(),openfluid::core::Value::DOUBLE);

  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("2.99").guessTypeConversion(),openfluid::core::Value::DOUBLE);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("+2.99").guessTypeConversion(),openfluid::core::Value::DOUBLE);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("-2.99").guessTypeConversion(),openfluid::core::Value::DOUBLE);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("-1E-18").guessTypeConversion(),openfluid::core::Value::DOUBLE);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("-1e-18").guessTypeConversion(),openfluid::core::Value::DOUBLE);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("1.25e-18").guessTypeConversion(),openfluid::core::Value::DOUBLE);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("1.25e+18").guessTypeConversion(),openfluid::core::Value::DOUBLE);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("-1 E-18").guessTypeConversion(),openfluid::core::Value::STRING);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("2,99").guessTypeConversion(),openfluid::core::Value::STRING);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("2 99").guessTypeConversion(),openfluid::core::Value::STRING);

  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("[]").guessTypeConversion(),openfluid::core::Value::VECTOR);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("[1]").guessTypeConversion(),openfluid::core::Value::VECTOR);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("[1,2.09]").guessTypeConversion(),openfluid::core::Value::VECTOR);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("[1,2.09").guessTypeConversion(),openfluid::core::Value::STRING);

  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("[[]]").guessTypeConversion(),openfluid::core::Value::MATRIX);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("[[1,2],[33.3,44.444]]").guessTypeConversion(),
                      openfluid::core::Value::MATRIX);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("[[1,2],[33.3,44.444]").guessTypeConversion(),
                      openfluid::core::Value::VECTOR);

  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("{}").guessTypeConversion(),openfluid::core::Value::MAP);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("{\"k1\":1.2,\"k2\":\"str\"}").guessTypeConversion(),
                      openfluid::core::Value::MAP);

  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("null").guessTypeConversion(),openfluid::core::Value::NULLL);
  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue("NULL").guessTypeConversion(),openfluid::core::Value::STRING);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_convert)
{
  {
    openfluid::core::DoubleValue Val;
    BOOST_REQUIRE(openfluid::core::StringValue("2.199e-3").convert(Val));
    BOOST_REQUIRE_CLOSE(Val.get(),0.002199,0.00000001);
    BOOST_REQUIRE(openfluid::core::StringValue("-2.199e+1").convert(Val));
    BOOST_REQUIRE_CLOSE(Val.get(),-21.99,000001);
  }

  {
    openfluid::core::IntegerValue Val;
    BOOST_REQUIRE(openfluid::core::StringValue("-199").convert(Val));
    BOOST_REQUIRE_EQUAL(Val.get(),-199);
    BOOST_REQUIRE(openfluid::core::StringValue("199").convert(Val));
    BOOST_REQUIRE_EQUAL(Val.get(),199);
  }

  {
    openfluid::core::BooleanValue Val;
    BOOST_REQUIRE(openfluid::core::StringValue("true").convert(Val));
    BOOST_REQUIRE(Val.get());
  }

  {
    openfluid::core::VectorValue Val;
    BOOST_REQUIRE(openfluid::core::StringValue("[1.25,-2.199e+1]").convert(Val));
    BOOST_REQUIRE_EQUAL(Val.size(),2);
  }

  {
    openfluid::core::MatrixValue Val;
    BOOST_REQUIRE(openfluid::core::StringValue("[[1.25,-2.199e+1],[-1.25,2.199e-4]]").convert(Val));
    BOOST_REQUIRE_EQUAL(Val.size(),4);
  }

  {
    openfluid::core::MapValue Val;
    BOOST_REQUIRE(openfluid::core::StringValue("{\"k1\":1.2,\"k2\":\"str\"}").convert(Val));
    BOOST_REQUIRE_EQUAL(Val.size(),2);
  }
}
