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
  \file RunSimulationDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <QPushButton>
#include <QShowEvent>
#include <QMessageBox>
#include <QThread>




#include "ui_RunSimulationDialog.h"


#include <openfluid/ui/common/RunSimulationDialog.hpp>
#include <openfluid/ui/common/RunSimulationWorker.hpp>


#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/Factory.hpp>
#include <openfluid/machine/Generator.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>

#include <iostream>

namespace openfluid { namespace ui { namespace common {




RunSimulationDialog::RunSimulationDialog(QWidget *Parent, openfluid::fluidx::FluidXDescriptor* FXDesc):
  QDialog(Parent,Qt::CustomizeWindowHint|Qt::WindowTitleHint), ui(new Ui::RunSimulationDialog), mp_FXDesc(FXDesc),
  m_Launched(false), m_Success(false)
{
  setWindowModality(Qt::ApplicationModal);

  ui->setupUi(this);

  setFixedSize(geometry().width(),geometry().height());

  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  ui->MessageLabel->setText(tr("Run Simulation"));

  ui->DurationLabel->setText("");

  ui->SimulationBar->setValue(0);
  ui->SimulationBar->setMinimum(0);
  ui->SimulationBar->setMaximum(100);

  ui->PresimLabel->setText(tr("Pre-simulation"));
  ui->InitLabel->setText(tr("Initialization"));
  ui->RunLabel->setText(tr("Run"));
  ui->FinalLabel->setText(tr("Finalization"));

  ui->ButtonBox->button(QDialogButtonBox::Close)->setEnabled(false);

  setStage(openfluid::ui::common::RunSimulationListener::RUNW_BEFORE);

  mp_Listener = new openfluid::ui::common::RunSimulationListener();
  QThread* WThread = new QThread;
  openfluid::ui::common::RunSimulationWorker* Worker = new openfluid::ui::common::RunSimulationWorker(mp_FXDesc,mp_Listener);
  Worker->moveToThread(WThread);

  connect(Worker, SIGNAL(error(QString)), this, SLOT(handleError(QString)));
  connect(Worker, SIGNAL(finished()), this, SLOT(handleFinish()));
  connect(WThread, SIGNAL(started()), Worker, SLOT(run()));
  connect(Worker, SIGNAL(finished()), WThread, SLOT(quit()));
  connect(Worker, SIGNAL(finished()), Worker, SLOT(deleteLater()));
  connect(WThread, SIGNAL(finished()), WThread, SLOT(deleteLater()));
  connect(WThread, SIGNAL(finished()), mp_Listener, SLOT(deleteLater()));

  qRegisterMetaType<openfluid::ui::common::RunSimulationListener::Stage>("openfluid::ui::common::RunSimulationListener::Stage");
  qRegisterMetaType<openfluid::core::TimeIndex_t>("openfluid::core::TimeIndex_t");

  connect(Worker, SIGNAL(periodChanged(QString,QString,int)), this, SLOT(setPeriod(QString,QString,int)));
  connect(mp_Listener,SIGNAL(stageChanged(openfluid::ui::common::RunSimulationListener::Stage)),
          this,SLOT(setStage(openfluid::ui::common::RunSimulationListener::Stage)));
  connect(mp_Listener,SIGNAL(progressValueChanged(int)),this,
          SLOT(setProgressValue(int)));
  connect(mp_Listener,SIGNAL(progressMaxChanged(int)),this,
          SLOT(setProgressMax(int)));

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  WThread->start();
}


// =====================================================================
// =====================================================================


RunSimulationDialog::~RunSimulationDialog()
{
  delete ui;
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
         QString::number(Minutes)+tr(" minute(s), ")+QString::number(Seconds)+tr(" second(s)");
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


void RunSimulationDialog::setPeriod(QString /*Begin*/, QString /*End*/, int Duration)
{
  // TODO check if better to display period in message box
//  ui->MessageLabel->setText(ui->MessageLabel->text()+tr(" from ")+Begin+tr(" to ")+End);
  ui->DurationLabel->setText(tr("Simulated duration is ")+getDurationAsDaysHoursMinsSecsString(Duration));
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::setStage(openfluid::ui::common::RunSimulationListener::Stage S)
{
  if (S == openfluid::ui::common::RunSimulationListener::RUNW_BEFORE)
  {
    ui->PresimLabel->setStyleSheet("");
    ui->InitLabel->setStyleSheet("");
    ui->RunLabel->setStyleSheet("");
    ui->FinalLabel->setStyleSheet("");
  }
  else if (S == openfluid::ui::common::RunSimulationListener::RUNW_PRESIM)
  {
    ui->PresimLabel->setStyleSheet("font : bold; text-decoration : underline;");
    ui->InitLabel->setStyleSheet("");
    ui->RunLabel->setStyleSheet("");
    ui->FinalLabel->setStyleSheet("");
  }
  else if (S == openfluid::ui::common::RunSimulationListener::RUNW_INIT)
  {
    ui->PresimLabel->setStyleSheet("font : bold;");
    ui->InitLabel->setStyleSheet("font : bold; text-decoration : underline;");
    ui->RunLabel->setStyleSheet("");
    ui->FinalLabel->setStyleSheet("");
  }
  else if (S == openfluid::ui::common::RunSimulationListener::RUNW_RUN)
  {
    ui->PresimLabel->setStyleSheet("font : bold;");
    ui->InitLabel->setStyleSheet("font : bold;");
    ui->RunLabel->setStyleSheet("font : bold; text-decoration : underline;");
    ui->FinalLabel->setStyleSheet("");
  }
  else if (S == openfluid::ui::common::RunSimulationListener::RUNW_FINAL)
  {
    ui->PresimLabel->setStyleSheet("font : bold;");
    ui->InitLabel->setStyleSheet("font : bold;");
    ui->RunLabel->setStyleSheet("font : bold;");
    ui->FinalLabel->setStyleSheet("font : bold; text-decoration : underline;");
  }
  else if (S == openfluid::ui::common::RunSimulationListener::RUNW_AFTER)
  {
    ui->PresimLabel->setStyleSheet("font : bold;");
    ui->InitLabel->setStyleSheet("font : bold;");
    ui->RunLabel->setStyleSheet("font : bold;");
    ui->FinalLabel->setStyleSheet("font : bold;");
  }
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::handleError(QString Msg)
{
  QApplication::restoreOverrideCursor();
  QMessageBox::critical(NULL,tr("Simulation error"),Msg);
  reject();
}


// =====================================================================
// =====================================================================


void RunSimulationDialog::handleFinish()
{
  QApplication::restoreOverrideCursor();
  ui->ButtonBox->button(QDialogButtonBox::Close)->setEnabled(true);
  m_Success = true;
}


} } } // namespaces


