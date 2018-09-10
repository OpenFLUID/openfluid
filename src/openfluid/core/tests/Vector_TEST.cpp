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
  @file Vector_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_vector


#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <openfluid/core/Vector.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::Vector<double> V1,V4;
  openfluid::core::Vector<double> V2(5);
  openfluid::core::Vector<double> V3(50000,12.3);

  BOOST_REQUIRE_EQUAL(V1.size(),0);
  BOOST_REQUIRE_EQUAL(V2.size(),5);
  BOOST_REQUIRE_EQUAL(V3.size(),50000);

  BOOST_REQUIRE_CLOSE(V3.at(17564),12.3,0.0001);
  BOOST_REQUIRE_CLOSE(V3.at(0),12.3,0.0001);
  BOOST_REQUIRE_CLOSE(V3.at(49999),12.3,0.0001);

  V4 = openfluid::core::Vector<double>(V3);
  BOOST_REQUIRE_CLOSE(V4.at(17564),12.3,0.0001);
  BOOST_REQUIRE_CLOSE(V4.at(0),12.3,0.0001);
  BOOST_REQUIRE_CLOSE(V4.at(49999),12.3,0.0001);

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::Vector<double> V1(10,12.3);
  openfluid::core::Vector<double> V2,V3;

  BOOST_REQUIRE_EQUAL(V1.getSize(),10);

  for (openfluid::core::Vector<double>::iterator it = V1.begin(); it!=V1.end(); ++it)
      std::cout << "using iterator, " << (*it) << std::endl;

  V1[1] = -5.18;
  V1.setElement(3,12345.6789);
  BOOST_REQUIRE_CLOSE(V1[1],-5.18,0.0001);
  BOOST_REQUIRE_CLOSE(V1.at(3),12345.6789,0.0001);

  V2.setData(V1.data(),V1.getSize());
  BOOST_REQUIRE_EQUAL(V2.getSize(),10);
  BOOST_REQUIRE_CLOSE(V2.at(1),-5.18,0.0001);
  BOOST_REQUIRE_CLOSE(V2.getElement(3),12345.6789,0.0001);

  V3 = V2;
  BOOST_REQUIRE_EQUAL(V3.getSize(),10);
  BOOST_REQUIRE_CLOSE(V3.at(1),-5.18,0.0001);
  BOOST_REQUIRE_CLOSE(V3.getElement(3),12345.6789,0.0001);

  for (openfluid::core::Vector<double>::const_iterator it = V3.begin(); it!=V3.end(); ++it)
      std::cout << "using const iterator, " << (*it) << std::endl;

  V3.clear();
  BOOST_REQUIRE_EQUAL(V3.getSize(),0);
}

