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


#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/waresdev/WareSrcFactory.hpp>
#include <openfluid/waresdev/GhostsHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/IDHelpers.hpp>

#include "WareTasks.hpp"


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
    return notImplemented(); // TOIMPL
  }
  else if (m_Cmd.getName() == "docalyze")
  {
    return notImplemented(); // TOIMPL
  }
  else if (m_Cmd.getName() == "configure")
  {
    return notImplemented(); // TOIMPL
  }
  else if (m_Cmd.getName() == "build")
  {
    return notImplemented(); // TOIMPL
  }
  else if (m_Cmd.getName() == "buildinstall")
  {
    return notImplemented(); // TOIMPL
  }
  else if (m_Cmd.getName() == "purge")
  {
    return notImplemented(); // TOIMPL
  }
  else if (m_Cmd.getName() == "info2build")
  {
    return notImplemented(); // TOIMPL
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

