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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
 */


// OpenFLUID:stylecheck:!brac


#include <QApplication>
#include <QMessageBox>
#include <QTime>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/ui/waresdev/WareSrcFiletypeManager.hpp>
#include <openfluid/machine/ObserverRegistry.hpp>
#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/machine/ObserverPluginsManager.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/ui/QtHelpers.hpp>
#include <openfluid/waresdev/GhostsHelpers.hpp>

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
  openfluid::ui::waresdev::WareSrcFiletypeManager::kill();
  openfluid::machine::ObserverRegistry::kill();
  openfluid::machine::SimulatorRegistry::kill();
  openfluid::machine::ObserverPluginsManager::kill();
  openfluid::machine::SimulatorPluginsManager::kill();
  WaresTranslationsRegistry::kill();
  ExtensionsRegistry::kill();
  ExtensionPluginsManager::kill();
  openfluid::base::WorkspaceManager::kill();
  openfluid::base::PreferencesManager::kill();
  openfluid::base::RunContextManager::kill();
}


// =====================================================================
// =====================================================================


void BuilderApp::initialize()
{
  openfluid::base::PreferencesManager* PrefsMgr = openfluid::base::PreferencesManager::instance();


  mp_Splash->setMessage(tr("Initializing workspace"));

  openfluid::base::WorkspaceManager::instance()->openWorkspace(PrefsMgr->getCurrentWorkspacePath());


  // TODO see if this is moved into ProjectCoordinator or ProjectModule

  mp_Splash->setMessage(tr("Initializing wares paths"));

  QStringList ExtraPaths = openfluid::ui::toQStringList(PrefsMgr->getBuilderExtraSimulatorsPaths());
  for (int i=0;i<ExtraPaths.size(); i++)
  {
    openfluid::base::Environment::addExtraSimulatorsDirs(ExtraPaths[i].toStdString());
  }

  ExtraPaths = openfluid::ui::toQStringList(PrefsMgr->getBuilderExtraObserversPaths());
  for (int i=0;i<ExtraPaths.size(); i++)
  {
    openfluid::base::Environment::addExtraObserversDirs(ExtraPaths[i].toStdString());
  }

  ExtraPaths = openfluid::ui::toQStringList(PrefsMgr->getBuilderExtraExtensionsPaths());
  for (int i=0;i<ExtraPaths.size(); i++)
  {
    openfluid::base::Environment::addExtraBuilderextsDirs(ExtraPaths[i].toStdString());
  }

  // Diagnosis of existing ghost simulators in previous format (xml)
  std::vector<std::string> GhostPaths;
  for (const auto& P : openfluid::machine::SimulatorPluginsManager::instance()->getPluginsSearchPaths())
  {
    std::vector<std::string> TmpFiles = openfluid::tools::Filesystem::findFilesBySuffixAndExtension(P,
                                                                           openfluid::config::SIMULATORS_GHOSTS_SUFFIX,
                                                                           ".xml",
                                                                           true,true);
    for (const auto& F : TmpFiles)
    {
      GhostPaths.push_back(F);
    }
  }

  mp_Splash->setMessage(tr("Checking ghost simulators"));

  if(GhostPaths.size() > 0 && !openfluid::base::PreferencesManager::instance()->isBuilderSkipGhostMigration())
  {
    if (QMessageBox::warning(QApplication::activeWindow(), tr("Migrate ghosts"),
    tr("Do you want to automatically migrate following ghost wares?\n\n%1").arg(QString::fromStdString("- "+ \
      openfluid::tools::join(GhostPaths, "\n- "))),
    QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel)
      == QMessageBox::Ok)
    {
      std::vector<openfluid::tools::Path> SuccessGhostPaths;
      std::vector<std::string> FailGhosts;
      for (const auto& F : GhostPaths)
      {
        // migrate ghosts automatically
        const auto GhostPath = openfluid::tools::FilesystemPath(F);
        if (openfluid::waresdev::migrateGhostSimulator(GhostPath.dirname(), GhostPath.basename()))
        {
          SuccessGhostPaths.push_back(GhostPath);
        }
        else
        {
          FailGhosts.push_back(F);
        }
      }
      unsigned int NumberSuccess = SuccessGhostPaths.size();
      if (NumberSuccess>0 && QMessageBox::warning(QApplication::activeWindow(), tr("Migrate ghosts"),
        tr("Migration succeeded for %1 ghost(s). Original file(s) were renamed with extension .xml.old. "
            "Do you want to remove them?").arg(QString::fromStdString(std::to_string(NumberSuccess))),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
      {
        for (const auto& GhostPath : SuccessGhostPaths)
        {
          openfluid::waresdev::oldSignaturePath(GhostPath.dirname(), GhostPath.basename()).removeFile();
        }
      }
      if (!FailGhosts.empty())
      {
        QMessageBox::warning(QApplication::activeWindow(), tr("Migrate ghosts"),
          tr("Migration failed for following ghost(s): \n%1\n\nYou can migrate them manually with "
          "command: \nopenfluid migrate-ghostsim").arg(QString::fromStdString("- "+ \
            openfluid::tools::join(FailGhosts, "\n- "))),
          QMessageBox::Ok, QMessageBox::Ok);
      }
    }
    else
    {
      openfluid::base::PreferencesManager::instance()->setBuilderSkipGhostMigration(true);
    }
  }


  // Extensions

  mp_Splash->setMessage(tr("Loading extensions"));

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
