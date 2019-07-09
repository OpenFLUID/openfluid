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
  @file AppCoordinator.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QUrl>
#include <QStatusBar>
#include <QDesktopServices>
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
#include <QLabel>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/Exception.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/buddies/ExamplesBuddy.hpp>
#include <openfluid/ui/common/AboutDialog.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/config.hpp>
#include <openfluid/tools/FileHelpers.hpp>

#include "AppCoordinator.hpp"
#include "MainWindow.hpp"
#include "AppActions.hpp"
#include "OpenExampleProjectDialog.hpp"
#include "HomeModuleWidget.hpp"
#include "ProjectModuleWidget.hpp"
#include "DashboardFrame.hpp"
#include "ProjectCentral.hpp"
#include "builderconfig.hpp"


AppCoordinator::AppCoordinator(MainWindow& MainWin, AppActions& Actions):
  m_MainWindow(MainWin),m_Actions(Actions),
  mp_DockWidget(nullptr), mp_CurrentModule(nullptr)
{
  m_Actions.action("ProjectSave")->setEnabled(false);


  // Project
  connect(m_Actions.action("ProjectQuit"), SIGNAL(triggered()),
          this, SLOT(whenQuitAsked()));
  connect(m_Actions.action("ProjectNew"), SIGNAL(triggered()),
          this, SLOT(whenNewAsked()));
  connect(m_Actions.action("ProjectOpen"), SIGNAL(triggered()),
          this, SLOT(whenOpenAsked()));
  connect(m_Actions.action("ProjectReload"), SIGNAL(triggered()),
            this, SLOT(whenReloadAsked()));
  connect(m_Actions.action("ProjectSave"), SIGNAL(triggered()),
          this, SLOT(whenSaveAsked()));
  connect(m_Actions.action("ProjectSaveAs"), SIGNAL(triggered()),
          this, SLOT(whenSaveAsAsked()));
  connect(m_Actions.action("ProjectProperties"), SIGNAL(triggered()),
          this, SLOT(whenPropertiesAsked()));
  connect(m_Actions.action("ProjectExplore"), SIGNAL(triggered()),
          this, SLOT(whenExploreAsked()));
  connect(m_Actions.action("ProjectClose"), SIGNAL(triggered()),
          this, SLOT(whenCloseAsked()));

  // Edit
  connect(m_Actions.action("EditPreferences"), SIGNAL(triggered()),
          this, SLOT(whenPreferencesAsked()));

  // Development
  connect(m_Actions.action("DevNewSimulator"), SIGNAL(triggered()),
          this, SLOT(whenNewSimulatorSrcAsked()));
  connect(m_Actions.action("DevNewGhostSimulator"), SIGNAL(triggered()),
            this, SLOT(whenNewGhostSimulatorAsked()));
  connect(m_Actions.action("DevOpenSimulator"), SIGNAL(triggered()),
          this, SLOT(whenOpenSimulatorSrcAsked()));
  connect(m_Actions.action("DevNewObserver"), SIGNAL(triggered()),
          this, SLOT(whenNewObserverSrcAsked()));
  connect(m_Actions.action("DevOpenObserver"), SIGNAL(triggered()),
          this, SLOT(whenOpenObserverSrcAsked()));
  connect(m_Actions.action("DevLaunchDevStudio"), SIGNAL(triggered()),
          this, SLOT(whenLaunchDevStudioAsked()));

  // Simulation
  connect(m_Actions.action("WaresRefresh"), SIGNAL(triggered()),
          this, SLOT(whenWaresRefreshAsked()));
  connect(m_Actions.action("SimulationRun"), SIGNAL(triggered()),
          this, SLOT(whenRunAsked()));
  connect(m_Actions.action("SimulationModeDefault"), SIGNAL(triggered()),
          this, SLOT(whenRunDefaultModeAsked()));
  connect(m_Actions.action("SimulationModeCLI"), SIGNAL(triggered()),
          this, SLOT(whenRunCLIModeAsked()));

  // View
  connect(m_Actions.action("ViewDashboard"), SIGNAL(triggered()),
          this, SLOT(whenViewDashboardAsked()));
  connect(m_Actions.action("ViewToolbar"), SIGNAL(triggered()),
          this, SLOT(whenViewToolbarAsked()));
  connect(m_Actions.action("ViewRestore"), SIGNAL(triggered()),
          this, SLOT(whenViewRestoreAsked()));

  // Help
  connect(m_Actions.action("HelpOnlineWeb"), SIGNAL(triggered()),
          this, SLOT(whenOnlineWebAsked()));
  connect(m_Actions.action("HelpOnlineCommunity"), SIGNAL(triggered()),
          this, SLOT(whenOnlineCommunityAsked()));
  connect(m_Actions.action("HelpEmail"), SIGNAL(triggered()),
          this, SLOT(whenEmailAsked()));
  connect(m_Actions.action("HelpExamplesOpen"), SIGNAL(triggered()),
          this, SLOT(whenOpenExampleAsked()));
  connect(m_Actions.action("HelpExamplesRestore"), SIGNAL(triggered()),
          this, SLOT(whenRestoreExamplesAsked()));
  connect(m_Actions.action("HelpAbout"), SIGNAL(triggered()),
          this, SLOT(whenAboutAsked()));

  // Market
  connect(m_Actions.action("MarketAccess"), SIGNAL(triggered()),
          this, SLOT(whenMarketAsked()));


  // connection of recent open projects

  std::vector<QAction*> RecentActions = m_Actions.recentProjectActions();

  for (unsigned int i=0;i<RecentActions.size();i++)
    connect(RecentActions[i], SIGNAL(triggered()), this, SLOT(whenOpenRecentAsked()));

  m_MainWindow.setQuitAction(m_Actions.action("ProjectQuit"));

  QString TmpLabel = tr("Current workspace: %1")
                     .arg(QDir::toNativeSeparators(openfluid::base::PreferencesManager::instance()
                                                     ->getBuilderWorkspacePath()));
  m_MainWindow.statusBar()->addPermanentWidget(new QLabel(TmpLabel),1);
}


// =====================================================================
// =====================================================================


AppCoordinator::~AppCoordinator()
{

}


// =====================================================================
// =====================================================================


void AppCoordinator::connectExtensions()
{
  std::map<openfluid::ware::WareID_t,QAction*>::const_iterator it;
  std::map<openfluid::ware::WareID_t,QAction*>::const_iterator itb = m_Actions.extensionsActions().begin();
  std::map<openfluid::ware::WareID_t,QAction*>::const_iterator ite = m_Actions.extensionsActions().end();

  for (it = itb; it != ite; ++it)
    connect((*it).second,SIGNAL(triggered()),this, SLOT(whenExtensionAsked()));
}


// =====================================================================
// =====================================================================


void AppCoordinator::unsetCurrentModule()
{
  if (mp_CurrentModule != nullptr)
    delete mp_CurrentModule;
  mp_CurrentModule = nullptr;

  if (mp_DockWidget != nullptr)
    delete mp_DockWidget;
  mp_DockWidget = nullptr;
}


// =====================================================================
// =====================================================================


void AppCoordinator::setCurrentModule(AbstractModuleWidget* Module)
{
  unsetCurrentModule();
  mp_CurrentModule = Module;

  m_MainWindow.setWidget(Module);
}


// =====================================================================
// =====================================================================


void AppCoordinator::setHomeModule()
{
  if (mp_DockWidget != nullptr)
  {
    m_MainWindow.removeDockWidget(mp_DockWidget);
    delete mp_DockWidget;
    mp_DockWidget = nullptr;
  }

  AbstractModuleWidget* Module = new HomeModuleWidget(&m_Actions,&m_MainWindow);

  setCurrentModule(Module);

  m_Actions.setHomeMode();

  m_MainWindow.setProjectName();
}


// =====================================================================
// =====================================================================


bool AppCoordinator::setProjectModule(const QString& ProjectPath)
{
  try
  {
    unsetCurrentModule();

    AbstractModuleWidget* Module = new ProjectModuleWidget(ProjectPath,&m_MainWindow);

    setCurrentModule(Module);

    m_Actions.setProjectMode();


    if (mp_DockWidget == nullptr)
      mp_DockWidget = new QDockWidget(tr("Project dashboard"),&m_MainWindow);

    DashboardFrame* DockedWidget =
      static_cast<DashboardFrame*>(static_cast<ProjectModuleWidget*>(Module)->dockWidgetRebuilt(mp_DockWidget));
    DockedWidget->updateOrientation(openfluid::base::PreferencesManager::instance()->getBuilderDockPosition());

    mp_DockWidget->setObjectName("DockWidget");
    mp_DockWidget->setWidget(DockedWidget);
    mp_DockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
                                   Qt::RightDockWidgetArea |
                                   Qt::BottomDockWidgetArea);

    mp_DockWidget->setStyleSheet(QString("QDockWidget {padding: 5px; font: bold; background: %1;} "
                                         "QDockWidget::title {padding : 5px; font: bold; }")
                                        .arg(openfluid::ui::config::TOOLBAR_BGCOLOR));

    m_MainWindow.addDockWidget(openfluid::base::PreferencesManager::instance()->getBuilderDockPosition(),
                               mp_DockWidget);

    connect(mp_DockWidget,SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
            DockedWidget,SLOT(updateOrientation(Qt::DockWidgetArea)));

    connect(mp_DockWidget,SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
            this,SLOT(saveDockArea(Qt::DockWidgetArea)));


    connect(static_cast<ProjectModuleWidget*>(mp_CurrentModule),
            SIGNAL(fluidxChanged(openfluid::builderext::FluidXUpdateFlags::Flags)),
            this,SLOT(enableSave()));

    connect(static_cast<ProjectModuleWidget*>(mp_CurrentModule),SIGNAL(savePerformed()),
            this,SLOT(disableSave()));

    connect(static_cast<ProjectModuleWidget*>(mp_CurrentModule),SIGNAL(runEnabled(bool)),
            this,SLOT(enableRun(bool)));

    connect(static_cast<ProjectModuleWidget*>(mp_CurrentModule),SIGNAL(refreshWaresEnabled(bool)),
            m_Actions.action("WaresRefresh"),SLOT(setEnabled(bool)));

    enableRun(static_cast<ProjectModuleWidget*>(Module)->isOkForSimulation());

    m_Actions.action("WaresRefresh")
        ->setEnabled(!openfluid::base::PreferencesManager::instance()->isBuilderWaresWatchersActive());
  }
  catch (openfluid::base::Exception& E)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::critical(nullptr,tr("Project error"),QString(E.getMessage().c_str()));
    setHomeModule();
    return false;
  }

  return true;
}


// =====================================================================
// =====================================================================


bool AppCoordinator::createProject(const QString& Name, const QString& Path,
                                   const QString& Description, const QString& Authors,
                                   NewProjectDialog::ImportType IType, const QString& ISource)
{

  openfluid::base::RunContextManager* PrjMan = openfluid::base::RunContextManager::instance();
  openfluid::base::PreferencesManager* PrefsMan = openfluid::base::PreferencesManager::instance();

  if (!PrjMan->createProject(Path.toStdString(), Name.toStdString(),
                             Description.toStdString(), Authors.toStdString(),
                             false))
    return false;

  if (IType == NewProjectDialog::IMPORT_NONE)
  {
    openfluid::base::IOListener Listener;
    openfluid::fluidx::FluidXDescriptor FXD(&Listener);
    openfluid::core::DateTime DT;
    DT.setFromISOString(PrefsMan->getBuilderBegin().toStdString());
    FXD.runConfiguration().setBeginDate(DT);
    DT.setFromISOString(PrefsMan->getBuilderEnd().toStdString());
    FXD.runConfiguration().setEndDate(DT);
    FXD.runConfiguration().setDeltaT(PrefsMan->getBuilderDeltaT());
    FXD.runConfiguration().setFilled(true);

    FXD.writeToManyFiles(PrjMan->getInputDir());

    return true;
  }
  else if (IType == NewProjectDialog::IMPORT_PROJECT)
  {
    openfluid::tools::copyDirectoryContentsRecursively(ISource.toStdString()+"/IN",PrjMan->getInputDir());
    return true;
  }
  else
  {
    openfluid::tools::copyDirectoryContentsRecursively(ISource.toStdString(),PrjMan->getInputDir());
    return true;
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::openProject(const QString& Name, const QString& Path)
{
  // update recents projects
  openfluid::base::PreferencesManager::instance()->addBuilderRecentProject(
        QString::fromStdString(openfluid::base::RunContextManager::instance()->getProjectName()),
        QString::fromStdString(openfluid::base::RunContextManager::instance()->getProjectPath()));

  if (setProjectModule(Path))
  {
    m_Actions.updateRecentProjectsActions();
    m_MainWindow.setProjectName(Name);
  }
}


// =====================================================================
// =====================================================================


bool AppCoordinator::closeProject()
{
  if (m_Actions.action("ProjectSave")->isEnabled())
  {
    QMessageBox::StandardButton Ret =
        QMessageBox::question(&m_MainWindow,
                              tr("Close project"),
                              tr("Do you want to save the current project before closing?"),
                              QMessageBox::Cancel | QMessageBox::Save | QMessageBox::Discard);

    if (Ret != QMessageBox::Cancel)
    {
      if (Ret == QMessageBox::Save)
        whenSaveAsked();

      m_Actions.action("ProjectSave")->setEnabled(false);
      openfluid::base::RunContextManager::instance()->closeProject();

      return true;
    }
    return false;
  }

  openfluid::base::RunContextManager::instance()->closeProject();

  return true;
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenQuitAsked()
{
  if (mp_CurrentModule->whenQuitAsked())
  {
    bool ReallyQuit = (QMessageBox::question(&m_MainWindow,tr("Quit"),
                                             tr("Are you sure you want to quit OpenFLUID-Builder?"),
                                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes);

    if ( ReallyQuit && closeProject())
      QCoreApplication::exit(0);
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenNewAsked()
{
  if (mp_CurrentModule->whenNewAsked() && closeProject())
  {
    NewProjectDialog NewPrjDlg(&m_MainWindow);
    if (NewPrjDlg.exec() == QDialog::Accepted)
    {
      try
      {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        if (createProject(NewPrjDlg.getProjectName(),NewPrjDlg.getProjectFullPath(),
                          NewPrjDlg.getProjectDescription(), NewPrjDlg.getProjectAuthors(),
                          NewPrjDlg.getImportType(), NewPrjDlg.getImportSource()))
        {
          openProject(NewPrjDlg.getProjectName(),NewPrjDlg.getProjectFullPath());
          QApplication::restoreOverrideCursor();
        }
        else
        {
          QApplication::restoreOverrideCursor();
          QMessageBox::critical(&m_MainWindow,
                                tr("Project error"),
                                tr("Error creating project %1 in\n%2").arg(NewPrjDlg.getProjectName(),
                                                                           NewPrjDlg.getProjectFullPath()));
        }
      }
      catch (openfluid::base::Exception& E)
      {
        QMessageBox::critical(&m_MainWindow,
                              tr("Project error"),
                              tr("Error creating project %1 in\n%2").arg(NewPrjDlg.getProjectName(),
                                                                         NewPrjDlg.getProjectFullPath()));
        QApplication::restoreOverrideCursor();
      }
    }


  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenOpenAsked()
{
  if (mp_CurrentModule->whenOpenAsked() && closeProject())
  {
    // TODO develop custom dialog for opening projects
    QString SelectedDir =
        QFileDialog::getExistingDirectory(&m_MainWindow,tr("Open project"),
                                          openfluid::base::PreferencesManager::instance()->getBuilderProjectsPath());
    if (SelectedDir !=  "")
    {
      if (openfluid::base::RunContextManager::isProject(SelectedDir.toStdString()))
      {
        openfluid::base::RunContextManager::instance()->openProject(SelectedDir.toStdString());

        try
        {
          QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
          openProject(QDir(SelectedDir).dirName(),SelectedDir);
          QApplication::restoreOverrideCursor();
        }
        catch (openfluid::base::Exception& E)
        {
          openfluid::base::RunContextManager::instance()->closeProject();
          QApplication::restoreOverrideCursor();
          QMessageBox::critical(&m_MainWindow,tr("Project error"),QString(E.what()));
          return;
        }
      }
      else
      {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(&m_MainWindow,
                              tr("Project error"),
                              tr("%1\n\nis not a valid OpenFLUID project").arg(SelectedDir));
      }
    }
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenOpenRecentAsked()
{
  if (closeProject())
  {
    QAction *Action = qobject_cast<QAction*>(sender());
    if (Action)
    {
      QString ProjectPath = Action->data().toString();

      if (openfluid::base::RunContextManager::isProject(ProjectPath.toStdString()))
      {
        openfluid::base::RunContextManager::instance()->openProject(ProjectPath.toStdString());

        try
        {
          QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
          openProject(QDir(ProjectPath).dirName(),ProjectPath);
          QApplication::restoreOverrideCursor();
        }
        catch (openfluid::base::Exception& E)
        {
          openfluid::base::RunContextManager::instance()->closeProject();
          QApplication::restoreOverrideCursor();
          QMessageBox::critical(&m_MainWindow,tr("Project error"),QString(E.what()));
          return;
        }
      }
      else
      {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(&m_MainWindow,
                              tr("Project error"),
                              tr("%1\n\nis not a valid OpenFLUID project").arg(ProjectPath));
      }
    }
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenReloadAsked()
{
  QString ProjectDir = QString::fromStdString(openfluid::base::RunContextManager::instance()->getProjectPath());

  if (mp_CurrentModule->whenReloadAsked())
  {
    openfluid::base::RunContextManager::instance()->closeProject();

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    if (ProjectDir !=  "")
    {
      if (openfluid::base::RunContextManager::isProject(ProjectDir.toStdString()))
      {
        unsetCurrentModule();

        openfluid::base::RunContextManager::instance()->openProject(ProjectDir.toStdString());

        try
        {
          openProject(QDir(ProjectDir).dirName(),ProjectDir);
          QApplication::restoreOverrideCursor();
        }
        catch (openfluid::base::Exception& E)
        {
          openfluid::base::RunContextManager::instance()->closeProject();
          QApplication::restoreOverrideCursor();
          QMessageBox::critical(&m_MainWindow,tr("Project error"),QString(E.what()));
          return;
        }
      }
      else
      {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(&m_MainWindow,
                              tr("Project error"),
                              tr("%1\n\nis not a valid OpenFLUID project").arg(ProjectDir));
      }
    }
    else
    {
      QApplication::restoreOverrideCursor();
    }
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenSaveAsked()
{
  mp_CurrentModule->whenSaveAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenSaveAsAsked()
{
  if (mp_CurrentModule->whenSaveAsAsked())
  {
    openfluid::base::RunContextManager* PrjMan = openfluid::base::RunContextManager::instance();

    openfluid::base::PreferencesManager::instance()->addBuilderRecentProject(
          QString::fromStdString(PrjMan->getProjectName()),
          QString::fromStdString(PrjMan->getProjectPath()));

    m_Actions.updateRecentProjectsActions();
    m_MainWindow.setProjectName(QString::fromStdString(PrjMan->getProjectName()));
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenPropertiesAsked()
{
  mp_CurrentModule->whenPropertiesAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenExploreAsked()
{
  mp_CurrentModule->whenExploreAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenCloseAsked()
{
  if (mp_CurrentModule->whenCloseAsked() && closeProject())
  {
    setHomeModule();
  }

}


// =====================================================================
// =====================================================================


void AppCoordinator::whenPreferencesAsked()
{
  bool RestartRequired = mp_CurrentModule->whenPreferencesAsked();

  m_Actions.updateRecentProjectsActions();
  mp_CurrentModule->whenRecentProjectsActionsChanged();

  if (RestartRequired && openfluid::ui::common::OpenFLUIDDialog::confirmRestartAfterPreferences(&m_MainWindow))
  {
    QCoreApplication::exit(openfluid::ui::config::EXIT_CODE_FOR_RESTART);
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenWaresRefreshAsked()
{
  mp_CurrentModule->whenWaresRefreshAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenRunAsked()
{
  mp_CurrentModule->whenRunAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenRunDefaultModeAsked()
{
  mp_CurrentModule->whenRunModeAsked(ProjectCentral::RunMode::DEFAULT);
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenRunCLIModeAsked()
{
  mp_CurrentModule->whenRunModeAsked(ProjectCentral::RunMode::CLI);
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenExtensionAsked()
{
  QAction* Sender = (QAction*)(QObject::sender());
  if ( Sender != nullptr)
    mp_CurrentModule->whenExtensionAsked(Sender->data().toString());
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenViewDashboardAsked()
{
  if (mp_DockWidget == nullptr)
    return;

  if (mp_DockWidget->isVisible())
    mp_DockWidget->setVisible(false);
  else
    mp_DockWidget->setVisible(true);
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenViewToolbarAsked()
{
  if (!m_Actions.mainToolbar())
    return;

  if (m_Actions.mainToolbar()->isVisible())
    m_Actions.mainToolbar()->setVisible(false);
  else
    m_Actions.mainToolbar()->setVisible(true);
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenViewRestoreAsked()
{
  m_MainWindow.removeDockWidget(mp_DockWidget);
  m_MainWindow.addDockWidget(Qt::LeftDockWidgetArea, mp_DockWidget);
  openfluid::base::PreferencesManager::instance()->setBuilderDockPosition(Qt::LeftDockWidgetArea);
  mp_DockWidget->setFloating(false);
  mp_DockWidget->setVisible(true);

  if (m_Actions.mainToolbar())
    m_Actions.mainToolbar()->setVisible(true);
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenMarketAsked()
{
  mp_CurrentModule->whenMarketAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenOnlineWebAsked()
{
  QDesktopServices::openUrl(QUrl(QString::fromStdString(openfluid::config::WEBSITE_URL), QUrl::TolerantMode));
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenOnlineCommunityAsked()
{
  QDesktopServices::openUrl(QUrl(QString::fromStdString(openfluid::config::COMMUNITY_URL), QUrl::TolerantMode));
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenEmailAsked()
{
  QDesktopServices::openUrl(QUrl(QString::fromStdString(openfluid::config::CONTACTEMAIL_URL), QUrl::TolerantMode));
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenOpenExampleAsked()
{
  if (mp_CurrentModule->whenOpenExampleAsked() && closeProject())
  {
    OpenExampleProjectDialog OpenExDlg(&m_MainWindow);
    if (OpenExDlg.exec() == QDialog::Accepted)
    {
      QString SelectedDir = OpenExDlg.getSelectedProjectPath();

      if (openfluid::base::RunContextManager::isProject(SelectedDir.toStdString()))
      {
        openfluid::base::RunContextManager::instance()->openProject(SelectedDir.toStdString());

        try
        {
          QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
          openProject(QDir(SelectedDir).dirName(),SelectedDir);
          QApplication::restoreOverrideCursor();
        }
        catch (openfluid::base::Exception& E)
        {
          openfluid::base::RunContextManager::instance()->closeProject();
          QApplication::restoreOverrideCursor();
          QMessageBox::critical(&m_MainWindow,tr("Project error"),QString(E.what()));
          return;
        }
      }
      else
      {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(&m_MainWindow,
                              tr("Project error"),
                              tr("%1\n\nis not a valid OpenFLUID project").arg(SelectedDir));
      }
    }
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenRestoreExamplesAsked()
{
  if (QMessageBox::question(&m_MainWindow,tr("Reinstall examples projects"),
                            tr("Reinstalling will overwrite all modifications "
                                "and delete simulations results associated to these examples.")+
                            "\n\n"+
                            tr("Proceed anyway?"),
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    openfluid::buddies::BuddiesListener Listener;

    openfluid::buddies::ExamplesBuddy Buddy(&Listener);
    Buddy.parseOptions("force=1");
    try
    {
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
      Buddy.run();
      QApplication::restoreOverrideCursor();
    }
    catch (openfluid::base::Exception& e)
    {
      QApplication::restoreOverrideCursor();
      QMessageBox::critical(&m_MainWindow,tr("Restore examples projects"),
                            tr("Error restoring example projects"));
    }
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenAboutAsked()
{
  openfluid::ui::common::AboutDialog AboutDlg(&m_MainWindow,
                                              m_Actions.action("HelpOnlineWeb"),
                                              m_Actions.action("HelpEmail"));

  AboutDlg.exec();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenNewSimulatorSrcAsked()
{
  mp_CurrentModule->whenNewSimulatorSrcAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenNewGhostSimulatorAsked()
{
  mp_CurrentModule->whenNewGhostSimulatorAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenOpenSimulatorSrcAsked()
{
  mp_CurrentModule->whenOpenSimulatorSrcAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenNewObserverSrcAsked()
{
  mp_CurrentModule->whenNewObserverSrcAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenOpenObserverSrcAsked()
{
  mp_CurrentModule->whenOpenObserverSrcAsked();

}


// =====================================================================
// =====================================================================


void AppCoordinator::whenLaunchDevStudioAsked()
{
  mp_CurrentModule->whenLaunchDevStudioAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::enableSave()
{
  m_Actions.action("ProjectSave")->setEnabled(true);
}


// =====================================================================
// =====================================================================


void AppCoordinator::disableSave()
{
  m_Actions.action("ProjectSave")->setEnabled(false);
}


// =====================================================================
// =====================================================================


void AppCoordinator::saveDockArea(Qt::DockWidgetArea Area)
{
  openfluid::base::PreferencesManager::instance()->setBuilderDockPosition(Area);
}


// =====================================================================
// =====================================================================


void AppCoordinator::enableRun(bool Enabled)
{
  m_Actions.action("SimulationRun")->setEnabled(Enabled);
}
