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
 \file BuilderAppProjectState.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderAppProjectState.hpp"

#include "BuilderAppCoordinator.hpp"
#include "BuilderProjectWithExplorer.hpp"
#include "ProjectPropertiesDialog.hpp"

#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include <openfluid/guicommon/PreferencesManager.hpp>

// =====================================================================
// =====================================================================


BuilderAppProjectState::BuilderAppProjectState(
    BuilderAppCoordinator& AppCoordinator) :
  m_App(AppCoordinator)
{
  mp_ProjectPropertiesDialog = new ProjectPropertiesDialog();
}

// =====================================================================
// =====================================================================


void BuilderAppProjectState::whenNewProjectAsked()
{
  if (m_App.showCloseProjectDialog())
  {
    m_App.closeProject();
    m_App.createProject();
  }
}

// =====================================================================
// =====================================================================


void BuilderAppProjectState::whenOpenProjectAsked()
{
  if (m_App.showCloseProjectDialog())
  {
    m_App.closeProject();
    m_App.openProject();
  }
}

// =====================================================================
// =====================================================================


void BuilderAppProjectState::whenCloseProjectAsked()
{
  if (m_App.showCloseProjectDialog())
  {
    m_App.closeProject();
  }
}

// =====================================================================
// =====================================================================


void BuilderAppProjectState::whenQuitAsked()
{
  if (m_App.showCloseProjectDialog())
  {
    if (m_App.showQuitAppDialog())
      m_App.quitApp();
    else
    {
      m_App.setHomeModule();
      m_App.setState(*m_App.getHomeState());
    }
  }
}

// =====================================================================
// =====================================================================


void BuilderAppProjectState::whenRunAsked()
{
  static_cast<BuilderProjectWithExplorer*> (m_App.getCurrentModule())->runAsked();
}

// =====================================================================
// =====================================================================


void BuilderAppProjectState::whenSaveAsked()
{
  ((BuilderProjectWithExplorer*) m_App.getCurrentModule())->saveAsked();
}

// =====================================================================
// =====================================================================


void BuilderAppProjectState::whenSaveAsAsked()
{
  openfluid::guicommon::DialogBoxFactory::showDisabledFeatureMessage();
}

// =====================================================================
// =====================================================================


void BuilderAppProjectState::whenMapViewAsked()
{
  openfluid::guicommon::DialogBoxFactory::showDisabledFeatureMessage();
}

// =====================================================================
// =====================================================================


void BuilderAppProjectState::whenRefreshAsked()
{
  ((BuilderProjectWithExplorer*) m_App.getCurrentModule())->refreshAsked();
}

// =====================================================================
// =====================================================================


void BuilderAppProjectState::whenPreferencesAsked()
{
  m_App.showPreferencesDialog();

  std::vector<std::string>
      RunEnvXPaths =
          openfluid::base::RuntimeEnvironment::getInstance()->getExtraPluginsPaths();

  std::vector<Glib::ustring>
      PrefXPaths =
          openfluid::guicommon::PreferencesManager::getInstance()->getExtraPlugPaths();

  std::reverse(PrefXPaths.begin(), PrefXPaths.end());

  if (!(RunEnvXPaths.size() == PrefXPaths.size() && std::equal(
      RunEnvXPaths.begin(), RunEnvXPaths.end(), PrefXPaths.begin())))
  {
    openfluid::base::RuntimeEnvironment::getInstance()->resetExtraPluginsPaths();

    for (int i = PrefXPaths.size() - 1; i > -1; i--)
      openfluid::base::RuntimeEnvironment::getInstance()->addExtraPluginsPaths(
          PrefXPaths[i]);

    ((BuilderProjectWithExplorer*) m_App.getCurrentModule())->updatePluginPathsMonitors();
    ((BuilderProjectWithExplorer*) m_App.getCurrentModule())->refreshAsked();
  }

}

// =====================================================================
// =====================================================================


void BuilderAppProjectState::whenPropertiesAsked()
{
  mp_ProjectPropertiesDialog->show();
}