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
  @file IntegerValue_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_integervalue


#include <boost/test/unit_test.hpp>

#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/VectorValue.hpp>


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


BOOST_AUTO_TEST_CASE(check_convert)
{
  {
    openfluid::core::DoubleValue Val;
    BOOST_REQUIRE(openfluid::core::IntegerValue(1947).convert(Val));
    BOOST_REQUIRE_CLOSE(Val.get(),1947,0.000001);
  }

  {
    openfluid::core::VectorValue Val;
    BOOST_REQUIRE(openfluid::core::IntegerValue(19).convert(Val));
    BOOST_REQUIRE_EQUAL(Val.size(),1);
    BOOST_REQUIRE_CLOSE(Val.get(0),19,0.000001);
  }

  {
    openfluid::core::MatrixValue Val;
    BOOST_REQUIRE(openfluid::core::IntegerValue(47).convert(Val));
    BOOST_REQUIRE_EQUAL(Val.size(),1);
    BOOST_REQUIRE_EQUAL(Val.getColsNbr(),1);
    BOOST_REQUIRE_EQUAL(Val.getRowsNbr(),1);
    BOOST_REQUIRE_CLOSE(Val.get(0,0),47,0.000001);
  }
}

