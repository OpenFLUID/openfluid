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
  @file MarketPackage_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_marketpackage
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/market/MarketSrcSimulatorPackage.hpp>
#include <openfluid/market/MarketBinSimulatorPackage.hpp>
#include <openfluid/market/MarketSrcObserverPackage.hpp>
#include <openfluid/market/MarketBinObserverPackage.hpp>
#include <openfluid/market/MarketSrcBuilderextPackage.hpp>
#include <openfluid/market/MarketBinBuilderextPackage.hpp>
#include <openfluid/market/MarketDatasetPackage.hpp>
#include <openfluid/tools/Filesystem.hpp>

#include <tests-config.hpp>
#include <QCoreApplication>

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  std::string TmpDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp";
  std::string MarketBagSimulatorDir =
      CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-simulators";
  std::string MarketBagObserverDir =
      CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-observers";
  std::string MarketBagBuilderextDir =
      CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-builderexts";
  std::string MarketBagDatasetDir =
      CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-datasets";
  std::string MarketBagBinSubDir = "bin";
  std::string MarketBagSrcSubDir = "src";

  openfluid::market::MarketPackage::setWorksDirs(TmpDir, MarketBagSimulatorDir, MarketBagObserverDir,
      MarketBagBuilderextDir, MarketBagDatasetDir, MarketBagBinSubDir, MarketBagSrcSubDir);

  openfluid::market::MarketPackage::initialize(false);

  openfluid::tools::Filesystem::removeDirectory(TmpDir);
  openfluid::tools::Filesystem::removeDirectory(MarketBagSimulatorDir);
  openfluid::tools::Filesystem::removeDirectory(MarketBagObserverDir);
  openfluid::tools::Filesystem::removeDirectory(MarketBagBuilderextDir);
  openfluid::tools::Filesystem::removeDirectory(MarketBagDatasetDir);
  openfluid::tools::Filesystem::removeDirectory(MarketBagSrcSubDir);
  openfluid::tools::Filesystem::removeDirectory(MarketBagBinSubDir);


  BOOST_REQUIRE(
      openfluid::tools::Filesystem::makeDirectory(openfluid::market::MarketPackage::getMarketBagSimulatorDir())
  );

  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(openfluid::market::MarketPackage::getMarketBagSimulatorDir()
                +"/"+ openfluid::market::MarketPackage::getMarketBagBinSubDir()));
  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(openfluid::market::MarketPackage::getMarketBagSimulatorDir()
                +"/"+ openfluid::market::MarketPackage::getMarketBagSrcSubDir()));

  BOOST_REQUIRE(
      openfluid::tools::Filesystem::makeDirectory(openfluid::market::MarketPackage::getMarketBagObserverDir())
  );
  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(openfluid::market::MarketPackage::getMarketBagObserverDir()
                +"/"+ openfluid::market::MarketPackage::getMarketBagBinSubDir()));
  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(openfluid::market::MarketPackage::getMarketBagObserverDir()
                +"/"+ openfluid::market::MarketPackage::getMarketBagSrcSubDir()));

  BOOST_REQUIRE(
      openfluid::tools::Filesystem::makeDirectory(openfluid::market::MarketPackage::getMarketBagBuilderextDir())
  );
  BOOST_REQUIRE(
      openfluid::tools::Filesystem::makeDirectory(openfluid::market::MarketPackage::getMarketBagBuilderextDir()
      +"/"+ openfluid::market::MarketPackage::getMarketBagBinSubDir())
  );
  BOOST_REQUIRE(
      openfluid::tools::Filesystem::makeDirectory(openfluid::market::MarketPackage::getMarketBagBuilderextDir()
      +"/"+ openfluid::market::MarketPackage::getMarketBagSrcSubDir())
  );

  BOOST_REQUIRE(
      openfluid::tools::Filesystem::makeDirectory(openfluid::market::MarketPackage::getMarketBagDatasetDir())
  );

  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(openfluid::market::MarketPackage::getTempBuildsDir()));
  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(openfluid::market::MarketPackage::getTempDownloadsDir()));


  openfluid::market::MarketSrcSimulatorPackage SFPack("dummy.id","file:://path/to/file");
  BOOST_REQUIRE_EQUAL(SFPack.getID(),"dummy.id");
  BOOST_REQUIRE_EQUAL(SFPack.getInstallPath(), openfluid::market::MarketPackage::getMarketBagSimulatorDir()
                      + "/" + openfluid::market::MarketPackage::getMarketBagSrcSubDir());

  openfluid::market::MarketBinSimulatorPackage BFPack("dummy.id","file:://path/to/file");
  BOOST_REQUIRE_EQUAL(BFPack.getID(),"dummy.id");
  BOOST_REQUIRE_EQUAL(BFPack.getInstallPath(), openfluid::market::MarketPackage::getMarketBagSimulatorDir()
                      + "/" + openfluid::market::MarketPackage::getMarketBagBinSubDir());


  openfluid::market::MarketSrcObserverPackage SOPack("dummy.id","file:://path/to/file");
  BOOST_REQUIRE_EQUAL(SOPack.getID(),"dummy.id");
  BOOST_REQUIRE_EQUAL(SOPack.getInstallPath(), openfluid::market::MarketPackage::getMarketBagObserverDir()
                      + "/" + openfluid::market::MarketPackage::getMarketBagSrcSubDir());

  openfluid::market::MarketBinObserverPackage BOPack("dummy.id","file:://path/to/file");
  BOOST_REQUIRE_EQUAL(BOPack.getID(),"dummy.id");
  BOOST_REQUIRE_EQUAL(BOPack.getInstallPath(), openfluid::market::MarketPackage::getMarketBagObserverDir()
                      + "/" + openfluid::market::MarketPackage::getMarketBagBinSubDir());


  openfluid::market::MarketSrcBuilderextPackage SBPack("dummy.id","file:://path/to/file");
  BOOST_REQUIRE_EQUAL(SBPack.getID(),"dummy.id");
  BOOST_REQUIRE_EQUAL(SBPack.getInstallPath(), openfluid::market::MarketPackage::getMarketBagBuilderextDir()
                      + "/" + openfluid::market::MarketPackage::getMarketBagSrcSubDir());

  openfluid::market::MarketBinBuilderextPackage BBPack("dummy.id","file:://path/to/file");
  BOOST_REQUIRE_EQUAL(BBPack.getID(),"dummy.id");
  BOOST_REQUIRE_EQUAL(BBPack.getInstallPath(), openfluid::market::MarketPackage::getMarketBagBuilderextDir()
                      + "/" + openfluid::market::MarketPackage::getMarketBagBinSubDir());


  openfluid::market::MarketDatasetPackage DPack("dummy.id","file:://path/to/file");
  BOOST_REQUIRE_EQUAL(DPack.getID(),"dummy.id");
  BOOST_REQUIRE_EQUAL(DPack.getInstallPath(), openfluid::market::MarketPackage::getMarketBagDatasetDir());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  std::string VersionStr = CONFIGTESTS_VERSION_MAJOR+"."+CONFIGTESTS_VERSION_MINOR+"."+CONFIGTESTS_VERSION_PATCH;


  std::string TmpDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp";
  std::string MarketBagSimulatorDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-simulators";
  std::string MarketBagObserverDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-observers";
  std::string MarketBagBuilderextDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-builderexts";
  std::string MarketBagDatasetDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-datasets";
  std::string MarketBagBinSubDir = "bin";
  std::string MarketBagSrcSubDir = "src";

  openfluid::market::MarketPackage::setWorksDirs(TmpDir, MarketBagSimulatorDir, MarketBagObserverDir,
      MarketBagBuilderextDir, MarketBagDatasetDir, MarketBagBinSubDir, MarketBagSrcSubDir);

  openfluid::market::MarketPackage::initialize(true);

  std::string Arch = openfluid::base::Environment::getMarketBagBinSubDir();

  openfluid::market::MarketBinSimulatorPackage BSPack("tests.market.sim.binonly",
                                                      "file://"+
                                                      CONFIGTESTS_OUTPUT_DATA_DIR+
                                                                              "/marketplace/simulators/"+Arch+
                                                                              "/tests.market.sim.binonly_"+VersionStr+
                                                                              "_"+Arch+".ofpk");

  BSPack.download();
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+
      "/market/packages/temp/downloads/tests.market.sim.binonly_"+VersionStr+"_"+
      Arch+".ofpk"));

  BSPack.process();
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-simulators/bin/tests.market.sim.binonly")
  );

  openfluid::market::MarketBinObserverPackage BOPack("tests.market.obs.binonly",
                                                     "file://"+CONFIGTESTS_OUTPUT_DATA_DIR+
                                                                                       "/marketplace/observers/"+Arch+
                                                                                       "/tests.market.obs.binonly_"+
                                                                                       VersionStr+"_"+Arch+
                                                                                       ".ofpk");

  BOPack.download();
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+
      "/market/packages/temp/downloads/tests.market.obs.binonly_"+VersionStr+"_"+
      Arch+".ofpk")
  );

  BOPack.process();
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+
      "/market/packages/market-observers/bin/tests.market.obs.binonly")
  );

  openfluid::market::MarketBinBuilderextPackage BBPack("tests.market.bext.binonly",
                                                       "file://"+
                                                       CONFIGTESTS_OUTPUT_DATA_DIR+
                                                       "/marketplace/builderexts/"+Arch+
                                                       "/tests.market.bext.binonly_"+
                                                       VersionStr+"_"+Arch+".ofpk");

  BBPack.download();
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+
      "/market/packages/temp/downloads/tests.market.bext.binonly_"+
      VersionStr+"_"+Arch+".ofpk")
  );

  BBPack.process();
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+
      "/market/packages/market-builderexts/bin/tests.market.bext.binonly"))
  ;



  std::cout << "CMake Options : " << CONFIGTESTS_OPTIONS_FOR_CMAKE << std::endl;

  openfluid::market::MarketSrcSimulatorPackage SSPack("tests.market.sim.srconly",
                                                      "file://"+

                                                          CONFIGTESTS_OUTPUT_DATA_DIR+
                                                          "/marketplace/simulators/src/tests.market.sim.srconly_"+
                                                          VersionStr+"_src.ofpk");
  SSPack.setBuildConfigOptions(CONFIGTESTS_OPTIONS_FOR_CMAKE);

  SSPack.download();
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+
      "/market/packages/temp/downloads/tests.market.sim.srconly_"+VersionStr+"_src.ofpk")
  );

  SSPack.process();
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+
                              "/market/packages/temp/builds/tests.market.sim.srconly/tests.market.sim.srconly"+
                              openfluid::config::SIMULATORS_PLUGINS_SUFFIX+openfluid::config::PLUGINS_EXT));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(
      CONFIGTESTS_OUTPUT_DATA_DIR+ "/market/packages/market-simulators/src/tests.market.sim.srconly")
  );
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+
                              "/market/packages/market-simulators/bin/tests.market.sim.srconly"+
                              openfluid::config::SIMULATORS_PLUGINS_SUFFIX+openfluid::config::PLUGINS_EXT));


  openfluid::market::MarketSrcObserverPackage SOPack("tests.market.obs.srconly",
                                                     "file://"+

                                                         CONFIGTESTS_OUTPUT_DATA_DIR+
                                                         "/marketplace/observers/src/tests.market.obs.srconly_"+
                                                         VersionStr+"_src.ofpk");
  SOPack.setBuildConfigOptions(CONFIGTESTS_OPTIONS_FOR_CMAKE);

  SOPack.download();
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+
                              "/market/packages/temp/downloads/tests.market.obs.srconly_"+VersionStr+"_src.ofpk"));

  SOPack.process();
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+
                              "/market/packages/temp/builds/tests.market.obs.srconly/tests.market.obs.srconly"+
                              openfluid::config::OBSERVERS_PLUGINS_SUFFIX+openfluid::config::PLUGINS_EXT));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(
      CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-observers/src/tests.market.obs.srconly")
  );
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+
                              "/market/packages/market-observers/bin/tests.market.obs.srconly"+
                              openfluid::config::OBSERVERS_PLUGINS_SUFFIX+openfluid::config::PLUGINS_EXT)
  );


  openfluid::market::MarketDatasetPackage DPack("tests.market.data",
                                                "file://"+
                                                CONFIGTESTS_OUTPUT_DATA_DIR+
                                                "/marketplace/datasets/tests.market.data_"+
                                                VersionStr+"_data.ofpk");

  DPack.download();
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+
      "/market/packages/temp/downloads/tests.market.data_"+VersionStr+"_data.ofpk")
  );

  DPack.process();
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(
      CONFIGTESTS_OUTPUT_DATA_DIR+
      "/market/packages/market-datasets/tests.market.data")
  );
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(
      CONFIGTESTS_OUTPUT_DATA_DIR+
      "/market/packages/market-datasets/tests.market.data/tests.market.data.fluidx")
  );
}

// =====================================================================
// =====================================================================

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}

