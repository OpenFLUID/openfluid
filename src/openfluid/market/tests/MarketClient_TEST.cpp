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
  \file MarketClient_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_marketclient
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <openfluid/market/MarketClient.hpp>

#include <tests-config.hpp>

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

  std::string TmpDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/repository/temp").string();
  std::string MarketBagObserverDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/repository/market-observers").string();
  std::string MarketBagBinSubDir = boost::filesystem::path("bin").string();;
  std::string MarketBagSrcSubDir = boost::filesystem::path("src").string();;

//  openfluid::market::MarketPackage::initialize(false);

  openfluid::market::MarketPackage::setWorksDirs(TmpDir,MarketBagObserverDir,MarketBagBinSubDir,MarketBagSrcSubDir);

  boost::filesystem::remove_all(boost::filesystem::path(TmpDir));
  boost::filesystem::remove_all(boost::filesystem::path(MarketBagObserverDir));
  boost::filesystem::remove_all(boost::filesystem::path(MarketBagBinSubDir));
  boost::filesystem::remove_all(boost::filesystem::path(MarketBagSrcSubDir));

  MC.enableLog(true);


  MC.connect("file://"+boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/market/repository").string());

  BOOST_REQUIRE_EQUAL(MC.isConnected(),true);

  MC.getMarketInfo(MI);
  BOOST_REQUIRE_EQUAL(MI.Name,"OpenFLUID-Market for tests");

  openfluid::market::MetaPackagesCatalog_t PC = MC.getMetaPackagesCatalog();

  std::cout << "Meta-packages count: " << PC.size() << std::endl;

  openfluid::market::MetaPackagesCatalog_t::iterator PCit;


  for (PCit = PC.begin(); PCit!=PC.end(); ++PCit)
  {
    std::cout << "############ " << PCit->first << " ############" << std::endl;
    std::cout << "ID: " << PCit->second.ID << std::endl;

    if (PCit->second.AvailablePackages.find(openfluid::market::MetaPackageInfo::BIN) != PCit->second.AvailablePackages.end())
    {
      std::cout << "BIN | URL: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::BIN].URL << std::endl;
/*      std::cout << "BIN | Name: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::BIN].Name << std::endl;
      std::cout << "BIN | Desc: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::BIN].Description << std::endl;
      std::cout << "BIN | Authors: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::BIN].Authors << std::endl;*/
      std::cout << "BIN | License: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::BIN].License << std::endl;

    }

    if (PCit->second.AvailablePackages.find(openfluid::market::MetaPackageInfo::SRC) != PCit->second.AvailablePackages.end())
    {
      std::cout << "SRC | URL: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::SRC].URL << std::endl;
/*      std::cout << "SRC | Name: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::SRC].Name << std::endl;
      std::cout << "SRC | Desc: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::SRC].Description << std::endl;
      std::cout << "SRC | Authors: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::SRC].Authors << std::endl;*/
      std::cout << "SRC | License: " << PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::SRC].License << std::endl;
    }

    BOOST_REQUIRE_EQUAL(PCit->second.Selected,openfluid::market::MetaPackageInfo::NONE);

  }

  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.dummy",openfluid::market::MetaPackageInfo::BIN));
  BOOST_REQUIRE_EQUAL(MC.getSelectionFlag("tests.market.dummy"),openfluid::market::MetaPackageInfo::BIN);
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.dummy",openfluid::market::MetaPackageInfo::SRC));
  BOOST_REQUIRE_EQUAL(MC.getSelectionFlag("tests.market.dummy"),openfluid::market::MetaPackageInfo::SRC);
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.dummy",openfluid::market::MetaPackageInfo::NONE));
  BOOST_REQUIRE_EQUAL(MC.getSelectionFlag("tests.market.dummy"),openfluid::market::MetaPackageInfo::NONE);


  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.dummy",openfluid::market::MetaPackageInfo::NONE));
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.binonly.dummy",openfluid::market::MetaPackageInfo::BIN));
  BOOST_REQUIRE(MC.setSelectionFlag("tests.market.srconly.dummy",openfluid::market::MetaPackageInfo::SRC));


  //MC.addBuildConfigOptions(CONFIGTESTS_OPTIONS_FOR_CMAKE);

  openfluid::market::MarketPackage::setCommonBuildOptions(CONFIGTESTS_OPTIONS_FOR_CMAKE);

  MC.installSelection();

  MC.disconnect();

  BOOST_REQUIRE_EQUAL(MC.isConnected(),false);
}



