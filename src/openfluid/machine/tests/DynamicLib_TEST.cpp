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
  @file DynamicLib_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_dynamiclib


#include <iostream>
#include <memory>

#include <boost/test/unit_test.hpp>

#include <openfluid/machine/DynamicLib.hpp>
#include <openfluid/config.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::machine::DynamicLib DLib("/path/to/dlib");

  BOOST_REQUIRE_EQUAL(DLib.isLoaded(),false);
  BOOST_REQUIRE_EQUAL(DLib.getPath(),"/path/to/dlib");
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_errors)
{
  openfluid::machine::DynamicLib DLib("/path/to/dlib.libext");

  BOOST_REQUIRE_EQUAL(DLib.load(),false);
  std::cout << "Error message: " << DLib.getLatestErrorMsg() << std::endl;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations_plugin1)
{
  typedef int AnswerFunction();

  std::string PluginPath = CONFIGTESTS_BINARY_DIR+"/dlibplugin1"+openfluid::config::PLUGINS_EXT;
  std::cout << PluginPath << std::endl;

  openfluid::machine::DynamicLib DLib(PluginPath);
  BOOST_REQUIRE_EQUAL(DLib.load(),true);
  BOOST_REQUIRE_EQUAL(DLib.isLoaded(),true);
  
  BOOST_REQUIRE_EQUAL(DLib.unload(),true);
  BOOST_REQUIRE_EQUAL(DLib.isLoaded(),false);
  BOOST_REQUIRE_EQUAL(DLib.load(),true);

  BOOST_REQUIRE_EQUAL(DLib.hasSymbol("theanswer"),true);
  BOOST_REQUIRE_EQUAL(DLib.hasSymbol("notexist"),false);
  BOOST_REQUIRE_EQUAL(DLib.hasSymbol("TheAnswer"),false);

  AnswerFunction* AF = DLib.getSymbol<AnswerFunction*>("theanswer");
  BOOST_REQUIRE_EQUAL(AF(),42);

  BOOST_REQUIRE_EQUAL(DLib.unload(),true);
  BOOST_REQUIRE_EQUAL(DLib.isLoaded(),false);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations_plugin2)
{
  typedef std::string* NameFunction();
  typedef int VersionFunction();

  std::string PluginPath = CONFIGTESTS_BINARY_DIR+"/dlibplugin2"+openfluid::config::PLUGINS_EXT;
  std::cout << PluginPath << std::endl;

  openfluid::machine::DynamicLib DLib(PluginPath);
  BOOST_REQUIRE_EQUAL(DLib.load(),true);
  BOOST_REQUIRE_EQUAL(DLib.isLoaded(),true);

  BOOST_REQUIRE_EQUAL(DLib.hasSymbol("getPluginVersion"),true);
  auto VF = DLib.getSymbol<VersionFunction*>("getPluginVersion");
  BOOST_REQUIRE_EQUAL(VF(),3);

  BOOST_REQUIRE_EQUAL(DLib.hasSymbol("getThePluggedClassName"),true);
  auto NF = DLib.getSymbol<NameFunction*>("getThePluggedClassName");
  std::unique_ptr<std::string> NameStr(NF());
  BOOST_REQUIRE_EQUAL(*NameStr,"ThePluggedClass");

  BOOST_REQUIRE_EQUAL(DLib.unload(),true);
  BOOST_REQUIRE_EQUAL(DLib.isLoaded(),false);
}