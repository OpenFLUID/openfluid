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
  @file Tree_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_tree
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <openfluid/core/Tree.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::Tree<std::string,double> TreeStrDouble;
  openfluid::core::Tree<int,int> TreeIntInt;

  BOOST_REQUIRE_EQUAL(TreeStrDouble.size(),1);
  BOOST_REQUIRE_EQUAL(TreeIntInt.size(),1);
  BOOST_REQUIRE(!TreeIntInt.hasValue());
  BOOST_REQUIRE_EQUAL(TreeIntInt.getChildValue(19,42),42);
  BOOST_REQUIRE_THROW(TreeIntInt.getValue(),openfluid::base::FrameworkException);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::Tree<int,int> TreeIntInt;

  TreeIntInt.addChild(2,20);
  BOOST_REQUIRE_EQUAL(TreeIntInt.size(),2);
  TreeIntInt.addChild(3).addChild(300,3000);
  BOOST_REQUIRE_EQUAL(TreeIntInt.size(),4);

  BOOST_REQUIRE(TreeIntInt.child(3).child(300).hasValue());
  BOOST_REQUIRE_EQUAL(TreeIntInt.child(3).child(300).getValue(),3000);


  BOOST_CHECK_THROW(TreeIntInt.child(9999),openfluid::base::FrameworkException);


  TreeIntInt.child(3).addChild(400,4000);
  TreeIntInt.child(3).addChild(500,5000);
  TreeIntInt.child(3).addChild(600,6000);
  TreeIntInt.child(3).addChild(700,7000);
  BOOST_CHECK_THROW(TreeIntInt.child(3).getValue(),openfluid::base::FrameworkException);
  BOOST_CHECK_EQUAL(TreeIntInt.child(3).getValue(18),18);
  BOOST_CHECK_THROW(TreeIntInt.child(3).addChild(700,7000),openfluid::base::FrameworkException);

  BOOST_CHECK_EQUAL(TreeIntInt.child(3).getChildValue(600,0),6000);
  BOOST_CHECK_EQUAL(TreeIntInt.child(3).getChildValue(900,9000),9000);

  BOOST_REQUIRE_EQUAL(TreeIntInt.size(),8);

  int Counter = 0;
  for (auto& Node : TreeIntInt.child(3))
  {
    std::cout << Node.first << " -> " << Node.second.getValue() << std::endl;
    BOOST_REQUIRE_EQUAL((Node.first*10),Node.second.getValue());
    Counter++;
  }
  BOOST_REQUIRE_EQUAL(Counter,5);
  BOOST_REQUIRE_EQUAL(Counter,TreeIntInt.child(3).children().size());


  TreeIntInt.writeToStreamHierarchically(std::cout);
  std::cout << std::endl;
  TreeIntInt.writeToStreamFlat(std::cout);
  std::cout << std::endl;

  TreeIntInt.clear();
  BOOST_REQUIRE_EQUAL(TreeIntInt.size(),1);
  BOOST_REQUIRE_EQUAL(TreeIntInt.getValue(21),21);

  BOOST_CHECK_THROW(TreeIntInt.child(9999),openfluid::base::FrameworkException);

  TreeIntInt.addChild(4).addChild(40,4000);
  TreeIntInt.deleteChild(3);
  BOOST_CHECK_THROW(TreeIntInt.child(3),openfluid::base::FrameworkException);
  BOOST_REQUIRE_EQUAL(TreeIntInt.size(),3);
}


// =====================================================================
// =====================================================================

