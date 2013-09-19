/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file RuntimeEnv_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
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
  BOOST_REQUIRE_NE(openfluid::base::RuntimeEnvironment::getInstance()->getInputDir(),"");
  BOOST_REQUIRE_NE(openfluid::base::RuntimeEnvironment::getInstance()->getOutputDir(),"");
  BOOST_REQUIRE_NE(openfluid::base::RuntimeEnvironment::getInstance()->getTempDir(),"");
  BOOST_REQUIRE_GT(openfluid::base::RuntimeEnvironment::getInstance()->getSimulatorsPluginsPaths().size(),0);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->isClearOutputDir(),false);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->isWriteResults(),true);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->isWriteSimReport(),true);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->isUserValuesBufferSize(),false);

  std::cout << "Arch: " << openfluid::base::RuntimeEnvironment::getInstance()->getArch() << std::endl;
  std::cout << "Hostname: " << openfluid::base::RuntimeEnvironment::getInstance()->getHostName() << std::endl;
  std::cout << "UserID: " << openfluid::base::RuntimeEnvironment::getInstance()->getUserID() << std::endl;
  std::cout << "Home dir: " << openfluid::base::RuntimeEnvironment::getInstance()->getUserHomeDir() << std::endl;
  std::cout << "User data path for foo: " << openfluid::base::RuntimeEnvironment::getInstance()->getUserDataPath("foo") << std::endl;
  std::cout << "Default config file: " << openfluid::base::RuntimeEnvironment::getInstance()->getDefaultConfigFile() << std::endl;
  std::cout << "Input dir: " << openfluid::base::RuntimeEnvironment::getInstance()->getInputDir() << std::endl;
  std::cout << "Output dir: " << openfluid::base::RuntimeEnvironment::getInstance()->getOutputDir() << std::endl;
  std::cout << "Temp dir: " << openfluid::base::RuntimeEnvironment::getInstance()->getTempDir() << std::endl;
  std::cout << "MarketBag dir: " << openfluid::base::RuntimeEnvironment::getInstance()->getMarketBagDir() << std::endl;
  std::cout << "User examples dir: " << openfluid::base::RuntimeEnvironment::getInstance()->getUserExamplesDir() << std::endl;
  std::cout << "Provided examples dir: " << openfluid::base::RuntimeEnvironment::getInstance()->getProvidedExamplesDir() << std::endl;
  for (unsigned int i=0;i<openfluid::base::RuntimeEnvironment::getInstance()->getSimulatorsPluginsPaths().size();i++)
    std::cout << "Simulators search dir #" << (i+1) << " " << openfluid::base::RuntimeEnvironment::getInstance()->getSimulatorsPluginsPaths().at(i) << std::endl;
  for (unsigned int i=0;i<openfluid::base::RuntimeEnvironment::getInstance()->getObserversPluginsPaths().size();i++)
    std::cout << "Observers search dir #" << (i+1) << " " << openfluid::base::RuntimeEnvironment::getInstance()->getObserversPluginsPaths().at(i) << std::endl;

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{

  openfluid::base::RuntimeEnvironment::getInstance()->setInputDir("/foo/bar");
  openfluid::base::RuntimeEnvironment::getInstance()->setOutputDir("/bar/foo/bar");
  openfluid::base::RuntimeEnvironment::getInstance()->setOutputDir("/bar/foo/bar");
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->getInputDir(),"/foo/bar");
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->getOutputDir(),"/bar/foo/bar");
  BOOST_REQUIRE_NE(openfluid::base::RuntimeEnvironment::getInstance()->getTempDir(),"");


  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->getExtraSimulatorsPluginsPaths().size(),0);
  openfluid::base::RuntimeEnvironment::getInstance()->addExtraSimulatorsPluginsPaths("/bar/foo/foo/bar");
  openfluid::base::RuntimeEnvironment::getInstance()->addExtraSimulatorsPluginsPaths("/bar/foo/foo/bar/bar/bar");
  BOOST_REQUIRE_GT(openfluid::base::RuntimeEnvironment::getInstance()->getSimulatorsPluginsPaths().size(),2);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->getExtraSimulatorsPluginsPaths().size(),2);

  openfluid::base::RuntimeEnvironment::getInstance()->resetExtraSimulatorsPluginsPaths();
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->getExtraSimulatorsPluginsPaths().size(),0);
  openfluid::base::RuntimeEnvironment::getInstance()->addExtraSimulatorsPluginsPaths("/bar/foo/foo/bar");
  openfluid::base::RuntimeEnvironment::getInstance()->addExtraSimulatorsPluginsPaths("/bar/foo/foo/bar/bar/bar");


  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->getExtraObserversPluginsPaths().size(),0);
  openfluid::base::RuntimeEnvironment::getInstance()->addExtraObserversPluginsPaths("/bar/foo/foo/bar");
  openfluid::base::RuntimeEnvironment::getInstance()->addExtraObserversPluginsPaths("/bar/foo/foo/bar/bar/bar");
  BOOST_REQUIRE_GT(openfluid::base::RuntimeEnvironment::getInstance()->getObserversPluginsPaths().size(),2);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->getExtraObserversPluginsPaths().size(),2);

  openfluid::base::RuntimeEnvironment::getInstance()->resetExtraObserversPluginsPaths();
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->getExtraObserversPluginsPaths().size(),0);
  openfluid::base::RuntimeEnvironment::getInstance()->addExtraObserversPluginsPaths("/bar/foo/foo/bar");
  openfluid::base::RuntimeEnvironment::getInstance()->addExtraObserversPluginsPaths("/bar/foo/foo/bar/bar/bar");



  openfluid::base::RuntimeEnvironment::getInstance()->setClearOutputDir(true);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->isClearOutputDir(),true);

  openfluid::base::RuntimeEnvironment::getInstance()->setWriteResults(false);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->isWriteResults(),false);

  openfluid::base::RuntimeEnvironment::getInstance()->setWriteSimReport(false);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->isWriteSimReport(),false);

  openfluid::base::RuntimeEnvironment::getInstance()->setValuesBufferSize(2345);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->isUserValuesBufferSize(),true);
  BOOST_REQUIRE_EQUAL(openfluid::base::RuntimeEnvironment::getInstance()->getValuesBufferSize(),2345);



  openfluid::base::EnvironmentProperties* SimEnv;
  bool BoolValue;
  std::string StrValue;

  SimEnv = openfluid::base::RuntimeEnvironment::getInstance()->getWareEnvironment();
  BOOST_REQUIRE(SimEnv != NULL);

  BOOST_REQUIRE_EQUAL(SimEnv->getValue("wrong.fake",StrValue),false);
  BOOST_REQUIRE_EQUAL(SimEnv->getValue("fake.wrong",BoolValue),false);

  BOOST_REQUIRE_EQUAL(SimEnv->getValue("dir.input",StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,openfluid::base::RuntimeEnvironment::getInstance()->getInputDir());

  BOOST_REQUIRE_EQUAL(SimEnv->getValue("dir.output",StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,openfluid::base::RuntimeEnvironment::getInstance()->getOutputDir());

  BOOST_REQUIRE_EQUAL(SimEnv->getValue("dir.temp",StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,openfluid::base::RuntimeEnvironment::getInstance()->getTempDir());

  BOOST_REQUIRE_EQUAL(SimEnv->getValue("mode.clearoutputdir",BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,openfluid::base::RuntimeEnvironment::getInstance()->isClearOutputDir());

  BOOST_REQUIRE_EQUAL(SimEnv->getValue("mode.saveresults",BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,openfluid::base::RuntimeEnvironment::getInstance()->isWriteResults());

  BOOST_REQUIRE_EQUAL(SimEnv->getValue("mode.writereport",BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,openfluid::base::RuntimeEnvironment::getInstance()->isWriteSimReport());

}

// =====================================================================
// =====================================================================
