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
  @file GDALHelpers_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_gedalHelpers


#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/utils/GDALHelpers.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_geometry_types)
{
  // wkbPoint
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRPunctualType(wkbPoint),true);
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRLinearType(wkbPoint),false);
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRSurfacicType(wkbPoint),false);

  // wkbLineString
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRPunctualType(wkbLineString),false);
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRLinearType(wkbLineString),true);
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRSurfacicType(wkbLineString),false);

  // wkbPolygon
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRPunctualType(wkbPolygon),false);
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRLinearType(wkbPolygon),false);
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRSurfacicType(wkbPolygon),true);

  // wkbMultiPoint
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRPunctualType(wkbMultiPoint),true);
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRLinearType(wkbMultiPoint),false);
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRSurfacicType(wkbMultiPoint),false);

  // wkbMultiLineString
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRPunctualType(wkbMultiLineString),false);
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRLinearType(wkbMultiLineString),true);
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRSurfacicType(wkbMultiLineString),false);

  // wkbMultiPolygon
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRPunctualType(wkbMultiPolygon),false);
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRLinearType(wkbMultiPolygon),false);
  BOOST_REQUIRE_EQUAL(openfluid::utils::isOGRSurfacicType(wkbMultiPolygon),true);

}

