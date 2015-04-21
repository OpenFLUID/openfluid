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
  @file RuntimeEnv_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_runtimeenv
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/base/RuntimeEnv.hpp>

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  BOOST_REQUIRE_NE(openfluid::base::RuntimeEnvironment::instance()->getInputDir(),"");
  BOOST_REQUIRE_NE(openfluid::base::RuntimeEnvironment::instance()->getOutputDir(),"");
  BOOST_REQUIRE_NE(openfluid::base::RuntimeEnvironment::instance()->getTempDir(),"");
  BOOST_REQUIRE_GT(openfluid::base::RuntimeEnvironment::instance()->getSimulatorsPluginsPaths().size(),0);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::instance()->isClearOutputDir(),false);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::instance()->isUserValuesBufferSize(),false);

  BOOST_REQUIRE(!openfluid::base::RuntimeEnvironment::instance()->getArch().empty());

  std::cout << "Arch: " << openfluid::base::RuntimeEnvironment::instance()->getArch() << std::endl;
  std::cout << "Hostname: " << openfluid::base::RuntimeEnvironment::instance()->getHostName() << std::endl;
  std::cout << "UserID: " << openfluid::base::RuntimeEnvironment::instance()->getUserID() << std::endl;
  std::cout << "Home dir: " << openfluid::base::RuntimeEnvironment::instance()->getUserHomeDir() << std::endl;
  std::cout << "User data path for foo: "
            << openfluid::base::RuntimeEnvironment::instance()->getUserDataPath("foo") << std::endl;
  std::cout << "Default config file: "
            << openfluid::base::RuntimeEnvironment::instance()->getDefaultConfigFile() << std::endl;
  std::cout << "Input dir: " << openfluid::base::RuntimeEnvironment::instance()->getInputDir() << std::endl;
  std::cout << "Output dir: " << openfluid::base::RuntimeEnvironment::instance()->getOutputDir() << std::endl;
  std::cout << "Temp dir: " << openfluid::base::RuntimeEnvironment::instance()->getTempDir() << std::endl;
  std::cout << "MarketBag dir: " << openfluid::base::RuntimeEnvironment::instance()->getMarketBagDir() << std::endl;
  std::cout << "User examples dir: "
            << openfluid::base::RuntimeEnvironment::instance()->getUserExamplesDir() << std::endl;
  std::cout << "Provided examples dir: "
            << openfluid::base::RuntimeEnvironment::instance()->getProvidedExamplesDir() << std::endl;
  for (unsigned int i=0;i<openfluid::base::RuntimeEnvironment::instance()->getSimulatorsPluginsPaths().size();i++)
    std::cout << "Simulators search dir #" << (i+1) << " "
              << openfluid::base::RuntimeEnvironment::instance()->getSimulatorsPluginsPaths().at(i) << std::endl;
  for (unsigned int i=0;i<openfluid::base::RuntimeEnvironment::instance()->getObserversPluginsPaths().size();i++)
    std::cout << "Observers search dir #" << (i+1) << " "
              << openfluid::base::RuntimeEnvironment::instance()->getObserversPluginsPaths().at(i) << std::endl;

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{

  openfluid::base::RuntimeEnvironment::instance()->setInputDir("/foo/bar");
  openfluid::base::RuntimeEnvironment::instance()->setOutputDir("/bar/foo/bar");
  openfluid::base::RuntimeEnvironment::instance()->setOutputDir("/bar/foo/bar");
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::instance()->getInputDir(),"/foo/bar");
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::instance()->getOutputDir(),"/bar/foo/bar");
  BOOST_REQUIRE_NE(openfluid::base::RuntimeEnvironment::instance()->getTempDir(),"");


  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::instance()->getExtraSimulatorsPluginsPaths().size(),0);
  openfluid::base::RuntimeEnvironment::instance()->addExtraSimulatorsPluginsPaths("/bar/foo/foo/bar");
  openfluid::base::RuntimeEnvironment::instance()->addExtraSimulatorsPluginsPaths("/bar/foo/foo/bar/bar/bar");
  BOOST_REQUIRE_GT(openfluid::base::RuntimeEnvironment::instance()->getSimulatorsPluginsPaths().size(),2);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::instance()->getExtraSimulatorsPluginsPaths().size(),2);

  openfluid::base::RuntimeEnvironment::instance()->resetExtraSimulatorsPluginsPaths();
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::instance()->getExtraSimulatorsPluginsPaths().size(),0);
  openfluid::base::RuntimeEnvironment::instance()->addExtraSimulatorsPluginsPaths("/bar/foo/foo/bar");
  openfluid::base::RuntimeEnvironment::instance()->addExtraSimulatorsPluginsPaths("/bar/foo/foo/bar/bar/bar");


  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::instance()->getExtraObserversPluginsPaths().size(),0);
  openfluid::base::RuntimeEnvironment::instance()->addExtraObserversPluginsPaths("/bar/foo/foo/bar");
  openfluid::base::RuntimeEnvironment::instance()->addExtraObserversPluginsPaths("/bar/foo/foo/bar/bar/bar");
  BOOST_REQUIRE_GT(openfluid::base::RuntimeEnvironment::instance()->getObserversPluginsPaths().size(),2);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::instance()->getExtraObserversPluginsPaths().size(),2);

  openfluid::base::RuntimeEnvironment::instance()->resetExtraObserversPluginsPaths();
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::instance()->getExtraObserversPluginsPaths().size(),0);
  openfluid::base::RuntimeEnvironment::instance()->addExtraObserversPluginsPaths("/bar/foo/foo/bar");
  openfluid::base::RuntimeEnvironment::instance()->addExtraObserversPluginsPaths("/bar/foo/foo/bar/bar/bar");



  openfluid::base::RuntimeEnvironment::instance()->setClearOutputDir(true);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::instance()->isClearOutputDir(),true);

  openfluid::base::RuntimeEnvironment::instance()->setValuesBufferSize(2345);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::instance()->isUserValuesBufferSize(),true);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::instance()->getValuesBufferSize(),2345);



  openfluid::base::EnvironmentProperties* SimEnv;
  bool BoolValue;
  std::string StrValue;

  SimEnv = openfluid::base::RuntimeEnvironment::instance()->wareEnvironment();
  BOOST_REQUIRE(SimEnv != NULL);

  BOOST_REQUIRE_EQUAL(SimEnv->getValue("wrong.fake",StrValue),false);
  BOOST_REQUIRE_EQUAL(SimEnv->getValue("fake.wrong",BoolValue),false);

  BOOST_REQUIRE_EQUAL(SimEnv->getValue("dir.input",StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,openfluid::base::RuntimeEnvironment::instance()->getInputDir());

  BOOST_REQUIRE_EQUAL(SimEnv->getValue("dir.output",StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,openfluid::base::RuntimeEnvironment::instance()->getOutputDir());

  BOOST_REQUIRE_EQUAL(SimEnv->getValue("dir.temp",StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,openfluid::base::RuntimeEnvironment::instance()->getTempDir());

  BOOST_REQUIRE_EQUAL(SimEnv->getValue("mode.clearoutputdir",BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,openfluid::base::RuntimeEnvironment::instance()->isClearOutputDir());

}

// =====================================================================
// =====================================================================
