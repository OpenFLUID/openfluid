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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_stringvalue
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

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
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(!Val3.toVectorValue(";",VectV));
  BOOST_REQUIRE(!Val3.toMatrixValue(";","|",MatV));
  BOOST_REQUIRE(!Val3.toMapValue(";",MapV));
  BOOST_REQUIRE_EQUAL(NullV.toString(),"null");
  std::cout << NullV << std::endl;

  // to double
  Val3.set("2.79");
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(Val3.toVectorValue(";",VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(";","|",MatV));
  BOOST_REQUIRE(!Val3.toMapValue(";",MapV));
  BOOST_REQUIRE_CLOSE(DoubleV.get(),2.79,0.000001);
  BOOST_REQUIRE_EQUAL(VectV.getSize(),1);
  std::cout << DoubleV << std::endl;

  // to int
  Val3.set("17");
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(Val3.toVectorValue(";",VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(";","|",MatV));
  BOOST_REQUIRE(!Val3.toMapValue(";",MapV));
  BOOST_REQUIRE_EQUAL(IntV.get(),17);
  std::cout << IntV << std::endl;


  // to bool
  Val3.set("true");
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(!Val3.toVectorValue(";",VectV));
  BOOST_REQUIRE(!Val3.toMatrixValue(";","|",MatV));
  BOOST_REQUIRE(!Val3.toMapValue(";",MapV));
  BOOST_REQUIRE_EQUAL(BoolV.get(),true);
  std::cout << BoolV << std::endl;

  Val3.set("0");
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(Val3.toVectorValue(";",VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(";","|",MatV));
  BOOST_REQUIRE(!Val3.toMapValue(";",MapV));
  BOOST_REQUIRE_EQUAL(BoolV.get(),false);
  std::cout << BoolV << std::endl;

  // to vector
  Val3.set("3;5;2.8;6;17.999923");
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(Val3.toVectorValue(";",VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(";","|",MatV));
  BOOST_REQUIRE(!Val3.toMapValue(";",MapV));
  BOOST_REQUIRE_EQUAL(VectV.getSize(),5);
  BOOST_REQUIRE_CLOSE(VectV.get(1),5.000,0.00001);
  BOOST_REQUIRE_CLOSE(VectV.get(4),17.999923,0.0000001);
  BOOST_REQUIRE_EQUAL(MatV.getColsNbr(),5);
  BOOST_REQUIRE_EQUAL(MatV.getRowsNbr(),1);
  std::cout << VectV << std::endl;

  // to matrix
  Val3.set("3;5;2.8;6;17.999923|1;1;1;1;1|2.11;2.12;2.13;2.14;2.15");
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(!Val3.toVectorValue(";",VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(";","|",MatV));
  BOOST_REQUIRE(!Val3.toMapValue(";",MapV));
  BOOST_REQUIRE_EQUAL(MatV.getColsNbr(),5);
  BOOST_REQUIRE_EQUAL(MatV.getRowsNbr(),3);
  BOOST_REQUIRE_CLOSE(MatV.get(0,0),3.000,0.00001);
  BOOST_REQUIRE_CLOSE(MatV.get(4,2),2.15,0.00001);
  BOOST_REQUIRE_CLOSE(MatV.get(2,1),1.00,0.00001);
  std::cout << MatV << std::endl;

  Val3.set("3;5;2.8;6;17.999923;1;1;1;1;1;2.11;2.12;2.13;2.14;2.15");
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(Val3.toVectorValue(";",VectV));
  BOOST_REQUIRE(Val3.toMatrixValue(";",5,MatV));
  BOOST_REQUIRE(!Val3.toMapValue(";",MapV));
  BOOST_REQUIRE_EQUAL(VectV.getSize(),15);
  BOOST_REQUIRE_EQUAL(MatV.getColsNbr(),5);
  BOOST_REQUIRE_EQUAL(MatV.getRowsNbr(),3);
  BOOST_REQUIRE_CLOSE(MatV.get(0,0),3.000,0.00001);
  BOOST_REQUIRE_CLOSE(MatV.get(4,2),2.15,0.00001);
  BOOST_REQUIRE_CLOSE(MatV.get(2,1),1.00,0.00001);
  std::cout << MatV << std::endl;


  // to map
  Val3.set("str=toto;dbl=12.56;int=17;bool=false");
  std::cout << Val3 << " -> ";
  BOOST_REQUIRE(!Val3.toNullValue(NullV));
  BOOST_REQUIRE(!Val3.toDoubleValue(DoubleV));
  BOOST_REQUIRE(!Val3.toIntegerValue(IntV));
  BOOST_REQUIRE(!Val3.toBooleanValue(BoolV));
  BOOST_REQUIRE(!Val3.toVectorValue(";",VectV));
  BOOST_REQUIRE(!Val3.toMatrixValue(";","|",MatV));
  BOOST_REQUIRE(Val3.toMapValue(";",MapV));
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

}


