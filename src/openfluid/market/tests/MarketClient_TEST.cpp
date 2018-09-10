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
  @file MarketClient_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_marketclient


#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <QCoreApplication>

#include <openfluid/market/MarketClient.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/utils/CMakeProxy.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::market::MarketClient MC;

  BOOST_REQUIRE_EQUAL(MC.isConnected(),false);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::market::MarketClient MC;
  openfluid::market::MarketInfo MI;

  std::string TmpDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/market/repository/temp";
  std::string MarketBagSimulatorDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/market/repository/market-simulators";
  std::string MarketBagObserverDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/market/repository/market-observers";
  std::string MarketBagBuilderextDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/market/repository/market-builderexts";
  std::string MarketBagDatasetDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/market/repository/market-datasets";
  std::string MarketBagBinSubDir = "bin";
  std::string MarketBagSrcSubDir = "src";

//  openfluid::market::MarketPackage::initialize(false);

  openfluid::market::MarketPackage::setWorksDirs(TmpDir, MarketBagSimulatorDir, MarketBagObserverDir,
      MarketBagBuilderextDir, MarketBagDatasetDir, MarketBagBinSubDir, MarketBagSrcSubDir);

  openfluid::tools::Filesystem::removeDirectory(TmpDir);
  openfluid::tools::Filesystem::removeDirectory(MarketBagSimulatorDir);
  openfluid::tools::Filesystem::removeDirectory(MarketBagObserverDir);
  openfluid::tools::Filesystem::removeDirectory(MarketBagBuilderextDir);
  openfluid::tools::Filesystem::removeDirectory(MarketBagDatasetDir);
  openfluid::tools::Filesystem::removeDirectory(MarketBagBinSubDir);
  openfluid::tools::Filesystem::removeDirectory(MarketBagSrcSubDir);

  MC.enableLog(true);


  MC.connect("file://"+CONFIGTESTS_OUTPUT_DATA_DIR+"/marketplace");

  BOOST_REQUIRE_EQUAL(MC.isConnected(),true);

  MC.getMarketInfo(MI);
  BOOST_REQUIRE_EQUAL(MI.Name,"OpenFLUID-Market generated automatically");

  openfluid::market::TypesMetaPackagesCatalogs_t TPC = MC.typesMetaPackagesCatalogs();

  openfluid::market::TypesMetaPackagesCatalogs_t::iterator TPCit;
  openfluid::market::MetaPackagesCatalog_t::iterator PCit;


  for (TPCit = TPC.begin(); TPCit != TPC.end(); ++TPCit)
  {
    //std::cout << "Meta-packages count: " << TPCit->second.size() << std::endl;

    for (PCit = TPCit->second.begin(); PCit != TPCit->second.end(); ++PCit)
    {
      //std::cout << "############ " << PCit->first << " ############" << std::endl;
      //std::cout << "ID: " << PCit->second.ID << std::endl;

      if (PCit->second.AvailablePackages.find(openfluid::market::MetaPackageInfo::BIN) !=
          PCit->second.AvailablePackages.end())
      {
  /*      std::cout << "BIN | URL: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::BIN].URL
                    << std::endl;
        std::cout << "BIN | Name: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::BIN].Name
                  << std::endl;
        std::cout << "BIN | Desc: "
                  << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::BIN].Description << std::endl;
        std::cout << "BIN | Authors: "
                  << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::BIN].Authors << std::endl;
        std::cout << "BIN | License: "
                  << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::BIN].License << std::endl;
*/
      }

      if (PCit->second.AvailablePackages.find(openfluid::market::MetaPackageInfo::SRC) !=
          PCit->second.AvailablePackages.end())
      {
  /*      std::cout << "SRC | URL: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::SRC].URL
                    << std::endl;
        std::cout << "SRC | Name: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::SRC].Name
                  << std::endl;
        std::cout << "SRC | Desc: "
                  << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::SRC].Description << std::endl;
        std::cout << "SRC | Authors: "
                  << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::SRC].Authors << std::endl;
        std::cout << "SRC | License: "
                  << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::SRC].License << std::endl;
*/      }

      BOOST_REQUIRE_EQUAL(PCit->second.Selected,openfluid::market::MetaPackageInfo::NONE);

    }
  }

  /** Simulators **/
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.sim",openfluid::market::MetaPackageInfo::BIN));
  BOOST_REQUIRE_EQUAL(MC.getSelectionFlag("tests.market.sim"),openfluid::market::MetaPackageInfo::BIN);
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.sim",openfluid::market::MetaPackageInfo::SRC));
  BOOST_REQUIRE_EQUAL(MC.getSelectionFlag("tests.market.sim"),openfluid::market::MetaPackageInfo::SRC);
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.sim",openfluid::market::MetaPackageInfo::NONE));
  BOOST_REQUIRE_EQUAL(MC.getSelectionFlag("tests.market.sim"),openfluid::market::MetaPackageInfo::NONE);

  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.sim",openfluid::market::MetaPackageInfo::NONE));
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.sim.binonly",openfluid::market::MetaPackageInfo::BIN));
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.sim.srconly",openfluid::market::MetaPackageInfo::SRC));

  /** Observers **/
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.obs",openfluid::market::MetaPackageInfo::BIN));
  BOOST_REQUIRE_EQUAL(MC.getSelectionFlag("tests.market.obs"),openfluid::market::MetaPackageInfo::BIN);
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.obs",openfluid::market::MetaPackageInfo::SRC));
  BOOST_REQUIRE_EQUAL(MC.getSelectionFlag("tests.market.obs"),openfluid::market::MetaPackageInfo::SRC);
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.obs",openfluid::market::MetaPackageInfo::NONE));
  BOOST_REQUIRE_EQUAL(MC.getSelectionFlag("tests.market.obs"),openfluid::market::MetaPackageInfo::NONE);

  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.obs",openfluid::market::MetaPackageInfo::NONE));
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.obs.binonly",openfluid::market::MetaPackageInfo::BIN));
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.obs.srconly",openfluid::market::MetaPackageInfo::SRC));

  /** Buildexts **/
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.bext",openfluid::market::MetaPackageInfo::BIN));
  BOOST_REQUIRE_EQUAL(MC.getSelectionFlag("tests.market.bext"),openfluid::market::MetaPackageInfo::BIN);
  //BOOST_REQUIRE(MC.setSelectionFlag("tests.market.bext",openfluid::market::MetaPackageInfo::SRC));
  //BOOST_REQUIRE_EQUAL(MC.getSelectionFlag("tests.market.bext"),openfluid::market::MetaPackageInfo::SRC);
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.bext",openfluid::market::MetaPackageInfo::NONE));
  BOOST_REQUIRE_EQUAL(MC.getSelectionFlag("tests.market.bext"),openfluid::market::MetaPackageInfo::NONE);

  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.bext",openfluid::market::MetaPackageInfo::NONE));
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.bext.binonly",openfluid::market::MetaPackageInfo::BIN));
  //BOOST_REQUIRE(MC.setSelectionFlag("tests.market.bext.srconly",openfluid::market::MetaPackageInfo::SRC));

  /** Datasets **/
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.data",openfluid::market::MetaPackageInfo::FLUIDX));
  BOOST_REQUIRE_EQUAL(MC.getSelectionFlag("tests.market.data"),openfluid::market::MetaPackageInfo::FLUIDX);

  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.data",openfluid::market::MetaPackageInfo::FLUIDX));

  //MC.addBuildConfigOptions(CONFIGTESTS_OPTIONS_FOR_CMAKE);

  openfluid::market::MarketPackage::setCommonBuildOptions(openfluid::market::PackageInfo::SIM,
                                                          CONFIGTESTS_OPTIONS_FOR_CMAKE);
  openfluid::market::MarketPackage::setCommonBuildOptions(openfluid::market::PackageInfo::OBS,
                                                          CONFIGTESTS_OPTIONS_FOR_CMAKE);
  openfluid::market::MarketPackage::setCommonBuildOptions(openfluid::market::PackageInfo::BUILD,
                                                          CONFIGTESTS_OPTIONS_FOR_CMAKE);

  MC.installSelection();

  MC.disconnect();

  BOOST_REQUIRE_EQUAL(MC.isConnected(),false);
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  openfluid::base::Environment::init();

  if (openfluid::utils::CMakeProxy::isAvailable())
  {
    std::cout << openfluid::utils::CMakeProxy::getVersion().toStdString() << std::endl;
    return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
  }
  else
    std::cout << "** Test not run due to failing to find CMake program **" << std::endl;

  return 0;
}
