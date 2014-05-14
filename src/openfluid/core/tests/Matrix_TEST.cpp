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
  \file Vector_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_matrix
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <openfluid/core/Matrix.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::Matrix<double> V1,V4;
  openfluid::core::Matrix<double> V2(5,5);
  openfluid::core::Matrix<double> V3(200,25,12.3);

  BOOST_REQUIRE_EQUAL(V1.size(),0);
  BOOST_REQUIRE_EQUAL(V2.size(),25);
  BOOST_REQUIRE_EQUAL(V3.size(),5000);

  BOOST_REQUIRE_CLOSE(V3.at(47,18),12.3,0.0001);
  BOOST_REQUIRE_CLOSE(V3.at(0,0),12.3,0.0001);
  BOOST_REQUIRE_CLOSE(V3.at(199,24),12.3,0.0001);

  V4 = openfluid::core::Matrix<double>(V3);
  BOOST_REQUIRE_EQUAL(V4.getColsNbr(),200);
  BOOST_REQUIRE_EQUAL(V4.getRowsNbr(),25);
  BOOST_REQUIRE_EQUAL(V4.size(),5000);
  BOOST_REQUIRE_CLOSE(V4.at(47,18),12.3,0.0001);
  BOOST_REQUIRE_CLOSE(V4.at(0,0),12.3,0.0001);
  BOOST_REQUIRE_CLOSE(V4.at(199,24),12.3,0.0001);


}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::Matrix<double> V1(10,10,12.3);
  openfluid::core::Matrix<double> V2,V3;

  BOOST_REQUIRE_EQUAL(V1.getColsNbr(),10);
  BOOST_REQUIRE_EQUAL(V1.getRowsNbr(),10);

  V1.set(1,1,-5.18);
  V1.setElement(3,3,12345.6789);
  BOOST_REQUIRE_CLOSE(V1.at(1,1),-5.18,0.0001);
  BOOST_REQUIRE_CLOSE(V1.at(3,3),12345.6789,0.0001);

  V3 = V1;
  BOOST_REQUIRE_EQUAL(V3.getColsNbr(),10);
  BOOST_REQUIRE_EQUAL(V3.getRowsNbr(),10);
  BOOST_REQUIRE_CLOSE(V3.at(1,1),-5.18,0.0001);
  BOOST_REQUIRE_CLOSE(V3.getElement(3,3),12345.6789,0.0001);

  V3.clear();
  BOOST_REQUIRE_EQUAL(V3.getSize(),0);

  BOOST_REQUIRE_EQUAL(V1.getColsNbr(),10);
  BOOST_REQUIRE_EQUAL(V1.getRowsNbr(),10);


}

// =====================================================================
// =====================================================================

