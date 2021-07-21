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
  @file WorkspaceManager.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
 */


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace base {


OPENFLUID_SINGLETON_INITIALIZATION(WorkspaceManager)


const std::string WorkspaceManager::m_SettingsRole = "openfluid-workspace-settings";


// =====================================================================
// =====================================================================


WorkspaceManager::WorkspaceManager()
{

}


// =====================================================================
// =====================================================================


WorkspaceManager::~WorkspaceManager()
{

}


// =====================================================================
// =====================================================================


void WorkspaceManager::updateSettingsFile(const std::string& FilePath) const
{
#if (OPENFLUID_VERSION_NUMERIC >= OPENFLUID_VERSION_COMPUTE(2,3,0))
# warning "the project file format compatibility is deprecated and should be removed"
#endif


  if (!openfluid::tools::Filesystem::isFile(FilePath))
  {
    std::string FormerFilePath = 
      openfluid::tools::Filesystem::joinPath({
        openfluid::tools::Filesystem::dirname(FilePath),
        "openfluid-waresdev.conf"
      });

    if (openfluid::tools::Filesystem::isFile(FormerFilePath))
    {
      auto cleanKeyValue = [](const std::string& Key, const std::string& Value)
      {
         std::string UnquotedVal = Value;
         if (Value.front() == '"' && Value.back() == '"')
         {
           UnquotedVal = Value.substr(1,UnquotedVal.length()-2);
         }

         return std::make_pair(
           openfluid::tools::trim(Key),
           openfluid::tools::trim(UnquotedVal)
         );
      };

      boost::property_tree::ptree INI;
      boost::property_tree::ini_parser::read_ini(FormerFilePath,INI);
      openfluid::tools::SettingsBackend SB(FilePath,m_SettingsRole,false);

      for(const auto& Section : INI)
      {
        if (Section.first == "openfluid.workspace.waresdev")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());

            if (openfluid::tools::toLowerCase(Key) == "openwares")
            {
              std::vector<std::string> OpenArray;
              auto OpenWares = openfluid::tools::splitString(Value,",");

              for (const auto& W : OpenWares)
              {
                OpenArray.push_back(openfluid::tools::trim(W));
              }

              SB.setValue("/waresdev/wares","open",OpenArray);
            }
            else if (openfluid::tools::toLowerCase(Key) == "activeware")
            {
              SB.setValue("/waresdev/wares","active",Value);
            }
            else if (openfluid::tools::toLowerCase(Key) == "buildmode")
            {
              std::string ConfigMode;
              std::string BuildMode;
              auto Modes = openfluid::tools::splitString(Value,"|");
              
              if (Modes.size() == 2)
              {
                ConfigMode = Modes[0];
                BuildMode = Modes[1];
              }

              SB.setValue("/waresdev/mode","configure",ConfigMode);
              SB.setValue("/waresdev/mode","build",BuildMode);
            }
          }
        }
      }

      SB.save();
    }
  }
}


// =====================================================================
// =====================================================================


void WorkspaceManager::prepareWorkspace()
{
  m_IsOpen = true;

  if (!m_WorkspacePath.empty())
  {
    const std::vector<std::string> PathsToCheck = {
      m_WorkspacePath,
      getProjectsPath(),
      getWaresPath(),
      getWaresPath(openfluid::ware::WareType::SIMULATOR),
      getWaresPath(openfluid::ware::WareType::OBSERVER),
      getWaresPath(openfluid::ware::WareType::BUILDEREXT)
    };

    for (const auto& P: PathsToCheck)
    {
      openfluid::tools::Filesystem::makeDirectory(P);
      m_IsOpen = m_IsOpen && openfluid::tools::Filesystem::isDirectory(P);  
    }

    // try to convert a former openfluid-waredev.conf file if necessary
    updateSettingsFile(getSettingsFile());
  }
}


// =====================================================================
// =====================================================================


void WorkspaceManager::loadSettings()
{
  m_Settings = 
    std::make_unique<openfluid::tools::SettingsBackend>(getSettingsFile(),m_SettingsRole);

  if (!openfluid::tools::Filesystem::isFile(getSettingsFile()))
  {
    m_Settings->save();
  }
}


// =====================================================================
// =====================================================================


void WorkspaceManager::openWorkspace(const std::string& Path)
{
  closeWorkspace();

  m_WorkspacePath = Path;

  prepareWorkspace();

  loadSettings();
}


// =====================================================================
// =====================================================================


void  WorkspaceManager::closeWorkspace()
{
  m_IsOpen = false;
  m_Settings.reset();  
  m_WorkspacePath.clear();
}


// =====================================================================
// =====================================================================


std::string WorkspaceManager::getSettingsFile(const std::string& WorkspacePath)
{
  return openfluid::tools::Filesystem::joinPath({WorkspacePath,openfluid::config::WORKSPACE_CONFIGFILE});
}


// =====================================================================
// =====================================================================


std::string WorkspaceManager::getProjectsPath(const std::string& WorkspacePath)
{
  return openfluid::tools::Filesystem::joinPath({WorkspacePath,openfluid::config::PROJECTS_PATH});
}


// =====================================================================
// =====================================================================


std::string WorkspaceManager::getWaresPath(const std::string& WorkspacePath, openfluid::ware::WareType Type)
{
  if (Type == openfluid::ware::WareType::SIMULATOR)
  {
    return openfluid::tools::Filesystem::joinPath({WorkspacePath,openfluid::config::WARESDEV_PATH,
                                                   openfluid::config::SIMULATORS_PATH});
  }
  else if (Type == openfluid::ware::WareType::OBSERVER)
  {
    return openfluid::tools::Filesystem::joinPath({WorkspacePath,openfluid::config::WARESDEV_PATH,
                                                   openfluid::config::OBSERVERS_PATH});
  }
  else if (Type == openfluid::ware::WareType::BUILDEREXT)
  {
    return openfluid::tools::Filesystem::joinPath({WorkspacePath,openfluid::config::WARESDEV_PATH,
                                                   openfluid::config::BUILDEREXTS_PATH});
  }

  return openfluid::tools::Filesystem::joinPath({WorkspacePath,openfluid::config::WARESDEV_PATH});
}


// =====================================================================
// =====================================================================


std::string WorkspaceManager::getSettingsFile() const
{
  if (m_IsOpen)
  {
    return WorkspaceManager::getSettingsFile(m_WorkspacePath);
  }

  return "";
}


// =====================================================================
// =====================================================================


std::string WorkspaceManager::getProjectsPath() const
{
  if (m_IsOpen)
  {
    return WorkspaceManager::getProjectsPath(m_WorkspacePath);
  }

  return "";
}


// =====================================================================
// =====================================================================


std::string WorkspaceManager::getProjectPath(const std::string& Name) const
{
  if (m_IsOpen)
  {
    return openfluid::tools::Filesystem::joinPath({getProjectsPath(),Name});
  }

  return "";
}


// =====================================================================
// =====================================================================


void WorkspaceManager::insertRecentProject(const std::string& Name, const std::string& Path)
{
  if (m_IsOpen)
  {
    auto RecentList = WorkspaceManager::getRecentProjects();

    m_Settings->enableAutoSave(false);

    // remove existing project from list if project to insert is already present
    auto it = RecentList.begin();
    while (it != RecentList.end()) 
    {
      if ((*it).Path != Path)
      {
        it++;  // Existing path does not match inserted path: keep it
      }
      else
      {
        it = RecentList.erase(it); // Existing path matches inserted path: remove it
      }
    }

    while (RecentList.size() > (RecentProjectsMax-1))
    {
      RecentList.pop_back();
    }

    RecentProject_t RP;
    RP.Name = Name;
    RP.Path = Path;
    RecentList.push_front(RP);
  
    unsigned int Index = 0;
    for (const auto& R : RecentList)
    {
      m_Settings->setValue("/projects/recent/"+std::to_string(Index),"name",R.Name);  
      m_Settings->setValue("/projects/recent/"+std::to_string(Index),"path",R.Path);

      Index++;
    }

    m_Settings->save();
    m_Settings->enableAutoSave(true);
  }
}


// =====================================================================
// =====================================================================


std::list<WorkspaceManager::RecentProject_t> WorkspaceManager::getRecentProjects() const
{  
  std::list<WorkspaceManager::RecentProject_t> RecentList;

  if (m_IsOpen)
  { 
    auto Value = m_Settings->getValue("/projects/recent");

    if (Value.isNull())
    {
      return RecentList;
    }

    auto JSONArray = Value.JSONValue();

    if (JSONArray != nullptr && JSONArray->IsArray())
    {
      for (const auto& V : JSONArray->GetArray())
      {
        if (V.IsObject() && V.HasMember("name") && V.HasMember("path"))
        { 
          RecentProject_t RP;
          RP.Name = V["name"].GetString();
          RP.Path = V["path"].GetString();
          RecentList.push_back(RP);
        }
      }
    }
  }

  return RecentList;
}


// =====================================================================
// =====================================================================


void WorkspaceManager::clearRecentProjects()
{
  if (m_IsOpen)
  {
    m_Settings->remove("/projects/recent");
  }
}


// =====================================================================
// =====================================================================


std::string WorkspaceManager::getWaresPath(openfluid::ware::WareType Type) const
{
  if (m_IsOpen)
  {
    return getWaresPath(m_WorkspacePath,Type);
  }

  return "";
}


// =====================================================================
// =====================================================================


std::string WorkspaceManager::getWarePath(openfluid::ware::WareType Type, const openfluid::ware::WareID_t& ID) const
{
  if (m_IsOpen)
  {
    return openfluid::tools::Filesystem::joinPath({getWaresPath(Type),ID});
  }

  return "";
}


// =====================================================================
// =====================================================================


std::vector<std::string> WorkspaceManager::getOpenWaresPaths() const
{
  if (m_IsOpen)
  {
    return m_Settings->getValue("/waresdev/wares/open").get<std::vector<std::string>>(std::vector<std::string>());
  }

  return std::vector<std::string>();
}


// =====================================================================
// =====================================================================


void WorkspaceManager::setOpenWaresPaths(const std::vector<std::string>& Paths)
{
  if (m_IsOpen)
  {
     m_Settings->setValue("/waresdev/wares","open",Paths);
  }
}


// =====================================================================
// =====================================================================


std::string WorkspaceManager::getActiveWarePath() const
{
  if (m_IsOpen)
  {
    return m_Settings->getValue("/waresdev/wares/active").get<std::string>("");
  }

  return "";
}


// =====================================================================
// =====================================================================


void WorkspaceManager::setActiveWarePath(const std::string& Path)
{
  if (m_IsOpen)
  {
     m_Settings->setValue("/waresdev/wares","active",Path);
  }
}


// =====================================================================
// =====================================================================


std::string WorkspaceManager::getWaresConfigureMode() const
{
  if (m_IsOpen)
  {
    return openfluid::tools::toUpperCase(m_Settings->getValue("/waresdev/mode/configure").get<std::string>(""));
  }

  return "";
}


// =====================================================================
// =====================================================================


void WorkspaceManager::setWaresConfigureMode(const std::string& Mode)
{
  if (m_IsOpen)
  {
     m_Settings->setValue("/waresdev/mode","configure",Mode);
  }
}


// =====================================================================
// =====================================================================


std::string WorkspaceManager::getWaresBuildMode() const
{
  if (m_IsOpen)
  {
    return openfluid::tools::toUpperCase(m_Settings->getValue("/waresdev/mode/build").get<std::string>(""));
  }

  return "";
}


// =====================================================================
// =====================================================================


void WorkspaceManager::setWaresBuildMode(const std::string& Mode)
{
  if (m_IsOpen)
  {
     m_Settings->setValue("/waresdev/mode","build",Mode);
  }
}


} } //namespaces
