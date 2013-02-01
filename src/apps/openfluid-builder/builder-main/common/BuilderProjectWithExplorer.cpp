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
 \file BuilderProjectWithExplorer.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderProjectWithExplorer.hpp"

#include "ProjectExplorerComponent.hpp"
#include "ProjectWorkspace.hpp"
#include "ProjectCoordinator.hpp"
#include "ProjectDashboard.hpp"
#include "EngineProject.hpp"

#include "BuilderListToolBoxFactory.hpp"


// =====================================================================
// =====================================================================


BuilderProjectWithExplorer::BuilderProjectWithExplorer(
    std::string ProjectFolder)
{
  mp_EngineProject = new EngineProject(ProjectFolder, true);

  mp_ProjectExplorerMVP = new ProjectExplorerComponent(mp_EngineProject->getBuilderDesc());

  mp_Workspace = new ProjectWorkspace();

  mp_ProjectDashboard = new ProjectDashboard();

  mp_Coordinator = new ProjectCoordinator(*mp_ProjectExplorerMVP->getModel(),
      *mp_Workspace, *mp_EngineProject, *mp_ProjectDashboard);

  mp_ToolBoxFactory = new BuilderListToolBoxFactory();

  mp_Coordinator->signal_CheckHappened().connect(sigc::mem_fun(*this,
      &BuilderProjectWithExplorer::whenCheckHappened));

  mp_Coordinator->signal_ChangeHappened().connect(sigc::mem_fun(*this,
      &BuilderProjectWithExplorer::whenChangeHappened));

  mp_EngineProject->signal_SaveHappened().connect(sigc::mem_fun(*this,
      &BuilderProjectWithExplorer::whenSaveHappened));
}

// =====================================================================
// =====================================================================


void BuilderProjectWithExplorer::whenCheckHappened(bool IsCheckOk)
{
  m_signal_CheckHappened.emit(IsCheckOk);
}

// =====================================================================
// =====================================================================


void BuilderProjectWithExplorer::whenChangeHappened()
{
  m_signal_ChangeHappened.emit();
}

// =====================================================================
// =====================================================================


void BuilderProjectWithExplorer::whenSaveHappened()
{
  m_signal_SaveHappened.emit();
}


// =====================================================================
// =====================================================================


sigc::signal<void, bool> BuilderProjectWithExplorer::signal_CheckHappened()
{
  return m_signal_CheckHappened;
}

// =====================================================================
// =====================================================================


sigc::signal<void> BuilderProjectWithExplorer::signal_ChangeHappened()
{
  return m_signal_ChangeHappened;
}

// =====================================================================
// =====================================================================


sigc::signal<void> BuilderProjectWithExplorer::signal_SaveHappened()
{
  return m_signal_SaveHappened;
}

// =====================================================================
// =====================================================================


BuilderProjectWithExplorer::~BuilderProjectWithExplorer()
{
  delete mp_ToolBoxFactory;
  delete mp_Coordinator;
  delete mp_Workspace;
  delete mp_ProjectExplorerMVP;
  delete mp_EngineProject;
  delete mp_ProjectDashboard;
}

// =====================================================================
// =====================================================================


void BuilderProjectWithExplorer::compose()
{
  mp_MainPaned = Gtk::manage(new Gtk::HPaned());
  mp_MainPaned->set_visible(true);

  //  Gtk::Label* NothingLabel = Gtk::manage(new Gtk::Label(
  //      _("Nothing to display at this time")));
  //  NothingLabel->set_visible(true);

  Gtk::VPaned* LeftBox = Gtk::manage(new Gtk::VPaned());
  LeftBox->pack1(*mp_ProjectExplorerMVP->asWidget());
  LeftBox->pack2(*mp_ProjectDashboard->asWidget(), true, true);
  LeftBox->set_visible(true);

  mp_MainPaned->add1(*LeftBox);
  mp_MainPaned->add2(*mp_Workspace->asWidget());
  //  mp_MainPaned->add2(*NothingLabel);
}

// =====================================================================
// =====================================================================


void BuilderProjectWithExplorer::runAsked()
{
  mp_Coordinator->setFileMonitorDisplayState(false);

  mp_EngineProject->run();

  mp_Coordinator->setFileMonitorDisplayState(true);
}

// =====================================================================
// =====================================================================


void BuilderProjectWithExplorer::saveAsked()
{
  mp_EngineProject->save();
}

// =====================================================================
// =====================================================================


void BuilderProjectWithExplorer::checkAsked()
{
  mp_Coordinator->checkProject();
}

// =====================================================================
// =====================================================================


void BuilderProjectWithExplorer::refreshAsked()
{
  mp_Coordinator->whenUpdatePluginsAsked();
}

// =====================================================================
// =====================================================================


void BuilderProjectWithExplorer::updatePluginPathsMonitors()
{
  mp_Coordinator->updatePluginPathsMonitors();
}

// =====================================================================
// =====================================================================


void BuilderProjectWithExplorer::mapViewAsked()
{
  mp_Coordinator->whenMapViewAsked();
}

// =====================================================================
// =====================================================================


void BuilderProjectWithExplorer::extensionAsked(const std::string& ExtensionID)
{
  mp_Coordinator->launchExtension(ExtensionID);
}


// =====================================================================
// =====================================================================


Gtk::Widget* BuilderProjectWithExplorer::asWidget()
{
  return mp_MainPaned;
}

