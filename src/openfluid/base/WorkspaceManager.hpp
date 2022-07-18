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
  @file WorkspaceManager.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
 */


#ifndef __OPENFLUID_BASE_WORKSPACEMANAGER_HPP__
#define __OPENFLUID_BASE_WORKSPACEMANAGER_HPP__


#include <memory>

#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/tools/SettingsBackend.hpp>
#include <openfluid/utils/SingletonMacros.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace base {


class OPENFLUID_API WorkspaceManager
{

  OPENFLUID_SINGLETON_DEFINITION(WorkspaceManager)


  private:

    std::string m_WorkspacePath;

    std::unique_ptr<openfluid::tools::SettingsBackend> m_Settings;

    bool m_IsOpen = false;

    static const std::string m_SettingsRole;

    WorkspaceManager();

    ~WorkspaceManager();

    static void updateSettingsFile(const std::string& FilePath);

    void prepareWorkspace();

    void loadSettings();


  public:

    struct RecentProject
    {
      std::string Name;

      std::string Path;
    };

    static void prepareWorkspace(const std::string& Path);

    void openWorkspace(const std::string& Path);

    void closeWorkspace();

    bool isOpen() const
    {
      return m_IsOpen;
    }

    std::string getWorkspacePath() const
    {
      return m_WorkspacePath;
    }
    
    static std::string getSettingsFile(const std::string& WorkspacePath);

    static std::string getProjectsPath(const std::string& WorkspacePath);

    static std::string getWaresPath(const std::string& WorkspacePath,
                                    openfluid::ware::WareType Type = openfluid::ware::WareType::UNDEFINED);

    static std::string getWarePath(const std::string& WorkspacePath,
                                   openfluid::ware::WareType Type, const openfluid::ware::WareID_t& ID);

    std::string getSettingsFile() const;

    std::string getProjectsPath() const;

    std::string getProjectPath(const std::string& Name) const;

    void insertRecentProject(const std::string& Name, const std::string& Path);

    std::list<RecentProject> getRecentProjects() const;

    inline const static unsigned int RecentProjectsMax = 10;

    void clearRecentProjects();

    std::string getWaresPath(openfluid::ware::WareType Type = openfluid::ware::WareType::UNDEFINED) const;

    std::string getWarePath(openfluid::ware::WareType Type, const openfluid::ware::WareID_t& ID) const;

    std::vector<std::string> getOpenWaresPaths() const;

    void setOpenWaresPaths(const std::vector<std::string>& Paths);

    std::string getActiveWarePath() const;

    void setActiveWarePath(const std::string& Path);

    std::string getWaresConfigureMode() const;

    void setWaresConfigureMode(const std::string& Mode);

    std::string getWaresBuildMode() const;

    void setWaresBuildMode(const std::string& Mode);

    bool isWaresParallelJobsEnabled() const;

    void setWaresParallelJobsEnabled(bool Enabled);

    int getWaresParallelJobsCount() const;

    void setWaresParallelJobsCount(int Jobs);
};


} } //namespaces


#endif /* __OPENFLUID_BASE_WORKSPACEMANAGER_HPP__ */
