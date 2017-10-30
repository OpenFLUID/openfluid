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
  @file ColumnTextParser_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_coltextparser
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/tools/ColumnTextParser.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/MapValue.hpp>

#include <tests-config.hpp>


// =====================================================================
// =====================================================================


void checkParsing(const openfluid::tools::ColumnTextParser& Parser)
{
  BOOST_REQUIRE_EQUAL(Parser.getValue(0,0),"1");

  openfluid::core::DoubleValue DblVal;
  BOOST_REQUIRE(openfluid::core::StringValue(Parser.getValue(0,1)).toDoubleValue(DblVal));
  BOOST_REQUIRE_CLOSE(DblVal.get(),12.3,0.00001);

  BOOST_REQUIRE_EQUAL(openfluid::core::StringValue(Parser.getValue(0,3)).get(),"so far, so long");

  openfluid::core::BooleanValue BoolVal;
  BOOST_REQUIRE(openfluid::core::StringValue(Parser.getValue(1,1)).toBooleanValue(BoolVal));
  BOOST_REQUIRE(BoolVal);

  BOOST_REQUIRE(openfluid::core::StringValue(Parser.getValue(1,2)).toBooleanValue(BoolVal));
  BOOST_REQUIRE(!BoolVal);

  openfluid::core::MapValue MapVal;
  BOOST_REQUIRE(openfluid::core::StringValue(Parser.getValue(1,3)).toMapValue(MapVal));
  BOOST_REQUIRE_CLOSE(MapVal.getDouble("k1"),18.3,0.00001);
  BOOST_REQUIRE_EQUAL(MapVal.getString("k2"),"yes we can!");
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_from_string)
{
  openfluid::tools::ColumnTextParser Parser("#");

  Parser.setFromString(
    "1 12.3\t 15.3 \"so far, so long\"\n"
    "2 true false {\\\"k1\\\":18.3,\\\"k2\\\":\\\"yes\\ we\\ can!\\\"}\n"
    "3 1 2 [1.5,2,3.6]\n\n\n\n\n  \t"
    "4 0 0 [[0,0],[3.6,6.3],[1,1]]",
    4
  );

  Parser.streamContents(std::cout);

  checkParsing(Parser);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_from_file)
{
  openfluid::tools::ColumnTextParser Parser("#");

  Parser.loadFromFile(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ColumnTextParser/test01.txt");

  Parser.streamContents(std::cout);

  checkParsing(Parser);
}
