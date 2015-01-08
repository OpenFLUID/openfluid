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
  @file VectorValue_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_vectorvalue
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/core/VectorValue.hpp>


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



