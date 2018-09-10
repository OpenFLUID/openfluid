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
  @file MapValue_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_mapvalue


#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/core/MapValue.hpp>


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
  BOOST_REQUIRE_EQUAL(Val1.at("bar").asVectorValue().getSize(),5);
  BOOST_REQUIRE_EQUAL(Val1.at("foofoobar").asBooleanValue().get(),true);
  BOOST_REQUIRE_EQUAL(Val1.at("foobar").asStringValue().get(),"you're talking to me?");

  Val1.setVectorValue("bar",openfluid::core::VectorValue(7,7.7));
  BOOST_REQUIRE_EQUAL(Val1.getVectorValue("bar").getSize(),7);
  std::cout << Val1 << std::endl;

  BOOST_REQUIRE_EQUAL(Val1.getKeys().size(),4);
  BOOST_REQUIRE_EQUAL(Val1.getKeys().at(0),"bar");
  BOOST_REQUIRE_EQUAL(Val1.getKeys().at(1),"foo");
  BOOST_REQUIRE_EQUAL(Val1.getKeys().at(2),"foobar");
  BOOST_REQUIRE_EQUAL(Val1.getKeys().at(3),"foofoobar");

  for (openfluid::core::MapValue::iterator it = Val1.begin(); it!=Val1.end(); ++it)
    std::cout << "using iterator, " << (*it).first << " = " << (*(*it).second) << std::endl;

  for (openfluid::core::MapValue::const_iterator it = Val1.begin(); it!=Val1.end(); ++it)
    std::cout << "using const iterator, " << (*it).first << " = " << (*(*it).second) << std::endl;


  Val1.remove("foobar");

  BOOST_REQUIRE_EQUAL(Val1.getKeys().size(),3);
  BOOST_REQUIRE_EQUAL(Val1.getKeys().at(0),"bar");
  BOOST_REQUIRE_EQUAL(Val1.getKeys().at(1),"foo");
  BOOST_REQUIRE_EQUAL(Val1.getKeys().at(2),"foofoobar");

  for (openfluid::core::MapValue::iterator it = Val1.begin(); it!=Val1.end(); ++it)
    std::cout << "using iterator, " << (*it).first << " = " << (*(*it).second) << std::endl;

  for (openfluid::core::MapValue::const_iterator it = Val1.begin(); it!=Val1.end(); ++it)
    std::cout << "using const iterator, " << (*it).first << " = " << (*(*it).second) << std::endl;

  BOOST_REQUIRE_EQUAL(Val1.getSize(),3);
  Val1.setInteger("bar",17);
  BOOST_REQUIRE_EQUAL(Val1["bar"].getType(),openfluid::core::Value::INTEGER);
  BOOST_REQUIRE_EQUAL(Val1["bar"].asIntegerValue().get(),17);
  std::cout << Val1 << std::endl;

  openfluid::core::MapValue Val2 = Val1;

  BOOST_REQUIRE_EQUAL(Val2.getSize(),3);
  BOOST_REQUIRE_CLOSE(Val2.getDouble("foo"),1.5,0.0001);
  BOOST_REQUIRE_EQUAL(Val2.at("bar").asIntegerValue().get(),17);
  BOOST_REQUIRE_EQUAL(Val2.at("foofoobar").asBooleanValue().get(),true);

  Val2.setMatrixValue("foofoobar",openfluid::core::MatrixValue(5,9,29.5));
  BOOST_REQUIRE_EQUAL(Val1["foofoobar"].getType(),openfluid::core::Value::BOOLEAN);
  BOOST_REQUIRE_EQUAL(Val2["foofoobar"].getType(),openfluid::core::Value::MATRIX);
  BOOST_REQUIRE_EQUAL(Val2["foofoobar"].asMatrixValue().getColsNbr(),5);
  BOOST_REQUIRE_EQUAL(Val2.getMatrixValue("foofoobar").getRowsNbr(),9);

  BOOST_REQUIRE_THROW(Val2.getMatrixValue("foo"),openfluid::base::FrameworkException);
  BOOST_REQUIRE_THROW(Val2.at("foo").asIntegerValue(),openfluid::base::FrameworkException);
  BOOST_REQUIRE_THROW(Val2.at("wrongfoo"),openfluid::base::FrameworkException);

  for (openfluid::core::MapValue::iterator it = Val1.begin(); it!=Val1.end(); ++it)
  {
    (*it).second.reset(new openfluid::core::DoubleValue(1.0));
    std::cout << "using iterator, " << (*it).first << " = " << (*(*it).second) << std::endl;
  }


}

