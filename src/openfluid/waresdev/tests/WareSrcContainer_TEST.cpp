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
  @file WareSrcContainer_TEST.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_waresrcabstractcontainer


#include <string>
#include <fstream>
#include <map>
#include <vector>

#include <boost/test/unit_test.hpp>

#include <openfluid/waresdev/WareSrcContainer.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/config.hpp>


// =====================================================================
// =====================================================================


struct F
{
  std::string m_WorkspacePath;
  std::string m_WaresdevPath;
  std::string m_SimulatorsPath;


  struct RealInfos
  {
    std::string AbsolutePath;
    std::vector<std::string> FilesNames;
  };

  std::map<std::string,RealInfos> m_RealDirsFiles;


  F()
  {
    m_WorkspacePath = openfluid::base::PreferencesManager::instance()->getCurrentWorkspacePath();
    m_WaresdevPath = openfluid::tools::Filesystem::joinPath({m_WorkspacePath,openfluid::config::WARESDEV_PATH});
    m_SimulatorsPath = openfluid::tools::Filesystem::joinPath({m_WaresdevPath,openfluid::config::SIMULATORS_PATH});
  }


  void appendRealDirFile(const std::string& Dirname, const std::string& Filename = "")
  {
    const auto it = m_RealDirsFiles.find(Dirname);

    if (it != m_RealDirsFiles.end() && !Filename.empty())
    {
      (*it).second.FilesNames.push_back(Filename);
    }
    else
    {
      m_RealDirsFiles[Dirname].AbsolutePath = openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,Dirname});

      if (!Filename.empty())
      {
        m_RealDirsFiles[Dirname].FilesNames.push_back(Filename);
      }
    }
  }


  void createTestFiles()
  {
    // right CMake content
    appendRealDirFile("ware_ok","src/"+openfluid::config::WARESDEV_SRC_CMAKEUSERFILE);
    appendRealDirFile("ware_ok","src/"+openfluid::config::WARESDEV_SRC_MAINFILE);
    appendRealDirFile("ware_ok","src/other.cpp");
    appendRealDirFile("ware_wrongcmake");
    
    // wrong CMake content
    appendRealDirFile("ware_wrongcmake","src/"+openfluid::config::WARESDEV_SRC_CMAKEUSERFILE);
    appendRealDirFile("ware_wrongcmake","src/WareMain.cpp");
    appendRealDirFile("ware_wrongcmake","src/other.txt");
    
    // right CMake content
    appendRealDirFile("ware_nomaincpp","src/"+openfluid::config::WARESDEV_SRC_CMAKEUSERFILE);
    appendRealDirFile("ware_nomaincpp","src/other.cpp");
    appendRealDirFile("ware_nocmake","src/WareMain.cpp");
    appendRealDirFile("ware_nocmake","src/other.cpp");
    
    // right CMake content
    appendRealDirFile("ware_nocpp","src/"+openfluid::config::WARESDEV_SRC_CMAKEUSERFILE);
    appendRealDirFile("ware_empty");
    appendRealDirFile("ware_empty2");
    appendRealDirFile("ware_empty2/subdir","src/"+openfluid::config::WARESDEV_SRC_CMAKEUSERFILE);

    for (const auto& DF : m_RealDirsFiles)
    {
      const auto DirPath = openfluid::tools::Path(DF.second.AbsolutePath);

      DirPath.makeDirectory();
      for (const auto& F : DF.second.FilesNames)
      {
        DirPath.makeDirectory("src");
        DirPath.makeFile(F);
      }
    }

    std::vector<std::string> RightCmake;
    RightCmake.push_back(openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware_ok","src",
                                                                 openfluid::config::WARESDEV_SRC_CMAKEUSERFILE}));
    RightCmake.push_back(openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware_nomaincpp","src",
                                                                 openfluid::config::WARESDEV_SRC_CMAKEUSERFILE}));
    RightCmake.push_back(openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware_nocpp","src",
                                                                 openfluid::config::WARESDEV_SRC_CMAKEUSERFILE}));
    for(const auto& F : RightCmake)
    {
      std::ofstream RCFile(F);
      RCFile << 
        "# list of CPP files, the sim2doc tag must be contained in the first one\n"
        "# ex: SET(SIM_CPP MySim.cpp)\n"
        "SET(SIM_CPP main.cpp)\n";
      RCFile.close();
    }

    std::ofstream File(openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware_wrongcmake","src",
                                                               openfluid::config::WARESDEV_SRC_CMAKEUSERFILE}));
    File << "# list of CPP files, the sim2doc tag must be contained in the first one\n"
            "# ex: SET(SIM_CPP MySim.cpp)\n"
            "SET(SIM_CPP wrongmain.cpp)\n";
    File.close();
  }

};


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(getDefaultFiles,F)
{
  openfluid::base::Environment::init();

  openfluid::tools::Path(m_WorkspacePath).removeDirectory();

  createTestFiles();


  std::vector<std::string> List;
  List = openfluid::waresdev::WareSrcContainer(m_RealDirsFiles["ware_ok"].AbsolutePath,
                                               openfluid::ware::WareType::SIMULATOR,"")
                                                 .getDefaultFilesPaths();
  BOOST_REQUIRE_EQUAL(List.size(),2);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[0]).filename(),openfluid::config::WARESDEV_SRC_CMAKEUSERFILE);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[1]).filename(),"WareMain.cpp");


  List = openfluid::waresdev::WareSrcContainer(m_RealDirsFiles["ware_wrongcmake"].AbsolutePath,
                                               openfluid::ware::WareType::SIMULATOR,"")
                                                .getDefaultFilesPaths();
  BOOST_REQUIRE_EQUAL(List.size(),2);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[0]).filename(),openfluid::config::WARESDEV_SRC_CMAKEUSERFILE);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[1]).filename(),openfluid::config::WARESDEV_SRC_MAINFILE);

  List = openfluid::waresdev::WareSrcContainer(m_RealDirsFiles["ware_nomaincpp"].AbsolutePath,
                                               openfluid::ware::WareType::SIMULATOR,"")
                                                 .getDefaultFilesPaths();
  BOOST_REQUIRE_EQUAL(List.size(),2);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[0]).filename(),openfluid::config::WARESDEV_SRC_CMAKEUSERFILE);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[1]).filename(),"other.cpp");

  List = openfluid::waresdev::WareSrcContainer(m_RealDirsFiles["ware_nocmake"].AbsolutePath, 
                                               openfluid::ware::WareType::SIMULATOR,"")
                                                 .getDefaultFilesPaths();
  BOOST_REQUIRE_EQUAL(List.size(),1);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[0]).filename(),openfluid::config::WARESDEV_SRC_MAINFILE);

  List = openfluid::waresdev::WareSrcContainer(m_RealDirsFiles["ware_nocpp"].AbsolutePath,
                                               openfluid::ware::WareType::SIMULATOR,"")
                                               .getDefaultFilesPaths();
  BOOST_REQUIRE_EQUAL(List.size(),1);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[0]).filename(),openfluid::config::WARESDEV_SRC_CMAKEUSERFILE);

  List = openfluid::waresdev::WareSrcContainer(m_RealDirsFiles["ware_empty"].AbsolutePath,
                                               openfluid::ware::WareType::SIMULATOR,"")
                                                 .getDefaultFilesPaths();
  BOOST_REQUIRE_EQUAL(List.size(),0);

  List = openfluid::waresdev::WareSrcContainer(m_RealDirsFiles["ware_empty2/src"].AbsolutePath,
                                               openfluid::ware::WareType::SIMULATOR,"")
                                                 .getDefaultFilesPaths();
  BOOST_REQUIRE_EQUAL(List.size(),0);

  openfluid::tools::Path(m_WorkspacePath).removeDirectory();
}

