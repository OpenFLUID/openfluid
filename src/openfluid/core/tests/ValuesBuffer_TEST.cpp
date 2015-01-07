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
  @file SSerieValues_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_sserievalues
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <openfluid/core/ValuesBuffer.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/VectorValue.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::ValuesBuffer VBuffer;
  openfluid::core::IndexedValue IValue;
  openfluid::core::IndexedValueList IValueList;

  BOOST_REQUIRE_EQUAL(IValue.getIndex(),0);
  BOOST_REQUIRE(IValue.value()->isNullValue());

  BOOST_REQUIRE_EQUAL(VBuffer.getValuesCount(),0);
  BOOST_REQUIRE_EQUAL(VBuffer.getCurrentIndex(),-1);
  BOOST_REQUIRE_EQUAL(VBuffer.getLatestIndexedValue(IValue),false);

  BOOST_REQUIRE_EQUAL(VBuffer.getLatestIndexedValues(0,IValueList),false);

  BOOST_REQUIRE_EQUAL(VBuffer.getIndexedValues(1,2,IValueList),false);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{

  openfluid::core::ValuesBufferProperties::setBufferSize(5);
  openfluid::core::ValuesBuffer VBuffer;
  openfluid::core::DoubleValue Value;
  openfluid::core::IndexedValue IValue;
  openfluid::core::IndexedValueList IValueList;

  BOOST_REQUIRE_EQUAL(VBuffer.getValuesCount(),0);

  BOOST_REQUIRE_EQUAL(VBuffer.modifyCurrentValue(openfluid::core::DoubleValue(1.1)),false);

  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(0,openfluid::core::DoubleValue(1.1)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(1,openfluid::core::DoubleValue(2.2)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(2,openfluid::core::DoubleValue(3.3)),true);

  BOOST_REQUIRE_EQUAL(VBuffer.getCurrentIndex(),2);

  BOOST_REQUIRE_EQUAL(VBuffer.getValue(2,&Value),true);
  BOOST_REQUIRE_CLOSE(Value.get(),3.3,0.001);

  BOOST_REQUIRE_EQUAL(VBuffer.getLatestIndexedValue(IValue),true);
  BOOST_REQUIRE_EQUAL(IValue.getIndex(),2);
  BOOST_REQUIRE_CLOSE(IValue.value()->asDoubleValue().get(),3.3,0.001);

  BOOST_REQUIRE_EQUAL(VBuffer.getLatestIndexedValues(1,IValueList),true);
  BOOST_REQUIRE_EQUAL(IValueList.size(),2);
  BOOST_REQUIRE_CLOSE(IValueList.front().value()->asDoubleValue().get(),2.2,0.001);
  BOOST_REQUIRE_CLOSE(IValueList.back().value()->asDoubleValue().get(),3.3,0.001);


  BOOST_REQUIRE_EQUAL(VBuffer.getLatestIndexedValues(2,IValueList),true);
  BOOST_REQUIRE_EQUAL(IValueList.size(),1);

  BOOST_REQUIRE_EQUAL(VBuffer.getLatestIndexedValues(3,IValueList),true);
  BOOST_REQUIRE_EQUAL(IValueList.size(),0);

  BOOST_REQUIRE_EQUAL(VBuffer.getLatestIndexedValues(0,IValueList),true);
  BOOST_REQUIRE_EQUAL(IValueList.size(),3);



  BOOST_REQUIRE_EQUAL(VBuffer.getIndexedValues(1,2,IValueList),true);
  BOOST_REQUIRE_EQUAL(IValueList.size(),2);
  BOOST_REQUIRE_CLOSE(IValueList.front().value()->asDoubleValue().get(),2.2,0.001);
  BOOST_REQUIRE_CLOSE(IValueList.back().value()->asDoubleValue().get(),3.3,0.001);

  BOOST_REQUIRE_EQUAL(VBuffer.getIndexedValues(2,1,IValueList),false);
  BOOST_REQUIRE_EQUAL(IValueList.size(),0);

  BOOST_REQUIRE_EQUAL(VBuffer.getIndexedValues(0,2,IValueList),true);
  BOOST_REQUIRE_EQUAL(IValueList.size(),3);
  BOOST_REQUIRE_CLOSE(IValueList.front().value()->asDoubleValue().get(),1.1,0.001);
  BOOST_REQUIRE_CLOSE(IValueList.back().value()->asDoubleValue().get(),3.3,0.001);

  BOOST_REQUIRE_EQUAL(VBuffer.getIndexedValues(5,18,IValueList),true);
  BOOST_REQUIRE_EQUAL(IValueList.size(),0);


  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(3,openfluid::core::DoubleValue(4.4)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(4,openfluid::core::DoubleValue(5.5)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(5,openfluid::core::DoubleValue(6.6)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(6,openfluid::core::DoubleValue(7.7)),true);

  BOOST_REQUIRE_EQUAL(VBuffer.getCurrentIndex(),6);

  BOOST_REQUIRE_EQUAL(VBuffer.getValuesCount(),5);

  BOOST_REQUIRE_EQUAL(VBuffer.getCurrentValue(&Value),true);
  BOOST_REQUIRE_CLOSE(Value.get(),7.7,0.001);

  BOOST_REQUIRE_EQUAL(VBuffer.getValue(0,&Value),false);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(7,&Value),false);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(1,&Value),false);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(2,&Value),true);


  BOOST_REQUIRE_EQUAL(VBuffer.getValue(3,&Value),true);
  BOOST_REQUIRE_CLOSE(Value.get(),4.4,0.001);

  BOOST_REQUIRE_EQUAL(VBuffer.modifyValue(3,openfluid::core::DoubleValue(44.0)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(3,&Value),true);
  BOOST_REQUIRE_CLOSE(Value.get(),44.0,0.001);


  BOOST_REQUIRE_EQUAL(VBuffer.modifyValue(6,openfluid::core::DoubleValue(77.0)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(6,&Value),true);
  BOOST_REQUIRE_CLOSE(Value.get(),77.0,0.001);

  BOOST_REQUIRE_EQUAL(VBuffer.modifyCurrentValue(openfluid::core::DoubleValue(7.7)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(6,&Value),true);
  BOOST_REQUIRE_CLOSE(Value.get(),7.7,0.001);


/*  BOOST_REQUIRE_EQUAL(VBuffer.deleteValues(3),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(3,&Value),false);
  BOOST_REQUIRE_NE(VBuffer.getValue(3,&Value),true);*/

  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(7,openfluid::core::DoubleValue(10.1)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(8,openfluid::core::DoubleValue(20.2)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(9,openfluid::core::DoubleValue(30.3)),true);

//  BOOST_REQUIRE_EQUAL(VBuffer.deleteValues(8),true);

}

BOOST_AUTO_TEST_CASE(check_type_operations)
{
  openfluid::core::ValuesBufferProperties::setBufferSize(8);
  openfluid::core::ValuesBuffer VBuffer;

  openfluid::core::DoubleValue DblValue;
  openfluid::core::IntegerValue IntValue;
  openfluid::core::StringValue StrValue;
  openfluid::core::BooleanValue BoolValue;
  openfluid::core::NullValue NullValue;
  openfluid::core::VectorValue VectValue;

  BOOST_REQUIRE_EQUAL(VBuffer.getCurrentIndex(),-1);

  BOOST_REQUIRE_EQUAL(VBuffer.isValueExist(0),false);
  BOOST_REQUIRE_EQUAL(VBuffer.isValueExist(5),false);

  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(10,openfluid::core::DoubleValue(1000.1)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(11,openfluid::core::DoubleValue(1001.1)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getCurrentIndex(),11);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(12,openfluid::core::DoubleValue(1002.1)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(13,openfluid::core::DoubleValue(1003.1)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.isValueExist(0),false);
  BOOST_REQUIRE_EQUAL(VBuffer.isValueExist(5),false);
  BOOST_REQUIRE_EQUAL(VBuffer.isValueExist(10),true);
  BOOST_REQUIRE_EQUAL(VBuffer.isValueExist(11),true);
  BOOST_REQUIRE_EQUAL(VBuffer.isValueExist(13),true);
  BOOST_REQUIRE_EQUAL(VBuffer.isValueExist(15),false);
  BOOST_REQUIRE_EQUAL(VBuffer.getCurrentIndex(),13);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(10,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),1000.1,0.001);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(12,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),1002.1,0.001);
  DblValue.set(0.0);
  DblValue = *VBuffer.value(12);
  BOOST_REQUIRE_CLOSE(DblValue.get(),1002.1,0.001);
  BOOST_REQUIRE(!VBuffer.value(14));

  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(14,openfluid::core::IntegerValue(1004)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(12,&IntValue),false);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(14,&IntValue),true);
  BOOST_REQUIRE_EQUAL(IntValue.get(),1004);
  BOOST_REQUIRE_EQUAL((long)IntValue,1004);
  BOOST_REQUIRE_EQUAL((int)IntValue,1004);
  IntValue.set(0);
  IntValue = *VBuffer.value(14);
  BOOST_REQUIRE_EQUAL(IntValue.get(),1004);

  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(15,openfluid::core::StringValue("1005")),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(12,&StrValue),false);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(15,&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue.get(),"1005");
  BOOST_REQUIRE_EQUAL((std::string)StrValue,"1005");
  StrValue.set("");
  StrValue = *VBuffer.value(15);
  BOOST_REQUIRE_EQUAL(StrValue.get(),"1005");

  BOOST_REQUIRE_EQUAL(VBuffer.getCurrentIndex(),15);

  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(16,openfluid::core::BooleanValue(true)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(12,&BoolValue),false);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(16,&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue.get(),true);
  BOOST_REQUIRE_EQUAL((bool)BoolValue,true);
  BoolValue.set(false);
  BoolValue = *VBuffer.value(16);
  BOOST_REQUIRE_EQUAL(BoolValue.get(),true);

  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(17,openfluid::core::NullValue()),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(12,&NullValue),false);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(17,&NullValue),true);

  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(18,openfluid::core::VectorValue(3,1.1)),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(12,&VectValue),false);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(18,&VectValue),true);
  BOOST_REQUIRE_EQUAL(VectValue.size(),3);
  BOOST_REQUIRE_EQUAL(VectValue[0],1.1);
  BOOST_REQUIRE_EQUAL(VectValue[2],1.1);
  VectValue.clear();
  VectValue = *VBuffer.value(18);
  BOOST_REQUIRE_EQUAL(VectValue.size(),3);
  BOOST_REQUIRE_EQUAL(VectValue[0],1.1);
  BOOST_REQUIRE_EQUAL(VectValue[2],1.1);


  BOOST_REQUIRE_EQUAL(VBuffer.modifyValue(13,IntValue),true);
  BOOST_REQUIRE_EQUAL(VBuffer.modifyValue(14,StrValue),true);
  BOOST_REQUIRE_EQUAL(VBuffer.modifyValue(15,BoolValue),true);
  BOOST_REQUIRE_EQUAL(VBuffer.modifyValue(16,NullValue),true);
  BOOST_REQUIRE_EQUAL(VBuffer.modifyValue(17,VectValue),true);
  BOOST_REQUIRE_EQUAL(VBuffer.modifyValue(18,DblValue),true);

  BOOST_REQUIRE_EQUAL(VBuffer.getValue(13,&IntValue),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(14,&IntValue),false);
  IntValue.set(0);
  IntValue = *VBuffer.value(13);
  BOOST_REQUIRE_EQUAL(IntValue.get(),1004);

  BOOST_REQUIRE_EQUAL(VBuffer.getValue(14,&StrValue),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(12,&StrValue),false);
  StrValue.set("");
  StrValue = *VBuffer.value(14);
  BOOST_REQUIRE_EQUAL(StrValue.get(),"1005");

  BOOST_REQUIRE_EQUAL(VBuffer.getValue(15,&BoolValue),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(16,&BoolValue),false);
  BOOST_REQUIRE_EQUAL(BoolValue.get(),true);
  BOOST_REQUIRE_EQUAL((bool)BoolValue,true);
  BoolValue.set(false);
  BoolValue = *VBuffer.value(15);
  BOOST_REQUIRE_EQUAL(BoolValue.get(),true);

  BOOST_REQUIRE_EQUAL(VBuffer.getValue(16,&NullValue),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(17,&NullValue),false);

  BOOST_REQUIRE_EQUAL(VBuffer.getValue(17,&VectValue),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(18,&VectValue),false);
  VectValue.clear();
  VectValue = *VBuffer.value(17);
  BOOST_REQUIRE_EQUAL(VectValue.size(),3);
  BOOST_REQUIRE_EQUAL(VectValue[0],1.1);
  BOOST_REQUIRE_EQUAL(VectValue[2],1.1);

  BOOST_REQUIRE_EQUAL(VBuffer.getValue(18,&DblValue),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(13,&DblValue),false);
  DblValue.set(0.0);
  DblValue = *VBuffer.value(18);
  BOOST_REQUIRE_CLOSE(DblValue.get(),1002.1,0.001);


  //TODO check other Compound Values

  std::cout << VBuffer.getValuesCount() << std::endl;

  // check insertion before simulation head
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(16,openfluid::core::BooleanValue(true)),false);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(18,openfluid::core::BooleanValue(true)),false);

  BOOST_REQUIRE_EQUAL(VBuffer.getCurrentIndex(),18);

}

// =====================================================================
// =====================================================================
