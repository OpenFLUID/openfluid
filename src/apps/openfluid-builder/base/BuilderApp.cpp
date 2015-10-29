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
  @file BuilderApp.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <QApplication>
#include <QTime>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/RuntimeEnv.hpp>

#include "BuilderApp.hpp"
#include "ExtensionsRegistry.hpp"
#include "WaresTranslationsRegistry.hpp"
#include "ProjectCentral.hpp"


BuilderApp::BuilderApp(openfluid::ui::common::OpenFLUIDSplashScreen* Splash):
  m_Coordinator(m_MainWindow,m_Actions), mp_Splash(Splash)
{
  QApplication::setAttribute((Qt::AA_DontShowIconsInMenus));
}


// =====================================================================
// =====================================================================


BuilderApp::~BuilderApp()
{
  ProjectCentral::kill();
  openfluid::base::PreferencesManager::kill();
  WaresTranslationsRegistry::kill();
  ExtensionPluginsManager::kill();
}


// =====================================================================
// =====================================================================


void BuilderApp::initialize()
{
  qsrand((uint)QTime::currentTime().msec());

  openfluid::base::PreferencesManager* PrefsMgr =
    openfluid::base::PreferencesManager::instance();


  // TODO see if this is moved into ProjectCoordinator or ProjectModule

  mp_Splash->setMessage(tr("Initializing wares paths"));

  QStringList ExtraPaths = PrefsMgr->getExtraSimulatorsPaths();
  for (int i=0;i<ExtraPaths.size(); i++)
    openfluid::base::RuntimeEnvironment::instance()->addExtraSimulatorsPluginsPaths(ExtraPaths[i].toStdString());

  ExtraPaths = PrefsMgr->getExtraObserversPaths();
  for (int i=0;i<ExtraPaths.size(); i++)
    openfluid::base::RuntimeEnvironment::instance()->addExtraObserversPluginsPaths(ExtraPaths[i].toStdString());



  // Extensions

  mp_Splash->setMessage(tr("Loading extensions"));

  ExtraPaths = PrefsMgr->getExtraExtensionsPaths();
  ExtensionPluginsManager::instance(ExtraPaths); // initialization parameterized with extra paths
  ExtensionsRegistry::instance()->registerExtensions();


  mp_Splash->setMessage(tr("Preparing and configuring UI"));

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
