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
  @file WareTasks.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <algorithm>
#include <fstream>
#include <memory>

#include <openfluid/base/IOListener.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/config.hpp>
#include <openfluid/fluidx/FluidXIO.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/waresdev/WareSrcFactory.hpp>
#include <openfluid/waresdev/GhostsHelpers.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>
#include <openfluid/waresdev/ObserverSignatureSerializer.hpp>
#include <openfluid/waresdev/BuilderextSignatureSerializer.hpp>
#include <openfluid/waresdev/WareSrcMigrator.hpp>
#include <openfluid/waresdev/WareSrcChecker.hpp>
#include <openfluid/waresdev/WareSrcDocalyzer.hpp>
#include <openfluid/waresdev/WareSrcHelpers.hpp>
#include <openfluid/utils/Process.hpp>
#include <openfluid/utils/CMakeProxy.hpp>
#include <openfluid/utils/GitProxy.hpp>
#include <openfluid/utils/FluidHubAPIClient.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/thirdparty/JSON.hpp>
#include <openfluid/config.hpp>

#include "WareTasks.hpp"
#include "DefaultMigrationListener.hpp"
#include "DefaultDocalyzeListener.hpp"


void WareTasks::postWareCreation(const std::string& WarePath) const
{
  std::cout << "Ware successfully created at path " << WarePath << std::endl;
  if (m_Cmd.isOptionActive("set-remote"))
  {
    if (!openfluid::utils::GitProxy::isAvailable() || openfluid::utils::GitProxy::setRemote(WarePath, 
                                                       m_Cmd.getOptionValue("set-remote")) != 0)
    {
      std::cout << "A warning was issued during ware creation, check OpenFLUID log file for details." << std::endl;
      openfluid::base::log::warning("Ware creation", "set-remote failed");
    }
  }
}


// =====================================================================
// =====================================================================


int WareTasks::processCreate() const
{
  if (!m_Cmd.isOptionActive("id"))
  {
    return error("missing ware ID");
  }
  
  const auto ID = m_Cmd.getOptionValue("id");

  if (!openfluid::tools::isValidWareID(ID))
  {
    return error("invalid ware ID");
  }

  const auto ParentPath = (m_Cmd.getOptionValue("parent-path").empty() ? openfluid::tools::Filesystem::currentPath() : 
                                                                          m_Cmd.getOptionValue("parent-path"));

  openfluid::waresdev::WareSrcFactory::Configuration Config;
  Config.WithParamsUI = m_Cmd.isOptionActive("with-paramsui");
  Config.ParamsUIClassName = (m_Cmd.getOptionValue("paramsui-class").empty() ? Config.ParamsUIClassName : 
                                                                               m_Cmd.getOptionValue("paramsui-class"));

  Config.MainClassName = (m_Cmd.getOptionValue("main-class").empty() ? Config.MainClassName : 
                                                                           m_Cmd.getOptionValue("main-class"));

  if (m_Cmd.isOptionActive("from"))
  {
    const std::string ReferenceWarePath = m_Cmd.getOptionValue("from");
    if (!openfluid::tools::Path(ReferenceWarePath).exists() || (
          !openfluid::tools::Path(ReferenceWarePath).fromThis(openfluid::config::WARESDEV_WAREMETA_FILE).exists() && 
          !openfluid::tools::Path(ReferenceWarePath).fromThis(openfluid::config::WARESDEV_WARESHUB_FILE).exists()))
    {
      return error("invalid reference ware path");
    }
    try
    {
      const std::string WarePath = openfluid::waresdev::WareSrcFactory::duplicateWare(ID, ParentPath, 
                                                                                    m_Cmd.getOptionValue("from"), 
                                                                                    m_Cmd.isOptionActive("accept-all"));
      postWareCreation(WarePath);
      return 0;
    }
    catch(const std::exception& e)
    {
      return error(e.what());
    }
  }

  if (!m_Cmd.isOptionActive("type"))
  {
    return error("missing ware type");
  }
  const auto TypeStr = m_Cmd.getOptionValue("type");

  if (TypeStr == "simulator")
  {
    openfluid::ware::SimulatorSignature Sign;
    Sign.ID = ID;
    try
    {
      const std::string WarePath = openfluid::waresdev::WareSrcFactory::createSimulator(Sign,Config,ParentPath);
      postWareCreation(WarePath);
      return 0;
    }
    catch(const openfluid::base::FrameworkException& E)
    {
      return error("problem occurred during creation of simulator sources. " + std::string(E.what()));
    }
  }
  else if (TypeStr == "observer")
  {
    openfluid::ware::ObserverSignature Sign;
    Sign.ID = ID;
    try
    {
      const std::string WarePath = openfluid::waresdev::WareSrcFactory::createObserver(Sign,Config,ParentPath);
      postWareCreation(WarePath);
      return 0;
    }
    catch(const openfluid::base::FrameworkException& E)
    {
      return error("problem occurred during creation of observer sources. " + std::string(E.what()));
    }
  }
  else if (TypeStr == "builderext")
  {
    openfluid::builderext::BuilderExtensionSignature Sign;
    Sign.ID = ID;

    // Mode
    Sign.Mode = openfluid::builderext::ExtensionMode::MODAL;
    auto Mode = m_Cmd.getOptionValue("bext-mode");

    if (Mode == "modeless")
    {
      Sign.Mode = openfluid::builderext::ExtensionMode::MODELESS;
    }
    else if (Mode == "workspace")
    {
      Sign.Mode = openfluid::builderext::ExtensionMode::WORKSPACE;
    }

    // Category
    Sign.Category = openfluid::builderext::ExtensionCategory::OTHER;
    auto Cat = m_Cmd.getOptionValue("bext-category");

    if (Cat == "spatial")
    {
      Sign.Category = openfluid::builderext::ExtensionCategory::SPATIAL;
    }
    else if (Cat == "model")
    {
      Sign.Category = openfluid::builderext::ExtensionCategory::MODEL;
    }
    if (Cat == "results")
    {
      Sign.Category = openfluid::builderext::ExtensionCategory::RESULTS;
    }

    // MenuText
    Sign.MenuText = (m_Cmd.getOptionValue("bext-menutext").empty() ? 
                     "NewExtension" : m_Cmd.getOptionValue("bext-menutext"));

    try
    {
      const std::string WarePath = openfluid::waresdev::WareSrcFactory::createBuilderext(Sign,Config,ParentPath);
      postWareCreation(WarePath);
      return 0;
    }
    catch(const openfluid::base::FrameworkException& E)
    {
      return error("problem occurred during creation of builder-extension sources. " + std::string(E.what()));
    }
  }
  else
  {
    return error("unknown ware type");
  }

  return error();
}


// =====================================================================
// =====================================================================


int WareTasks::processImport() const
{
  const auto ParentPath = (m_Cmd.getOptionValue("parent-path").empty() ? openfluid::tools::Filesystem::currentPath() : 
                                                                          m_Cmd.getOptionValue("parent-path"));

  std::string ID = m_Cmd.getOptionValue("id");
  std::string SourceType = "hub";
  std::string SourceURL = "";
  std::string WareType = "";

  if (m_Cmd.isOptionActive("hub") && m_Cmd.isOptionActive("git"))
  {
    return error("extra remote URL: --git=<url> or --hub[=<url>] must be provided, not both");
  }
  if (m_Cmd.isOptionActive("git"))
  {
    SourceType = "git";
    SourceURL = m_Cmd.getOptionValue("git");
    if (m_Cmd.isOptionActive("id"))
    { 
      ID = m_Cmd.getOptionValue("id");
      if (!openfluid::tools::isValidWareID(ID))
      {
        return error("invalid ware ID");
      }
    }
  }
  else if (m_Cmd.isOptionActive("hub"))
  {
    if (!m_Cmd.isOptionActive("id"))
    {
      return error("missing ware ID");
    }
    ID = m_Cmd.getOptionValue("id");

    if (!openfluid::tools::isValidWareID(ID))
    {
      return error("invalid ware ID");
    }
    if (!m_Cmd.isOptionActive("type"))
    {
      return error("missing ware type");
    }
    SourceURL = m_Cmd.getOptionValue("hub");
    WareType = m_Cmd.getOptionValue("type");
    std::vector<std::string> ValidWareTypes = {"simulator", "observer", "builderext"};
    if(std::find(ValidWareTypes.begin(), ValidWareTypes.end(), WareType) == ValidWareTypes.end())
    {
      return error("invalid ware type");
    }
  }
  else
  {
    return error("remote URL missing: --git=<url> or --hub[=<url>] must be provided");
  }
  if (openfluid::waresdev::cloneWare(SourceURL, SourceType, ParentPath, ID, WareType) != 0)
  {
    return error("Error while cloning ware");
  }
  return success("Ware import successfully completed");
}

// =====================================================================
// =====================================================================
//TOIMPL LOG OR COUT? TODO ADD UNDEF
#define RETURN_ERROR_OR_PRINT(M)                      \
{                                          \
  if (IsStrict) \
  { \
    return error(M); \
  } \
  else \
  { \
    std::cout << M << std::endl; \
  } \
}

int WareTasks::processSetup() const
{
  const auto ParentPathStr = (m_Cmd.getOptionValue("parent-path").empty() ? openfluid::tools::Filesystem::currentPath() : 
                                                                          m_Cmd.getOptionValue("parent-path"));

  std::string WaresOrigin = m_Cmd.getOptionValue("wares-origin");
  std::string SetOption = m_Cmd.getOptionValue("set");

  bool IsStrict = !m_Cmd.isOptionActive("tolerant");
  
  // set source is deduced from name, starting with http means a wareset distant resource
  std::string WaresetSourceType = "dataset";
  std::string WareSourceType = "local";
  //BREAKPOINT
  if (SetOption.empty())
  {
    return error("Set URL or path required");
  }
  if (SetOption.substr(0,4) == "http" || SetOption.find("/") == std::string::npos)
  {
    WaresetSourceType = "hub";
  }
  if (!WaresOrigin.empty() && WaresOrigin.substr(0,4) == "http")
  {
    WareSourceType = "hub";
  }

  std::string ID = "wareset";

  std::string WaresetName;
  std::string WaresetListJson;
  openfluid::thirdparty::json JSONWareset;
  
  //DIRTYCODE split and dispatch ops from here to improve SOLIDity

  if (WaresetSourceType == "hub")
  { 
    //curl http://147.100.175.211:8181/api/wares/sets -H "Accept: application/x.openfluid.fluidhub+json; version=1.0"
    
    // Define source URL
    std::string SourceURL;
    // 1) from set full URL
    if (SetOption.substr(0,4) == "http")
    {
      SourceURL = openfluid::tools::split(SetOption, "/api/").front()+"/api/";
    }
    // 2) from wares origin
    else if (WaresOrigin.substr(0,4) == "http")
    {
      SourceURL = WaresOrigin;
    }
    else
    {
      return error("Hub URL can not be deduced from provided information");
    }

    if (!SetOption.empty() && SetOption.find("/") == std::string::npos)
    {
      ID = SetOption;
    }
    else
    {
      ID = openfluid::tools::split(SetOption, "/").back();
    }
    //TOIMPL else with split on / and get last part
    

    openfluid::utils::FluidHubAPIClient FHClient;
    try
    {
      if (FHClient.connect(SourceURL,false))
      {
        openfluid::tools::TemplateProcessor::Data Waresets;
        WaresetListJson = FHClient.getWareset(ID);
        bool Found = !WaresetListJson.empty();
        if (!Found)
        {
          return error("Wareset not found on hub instance");
        }
      }
      else
      {
        return error("Error during FluidHub connection");
      }
    }
    catch(openfluid::base::FrameworkException& E)
    {
      return error("Wareset request failure ("+std::string(E.what())+")");
    }

    try
    {
      JSONWareset = openfluid::thirdparty::json::parse(WaresetListJson);
    }
    catch (openfluid::thirdparty::json::parse_error&)
    {
      std::cout << "JSON ERROR" << std::endl; // TOIMPL better error
    }
  }
  else if (WaresetSourceType == "dataset")
  {
    //TOIMPL deduce ID from path: either last part or the one before if last is "IN"?

    //TOIMPL check wares listed in given location
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
        JSONWareset.push_back(WareJson);
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
        JSONWareset.push_back(WareJson);
      }
    }
    // Use version data from potential openfluid-dataset.json
    // TOIMPL this function should be in dataset processing class
    std::ifstream FileStream;
    FileStream.open(SetOption+"/openfluid-dataset.json",std::ifstream::in);//TOIMPL better path management
    if (!FileStream.is_open())
    {
      std::cout << "No dataset metadata" << std::endl;//TOIMPL better error management
    }
    else
    {
      try
      {
        openfluid::thirdparty::json DatasetMetadataJson = openfluid::thirdparty::json::parse(FileStream);

        for (const auto& Ware : DatasetMetadataJson["dataset"]["ware-versions"])
        {
          for (auto& WareFromFluidx : JSONWareset)
          {
            if (WareFromFluidx["id"] == Ware["id"] && WareFromFluidx["type"] == Ware["type"])//TOIMPL add tolerance singular/plural on ware type?
            {
              std::cout << "Ware precision found" << std::endl;
              WareFromFluidx["version"] = Ware["version"];
              if (Ware.contains("git-url"))
              {
                WareFromFluidx["git-url"] = Ware["git-url"];
                WareSourceType = "git";
              }
            }
          }
        }
      }
      catch (openfluid::thirdparty::json::parse_error&)
      {
        RETURN_ERROR_OR_PRINT("dataset metadata json format parsing failed");
      }
    }
  }
  // format: [{"id":"export.vars.files.csv","type":"observers","version":"openfluid-2.2"},{"id":"water.atm-surf.rain-su.files","type":"simulators","version":"openfluid-2.2"},{"id":"water.surf.transfer-rs.hayami","type":"simulators","version":"openfluid-2.2"},{"id":"water.surf.transfer-su.hayami","type":"simulators","version":"openfluid-2.2"},{"id":"water.surf-uz.runoff-infiltration.mseytoux","type":"simulators","version":"openfluid-2.2"}]
  
  // 0- Setup userdata
  std::cout << "setup userdata at " << ParentPathStr << std::endl;
  
  openfluid::tools::Path ParentPath(ParentPathStr);
  if (!ParentPath.exists())
  {
    std::error_code ErrorCode;
    if (!ParentPath.makeDirectory("", ErrorCode))
    {
      return error("Userdata creation failed: "+ErrorCode.message());
    }
  }
  else
  {
    return error("Destination already exists");
  }
  openfluid::base::WorkspaceManager::prepareWorkspace(ParentPathStr);
  
  // 1- Fetching wares 
  std::string WareSourceURL = WaresOrigin;

  std::ofstream CallerCmake;
  std::ofstream WaresetCmake;
  CallerCmake.open(ParentPath.fromThis("wares-dev").fromThis("CMakeLists.txt").toGeneric());
  WaresetCmake.open(ParentPath.fromThis("wares-dev").fromThis("CMake."+ID+".config").toGeneric());

  CallerCmake << "# WARNING: FILE AUTOGENERATED, DONT EDIT IT DIRECTLY (it will be erased by next wareset command)\n";
  CallerCmake << "cmake_minimum_required(VERSION 3.20)\n";
  CallerCmake << "PROJECT(" << ID << ")\n";
  CallerCmake << "SET(MULTI_WARE 1)\n";
  CallerCmake << "INCLUDE(" << "CMake."+ID+".config" << ")\n";
  CallerCmake.close();
  for (const auto& Ware : JSONWareset)
  {
    //TODO in most cases, wares can be built in parallel, so handle it here? (redundant with "project cmakelists" strategy proposal that would handle it directly)
    //   1.1- Checking presence/git (TOIMPL allow skip of this step to be independant from network access, using local ware repositories as reference)
    std::string WareType = Ware["type"];
    std::string WareID = Ware["id"];
    std::string WareVersion = Ware["version"];
    std::cout << WareID << ": " << WareVersion << " " << WareType << std::endl;
    const auto WarePath = ParentPath.fromThis("wares-dev").fromThis(WareType).fromThis(WareID);//TOIMPL replace "wares-dev" by var
    if (!WarePath.exists())
    {
      if (WareSourceType == "hub")
      {
        if (openfluid::waresdev::cloneWare(WareSourceURL, "hub", ParentPath.fromThis("wares-dev").fromThis(WareType).toGeneric(), WareID, WareType) != 0)
        {
          RETURN_ERROR_OR_PRINT("Error while cloning ware "+WareID);
        }
      }
      else if (WareSourceType == "git") // TOIMPL merge with previous into source type == remote
      {
        if (Ware.contains("git-url")) // use git URL if provided
        {
          std::string GitUrl = Ware["git-url"];
          if (openfluid::waresdev::cloneWare(GitUrl, "git", ParentPath.fromThis("wares-dev").fromThis(WareType).toGeneric(), WareID) != 0)
          {
            RETURN_ERROR_OR_PRINT("Error while cloning ware "+WareID+" from "+GitUrl);
          }
        }
        else if (!WaresOrigin.empty()) // fallback on hub if provided
        {
          if (openfluid::waresdev::cloneWare(WareSourceURL, "hub", ParentPath.fromThis("wares-dev").fromThis(WareType).toGeneric(), WareID, WareType) != 0)
          {
            RETURN_ERROR_OR_PRINT("Error while cloning ware "+WareID);
          }
        }
        else
        {
          return error("Neither git URL nor hub URL from which fetch ware");
        }
      }
      else // copy local folder
      {
        openfluid::tools::Path OriginWarePath(WaresOrigin);
        if (OriginWarePath.fromThis(WareType).fromThis(WareID).exists())
        {
          if (!WarePath.makeDirectory())
          {
            RETURN_ERROR_OR_PRINT("Error while creating temporary ware source dir");
          }
          if (!openfluid::tools::Filesystem::copyDirectoryContent(OriginWarePath.fromThis(WareType).fromThis(WareID).stdPath(), WarePath.stdPath()))
          {
            RETURN_ERROR_OR_PRINT("Error while copying ware source");
          }
        }
        else
        {
          RETURN_ERROR_OR_PRINT("Unable to find given ware in origin folder: " + OriginWarePath.fromThis(WareType).fromThis(WareID).toGeneric());
        }
      }
    }
    else
    {
      std::cout << WarePath.toGeneric() << "already exists" << std::endl;//TOIMPL better handling of this case
    }
    if (WarePath.exists())
    {
      //1.3- Build centralized wareset CMakeLists.txt
      WaresetCmake << "ADD_SUBDIRECTORY("<< WareType << "/" << WareID << ")\n";
    }
    else
    {
      RETURN_ERROR_OR_PRINT("Ware path not found: " + WarePath.toGeneric());
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
        RETURN_ERROR_OR_PRINT("error during ware checkout");
      }
    }

  }
  WaresetCmake.close();
  if (m_Cmd.isOptionActive("no-build"))
  {
    return success("Wareset setup successfully completed (without ware build)");
  }
  // 2- Building ware 
  std::string BuildType = "Release";
  std::string Target = "install";
  unsigned int JobsNbr = openfluid::base::Environment::getIdealJobsCount();

  if (m_Cmd.isOptionActive("jobs") && !m_Cmd.getOptionValue("jobs").empty())
  {
    JobsNbr = std::stoi(m_Cmd.getOptionValue("jobs"));
  }

  std::map<std::string,std::string> Vars = openfluid::waresdev::initializeConfigureVariables();

  Vars["CMAKE_BUILD_TYPE"] = BuildType;
  Vars["WARES_PREFIX_INSTALL_PATH"] = ParentPathStr+"/wares";
  
  const char* WareIncludeDirs = std::getenv("WARE_INTERNAL_INCLUDE_DIRS"); //FIXME find a cleaner way, probably useful only for test context
  if (WareIncludeDirs != NULL) 
  {
    Vars["WARE_INTERNAL_INCLUDE_DIRS"] = std::string(WareIncludeDirs);
  }
    
  bool BuildTogether = m_Cmd.isOptionActive("multi-builds");
  if (BuildTogether)
  {
    const auto BuildPath = openfluid::tools::Path({ParentPath.fromThis("wares-dev").toGeneric(),openfluid::utils::CMakeProxy::getBuildDir(BuildType)});
    if (BuildPath.isDirectory())
    {
      BuildPath.removeDirectory();
    }
    BuildPath.makeDirectory();
    
    auto CMakeCmd = openfluid::utils::CMakeProxy::getConfigureCommand(BuildPath.toGeneric(),ParentPath.fromThis("wares-dev").toGeneric(),
                                                                      Vars);

    if (openfluid::utils::Process::system(CMakeCmd) != 0)
    {
      RETURN_ERROR_OR_PRINT("Configure failure"); //TOIMPL send error signal at the end if one failed
    }
    
    //   2.2- Build ware
    std::cout << "Trying parallel ware build..." << std::endl;

    auto CMakeCmdBuild = openfluid::utils::CMakeProxy::getBuildCommand(BuildPath.toGeneric(),Target,JobsNbr);

    if (openfluid::utils::Process::system(CMakeCmdBuild) != 0)
    {
      RETURN_ERROR_OR_PRINT("Build failure"); //TOIMPL send error signal at the end if one failed
    }          
  }
  else
  {
    for (const auto& Ware : JSONWareset)
    {
      //   2.1- Configure ware for installation 
      std::string WareType = Ware["type"];
      std::string WareID = Ware["id"];
      const auto WarePath = ParentPath.fromThis("wares-dev").fromThis(WareType).fromThis(WareID);//TOIMPL replace "wares-dev" by var
      // DIRTYCODE merge with other configure steps of this file
      const auto BuildPath = openfluid::tools::Path({WarePath.toGeneric(),openfluid::utils::CMakeProxy::getBuildDir(BuildType)});
      if (BuildPath.isDirectory())
      {
        BuildPath.removeDirectory();
      }
      BuildPath.makeDirectory();

      auto CMakeCmd = openfluid::utils::CMakeProxy::getConfigureCommand(BuildPath.toGeneric(),WarePath.toGeneric(),
                                                                        Vars);

      if (openfluid::utils::Process::system(CMakeCmd) != 0)
      {
        RETURN_ERROR_OR_PRINT("Configure failure"); //TOIMPL send error signal at the end if one failed
      }
      
      //   2.2- Build ware

      auto CMakeCmdBuild = openfluid::utils::CMakeProxy::getBuildCommand(BuildPath.toGeneric(),Target,JobsNbr);

      if (openfluid::utils::Process::system(CMakeCmdBuild) != 0)
      {
        RETURN_ERROR_OR_PRINT("Build failure"); //TOIMPL send error signal at the end if one failed
      }          
      // 3- Check if binary valid
      //   3.1- check if Found in UD/wares/
      // TODO
      
      //   3.2- ensure validity (via symbols?)
      // TODO
    }
  }
  //return error("Error during wareset setup");
  if (WaresetSourceType == "dataset")
  {
    std::cout << "A simulation can be launched using this setup:"<< std::endl;
    std::cout << "OPENFLUID_USERDATA_PATH="+ParentPathStr+" openfluid run "+SetOption+" "+ParentPathStr+"/OUT" << std::endl;//TOIMPL better path management
  }
  if (m_Cmd.isOptionActive("run"))
  {
    if (WaresetSourceType == "dataset")
    {
      // run simulation
      openfluid::utils::Process::Environment Env;
      Env.Vars["OPENFLUID_USERDATA_PATH"] = ParentPathStr;
      for (const auto& V : Env.Vars)
      {
        std::cout << V.first << ":" << V.second << std::endl;
      }
      //TOIMPL find if other var contains openfluid executable 
      int ReturnCode = openfluid::utils::Process::system(openfluid::config::INSTALL_PREFIX+"/bin/openfluid", {"run", SetOption, ParentPathStr+"/OUT", "-q"}, Env);  // TOIMPL better path management
      if (ReturnCode != 0)
      {
        RETURN_ERROR_OR_PRINT("Simulation error (return code: "+std::to_string(ReturnCode)+")");  
      }
    }
    else
    {
      RETURN_ERROR_OR_PRINT("Can't run simulation since set provided is not a dataset");
    }
  }
  return success("Wareset setup successfully completed");
}


// =====================================================================
// =====================================================================


int WareTasks::processConfigure() const
{
  if (!openfluid::utils::CMakeProxy::isAvailable())
  {
    return error("CMake program not found");
  }

  if (!m_Cmd.isOptionActive("src-path") || m_Cmd.getOptionValue("src-path").empty())
  {
    return error("missing or empty source path");
  }

  auto SrcFSP = openfluid::tools::Path(m_Cmd.getOptionValue("src-path"));

  std::string BuildType = openfluid::utils::CMakeProxy::DefaultBuildType;
  if (m_Cmd.isOptionActive("build-type") && !m_Cmd.getOptionValue("build-type").empty())
  {
    BuildType = m_Cmd.getOptionValue("build-type");
  }

  auto BuildFSP = openfluid::tools::Path("");
  if (m_Cmd.isOptionActive("build-path") && !m_Cmd.getOptionValue("build-path").empty())
  {
    BuildFSP = openfluid::tools::Path(m_Cmd.getOptionValue("build-path"));
  }
  else 
  {
    BuildFSP = openfluid::tools::Path({SrcFSP.toGeneric(),openfluid::utils::CMakeProxy::getBuildDir(BuildType)});
  }

  std::string Generator = "";
  if (m_Cmd.isOptionActive("generator") && !m_Cmd.getOptionValue("generator").empty())
  {
    Generator = m_Cmd.getOptionValue("generator");
  }

  if (BuildFSP.isDirectory())
  {
    BuildFSP.removeDirectory();
  }
  BuildFSP.makeDirectory();

  std::map<std::string,std::string> Vars = openfluid::waresdev::initializeConfigureVariables();

  Vars["CMAKE_BUILD_TYPE"] = BuildType;

  auto CMakeCmd = openfluid::utils::CMakeProxy::getConfigureCommand(BuildFSP.toGeneric(),SrcFSP.toGeneric(),
                                                                    Vars,Generator);
  CMakeCmd.Args << m_ThirdPartyArgs;

  return openfluid::utils::Process::system(CMakeCmd);
}


// =====================================================================
// =====================================================================


int WareTasks::processBuild() const
{
  if (!openfluid::utils::CMakeProxy::isAvailable())
  {
    return error("CMake program not found");
  }

  auto BuildFSP = openfluid::tools::Path("");

  if (!m_Cmd.isOptionActive("build-path") || m_Cmd.getOptionValue("build-path").empty())
  {
    if (m_Cmd.isOptionActive("src-path") && !m_Cmd.getOptionValue("src-path").empty())
    {
      if(m_Cmd.isOptionActive("build-type") && !m_Cmd.getOptionValue("build-type").empty())
      {
        BuildFSP = openfluid::tools::Path({m_Cmd.getOptionValue("src-path"),
                                      openfluid::utils::CMakeProxy::getBuildDir(m_Cmd.getOptionValue("build-type"))});
      }
      else
      {
        BuildFSP = openfluid::tools::Path({m_Cmd.getOptionValue("src-path"),
                                           openfluid::utils::CMakeProxy::getBuildDir()});
      }
    }
    else
    {
      return error("missing or empty build path or source path");
    }
  }
  else
  {
    BuildFSP = openfluid::tools::Path(m_Cmd.getOptionValue("build-path"));
  }

  std::string Target = "";
  if (m_Cmd.isOptionActive("with-install"))
  {
    Target = "install";
  }
  else if (m_Cmd.isOptionActive("target"))
  {
    Target = m_Cmd.getOptionValue("target");
  }

  unsigned int JobsNbr = 1;
  if (m_Cmd.isOptionActive("jobs"))
  {
    try
    {
      JobsNbr = stoi(m_Cmd.getOptionValue("jobs"));
    }
    catch (...)
    {
      return error("missing or empty jobs number");
    }
  }


  auto CMakeCmd = openfluid::utils::CMakeProxy::getBuildCommand(BuildFSP.toGeneric(),Target,JobsNbr);
  CMakeCmd.Args << m_ThirdPartyArgs;

  return openfluid::utils::Process::system(CMakeCmd);
}


// =====================================================================
// =====================================================================


int WareTasks::processMigrate() const
{
  if (!m_Cmd.isOptionActive("src-path") || m_Cmd.getOptionValue("src-path").empty())
  {
    return error("missing or empty ware sources path");
  }

  try 
  {
    auto Listener = std::make_unique<DefaultMigrationListener>();
    Listener->setVerbose(m_Cmd.isOptionActive("verbose"));

    // Check if dest-path contains files and dest-path different from src-path
    if(!m_Cmd.isOptionActive("force"))
    {
      if(m_Cmd.isOptionActive("dest-path") && !m_Cmd.getOptionValue("dest-path").empty())
      {
        openfluid::tools::Path DestPath(m_Cmd.getOptionValue("dest-path"));
        if(DestPath.toGeneric() != m_Cmd.getOptionValue("src-path"))
        {
          if(DestPath.exists() && !DestPath.empty())
          {
            return error("Destination path is not empty. Use --force to migrate anyway");
          }
        }
      }
    }

    auto Migrator = openfluid::waresdev::WareSrcMigrator(m_Cmd.getOptionValue("src-path"),
                                                         Listener.get(),
                                                         m_Cmd.getOptionValue("dest-path"));
    Migrator.performMigration();
  }
  catch(const openfluid::base::FrameworkException& E)
  {
    return error(E.what());
  }
  catch(...)
  {
    return error("unknown error");
  }

  return success("Ware migration successfully completed");
}


// =====================================================================
// =====================================================================


std::string getReportStatusAsString(openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus Status)
{
  if (Status == openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::DISABLED)
  {
    return "disabled";
  }
  if (Status == openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::OK)
  {
    return "ok";
  }
  else if (Status == openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::WARNING)
  {
    return "warning";
  }
  else if (Status == openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::ERROR_STATUS)
  {
    return "error";
  }

  return "unknown";
}


// =====================================================================
// =====================================================================


void printCheckReportAsJSON(const openfluid::waresdev::WareSrcChecker::ReportingData& Data)
{
  openfluid::thirdparty::json JSON(openfluid::thirdparty::json::value_t::object);

  for (const auto& Cat : Data.Categories)
  {
    for (const auto& Item : Cat.second.Items)
    {
      JSON[Cat.first][Item.Message] = getReportStatusAsString(Item.Status);
    }
  }

  std::cout << JSON.dump(2)  << std::endl;
}


// =====================================================================
// =====================================================================


void printCheckReportAsText(const openfluid::waresdev::WareSrcChecker::ReportingData& Data, bool FullReport)
{
  for (const auto& Cat : Data.Categories)
  {
    for (const auto& Item : Cat.second.Items)
    {
      if (FullReport || (Item.Status >= openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::WARNING))
      {
        std::cout << Cat.first << "/" << Item.Message << " : " << getReportStatusAsString(Item.Status) << std::endl;
      }
    }
  }
}


// =====================================================================
// =====================================================================


int WareTasks::processCheck() const
{
  if (!m_Cmd.isOptionActive("src-path") || m_Cmd.getOptionValue("src-path").empty())
  {
    return error("missing or empty ware sources path");
  }

  std::string Format = "text";
  if (m_Cmd.isOptionActive("format"))
  {
    Format = m_Cmd.getOptionValue("format");
  }
  if (Format != "text" && Format != "json")
  {
    return error("unknown format");
  }

  openfluid::waresdev::WareSrcChecker::ChecksList Ignored;

  if (m_Cmd.isOptionActive("ignore") && !m_Cmd.getOptionValue("ignore").empty())
  {
    Ignored = openfluid::tools::split(m_Cmd.getOptionValue("ignore"),",");
  }

  try 
  {
    auto Checker = openfluid::waresdev::WareSrcChecker(m_Cmd.getOptionValue("src-path"),Ignored);
    auto Report = Checker.performCheck(m_Cmd.isOptionActive("pedantic"));

    if (Format =="json")
    {
      printCheckReportAsJSON(Report);
    }
    else
    {
      printCheckReportAsText(Report,m_Cmd.isOptionActive("full-report"));
    }

    bool IsAccepted = 
      (Report.getStatus() <= (m_Cmd.isOptionActive("warnings-as-failures") ? 
                              openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::OK : 
                              openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::WARNING));

    return (IsAccepted ? success("No issue") : error("failed checks"));
  }
  catch(const openfluid::base::FrameworkException& E)
  {
    return error(E.what());
  }
  catch(...)
  {
    return error("unknown error");
  }

  return success("No issue");
}


// =====================================================================
// =====================================================================


int WareTasks::processDocalyze() const
{
  if (!m_Cmd.isOptionActive("src-path") || m_Cmd.getOptionValue("src-path").empty())
  {
    return error("missing or empty ware sources path");
  }

  std::vector<std::string> InputFormats = {};
  if (m_Cmd.isOptionActive("input-format"))
  {
    InputFormats.push_back(m_Cmd.getOptionValue("input-format"));
  }
  if (!InputFormats.empty() && InputFormats.front() != "tex" && InputFormats.front() != "rmd" && 
      InputFormats.front() != "md" && InputFormats.front() != "readme")
  {
    if(InputFormats.front() == "auto")
    {
      InputFormats = {};
    }
    else 
    {
      return error("unknown input format for documentation sources");
    }
  }

  std::string OutputPath = "";
  if (m_Cmd.isOptionActive("output-path"))
  {
    OutputPath = m_Cmd.getOptionValue("output-path");
  }

  try 
  {
    auto Listener = std::make_unique<DefaultDocalyzeListener>();
    Listener->setVerbose(true); // TODO manage this consistently with ware migration command line
    auto Docalyzer = openfluid::waresdev::WareSrcDocalyzer(m_Cmd.getOptionValue("src-path"),OutputPath,Listener.get(),
                                                           InputFormats);
    Docalyzer.performDocalyze(m_Cmd.isOptionActive("keep-data"),m_Cmd.isOptionActive("include-empty-fields"));
  }
  catch(const openfluid::base::FrameworkException& E)
  {
    if(std::string(E.what()).find(openfluid::config::ERROR_MESSAGE_MISSING_R_PACKAGE) != std::string::npos)
    {
      return errorByCode(openfluid::config::RETURN_CODE_DEPENDENCY_ISSUE, E.what());
    }
    else 
    {
      return error(E.what());
    }
    
  }
  catch(...)
  {
    return error("unknown error");
  }

  return success("Ware docalyze successfully completed");
}


// =====================================================================
// =====================================================================


int WareTasks::processInfo2Build() const
{
  if (!m_Cmd.isOptionActive("src-path") || m_Cmd.getOptionValue("src-path").empty())
  {
    return error("missing or empty source path");
  }

  if (!m_Cmd.isOptionActive("dest-path") || m_Cmd.getOptionValue("dest-path").empty())
  {
    return error("missing or empty destination path");
  }

  openfluid::tools::Path SrcFilehObj({m_Cmd.getOptionValue("src-path"),openfluid::config::WARESDEV_WAREMETA_FILE});
  openfluid::tools::Path DestDirObj(m_Cmd.getOptionValue("dest-path"));

  if (SrcFilehObj.isFile())
  {
    const auto Type = openfluid::waresdev::detectWareType(SrcFilehObj.toGeneric());

    if (Type == openfluid::ware::WareType::SIMULATOR)
    {
      const auto Sign = openfluid::waresdev::SimulatorSignatureSerializer().readFromJSONFile(SrcFilehObj.toGeneric());
      openfluid::waresdev::SimulatorSignatureSerializer().writeToBuildFiles(Sign,DestDirObj.toGeneric());
    }
    else if (Type == openfluid::ware::WareType::OBSERVER)
    {
      const auto Sign = openfluid::waresdev::ObserverSignatureSerializer().readFromJSONFile(SrcFilehObj.toGeneric());
      openfluid::waresdev::ObserverSignatureSerializer().writeToBuildFiles(Sign,DestDirObj.toGeneric());
    }
    else if (Type == openfluid::ware::WareType::BUILDEREXT)
    {      
      const auto Sign = openfluid::waresdev::BuilderextSignatureSerializer().readFromJSONFile(SrcFilehObj.toGeneric());
      openfluid::waresdev::BuilderextSignatureSerializer().writeToBuildFiles(Sign,DestDirObj.toGeneric());
    }
    else
    {
      return error("unable to detect ware type in " + 
                   openfluid::config::WARESDEV_WAREMETA_FILE + " file");
    }
  }
  else
  {
    return error(openfluid::config::WARESDEV_WAREMETA_FILE + " file not found in source directory");
  }

  return success("Ware info2build successfully completed");
}


// =====================================================================
// =====================================================================


int WareTasks::processPurge() const
{
  std::string WarePath = m_Cmd.getOptionValue("src-path");

  if (!m_Cmd.isOptionActive("src-path") || WarePath.empty())
  {
    return error("missing or empty ware sources path");
  }

  if (!m_Cmd.isOptionActive("build-type") && !m_Cmd.isOptionActive("build-version"))
  {
    return error("missing build type or build version to purge");
  }

  std::string ReleaseModeStr = "release";
  std::string DebugModeStr = "debug";
  std::string CurrentVersionStr = "current";
  std::string OtherVersionsStr = "other";

  bool ReleaseMode = true;
  bool DebugMode = true;
  bool CurrentVersion = true;
  bool OtherVersions = true;

  if(m_Cmd.isOptionActive("build-type") && !m_Cmd.getOptionValue("build-type").empty())
  {
    std::vector<std::string> BuildTypes = openfluid::tools::split(m_Cmd.getOptionValue("build-type"), "+");
    bool HasReleaseMode = false;
    bool HasDebugMode = false;
    for(auto& BuildType : BuildTypes)
    {
      BuildType = openfluid::tools::toLowerCase(BuildType);
      if(BuildType != ReleaseModeStr && BuildType != DebugModeStr)
      {
        return error("wrong build type");
      }
      if(BuildType == ReleaseModeStr)
      {
        HasReleaseMode = true;
      }
      if(BuildType == DebugModeStr)
      {
        HasDebugMode = true;
      }
    }

    ReleaseMode = HasReleaseMode;
    DebugMode = HasDebugMode;
    if(!ReleaseMode && !DebugMode)
    {
      return error("wrong build type");
    }
  }

  if(m_Cmd.isOptionActive("build-version") && !m_Cmd.getOptionValue("build-version").empty())
  {
    std::vector<std::string> BuildVersions = openfluid::tools::split(m_Cmd.getOptionValue("build-version"), "+");
    bool HasCurrentVersion = false;
    bool HasOtherVersions = false;
    for(auto& BuildVersion : BuildVersions)
    {
      BuildVersion = openfluid::tools::toLowerCase(BuildVersion);
      if(BuildVersion != CurrentVersionStr && BuildVersion != OtherVersionsStr)
      {
        return error("wrong build version");
      }
      if(BuildVersion == CurrentVersionStr)
      {
        HasCurrentVersion = true;
      }
      if(BuildVersion == OtherVersionsStr)
      {
        HasOtherVersions = true;
      }
    }
    
    CurrentVersion = HasCurrentVersion;
    OtherVersions = HasOtherVersions;
    if(!CurrentVersion && !OtherVersions)
    {
      return error("wrong build version");
    }
  }

  openfluid::waresdev::WarePurgeHandler PurgeHandler(CurrentVersion, OtherVersions, ReleaseMode, DebugMode);

  auto WriteMessageFunc = [this](std::string Msg, std::string LevelInfo) -> void
  {
    std::string Color = LevelInfo == "Error" ? "red" : "green";
    LevelInfo == "Error" ? openfluid::tools::Console::setErrorColor() : openfluid::tools::Console::setOKColor();
    std::cout << Msg << std::endl;
    openfluid::tools::Console::resetAttributes();
  };

  PurgeHandler.purge(WarePath, WriteMessageFunc, [](bool /*Status*/) {});

  return success("Ware purge successfully completed");
}


// =====================================================================
// =====================================================================


int WareTasks::process() const
{
  if (m_Cmd.getName() == "create-ware")
  {
    return processCreate();
  }
  else if (m_Cmd.getName() == "import-ware")
  {
    return processImport();
  }
  else if (m_Cmd.getName() == "setup-wareset")
  {
    return processSetup();
  }
  else if (m_Cmd.getName() == "check")
  {
    return processCheck();
  }
  else if (m_Cmd.getName() == "migrate-ware")
  {
    return processMigrate();
  }
  else if (m_Cmd.getName() == "docalyze")
  {
    return processDocalyze();
  }
  else if (m_Cmd.getName() == "configure")
  {
    return processConfigure();
  }
  else if (m_Cmd.getName() == "build")
  {
    return processBuild();
  }
  else if (m_Cmd.getName() == "purge")
  {
    return processPurge();
  }
  else if (m_Cmd.getName() == "info2build")
  {
    return processInfo2Build();
  }
  else if (m_Cmd.getName() == "migrate-ghostsim")
  {
    if (!m_Cmd.isOptionActive("id"))
    {
      return error("missing ware ID");
    }

    const auto ID = m_Cmd.getOptionValue("id");
    const auto ParentPath = (m_Cmd.getOptionValue("parent-path").empty() ? 
                             openfluid::tools::Filesystem::currentPath() : m_Cmd.getOptionValue("parent-path"));

    if (openfluid::waresdev::migrateGhostSimulator(ParentPath,ID))
    {
      return 0;
    }
    return error();
  }
  
  return unknownCommand();
}

