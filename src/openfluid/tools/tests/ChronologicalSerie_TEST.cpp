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
  @file ChronologicalSerie_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_chronologicalserie


#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/tools/ChronologicalSerie.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::tools::ChronologicalSerie CS;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{

  openfluid::tools::ChronItem_t B, A;

  openfluid::tools::ChronologicalSerie CS1;

  CS1.push_back(std::make_pair(openfluid::core::DateTime(2013,06,26,0,0,0),15.3));
  CS1.push_back(std::make_pair(openfluid::core::DateTime(2013,06,26,1,0,0),15.1));
  CS1.push_back(std::make_pair(openfluid::core::DateTime(2013,06,26,6,23,51),17.3));
  CS1.push_back(std::make_pair(openfluid::core::DateTime(2013,06,26,8,30,0),19.7));
  CS1.push_back(std::make_pair(openfluid::core::DateTime(2013,06,26,9,23,51),21.6));
  CS1.push_back(std::make_pair(openfluid::core::DateTime(2013,06,26,10,0,11),22.0));


  // somewhere in the serie
  BOOST_REQUIRE(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,26,1,30,25),B,A));
  BOOST_REQUIRE_EQUAL(B.first.getAsISOString(),openfluid::core::DateTime(2013,06,26,1,0,0).getAsISOString());
  BOOST_REQUIRE_EQUAL(A.first.getAsISOString(),openfluid::core::DateTime(2013,06,26,6,23,51).getAsISOString());


  // after serie
  BOOST_REQUIRE_EQUAL(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,26,21,30,25),B,A),false);


  // right on last
  BOOST_REQUIRE_EQUAL(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,26,10,0,11),B,A),true);
  BOOST_REQUIRE_EQUAL(B.first.getAsISOString(),A.first.getAsISOString());


  // right on first
  BOOST_REQUIRE_EQUAL(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,26,0,0,0),B,A),true);
  BOOST_REQUIRE_EQUAL(B.first.getAsISOString(),A.first.getAsISOString());


  // right on element
  BOOST_REQUIRE_EQUAL(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,26,8,30,0),B,A),true);
  BOOST_REQUIRE_EQUAL(B.first.getAsISOString(),A.first.getAsISOString());


  // somewhere in the serie
  BOOST_REQUIRE_EQUAL(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,26,8,30,1),B,A),true);
  BOOST_REQUIRE_EQUAL(B.first.getAsISOString(),openfluid::core::DateTime(2013,06,26,8,30,0).getAsISOString());
  BOOST_REQUIRE_EQUAL(A.first.getAsISOString(),openfluid::core::DateTime(2013,06,26,9,23,51).getAsISOString());


  // before serie
  BOOST_REQUIRE_EQUAL(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,25,21,30,25),B,A),false);


  // *****************

  CS1.reset();

  // before serie
  BOOST_REQUIRE_EQUAL(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,25,21,30,25),B,A),false);


  // right on element
  BOOST_REQUIRE_EQUAL(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,26,8,30,0),B,A),true);
  BOOST_REQUIRE_EQUAL(B.first.getAsISOString(),A.first.getAsISOString());


  // right on last
  BOOST_REQUIRE_EQUAL(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,26,10,0,11),B,A),true);
  BOOST_REQUIRE_EQUAL(B.first.getAsISOString(),A.first.getAsISOString());


  // after serie
  BOOST_REQUIRE_EQUAL(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,26,21,30,25),B,A),false);


  // somewhere in the serie
  BOOST_REQUIRE(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,26,1,30,25),B,A));
  BOOST_REQUIRE_EQUAL(B.first.getAsISOString(),openfluid::core::DateTime(2013,06,26,1,0,0).getAsISOString());
  BOOST_REQUIRE_EQUAL(A.first.getAsISOString(),openfluid::core::DateTime(2013,06,26,6,23,51).getAsISOString());


  // right on first
  BOOST_REQUIRE_EQUAL(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,26,0,0,0),B,A),true);
  BOOST_REQUIRE_EQUAL(B.first.getAsISOString(),A.first.getAsISOString());


  // somewhere in the serie
  BOOST_REQUIRE_EQUAL(CS1.getSurroundingValues(openfluid::core::DateTime(2013,06,26,8,30,1),B,A),true);
  BOOST_REQUIRE_EQUAL(B.first.getAsISOString(),openfluid::core::DateTime(2013,06,26,8,30,0).getAsISOString());
  BOOST_REQUIRE_EQUAL(A.first.getAsISOString(),openfluid::core::DateTime(2013,06,26,9,23,51).getAsISOString());
}

