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
#include <openfluid/utils/Process.hpp>
#include <openfluid/waresdev/WareSrcHelpers.hpp>

#include "WareSetManager.hpp"


namespace openfluid { namespace waresdev {


void logAndPrint(std::string M, unsigned int& Problems)
{
  openfluid::tools::Console::setErrorColor();
  std::cout << M;
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;
  openfluid::base::log::error("Wareset setup", M);
  Problems++;
}

void throwOrPrint(bool IsStrict, std::string M, unsigned int& Problems)
{
  if (IsStrict)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, M);
  }
  else
  {
    logAndPrint(M, Problems);
  }
}


// =====================================================================
// =====================================================================


void WareSetManager::displayStatus()
{
  // Print status table
  std::cout << "== Wareset status" << std::endl;
  std::vector OrderedSteps = {"fetch", "ckout", "config", "build", "install", "run"};
  if (m_IsPreconfigureCommand)
  {
    OrderedSteps = {"fetch", "ckout", "custom", "config", "build", "install", "run"};
  }
  std::cout << "                           ";
  for (const std::string Step : OrderedSteps)
  {
    std::cout << "\t" << Step;
  }
  std::cout << std::endl;
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


// =====================================================================
// =====================================================================


WareSetManager::WareSetManager(const std::string& WareSourceType, const std::string& WaresetSourceType, 
                               const std::string& SetOption, const std::string& WaresOrigin, std::string& ID) : 
  m_WareSourceType(WareSourceType), m_WaresetSourceType(WaresetSourceType), m_ID(ID), m_WaresOrigin(WaresOrigin), 
  m_IsPreconfigureCommand(false)
{
  m_Problems = 0;
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
    //TODO deduce m_ID from path: either last part or the one before if last is "IN"?
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
    // Use version data from potential wareset-setup.json
    std::ifstream FileStream;
    FileStream.open(openfluid::tools::Filesystem::joinPath({SetOption, "wareset-setup.json"}),std::ifstream::in);
    //TODO make filename var
    if (!FileStream.is_open())
    {
      openfluid::base::log::warning("Wareset setup", "No wareset metadata");
    }
    else
    {
      std::string ConfigureOptionKey = "configure-options";
      std::string CMakeContentKey = "cmake-content";
      try
      {
        openfluid::thirdparty::json WaresetJson = openfluid::thirdparty::json::parse(FileStream);
        
        // First check generic ops
        for (const auto& Ware : WaresetJson["wares-setup"]) //TODO externalise function for solo-ware level?
        {
          if ( Ware["id"] == "*")
          {
            for (auto& WareFromFluidx : m_JSONWareset)
            {
              for (const std::string& Key : {(std::string)"version", (std::string)"pre-configure-commands", 
                                             CMakeContentKey})
              {
                if (Ware.contains(Key))
                {
                  WareFromFluidx[Key] = Ware[Key];
                }
                if ((std::string)Key == "pre-configure-commands")
                {
                  m_IsPreconfigureCommand = true;
                }
              }
              if (Ware.contains(ConfigureOptionKey))
              {
                if (!WareFromFluidx.contains(ConfigureOptionKey))
                {
                  WareFromFluidx[ConfigureOptionKey] = thirdparty::json::object();
                }
                for (const auto& Option : Ware[ConfigureOptionKey].items())
                {
                  WareFromFluidx[ConfigureOptionKey][Option.key()] = Option.value();
                }
              }
            }
          }
        }


        for (const auto& Ware : WaresetJson["wares-setup"])
        {
          if ( Ware["id"] != "*")
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
                if (Ware.contains("git-url"))
                {
                  WareFromFluidx["git-url"] = Ware["git-url"];
                  m_WareSourceType = "remote";
                }
                if (Ware.contains(ConfigureOptionKey))
                {
                  if (!WareFromFluidx.contains(ConfigureOptionKey))
                  {
                    WareFromFluidx[ConfigureOptionKey] = thirdparty::json::object();
                  }
                  for (const auto& Option : Ware[ConfigureOptionKey].items())
                  {
                    WareFromFluidx[ConfigureOptionKey][Option.key()] = Option.value();
                  }
                }
                if (Ware.contains(CMakeContentKey))
                {
                  if (!WareFromFluidx.contains(CMakeContentKey))
                  {
                    WareFromFluidx[CMakeContentKey] = Ware[CMakeContentKey];
                  }
                  else
                  {
                    WareFromFluidx[CMakeContentKey] += Ware[CMakeContentKey];
                  }
                }
               
                if (Ware.contains("version"))
                {
                  WareFromFluidx["version"] = Ware["version"];
                }
                std::string PreconfigString = "pre-configure-commands";
                if (Ware.contains(PreconfigString))
                {
                  if (!WareFromFluidx.contains(PreconfigString))
                  {
                    WareFromFluidx[PreconfigString] = thirdparty::json::array();
                  }
                  for (const auto& Command : Ware[PreconfigString])
                  {
                    WareFromFluidx[PreconfigString].push_back(Command);
                  }
                  m_IsPreconfigureCommand = true;
                }
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
  else if (m_WaresetSourceType == "lockfile")
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
        openfluid::thirdparty::json WaresetJson = openfluid::thirdparty::json::parse(FileStream);

        for (auto& Ware : WaresetJson)
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
  else if (m_WaresetSourceType == "listfile")
  {
    std::ifstream FileStream;
    FileStream.open(SetOption,std::ifstream::in);
    if (!FileStream.is_open())
    {
      openfluid::base::log::warning("Wareset setup", "No wareset file");
    }
    else
    {
      std::string Line;
      while(getline(FileStream, Line))
      {
        std::string LocalPath = "";
        std::vector<std::string> LS = openfluid::tools::split(Line, " ");
        std::string FullRepoURL = LS[0];
        if (LS.size() > 1)
        {
          LocalPath = LS[1]; // TODO refinement possible here: 
          // local path not used for now since we follow userdata file structure
        }
        std::string GitVersion = "";
        std::vector<std::string> GitData = openfluid::tools::split(FullRepoURL, "#");
        std::string GitURL = GitData[0];
        if (GitData.size() > 1)
        {
          GitVersion = GitData[1];
        }

        openfluid::thirdparty::json WareJson = openfluid::thirdparty::json::object();
        WareJson["type"] = "";
        for (const std::string Type : {"simulators", "observers", "builderexts"})
        {
          if (GitURL.find("/wares/"+Type) != std::string::npos)
          {
            WareJson["type"] = Type;
            break;
          }
        }
        WareJson["id"] = openfluid::tools::split(GitURL, "/").back();
        WareJson["version"] = GitVersion;
        WareJson["git-url"] = GitURL;
        std::string WareKey = std::string(WareJson["type"])+"/"+std::string(WareJson["id"]);
    
        m_JSONWareset.push_back(WareJson);
    
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


int WareSetManager::scaffoldWareset(const std::string& UserdataPathStr, 
                                    const std::string& WareSourceURL, 
                                    const std::string& WorkspaceName, bool IsStrict, 
                                    bool NoBuild, unsigned int JobsNbr, bool BuildTogether, bool Overwrite)
{
  const openfluid::tools::Path ParentPath(UserdataPathStr);
  std::ofstream CallerCmake;
  std::ofstream WaresetCmake;
  bool WaresetCMakeList = true;
  const auto WaresdevPath = ParentPath.fromThis(WorkspaceName).fromThis(
    openfluid::config::WARESDEV_PATH);
  if (WaresetCMakeList)
  {
    // TODO improve project CMake file structure
    CallerCmake.open(WaresdevPath.fromThis("CMakeLists.txt").toGeneric());
    WaresetCmake.open(WaresdevPath.fromThis("CMake."+m_ID+".config").toGeneric());

    CallerCmake << "# WARNING: FILE AUTOGENERATED, DO NOT EDIT IT (it will be erased by next wareset command)\n";
    CallerCmake << "cmake_minimum_required(VERSION 3.20)\n";
    CallerCmake << "PROJECT(" << m_ID << ")\n";
    CallerCmake << "ENABLE_TESTING()\n";
    CallerCmake << "SET(OPENFLUID_MULTI_WARE 1)\n";
    CallerCmake << "INCLUDE(" << "CMake."+m_ID+".config" << ")\n";
    CallerCmake << "OPENFLUID_ADD_MULTIWARE_TARGETS()\n";
    
    CallerCmake.close();

  }
  for (auto& Ware : m_JSONWareset)
  {
    // --------------------------------------
    //   1.1- Checking presence/git
    // --------------------------------------

    std::string WareType = Ware["type"];
    std::string WareID = Ware["id"];
    std::string WareVersion = Ware["version"];
    if (WareVersion == "-")
    {
      WareVersion = "";
    }
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
            throwOrPrint(IsStrict, "Error while cloning ware "+WareID+" from "+GitUrl, m_Problems);
          }
        }
        else if (!m_WaresOrigin.empty()) // fallback on hub if provided
        {
          std::cout << "  from Hub repository: " << WareSourceURL << std::endl;
          Ware["git-url"] = openfluid::waresdev::buildHubWareURL(WareSourceURL, WareID, WareType);
          if (openfluid::waresdev::cloneWare(WareSourceURL, "hub", WareTypePath.toGeneric(), WareID, WareType) == 0)
          {
            m_WareStatus[WareKey]["fetch"] = OK_STRING;
          }
          else
          {
            m_WareStatus[WareKey]["fetch"] = KO_STRING;
            throwOrPrint(IsStrict, "Error while cloning ware "+WareID, m_Problems);
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
            throwOrPrint(IsStrict, "Error while creating temporary ware source dir", m_Problems);
          }
          if (openfluid::tools::Filesystem::copyDirectoryContent(
                CurrentWareOrigin.stdPath(), WarePath.stdPath()))
          {
            m_WareStatus[WareKey]["fetch"] = OK_STRING;
          }
          else
          {
            m_WareStatus[WareKey]["fetch"] = KO_STRING;
            throwOrPrint(IsStrict, "Error while copying ware source", m_Problems);
          }
        }
        else
        {
          m_WareStatus[WareKey]["fetch"] = KO_STRING;
          throwOrPrint(IsStrict, "Unable to find given ware in origin folder: " + CurrentWareOrigin.toGeneric(), 
                       m_Problems);
        }
      }
    }
    else
    {
      throwOrPrint(!Overwrite, WarePath.toGeneric() + " already exists", 
                       m_Problems);
      m_WareStatus[WareKey]["fetch"] = "skip"; 
    }
    if (WarePath.exists() && WaresetCMakeList)
    {
      WaresetCmake << "ADD_SUBDIRECTORY("<< WareType << "/" << WareID << ")\n";
    }
    else
    {
      throwOrPrint(IsStrict, "Ware path not found: " + WarePath.toGeneric(), m_Problems);
    }
    
    // --------------------------------------
    //   1.2- Checking version/checkout 
    // --------------------------------------

    if (!WarePath.exists())
    {
      // can't work without ware path
    }
    else if (!WareVersion.empty())
    {
      openfluid::utils::GitProxy Git;
      openfluid::utils::Process::Command CmdCheckout{
        .Program = Git.getExecutablePath(),
        .Args = {"checkout", WareVersion},
        .WorkDir = WarePath.toGeneric()
      };
      openfluid::utils::Process PCheckout(CmdCheckout);
      if (!PCheckout.run() || !(PCheckout.getExitCode() == 0))
      {
        m_WareStatus[WareKey]["ckout"] = KO_STRING;
        for (const auto& l : PCheckout.stdOutLines())
        {
          logAndPrint(l, m_Problems);
        }
        for (const auto& l : PCheckout.stdErrLines())
        {
          logAndPrint(l, m_Problems);
        }
        throwOrPrint(IsStrict, "error during ware checkout", m_Problems);
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
  freeze(UserdataPathStr);

  if (WaresetCMakeList)
  {
    WaresetCmake.close();
  }

  // 1.3 Custom pre-configure commands

  for (const auto& Ware : m_JSONWareset)
  {
    if (Ware.contains("pre-configure-commands"))
    {
      // Convert template variables:
      for (const auto& CommandJson : Ware["pre-configure-commands"])
      {
        std::string PreConfigureCommand = CommandJson["program"];
        // %%OF%% //OF only replaced as command
        PreConfigureCommand = openfluid::tools::replace(PreConfigureCommand, "%%OF%%", 
          openfluid::tools::Path({openfluid::base::Environment::getInstallPrefix(),
                                                  openfluid::config::INSTALL_BIN_PATH,
                                                  openfluid::base::Environment::getBinName()}).toNative());
        std::string WareType = Ware["type"];
        std::string WareID = Ware["id"];
        std::string WareKey = WareType.substr(0,3)+"/"+WareID;
        const auto WarePath = WaresdevPath.fromThis(WareType).fromThis(WareID);
        std::vector<std::string> Args;
        // Placeholders strings:
        // %%BuildParent%% for ware or waresdev folder depending on build context
        // %%Ware%% for the current ware root folder
        // %%Waresdev%% for the current waresdev folder
        // %%??%% will try to convert to environment value if exists
        if (CommandJson.contains("args"))
        {
          for (const auto& Arg : CommandJson["args"])
          {
            std::string ArgProcessed;
            if (BuildTogether)
            {
              ArgProcessed = openfluid::tools::replace(Arg, "%%BuildParent%%", "%%Waresdev%%");
            }
            else
            {
              ArgProcessed = openfluid::tools::replace(Arg, "%%BuildParent%%", "%%Ware%%");
            }
            ArgProcessed = openfluid::tools::replace(ArgProcessed, "%%Ware%%", WarePath.toGeneric());
            ArgProcessed = openfluid::tools::replace(ArgProcessed, "%%Waresdev%%", WaresdevPath.toGeneric());

            // check for other vars
            std::regex WordsRegex("%%(.*?)%%");
            auto WordsBegin = std::sregex_iterator(ArgProcessed.begin(), ArgProcessed.end(), WordsRegex);
                        
            for (std::sregex_iterator i = WordsBegin; i != std::sregex_iterator(); ++i)
            {
              std::string MatchStr = (*i).str(1);
              // check if env var found
                char* EnvVarValueChar = std::getenv(MatchStr.c_str());
                if (EnvVarValueChar != NULL)
                {
                  std::string EnvVarValue = std::string(EnvVarValueChar);
                  ArgProcessed = openfluid::tools::replace(ArgProcessed, (*i).str(), EnvVarValue);
                }
                else
                {
                  std::cout << "Env var detected between '%%' but not found: " << MatchStr << std::endl;
                }
            }

            Args.push_back(ArgProcessed);
          }
        }
        

        openfluid::utils::Process::Environment Env;
        std::cout << "Ware " << WareID << ": Triggering custom command " << PreConfigureCommand << \
                     " " << openfluid::tools::join(Args, " ") << std::endl;
        int ReturnCode = openfluid::utils::Process::system(PreConfigureCommand, Args, Env);
        if (ReturnCode == 0)
        {
          m_WareStatus[WareKey]["custom"] = OK_STRING;
          std::cout << "[OK]" << std::endl;
        }
        else
        {
          m_WareStatus[WareKey]["custom"] = KO_STRING;//TODO keep worst of all commands
          throwOrPrint(IsStrict, "Custom command failed", m_Problems);
        }
      }
    }
  }

  // --------------------------------------
  // 2- Configuring / building ware 
  // --------------------------------------

  std::cout << "Configuring wares..." << std::endl;
  std::string BuildType = "Release";
  std::string Target = "install";

  std::map<std::string,std::string> Vars = openfluid::waresdev::initializeConfigureVariables();

  Vars["CMAKE_BUILD_TYPE"] = BuildType;
  Vars["WARES_PREFIX_INSTALL_PATH"] = UserdataPathStr+"/wares";
  
  //FIXME find a cleaner way, probably useful only for test context
  const char* WareIncludeDirs = std::getenv("WARE_INTERNAL_INCLUDE_DIRS");
  if (WareIncludeDirs != NULL) 
  {
    Vars["WARE_INTERNAL_INCLUDE_DIRS"] = std::string(WareIncludeDirs);
  }


  for (const auto& Ware : m_JSONWareset)
  {
    //TODO FIX CONTAMINATING OTHER WARES
    if (Ware.contains("configure-options") || Ware.contains("cmake-content"))
    {
      Vars["CONTEXT_VARS"] = "ON";

      std::ofstream WareCmakeLists;
      std::ofstream WareCmakeContextual;
      
      std::string WareType = Ware["type"];
      std::string WareID = Ware["id"];
      const auto WarePath = WaresdevPath.fromThis(WareType).fromThis(WareID);
      // 1. check if "CMake.in.contextual.config" exists
      const auto ContextualConfigFile = WarePath.fromThis("CMake.in.contextual.config");
      if (ContextualConfigFile.exists())
      {
        throwOrPrint(!Overwrite, "CMake.in.contextual.config file already exists", m_Problems);
      }


      // 2. add line in CMakeLists.txt if not already there
      std::ifstream ReadingFile(WarePath.fromThis("CMakeLists.txt").toGeneric());

      // String to store each line of the file.
      std::string Line;
      std::string RewrittenFile;

      bool LineAdded = false;
      std::string ContextualLine = "INCLUDE(CMake.in.contextual.config OPTIONAL)";
      std::string ContextualBlock = "# Optional file dedicated for automatic cmake configuration, "
        "loaded if CONTEXT_VARS is ON\n"
        "OPTION(CONTEXT_VARS \"Using contextual cmake variables\" OFF)\n"
        "IF (CONTEXT_VARS)\n"
        "  MESSAGE(STATUS \"Using contextual variables (warning: may be overwritten by subsequent cmake line)\")\n"
        "  INCLUDE(CMake.in.contextual.config OPTIONAL)\n"
        "ENDIF()\n"
        "# For manual adjustments use a CMake.in.local.config file and include it here\n";
      if (ReadingFile.is_open())
      {
        while (getline(ReadingFile, Line))
        {
          RewrittenFile += Line+"\n";
          if (!LineAdded && Line.find(ContextualLine) != Line.npos)
          {
            LineAdded = true;
          }
        }
      }
      if (!LineAdded)
      {
        RewrittenFile = ContextualBlock+"\n" + RewrittenFile;
      }

      WareCmakeLists.open(WarePath.fromThis("CMakeLists.txt").toGeneric());
      WareCmakeLists << RewrittenFile;

      WareCmakeLists.close();
      // 3. write contextual config file
      WareCmakeContextual.open(ContextualConfigFile.toGeneric());
      WareCmakeContextual << "# " << m_ID << " context\n";

      for (const auto& Option : Ware["configure-options"].items())
      {
        WareCmakeContextual << "SET(" << Option.key() << " " << Option.value() << ")\n";
      }

      if (Ware.contains("cmake-content"))
      {
        WareCmakeContextual << "\n# Custom cmake content from wareset\n" << Ware["cmake-content"].get<std::string>() \
                            << "\n";
      }
      WareCmakeContextual.close();
    }
    else
    {
        Vars.erase("CONTEXT_VARS");
    }
  }

  if (NoBuild)
  {
    for (auto& WareLine : m_WareStatus)
    {
      for (const auto& Step : {"config", "build", "install"})
      {
        WareLine.second[Step] = "skip";
      }
    }
    return 0;
  }
  const auto WaresdevPathStr = WaresdevPath.toGeneric();
  
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
      throwOrPrint(IsStrict, "Configure failure", m_Problems);
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
      throwOrPrint(IsStrict, "Build failure", m_Problems);
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
      const auto WarePath = WaresdevPath.fromThis(WareType).fromThis(WareID);

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
        throwOrPrint(IsStrict, "Configure failure", m_Problems);
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
        throwOrPrint(IsStrict, "Build failure", m_Problems);
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


// =====================================================================
// =====================================================================


void WareSetManager::freeze(const std::string& FolderPathStr)
{
  // investigates every ware location and asks git version
  const openfluid::tools::Path ParentPath(FolderPathStr);

  openfluid::thirdparty::json FreezeWaresInfo = openfluid::thirdparty::json::array();
  for (const auto& Ware : m_JSONWareset)
  {
    std::string WareType = Ware["type"];
    std::string WareID = Ware["id"];
    std::string WareKey = WareType.substr(0,3)+"/"+WareID;
    openfluid::thirdparty::json FreezeWareInfo = openfluid::thirdparty::json::object();
    FreezeWareInfo["type"] = WareType;
    FreezeWareInfo["id"] = WareID;

    const auto WarePath = ParentPath.fromThis(openfluid::config::WORKSPACE_PATH).fromThis(
      openfluid::config::WARESDEV_PATH).fromThis(WareType).fromThis(WareID);
    FreezeWareInfo["git-url"] = Ware.value("git-url", "-");
    openfluid::utils::GitProxy Git;
    try
    {
      FreezeWareInfo["version"] = Git.getCurrentPosition(WarePath.toGeneric(), 
                                    openfluid::utils::GitProxy::GIT_POSITION::COMMIT);
    }
    catch (openfluid::utils::GitOperationException& E)
    {
      FreezeWareInfo["version"] = "-";
    }
    // Add all other information for reproductibility
    for (const auto& Key : {"configure-options", "pre-configure-commands", "cmake-content"})
    {
      if (Ware.contains(Key))
      {
        FreezeWareInfo[Key] = Ware[Key];
      }
    }

    FreezeWaresInfo.push_back(FreezeWareInfo);
  }
  // saves in freeze file

  std::string LockFilePath = openfluid::tools::Path({FolderPathStr, "wareset-lock.json"}).toGeneric();
  std::ofstream OutFile(LockFilePath);
  OutFile << std::setw(4) << FreezeWaresInfo << std::endl;
  OutFile.close();
  std::cout << "Wareset lock file written at " << LockFilePath << std::endl;
}


} }  // namespaces

