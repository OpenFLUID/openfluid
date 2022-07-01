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
 @file WareSrcFactory_TEST.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_WareSrcEnquirer


#include <boost/test/unit_test.hpp>

#include <openfluid/waresdev/WareSrcFactory.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/config.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(create_files_sim)
{
  auto Sign = openfluid::ware::SimulatorSignature();
  Sign.ID = "sim.waresrcfactory.test";

  openfluid::waresdev::WareSrcFactory::Configuration Config;
  Config.MainClassName = "TestSimulator";

  auto SimsPath = openfluid::tools::Path({CONFIGTESTS_OUTPUT_DATA_DIR,"WareSrcFactory","simulators"});

  SimsPath.removeDirectory(Sign.ID); 
  BOOST_CHECK(!SimsPath.exists(Sign.ID));

  openfluid::waresdev::WareSrcFactory::createSimulator(Sign,Config,SimsPath.toGeneric());

  BOOST_CHECK(SimsPath.isDirectory(Sign.ID));
  BOOST_CHECK(SimsPath.isDirectory(Sign.ID+"/src"));
  BOOST_CHECK(SimsPath.isDirectory(Sign.ID+"/doc"));
  BOOST_CHECK(SimsPath.isDirectory(Sign.ID+"/tests"));

  BOOST_CHECK(SimsPath.isFile(Sign.ID+"/CMakeLists.txt"));
  BOOST_CHECK(SimsPath.isFile(Sign.ID+"/src/WareMain.cpp"));
  BOOST_CHECK(!SimsPath.isFile(Sign.ID+"/src/WareUI.hpp"));
  BOOST_CHECK(!SimsPath.isFile(Sign.ID+"/src/WareUI.cpp"));
  BOOST_CHECK(SimsPath.isFile(Sign.ID+"/src/CMakeLists.txt"));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(create_files_sim_withui)
{
  auto Sign = openfluid::ware::SimulatorSignature();
  Sign.ID = "sim.waresrcfactory.test-withui";

  openfluid::waresdev::WareSrcFactory::Configuration Config;
  Config.MainClassName = "TestSimulator";
  Config.WithParamsUI = true;
  Config.ParamsUIClassName = "TestSimulatorUI";

  auto SimsPath = openfluid::tools::Path({CONFIGTESTS_OUTPUT_DATA_DIR,"WareSrcFactory","simulators"});

  SimsPath.removeDirectory(Sign.ID); 
  BOOST_CHECK(!SimsPath.exists(Sign.ID));

  openfluid::waresdev::WareSrcFactory::createSimulator(Sign,Config,SimsPath.toGeneric());

  BOOST_CHECK(SimsPath.isDirectory(Sign.ID));
  BOOST_CHECK(SimsPath.isDirectory(Sign.ID+"/src"));
  BOOST_CHECK(SimsPath.isDirectory(Sign.ID+"/doc"));
  BOOST_CHECK(SimsPath.isDirectory(Sign.ID+"/tests"));

  BOOST_CHECK(SimsPath.isFile(Sign.ID+"/CMakeLists.txt"));
  BOOST_CHECK(SimsPath.isFile(Sign.ID+"/src/WareMain.cpp"));
  BOOST_CHECK(SimsPath.isFile(Sign.ID+"/src/WareUI.hpp"));
  BOOST_CHECK(SimsPath.isFile(Sign.ID+"/src/WareUI.cpp"));
  BOOST_CHECK(SimsPath.isFile(Sign.ID+"/src/CMakeLists.txt"));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(create_files_obs)
{
  auto Sign = openfluid::ware::ObserverSignature();
  Sign.ID = "obs.waresrcfactory.test-withui";

  openfluid::waresdev::WareSrcFactory::Configuration Config;
  Config.MainClassName = "TestObserver";

  auto ObssPath = openfluid::tools::Path({CONFIGTESTS_OUTPUT_DATA_DIR,"WareSrcFactory","observers"});

  ObssPath.removeDirectory(Sign.ID); 
  BOOST_CHECK(!ObssPath.exists(Sign.ID));

  openfluid::waresdev::WareSrcFactory::createObserver(Sign,Config,ObssPath.toGeneric());

  BOOST_CHECK(ObssPath.isDirectory(Sign.ID));
  BOOST_CHECK(ObssPath.isDirectory(Sign.ID+"/src"));
  BOOST_CHECK(ObssPath.isDirectory(Sign.ID+"/doc"));
  BOOST_CHECK(ObssPath.isDirectory(Sign.ID+"/tests"));

  BOOST_CHECK(ObssPath.isFile(Sign.ID+"/CMakeLists.txt"));
  BOOST_CHECK(ObssPath.isFile(Sign.ID+"/src/WareMain.cpp"));
  BOOST_CHECK(!ObssPath.isFile(Sign.ID+"/src/WareUI.hpp"));
  BOOST_CHECK(!ObssPath.isFile(Sign.ID+"/src/WareUI.cpp"));
  BOOST_CHECK(ObssPath.isFile(Sign.ID+"/src/CMakeLists.txt"));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(create_files_obs_withui)
{
  auto Sign = openfluid::ware::ObserverSignature();
  Sign.ID = "obs.waresrcfactory.test";

  openfluid::waresdev::WareSrcFactory::Configuration Config;
  Config.MainClassName = "TestObserver";
  Config.WithParamsUI = true;
  Config.ParamsUIClassName = "TestObserverUI";

  auto ObssPath = openfluid::tools::Path({CONFIGTESTS_OUTPUT_DATA_DIR,"WareSrcFactory","observers"});

  ObssPath.removeDirectory(Sign.ID); 
  BOOST_CHECK(!ObssPath.exists(Sign.ID));

  openfluid::waresdev::WareSrcFactory::createObserver(Sign,Config,ObssPath.toGeneric());

  BOOST_CHECK(ObssPath.isDirectory(Sign.ID));
  BOOST_CHECK(ObssPath.isDirectory(Sign.ID+"/src"));
  BOOST_CHECK(ObssPath.isDirectory(Sign.ID+"/doc"));
  BOOST_CHECK(ObssPath.isDirectory(Sign.ID+"/tests"));

  BOOST_CHECK(ObssPath.isFile(Sign.ID+"/CMakeLists.txt"));
  BOOST_CHECK(ObssPath.isFile(Sign.ID+"/src/WareMain.cpp"));
  BOOST_CHECK(ObssPath.isFile(Sign.ID+"/src/WareUI.hpp"));
  BOOST_CHECK(ObssPath.isFile(Sign.ID+"/src/WareUI.cpp"));
  BOOST_CHECK(ObssPath.isFile(Sign.ID+"/src/CMakeLists.txt"));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(create_files_bext)
{
  auto Sign = openfluid::builderext::BuilderExtensionSignature();
  Sign.ID = "bext.waresrcfactory.test";
  Sign.Role = openfluid::builderext::ExtensionRole::FEATURE;
  Sign.Category = openfluid::builderext::ExtensionCategory::RESULTS;
  Sign.MenuText = "Test Builer-Extension";

  openfluid::waresdev::WareSrcFactory::Configuration Config;
  Config.MainClassName = "TestBuilderext";
  Config.UIMode = openfluid::builderext::ExtensionMode::WORKSPACE;
 

  auto BextsPath = openfluid::tools::Path({CONFIGTESTS_OUTPUT_DATA_DIR,"WareSrcFactory","builderexts"});

  BextsPath.removeDirectory(Sign.ID); 
  BOOST_CHECK(!BextsPath.exists(Sign.ID));

  openfluid::waresdev::WareSrcFactory::createBuilderext(Sign,Config,BextsPath.toGeneric());

  BOOST_CHECK(BextsPath.isDirectory(Sign.ID));
  BOOST_CHECK(BextsPath.isDirectory(Sign.ID+"/src"));
  BOOST_CHECK(BextsPath.isDirectory(Sign.ID+"/doc"));
  BOOST_CHECK(BextsPath.isDirectory(Sign.ID+"/tests"));

  BOOST_CHECK(BextsPath.isFile(Sign.ID+"/CMakeLists.txt"));
  BOOST_CHECK(BextsPath.isFile(Sign.ID+"/src/WareMain.cpp"));
  BOOST_CHECK(BextsPath.isFile(Sign.ID+"/src/WareMain.hpp"));
  BOOST_CHECK(BextsPath.isFile(Sign.ID+"/src/CMakeLists.txt"));
}

