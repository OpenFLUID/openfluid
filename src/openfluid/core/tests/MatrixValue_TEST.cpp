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
  @file MatrixValue_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_matrixvalue
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/StringValue.hpp>


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

