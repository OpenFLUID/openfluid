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
 \file BuilderAppCoordinator.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderAppCoordinator.hpp"

#include "BuilderAppWindow.hpp"
#include "BuilderModuleFactory.hpp"
#include "BuilderProjectModule.hpp"
#include "BuilderAppDialogFactory.hpp"
#include "BuilderAppHomeState.hpp"
#include "BuilderAppProjectState.hpp"


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::unsetCurrentModule()
{
  m_MainWindow.unsetCurrentModuleWidget();
  delete mp_CurrentModule;
}


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::setCurrentModule(BuilderModule* Module)
{
  unsetCurrentModule();
  mp_CurrentModule = Module;
  mp_CurrentModule->initialize();
  m_MainWindow.setCurrentModuleWidget(
      *mp_CurrentModule->composeAndGetAsWidget());
}


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenNewProjectAsked()
{
  mp_CurrentState->whenNewProjectAsked();
}


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenOpenProjectAsked()
{
  mp_CurrentState->whenOpenProjectAsked();
}


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenCloseProjectAsked()
{
  mp_CurrentState->whenCloseProjectAsked();
}


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenQuitAsked()
{
  mp_CurrentState->whenQuitAsked();
}


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenRunAsked()
{
  mp_CurrentState->whenRunAsked();
}


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::whenMarketAsked()
{
  mp_CurrentState->whenMarketAsked();
}


// =====================================================================
// =====================================================================


BuilderAppCoordinator::BuilderAppCoordinator(BuilderAppWindow& MainWindow,
    BuilderAppActions& Actions) :
  m_MainWindow(MainWindow), m_Actions(Actions)
{
  mp_CurrentModule = 0;

  mp_HomeState = new BuilderAppHomeState(*this);
  mp_ProjectState = new BuilderAppProjectState(*this);

  m_Actions.getFileNewAction()->signal_activate().connect(sigc::mem_fun(*this,
      &BuilderAppCoordinator::whenNewProjectAsked));
  m_Actions.getFileOpenAction()->signal_activate().connect(sigc::mem_fun(*this,
      &BuilderAppCoordinator::whenOpenProjectAsked));
  m_Actions.getFileCloseAction()->signal_activate().connect(sigc::mem_fun(
      *this, &BuilderAppCoordinator::whenCloseProjectAsked));
  m_Actions.getFileQuitAction()->signal_activate().connect(sigc::mem_fun(*this,
      &BuilderAppCoordinator::whenQuitAsked));

  m_Actions.getSimulationRunAction()->signal_activate().connect(sigc::mem_fun(
      *this, &BuilderAppCoordinator::whenRunAsked));

  m_Actions.getAppMarketAction()->signal_activate().connect(sigc::mem_fun(
        *this, &BuilderAppCoordinator::whenMarketAsked));


  setState(*mp_HomeState);
}


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::setState(BuilderAppState& State)
{
  mp_CurrentState = &State;
}


// =====================================================================
// =====================================================================


BuilderAppState* BuilderAppCoordinator::getHomeState()
{
  return mp_HomeState;
}
BuilderAppState* BuilderAppCoordinator::getProjectState()
{
  return mp_ProjectState;
}


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::setHomeModule()
{
  setCurrentModule(BuilderModuleFactory::createHomeModule(m_Actions));
  m_Actions.setProjectActionGroupVisible(false);
  m_MainWindow.setToolBarVisible(false);
}


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::setProjectModule(std::string FolderIn)
{
	setCurrentModule(new BuilderProjectModulePlainGtk(FolderIn));
  m_Actions.setProjectActionGroupVisible(true);
  m_MainWindow.setToolBarVisible(true);
}


// =====================================================================
// =====================================================================


BuilderModule* BuilderAppCoordinator::getCurrentModule()
{
  return mp_CurrentModule;
}


// =====================================================================
// =====================================================================


void BuilderAppCoordinator::quitApp()
{
  unsetCurrentModule();
  m_MainWindow.hide();
}


// =====================================================================
// =====================================================================


bool BuilderAppCoordinator::showCloseProjectDialog()
{
  return BuilderAppDialogFactory::showSimpleOkCancelQuestionDialog(
      _("Are you sure you want to close this project ?"));
}


// =====================================================================
// =====================================================================


bool BuilderAppCoordinator::showQuitAppDialog()
{
  return BuilderAppDialogFactory::showSimpleOkCancelQuestionDialog(
      _("Are you sure you want to quit ?"));
}


// =====================================================================
// =====================================================================


std::string BuilderAppCoordinator::showOpenProjectDialog()
{
  return BuilderAppDialogFactory::showOpenProjectDialog();
}


