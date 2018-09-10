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
  @file TreeValue_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_treevalue


#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/core/TreeValue.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_tree)
{
  std::cout << "======== check_tree ========" << std::endl;

  openfluid::core::TreeValue Val1;


  BOOST_REQUIRE_EQUAL(Val1.size(),1);


  BOOST_REQUIRE_EQUAL(Val1.isSimple(),false);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),true);


  Val1.addChild("i1").addChild("i2").addChild("i3",3.0);
  BOOST_REQUIRE_EQUAL(Val1.size(),4);


  Val1.addChild("j1").addChild("j2").addChild("j3",33.0);
  Val1.child("j1").child("j2").addChild("j33",66.0);
  BOOST_REQUIRE_EQUAL(Val1.size(),8);

  BOOST_REQUIRE_CLOSE(Val1.child("j1").child("j2").getChildValue("j33",0.0),66,000.1);

  Val1.writeToStream(std::cout);
  std::cout << std::endl;
}

