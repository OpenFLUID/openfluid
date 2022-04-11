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
  std::string m_CMakeConfigFile;

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
    m_CMakeConfigFile = openfluid::config::WARESDEV_CMAKE_USERFILE;
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
    appendRealDirFile("ware_ok",m_CMakeConfigFile);
    appendRealDirFile("ware_ok","main.cpp");
    appendRealDirFile("ware_ok","other.cpp");
    appendRealDirFile("ware_wrongcmake");
    
    // wrong CMake content
    appendRealDirFile("ware_wrongcmake",m_CMakeConfigFile);
    appendRealDirFile("ware_wrongcmake","main.cpp");
    appendRealDirFile("ware_wrongcmake","other.txt");
    
    // right CMake content
    appendRealDirFile("ware_nomaincpp",m_CMakeConfigFile);
    appendRealDirFile("ware_nomaincpp","other.cpp");
    appendRealDirFile("ware_nocmake","main.cpp");
    appendRealDirFile("ware_nocmake","other.cpp");
    
    // right CMake content
    appendRealDirFile("ware_nocpp",m_CMakeConfigFile);
    appendRealDirFile("ware_empty");
    appendRealDirFile("ware_empty2");
    appendRealDirFile("ware_empty2/subdir",m_CMakeConfigFile);

    for (const auto& DF : m_RealDirsFiles)
    {
      const auto DirPath = openfluid::tools::Path(DF.second.AbsolutePath);

      DirPath.makeDirectory();
      for (const auto& F : DF.second.FilesNames)
      {
        DirPath.makeFile(F);
      }
    }

    std::vector<std::string> RightCmake;
    RightCmake.push_back(openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware_ok",m_CMakeConfigFile}));
    RightCmake.push_back(openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware_nomaincpp",m_CMakeConfigFile}));
    RightCmake.push_back(openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware_nocpp",m_CMakeConfigFile}));
    for(const auto& F : RightCmake)
    {
      std::ofstream RCFile(F);
      RCFile << 
        "# list of CPP files, the sim2doc tag must be contained in the first one\n"
        "# ex: SET(SIM_CPP MySim.cpp)\n"
        "SET(SIM_CPP main.cpp)\n";
      RCFile.close();
    }

    std::ofstream File(openfluid::tools::Filesystem::joinPath({m_SimulatorsPath,"ware_wrongcmake",m_CMakeConfigFile}));
    File << "# list of CPP files, the sim2doc tag must be contained in the first one\n"
            "# ex: SET(SIM_CPP MySim.cpp)\n"
            "SET(SIM_CPP wrongmain.cpp)\n";
    File.close();
  }

};


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(searchMainCppFileName)
{
  openfluid::base::Environment::init();

  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("AEIOU"),"");

  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP file.cpp )"),"file.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(SIM_CPP file.cpp )"),"file.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(SIM_CPP file.cpp)"),"file.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName(" SET (SIM_CPP file.cpp ) "),
                    "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP file.cpp other_file.cpp)"),"file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP file.cpp other_file.cpp) "),
                                                                   "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(SIM_CPP file.cpp other_file.cpp)"),"file.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName(
          "# list of CPP files, the sim2doc tag must be contained in the first one\n"
          "# ex: SET(SIM_CPP MySimulator.cpp)\n"
          "SET(SIM_CPP file.cpp other_file.cpp)\n"),"file.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName(
          "# list of CPP files, the sim2doc tag must be contained in the first one\n"
          "# ex: SET(SIM_CPP MySimulator.cpp)\n"
          "#SET(SIM_CPP file.cpp)\n"),"");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP file.hpp)"),"");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP fi-le.cpp)"),
                    "fi-le.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP fi_le.cpp)"),
                    "fi_le.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP fi.le.cpp)"),
                    "fi.le.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(SIM_CPP trap.cppabc.cpp)"),
                    "trap.cppabc.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (OBS_CPP file.cpp )"),
                    "file.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(OBS_CPP file.cpp )"),
                    "file.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(OBS_CPP file.cpp)"),
                    "file.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName(" SET (OBS_CPP file.cpp ) "),
                    "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (OBS_CPP file.cpp other_file.cpp)"),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (OBS_CPP file.cpp other_file.cpp) "),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(OBS_CPP file.cpp other_file.cpp)"),
      "file.cpp");
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(searchUiParamCppFileName)
{
  openfluid::base::Environment::init();

  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("AEIOU"), "");

  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET (OBS_PARAMSUI_CPP file.cpp )"),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET(OBS_PARAMSUI_CPP file.cpp )"),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET(OBS_PARAMSUI_CPP file.cpp)"),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName(" SET (OBS_PARAMSUI_CPP file.cpp ) "),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET (OBS_PARAMSUI_CPP file.cpp other_file.cpp)"),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::
            searchUiParamCppFileName("SET (OBS_PARAMSUI_CPP file.cpp other_file.cpp) "),"file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET(OBS_PARAMSUI_CPP file.cpp other_file.cpp)"),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName(
          "# list of CPP files for parameterization widget, if any\n"
          "# ex: SET(OBS_PARAMSUI_CPP MyWidget.cpp)\n"
          "SET(OBS_PARAMSUI_CPP file.cpp other_file.cpp)\n"),"file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName(
          "# list of CPP files for parameterization widget, if any\n"
          "# ex: SET(OBS_PARAMSUI_CPP MyWidget.cpp)\n"
          "#SET(OBS_PARAMSUI_CPP file.cpp)\n"),"");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET (SIM_PARAMSUI_CPP file.hpp)"),"");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET (SIM_PARAMSUI_CPP fi-le.cpp)"),"fi-le.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET (SIM_PARAMSUI_CPP fi_le.cpp)"),"fi_le.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET (SIM_PARAMSUI_CPP fi.le.cpp)"),"fi.le.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET(SIM_PARAMSUI_CPP piege.cppabc.cpp)"),
      "piege.cppabc.cpp");
}


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
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[0]).filename(),m_CMakeConfigFile);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[1]).filename(),"main.cpp");


  List = openfluid::waresdev::WareSrcContainer(m_RealDirsFiles["ware_wrongcmake"].AbsolutePath,
                                               openfluid::ware::WareType::SIMULATOR,"")
                                                .getDefaultFilesPaths();
  BOOST_REQUIRE_EQUAL(List.size(),2);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[0]).filename(),m_CMakeConfigFile);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[1]).filename(),"main.cpp");

  List = openfluid::waresdev::WareSrcContainer(m_RealDirsFiles["ware_nomaincpp"].AbsolutePath,
                                               openfluid::ware::WareType::SIMULATOR,"")
                                                 .getDefaultFilesPaths();
  BOOST_REQUIRE_EQUAL(List.size(),2);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[0]).filename(),m_CMakeConfigFile);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[1]).filename(),"other.cpp");

  List = openfluid::waresdev::WareSrcContainer(m_RealDirsFiles["ware_nocmake"].AbsolutePath, 
                                               openfluid::ware::WareType::SIMULATOR,"")
                                                 .getDefaultFilesPaths();
  BOOST_REQUIRE_EQUAL(List.size(),1);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[0]).filename(),"main.cpp");

  List = openfluid::waresdev::WareSrcContainer(m_RealDirsFiles["ware_nocpp"].AbsolutePath,
                                               openfluid::ware::WareType::SIMULATOR,"")
                                               .getDefaultFilesPaths();
  BOOST_REQUIRE_EQUAL(List.size(),1);
  BOOST_CHECK_EQUAL(openfluid::tools::Path(List[0]).filename(),m_CMakeConfigFile);

  List = openfluid::waresdev::WareSrcContainer(m_RealDirsFiles["ware_empty"].AbsolutePath,
                                               openfluid::ware::WareType::SIMULATOR,"")
                                                 .getDefaultFilesPaths();
  BOOST_REQUIRE_EQUAL(List.size(),0);

  List = openfluid::waresdev::WareSrcContainer(m_RealDirsFiles["ware_empty2"].AbsolutePath,
                                               openfluid::ware::WareType::SIMULATOR,"")
                                                 .getDefaultFilesPaths();
  BOOST_REQUIRE_EQUAL(List.size(),0);

  openfluid::tools::Path(m_WorkspacePath).removeDirectory();
}

