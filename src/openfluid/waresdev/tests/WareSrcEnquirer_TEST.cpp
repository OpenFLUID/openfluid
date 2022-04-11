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
 @file WareSrcEnquirer_TEST.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_waresrcenquirer


#include <fstream>  

#include <boost/test/unit_test.hpp>

#include <openfluid/waresdev/WareSrcEnquirer.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/config.hpp>


// =====================================================================
// =====================================================================


struct F
{
    std::string m_WorkspacePath;
    std::string m_WaresdevPath;
    std::string m_SimulatorsPath;

    std::vector<std::string> m_RealDirs;
    std::vector<std::string> m_RealFiles;

    F()
    {
      m_WorkspacePath = openfluid::base::PreferencesManager::instance()->getCurrentWorkspacePath();

      openfluid::base::WorkspaceManager::instance()->openWorkspace(m_WorkspacePath);

      std::cout << "Workspace path: " << m_WorkspacePath << std::endl;

      m_WaresdevPath = openfluid::tools::Filesystem::joinPath({m_WorkspacePath,openfluid::config::WARESDEV_PATH});

      m_SimulatorsPath = openfluid::tools::Filesystem::joinPath({m_WaresdevPath,openfluid::config::SIMULATORS_PATH});

      openfluid::tools::Filesystem::removeDirectory(m_WorkspacePath);
    }

    ~F()
    {
      openfluid::tools::Filesystem::removeDirectory(m_WorkspacePath);
    }

    void createTestFiles()
    {

      m_RealDirs = {
        openfluid::tools::Filesystem::joinPath({m_WorkspacePath,"nowaresdevdir"}),
        openfluid::tools::Filesystem::joinPath({m_WaresdevPath,"nowaretype"}),
        openfluid::tools::Filesystem::joinPath({m_WaresdevPath,"nowaretype","ware1"}),
        openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware1"}),
        openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware1","subdir1"}),
        openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware1","subdir1","subdir2"}),
        openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware2"}),
        openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware2","subdir1"})
      };

      m_RealFiles = {
        openfluid::tools::Filesystem::joinPath({m_WorkspacePath,"file0.txt"}),
        openfluid::tools::Filesystem::joinPath({m_WorkspacePath,"nowaresdevdir","file1.txt"}),
        openfluid::tools::Filesystem::joinPath({m_WaresdevPath,"file2.txt"}),
        openfluid::tools::Filesystem::joinPath({m_WaresdevPath,"nowaretype","file3.txt"}),
        openfluid::tools::Filesystem::joinPath({m_WaresdevPath,"nowaretype","ware1","file4.txt"}),
        openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"file5.txt"}),
        openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware1","file6.txt"}),
        openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware1","subdir1","file7.txt"}),
        openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware1","subdir1","subdir2","file8.txt"}),
        openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware2","file9.txt"}),
        openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware2","subdir1","file10.txt"}),
      };

      for(const auto& D : m_RealDirs)
      {
        openfluid::tools::Filesystem::makeDirectory(D);
      }

      for(const auto& F : m_RealFiles)
      {
        std::ofstream output(F);
      }
    }

  };


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(constructor,F)
{
  openfluid::base::Environment::init();

  BOOST_CHECK(!openfluid::tools::Filesystem::exists(m_WaresdevPath));
  BOOST_CHECK(!openfluid::tools::Filesystem::exists(m_SimulatorsPath));
}


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(getWareTypePath,F)
{
  openfluid::base::Environment::init();

  BOOST_CHECK_EQUAL(
    openfluid::base::WorkspaceManager::instance()->getWaresPath(openfluid::ware::WareType::SIMULATOR),
    m_SimulatorsPath
  );
}


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(getPathInfo,F)
{
  openfluid::base::Environment::init();

  createTestFiles();


  openfluid::waresdev::WareSrcEnquirer::WarePathInfo Info;


  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath("toto/warename");
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, false);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath("toto");
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, false);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealDirs.at(0));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealDirs.at(1));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealDirs.at(2));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealDirs.at(3));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, true);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);
  BOOST_CHECK_EQUAL(Info.AbsolutePath, m_RealDirs.at(3));
  BOOST_CHECK_EQUAL(static_cast<int>(Info.WareType), 
                    static_cast<int>(openfluid::ware::WareType::SIMULATOR));
  BOOST_CHECK_EQUAL(Info.WareDirName, "ware1");
  BOOST_CHECK_EQUAL(Info.AbsoluteWarePath,openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware1"}));

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealDirs.at(4));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealDirs.at(5));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealDirs.at(6));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, true);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);
  BOOST_CHECK_EQUAL(Info.AbsoluteWarePath,openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware2"}));

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealDirs.at(7));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);


  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath("/toto/warename/file.txt");
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, false);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath("file.txt");
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, false);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath("file.txt");
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, false);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealFiles.at(0));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealFiles.at(1));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealFiles.at(2));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealFiles.at(3));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealFiles.at(4));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealFiles.at(5));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, false);

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealFiles.at(6));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, true);
  BOOST_CHECK_EQUAL(Info.AbsolutePath, m_RealFiles.at(6));
  BOOST_CHECK_EQUAL(static_cast<int>(Info.WareType), 
                    static_cast<int>(openfluid::ware::WareType::SIMULATOR));
  BOOST_CHECK_EQUAL(Info.WareDirName, "ware1");
  BOOST_CHECK_EQUAL(Info.RelativePathToWare, "file6.txt");
  BOOST_CHECK_EQUAL(Info.AbsoluteWarePath, openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware1"}));
  BOOST_CHECK_EQUAL(Info.FileName, "file6.txt");

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealFiles.at(7));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, true);
  BOOST_CHECK_EQUAL(Info.AbsolutePath,m_RealFiles.at(7));
  BOOST_CHECK_EQUAL(static_cast<int>(Info.WareType), 
                    static_cast<int>(openfluid::ware::WareType::SIMULATOR));
  BOOST_CHECK_EQUAL(Info.WareDirName, "ware1");
  BOOST_CHECK_EQUAL(Info.RelativePathToWare, "subdir1/file7.txt");
  BOOST_CHECK_EQUAL(Info.AbsoluteWarePath, openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware1"}));
  BOOST_CHECK_EQUAL(Info.FileName, "file7.txt");

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealFiles.at(8));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, true);
  BOOST_CHECK_EQUAL(Info.AbsolutePath,(m_RealFiles.at(8)));
  BOOST_CHECK_EQUAL(static_cast<int>(Info.WareType), 
                    static_cast<int>(openfluid::ware::WareType::SIMULATOR));
  BOOST_CHECK_EQUAL(Info.WareDirName, "ware1");
  BOOST_CHECK_EQUAL(Info.RelativePathToWare, "subdir1/subdir2/file8.txt");
  BOOST_CHECK_EQUAL(Info.AbsoluteWarePath, openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware1"}));
  BOOST_CHECK_EQUAL(Info.FileName, "file8.txt");

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealFiles.at(9));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, true);
  BOOST_CHECK_EQUAL(Info.AbsolutePath, m_RealFiles.at(9));
  BOOST_CHECK_EQUAL(static_cast<int>(Info.WareType), 
                    static_cast<int>(openfluid::ware::WareType::SIMULATOR));
  BOOST_CHECK_EQUAL(Info.WareDirName, "ware2");
  BOOST_CHECK_EQUAL(Info.RelativePathToWare, "file9.txt");
  BOOST_CHECK_EQUAL(Info.AbsoluteWarePath, openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware2"}));
  BOOST_CHECK_EQUAL(Info.FileName, "file9.txt");

  Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(m_RealFiles.at(10));
  BOOST_CHECK_EQUAL(Info.IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.IsWareDirectory, false);
  BOOST_CHECK_EQUAL(Info.IsWareFile, true);
  BOOST_CHECK_EQUAL(Info.AbsolutePath, m_RealFiles.at(10));
  BOOST_CHECK_EQUAL(static_cast<int>(Info.WareType), 
                    static_cast<int>(openfluid::ware::WareType::SIMULATOR));
  BOOST_CHECK_EQUAL(Info.WareDirName, "ware2");
  BOOST_CHECK_EQUAL(Info.RelativePathToWare, "subdir1/file10.txt");
  BOOST_CHECK_EQUAL(Info.AbsoluteWarePath, openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware2"}));
  BOOST_CHECK_EQUAL(Info.FileName, "file10.txt");
}
