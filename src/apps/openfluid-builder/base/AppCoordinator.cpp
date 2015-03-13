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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/base/Exception.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/buddies/ExamplesBuddy.hpp>
#include <openfluid/ui/common/AboutDialog.hpp>
#include <openfluid/tools/FileHelpers.hpp>

#include "AppCoordinator.hpp"
#include "MainWindow.hpp"
#include "AppActions.hpp"
#include "OpenExampleProjectDialog.hpp"
#include "HomeModule.hpp"
#include "ProjectModule.hpp"
#include "DashboardFrame.hpp"
#include "builderconfig.hpp"


AppCoordinator::AppCoordinator(MainWindow& MainWin, AppActions& Actions):
  m_MainWindow(MainWin),m_Actions(Actions),
  mp_DockWidget(NULL), mp_CurrentModule(NULL)
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
  connect(m_Actions.action("ProjectClose"), SIGNAL(triggered()),
          this, SLOT(whenCloseAsked()));

  // Edit
  connect(m_Actions.action("EditPreferences"), SIGNAL(triggered()),
          this, SLOT(whenPreferencesAsked()));

  // Development
  connect(m_Actions.action("DevNewSimulator"), SIGNAL(triggered()),
          this, SLOT(whenNewSimulatorSrcAsked()));
  connect(m_Actions.action("DevOpenSimulator"), SIGNAL(triggered()),
          this, SLOT(whenOpenSimulatorSrcAsked()));
  connect(m_Actions.action("DevNewObserver"), SIGNAL(triggered()),
          this, SLOT(whenNewObserverSrcAsked()));
  connect(m_Actions.action("DevOpenObserver"), SIGNAL(triggered()),
          this, SLOT(whenOpenObserverSrcAsked()));
  connect(m_Actions.action("DevLaunchDevStudio"), SIGNAL(triggered()),
          this, SLOT(whenLaunchDevStudioAsked()));

  // Simulation
  connect(m_Actions.action("SimulationRun"), SIGNAL(triggered()),
          this, SLOT(whenRunAsked()));
  connect(m_Actions.action("WaresRefresh"), SIGNAL(triggered()),
          this, SLOT(whenWaresRefreshAsked()));

  // View
  connect(m_Actions.action("ViewDashboard"), SIGNAL(triggered()),
          this, SLOT(whenViewDashboardAsked()));
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
  if (mp_CurrentModule != NULL) delete mp_CurrentModule;
  mp_CurrentModule = NULL;

  if (mp_DockWidget != NULL) delete mp_DockWidget;
  mp_DockWidget = NULL;
}


// =====================================================================
// =====================================================================


void AppCoordinator::setCurrentModule(AbstractModule* Module)
{
  unsetCurrentModule();
  mp_CurrentModule = Module;

  m_MainWindow.setWidget(Module->mainWidgetRebuilt(&m_MainWindow));
}


// =====================================================================
// =====================================================================


void AppCoordinator::setHomeModule()
{
  if (mp_DockWidget != NULL)
  {
    m_MainWindow.removeDockWidget(mp_DockWidget);
    delete mp_DockWidget;
    mp_DockWidget = NULL;
  }

  AbstractModule* Module = new HomeModule(&m_Actions);

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
    AbstractModule* Module = new ProjectModule(ProjectPath);

    unsetCurrentModule();
    setCurrentModule(Module);

    m_Actions.setProjectMode();


    if (mp_DockWidget == NULL)
      mp_DockWidget = new QDockWidget(tr("Project dashboard"),&m_MainWindow);

    DashboardFrame* DockedWidget = (DashboardFrame*)(((ProjectModule*)Module)->dockWidgetRebuilt(mp_DockWidget));
    DockedWidget->updateOrientation(openfluid::base::PreferencesManager::instance()->getDockPosition());

    mp_DockWidget->setObjectName("DockWidget");
    mp_DockWidget->setWidget(DockedWidget);
    mp_DockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
                                   Qt::RightDockWidgetArea |
                                   Qt::BottomDockWidgetArea);

    mp_DockWidget->setStyleSheet(QString("QDockWidget {padding: 5px; font: bold; background: %1;} "
                                         "QDockWidget::title {padding : 5px; font: bold; }")
                                        .arg(BUILDER_TOOLBAR_BGCOLOR));

    m_MainWindow.addDockWidget(openfluid::base::PreferencesManager::instance()->getDockPosition(),
                               mp_DockWidget);

    connect(mp_DockWidget,SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
            DockedWidget,SLOT(updateOrientation(Qt::DockWidgetArea)));

    connect(mp_DockWidget,SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
            this,SLOT(saveDockArea(Qt::DockWidgetArea)));


    connect((ProjectModule*)mp_CurrentModule,SIGNAL(fluidxChanged(openfluid::builderext::FluidXUpdateFlags::Flags)),
            this,SLOT(enableSave()));

    connect((ProjectModule*)mp_CurrentModule,SIGNAL(savePerformed()),
            this,SLOT(disableSave()));

    connect((ProjectModule*)mp_CurrentModule,SIGNAL(runEnabled(bool)),
            this,SLOT(enableRun(bool)));

    connect((ProjectModule*)mp_CurrentModule,SIGNAL(refreshWaresEnabled(bool)),
            m_Actions.action("WaresRefresh"),SLOT(setEnabled(bool)));

    enableRun(((ProjectModule*)Module)->isOkForSimulation());

    m_Actions.action("WaresRefresh")
        ->setEnabled(!openfluid::base::PreferencesManager::instance()->isWaresWatchersActive());
  }
  catch (openfluid::base::Exception& E)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::critical(NULL,tr("Project error"),QString(E.getMessage().c_str()));
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

  openfluid::base::ProjectManager* PrjMan = openfluid::base::ProjectManager::instance();
  openfluid::base::PreferencesManager* PrefsMan = openfluid::base::PreferencesManager::instance();

  if (!PrjMan->create(Path.toStdString(), Name.toStdString(),
                      Description.toStdString(), Authors.toStdString(),
                      false))
    return false;

  if (IType == NewProjectDialog::IMPORT_NONE)
  {
    openfluid::fluidx::FluidXDescriptor FXD(NULL);
    openfluid::core::DateTime DT;
    DT.setFromISOString(PrefsMan->getBegin().toStdString());
    FXD.runDescriptor().setBeginDate(DT);
    DT.setFromISOString(PrefsMan->getEnd().toStdString());
    FXD.runDescriptor().setEndDate(DT);
    FXD.runDescriptor().setDeltaT(PrefsMan->getDeltaT());
    FXD.runDescriptor().setFilled(true);

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
  openfluid::base::PreferencesManager::instance()->addRecentProject(
        QString::fromStdString(openfluid::base::ProjectManager::instance()->getName()),
        QString::fromStdString(openfluid::base::ProjectManager::instance()->getPath()));

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
      if (Ret == QMessageBox::Save) whenSaveAsked();
      m_Actions.action("ProjectSave")->setEnabled(false);
      openfluid::base::ProjectManager::instance()->close();
      return true;
    }
    return false;
  }
  openfluid::base::ProjectManager::instance()->close();
  return true;
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenQuitAsked()
{
  if (mp_CurrentModule->whenQuitAsked())
  {
    if (closeProject())
    {
      if (QMessageBox::question(&m_MainWindow,tr("Quit"),
                                tr("Are you sure you want to quit OpenFLUID-Builder?"),
                                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
      {
        QCoreApplication::exit(0);
      }
      else
      {
        setHomeModule();
      }
    }
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
                                          openfluid::base::PreferencesManager::instance()->getProjectsPath());
    if (SelectedDir !=  "")
    {
      if (openfluid::base::ProjectManager::isProject(SelectedDir.toStdString()))
      {
        openfluid::base::ProjectManager::instance()->open(SelectedDir.toStdString());

        try
        {
          QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
          openProject(QDir(SelectedDir).dirName(),SelectedDir);
          QApplication::restoreOverrideCursor();
        }
        catch (openfluid::base::Exception& E)
        {
          openfluid::base::ProjectManager::instance()->close();
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

      if (openfluid::base::ProjectManager::isProject(ProjectPath.toStdString()))
      {
        openfluid::base::ProjectManager::instance()->open(ProjectPath.toStdString());

        try
        {
          QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
          openProject(QDir(ProjectPath).dirName(),ProjectPath);
          QApplication::restoreOverrideCursor();
        }
        catch (openfluid::base::Exception& E)
        {
          openfluid::base::ProjectManager::instance()->close();
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
  QString ProjectDir = QString::fromStdString(openfluid::base::ProjectManager::instance()->getPath());

  if (mp_CurrentModule->whenReloadAsked())
  {
    openfluid::base::ProjectManager::instance()->close();

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    if (ProjectDir !=  "")
    {
      if (openfluid::base::ProjectManager::isProject(ProjectDir.toStdString()))
      {
        unsetCurrentModule();

        openfluid::base::ProjectManager::instance()->open(ProjectDir.toStdString());

        try
        {
          openProject(QDir(ProjectDir).dirName(),ProjectDir);
          QApplication::restoreOverrideCursor();
        }
        catch (openfluid::base::Exception& E)
        {
          openfluid::base::ProjectManager::instance()->close();
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
    openfluid::base::ProjectManager* PrjMan = openfluid::base::ProjectManager::instance();

    openfluid::base::PreferencesManager::instance()->addRecentProject(
          QString::fromStdString(PrjMan->getName()),
          QString::fromStdString(PrjMan->getPath()));

    m_Actions.updateRecentProjectsActions();
    m_MainWindow.setProjectName(QString::fromStdString(PrjMan->getName()));
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
  mp_CurrentModule->whenPreferencesAsked();

  m_Actions.updateRecentProjectsActions();
  mp_CurrentModule->whenRecentProjectsActionsChanged();
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


void AppCoordinator::whenExtensionAsked()
{
  QAction* Sender = (QAction*)(QObject::sender());
  if ( Sender != NULL)
    mp_CurrentModule->whenExtensionAsked(Sender->data().toString());
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenViewDashboardAsked()
{
  if (mp_DockWidget == NULL) return;

  if (mp_DockWidget->isVisible())
    mp_DockWidget->setVisible(false);
  else
    mp_DockWidget->setVisible(true);
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenViewRestoreAsked()
{
  m_MainWindow.removeDockWidget(mp_DockWidget);
  m_MainWindow.addDockWidget(Qt::LeftDockWidgetArea, mp_DockWidget);
  openfluid::base::PreferencesManager::instance()->setDockPosition(Qt::LeftDockWidgetArea);
  mp_DockWidget->setFloating(false);
  mp_DockWidget->setVisible(true);
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
  QDesktopServices::openUrl(QUrl(BUILDER_URL_WEBSITE, QUrl::TolerantMode));
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenOnlineCommunityAsked()
{
  QDesktopServices::openUrl(QUrl(BUILDER_URL_COMMUNITY, QUrl::TolerantMode));
}


// =====================================================================
// =====================================================================

void AppCoordinator::whenEmailAsked()
{
  QDesktopServices::openUrl(QUrl(BUILDER_URL_EMAIL, QUrl::TolerantMode));
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

      if (openfluid::base::ProjectManager::isProject(SelectedDir.toStdString()))
      {
        openfluid::base::ProjectManager::instance()->open(SelectedDir.toStdString());

        try
        {
          QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
          openProject(QDir(SelectedDir).dirName(),SelectedDir);
          QApplication::restoreOverrideCursor();
        }
        catch (openfluid::base::Exception& E)
        {
          openfluid::base::ProjectManager::instance()->close();
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
    openfluid::buddies::ExamplesBuddy Buddy(NULL);
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
  openfluid::base::PreferencesManager::instance()->setDockPosition(Area);
}


// =====================================================================
// =====================================================================


void AppCoordinator::enableRun(bool Enabled)
{
  m_Actions.action("SimulationRun")->setEnabled(Enabled);
}
