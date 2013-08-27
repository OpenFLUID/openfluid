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
#include "AppCoordinator.hpp"
#include "MainWindow.hpp"
#include "AppActions.hpp"
#include "HomeModule.hpp"
#include "ProjectModule.hpp"
#include "builderconfig.hpp"


AppCoordinator::AppCoordinator(MainWindow& MainWin, AppActions& Actions):
  m_MainWindow(MainWin),m_Actions(Actions), mp_CurrentModule(NULL)
{
  connect(m_Actions.getAction("ProjectQuit"), SIGNAL(triggered()), this, SLOT(whenQuitAsked()));
  connect(m_Actions.getAction("ProjectNew"), SIGNAL(triggered()), this, SLOT(whenNewAsked()));
  connect(m_Actions.getAction("ProjectOpen"), SIGNAL(triggered()), this, SLOT(whenOpenAsked()));
  connect(m_Actions.getAction("ProjectSave"), SIGNAL(triggered()), this, SLOT(whenSaveAsked()));
  connect(m_Actions.getAction("ProjectSaveAs"), SIGNAL(triggered()), this, SLOT(whenSaveAsAsked()));
  connect(m_Actions.getAction("ProjectProperties"), SIGNAL(triggered()), this, SLOT(whenPropertiesAsked()));
  connect(m_Actions.getAction("ProjectClose"), SIGNAL(triggered()), this, SLOT(whenCloseAsked()));

  connect(m_Actions.getAction("EditPreferences"), SIGNAL(triggered()), this, SLOT(whenPreferencesAsked()));

  connect(m_Actions.getAction("SimulationRun"), SIGNAL(triggered()), this, SLOT(whenRunAsked()));
  connect(m_Actions.getAction("WaresRefresh"), SIGNAL(triggered()), this, SLOT(whenRefreshAsked()));

  connect(m_Actions.getAction("HelpOnlineWeb"), SIGNAL(triggered()), this, SLOT(whenOnlineWebAsked()));
  connect(m_Actions.getAction("HelpOnlineCommunity"), SIGNAL(triggered()), this, SLOT(whenOnlineCommunityAsked()));
  connect(m_Actions.getAction("HelpExamplesOpen"), SIGNAL(triggered()), this, SLOT(whenOpenExampleAsked()));
  connect(m_Actions.getAction("HelpExamplesRestore"), SIGNAL(triggered()), this, SLOT(whenRestoreExamplesAsked()));
  connect(m_Actions.getAction("HelpAbout"), SIGNAL(triggered()), this, SLOT(whenAboutAsked()));

  connect(m_Actions.getAction("MarketAccess"), SIGNAL(triggered()), this, SLOT(whenMarketAsked()));


  // connection of recent oen projects

  std::vector<QAction*> RecentActions = m_Actions.getRecentProjectActions();

  for (unsigned int i=0;i<RecentActions.size();i++)
    connect(RecentActions[i], SIGNAL(triggered()), this, SLOT(whenOpenRecentAsked()));

}


// =====================================================================
// =====================================================================


AppCoordinator::~AppCoordinator()
{

}


// =====================================================================
// =====================================================================


void AppCoordinator::unsetCurrentModule()
{
  if (mp_CurrentModule != NULL) delete mp_CurrentModule;
  mp_CurrentModule = NULL;
}


// =====================================================================
// =====================================================================


void AppCoordinator::setCurrentModule(AbstractModule* Module)
{
  unsetCurrentModule();
  mp_CurrentModule = Module;

  m_MainWindow.setWidget(Module->getWidget(&m_MainWindow));
}


// =====================================================================
// =====================================================================


void AppCoordinator::setHomeModule()
{
  AbstractModule* Module = new HomeModule(&m_Actions);

  setCurrentModule(Module);

  m_Actions.setHomeMode();

  m_MainWindow.setWindowTitle("OpenFLUID-Builder");

}


// =====================================================================
// =====================================================================


void AppCoordinator::setProjectModule(const QString& ProjectPath)
{
  AbstractModule* Module = new ProjectModule(ProjectPath);

  setCurrentModule(Module);

  m_Actions.setProjectMode();

}


// =====================================================================
// =====================================================================


void AppCoordinator::updateRecentsList()
{
  // add of the current project
  openfluid::guicommon::PreferencesManager::getInstance()->addRecentProject(
      QString(openfluid::base::ProjectManager::getInstance()->getName().c_str()),
      QString(openfluid::base::ProjectManager::getInstance()->getPath().c_str()));

  m_Actions.updateRecentProjectsActions();
}


// =====================================================================
// =====================================================================


bool AppCoordinator::createProject(const QString& Name, const QString& Path, const QString& Description, const QString& Authors,
                                   NewProjectDialog::ImportType IType, const QString& ISource)
{

  openfluid::base::ProjectManager* PrjMan = openfluid::base::ProjectManager::getInstance();
  openfluid::guicommon::PreferencesManager* PrefsMan = openfluid::guicommon::PreferencesManager::getInstance();

  if (!PrjMan->create(Path.toStdString(), Name.toStdString(), Description.toStdString(), Authors.toStdString(), false))
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
  setProjectModule(Path);
  updateRecentsList();
  m_MainWindow.setWindowTitle("OpenFLUID-Builder  [ " +  Name +" ]");
}


// =====================================================================
// =====================================================================



void AppCoordinator::whenQuitAsked()
{
  if (mp_CurrentModule->whenQuitAsked())
  {
    unsetCurrentModule();
    m_MainWindow.close();
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenNewAsked()
{
  std::cout << __PRETTY_FUNCTION__ << " import is missing" << std::endl;

  if (mp_CurrentModule->whenNewAsked())
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
  if (mp_CurrentModule->whenOpenAsked())
  {
    // TODO develop custom open project
    /*openfluid::guicommon::OpenProjectDialog OpenPrjDlg(&m_MainWindow);
    if (OpenPrjDlg.run()) setProjectModule(OpenPrjDlg.getProjectFolder());*/
    QString SelectedDir = QFileDialog::getExistingDirectory(&m_MainWindow,tr("Open project"));
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
  QAction *Action = qobject_cast<QAction*>(sender());
  if (Action)
  {
    std::cout << __PRETTY_FUNCTION__ << " " << Action->data().toString().toStdString() << std::endl;

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
      QMessageBox::critical(&m_MainWindow,tr("Project error"),ProjectPath+ "\n\n" + tr("is not a valid OpenFLUID project"));
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
  mp_CurrentModule->whenSaveAsAsked();
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
  if (mp_CurrentModule->whenCloseAsked())
  {
    setHomeModule();
  }
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenPreferencesAsked()
{
  mp_CurrentModule->whenPreferencesAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenRefreshAsked()
{
  mp_CurrentModule->whenRefreshAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenRunAsked()
{
  mp_CurrentModule->whenRunAsked();
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
  QDesktopServices::openUrl(QUrl(QString(BUILDER_URL_WEBSITE.c_str()), QUrl::TolerantMode));
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenOnlineCommunityAsked()
{
  QDesktopServices::openUrl(QUrl(QString(BUILDER_URL_COMMUNITY.c_str()), QUrl::TolerantMode));
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenOpenExampleAsked()
{
  mp_CurrentModule->whenOpenExampleAsked();
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenRestoreExamplesAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}


// =====================================================================
// =====================================================================


void AppCoordinator::whenAboutAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}
