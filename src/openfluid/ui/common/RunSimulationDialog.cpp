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
  @file RunSimulationDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QPushButton>
#include <QShowEvent>
#include <QMessageBox>
#include <QThread>

#include <openfluid/ui/common/RunSimulationDialog.hpp>
#include <openfluid/ui/common/RunSimulationWorker.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/Factory.hpp>
#include <openfluid/machine/Generator.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/tools/MiscHelpers.hpp>

#include "ui_RunSimulationDialog.h"


// =====================================================================
// =====================================================================


constexpr const char* WARNINGS_STYLE_ANY = "QLabel {font-weight: bold;}";


namespace openfluid { namespace ui { namespace common {


RunSimulationDialog::RunSimulationDialog(QWidget *Parent, const openfluid::fluidx::FluidXDescriptor* FXDesc):
  MessageDialog(Parent), ui(new Ui::RunSimulationDialog), mp_FXDesc(FXDesc),
  m_Launched(false), m_Success(true), m_CanClose(true)
{
  setWindowModality(Qt::ApplicationModal);
  setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

  ui->setupUi(this);

  setupMessageUi(tr("Run Simulation"),QDialogButtonBox::NoButton);

  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  ui->DurationLabel->setText("");
  ui->ElapsedLabel->setText("-");

  ui->SimulationBar->setValue(0);
  ui->SimulationBar->setMinimum(0);
  ui->SimulationBar->setMaximum(100);

  ui->PauseButton->setIcon(openfluid::ui::common::getIcon("pause","/ui/common"));
  ui->PauseButton->setIconSize(QSize(16,16));

  ui->StopButton->setIcon(openfluid::ui::common::getIcon("stop","/ui/common"));
  ui->StopButton->setIconSize(QSize(16,16));
  ui->StopButton->setEnabled(false);

  ui->StatusLabel->setText("");
  ui->ShowErrorLabel->setVisible(false);
  ui->ErrorDetailsWidget->setVisible(false);


  ui->WarningsLabel->setText("0");

  ui->ButtonBox->button(QDialogButtonBox::Close)->setEnabled(false);

  setStage(openfluid::ui::common::RunSimulationListener::RUNW_BEFORE);

  mp_Listener = new openfluid::ui::common::RunSimulationListener();
  QThread* WThread = new QThread;
  openfluid::ui::common::RunSimulationWorker* Worker =
      new openfluid::ui::common::RunSimulationWorker(mp_FXDesc,mp_Listener);
  Worker->moveToThread(WThread);

  qRegisterMetaType<openfluid::base::ExceptionContext>("openfluid::base::ExceptionContext");

  connect(Worker, SIGNAL(error(QString,openfluid::base::ExceptionContext)),
          this, SLOT(handleError(QString,openfluid::base::ExceptionContext)));
  connect(Worker, SIGNAL(userAbort()), this, SLOT(handleUserAbort()));
  connect(Worker, SIGNAL(finished()), this, SLOT(handleFinish()));
  connect(WThread, SIGNAL(started()), Worker, SLOT(run()));
  connect(Worker, SIGNAL(finished()), WThread, SLOT(quit()));
  connect(Worker, SIGNAL(finished()), Worker, SLOT(deleteLater()));
  connect(WThread, SIGNAL(finished()), WThread, SLOT(deleteLater()));
  connect(WThread, SIGNAL(finished()), mp_Listener, SLOT(deleteLater()));

  connect(ui->ShowErrorLabel,SIGNAL(clicked()),this,SLOT(showErrorDetails()));

  qRegisterMetaType<openfluid::ui::common::RunSimulationListener::Stage>(
      "openfluid::ui::common::RunSimulationListener::Stage");
  qRegisterMetaType<openfluid::core::TimeIndex_t>("openfluid::core::TimeIndex_t");

  connect(Worker, SIGNAL(periodChanged(QString,QString,int)), this, SLOT(setPeriod(QString,QString,int)));
  connect(Worker, SIGNAL(warningsChanged(unsigned int)), this, SLOT(setWarningsCount(unsigned int)));

  connect(mp_Listener,SIGNAL(stageChanged(openfluid::ui::common::RunSimulationListener::Stage)),
          this,SLOT(setStage(openfluid::ui::common::RunSimulationListener::Stage)));
  connect(mp_Listener,SIGNAL(progressValueChanged(int)),this,
          SLOT(setProgressValue(int)));
  connect(mp_Listener,SIGNAL(progressMaxChanged(int)),this,
          SLOT(setProgressMax(int)));

  connect(ui->StopButton, SIGNAL(clicked()), this, SLOT(requestAbort()));
  connect(ui->PauseButton, SIGNAL(clicked()), this, SLOT(requestSuspendResume()));
  connect(mp_Listener, SIGNAL(pauseConfirmed()), this, SLOT(validateSuspend()));

  adjustSize();

  // thread launch: forbid window closing
  m_CanClose = false;
  WThread->start();
  m_ElapsedTimer.start();
}


// =====================================================================
// =====================================================================


RunSimulationDialog::~RunSimulationDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


bool RunSimulationDialog::event(QEvent* event)
{
  QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
  if(keyEvent && keyEvent->key() == Qt::Key_Escape && !m_CanClose) // disable escape key when closing is forbidden
  {
    keyEvent->accept();
    return true;
  }
  return MessageDialog::event(event);
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::closeEvent(QCloseEvent *event)
{
  event->ignore();
}


// =====================================================================
// =====================================================================


QString RunSimulationDialog::getDurationAsDaysHoursMinsSecsString(openfluid::core::Duration_t Duration)
{
  QString res;
  int Seconds = (int) (Duration % 60);
  Duration /= 60;
  int Minutes = (int) (Duration % 60);
  Duration /= 60;
  int Hours = (int) (Duration % 24);
  int Days = (int) (Duration / 24);
  return QString::number(Days)+tr(" day(s), ")+QString::number(Hours)+tr(" hour(s), ")+
         QString::number(Minutes)+tr(" min(s), ")+QString::number(Seconds)+tr(" sec(s)");
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::requestAbort()
{
  if (QMessageBox::question(QApplication::activeWindow(),
                                  "OpenFLUID",
                                  tr("You are requesting the simulation to abort.\n"
                                     "Simulation data not written on disk will be lost.\n\nProceed anyway?"),
                                  QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
  {
    ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::REQUESTING_ABORT);
    mp_Listener->requestAbort();
  }
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::requestSuspendResume()
{
  if (!mp_Listener->isPausedByUser())
  {
    ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::REQUESTING_PAUSE);
    ui->PauseButton->setEnabled(false);
  }
  else
  {
    ui->StopButton->setEnabled(false);
    ui->PauseButton->setIcon(openfluid::ui::common::getIcon("pause","/ui/common"));
    ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::RUNNING);
  }

  mp_Listener->requestSuspendResume();
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::validateSuspend()
{
  ui->PauseButton->setIcon(openfluid::ui::common::getIcon("start","/ui/common"));
  ui->PauseButton->setEnabled(true);
  ui->StopButton->setEnabled(true);
  ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::PAUSED);
}


// =====================================================================
// =====================================================================


bool RunSimulationDialog::execute()
{
  exec();
  return m_Success;
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::setProgressValue(int Index)
{
  ui->SimulationBar->setValue(Index);
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::setProgressMax(int Duration)
{
  ui->SimulationBar->setMaximum(Duration);
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::setWarningsCount(unsigned int Count)
{
  if (Count)
  {
    ui->WarningsLabel->setStyleSheet(WARNINGS_STYLE_ANY);
  }
  ui->WarningsLabel->setText(QString("%1").arg(Count));
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::setPeriod(QString /*Begin*/, QString /*End*/, int Duration)
{
  ui->DurationLabel->setText(getDurationAsDaysHoursMinsSecsString(Duration));
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::setStage(openfluid::ui::common::RunSimulationListener::Stage S)
{
  if (S == openfluid::ui::common::RunSimulationListener::RUNW_BEFORE)
  {
    ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::NOTSTARTED);
    ui->StageLabel->setText("-");
  }
  else if (S == openfluid::ui::common::RunSimulationListener::RUNW_PRESIM)
  {
    ui->StageLabel->setText(tr("pre-simulation"));
    ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::RUNNING);
  }
  else if (S == openfluid::ui::common::RunSimulationListener::RUNW_INIT)
  {
    ui->StageLabel->setText(tr("initialization"));
    ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::RUNNING);
  }
  else if (S == openfluid::ui::common::RunSimulationListener::RUNW_RUN)
  {
    ui->StageLabel->setText(tr("simulation"));
    ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::RUNNING);
  }
  else if (S == openfluid::ui::common::RunSimulationListener::RUNW_FINAL)
  {
    ui->StageLabel->setText(tr("finalization"));
    ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::RUNNING);
  }
  else if (S == openfluid::ui::common::RunSimulationListener::RUNW_AFTER)
  {
    ui->StageLabel->setText("-");
    ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::SUCCEEDED);
  }
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::setCompleted()
{
  m_CanClose = true;  
  
  ui->PauseButton->setEnabled(false);
  ui->StopButton->setEnabled(false);

  ui->ElapsedLabel
    ->setText(QString::fromStdString(openfluid::tools::getDurationAsPrettyString(m_ElapsedTimer.elapsed())));
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::handleError(QString Msg,openfluid::base::ExceptionContext Context)
{
  setCompleted();

  ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::FAILED);
  ui->StatusLabel->setToolTip(Msg);

  ui->ErrorMessageEdit->setText(Msg);

  QString ContextStr;
  for (auto& it : Context)
  {
    ContextStr += QString::fromStdString("<u>" + it.first + " :</u> " + it.second + "<br/>");
  }

  ui->ErrorContextEdit->setText(ContextStr);
  ui->ShowErrorLabel->setVisible(true);
  
  m_Success = false;
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::handleUserAbort()
{
  setCompleted();

  ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::ABORTED);

  m_Success = false;
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::handleFinish()
{
  setCompleted();

  ui->ButtonBox->button(QDialogButtonBox::Close)->setEnabled(true);
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::showErrorDetails()
{
  ui->ErrorDetailsWidget->setVisible(true);
  ui->ShowErrorLabel->setVisible(false);
}


} } } // namespaces

