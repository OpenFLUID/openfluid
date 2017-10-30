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
  @file WorkspaceDevProcessDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QThread>
#include <QClipboard>
#include <QFileDialog>
#include <QTextStream>

#include <openfluid/ui/waresdev/WorkspaceDevProcessDialog.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevDashboardDialog.hpp>

#include "ui_WorkspaceDevProcessDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


WorkspaceDevProcessDialog::WorkspaceDevProcessDialog(const QString& Title, WorkspaceDevProcessWorker* Worker,
                                                     WorkspaceDevDashboardDialog* Parent) :
    QDialog(Parent), ui(new Ui::WorkspaceDevProcessDialog)
{
  ui->setupUi(this);

  ui->ProgressBar->setValue(0);
  ui->ProgressBar->setMaximum(Worker->getWaresCount());

  setWindowTitle(Title);

  ui->CopyClipboardButton->setEnabled(false);
  ui->SaveButton->setEnabled(false);
  ui->ButtonBox->button(QDialogButtonBox::Close)->setEnabled(false);

  connect(ui->CopyClipboardButton,SIGNAL(clicked()),this,SLOT(copyToClipboard()));
  connect(ui->SaveButton,SIGNAL(clicked()),this,SLOT(saveToFile()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  QThread* WThread = new QThread;
  Worker->moveToThread(WThread);

  connect(WThread, SIGNAL(started()), Worker, SLOT(run()));
  connect(Worker, SIGNAL(finished()), WThread, SLOT(quit()));
  connect(Worker, SIGNAL(finished()), this, SLOT(handleWorkerFinished()));
  qRegisterMetaType<openfluid::ware::WareType>("openfluid::ware::WareType");
  connect(Worker, SIGNAL(processed(openfluid::ware::WareType,QString,QString, bool)),
          Parent, SLOT(setWareActionStatus(openfluid::ware::WareType,QString,QString, bool)));
  connect(Worker, SIGNAL(wareCompleted()),
          this, SLOT(handleWorkerWareCompleted()));
  connect(Worker, SIGNAL(messageWritten(QString)),this, SLOT(handleWorkerMessage(QString)));
  connect(Worker, SIGNAL(finished()), Worker, SLOT(deleteLater()));
  connect(WThread, SIGNAL(finished()), WThread, SLOT(deleteLater()));

  WThread->start();
}


// =====================================================================
// =====================================================================


WorkspaceDevProcessDialog::~WorkspaceDevProcessDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WorkspaceDevProcessDialog::handleWorkerFinished()
{
  ui->CopyClipboardButton->setEnabled(true);
  ui->SaveButton->setEnabled(true);
  ui->ButtonBox->button(QDialogButtonBox::Close)->setEnabled(true);
}


// =====================================================================
// =====================================================================


void WorkspaceDevProcessDialog::handleWorkerWareCompleted()
{
  ui->ProgressBar->setValue(ui->ProgressBar->value()+1);
}


// =====================================================================
// =====================================================================


void WorkspaceDevProcessDialog::handleWorkerMessage(QString Msg)
{
  ui->MessagesTextEdit->appendHtml(Msg);
}


// =====================================================================
// =====================================================================


void WorkspaceDevProcessDialog::copyToClipboard()
{
  QApplication::clipboard()->setText(ui->MessagesTextEdit->toPlainText());
}


// =====================================================================
// =====================================================================


void WorkspaceDevProcessDialog::saveToFile()
{
  QString FilePath = QFileDialog::getSaveFileName(this, tr("Export wares sources as package"),
                                                  QDir::home().absoluteFilePath("config-build-log.txt"));
  if (!FilePath.isEmpty())
  {
    QFile LogFile(FilePath);

    if (LogFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream LogStream(&LogFile);
      LogStream << ui->MessagesTextEdit->toPlainText() << "\n";
    }
  }
}


} } } // namespaces
