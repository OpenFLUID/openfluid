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
  @file WorkspaceDevPurgeWorker_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_purgeworker
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <iostream>
#include <QApplication>
#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevPurgeWorker.hpp>
#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_purge)
{
  QString WorkspacePath = QString::fromStdString(CONFIGTESTS_OUTPUT_DATA_DIR)+"/workspacepurgeworker";


  openfluid::ui::waresdev::WorkspaceDevDashboardTypes::WaresSelectionByType AllWares;
  openfluid::ui::waresdev::WorkspaceDevDashboardTypes::WareProcessInfos PInfos;

  PInfos.ID = "sim01";
  PInfos.Path = WorkspacePath+"/simulators/"+PInfos.ID;
  AllWares[openfluid::ware::WareType::SIMULATOR].push_back(PInfos);

  PInfos.ID = "sim02";
  PInfos.Path = WorkspacePath+"/simulators/"+PInfos.ID;
  AllWares[openfluid::ware::WareType::SIMULATOR].push_back(PInfos);

  PInfos.ID = "sim03";
  PInfos.Path = WorkspacePath+"/simulators/"+PInfos.ID;
  AllWares[openfluid::ware::WareType::SIMULATOR].push_back(PInfos);

  PInfos.ID = "obs10";
  PInfos.Path = WorkspacePath+"/observers/"+PInfos.ID;
  AllWares[openfluid::ware::WareType::OBSERVER].push_back(PInfos);

  PInfos.ID = "obs20";
  PInfos.Path = WorkspacePath+"/observers/"+PInfos.ID;
  AllWares[openfluid::ware::WareType::OBSERVER].push_back(PInfos);

  PInfos.ID = "bext100";
  PInfos.Path = WorkspacePath+"/builderexts/"+PInfos.ID;
  AllWares[openfluid::ware::WareType::BUILDEREXT].push_back(PInfos);


  openfluid::ui::waresdev::WorkspaceDevDashboardTypes::WaresSelectionByType Selection = AllWares;

  Selection[openfluid::ware::WareType::SIMULATOR].pop_front();


  struct PurgeConfig
  {
    bool CurrentVer;

    bool OtherVer;

    bool Release;

    bool Debug;
  };

  std::vector<PurgeConfig> Configs =
  {
    {true,true,true,true},
    {true,false,true,false},
    {true,false,false,true},
    {false,true,true,false},
    {false,true,false,true},
  };


  for (auto& Conf : Configs)
  {
    for (auto& WType : AllWares)
    {
      for (auto& WItem : WType.second)
      {
        openfluid::tools::Filesystem::makeDirectory(WItem.Path.toStdString()+"/_build-release-0.0");
        openfluid::tools::Filesystem::makeDirectory(WItem.Path.toStdString()+"/_build-debug-0.0");
        openfluid::tools::Filesystem::makeDirectory(WItem.Path.toStdString()+"/_build-release-"+
                                                    openfluid::base::Environment::getVersionMajorMinor());
        openfluid::tools::Filesystem::makeDirectory(WItem.Path.toStdString()+"/_build-debug-"+
                                                    openfluid::base::Environment::getVersionMajorMinor());

        openfluid::ui::waresdev::WorkspaceDevPurgeWorker Worker(Selection,Conf.CurrentVer,Conf.OtherVer,
                                                                          Conf.Release,Conf.Debug);
        Worker.run();
      }
    }


    for (auto& WType : Selection)
    {
      for (auto& WItem : WType.second)
      {
        std::string PathToCheck;

        PathToCheck = WItem.Path.toStdString()+"/_build-release-0.0";
        std::cout << "Checking : " << PathToCheck << std::endl;
        BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(PathToCheck) ==
                      !(Conf.OtherVer && Conf.Release));

        PathToCheck = WItem.Path.toStdString()+"/_build-debug-0.0";
        std::cout << "Checking : " << PathToCheck << std::endl;
        BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(PathToCheck) ==
                      !(Conf.OtherVer && Conf.Debug));

        PathToCheck = WItem.Path.toStdString()+"/_build-release-"+openfluid::base::Environment::getVersionMajorMinor();
        std::cout << "Checking : " << PathToCheck << std::endl;
        BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(PathToCheck) ==
                      !(Conf.CurrentVer && Conf.Release));

        PathToCheck = WItem.Path.toStdString()+"/_build-debug-"+openfluid::base::Environment::getVersionMajorMinor();
        std::cout << "Checking : " << PathToCheck << std::endl;
        BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(PathToCheck) ==
                      !(Conf.CurrentVer && Conf.Debug));
      }
    }
  }
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  openfluid::base::Environment::init();

  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
