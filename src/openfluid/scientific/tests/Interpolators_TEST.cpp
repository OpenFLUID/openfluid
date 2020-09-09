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
  @file Interpolators_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_interpolators


#include <iostream>

#include <boost/test/unit_test.hpp>

#include <openfluid/scientific/Interpolators.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  double Val = 0.0;


  // linear interpolation

  std::cout << "linear interpolation" << std::endl;

  Val = openfluid::scientific::linearInterpolation(1.0,1.0,2.0,2.0,1.5);
  std::cout << Val << std::endl;
  BOOST_REQUIRE_EQUAL(openfluid::scientific::isVeryClose(Val,1.5),true);


  Val = openfluid::scientific::linearInterpolation(1.0,1.0,2.0,-2.0,1.5);
  std::cout << Val << std::endl;
  BOOST_REQUIRE_EQUAL(openfluid::scientific::isVeryClose(Val,-0.5),true);


  Val = openfluid::scientific::linearInterpolation(1.0,1.0,3.0,3.0,15.3);
  std::cout << Val << std::endl;
  BOOST_REQUIRE_EQUAL(openfluid::scientific::isVeryClose(Val,15.3),true);


  // linear interpolation from origin (x1 = 0);

  std::cout << "linear interpolation from origin" << std::endl;

  double ValX0 = 0.0;
  ValX0 = openfluid::scientific::linearInterpolationFromXOrigin(5.0,5.0,0.0,2.5);
  std::cout << ValX0 << std::endl;
  Val = openfluid::scientific::linearInterpolation(0.0,5.0,5.0,0.0,2.5);
  std::cout << Val << std::endl;
  BOOST_REQUIRE_EQUAL(openfluid::scientific::isVeryClose(ValX0,Val),true);


  // linear interpolation with double

  double LVal;

  std::cout << "linear interpolation (double)" << std::endl;

  LVal = openfluid::scientific::linearInterpolation(1.0,1.0,2.0,2.0,1.5);
  std::cout << LVal << std::endl;
  BOOST_REQUIRE_EQUAL(openfluid::scientific::isVeryClose(LVal,1.5l),true);


  // linear interpolation with float

  float FVal;

  std::cout << "linear interpolation (float)" << std::endl;

  FVal = openfluid::scientific::linearInterpolation(1.0,1.0,2.0,2.0,1.5);
  std::cout << FVal << std::endl;
  BOOST_REQUIRE_EQUAL(openfluid::scientific::isVeryClose(FVal,1.5f),true);

}

