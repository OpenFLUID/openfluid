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
  @file Environment_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_environment


#include <iostream>

#include <boost/test/unit_test.hpp>

#include <openfluid/base/Environment.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_init)
{
  openfluid::base::Environment::init();

  std::cout << "SystemArch: " << openfluid::base::Environment::getSystemArch() << std::endl;
  std::cout << "Hostname: " << openfluid::base::Environment::getHostName() << std::endl;
  std::cout << "Username: " << openfluid::base::Environment::getUserName() << std::endl;
  std::cout << "OpenFLUID version: " << openfluid::base::Environment::getVersion() << std::endl;
  std::cout << "OpenFLUID version full: " << openfluid::base::Environment::getVersionFull() << std::endl;
  std::cout << "OpenFLUID version M.m: " << openfluid::base::Environment::getVersionMajorMinor() << std::endl;
  std::cout << "Install prefix: " << openfluid::base::Environment::getInstallPrefix() << std::endl;
  std::cout << "Home dir: " << openfluid::base::Environment::getUserHomeDir() << std::endl;
  std::cout << "Temp dir: " << openfluid::base::Environment::getTempDir() << std::endl;
  std::cout << "User data dir: " << openfluid::base::Environment::getUserDataDir() << std::endl;  
  std::cout << "User examples dir: " << openfluid::base::Environment::getUserExamplesDir() << std::endl;
  std::cout << "Provided examples dir: " << openfluid::base::Environment::getProvidedExamplesDir() << std::endl;
  for (auto  TmpDir : openfluid::base::Environment::getSimulatorsDirs())
  {
    std::cout << "Simulators search dir: " << TmpDir << std::endl;
  }
  for (auto  TmpDir : openfluid::base::Environment::getObserversDirs())
  {
    std::cout << "Observers search dir: " << TmpDir << std::endl;
  }
  for (auto  TmpDir : openfluid::base::Environment::getBuilderextsDirs())
  {
    std::cout << "Builderexts search dir: " << TmpDir << std::endl;
  }
  std::cout << "Translations dir: " << openfluid::base::Environment::getTranslationsDir() << std::endl;
  std::cout << "Common resources dir: " << openfluid::base::Environment::getCommonResourcesDir() << std::endl;
  std::cout << "App resources dir (appname = foo): " <<
               openfluid::base::Environment::getAppResourcesDir("foo") << std::endl;
  std::cout << "Default config file: " << openfluid::base::Environment::getSettingsFile() << std::endl;
  std::cout << "Ideal thread count: " << openfluid::base::Environment::getIdealThreadCount() << std::endl;

  BOOST_REQUIRE(!openfluid::base::Environment::getSystemArch().empty());
  BOOST_REQUIRE(!openfluid::base::Environment::getInstallPrefix().empty());
  BOOST_REQUIRE(!openfluid::base::Environment::getUserHomeDir().empty());
  BOOST_REQUIRE(!openfluid::base::Environment::getUserDataDir().empty());
  BOOST_REQUIRE_GT(openfluid::base::Environment::getSettingsFile().size(),0);

  BOOST_REQUIRE(!openfluid::base::Environment::getVersion().empty());
  BOOST_REQUIRE(!openfluid::base::Environment::getVersionFull().empty());
  BOOST_REQUIRE(!openfluid::base::Environment::getVersionMajorMinor().empty());

  BOOST_REQUIRE_GT(openfluid::base::Environment::getSimulatorsDirs().size(),0);
  BOOST_REQUIRE_GT(openfluid::base::Environment::getObserversDirs().size(),0);
  BOOST_REQUIRE_GT(openfluid::base::Environment::getBuilderextsDirs().size(),0);

  BOOST_REQUIRE(!openfluid::base::Environment::getTranslationsDir().empty());
  BOOST_REQUIRE(!openfluid::base::Environment::getCommonResourcesDir().empty());
  BOOST_REQUIRE(!openfluid::base::Environment::getAppResourcesDir("foo").empty());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::base::Environment::init();

  BOOST_REQUIRE_EQUAL(openfluid::base::Environment::getExtraSimulatorsDirs().size(),0);
  openfluid::base::Environment::addExtraSimulatorsDirs("/bar/foo/foo/bar");
  openfluid::base::Environment::addExtraSimulatorsDirs("/bar/foo/foo/bar/bar/bar");
  BOOST_REQUIRE_GT(openfluid::base::Environment::getSimulatorsDirs().size(),2);
  BOOST_REQUIRE_EQUAL(openfluid::base::Environment::getExtraSimulatorsDirs().size(),2);
  openfluid::base::Environment::resetExtraSimulatorsDirs();
  BOOST_REQUIRE_EQUAL(openfluid::base::Environment::getExtraSimulatorsDirs().size(),0);
  openfluid::base::Environment::addExtraSimulatorsDirs("/bar/foo/foo/bar");
  BOOST_REQUIRE_EQUAL(openfluid::base::Environment::getExtraSimulatorsDirs().size(),1);

  BOOST_REQUIRE_EQUAL(openfluid::base::Environment::getExtraObserversDirs().size(),0);
  openfluid::base::Environment::addExtraObserversDirs("/bar/foo/foo/bar");
  openfluid::base::Environment::addExtraObserversDirs("/bar/foo/foo/bar/bar/bar");
  BOOST_REQUIRE_GT(openfluid::base::Environment::getObserversDirs().size(),2);
  BOOST_REQUIRE_EQUAL(openfluid::base::Environment::getExtraObserversDirs().size(),2);
  openfluid::base::Environment::resetExtraObserversDirs();
  BOOST_REQUIRE_EQUAL(openfluid::base::Environment::getExtraObserversDirs().size(),0);
  openfluid::base::Environment::addExtraObserversDirs("/bar/foo/foo/bar/bar/bar");
  BOOST_REQUIRE_EQUAL(openfluid::base::Environment::getExtraObserversDirs().size(),1);

  BOOST_REQUIRE_EQUAL(openfluid::base::Environment::getExtraBuilderextsDirs().size(),0);
  openfluid::base::Environment::addExtraBuilderextsDirs("/bar/foo/foo/bar");
  openfluid::base::Environment::addExtraBuilderextsDirs("/bar/foo/foo/bar/bar/bar");
  BOOST_REQUIRE_GT(openfluid::base::Environment::getBuilderextsDirs().size(),2);
  BOOST_REQUIRE_EQUAL(openfluid::base::Environment::getExtraBuilderextsDirs().size(),2);
  openfluid::base::Environment::resetExtraBuilderextsDirs();
  BOOST_REQUIRE_EQUAL(openfluid::base::Environment::getExtraBuilderextsDirs().size(),0);
  openfluid::base::Environment::addExtraBuilderextsDirs("/bar/foo/foo/bar/bar/bar");
  BOOST_REQUIRE_EQUAL(openfluid::base::Environment::getExtraBuilderextsDirs().size(),1);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_global_versiondef)
{
  std::cout << "global, defined numeric version: " << OPENFLUID_VERSION_NUMERIC << std::endl;
  std::cout << "global, computed numeric version: "
            << OPENFLUID_VERSION_COMPUTE(OPENFLUID_VERSIONMAJOR_NUMBER,
                                         OPENFLUID_VERSIONMINOR_NUMBER,
                                         OPENFLUID_VERSIONPATCH_NUMBER) << std::endl;

#if !(OPENFLUID_VERSION_COMPUTE(OPENFLUID_VERSIONMAJOR_NUMBER,\
                                OPENFLUID_VERSIONMINOR_NUMBER,\
                                OPENFLUID_VERSIONPATCH_NUMBER) == OPENFLUID_VERSION_NUMERIC)
  BOOST_FAIL("Error in defined macros for OpenFLUID version");
#endif

#if !(OPENFLUID_VERSION_COMPUTE(0,0,0) < OPENFLUID_VERSION_NUMERIC)
  BOOST_FAIL("Error in OpenFLUID version");
#endif
}
