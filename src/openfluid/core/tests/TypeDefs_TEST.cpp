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
  \file TypeDefs_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_typedefs
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/DoubleValue.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::IDMap<bool>::Type IMbool;
  openfluid::core::IDMap<std::vector<double> >::Type IMvector;

  openfluid::core::IDMap<openfluid::core::DoubleValue>::Type IMDoubleValue;
  openfluid::core::IDMap<openfluid::core::MatrixValue>::Type IMMatrixValue;

  BOOST_REQUIRE_EQUAL(IMDoubleValue.size(),0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operationss)
{
  openfluid::core::IDMap<bool>::Type IMbool;

  openfluid::core::IDMap<openfluid::core::DoubleValue>::Type IMDoubleValue;
  openfluid::core::IDMap<openfluid::core::MatrixValue>::Type IMMatrixValue;


  IMbool[13] = false;
  IMbool[1] = false;
  IMbool[57] = true;
  BOOST_REQUIRE_EQUAL(IMbool.size(),3);
  BOOST_REQUIRE_EQUAL(IMbool[13],false);
  BOOST_REQUIRE_EQUAL(IMbool[57],true);
  IMbool[13] = true;
  BOOST_REQUIRE_EQUAL(IMbool[13],true);


  IMDoubleValue[33] = 0.0;
  IMDoubleValue[35] = 3.9;
  IMDoubleValue[3] = 9.81;
  IMDoubleValue[303] = 81.6561;
  BOOST_REQUIRE_EQUAL(IMDoubleValue.size(),4);
  BOOST_REQUIRE_CLOSE(IMDoubleValue[33].get(),0.0,0.000001);
  BOOST_REQUIRE_CLOSE(IMDoubleValue[303].get(),81.6561,0.000001);


  IMMatrixValue[17] = openfluid::core::MatrixValue(47,33,17.0);
  IMMatrixValue[15] = openfluid::core::MatrixValue(15,15,15.0);
  BOOST_REQUIRE_EQUAL(IMMatrixValue.size(),2);
  BOOST_REQUIRE_CLOSE(IMMatrixValue[17].get(1,1),17.0,0.000001);
  BOOST_REQUIRE_CLOSE(IMMatrixValue[17].get(46,32),17.0,0.000001);
  BOOST_REQUIRE_EQUAL(IMMatrixValue[17].getColsNbr(),47);
  BOOST_REQUIRE_EQUAL(IMMatrixValue[17].getRowsNbr(),33);
  IMMatrixValue[17].set(1,1,0.0);
  BOOST_REQUIRE_CLOSE(IMMatrixValue[17].get(1,1),0.0,0.000001);
  IMMatrixValue[17] = openfluid::core::MatrixValue(2,2,2.0);
  BOOST_REQUIRE_EQUAL(IMMatrixValue[17].getColsNbr(),2);
  BOOST_REQUIRE_EQUAL(IMMatrixValue[17].getRowsNbr(),2);
  BOOST_REQUIRE_EQUAL(IMMatrixValue[17].getSize(),4);
}

// =====================================================================
// =====================================================================


