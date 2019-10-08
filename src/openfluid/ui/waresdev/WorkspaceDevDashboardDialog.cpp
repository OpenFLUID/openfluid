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
  @file WorkspaceDevDashboardDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QMetaType>
#include <QThread>

#include <openfluid/config.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevDashboardDialog.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevDashboardWorker.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevDashboardTypes.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevProcessDialog.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevCheckWorker.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevPurgeWorker.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevBuildWorker.hpp>

#include "ui_WorkspaceDevDashboardDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


WorkspaceDevDashboardDialog::WorkspaceDevDashboardDialog(QWidget* Parent) :
    QDialog(Parent), ui(new Ui::WorkspaceDevDashboardDialog)
{
  ui->setupUi(this);

  ui->WaresTabWidget->setCurrentIndex(0);
  ui->ActionsTabWidget->setCurrentIndex(0);


  // Build and doc tab

  ui->BuildCheckBox->setChecked(true);
  connect(ui->BuildCheckBox,SIGNAL(toggled(bool)),this,SLOT(handleBuildChanged()));

#if OPENFLUID_SIM2DOC_ENABLED
  ui->DocCheckBox->setChecked(true);
  connect(ui->DocCheckBox,SIGNAL(toggled(bool)),this,SLOT(handleBuildChanged()));
#else
  ui->DocCheckBox->setChecked(false);
  ui->DocCheckBox->setVisible(false);
#endif

  connect(ui->BuildDocRunButton,SIGNAL(clicked()),this,SLOT(runBuildDoc()));


  // Check tab

  ui->WareshubCheckBox->setChecked(true);
  ui->ReadmeCheckBox->setChecked(true);
  ui->LicenseCheckBox->setChecked(true);

  connect(ui->WareshubCheckBox,SIGNAL(toggled(bool)),this,SLOT(handleCheckChanged()));
  connect(ui->ReadmeCheckBox,SIGNAL(toggled(bool)),this,SLOT(handleCheckChanged()));
  connect(ui->LicenseCheckBox,SIGNAL(toggled(bool)),this,SLOT(handleCheckChanged()));

  connect(ui->CheckRunButton,SIGNAL(clicked()),this,SLOT(runCheck()));


  // Purge tab

  ui->CurrentVersionCheckBox->setText(tr("Build files for current version (%1)")
                                      .arg(QString::fromStdString(
                                              openfluid::base::Environment::getVersionMajorMinor())));
  ui->OtherVersionsCheckBox->setText(tr("Build files for other versions"));
  ui->ReleaseModeCheckBox->setText(tr("Build files produced in release mode"));
  ui->DebugModeCheckBox->setText(tr("Build files produced in debug mode"));

  ui->CurrentVersionCheckBox->setChecked(true);
  ui->OtherVersionsCheckBox->setChecked(true);
  ui->ReleaseModeCheckBox->setChecked(true);
  ui->DebugModeCheckBox->setChecked(true);

  connect(ui->CurrentVersionCheckBox,SIGNAL(toggled(bool)),this,SLOT(handlePurgeChanged()));
  connect(ui->OtherVersionsCheckBox,SIGNAL(toggled(bool)),this,SLOT(handlePurgeChanged()));
  connect(ui->ReleaseModeCheckBox,SIGNAL(toggled(bool)),this,SLOT(handlePurgeChanged()));
  connect(ui->DebugModeCheckBox,SIGNAL(toggled(bool)),this,SLOT(handlePurgeChanged()));

  connect(ui->PurgeRunButton,SIGNAL(clicked()),this,SLOT(runPurge()));


  setCursor(Qt::WaitCursor);

  updateBatchActionsGroup(0);

  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));


  QThread* WThread = new QThread;
  WorkspaceDevDashboardWorker* Worker = new WorkspaceDevDashboardWorker();
  Worker->moveToThread(WThread);

  connect(WThread, SIGNAL(started()), Worker, SLOT(run()));
  connect(Worker, SIGNAL(finished()), WThread, SLOT(quit()));
  connect(Worker, SIGNAL(finished()), Worker, SLOT(deleteLater()));
  connect(WThread, SIGNAL(finished()), WThread, SLOT(deleteLater()));
  connect(Worker, SIGNAL(finished()), this, SLOT(activateDialog()));

  qRegisterMetaType<openfluid::ui::waresdev::WorkspaceDevDashboardTypes::WareGitInfos>(
      "openfluid::ui::waresdev::WorkspaceDevDashboardTypes::WareGitInfos");
  connect(Worker, SIGNAL(wareParsed(openfluid::ui::waresdev::WorkspaceDevDashboardTypes::WareGitInfos)),
          this, SLOT(addParsedWare(openfluid::ui::waresdev::WorkspaceDevDashboardTypes::WareGitInfos)));

  WThread->start();

}


// =====================================================================
// =====================================================================


WorkspaceDevDashboardDialog::~WorkspaceDevDashboardDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


WorkspaceDevDashboardTypes::WaresSelectionByType WorkspaceDevDashboardDialog::getSelectionByType() const
{
  return {
           {openfluid::ware::WareType::SIMULATOR,ui->SimulatorsWidget->getSelection()},
           {openfluid::ware::WareType::OBSERVER,ui->ObserversWidget->getSelection()},
           {openfluid::ware::WareType::BUILDEREXT,ui->BuilderextsWidget->getSelection()}
         };
}


// =====================================================================
// =====================================================================


void WorkspaceDevDashboardDialog::updateBatchActionsGroup(unsigned int SelectionCount)
{
  ui->ActionsGroupBox->setTitle(tr("Batch actions on %1 selected wares").arg(SelectionCount));
  ui->ActionsGroupBox->setEnabled(SelectionCount);
}


// =====================================================================
// =====================================================================


void WorkspaceDevDashboardDialog::addParsedWare(openfluid::ui::waresdev::WorkspaceDevDashboardTypes::WareGitInfos Infos)
{
  if (Infos.Type == openfluid::ware::WareType::SIMULATOR)
  {
    ui->WaresTabWidget->setCurrentIndex(0);
    ui->SimulatorsWidget->addWare(Infos);
  }
  else if (Infos.Type == openfluid::ware::WareType::OBSERVER)
  {
    ui->WaresTabWidget->setCurrentIndex(1);
    ui->ObserversWidget->addWare(Infos);
  }
  else if (Infos.Type == openfluid::ware::WareType::BUILDEREXT)
  {
    ui->WaresTabWidget->setCurrentIndex(2);
    ui->BuilderextsWidget->addWare(Infos);
  }
}


// =====================================================================
// =====================================================================


void WorkspaceDevDashboardDialog::setWareActionStatus(openfluid::ware::WareType Type,
                                                      QString ID, QString ActionName,
                                                      bool Status)
{
  if (Type == openfluid::ware::WareType::SIMULATOR)
  {
    ui->SimulatorsWidget->setState(ID,ActionName,Status);
  }
  else if (Type == openfluid::ware::WareType::OBSERVER)
  {
    ui->ObserversWidget->setState(ID,ActionName,Status);
  }
  else if (Type == openfluid::ware::WareType::BUILDEREXT)
  {
    ui->BuilderextsWidget->setState(ID,ActionName,Status);
  }
}


// =====================================================================
// =====================================================================


void WorkspaceDevDashboardDialog::activateDialog()
{
  ui->WaresTabWidget->setCurrentIndex(0);

  ui->SimulatorsWidget->update();
  ui->ObserversWidget->update();
  ui->BuilderextsWidget->update();

  connect(ui->SimulatorsWidget,SIGNAL(selectionChanged()),this,SLOT(handleSelectionChanged()));
  connect(ui->ObserversWidget,SIGNAL(selectionChanged()),this,SLOT(handleSelectionChanged()));
  connect(ui->BuilderextsWidget,SIGNAL(selectionChanged()),this,SLOT(handleSelectionChanged()));

  unsetCursor();
}


// =====================================================================
// =====================================================================


void WorkspaceDevDashboardDialog::handleSelectionChanged()
{
  auto SelectCount = ui->SimulatorsWidget->getSelection().size() +
                     ui->ObserversWidget->getSelection().size() +
                     ui->BuilderextsWidget->getSelection().size();

  updateBatchActionsGroup(SelectCount);
}


// =====================================================================
// =====================================================================


void WorkspaceDevDashboardDialog::handleBuildChanged()
{
  ui->BuildDocRunButton->setEnabled(ui->BuildCheckBox->isChecked() ||
                                    ui->DocCheckBox->isChecked());
}


// =====================================================================
// =====================================================================


void WorkspaceDevDashboardDialog::handleCheckChanged()
{
  ui->CheckRunButton->setEnabled(ui->WareshubCheckBox->isChecked() ||
                                 ui->ReadmeCheckBox->isChecked() ||
                                 ui->LicenseCheckBox->isChecked());
}


// =====================================================================
// =====================================================================


void WorkspaceDevDashboardDialog::handlePurgeChanged()
{
  ui->PurgeRunButton->setEnabled((ui->CurrentVersionCheckBox->isChecked() ||
                                  ui->OtherVersionsCheckBox->isChecked()) &&
                                 (ui->ReleaseModeCheckBox->isChecked() ||
                                  ui->DebugModeCheckBox->isChecked()));
}


// =====================================================================
// =====================================================================


void WorkspaceDevDashboardDialog::runBuildDoc()
{
  openfluid::waresdev::WareSrcContainer::ConfigMode ConfigM = ui->BuildOptionsWidget->getConfigureMode();
  openfluid::waresdev::WareSrcContainer::BuildMode BuildM = ui->BuildOptionsWidget->getBuildMode();
  unsigned int BuildJ = ui->BuildOptionsWidget->getBuildJobs();


  WorkspaceDevBuildWorker* Worker = new WorkspaceDevBuildWorker(getSelectionByType(),
                                                                ui->BuildCheckBox->isChecked(),
                                                                ui->DocCheckBox->isChecked(),
                                                                ConfigM,BuildM,BuildJ);

  WorkspaceDevProcessDialog Dialog(tr("Build"),Worker,this);

  Dialog.exec();
}


// =====================================================================
// =====================================================================


void WorkspaceDevDashboardDialog::runCheck()
{
  WorkspaceDevCheckWorker* Worker = new WorkspaceDevCheckWorker(getSelectionByType(),
                                                                ui->WareshubCheckBox->isChecked(),
                                                                ui->ReadmeCheckBox->isChecked(),
                                                                ui->LicenseCheckBox->isChecked());

  WorkspaceDevProcessDialog Dialog(tr("Check"),Worker,this);

  Dialog.exec();
}


// =====================================================================
// =====================================================================


void WorkspaceDevDashboardDialog::runPurge()
{
  WorkspaceDevPurgeWorker* Worker = new WorkspaceDevPurgeWorker(getSelectionByType(),
                                                                ui->CurrentVersionCheckBox->isChecked(),
                                                                ui->OtherVersionsCheckBox->isChecked(),
                                                                ui->ReleaseModeCheckBox->isChecked(),
                                                                ui->DebugModeCheckBox->isChecked());

  WorkspaceDevProcessDialog Dialog(tr("Purge"),Worker,this);

  Dialog.exec();

}


} } } // namespaces
