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
  @file SimulatorEnv_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_simulatorenv
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/base/EnvProperties.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::EnvironmentProperties SimEnv;
  bool BoolValue;
  std::string StrValue;

  BOOST_REQUIRE_EQUAL(SimEnv.getValue("fakebool",BoolValue),false);
  BOOST_REQUIRE_EQUAL(SimEnv.getValue("fakestring",StrValue),false);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::base::EnvironmentProperties SimEnv;
  bool BoolValue;
  std::string StrValue;



  BOOST_REQUIRE_EQUAL(SimEnv.setValue("bool1",true),true);
  BOOST_REQUIRE_EQUAL(SimEnv.getValue("bool1",BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,true);

  BOOST_REQUIRE_EQUAL(SimEnv.setValue("bool1",false),true);
  BOOST_REQUIRE_EQUAL(SimEnv.getValue("bool1",BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,false);

  BOOST_REQUIRE_EQUAL(SimEnv.setValue("bool2",false),true);
  BOOST_REQUIRE_EQUAL(SimEnv.getValue("bool2",BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,false);

  BOOST_REQUIRE_EQUAL(SimEnv.getValue("fakebool",BoolValue),false);



  BOOST_REQUIRE_EQUAL(SimEnv.setValue("str1",std::string("first string")),true);
  BOOST_REQUIRE_EQUAL(SimEnv.getValue("str1",StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,"first string");

  BOOST_REQUIRE_EQUAL(SimEnv.setValue("str1",std::string("string one")),true);
  BOOST_REQUIRE_EQUAL(SimEnv.getValue("str1",StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,"string one");

  BOOST_REQUIRE_EQUAL(SimEnv.setValue("str2",std::string("string 2")),true);
  BOOST_REQUIRE_EQUAL(SimEnv.getValue("str2",StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,"string 2");

  BOOST_REQUIRE_EQUAL(SimEnv.getValue("fakestring",StrValue),false);

}

// =====================================================================
// =====================================================================
