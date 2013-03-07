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
  \file MarketPackage_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_marketpackage
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <openfluid/market/MarketSrcFunctionPackage.hpp>
#include <openfluid/market/MarketBinFunctionPackage.hpp>
#include <openfluid/market/MarketSrcObserverPackage.hpp>
#include <openfluid/market/MarketBinObserverPackage.hpp>
#include <openfluid/market/MarketDatasetPackage.hpp>

#include <tests-config.hpp>

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  std::string TmpDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp").string();;
  std::string MarketBagFunctionDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-functions").string();
  std::string MarketBagObserverDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-observers").string();
  std::string MarketBagDatasetDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-datasets").string();
  std::string MarketBagBinSubDir = "bin";
  std::string MarketBagSrcSubDir = "src";

  openfluid::market::MarketPackage::setWorksDirs(TmpDir, MarketBagFunctionDir, MarketBagObserverDir,
      MarketBagDatasetDir, MarketBagBinSubDir, MarketBagSrcSubDir);

  openfluid::market::MarketPackage::initialize(false);

  boost::filesystem::remove_all(boost::filesystem::path(TmpDir));
  boost::filesystem::remove_all(boost::filesystem::path(MarketBagFunctionDir));
  boost::filesystem::remove_all(boost::filesystem::path(MarketBagObserverDir));
  boost::filesystem::remove_all(boost::filesystem::path(MarketBagDatasetDir));
  boost::filesystem::remove_all(boost::filesystem::path(MarketBagSrcSubDir));
  boost::filesystem::remove_all(boost::filesystem::path(MarketBagBinSubDir));


  BOOST_REQUIRE(boost::filesystem::create_directories(boost::filesystem::path(openfluid::market::MarketPackage::getMarketBagFunctionDir())));
  BOOST_REQUIRE(boost::filesystem::create_directories(boost::filesystem::path(openfluid::market::MarketPackage::getMarketBagFunctionDir()
                +"/"+ openfluid::market::MarketPackage::getMarketBagBinSubDir())));
  BOOST_REQUIRE(boost::filesystem::create_directories(boost::filesystem::path(openfluid::market::MarketPackage::getMarketBagFunctionDir()
                +"/"+ openfluid::market::MarketPackage::getMarketBagSrcSubDir())));

  BOOST_REQUIRE(boost::filesystem::create_directories(boost::filesystem::path(openfluid::market::MarketPackage::getMarketBagObserverDir())));
  BOOST_REQUIRE(boost::filesystem::create_directories(boost::filesystem::path(openfluid::market::MarketPackage::getMarketBagObserverDir()
                +"/"+ openfluid::market::MarketPackage::getMarketBagBinSubDir())));
  BOOST_REQUIRE(boost::filesystem::create_directories(boost::filesystem::path(openfluid::market::MarketPackage::getMarketBagObserverDir()
                +"/"+ openfluid::market::MarketPackage::getMarketBagSrcSubDir())));

  BOOST_REQUIRE(boost::filesystem::create_directories(boost::filesystem::path(openfluid::market::MarketPackage::getMarketBagDatasetDir())));

  BOOST_REQUIRE(boost::filesystem::create_directories(boost::filesystem::path(openfluid::market::MarketPackage::getTempBuildsDir())));
  BOOST_REQUIRE(boost::filesystem::create_directories(boost::filesystem::path(openfluid::market::MarketPackage::getTempDownloadsDir())));


  openfluid::market::MarketSrcFunctionPackage SFPack("dummy.id","file:://path/to/file");
  BOOST_REQUIRE_EQUAL(SFPack.getID(),"dummy.id");
  BOOST_REQUIRE_EQUAL(SFPack.getInstallPath(), openfluid::market::MarketPackage::getMarketBagFunctionDir()
                      + "/" + openfluid::market::MarketPackage::getMarketBagSrcSubDir());

  openfluid::market::MarketBinFunctionPackage BFPack("dummy.id","file:://path/to/file");
  BOOST_REQUIRE_EQUAL(BFPack.getID(),"dummy.id");
  BOOST_REQUIRE_EQUAL(BFPack.getInstallPath(), openfluid::market::MarketPackage::getMarketBagFunctionDir()
                      + "/" + openfluid::market::MarketPackage::getMarketBagBinSubDir());


  openfluid::market::MarketSrcObserverPackage SOPack("dummy.id","file:://path/to/file");
  BOOST_REQUIRE_EQUAL(SOPack.getID(),"dummy.id");
  BOOST_REQUIRE_EQUAL(SOPack.getInstallPath(), openfluid::market::MarketPackage::getMarketBagObserverDir()
                      + "/" + openfluid::market::MarketPackage::getMarketBagSrcSubDir());

  openfluid::market::MarketBinObserverPackage BOPack("dummy.id","file:://path/to/file");
  BOOST_REQUIRE_EQUAL(BOPack.getID(),"dummy.id");
  BOOST_REQUIRE_EQUAL(BOPack.getInstallPath(), openfluid::market::MarketPackage::getMarketBagObserverDir()
                      + "/" + openfluid::market::MarketPackage::getMarketBagBinSubDir());


  openfluid::market::MarketDatasetPackage DPack("dummy.id","file:://path/to/file");
  BOOST_REQUIRE_EQUAL(DPack.getID(),"dummy.id");
  BOOST_REQUIRE_EQUAL(DPack.getInstallPath(), openfluid::market::MarketPackage::getMarketBagDatasetDir());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{

  std::string TmpDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp").string();;
  std::string MarketBagFunctionDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-functions").string();
  std::string MarketBagObserverDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-observers").string();
  std::string MarketBagDatasetDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-datasets").string();
  std::string MarketBagBinSubDir = "bin";
  std::string MarketBagSrcSubDir = "src";

  openfluid::market::MarketPackage::setWorksDirs(TmpDir,MarketBagFunctionDir,MarketBagObserverDir,
      MarketBagDatasetDir,MarketBagBinSubDir,MarketBagSrcSubDir);

  openfluid::market::MarketPackage::initialize(true);


  openfluid::market::MarketBinFunctionPackage BFPack("tests.market.func.bin.dummy","file://"+boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/market/packages/tests.market.func.bin.dummy.ofpk").string());

  BFPack.download();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp/downloads/tests.market.func.bin.dummy.ofpk")));

  BFPack.process();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-functions/bin/tests.market.func.bin.dummy.pdf.txt")));
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-functions/bin/tests.market.func.bin.dummy.xxmpi.txt")));

  openfluid::market::MarketBinObserverPackage BOPack("tests.market.obs.bin.dummy","file://"+boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/market/packages/tests.market.obs.bin.dummy.ofpk").string());

  BOPack.download();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp/downloads/tests.market.obs.bin.dummy.ofpk")));

  BOPack.process();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-observers/bin/tests.market.obs.bin.dummy.pdf.txt")));
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-observers/bin/tests.market.obs.bin.dummy.xxmpi.txt")));



  std::cout << "CMake Options : " << CONFIGTESTS_OPTIONS_FOR_CMAKE << std::endl;

  openfluid::market::MarketSrcFunctionPackage SFPack("tests.market.func.src.use","file://"+boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/market/packages/tests.market.func.src.use.ofpk").string());
  SFPack.setBuildConfigOptions(CONFIGTESTS_OPTIONS_FOR_CMAKE);

  SFPack.download();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp/downloads/tests.market.func.src.use.ofpk")));

  SFPack.process();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp/builds/tests.market.func.src.use/tests.market.func.src.use"+openfluid::config::FUNCTIONS_PLUGINS_SUFFIX+openfluid::config::PLUGINS_EXT)));
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-functions/src/tests.market.func.src.use")));
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-functions/bin/tests.market.func.src.use"+openfluid::config::FUNCTIONS_PLUGINS_SUFFIX+openfluid::config::PLUGINS_EXT)));


  openfluid::market::MarketSrcObserverPackage SOPack("tests.market.obs.src.use","file://"+boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/market/packages/tests.market.obs.src.use.ofpk").string());
  SOPack.setBuildConfigOptions(CONFIGTESTS_OPTIONS_FOR_CMAKE);

  SOPack.download();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp/downloads/tests.market.obs.src.use.ofpk")));

  SOPack.process();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp/builds/tests.market.obs.src.use/tests.market.obs.src.use"+openfluid::config::FUNCTIONS_PLUGINS_SUFFIX+openfluid::config::PLUGINS_EXT)));
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-observers/src/tests.market.obs.src.use")));
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-observers/bin/tests.market.obs.src.use"+openfluid::config::FUNCTIONS_PLUGINS_SUFFIX+openfluid::config::PLUGINS_EXT)));



  openfluid::market::MarketDatasetPackage DPack("tests.market.data.dummy","file:://"+boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/market/packages/tests.market.data.dummy.ofpk").string());

  DPack.download();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp/downloads/tests.market.data.dummy.ofpk")));

  DPack.process();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market-datasets/tests.market.data.dummy")));
}



