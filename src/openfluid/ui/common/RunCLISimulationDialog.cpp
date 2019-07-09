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
  @file RunCLISimulationDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QDir>
#include <QProcess>
#include <QRegularExpression>

#include <openfluid/ui/common/RunCLISimulationDialog.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/config.hpp>

#include "ui_RunCLISimulationDialog.h"


namespace openfluid { namespace ui { namespace common {


RunCLISimulationDialog::RunCLISimulationDialog(QWidget* Parent, const QString& PrjPath):
  MessageDialog(Parent), ui(new Ui::RunCLISimulationDialog), m_PrjPath(PrjPath), 
  mp_Process(new QProcess()), mp_ANSIRegex(new QRegularExpression("\\x1B\\[([0-9]{1,2}(;[0-9]{1,2})?)?[m|K]")),
  m_IsKilled(false)
{
  setWindowModality(Qt::ApplicationModal);

  ui->setupUi(this);

  setupMessageUi(tr("Run Simulation"),QDialogButtonBox::NoButton);
  ui->ButtonBox->button(QDialogButtonBox::Close)->setEnabled(false);
  ui->KillButton->setIcon(openfluid::ui::common::getIcon("stop","/ui/common"));
  ui->KillButton->setIconSize(QSize(20,20));
  ui->KillButton->setEnabled(false);
  ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::NONE);

  ui->ProjectPathLabel->setText(QDir::toNativeSeparators(PrjPath));

  ui->CommandLineEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

  ui->ExecutionOutputEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
  ui->ExecutionOutputEdit->setStyleSheet("QPlainTextEdit { background-color : #333; color : #eee; }");
  ui->ExecutionOutputEdit->setFocus();

  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
}


// =====================================================================
// =====================================================================


RunCLISimulationDialog::~RunCLISimulationDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void RunCLISimulationDialog::showEvent(QShowEvent* event) 
{
  QWidget::showEvent( event );

  // build path to OpenFLUID command line
  std::string CmdPath = openfluid::tools::Filesystem::joinPath({openfluid::base::Environment::getInstallPrefix(),
                                                                "bin",
                                                                openfluid::config::CMD_APP
                                                               });

  QString Command = QString::fromStdString(CmdPath);
 
  // build command line arguments 
  QStringList Args;
  Args << "run" << m_PrjPath;


  auto RunCtxt = openfluid::base::RunContextManager::instance();
  char PathsSep = openfluid::tools::Filesystem::pathsListSeparator();

  auto ExtraSimPaths = openfluid::base::Environment::getExtraSimulatorsDirs();
  if (!ExtraSimPaths.empty())
  {
    Args << QString("--simulators-paths=%1").arg(openfluid::tools::toQStringList(ExtraSimPaths).join(PathsSep));
  }

  auto ExtraObsPaths = openfluid::base::Environment::getExtraObserversDirs();
  if (!ExtraObsPaths.empty())
  {
    Args << QString("--observers-paths=%1").arg(openfluid::tools::toQStringList(ExtraObsPaths).join(PathsSep));
  }


  if (RunCtxt->isProfiling())
  {
    Args << "--profiling";
  }

  if (RunCtxt->isClearOutputDir())
  {
    Args << "--clean-output-dir";
  }

  auto NumThreads = RunCtxt->getWaresMaxNumThreads();
  Args << QString("--max-threads=%1").arg(NumThreads);
  
  // display complete command line
  ui->CommandLineEdit->setText(QString("%1 %2").arg(Command).arg(Args.join(" ")));
  ui->CommandLineEdit->setCursorPosition(0);

  // reset display of execution output
  ui->ExecutionOutputEdit->clear();
  ui->ExecutionOutputEdit->moveCursor(QTextCursor::Start);

  connect(mp_Process.get(), SIGNAL(readyRead()), this, SLOT(displayOutput()));
  connect(mp_Process.get(), SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finishProcess()));
  connect(ui->KillButton,SIGNAL(clicked()),this,SLOT(killProcess()));

  // launch command line
  mp_Process->setProcessChannelMode(QProcess::MergedChannels);
  mp_Process->start(Command,Args);
  ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::RUNNING);
  ui->KillButton->setEnabled(true);
} 


// =====================================================================
// =====================================================================


void RunCLISimulationDialog::finishProcess()
{
  ui->KillButton->setEnabled(false);
  ui->ExecutionOutputEdit->moveCursor (QTextCursor::End);
  ui->ButtonBox->button(QDialogButtonBox::Close)->setEnabled(true);

  if (m_IsKilled)
  {
    ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::USERKILLED);
  }
  else if (mp_Process->exitCode())
  {
    ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::FAILED);
  }
  else
  {
    ui->StatusLabel->setStatus(ExecutionStatusLabel::Status::SUCCEEDED);
  }
}


// =====================================================================
// =====================================================================


void RunCLISimulationDialog::killProcess()
{
  ui->KillButton->setEnabled(false);
  mp_Process->kill();
  m_IsKilled = true;
}


// =====================================================================
// =====================================================================


void RunCLISimulationDialog::displayOutput()
{
  QString Text(mp_Process->readAll());

  // Remove the ANSI color codes
  Text = Text.remove(*mp_ANSIRegex);

  ui->ExecutionOutputEdit->moveCursor(QTextCursor::End);
  ui->ExecutionOutputEdit->insertPlainText(Text);
}


// =====================================================================
// =====================================================================


bool RunCLISimulationDialog::execute()
{
  exec();
  return (mp_Process->exitCode() == 0);
}


} } } // namespaces

