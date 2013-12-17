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
  \file BuilderApp.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <QApplication>
#include <QTime>

#include "BuilderApp.hpp"


#include <openfluid/guicommon/PreferencesManager.hpp>
#include <openfluid/base/RuntimeEnv.hpp>

#include "ExtensionsRegistry.hpp"


BuilderApp::BuilderApp():
  m_Coordinator(m_MainWindow,m_Actions)
{
  QApplication::setAttribute((Qt::AA_DontShowIconsInMenus));
}


// =====================================================================
// =====================================================================


BuilderApp::~BuilderApp()
{

}


// =====================================================================
// =====================================================================


void BuilderApp::initialize()
{
  qsrand((uint)QTime::currentTime().msec());

  openfluid::guicommon::PreferencesManager* PrefsMgr =
    openfluid::guicommon::PreferencesManager::getInstance();


  // TODO see if this is moved into ProjectCoordinator or ProjectModule

  QStringList ExtraPaths = PrefsMgr->getExtraSimulatorsPaths();
  for (int i=0;i<ExtraPaths.size(); i++)
    openfluid::base::RuntimeEnvironment::getInstance()->addExtraSimulatorsPluginsPaths(ExtraPaths[i].toStdString());

  ExtraPaths = PrefsMgr->getExtraObserversPaths();
  for (int i=0;i<ExtraPaths.size(); i++)
    openfluid::base::RuntimeEnvironment::getInstance()->addExtraObserversPluginsPaths(ExtraPaths[i].toStdString());


  // Extensions

  ExtraPaths = PrefsMgr->getExtraExtensionsPaths();
  ExtensionPluginsManager::getInstance(ExtraPaths); // initialization parameterized with extra paths
  ExtensionsRegistry::getInstance()->registerExtensions();

  m_Actions.createMenus(m_MainWindow);
  m_Actions.createToolbar(m_MainWindow);

  m_Coordinator.connectExtensions();

  m_Coordinator.setHomeModule();
}


// =====================================================================
// =====================================================================


void BuilderApp::run()
{
  m_MainWindow.show();
}
