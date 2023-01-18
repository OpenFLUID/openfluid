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
  @file WareTasks.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#include <memory>

#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/waresdev/WareSrcFactory.hpp>
#include <openfluid/waresdev/GhostsHelpers.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>
#include <openfluid/waresdev/ObserverSignatureSerializer.hpp>
#include <openfluid/waresdev/BuilderextSignatureSerializer.hpp>
#include <openfluid/waresdev/WareSrcMigrator.hpp>
#include <openfluid/waresdev/WareSrcHelpers.hpp>
#include <openfluid/utils/Process.hpp>
#include <openfluid/utils/CMakeProxy.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/config.hpp>

#include "WareTasks.hpp"
#include "DefaultMigrationListener.hpp"


int WareTasks::processCreateWare()
{
  if (!m_Cmd.isOptionActive("id"))
  {
    return error("missing ware ID");
  }

  if (!m_Cmd.isOptionActive("type"))
  {
    return error("missing ware type");
  }
  
  const auto ID = m_Cmd.getOptionValue("id");

  if (!openfluid::tools::isValidWareID(ID))
  {
    return error("invalid ware ID");
  }

  const auto TypeStr = m_Cmd.getOptionValue("type");

  const auto ParentPath = (m_Cmd.getOptionValue("parent-path").empty() ? openfluid::tools::Filesystem::currentPath() : 
                                                                          m_Cmd.getOptionValue("parent-path"));

  openfluid::waresdev::WareSrcFactory::Configuration Config;
  Config.WithParamsUI = m_Cmd.isOptionActive("with-paramsui");
  Config.ParamsUIClassName = (m_Cmd.getOptionValue("paramsui-class").empty() ? Config.ParamsUIClassName : 
                                                                               m_Cmd.getOptionValue("paramsui-class"));

  if (TypeStr == "simulator")
  {
    openfluid::ware::SimulatorSignature Sign;
    Sign.ID = ID;
    return (openfluid::waresdev::WareSrcFactory::createSimulator(Sign,Config,ParentPath).empty() ? 
            error("problem occurred during creation of simulator sources") : 0);
  }
  else if (TypeStr == "observer")
  {
    openfluid::ware::ObserverSignature Sign;
    Sign.ID = ID;
    return (openfluid::waresdev::WareSrcFactory::createObserver(Sign,Config,ParentPath).empty() ? 
            error("problem occurred during creation of observer sources") : 0);
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

    return (openfluid::waresdev::WareSrcFactory::createBuilderext(Sign,Config,ParentPath).empty() ? 
            error("problem occurred during creation of builder-extension sources") : 0);
  }
  else
  {
    return error("unknown ware type");
  }

  return error();
}


// =====================================================================
// =====================================================================


int WareTasks::processConfigure()
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

  auto CMakeCmd = openfluid::utils::CMakeProxy::getConfigureCommand(BuildFSP.toGeneric(),SrcFSP.toGeneric(),
                                                                    Vars,Generator);
  CMakeCmd.Args << m_ThirdPartyArgs;

  return openfluid::utils::Process::system(CMakeCmd);
}


// =====================================================================
// =====================================================================


int WareTasks::processBuild()
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
      BuildFSP = openfluid::tools::Path({m_Cmd.getOptionValue("src-path"),
                                         openfluid::utils::CMakeProxy::getBuildDir()});
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


int WareTasks::processMigrateWare()
{
  if (!m_Cmd.isOptionActive("src-path") || m_Cmd.getOptionValue("src-path").empty())
  {
    return error("missing or empty ware sources path");
  }

  try 
  {
    auto Listener = std::make_unique<DefaultMigrationListener>();
    Listener->setVerbose(m_Cmd.isOptionActive("verbose"));

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

  return 0;
}


// =====================================================================
// =====================================================================


int WareTasks::processInfo2Build()
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

  return 0;
}


// =====================================================================
// =====================================================================


int WareTasks::process()
{
  if (m_Cmd.getName() == "create-ware")
  {
    return processCreateWare();
  }
  else if (m_Cmd.getName() == "check")
  {
    return notImplemented(); // TOIMPL
  }
  else if (m_Cmd.getName() == "migrate-ware")
  {
    return processMigrateWare();
  }
  else if (m_Cmd.getName() == "docalyze")
  {
    return notImplemented(); // TOIMPL
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
    return notImplemented(); // TOIMPL
  }
  else if (m_Cmd.getName() == "info2build")
  {
    return processInfo2Build();
  }
  else if (m_Cmd.getName() == "migrate-ghostsim")
  {
    if (!m_Cmd.isOptionActive("type"))
    {
      return error("missing ware type");
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

