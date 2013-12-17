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
  \file AppCoordinator.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>
#include <QApplication>

#include <openfluid/guicommon/OpenProjectDialog.hpp>
#include <openfluid/guicommon/PreferencesManager.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/base/Exception.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/buddies/ExamplesBuddy.hpp>

#include "AppCoordinator.hpp"
#include "MainWindow.hpp"
#include "AppActions.hpp"
#include "AboutDialog.hpp"
#include "OpenExampleProjectDialog.hpp"
#include "HomeModule.hpp"
#include "ProjectModule.hpp"
#include "DashboardFrame.hpp"
#include "builderconfig.hpp"


AppCoordinator::AppCoordinator(MainWindow& MainWin, AppActions& Actions):
  m_MainWindow(MainWin),m_Actions(Actions),
  mp_DockWidget(NULL), mp_CurrentModule(NULL)
{
  m_Actions.getAction("ProjectSave")->setEnabled(false);

  connect(m_Actions.getAction("ProjectQuit"), SIGNAL(triggered()),
          this, SLOT(whenQuitAsked()));
  connect(m_Actions.getAction("ProjectNew"), SIGNAL(triggered()),
          this, SLOT(whenNewAsked()));
  connect(m_Actions.getAction("ProjectOpen"), SIGNAL(triggered()),
          this, SLOT(whenOpenAsked()));
  connect(m_Actions.getAction("ProjectReload"), SIGNAL(triggered()),
            this, SLOT(whenReloadAsked()));
  connect(m_Actions.getAction("ProjectSave"), SIGNAL(triggered()),
          this, SLOT(whenSaveAsked()));
  connect(m_Actions.getAction("ProjectSaveAs"), SIGNAL(triggered()),
          this, SLOT(whenSaveAsAsked()));
  connect(m_Actions.getAction("ProjectProperties"), SIGNAL(triggered()),
          this, SLOT(whenPropertiesAsked()));
  connect(m_Actions.getAction("ProjectClose"), SIGNAL(triggered()),
          this, SLOT(whenCloseAsked()));

  connect(m_Actions.getAction("EditPreferences"), SIGNAL(triggered()),
          this, SLOT(whenPreferencesAsked()));

  connect(m_Actions.getAction("SimulationRun"), SIGNAL(triggered()),
          this, SLOT(whenRunAsked()));
  connect(m_Actions.getAction("WaresRefresh"), SIGNAL(triggered()),
          this, SLOT(whenWaresRefreshAsked()));

  connect(m_Actions.getAction("ViewDashboard"), SIGNAL(triggered()),
          this, SLOT(whenViewDashboardAsked()));
  connect(m_Actions.getAction("ViewRestore"), SIGNAL(triggered()),
          this, SLOT(whenViewRestoreAsked()));

  connect(m_Actions.getAction("HelpOnlineWeb"), SIGNAL(triggered()),
          this, SLOT(whenOnlineWebAsked()));
  connect(m_Actions.getAction("HelpOnlineCommunity"), SIGNAL(triggered()),
          this, SLOT(whenOnlineCommunityAsked()));
  connect(m_Actions.getAction("HelpEmail"), SIGNAL(triggered()),
          this, SLOT(whenEmailAsked()));
  connect(m_Actions.getAction("HelpExamplesOpen"), SIGNAL(triggered()),
          this, SLOT(whenOpenExampleAsked()));
  connect(m_Actions.getAction("HelpExamplesRestore"), SIGNAL(triggered()),
          this, SLOT(whenRestoreExamplesAsked()));
  connect(m_Actions.getAction("HelpAbout"), SIGNAL(triggered()),
          this, SLOT(whenAboutAsked()));

  connect(m_Actions.getAction("MarketAccess"), SIGNAL(triggered()),
          this, SLOT(whenMarketAsked()));


  // connection of recent open projects

  std::vector<QAction*> RecentActions = m_Actions.getRecentProjectActions();

  for (unsigned int i=0;i<RecentActions.size();i++)
    connect(RecentActions[i], SIGNAL(triggered()), this, SLOT(whenOpenRecentAsked()));

  m_MainWindow.setQuitAction(m_Actions.getAction("ProjectQuit"));

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
  std::map<openfluid::ware::WareID_t,QAction*>::const_iterator itb = m_Actions.getExtensionsActions().begin();
  std::map<openfluid::ware::WareID_t,QAction*>::const_iterator ite = m_Actions.getExtensionsActions().end();

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

  m_MainWindow.setWidget(Module->getMainWidget(&m_MainWindow));
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


void AppCoordinator::setProjectModule(const QString& ProjectPath)
{
  try
  {
    unsetCurrentModule();

    AbstractModule* Module = new ProjectModule(ProjectPath);

    setCurrentModule(Module);

    m_Actions.setProjectMode();


    if (mp_DockWidget == NULL)
      mp_DockWidget = new QDockWidget(tr("Project dashboard"),&m_MainWindow);

    DashboardFrame* DockedWidget = (DashboardFrame*)(((ProjectModule*)Module)->getDockWidget(mp_DockWidget));
    DockedWidget->updateOrientation(openfluid::guicommon::PreferencesManager::getInstance()->getDockPosition());

    mp_DockWidget->setObjectName("DockWidget");
    mp_DockWidget->setWidget(DockedWidget);
    mp_DockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
                                   Qt::RightDockWidgetArea |
                                   Qt::BottomDockWidgetArea);

    mp_DockWidget->setStyleSheet(QString("QDockWidget {padding: 5px; font: bold; background: %1;} "
                                         "QDockWidget::title {padding : 5px; font: bold; }")
                                        .arg(BUILDER_TOOLBAR_BGCOLOR));

    m_MainWindow.addDockWidget(openfluid::guicommon::PreferencesManager::getInstance()->getDockPosition(),
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
            m_Actions.getAction("WaresRefresh"),SLOT(setEnabled(bool)));

    enableRun(((ProjectModule*)Module)->isOkForSimulation());

    m_Actions.getAction("WaresRefresh")->setEnabled(!openfluid::guicommon::PreferencesManager::getInstance()->isWaresWatchersActive());
  }
  catch (openfluid::base::Exception& E)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::critical(NULL,tr("Project error"),QString(E.getMessage().c_str()));
  }
}


// =====================================================================
// =====================================================================


bool AppCoordinator::createProject(const QString& Name, const QString& Path, const QString& Description, const QString& Authors,
                                   NewProjectDialog::ImportType IType, const QString& ISource)
{

  openfluid::base::ProjectManager* PrjMan = openfluid::base::ProjectManager::getInstance();
  openfluid::guicommon::PreferencesManager* PrefsMan = openfluid::guicommon::PreferencesManager::getInstance();

  if (!PrjMan->create(Path.toStdString(), Name.toStdString(),
                      Description.toStdString(), Authors.toStdString(),
                      false))
    return false;

  if (IType == NewProjectDialog::IMPORT_NONE)
  {
    openfluid::fluidx::FluidXDescriptor FXD(NULL);
    openfluid::core::DateTime DT;
    DT.setFromISOString(PrefsMan->getBegin().toStdString());
    FXD.getRunDescriptor().setBeginDate(DT);
    DT.setFromISOString(PrefsMan->getEnd().toStdString());
    FXD.getRunDescriptor().setEndDate(DT);
    FXD.getRunDescriptor().setDeltaT(PrefsMan->getDeltaT());
    FXD.getRunDescriptor().setFilled(true);

    FXD.writeToManyFiles(PrjMan->getInputDir());

    return true;
  }
  else if (IType == NewProjectDialog::IMPORT_PROJECT)
  {
    openfluid::tools::CopyDirectoryContentsRecursively(ISource.toStdString()+"/IN",PrjMan->getInputDir());
    return true;
  }
  else
  {
    openfluid::tools::CopyDirectoryContentsRecursively(ISource.toStdString(),PrjMan->getInputDir());
    return true;
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::openProject(const QString& Name, const QString& Path)
{
  // update recents projects
  openfluid::guicommon::PreferencesManager::getInstance()->addRecentProject(
        QString::fromStdString(openfluid::base::ProjectManager::getInstance()->getName()),
        QString::fromStdString(openfluid::base::ProjectManager::getInstance()->getPath()));

  setProjectModule(Path);
  m_Actions.updateRecentProjectsActions();
  m_MainWindow.setProjectName(Name);
}


// =====================================================================
// =====================================================================


bool AppCoordinator::closeProject()
{
  if (m_Actions.getAction("ProjectSave")->isEnabled())
  {
    QMessageBox::StandardButton Ret = QMessageBox::question(&m_MainWindow,
                                                            tr("Close project"),
                                                            tr("Do you want to save the current project before closing?"),
                                                            QMessageBox::Cancel | QMessageBox::Save | QMessageBox::Discard);
    if (Ret != QMessageBox::Cancel)
    {
      if (Ret == QMessageBox::Save) whenSaveAsked();
      m_Actions.getAction("ProjectSave")->setEnabled(false);
      openfluid::base::ProjectManager::getInstance()->close();
      return true;
    }
    return false;
  }
  openfluid::base::ProjectManager::getInstance()->close();
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
          QMessageBox::critical(&m_MainWindow,tr("Project error"),tr("Error creating project ") + NewPrjDlg.getProjectName() + tr(" in\n") + NewPrjDlg.getProjectFullPath());
        }
      }
      catch (openfluid::base::Exception& E)
      {
        QMessageBox::critical(&m_MainWindow,tr("Project error"),tr("Error creating project ") + NewPrjDlg.getProjectName() + tr(" in\n") + NewPrjDlg.getProjectFullPath());
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
    // TODO develop custom open project
    /*openfluid::guicommon::OpenProjectDialog OpenPrjDlg(&m_MainWindow);
    if (OpenPrjDlg.run()) setProjectModule(OpenPrjDlg.getProjectFolder());*/
    QString SelectedDir = QFileDialog::getExistingDirectory(&m_MainWindow,tr("Open project"),
                                                            openfluid::guicommon::PreferencesManager::getInstance()->getWorkdir());
    if (SelectedDir !=  "")
    {
      if (openfluid::base::ProjectManager::isProject(SelectedDir.toStdString()))
      {
        openfluid::base::ProjectManager::getInstance()->open(SelectedDir.toStdString());

        try
        {
          QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
          openProject(QDir(SelectedDir).dirName(),SelectedDir);
          QApplication::restoreOverrideCursor();
        }
        catch (openfluid::base::Exception& E)
        {
          openfluid::base::ProjectManager::getInstance()->close();
          QApplication::restoreOverrideCursor();
          QMessageBox::critical(&m_MainWindow,tr("Project error"),QString(E.what()));
          return;
        }
      }
      else
      {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(&m_MainWindow,tr("Project error"),SelectedDir+ "\n\n" + tr("is not a valid OpenFLUID project"));
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
        openfluid::base::ProjectManager::getInstance()->open(ProjectPath.toStdString());

        try
        {
          QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
          openProject(QDir(ProjectPath).dirName(),ProjectPath);
          QApplication::restoreOverrideCursor();
        }
        catch (openfluid::base::Exception& E)
        {
          openfluid::base::ProjectManager::getInstance()->close();
          QApplication::restoreOverrideCursor();
          QMessageBox::critical(&m_MainWindow,tr("Project error"),QString(E.what()));
          return;
        }
      }
      else
      {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(&m_MainWindow,tr("Project error"),
                              ProjectPath+ "\n\n" + tr("is not a valid OpenFLUID project"));
      }
    }
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenReloadAsked()
{
  QString ProjectDir = QString::fromStdString(openfluid::base::ProjectManager::getInstance()->getPath());

  if (mp_CurrentModule->whenReloadAsked())
  {
    openfluid::base::ProjectManager::getInstance()->close();

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    if (ProjectDir !=  "")
    {
      if (openfluid::base::ProjectManager::isProject(ProjectDir.toStdString()))
      {
        unsetCurrentModule();

        openfluid::base::ProjectManager::getInstance()->open(ProjectDir.toStdString());

        try
        {
          openProject(QDir(ProjectDir).dirName(),ProjectDir);
          QApplication::restoreOverrideCursor();
        }
        catch (openfluid::base::Exception& E)
        {
          openfluid::base::ProjectManager::getInstance()->close();
          QApplication::restoreOverrideCursor();
          QMessageBox::critical(&m_MainWindow,tr("Project error"),QString(E.what()));
          return;
        }
      }
      else
      {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(&m_MainWindow,tr("Project error"),ProjectDir+ "\n\n" + tr("is not a valid OpenFLUID project"));
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
    openfluid::base::ProjectManager* PrjMan = openfluid::base::ProjectManager::getInstance();

    openfluid::guicommon::PreferencesManager::getInstance()->addRecentProject(
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
  openfluid::guicommon::PreferencesManager::getInstance()->setDockPosition(Qt::LeftDockWidgetArea);
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
        openfluid::base::ProjectManager::getInstance()->open(SelectedDir.toStdString());

        try
        {
          QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
          openProject(QDir(SelectedDir).dirName(),SelectedDir);
          QApplication::restoreOverrideCursor();
        }
        catch (openfluid::base::Exception& E)
        {
          openfluid::base::ProjectManager::getInstance()->close();
          QApplication::restoreOverrideCursor();
          QMessageBox::critical(&m_MainWindow,tr("Project error"),QString(E.what()));
          return;
        }
      }
      else
      {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(&m_MainWindow,tr("Project error"),SelectedDir+ "\n\n" + tr("is not a valid OpenFLUID project"));
      }
    }
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenRestoreExamplesAsked()
{
  if (QMessageBox::question(&m_MainWindow,tr("Reinstall examples projects"),
                            tr("Reinstalling will overwrite all modifications and delete simulations results associated to these examples.")+
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
  AboutDialog AboutDlg(&m_MainWindow,
                       m_Actions.getAction("HelpOnlineWeb"),
                       m_Actions.getAction("HelpEmail"));

  AboutDlg.exec();
}


// =====================================================================
// =====================================================================


void AppCoordinator::enableSave()
{
  m_Actions.getAction("ProjectSave")->setEnabled(true);
}


// =====================================================================
// =====================================================================


void AppCoordinator::disableSave()
{
  m_Actions.getAction("ProjectSave")->setEnabled(false);
}


// =====================================================================
// =====================================================================


void AppCoordinator::saveDockArea(Qt::DockWidgetArea Area)
{
  openfluid::guicommon::PreferencesManager::getInstance()->setDockPosition(Area);
}


// =====================================================================
// =====================================================================


void AppCoordinator::enableRun(bool Enabled)
{
  m_Actions.getAction("SimulationRun")->setEnabled(Enabled);
}
