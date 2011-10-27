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
  \file Variable_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_variable
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <openfluid/core.hpp>
#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/BooleanValue.hpp>


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
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::ValuesBufferProperties::setBufferSize(8);
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

  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",openfluid::core::DoubleValue(0.0)),false);

  BOOST_REQUIRE_EQUAL(Vars.createVariable("foo"),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo"),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("foo",openfluid::core::Value::NONE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",0),false);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",openfluid::core::DoubleValue(0.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",openfluid::core::DoubleValue(1.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",openfluid::core::DoubleValue(2.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",openfluid::core::DoubleValue(3.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",openfluid::core::DoubleValue(4.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",0),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",4),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("foo",4,openfluid::core::Value::NONE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",5),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("foo",5,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",0,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",0,openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",0,openfluid::core::Value::INTEGER),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",4,openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",4,openfluid::core::Value::INTEGER),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",5,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",5,openfluid::core::Value::INTEGER),false);
  BOOST_REQUIRE_EQUAL(Vars.getVariableValuesCount("foo"),5);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",0,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),0.0,0.001);
  BOOST_REQUIRE_CLOSE((double)DblValue,0.0,0.001);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",0,&IntValue),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",3,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),3.0,0.001);
  BOOST_REQUIRE_CLOSE((double)DblValue,3.0,0.001);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",3,&IntValue),false);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("foo",3,openfluid::core::DoubleValue(34.5)),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",3,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),34.5,0.001);
  BOOST_REQUIRE_CLOSE((double)DblValue,34.5,0.001);

  BOOST_REQUIRE_EQUAL(Vars.createVariable("bar"),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",openfluid::core::DoubleValue(1000.1)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",openfluid::core::DoubleValue(1001.1)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",openfluid::core::DoubleValue(1002.1)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",openfluid::core::DoubleValue(1003.1)),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",0,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),1000.1,0.001);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",2,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),1002.1,0.001);
  DblValue.set(0.0);
//  DblValue = *Vars.getValue("bar",2);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",2,&DblValue),true);
  BOOST_REQUIRE_CLOSE(DblValue.get(),1002.1,0.001);
//  BOOST_REQUIRE(!Vars.getValue("bar",4));
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",4,&DblValue),false);

  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",openfluid::core::IntegerValue(1004)),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",4,openfluid::core::Value::INTEGER),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",4,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",4,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",2,&IntValue),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",4,&IntValue),true);
  BOOST_REQUIRE_EQUAL(IntValue.get(),1004);
  BOOST_REQUIRE_EQUAL((long)IntValue,1004);
  BOOST_REQUIRE_EQUAL((int)IntValue,1004);
  IntValue.set(0);
//  IntValue = *Vars.getValue("bar",4);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",4,&IntValue),true);
  BOOST_REQUIRE_EQUAL(IntValue.get(),1004);

  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",openfluid::core::StringValue("1005")),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",5,openfluid::core::Value::STRING),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",5,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",5,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",2,&StrValue),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",5,&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue.get(),"1005");
  BOOST_REQUIRE_EQUAL((std::string)StrValue,"1005");
  StrValue.set("");
//  StrValue = *Vars.getValue("bar",5);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",5,&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue.get(),"1005");

  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",openfluid::core::BooleanValue(true)),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",6,openfluid::core::Value::BOOLEAN),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",6,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",6,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",2,&BoolValue),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",6,&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue.get(),true);
  BOOST_REQUIRE_EQUAL((bool)BoolValue,true);
  BoolValue.set(false);
//  BoolValue = *Vars.getValue("bar",6);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",6,&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue.get(),true);

  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",openfluid::core::NullValue()),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",7,openfluid::core::Value::NULLL),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",7,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",7,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",2,&NullValue),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",7,&NullValue),true);

  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",openfluid::core::VectorValue(3,1.1)),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",8,openfluid::core::Value::VECTOR),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",8,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",8,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",2,&VectValue),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",8,&VectValue),true);
  BOOST_REQUIRE_EQUAL(VectValue.size(),3);
  BOOST_REQUIRE_EQUAL(VectValue[0],1.1);
  BOOST_REQUIRE_EQUAL(VectValue[2],1.1);
  VectValue.clear();
//  VectValue = *Vars.getValue("bar",8);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",8,&VectValue),true);
  BOOST_REQUIRE_EQUAL(VectValue.size(),3);
  BOOST_REQUIRE_EQUAL(VectValue[0],1.1);
  BOOST_REQUIRE_EQUAL(VectValue[2],1.1);


  BOOST_REQUIRE_EQUAL(Vars.modifyValue("bar",3,IntValue),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("bar",4,StrValue),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("bar",5,BoolValue),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("bar",6,NullValue),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("bar",7,VectValue),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("bar",8,DblValue),true);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",3,openfluid::core::Value::INTEGER),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",4,openfluid::core::Value::INTEGER),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",3,&IntValue),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",4,&IntValue),false);
  IntValue.set(0);
//  IntValue = *Vars.getValue("bar",3);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",3,&IntValue),true);
  BOOST_REQUIRE_EQUAL(IntValue.get(),1004);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",4,openfluid::core::Value::STRING),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",5,openfluid::core::Value::STRING),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",4,&StrValue),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",2,&StrValue),false);
  StrValue.set("");
//  StrValue = *Vars.getValue("bar",4);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",4,&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue.get(),"1005");

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",5,openfluid::core::Value::BOOLEAN),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",6,openfluid::core::Value::BOOLEAN),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",5,&BoolValue),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",6,&BoolValue),false);
  BOOST_REQUIRE_EQUAL(BoolValue.get(),true);
  BOOST_REQUIRE_EQUAL((bool)BoolValue,true);
  BoolValue.set(false);
//  BoolValue = *Vars.getValue("bar",5);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",5,&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue.get(),true);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",6,openfluid::core::Value::NULLL),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",7,openfluid::core::Value::NULLL),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",6,&NullValue),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",7,&NullValue),false);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",7,openfluid::core::Value::VECTOR),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",8,openfluid::core::Value::VECTOR),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",7,&VectValue),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",8,&VectValue),false);
  VectValue.clear();
//  VectValue = *Vars.getValue("bar",7);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",7,&VectValue),true);
  BOOST_REQUIRE_EQUAL(VectValue.size(),3);
  BOOST_REQUIRE_EQUAL(VectValue[0],1.1);
  BOOST_REQUIRE_EQUAL(VectValue[2],1.1);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",8,openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar",3,openfluid::core::Value::DOUBLE),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",8,&DblValue),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",3,&DblValue),false);
  DblValue.set(0.0);
//  DblValue = *Vars.getValue("bar",8);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",8,&DblValue),true);
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

  BOOST_REQUIRE_EQUAL(Vars.appendValue("aNone",openfluid::core::DoubleValue(0.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("aNone",openfluid::core::IntegerValue(0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("aNone",openfluid::core::VectorValue(5,0.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("aNone",openfluid::core::StringValue("strval")),true);

  BOOST_REQUIRE_EQUAL(Vars.modifyValue("aNone",0,openfluid::core::StringValue("strval")),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("aNone",3,openfluid::core::DoubleValue(0.0)),true);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aNone",0),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aNone",openfluid::core::Value::NONE),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aNone",openfluid::core::Value::STRING),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aNone",0,openfluid::core::Value::NONE),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aNone",0,openfluid::core::Value::STRING),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aNone",0,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aNone",0,openfluid::core::Value::STRING),true);


  BOOST_REQUIRE_EQUAL(Vars.createVariable("aDouble",openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aDouble"),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aDouble",0),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",0,openfluid::core::Value::DOUBLE),false);

  BOOST_REQUIRE_EQUAL(Vars.appendValue("aDouble",openfluid::core::DoubleValue(0.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("aDouble",openfluid::core::IntegerValue(0)),false);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("aDouble",openfluid::core::VectorValue(5,0.0)),false);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("aDouble",openfluid::core::StringValue("strval")),false);

  BOOST_REQUIRE_EQUAL(Vars.modifyValue("aDouble",0,openfluid::core::DoubleValue(0.0)),true);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("aDouble",3,openfluid::core::StringValue("strval")),false);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aDouble",0),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",openfluid::core::Value::STRING),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",0,openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",0,openfluid::core::Value::STRING),false);
  BOOST_REQUIRE_EQUAL(Vars.isTypedVariableExist("aDouble",0,openfluid::core::Value::NONE),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aDouble",0,openfluid::core::Value::DOUBLE),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aDouble",0,openfluid::core::Value::STRING),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("aDouble",0,openfluid::core::Value::NONE),false);
}

// =====================================================================
// =====================================================================
