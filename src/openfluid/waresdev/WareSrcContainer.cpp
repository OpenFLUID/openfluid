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
 @file WareSrcContainer.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/


#include <regex>
#include <fstream>
#include <algorithm>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/waresdev/WareSrcContainer.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace waresdev {


WareSrcContainer::WareSrcContainer(const std::string& AbsolutePath, openfluid::ware::WareType Type,
                                   const std::string& WareID) :
    m_AbsolutePath(AbsolutePath), m_Type(Type), m_ID(WareID),
    m_AbsoluteCMakeConfigPath(), m_AbsoluteMainCppPath(), m_AbsoluteUiParamCppPath(),
    m_AbsoluteCMakeListsPath(), m_AbsoluteJsonPath()
{
  update();

  m_OFVersion = openfluid::base::Environment::getVersionMajorMinor();

  setConfigMode(ConfigMode::CONFIG_RELEASE);
  setBuildMode(BuildMode::BUILD_WITHINSTALL);
  setBuildJobs(openfluid::base::Environment::getIdealJobsCount());
}


// =====================================================================
// =====================================================================


WareSrcContainer::~WareSrcContainer()
{

}


// =====================================================================
// =====================================================================


void WareSrcContainer::update()
{
  m_AbsoluteCMakeListsPath = 
  m_AbsoluteJsonPath = 
  m_AbsoluteCMakeConfigPath = 
  m_AbsoluteMainCppPath =
  m_AbsoluteUiParamCppPath = "";

  std::string CMakeListsFilePath = 
    openfluid::tools::Filesystem::joinPath({m_AbsolutePath,"src",openfluid::config::WARESDEV_SRC_CMAKESTDFILE});

  if (openfluid::tools::Path(CMakeListsFilePath).isFile())
  {
    m_AbsoluteCMakeListsPath = CMakeListsFilePath;
  }

  std::string JsonFilePath =
    openfluid::tools::Filesystem::joinPath({m_AbsolutePath,openfluid::config::WARESDEV_WARESHUB_FILE});
  if (openfluid::tools::Path(JsonFilePath).isFile())
  {
    m_AbsoluteJsonPath = JsonFilePath;
  }

  std::string CMakeConfigFilePath = 
      openfluid::tools::Filesystem::joinPath({m_AbsolutePath,"src",openfluid::config::WARESDEV_SRC_CMAKEUSERFILE});

  if (openfluid::tools::Path(CMakeConfigFilePath).isFile())
  {
    m_AbsoluteCMakeConfigPath = CMakeConfigFilePath;

    std::ifstream CMakeConfigFile(m_AbsoluteCMakeConfigPath);
    if (!CMakeConfigFile.is_open())
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "Cannot open file "+m_AbsoluteCMakeConfigPath);
    }

    const std::string CMakeConfigContent((std::istreambuf_iterator<char>(CMakeConfigFile)),
                                         (std::istreambuf_iterator<char>()));

    CMakeConfigFile.close();

    std::string MainCppFilename = openfluid::config::WARESDEV_SRC_MAINFILE;
    if (!MainCppFilename.empty())
    {
      std::string MainCppFilePath = openfluid::tools::Filesystem::joinPath({m_AbsolutePath,"src",MainCppFilename});
 
      if (openfluid::tools::Path(MainCppFilePath).isFile())
      {
        m_AbsoluteMainCppPath = MainCppFilePath;
      }
    }

    std::string UiParamCppFilename = openfluid::config::WARESDEV_SRC_PARAMSUIFILE;
    if (!UiParamCppFilename.empty())
    {
      std::string UiParamCppFilePath = 
        openfluid::tools::Filesystem::joinPath({m_AbsolutePath,"src",UiParamCppFilename});

      if (openfluid::tools::Path(UiParamCppFilePath).isFile())
      {
        m_AbsoluteUiParamCppPath = UiParamCppFilePath;
      }
    }

  }
}


// =====================================================================
// =====================================================================


std::vector<std::string> WareSrcContainer::getDefaultFilesPaths()
{
  std::vector<std::string> L;

  if (!m_AbsoluteCMakeConfigPath.empty())
  {
    L.push_back(m_AbsoluteCMakeConfigPath);
  }

  if (!m_AbsoluteMainCppPath.empty())
  {
    L.push_back(m_AbsoluteMainCppPath);
  }
  else
  {
    auto FoundFiles = openfluid::tools::Filesystem::findFilesByExtension(m_AbsolutePath+"/src","cpp",false);

    if (!FoundFiles.empty())
    {
      std::sort(FoundFiles.begin(),FoundFiles.end());
      L.push_back(FoundFiles[0]);
    }
  }

  return L;
}


// =====================================================================
// =====================================================================


std::string WareSrcContainer::getBuildDirPath() const
{
  return m_BuildDirPath;
}


// =====================================================================
// =====================================================================


std::string WareSrcContainer::getMainCppPath() const
{
  return m_AbsoluteMainCppPath;
}


// =====================================================================
// =====================================================================


std::string WareSrcContainer::getUiParamCppPath() const
{
  return m_AbsoluteUiParamCppPath;
}


// =====================================================================
// =====================================================================


std::string WareSrcContainer::getCMakeConfigPath() const
{
  return m_AbsoluteCMakeConfigPath;
}


// =====================================================================
// =====================================================================


std::string WareSrcContainer::getCMakeListsPath() const
{
  return m_AbsoluteCMakeListsPath;
}


// =====================================================================
// =====================================================================


std::string WareSrcContainer::getJsonPath() const
{
  return m_AbsoluteJsonPath;
}


// =====================================================================
// =====================================================================


std::map<std::string,std::string> WareSrcContainer::getConfigureVariables() const
{
  std::map<std::string,std::string> Vars;

  // build type
  Vars["CMAKE_BUILD_TYPE"] = (m_ConfigMode == ConfigMode::CONFIG_RELEASE ? "Release" : "Debug");

  char* ChOpenFLUIDInstallPrefix = nullptr;
  ChOpenFLUIDInstallPrefix = std::getenv("OPENFLUID_INSTALL_PREFIX");
  if (ChOpenFLUIDInstallPrefix != nullptr)
  {
    Vars["CMAKE_PREFIX_PATH"] = 
      openfluid::tools::Filesystem::joinPath({std::string(ChOpenFLUIDInstallPrefix),"lib","cmake"});
  }

  return Vars;
}


// =====================================================================
// =====================================================================


std::string WareSrcContainer::getConfigureGenerator() const
{
  return openfluid::base::PreferencesManager::instance()->getWaresdevConfigureGenerator();
}


// =====================================================================
// =====================================================================


std::string WareSrcContainer::getConfigureExtraOptions() const
{
  return openfluid::base::PreferencesManager::instance()->getWaresdevConfigureOptions();
}


// =====================================================================
// =====================================================================


void WareSrcContainer::prepareBuildDirectory() const
{
  openfluid::tools::Path BuildPathObj(m_BuildDirPath);

  if (BuildPathObj.exists())
  {
    openfluid::tools::Filesystem::emptyDirectory(BuildPathObj.toGeneric());
  }
  else if (!BuildPathObj.makeDirectory())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "unable to create build directory");
  }
}


// =====================================================================
// =====================================================================


void WareSrcContainer::setConfigMode(ConfigMode Mode)
{
  m_ConfigMode = Mode;

  std::string ConfigTag;
  switch (m_ConfigMode)
  {
    case ConfigMode::CONFIG_RELEASE:
      ConfigTag = "-release";
      break;
    case ConfigMode::CONFIG_DEBUG:
      ConfigTag = "-debug";
      break;
    default:
      ConfigTag = "";
      break;
  }

  m_BuildDirPath = openfluid::tools::Filesystem::joinPath({m_AbsolutePath,"_build"+ConfigTag+"-"+m_OFVersion});
}


// =====================================================================
// =====================================================================


void WareSrcContainer::setBuildMode(BuildMode Mode)
{
  m_BuildMode = Mode;
}


// =====================================================================
// =====================================================================


void WareSrcContainer::setBuildJobs(unsigned int Jobs)
{
  m_BuildJobs = Jobs;
}


// =====================================================================
// =====================================================================


std::string WareSrcContainer::getBuildTarget() const
{
  // build target is "install" if current build mode is BUILD_WITHINSTALL, "" in other cases
  return (m_BuildMode == BuildMode::BUILD_WITHINSTALL ? "install" : "");
}


// =====================================================================
// =====================================================================


unsigned int WareSrcContainer::getBuildJobs() const
{
  return m_BuildJobs;
}


// =====================================================================
// =====================================================================


std::string WareSrcContainer::getGenerateDocTarget() const
{
  // TOIMPL
  return std::string();
}


} } // namespaces
