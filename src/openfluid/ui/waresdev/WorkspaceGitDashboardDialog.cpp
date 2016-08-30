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
  @file WorkspaceGitDashboardDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <QThread>

#include <openfluid/ui/waresdev/WorkspaceGitDashboardDialog.hpp>
#include <openfluid/ui/waresdev/GitDashboardItemWidget.hpp>

#include "ui_WorkspaceGitDashboardDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


WorkspaceGitDashboardDialog::WorkspaceGitDashboardDialog(QWidget* Parent) :
    QDialog(Parent), ui(new Ui::WorkspaceGitDashboardDialog)
{
  ui->setupUi(this);

  ui->WaresTabWidget->setCurrentIndex(0);

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));


  QThread* WThread = new QThread;
  WorkspaceGitDashboardWorker* Worker = new WorkspaceGitDashboardWorker();
  Worker->moveToThread(WThread);

  connect(WThread, SIGNAL(started()), Worker, SLOT(run()));
  connect(Worker, SIGNAL(finished()), WThread, SLOT(quit()));
  connect(Worker, SIGNAL(finished()), Worker, SLOT(deleteLater()));
  connect(WThread, SIGNAL(finished()), WThread, SLOT(deleteLater()));

  qRegisterMetaType<openfluid::ui::waresdev::WorkspaceGitDashboardWorker::WareStatusInfo>(
      "openfluid::ui::waresdev::WorkspaceGitDashboardWorker::WareStatusInfo");
  connect(Worker, SIGNAL(wareParsed(openfluid::ui::waresdev::WorkspaceGitDashboardWorker::WareStatusInfo)),
          this, SLOT(addWareInfos(openfluid::ui::waresdev::WorkspaceGitDashboardWorker::WareStatusInfo)));

  WThread->start();
}



// =====================================================================
// =====================================================================


WorkspaceGitDashboardDialog::~WorkspaceGitDashboardDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WorkspaceGitDashboardDialog::addWareInfos(
    openfluid::ui::waresdev::WorkspaceGitDashboardWorker::WareStatusInfo Infos)
{
  QWidget* AreaContent = nullptr;
  int CurrentCounter = 0;

  if (Infos.Type == openfluid::ware::WareType::SIMULATOR)
  {
    AreaContent = ui->SimScrollAreaWidgetContents;
    m_SimCount++;
    CurrentCounter = m_SimCount;
  }
  else if (Infos.Type == openfluid::ware::WareType::OBSERVER)
  {
    AreaContent = ui->ObsScrollAreaWidgetContents;
    m_ObsCount++;
    CurrentCounter = m_ObsCount;
  }
  else if (Infos.Type == openfluid::ware::WareType::BUILDEREXT)
  {
    AreaContent = ui->BextScrollAreaWidgetContents;
    m_BextCount++;
    CurrentCounter = m_BextCount;
  }
  else
    return;


  GitDashboardItemWidget* Item = new GitDashboardItemWidget(Infos,AreaContent);

  if (CurrentCounter % 2)
    Item->setStyleSheet("background-color: #ffffff;");
  else
    Item->setStyleSheet("background-color: #f5faff;");

  static_cast<QBoxLayout*>(AreaContent->layout())->insertWidget(CurrentCounter-1,Item);

}


} } } // namespaces
