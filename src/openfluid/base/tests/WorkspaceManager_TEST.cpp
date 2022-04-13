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
  @file WorkspaceManager_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_workspacemanager


#include <boost/test/unit_test.hpp>

#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


const std::vector<std::string> ExpectedWaresPaths = {
  "/path/to/created-workspace/wares-dev/simulators/simA.test",
  "/path/to/created-workspace/wares-dev/observers/obsK.test",
  "/path/to/created-workspace/wares-dev/builderexts/bextX.test"
};
const std::string ActiveWarePath = "/path/to/created-workspace/wares-dev/observers/obsK.test";


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  auto* WM = openfluid::base::WorkspaceManager::instance();

  BOOST_CHECK(!WM->isOpen());
  BOOST_CHECK(WM->getWorkspacePath().empty());
  BOOST_CHECK(WM->getProjectsPath().empty());
  BOOST_CHECK(WM->getProjectPath("fake").empty());
  BOOST_CHECK(WM->getWaresPath().empty());
  BOOST_CHECK(WM->getWaresPath(openfluid::ware::WareType::SIMULATOR).empty());
  BOOST_CHECK(WM->getWaresPath(openfluid::ware::WareType::OBSERVER).empty());
  BOOST_CHECK(WM->getWaresPath(openfluid::ware::WareType::BUILDEREXT).empty());
  BOOST_CHECK(WM->getWarePath(openfluid::ware::WareType::SIMULATOR,"fake").empty());
  BOOST_CHECK(WM->getWarePath(openfluid::ware::WareType::OBSERVER,"fake").empty());
  BOOST_CHECK(WM->getWarePath(openfluid::ware::WareType::BUILDEREXT,"fake").empty());
  BOOST_CHECK(WM->getSettingsFile().empty());

  BOOST_CHECK(WM->getOpenWaresPaths().empty());
  BOOST_CHECK(WM->getActiveWarePath().empty());
  BOOST_CHECK(WM->getRecentProjects().empty());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_creation)
{
  const std::string WksPath = CONFIGTESTS_OUTPUT_DATA_DIR+"/WorkspaceManager/created-workspace";
  auto WksPathFSP = openfluid::tools::FilesystemPath(WksPath);

  if (WksPathFSP.isDirectory())
  {
    WksPathFSP.removeDirectory();
  }

  auto* WM = openfluid::base::WorkspaceManager::instance();
  
  WM->openWorkspace(WksPath);

  BOOST_REQUIRE(WM->isOpen());

  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getWorkspacePath()).isDirectory());
  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getProjectsPath()).isDirectory());
  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getWaresPath()).isDirectory());
  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getWaresPath(openfluid::ware::WareType::SIMULATOR)).isDirectory());
  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getWaresPath(openfluid::ware::WareType::OBSERVER)).isDirectory());
  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getWaresPath(openfluid::ware::WareType::BUILDEREXT)).isDirectory());
  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getSettingsFile()).isFile());

  for (int i = 1; i <= 5; i++)
  {
    WM->insertRecentProject("Project"+std::to_string(i),
                            "/path/to/created-workspace/projects/Project"+std::to_string(i));
  }

  WM->insertRecentProject("Project2","/path/to/created-workspace/projects/Project2");
  WM->insertRecentProject("Project4","/path/to/created-workspace/projects/Project4");


  BOOST_CHECK_EQUAL(WM->getRecentProjects().size(),5);

  WM->clearRecentProjects();
  BOOST_CHECK(WM->getRecentProjects().empty());

  for (unsigned int i = 1; i <= WM->RecentProjectsMax+5; i++)
  {
    WM->insertRecentProject("Project"+std::to_string(i),
                            "/path/to/created-workspace/projects/Project"+std::to_string(i));
  }
  BOOST_CHECK_EQUAL(WM->getRecentProjects().size(),WM->RecentProjectsMax);

  WM->setOpenWaresPaths(ExpectedWaresPaths);
  WM->setActiveWarePath(ActiveWarePath);
  WM->setWaresConfigureMode("DEBUG");
  WM->setWaresBuildMode("BUILDONLY");
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_opening)
{
  const std::string WksPath = CONFIGTESTS_OUTPUT_DATA_DIR+"/WorkspaceManager/created-workspace";
  auto WksPathFSP = openfluid::tools::FilesystemPath(WksPath);

  BOOST_CHECK(WksPathFSP.isDirectory());

  auto* WM = openfluid::base::WorkspaceManager::instance();

  WM->openWorkspace(WksPath);

  BOOST_CHECK(WM->isOpen());

  auto RetWaresPaths = WM->getOpenWaresPaths();
  BOOST_CHECK_EQUAL_COLLECTIONS(ExpectedWaresPaths.begin(),ExpectedWaresPaths.end(),
                                RetWaresPaths.begin(),RetWaresPaths.end());
  BOOST_CHECK_EQUAL(WM->getActiveWarePath(),ActiveWarePath);
  BOOST_CHECK_EQUAL(WM->getWaresConfigureMode(),"DEBUG");
  BOOST_CHECK_EQUAL(WM->getWaresBuildMode(),"BUILDONLY");

  auto RetRecentsPrj = WM->getRecentProjects();
  BOOST_CHECK_EQUAL(RetRecentsPrj.size(),WM->RecentProjectsMax);

  unsigned int Nbr = 15;
  for (const auto& R : RetRecentsPrj)
  {
    BOOST_CHECK_EQUAL(R.Name,"Project"+std::to_string(Nbr));
    BOOST_CHECK_EQUAL(R.Path,"/path/to/created-workspace/projects/Project"+std::to_string(Nbr));
    Nbr--;
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_fileformat_conversion)
{
  std::string OrigWksPath = CONFIGTESTS_INPUT_MISCDATA_DIR+"/WorkspaceManager";
  std::string WksPath = CONFIGTESTS_OUTPUT_DATA_DIR+"/WorkspaceManager/conversion";

  auto WksPathFSP = openfluid::tools::FilesystemPath(WksPath);

  if (WksPathFSP.isDirectory())
  {
    WksPathFSP.removeDirectory();
  }

  openfluid::tools::Filesystem::copyDirectory(OrigWksPath,WksPath);

  auto* WM = openfluid::base::WorkspaceManager::instance();
  WM->openWorkspace(WksPath);

  BOOST_REQUIRE(WM->isOpen());
  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getWorkspacePath()).isDirectory());
  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getProjectsPath()).isDirectory());
  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getWaresPath()).isDirectory());
  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getWaresPath(openfluid::ware::WareType::SIMULATOR)).isDirectory());
  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getWaresPath(openfluid::ware::WareType::OBSERVER)).isDirectory());
  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getWaresPath(openfluid::ware::WareType::BUILDEREXT)).isDirectory());
  BOOST_CHECK(openfluid::tools::FilesystemPath(WM->getSettingsFile()).isFile());
  BOOST_CHECK(WM->getRecentProjects().empty());
}
