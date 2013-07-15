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
  \file Value_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_value
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/scoped_ptr.hpp>


#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/MapValue.hpp>
#include <openfluid/core/StringValue.hpp>

#include <list>


#define DECLARE_TEST_TICKER(name) \
  boost::posix_time::ptime _M_Start_##name; \
  boost::posix_time::ptime _M_Stop_##name; \
  boost::posix_time::time_duration _M_Duration_##name;


#define START_TEST_TICKER(name) \
  _M_Start_##name = boost::posix_time::microsec_clock::local_time();


#define MARK_TEST_TICKER(name) \
  _M_Stop_##name = boost::posix_time::microsec_clock::local_time(); \
  _M_Duration_##name = _M_Stop_##name - _M_Start_##name; \


#define TEST_DURATION_AS_STRING(name) \
  (boost::posix_time::to_simple_string(_M_Duration_##name))


// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_null)
{
  std::cout << "======== check_null ========" << std::endl;

  openfluid::core::NullValue Val1;

  std::cout << Val1 << std::endl;
  BOOST_REQUIRE(Val1.isNullValue());

  BOOST_REQUIRE_EQUAL(Val1.isSimple(),false);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),false);

  openfluid::core::NullValue Val2 = Val1;
  BOOST_REQUIRE(Val2.isNullValue());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_integer)
{
  std::cout << "======== check_integer ========" << std::endl;

  openfluid::core::IntegerValue Val1;

  std::cout << Val1 << std::endl;
  BOOST_REQUIRE(Val1.isIntegerValue());
  BOOST_REQUIRE_EQUAL( Val1.get(), 0);

  BOOST_REQUIRE_EQUAL(Val1.isSimple(),true);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),false);

  Val1.set(17);
  std::cout << Val1 << std::endl;
  BOOST_REQUIRE_EQUAL( Val1.get(), 17);

  openfluid::core::IntegerValue Val2 = Val1;
  BOOST_REQUIRE_EQUAL( Val1.get(), Val2.get());

  Val2.set(17*7/45);
  std::cout << Val2 << " " << Val2.get() << " " << Val2.toString() << std::endl;


}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_double)
{
  std::cout << "======== check_double ========" << std::endl;

  openfluid::core::DoubleValue Val1;

  std::cout << Val1 << std::endl;
  BOOST_REQUIRE(Val1.isDoubleValue());
  BOOST_REQUIRE_CLOSE( Val1.get(), 0.0, 0.0001 );

  BOOST_REQUIRE_EQUAL(Val1.isSimple(),true);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),false);

  Val1.set(17.3);
  std::cout << Val1 << std::endl;
  BOOST_REQUIRE_CLOSE( Val1.get(), 17.3, 0.0001 );

  openfluid::core::DoubleValue Val2 = Val1;
  BOOST_REQUIRE_CLOSE( Val1.get(), Val2.get(), 0.0001 );

  Val2.set(17.3*7.19/45.67);
  std::cout << Val2 << " " << Val2.get() << " " << Val2.toString() << std::endl;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_boolean)
{
  std::cout << "======== check_boolean ========" << std::endl;

  openfluid::core::BooleanValue Val1;

  std::cout << Val1 << std::endl;
  BOOST_REQUIRE(Val1.isBooleanValue());
  BOOST_REQUIRE_EQUAL( Val1.get(), false );

  BOOST_REQUIRE_EQUAL(Val1.isSimple(),true);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),false);

  Val1.set(true);
  std::cout << Val1 << std::endl;
  BOOST_REQUIRE_EQUAL( Val1.get(),true);

  openfluid::core::BooleanValue Val2 = Val1;
  BOOST_REQUIRE_EQUAL( Val1.get(), Val2.get());

  Val2.set(true && (false || true));
  std::cout << Val2 << " " << Val2.get() << " " << Val2.toString() << std::endl;

  openfluid::core::BooleanValue Val3;
  Val3 = Val1;
  BOOST_REQUIRE_EQUAL( Val3.get(), Val2.get());

}


// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_vector)
{
  std::cout << "======== check_vector ========" << std::endl;

  openfluid::core::VectorValue Val1;


  std::cout << Val1 << std::endl;
  BOOST_REQUIRE(Val1.isVectorValue());
  BOOST_REQUIRE_EQUAL( Val1.getSize(), 0 );

  BOOST_REQUIRE_EQUAL(Val1.isSimple(),false);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),true);

  Val1 = openfluid::core::VectorValue(30,1.30);

  BOOST_REQUIRE_EQUAL( Val1.getSize(), 30 );
  BOOST_REQUIRE_CLOSE( Val1.at(16), 1.3,0.000001 );
  BOOST_REQUIRE_CLOSE( Val1[29], 1.3,0.000001 );

  std::cout << Val1 << std::endl;


  openfluid::core::VectorValue Val2 = Val1;

  BOOST_REQUIRE_EQUAL( Val1.getSize(), Val2.getSize());
  BOOST_REQUIRE_CLOSE( Val2.at(16), 1.3,0.000001 );
  BOOST_REQUIRE_CLOSE( Val1.at(16), Val2.at(16),0.000001 );
  BOOST_REQUIRE_CLOSE( Val2[29], 1.3,0.000001 );

  Val2[28] = 28.28;
  BOOST_REQUIRE_CLOSE( Val2[28], 28.28,0.000001 );
  BOOST_REQUIRE_CLOSE( Val1[28], 1.3,0.000001 );


  std::cout << Val2 << std::endl;

  std::cout << Val2.toString() << std::endl;

  Val2.clear();
  BOOST_REQUIRE_EQUAL(Val2.getSize(),0);
  BOOST_REQUIRE_EQUAL(Val1.getSize(),30);

  openfluid::core::VectorValue Val3;
  Val3 = Val2;

}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_matrix)
{
  std::cout << "======== check_matrix ========" << std::endl;

  openfluid::core::MatrixValue Val1;

  std::cout << Val1 << std::endl;
  BOOST_REQUIRE(Val1.isMatrixValue());
  BOOST_REQUIRE_EQUAL( Val1.getColsNbr(), 0 );
  BOOST_REQUIRE_EQUAL( Val1.getRowsNbr(), 0 );

  BOOST_REQUIRE_EQUAL(Val1.isSimple(),false);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),true);


  Val1 = openfluid::core::MatrixValue(20,30,1.35);

  BOOST_REQUIRE_EQUAL( Val1.getColsNbr(), 20 );
  BOOST_REQUIRE_EQUAL( Val1.getRowsNbr(), 30 );
  BOOST_REQUIRE_CLOSE( Val1.at(16,17), 1.35,0.000001 );
  BOOST_REQUIRE_CLOSE( Val1.get(19,29), 1.35,0.000001 );

  BOOST_REQUIRE_THROW(Val1.get(45,5),openfluid::base::FrameworkException);

  std::cout << Val1 << std::endl;


  openfluid::core::MatrixValue Val2;

  Val2 = Val1;

  BOOST_REQUIRE_EQUAL( Val1.getSize(), Val2.getSize());
  BOOST_REQUIRE_CLOSE( Val2.at(9,9), 1.35,0.000001 );
  BOOST_REQUIRE_CLOSE( Val1.at(9,9), Val2.at(9,9),0.000001 );
  BOOST_REQUIRE_CLOSE( Val2.get(9,9), 1.35,0.000001 );

  Val2.set(9,9,28.28);
  BOOST_REQUIRE_CLOSE( Val2.get(9,9), 28.28,0.000001 );
  BOOST_REQUIRE_CLOSE( Val1.get(9,9), 1.35,0.000001 );


  std::cout << Val2 << std::endl;

  std::cout << Val2.toString() << std::endl;

  Val2.clear();
  BOOST_REQUIRE_EQUAL(Val2.getSize(),0);
  BOOST_REQUIRE_EQUAL(Val2.getColsNbr(),0);
  BOOST_REQUIRE_EQUAL(Val2.getRowsNbr(),0);
  BOOST_REQUIRE_EQUAL(Val1.getSize(),600);


  openfluid::core::MatrixValue Val3 = openfluid::core::MatrixValue(2,3,0.0);
  BOOST_REQUIRE_EQUAL(Val3.getColsNbr(),2);
  BOOST_REQUIRE_EQUAL(Val3.getRowsNbr(),3);
  std::cout << Val3.toString() << std::endl;
  BOOST_REQUIRE_EQUAL(Val3.toString(),"0;0|0;0|0;0");

  openfluid::core::StringValue StrTest("1.1;1.2|2.1;2.2|3.1;3.2");
  Val3.clear();
  StrTest.toMatrixValue(";","|",Val3);
  std::cout << Val3.toString() << std::endl;
  BOOST_REQUIRE_EQUAL(Val3.toString(),"1.1;1.2|2.1;2.2|3.1;3.2");

  Val3.set(1,2,9.99);
  BOOST_REQUIRE_CLOSE( Val3.get(1,2), 9.99,0.000001 );

  openfluid::core::MatrixValue Val4;

  Val4 = Val3;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_map)
{
  std::cout << "======== check_map ========" << std::endl;

  openfluid::core::MapValue Val1;

  std::cout << Val1 << std::endl;
  BOOST_REQUIRE_EQUAL(Val1.isMapValue(),true);
  BOOST_REQUIRE_EQUAL(Val1.isSimple(),false);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),true);
  BOOST_REQUIRE_EQUAL(Val1.getSize(),0);
  BOOST_REQUIRE_EQUAL(Val1.isKeyExist("foo"),false);

  Val1.set("foo", new openfluid::core::DoubleValue(1.5));
  BOOST_REQUIRE_EQUAL(Val1["foo"].getType(),openfluid::core::Value::DOUBLE);
  BOOST_REQUIRE_EQUAL(Val1.isKeyExist("foo"),true);
  BOOST_REQUIRE_CLOSE(Val1["foo"].asDoubleValue().get(),1.5,0.000001);
  BOOST_REQUIRE_EQUAL(Val1.getSize(),1);
  std::cout << Val1 << std::endl;

  Val1.set("bar", new openfluid::core::VectorValue(5,5.5));
  Val1.setString("foobar", "you're talking to me?");
  Val1.setBoolean("foofoobar", true);
  BOOST_REQUIRE_EQUAL(Val1.getSize(),4);
  BOOST_REQUIRE_CLOSE(Val1.getDouble("foo"),1.5,0.0001);
  BOOST_REQUIRE_EQUAL(Val1.get("bar").asVectorValue().getSize(),5);
  BOOST_REQUIRE_EQUAL(Val1.get("foofoobar").asBooleanValue().get(),true);
  BOOST_REQUIRE_EQUAL(Val1.get("foobar").asStringValue().get(),"you're talking to me?");

  Val1.setVectorValue("bar",openfluid::core::VectorValue(7,7.7));
  BOOST_REQUIRE_EQUAL(Val1.getVectorValue("bar").getSize(),7);
  std::cout << Val1 << std::endl;

  Val1.remove("foobar");

  BOOST_REQUIRE_EQUAL(Val1.getSize(),3);
  Val1.setInteger("bar",17);
  BOOST_REQUIRE_EQUAL(Val1["bar"].getType(),openfluid::core::Value::INTEGER);
  BOOST_REQUIRE_EQUAL(Val1["bar"].asIntegerValue().get(),17);
  std::cout << Val1 << std::endl;

  openfluid::core::MapValue Val2 = Val1;

  BOOST_REQUIRE_EQUAL(Val2.getSize(),3);
  BOOST_REQUIRE_CLOSE(Val2.getDouble("foo"),1.5,0.0001);
  BOOST_REQUIRE_EQUAL(Val2.get("bar").asIntegerValue().get(),17);
  BOOST_REQUIRE_EQUAL(Val2.get("foofoobar").asBooleanValue().get(),true);

  Val2.setMatrixValue("foofoobar",openfluid::core::MatrixValue(5,9,29.5));
  BOOST_REQUIRE_EQUAL(Val1["foofoobar"].getType(),openfluid::core::Value::BOOLEAN);
  BOOST_REQUIRE_EQUAL(Val2["foofoobar"].getType(),openfluid::core::Value::MATRIX);
  BOOST_REQUIRE_EQUAL(Val2["foofoobar"].asMatrixValue().getColsNbr(),5);
  BOOST_REQUIRE_EQUAL(Val2.getMatrixValue("foofoobar").getRowsNbr(),9);

  BOOST_REQUIRE_THROW(Val2.getMatrixValue("foo"),openfluid::base::FrameworkException);
  BOOST_REQUIRE_THROW(Val2.get("foo").asIntegerValue(),openfluid::base::FrameworkException);
  BOOST_REQUIRE_THROW(Val2.get("wrongfoo"),openfluid::base::FrameworkException);
}


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
  BOOST_REQUIRE(MapV.get("dbl").asStringValue().toDoubleValue(DoubleV));
  BOOST_REQUIRE_CLOSE(DoubleV.get(),12.560,0.000001);
  BOOST_REQUIRE_EQUAL(MapV.getString("dbl"),"12.56");
  BOOST_REQUIRE(MapV.get("int").asStringValue().toIntegerValue(IntV));
  BOOST_REQUIRE_EQUAL(IntV.get(),17);
  BOOST_REQUIRE_EQUAL(MapV.getString("int"),"17");
  BOOST_REQUIRE(MapV.get("bool").asStringValue().toBooleanValue(BoolV));
  BOOST_REQUIRE_EQUAL(BoolV.get(),false);
  BOOST_REQUIRE_EQUAL(MapV.getString("bool"),"false");
  std::cout << MapV << std::endl;


}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_sequence)
{
  std::cout << "======== check_sequence ========" << std::endl;


  std::list< boost::shared_ptr<openfluid::core::Value> > ValuesSeq;


  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::DoubleValue>(new openfluid::core::DoubleValue(3.7)));
  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::MapValue>(new openfluid::core::MapValue()));
  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::BooleanValue>(new openfluid::core::BooleanValue(true)));
  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::VectorValue>(new openfluid::core::VectorValue(500,17.0)));
  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::MatrixValue>(new openfluid::core::MatrixValue(29,47,1955.19)));
  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::StringValue>(new openfluid::core::StringValue("Welcome to the jungle!")));
  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::IntegerValue>(new openfluid::core::IntegerValue(1984)));
  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::NullValue>(new openfluid::core::NullValue()));

  while(!ValuesSeq.empty())
  {
    bool Processed = false;

    if (ValuesSeq.front()->isBooleanValue())
    {
      Processed = true;

      BOOST_REQUIRE_EQUAL(ValuesSeq.front().get()->asBooleanValue().get(), true);

      openfluid::core::BooleanValue* BoolVTmp = static_cast<openfluid::core::BooleanValue*>(ValuesSeq.front().get());
      BOOST_REQUIRE_EQUAL(BoolVTmp->get(), true);

    }


    if (ValuesSeq.front()->isDoubleValue())
    {
      Processed = true;

      BOOST_REQUIRE_CLOSE(ValuesSeq.front().get()->asDoubleValue().get(), 3.7, 0.0001 );

      openfluid::core::DoubleValue* DblVTmp = static_cast<openfluid::core::DoubleValue*>(ValuesSeq.front().get());
      BOOST_REQUIRE_CLOSE(DblVTmp->get(), 3.7, 0.0001 );

    }


    if (ValuesSeq.front()->isIntegerValue())
    {

      BOOST_REQUIRE_EQUAL(ValuesSeq.front().get()->asIntegerValue().get(),1984);

      openfluid::core::IntegerValue* IntVTmp = static_cast<openfluid::core::IntegerValue*>(ValuesSeq.front().get());
      BOOST_REQUIRE_EQUAL(IntVTmp->get(),1984);

      Processed = true;
    }

    if (ValuesSeq.front()->isVectorValue())
    {
      BOOST_REQUIRE_EQUAL(ValuesSeq.front().get()->asVectorValue().getSize(),500);

      openfluid::core::VectorValue* VectVTmp = static_cast<openfluid::core::VectorValue*>(ValuesSeq.front().get());
      BOOST_REQUIRE_EQUAL(VectVTmp->getSize(),500);

      Processed = true;
    }


    if (ValuesSeq.front()->isMatrixValue())
    {
      BOOST_REQUIRE_EQUAL(ValuesSeq.front().get()->asMatrixValue().getSize(),(29*47));

      openfluid::core::MatrixValue* MatVTmp = static_cast<openfluid::core::MatrixValue*>(ValuesSeq.front().get());
      BOOST_REQUIRE_EQUAL(MatVTmp->getColsNbr(),29);
      BOOST_REQUIRE_EQUAL(MatVTmp->getRowsNbr(),47);

      Processed = true;
    }


    if (ValuesSeq.front()->isMapValue())
    {
      BOOST_REQUIRE_EQUAL(ValuesSeq.front().get()->asMapValue().getSize(),0);

      Processed = true;
    }


    if (ValuesSeq.front()->isStringValue())
    {
      BOOST_REQUIRE_EQUAL(ValuesSeq.front().get()->asStringValue().get(),"Welcome to the jungle!");

      Processed = true;
    }


    if (ValuesSeq.front()->isNullValue())
    {
      Processed = true;
    }

    //std::cout << (*(ValuesSeq.front())) << std::endl;
    ValuesSeq.front()->writeToStream(std::cout);
    std::cout << std::endl;

    ValuesSeq.pop_front();

    BOOST_REQUIRE(Processed);
  }
}

// =====================================================================
// =====================================================================


class ValueCapsule
{
  public:

    boost::shared_ptr<openfluid::core::Value> m_Val;

    ValueCapsule()
    : m_Val(new openfluid::core::NullValue())
    {

    }

    void set(const openfluid::core::Value& Val)
    {
      m_Val.reset(Val.clone());
    };

    void get(openfluid::core::Value* Val)
    {
      *Val = *m_Val;
    }

};


BOOST_AUTO_TEST_CASE(check_capsule)
{
  std::cout << "======== check_capsule ========" << std::endl;

  ValueCapsule Capsule;

  openfluid::core::DoubleValue DblVal(0.1);

  Capsule.set(openfluid::core::DoubleValue(3.14));

  Capsule.get(&DblVal);

  BOOST_REQUIRE_CLOSE(DblVal.get(),3.14,0.0001);

}



// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_performance)
{

  std::cout << "======== check_performance ========" << std::endl;


  DECLARE_TEST_TICKER();

  const unsigned int ElementsNbr = 10000000;
  const unsigned int BufferSize = 5;

  boost::circular_buffer<double> BufferDouble(BufferSize);
  boost::circular_buffer<boost::shared_ptr<openfluid::core::Value> > BufferValue(5000);





  START_TEST_TICKER();

  for (unsigned int i=0; i< ElementsNbr;i++)
  {
    BufferDouble.push_back(static_cast<double>(i)/ElementsNbr);
  }

  MARK_TEST_TICKER();
  std::cout << "Duration [double], populating: " << TEST_DURATION_AS_STRING() << std::endl;



  START_TEST_TICKER();

  for (unsigned int i=0; i< ElementsNbr;i++)
  {
    BufferValue.push_back(boost::shared_ptr<openfluid::core::Value>(new openfluid::core::DoubleValue(static_cast<double>(i)/ElementsNbr)));
  }

  MARK_TEST_TICKER();
  std::cout << "Duration [DoubleValue], populating: " << TEST_DURATION_AS_STRING() << std::endl;


  START_TEST_TICKER();

  for (unsigned int i=0; i< BufferSize;i++)
  {
    double Tmp = BufferDouble[i];
    std::cout << Tmp << std::endl;
  }

  MARK_TEST_TICKER();
  std::cout << "Duration [double], accessing: " << TEST_DURATION_AS_STRING() << std::endl;



  START_TEST_TICKER();

  for (unsigned int i=0; i< BufferSize;i++)
  {
    double Tmp = BufferValue[i].get()->asDoubleValue().get();
    std::cout << Tmp << std::endl;
  }

  MARK_TEST_TICKER();
  std::cout << "Duration [DoubleValue], accessing: " << TEST_DURATION_AS_STRING() << std::endl;




}


