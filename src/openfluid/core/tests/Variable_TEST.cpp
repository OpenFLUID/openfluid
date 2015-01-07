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
  @file Variable_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_variable
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <openfluid/core/ValuesBufferProperties.hpp>
#include <openfluid/core/Variables.hpp>
#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/IntegerValue.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::ValuesBufferProperties::setBufferSize(5);
  openfluid::core::Variables Vars;
  openfluid::core::DoubleValue Value;

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo"),false);
  BOOST_REQUIRE_EQUAL(Vars.getVariableValuesCount("foo"),-1);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",0,&Value),false);
  BOOST_REQUIRE_EQUAL(Vars.getCurrentValueIfIndex("bar",0,&Value),false);
  BOOST_REQUIRE(Vars.currentValueIfIndex("bar",0) == NULL);
  BOOST_REQUIRE_EQUAL(Vars.getCurrentValueIfIndex("bar",15,&Value),false);
  BOOST_REQUIRE_EQUAL(Vars.modifyCurrentValue("bar",Value),false);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::ValuesBufferProperties::setBufferSize(20);
  openfluid::core::Variables Vars;

  openfluid::core::DoubleValue DblValue;
  openfluid::core::IntegerValue IntValue;
  openfluid::core::StringValue StrValue;
  openfluid::core::BooleanValue BoolValue;
  openfluid::core::NullValue NullValue;
  openfluid::core::VectorValue VectValue;


  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo"),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",0),false);
  BOOST_REQUIRE_EQUAL(Vars.getVariableValuesCount("foo"),-1);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",0,&DblValue),false);

  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",1,openfluid::core::DoubleValue(0.0)),false);

  BOOST_REQUIRE_EQUAL(Vars.createVariable("foo"),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo"),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("foo",openfluid::core::Value::NONE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",0),false);
  BOOST_REQUIRE_EQUAL(Vars.modifyCurrentValue("foo",openfluid::core::DoubleValue(0.0)),false);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",10,openfluid::core::DoubleValue(0.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",11,openfluid::core::DoubleValue(1.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",12,openfluid::core::DoubleValue(2.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.getCurrentValueIfIndex("foo",12,&DblValue),true);
  BOOST_REQUIRE_EQUAL(Vars.getCurrentValueIfIndex("foo",11,&DblValue),false);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",13,openfluid::core::DoubleValue(3.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",14,openfluid::core::DoubleValue(4.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",10),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",14),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("foo",14,openfluid::core::Value::NONE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",5),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("foo",5,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",10,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",10,openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",10,openfluid::core::Value::INTEGER),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",14,openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",14,openfluid::core::Value::INTEGER),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",15,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",15,openfluid::core::Value::INTEGER),false);
  BOOST_REQUIRE_EQUAL(Vars.getVariableValuesCount("foo"),5);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",10,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),0.0,0.001);
  BOOST_REQUIRE_CLOSE((double)DblValue,0.0,0.001);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",0,&IntValue),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",13,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),3.0,0.001);
  BOOST_REQUIRE_CLOSE((double)DblValue,3.0,0.001);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",13,&IntValue),false);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("foo",13,openfluid::core::DoubleValue(34.5)),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",13,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),34.5,0.001);
  BOOST_REQUIRE_CLOSE((double)DblValue,34.5,0.001);
  BOOST_REQUIRE_EQUAL(Vars.getCurrentValueIfIndex("foo",13,&DblValue),false);
  BOOST_REQUIRE_EQUAL(Vars.getCurrentValueIfIndex("foo",14,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),4.0,0.001);
  BOOST_REQUIRE_EQUAL(Vars.getCurrentValueIfIndex("foo",15,&DblValue),false);
  BOOST_REQUIRE(Vars.currentValueIfIndex("foo",15) == NULL);
  BOOST_REQUIRE_CLOSE(DblValue.get(),4.0,0.001);

  BOOST_REQUIRE_EQUAL(Vars.modifyCurrentValue("foo",openfluid::core::DoubleValue(4.5)),true);
  BOOST_REQUIRE_EQUAL(Vars.getCurrentValue("foo",&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),4.5,0.001);
  BOOST_REQUIRE_EQUAL(Vars.modifyCurrentValue("foo",openfluid::core::DoubleValue(4.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.getCurrentValue("foo",&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),4.0,0.001);

  BOOST_REQUIRE_EQUAL(Vars.createVariable("bar"),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",15,openfluid::core::DoubleValue(1000.1)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",16,openfluid::core::DoubleValue(1001.1)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",17,openfluid::core::DoubleValue(1002.1)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",18,openfluid::core::DoubleValue(1003.1)),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",15,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),1000.1,0.001);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",17,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),1002.1,0.001);
  DblValue.set(0.0);
//  DblValue = *Vars.getValue("bar",2);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",17,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),1002.1,0.001);
//  BOOST_REQUIRE(!Vars.getValue("bar",4));
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",4,&DblValue),false);

  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",20,openfluid::core::IntegerValue(1004)),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",20,openfluid::core::Value::INTEGER),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",20,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",20,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",15,&IntValue),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",20,&IntValue),true);
  BOOST_REQUIRE_EQUAL(IntValue.get(),1004);
  BOOST_REQUIRE_EQUAL((long)IntValue,1004);
  BOOST_REQUIRE_EQUAL((int)IntValue,1004);
  IntValue.set(0);
//  IntValue = *Vars.getValue("bar",4);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",20,&IntValue),true);
  BOOST_REQUIRE_EQUAL(IntValue.get(),1004);

  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",30,openfluid::core::StringValue("1005")),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",30,openfluid::core::Value::STRING),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",30,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",30,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",20,&StrValue),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",30,&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue.get(),"1005");
  BOOST_REQUIRE_EQUAL((std::string)StrValue,"1005");
  StrValue.set("");
//  StrValue = *Vars.getValue("bar",5);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",30,&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue.get(),"1005");

  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",40,openfluid::core::BooleanValue(true)),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",40,openfluid::core::Value::BOOLEAN),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",40,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",40,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",20,&BoolValue),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",40,&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue.get(),true);
  BOOST_REQUIRE_EQUAL((bool)BoolValue,true);
  BoolValue.set(false);
//  BoolValue = *Vars.getValue("bar",6);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",40,&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue.get(),true);

  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",50,openfluid::core::NullValue()),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",50,openfluid::core::Value::NULLL),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",50,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",50,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",20,&NullValue),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",50,&NullValue),true);

  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",60,openfluid::core::VectorValue(3,1.1)),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",60,openfluid::core::Value::VECTOR),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",60,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",60,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",20,&VectValue),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",60,&VectValue),true);
  BOOST_REQUIRE_EQUAL(VectValue.size(),3);
  BOOST_REQUIRE_EQUAL(VectValue[0],1.1);
  BOOST_REQUIRE_EQUAL(VectValue[2],1.1);
  VectValue.clear();
//  VectValue = *Vars.getValue("bar",8);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",60,&VectValue),true);
  BOOST_REQUIRE_EQUAL(VectValue.size(),3);
  BOOST_REQUIRE_EQUAL(VectValue[0],1.1);
  BOOST_REQUIRE_EQUAL(VectValue[2],1.1);


  BOOST_REQUIRE_EQUAL(Vars.modifyValue("bar",15,IntValue),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("bar",20,StrValue),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("bar",30,BoolValue),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("bar",40,NullValue),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("bar",50,VectValue),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("bar",60,DblValue),true);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",15,openfluid::core::Value::INTEGER),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",20,openfluid::core::Value::INTEGER),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",15,&IntValue),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",20,&IntValue),false);
  IntValue.set(0);
//  IntValue = *Vars.getValue("bar",3);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",15,&IntValue),true);
  BOOST_REQUIRE_EQUAL(IntValue.get(),1004);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",20,openfluid::core::Value::STRING),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",30,openfluid::core::Value::STRING),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",20,&StrValue),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",30,&StrValue),false);
  StrValue.set("");
//  StrValue = *Vars.getValue("bar",4);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",20,&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue.get(),"1005");

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",30,openfluid::core::Value::BOOLEAN),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",20,openfluid::core::Value::BOOLEAN),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",30,&BoolValue),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",50,&BoolValue),false);
  BOOST_REQUIRE_EQUAL(BoolValue.get(),true);
  BOOST_REQUIRE_EQUAL((bool)BoolValue,true);
  BoolValue.set(false);
//  BoolValue = *Vars.getValue("bar",5);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",30,&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue.get(),true);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",40,openfluid::core::Value::NULLL),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",60,openfluid::core::Value::NULLL),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",40,&NullValue),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",50,&NullValue),false);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",50,openfluid::core::Value::VECTOR),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",30,openfluid::core::Value::VECTOR),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",50,&VectValue),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",15,&VectValue),false);
  VectValue.clear();
//  VectValue = *Vars.getValue("bar",7);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",50,&VectValue),true);
  BOOST_REQUIRE_EQUAL(VectValue.size(),3);
  BOOST_REQUIRE_EQUAL(VectValue[0],1.1);
  BOOST_REQUIRE_EQUAL(VectValue[2],1.1);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",60,openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",15,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",60,&DblValue),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",30,&DblValue),false);
  DblValue.set(0.0);
//  DblValue = *Vars.getValue("bar",8);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",60,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),1002.1,0.001);


  //TODO check other Compound Values

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo"),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar"),true);
}

BOOST_AUTO_TEST_CASE(check_typed_operations)
{
  openfluid::core::ValuesBufferProperties::setBufferSize(8);
  openfluid::core::Variables Vars;

  BOOST_REQUIRE_EQUAL(Vars.createVariable("aNone",openfluid::core::Value::NONE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aNone"),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aNone",openfluid::core::Value::NONE),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aNone",openfluid::core::Value::VECTOR),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aNone",0),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aNone",0,openfluid::core::Value::NONE),false);

  BOOST_REQUIRE_EQUAL(Vars.appendValue("aNone",71,openfluid::core::DoubleValue(0.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("aNone",72,openfluid::core::IntegerValue(0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("aNone",73,openfluid::core::VectorValue(5,0.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("aNone",74,openfluid::core::StringValue("strval")),true);

  BOOST_REQUIRE_EQUAL(Vars.modifyValue("aNone",71,openfluid::core::StringValue("strval")),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("aNone",74,openfluid::core::DoubleValue(0.0)),true);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aNone",71),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aNone",openfluid::core::Value::NONE),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aNone",openfluid::core::Value::STRING),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aNone",71,openfluid::core::Value::NONE),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aNone",71,openfluid::core::Value::STRING),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aNone",71,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aNone",71,openfluid::core::Value::STRING),true);


  BOOST_REQUIRE_EQUAL(Vars.createVariable("aDouble",openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aDouble"),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aDouble",71),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",71,openfluid::core::Value::DOUBLE),false);

  BOOST_REQUIRE_EQUAL(Vars.appendValue("aDouble",100,openfluid::core::DoubleValue(0.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("aDouble",101,openfluid::core::IntegerValue(0)),false);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("aDouble",102,openfluid::core::VectorValue(5,0.0)),false);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("aDouble",103,openfluid::core::StringValue("strval")),false);

  BOOST_REQUIRE_EQUAL(Vars.modifyValue("aDouble",100,openfluid::core::DoubleValue(0.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("aDouble",103,openfluid::core::StringValue("strval")),false);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aDouble",100),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",openfluid::core::Value::STRING),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",100,openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",100,openfluid::core::Value::STRING),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",100,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aDouble",100,openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aDouble",100,openfluid::core::Value::STRING),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aDouble",100,openfluid::core::Value::NONE),false);
}

// =====================================================================
// =====================================================================
