/*
 This file is part of OpenFLUID software
 Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
 along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

 In addition, as a special exception, INRA gives You the additional right
 to dynamically link the code of OpenFLUID with code not covered
 under the GNU General Public License ("Non-GPL Code") and to distribute
 linked combinations including the two, subject to the limitations in this
 paragraph. Non-GPL Code permitted under this exception must only link to
 the code of OpenFLUID dynamically through the OpenFLUID libraries
 interfaces, and only for building OpenFLUID plugins. The files of
 Non-GPL Code may be link to the OpenFLUID libraries without causing the
 resulting work to be covered by the GNU General Public License. You must
 obey the GNU General Public License in all respects for all of the
 OpenFLUID code and other code used in conjunction with OpenFLUID
 except the Non-GPL Code covered by this exception. If you modify
 this OpenFLUID, you may extend this exception to your version of the file,
 but you are not obligated to do so. If you do not wish to provide this
 exception without modification, you must delete this exception statement
 from your version and license this OpenFLUID solely under the GPL without
 exception.


 == Other Usage ==

 Other Usage means a use of OpenFLUID that is inconsistent with the GPL
 license, and requires a written agreement between You and INRA.
 Licensees for Other Usage of OpenFLUID may use this file in accordance
 with the terms contained in the written agreement between You and INRA.
 */

/**
 \file ProjectCoordinator.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __PROJECTCOORDINATOR_HPP__
#define __PROJECTCOORDINATOR_HPP__

#include <sigc++/sigc++.h>
#include <map>
#include <vector>
#include <string>

#include <giomm/filemonitor.h>
#include <gtkmm/messagedialog.h>

class ProjectExplorerModel;
class ProjectWorkspace;
class ProjectWorkspaceModule;
class EngineProject;
class BuilderModuleFactory;
class ProjectDashboard;

class ProjectCoordinator
{
  private:

    sigc::signal<void, bool> m_signal_CheckHappened;

    sigc::signal<void> m_signal_ChangeHappened;

    ProjectExplorerModel& m_ExplorerModel;

    ProjectWorkspace& m_Workspace;

    EngineProject& m_EngineProject;

    ProjectDashboard& m_ProjectDashboard;

    BuilderModuleFactory* mp_ModuleFactory;

    std::map<std::string, ProjectWorkspaceModule*> m_ModulesByPageNameMap;

    std::vector<std::string> m_ClassPageNames;

    std::vector<std::string> m_SetPageNames;

    bool m_HasRun;

    // only to keep ref to FileMonitors (otherwise they're lost)
    std::vector<Glib::RefPtr<Gio::FileMonitor> > m_DirMonitors;

    std::string m_ModelPageName;

    Gtk::MessageDialog* mp_FileMonitorDialog;

    bool m_FileMonitorHasChanged;

    bool m_FileMonitorHasToDisplay;

    void whenActivationChanged();

    void whenDomainChanged();

    void whenClassChanged();

    void whenRunChanged();

    void whenOutChanged();

    void whenResultsChanged();

    void whenMapViewChanged();

    void whenPageRemoved(std::string RemovedPageName);

    void updateResults();

    void updateWorkspaceModules();

    std::string constructClassPageName(std::string ClassName);

    std::string constructSetPageName(std::string SetName);

    void onDirMonitorChanged(const Glib::RefPtr<Gio::File>& File,
        const Glib::RefPtr<Gio::File>& OtherFile,
        Gio::FileMonitorEvent EventType);

  protected:

    void whenModelChanged();

    std::vector<std::string> getClassPagesToDelete();

    std::vector<std::string> getSetPagesToDelete();

  public:

    sigc::signal<void, bool> signal_CheckHappened();

    sigc::signal<void> signal_ChangeHappened();

    ProjectCoordinator(ProjectExplorerModel& ExplorerModel,
        ProjectWorkspace& Workspace, EngineProject& TheEngineProject,
        ProjectDashboard& TheProjectDashboard);

    ~ProjectCoordinator();

    void checkProject();

    void whenRunHappened();

    void whenUpdatePluginsAsked(int ResponseId = Gtk::RESPONSE_OK);

    void setFileMonitorDisplayState(bool HasToDisplay);

    void updatePluginPathsMonitors();

    void whenMapViewAsked();

};

class ProjectCoordinatorSub: public ProjectCoordinator
{
  public:

    ProjectCoordinatorSub(ProjectExplorerModel& ExplorerModel,
        ProjectWorkspace& Workspace, EngineProject& TheEngineProject,
        ProjectDashboard& TheProjectDashboard);

    void whenModelChanged();

    std::vector<std::string> getWorkspacePagesToDelete();

};

#endif /* __PROJECTCOORDINATOR_HPP__ */
