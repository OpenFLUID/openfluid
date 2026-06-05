/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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
  @file WareSetManager.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <fstream>
#include <iostream>
#include <iomanip>

#include <openfluid/base/InternalLogger.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/IOListener.hpp>
#include <openfluid/config.hpp>
#include <openfluid/fluidx/FluidXIO.hpp>
#include <openfluid/tools/Console.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/TemplateProcessor.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/utils/CMakeProxy.hpp>
#include <openfluid/utils/FluidHubAPIClient.hpp>
#include <openfluid/waresdev/WareSrcHelpers.hpp>

#include "WareSetManager.hpp"


namespace openfluid { namespace waresdev {


#define THROW_OR_PRINT(M)                      \
{                                          \
  if (IsStrict) \
  { \
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, M); \
  } \
  else \
  { \
    openfluid::tools::Console::setErrorColor(); \
    std::cout << M; \
    openfluid::tools::Console::resetAttributes(); \
    std::cout << std::endl; \
    openfluid::base::log::error("Wareset setup", M); \
    m_Problems++; \
  } \
}


void WareSetManager::displayStatus()
{
  // Print status table
  std::cout << "== Wareset status" << std::endl;
  std::vector OrderedSteps = {"fetch", "ckout", "config", "build", "install", "run"};
  std::cout << "                           ";
  for (const std::string Step : OrderedSteps)
  {
    std::cout << "\t" << Step;
  }
  std::cout << std::endl;
  // TODO set Common line last
  for (const auto& WareLine : m_WareStatus)
  {
    std::cout << "* " << std::setw(25) << (WareLine.first.length() > 25 ? 
                                            WareLine.first.substr(0,25)+"..." : WareLine.first);
    for (const std::string Step : OrderedSteps)
    {
      const auto It = WareLine.second.find(Step);
      if (It != WareLine.second.end())
      {
        std::cout << "\t";
        if (It->second == OK_STRING)
        {
          openfluid::tools::Console::setOKColor();
        }
        else if (It->second == KO_STRING)
        {
          openfluid::tools::Console::setErrorColor();
        }
        else
        {
          openfluid::tools::Console::setWarningColor();
        }
        std::cout << It->second;
        openfluid::tools::Console::resetAttributes();
      }
      else
      {
        std::cout << "\t";
      }
    }
    std::cout << std::endl;
  }
}

WareSetManager::WareSetManager(const std::string& WareSourceType, const std::string& WaresetSourceType, 
                               const std::string& SetOption, const std::string& WaresOrigin, std::string& ID) : 
  m_WareSourceType(WareSourceType), m_WaresetSourceType(WaresetSourceType), m_ID(ID), m_WaresOrigin(WaresOrigin)
{
  if (m_WaresetSourceType == "hub")
  {    
    // Define source URL
    std::string SourceURL;
    // 1) from set full URL
    if (SetOption.substr(0,4) == "http")
    {
      SourceURL = openfluid::tools::split(SetOption, "/api/").front()+"/api/";
    }
    // 2) from wares origin
    else if (m_WaresOrigin.substr(0,4) == "http")
    {
      SourceURL = m_WaresOrigin;
    }
    else
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                                                "Hub URL can not be deduced from provided information");
    }

    if (!SetOption.empty() && SetOption.find("/") == std::string::npos)
    {
      m_ID = SetOption;
    }
    else
    {
      m_ID = openfluid::tools::split(SetOption, "/").back();
    }
    

    std::string WaresetListJson;
    openfluid::utils::FluidHubAPIClient FHClient;
    try
    {
      if (FHClient.connect(SourceURL,false))
      {
        openfluid::tools::TemplateProcessor::Data Waresets;
        WaresetListJson = FHClient.getWareset(m_ID);
        bool Found = !WaresetListJson.empty();
        if (!Found)
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                                                "Wareset not found on hub instance");
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                                                "Error during FluidHub connection");
      }
    }
    catch(openfluid::base::FrameworkException& E)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                                                "Wareset request failure ("+std::string(E.what())+")");
    }

    try
    {
      m_JSONWareset = openfluid::thirdparty::json::parse(WaresetListJson);
    }
    catch (openfluid::thirdparty::json::parse_error&)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                                                "Wareset setup: Response json parsing error");
    }
  }
  else if (m_WaresetSourceType == "dataset")
  {
    //TOIMPL deduce m_ID from path: either last part or the one before if last is "IN"?
    std::set<std::string> AddedWares;
    std::unique_ptr<openfluid::base::IOListener> Listener = std::make_unique<openfluid::base::IOListener>();
    openfluid::fluidx::FluidXIO FXIO(Listener.get());
    auto FXDesc = FXIO.loadFromDirectory(SetOption);
    for (const auto& i : FXDesc.model().items())
    {
      if (i->isType(openfluid::ware::WareType::SIMULATOR) && i->isEnabled()) 
      {
        openfluid::thirdparty::json WareJson = openfluid::thirdparty::json::object();
        WareJson["type"] = "simulators";
        WareJson["id"] = i->getID();
        WareJson["version"] = "";
        std::string WareKey = std::string(WareJson["type"])+"/"+std::string(WareJson["id"]);
        if (AddedWares.find(WareKey) == AddedWares.end())
        {
          m_JSONWareset.push_back(WareJson);
          AddedWares.insert(WareKey);
        }
      }
    }
    for (const auto& i : FXDesc.monitoring().items())
    {
      if (i->isEnabled()) 
      {
        openfluid::thirdparty::json WareJson = openfluid::thirdparty::json::object();
        WareJson["type"] = "observers";
        WareJson["id"] = i->getID();
        WareJson["version"] = "";
        std::string WareKey = std::string(WareJson["type"])+"/"+std::string(WareJson["id"]);
        if (AddedWares.find(WareKey) == AddedWares.end())
        {
          m_JSONWareset.push_back(WareJson);
          AddedWares.insert(WareKey);
        }
      }
    }
    // Use version data from potential openfluid-dataset.json
    // TODO this function should be in dataset processing class
    std::ifstream FileStream;
    FileStream.open(openfluid::tools::Filesystem::joinPath({SetOption, "openfluid-dataset.json"}),std::ifstream::in);
    if (!FileStream.is_open())
    {
      openfluid::base::log::warning("Wareset setup", "No dataset metadata");
    }
    else
    {
      try
      {
        openfluid::thirdparty::json DatasetMetadataJson = openfluid::thirdparty::json::parse(FileStream);

        for (const auto& Ware : DatasetMetadataJson["dataset"]["ware-versions"])
        {
          for (auto& WareFromFluidx : m_JSONWareset)
          {
            // tolerance singular/plural on ware type
            std::string WareTypePlural = Ware["type"];
            if (WareTypePlural[WareTypePlural.size()-1] != 's')
            {
              WareTypePlural += 's';
            }
            if (WareFromFluidx["id"] == Ware["id"] && WareFromFluidx["type"] == WareTypePlural)
            {
              WareFromFluidx["version"] = Ware["version"];
              if (Ware.contains("git-url"))
              {
                WareFromFluidx["git-url"] = Ware["git-url"];
                m_WareSourceType = "remote";
              }
            }
          }
        }
      }
      catch (openfluid::thirdparty::json::parse_error&)
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                                                  "dataset metadata json format parsing failed");
      }
    }
  }
  else if (m_WaresetSourceType == "json")
  {

    std::ifstream FileStream;
    FileStream.open(SetOption,std::ifstream::in);
    if (!FileStream.is_open())
    {
      openfluid::base::log::warning("Wareset setup", "No json metadata");
    }
    else
    {
      try
      {
        openfluid::thirdparty::json DatasetMetadataJson = openfluid::thirdparty::json::parse(FileStream);

        for (auto& Ware : DatasetMetadataJson)
        {
          // // tolerance singular/plural on ware type
          // std::string WareTypePlural = Ware["type"];
          // if (WareTypePlural[WareTypePlural.size()-1] != 's')
          // {
          //   WareTypePlural += 's';
          // }
          
          if (Ware.contains("git-url"))
          {
            m_WareSourceType = "remote";
          }
          m_JSONWareset.push_back(Ware);
        }
      }
      catch (openfluid::thirdparty::json::parse_error&)
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                                                  "dataset metadata json format parsing failed");
      }
    }
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                                              "Wareset source type not supported: "+m_WaresetSourceType);
  }
}


// =====================================================================
// =====================================================================


int WareSetManager::scaffoldWareset(const std::string& ParentPathStr, const std::string& WareSourceURL, bool IsStrict, 
                                    bool NoBuild, unsigned int JobsNbr, bool BuildTogether)
{
  const openfluid::tools::Path ParentPath(ParentPathStr);
  std::ofstream CallerCmake;
  std::ofstream WaresetCmake;
  bool WaresetCMakeList = true;
  const auto WaresdevPath = ParentPath.fromThis(openfluid::config::WARESDEV_PATH);
  if (WaresetCMakeList)
  {
    //TOIMPL better location for these files?
    CallerCmake.open(WaresdevPath.fromThis("CMakeLists.txt").toGeneric());
    WaresetCmake.open(WaresdevPath.fromThis("CMake."+m_ID+".config").toGeneric());

    CallerCmake << "# WARNING: FILE AUTOGENERATED, DO NOT EDIT IT (it will be erased by next wareset command)\n";
    CallerCmake << "cmake_minimum_required(VERSION 3.20)\n";
    CallerCmake << "PROJECT(" << m_ID << ")\n";
    CallerCmake << "SET(OPENFLUID_MULTI_WARE 1)\n";
    CallerCmake << "INCLUDE(" << "CMake."+m_ID+".config" << ")\n";
    CallerCmake << "OPENFLUID_ADD_MULTIWARE_TARGETS()\n";
    
    CallerCmake.close();
  }
  for (const auto& Ware : m_JSONWareset)
  {
    //   1.1- Checking presence/git
    std::string WareType = Ware["type"];
    std::string WareID = Ware["id"];
    std::string WareVersion = Ware["version"];
    std::cout << "Fetching " << WareID << (WareVersion!="" ? " @ "+WareVersion : " (no version information)");
    std::cout << " [" << WareType << "]" << std::endl;
    const auto WareTypePath = WaresdevPath.fromThis(WareType);
    const auto WarePath = WareTypePath.fromThis(WareID);

    std::string WareKey = WareType.substr(0,3)+"/"+WareID;
    m_WareStatus[WareKey] = {{"fetch", "..."}};

    if (!WarePath.exists())
    {
      if (m_WareSourceType == "remote")
      {
        if (Ware.contains("git-url")) // use git URL if provided
        {
          std::string GitUrl = Ware["git-url"];
          std::cout << "  from Git repository: " << GitUrl << std::endl;
          if (openfluid::waresdev::cloneWare(GitUrl, "git", WareTypePath.toGeneric(), WareID) == 0)
          {
            m_WareStatus[WareKey]["fetch"] = OK_STRING;
          }
          else
          {
            m_WareStatus[WareKey]["fetch"] = KO_STRING;
            THROW_OR_PRINT("Error while cloning ware "+WareID+" from "+GitUrl);
          }
        }
        else if (!m_WaresOrigin.empty()) // fallback on hub if provided
        {
          std::cout << "  from Hub repository: " << WareSourceURL << std::endl;
          if (openfluid::waresdev::cloneWare(WareSourceURL, "hub", WareTypePath.toGeneric(), WareID, WareType) == 0)
          {
            m_WareStatus[WareKey]["fetch"] = OK_STRING;
          }
          else
          {
            m_WareStatus[WareKey]["fetch"] = KO_STRING;
            THROW_OR_PRINT("Error while cloning ware "+WareID);
          }
        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, 
                                                    "Neither git URL nor hub URL from which fetch ware");
        }
      }
      else // copy local folder
      {
        openfluid::tools::Path OriginWarePath(m_WaresOrigin);
        const auto CurrentWareOrigin = OriginWarePath.fromThis(WareType).fromThis(WareID);
        std::cout << "  from local folder: " << CurrentWareOrigin.toGeneric() << std::endl;
        if (CurrentWareOrigin.exists())
        {
          if (!WarePath.makeDirectory())
          {
            THROW_OR_PRINT("Error while creating temporary ware source dir");
          }
          if (openfluid::tools::Filesystem::copyDirectoryContent(
                CurrentWareOrigin.stdPath(), WarePath.stdPath()))
          {
            m_WareStatus[WareKey]["fetch"] = OK_STRING;
          }
          else
          {
            m_WareStatus[WareKey]["fetch"] = KO_STRING;
            THROW_OR_PRINT("Error while copying ware source");
          }
        }
        else
        {
          m_WareStatus[WareKey]["fetch"] = KO_STRING;
          THROW_OR_PRINT("Unable to find given ware in origin folder: " + CurrentWareOrigin.toGeneric());
        }
      }
    }
    else
    {
      std::cout << WarePath.toGeneric() << "already exists" << std::endl;
      m_WareStatus[WareKey]["fetch"] = "skip"; 
    }
    if (WarePath.exists() && WaresetCMakeList)
    {
      // Build centralized wareset CMakeLists.txt
      WaresetCmake << "ADD_SUBDIRECTORY("<< WareType << "/" << WareID << ")\n";
    }
    else
    {
      THROW_OR_PRINT("Ware path not found: " + WarePath.toGeneric());
    }
    
    //   1.2- Checking version/checkout 
    if (!WareVersion.empty())
    {
      openfluid::utils::GitProxy Git;
      openfluid::utils::Process::Command CmdCheckout{
        .Program = Git.getExecutablePath(),
        .Args = {"checkout", WareVersion},
        .WorkDir = WarePath.toGeneric()
      };
      openfluid::utils::Process PCheckout(CmdCheckout);
      if (!PCheckout.run() || !(PCheckout.getExitCode() == 0))//TOIMPL better logging
      {
        for (const auto& l : PCheckout.stdOutLines())
        {
          std::cout << l << std::endl;
        }
        for (const auto& l : PCheckout.stdErrLines())
        {
          std::cout << l << std::endl;
        }
        m_WareStatus[WareKey]["ckout"] = KO_STRING;
        THROW_OR_PRINT("error during ware checkout");
      }
      else
      {
        m_WareStatus[WareKey]["ckout"] = OK_STRING;
      }
    }
    else
    {
      m_WareStatus[WareKey]["ckout"] = "skip";
    }
  }
  freeze(ParentPathStr);

  if (NoBuild)
  {
    for (auto& WareLine : m_WareStatus)
    {
      for (const auto& Step : {"config", "build", "install"})//TOIMPL replace magic strings by vars
      {
        WareLine.second[Step] = "skip";
      }
    }
    return 0;
  }

  if (WaresetCMakeList)
  {
    WaresetCmake.close();
  }
  // 2- Building ware 
  std::cout << "Configuring wares..." << std::endl;
  std::string BuildType = "Release";
  std::string Target = "install";

  std::map<std::string,std::string> Vars = openfluid::waresdev::initializeConfigureVariables();

  Vars["CMAKE_BUILD_TYPE"] = BuildType;
  Vars["WARES_PREFIX_INSTALL_PATH"] = ParentPathStr+"/wares";
  
  //FIXME find a cleaner way, probably useful only for test context
  const char* WareIncludeDirs = std::getenv("WARE_INTERNAL_INCLUDE_DIRS");
  if (WareIncludeDirs != NULL) 
  {
    Vars["WARE_INTERNAL_INCLUDE_DIRS"] = std::string(WareIncludeDirs);
  }
    
  const auto WaresdevPathStr = ParentPath.fromThis(openfluid::config::WARESDEV_PATH).toGeneric();
  
  if (BuildTogether)
  {
    if (!WaresetCMakeList)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "multi-build requires wareset cmakelist");
    }

    const auto BuildPath = openfluid::tools::Path({WaresdevPathStr, 
                                                  openfluid::utils::CMakeProxy::getBuildDir(BuildType)});
    if (BuildPath.isDirectory())
    {
      BuildPath.removeDirectory();
    }
    BuildPath.makeDirectory();
    
    auto CMakeCmd = openfluid::utils::CMakeProxy::getConfigureCommand(BuildPath.toGeneric(),WaresdevPathStr,
                                                                      Vars);

    if (openfluid::utils::Process::system(CMakeCmd) == 0)
    {
      m_WareStatus[MULTIBUILD_STR]["config"] = OK_STRING;
    }
    else
    {
      m_WareStatus[MULTIBUILD_STR]["config"] = KO_STRING;
      THROW_OR_PRINT("Configure failure");
    }
    
    //   2.2- Build ware
    std::cout << "Launching parallel ware build..." << std::endl;

    auto CMakeCmdBuild = openfluid::utils::CMakeProxy::getBuildCommand(BuildPath.toGeneric(),Target,JobsNbr);

    if (openfluid::utils::Process::system(CMakeCmdBuild) == 0)
    {
      m_WareStatus[MULTIBUILD_STR]["build"] = OK_STRING;
      m_WareStatus[MULTIBUILD_STR]["install"] = OK_STRING;
    }
    else
    {
      m_WareStatus[MULTIBUILD_STR]["build"] = KO_STRING;
      m_WareStatus[MULTIBUILD_STR]["install"] = KO_STRING;
      THROW_OR_PRINT("Build failure");
    }          
  }
  else
  {
    for (const auto& Ware : m_JSONWareset)
    {
      //   2.1- Configure ware for installation 
      std::string WareType = Ware["type"];
      std::string WareID = Ware["id"];
      std::string WareKey = WareType.substr(0,3)+"/"+WareID;
      const auto WarePath = ParentPath.fromThis(openfluid::config::WARESDEV_PATH).fromThis(WareType).fromThis(WareID);
      // TOIMPL merge with other configure steps of this file
      const auto BuildPath = openfluid::tools::Path({WarePath.toGeneric(),
                                                    openfluid::utils::CMakeProxy::getBuildDir(BuildType)});
      if (BuildPath.isDirectory())
      {
        BuildPath.removeDirectory();
      }
      BuildPath.makeDirectory();

      auto CMakeCmd = openfluid::utils::CMakeProxy::getConfigureCommand(BuildPath.toGeneric(),WarePath.toGeneric(),
                                                                        Vars);

      if (openfluid::utils::Process::system(CMakeCmd) == 0)
      {
        m_WareStatus[WareKey]["config"] = OK_STRING;
      }
      else
      {
        m_WareStatus[WareKey]["config"] = KO_STRING;
        THROW_OR_PRINT("Configure failure");
      }
      
      //   2.2- Build ware

      auto CMakeCmdBuild = openfluid::utils::CMakeProxy::getBuildCommand(BuildPath.toGeneric(),Target,JobsNbr);

      if (openfluid::utils::Process::system(CMakeCmdBuild) == 0)
      {
        m_WareStatus[WareKey]["build"] = OK_STRING;
        m_WareStatus[WareKey]["install"] = OK_STRING;
      }
      else
      {
        m_WareStatus[WareKey]["build"] = KO_STRING;
        THROW_OR_PRINT("Build failure");
      }          
      // 3- Check if binary valid
      //   3.1- check if found in <userdata>/wares/
      // TODO
      
      //   3.2- ensure validity (via symbols?)
      // TODO
    }
  }
  return 0;
}

#undef THROW_OR_PRINT


// =====================================================================
// =====================================================================


void WareSetManager::freeze(const std::string& FolderPathStr)
{
  // investigates every ware location and asks git version
  const openfluid::tools::Path ParentPath(FolderPathStr);
  std::vector<std::map<std::string, std::string>> FreezeWaresInfo;

  for (const auto& Ware : m_JSONWareset)
  {
    std::string WareType = Ware["type"];
    std::string WareID = Ware["id"];
    std::string WareKey = WareType.substr(0,3)+"/"+WareID;
    std::map<std::string, std::string> FreezeWareInfo = {{"type", WareType}, {"id", WareID}};
    const auto WarePath = ParentPath.fromThis(openfluid::config::WARESDEV_PATH).fromThis(WareType).fromThis(WareID);
    FreezeWareInfo["git-url"] = Ware.value("git-url", "-");//TOIMPL check if functional in Hub case
    openfluid::utils::GitProxy Git;
    try
    {
      FreezeWareInfo["version"] = Git.getCurrentPosition(WarePath.toGeneric(), false);
    }
    catch (openfluid::utils::GitOperationException& E)
    {
      FreezeWareInfo["version"] = "-";
    }
    FreezeWaresInfo.push_back(FreezeWareInfo);
  }
  // saves in freeze file
  openfluid::thirdparty::json JSON = openfluid::thirdparty::json::array();

  for (const auto& Ware : FreezeWaresInfo)
  {
    JSON.push_back(Ware);
  }
  std::ofstream OutFile(openfluid::tools::Path({FolderPathStr, "wareset-lock.json"}).toGeneric());
  OutFile << std::setw(4) << JSON << std::endl;
  OutFile.close();
}


} }  // namespaces

