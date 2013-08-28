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
  \file ProjectModule.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <QApplication>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/guicommon/PreferencesManager.hpp>

#include "ProjectModule.hpp"
#include "PreferencesDialog.hpp"

#include <iostream>


// =====================================================================
// =====================================================================


ProjectModule::ProjectModule(const QString& ProjectPath):
AbstractModule(), mp_Widget(NULL), m_ProjectPath(ProjectPath), mp_ProjectCentral(NULL)
{
  mp_ProjectCentral = new ProjectCentral(ProjectPath);
}


// =====================================================================
// =====================================================================


ProjectModule::~ProjectModule()
{
  delete mp_ProjectCentral;
}


// =====================================================================
// =====================================================================


QWidget* ProjectModule::getWidget(QWidget* Parent)
{
  if (mp_Widget != NULL)
    delete mp_Widget;

  mp_Widget = new ProjectWidget(Parent);

  return mp_Widget;
}


// =====================================================================
// =====================================================================


bool ProjectModule::whenQuitAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  return true;
}


// =====================================================================
// =====================================================================


bool ProjectModule::whenNewAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  return false;
}

// =====================================================================
// =====================================================================


bool ProjectModule::whenOpenAsked()
{
  return false;
}


// =====================================================================
// =====================================================================


void ProjectModule::whenSaveAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}


// =====================================================================
// =====================================================================


void ProjectModule::whenSaveAsAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}


// =====================================================================
// =====================================================================


void ProjectModule::whenPropertiesAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}


// =====================================================================
// =====================================================================


bool ProjectModule::whenCloseAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  return true;
}


// =====================================================================
// =====================================================================


void ProjectModule::whenPreferencesAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  PreferencesDialog PrefsDlg(QApplication::activeWindow());
  PrefsDlg.exec();

  openfluid::guicommon::PreferencesManager* PrefsMgr =
    openfluid::guicommon::PreferencesManager::getInstance();

  if (PrefsDlg.isSimPathsChanged())
  {
    QStringList ExtraPaths = PrefsMgr->getExtraSimulatorsPaths();

    openfluid::base::RuntimeEnvironment::getInstance()->resetExtraSimulatorsPluginsPaths();
    for (int i=0;i<ExtraPaths.size(); i++)
      openfluid::base::RuntimeEnvironment::getInstance()->addExtraSimulatorsPluginsPaths(ExtraPaths[i].toStdString());

    // TODO update current project (monitoring paths, model, ...)
  }

  if (PrefsDlg.isObsPathsChanged())
  {
    QStringList ExtraPaths = PrefsMgr->getExtraObserversPaths();

    openfluid::base::RuntimeEnvironment::getInstance()->resetExtraObserversPluginsPaths();
    for (int i=0;i<ExtraPaths.size(); i++)
      openfluid::base::RuntimeEnvironment::getInstance()->addExtraObserversPluginsPaths(ExtraPaths[i].toStdString());

    // TODO update current project (monitoring paths, observers, ...)
  }

}


// =====================================================================
// =====================================================================


void ProjectModule::whenRunAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  mp_ProjectCentral->run();
}


// =====================================================================
// =====================================================================


void ProjectModule::whenMarketAsked()
{

}


// =====================================================================
// =====================================================================


void ProjectModule::whenRefreshAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}


// =====================================================================
// =====================================================================


bool ProjectModule::whenOpenExampleAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  return false;
}

