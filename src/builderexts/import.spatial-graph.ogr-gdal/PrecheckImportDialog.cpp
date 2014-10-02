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
  \file PrecheckImportDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_PrecheckImportDialog.h"
#include "PrecheckImportDialog.hpp"

#include <QCloseEvent>


PrecheckImportDialog::PrecheckImportDialog(int StepsCount, QWidget* Parent = NULL):
  QDialog(Parent),ui(new Ui::PrecheckImportDialog),
  m_IsFinished(false), m_IsCloseRequired(false)
{
  ui->setupUi(this);

  setWindowTitle("Spatial data import (OGR/GDAL)");

  ui->PerformLabel->setText(tr("Performing precheck"));

  ui->ProgressBar->setValue(0);
  ui->ProgressBar->setMaximum(StepsCount);

  ui->ButtonBox->setEnabled(false);

  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(close()));
}


// =====================================================================
// =====================================================================


PrecheckImportDialog::~PrecheckImportDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void PrecheckImportDialog::closeEvent(QCloseEvent* Event)
{
  if (!m_IsFinished) Event->ignore();
}


// =====================================================================
// =====================================================================


void PrecheckImportDialog::handleStepEntered(QString Message)
{
  ui->MessagesTextEdit->insertHtml(Message+" ");
  ui->MessagesTextEdit->moveCursor(QTextCursor::End);
}


// =====================================================================
// =====================================================================


void PrecheckImportDialog::handleStepCompleted(int StepNbr, QString Message)
{
  ui->ProgressBar->setValue(StepNbr);
  ui->MessagesTextEdit->insertHtml(Message+"<br/><br/>");
  ui->MessagesTextEdit->moveCursor(QTextCursor::End);
}


// =====================================================================
// =====================================================================


void PrecheckImportDialog::handleCompleted(QString Message, bool /*IsCloseRequired*/)
{
  ui->ProgressBar->setValue(ui->ProgressBar->maximum());
  ui->MessagesTextEdit->insertHtml("<br/>"+Message+"<br/>");
  ui->MessagesTextEdit->moveCursor(QTextCursor::End);
}


// =====================================================================
// =====================================================================


void PrecheckImportDialog::handleCloseRequired()
{
  m_IsCloseRequired = true;
}


// =====================================================================
// =====================================================================


void PrecheckImportDialog::handleFinished()
{
  m_IsFinished = true;
  ui->ButtonBox->setEnabled(true);
}


// =====================================================================
// =====================================================================


void PrecheckImportDialog::close()
{
  reject();

  if (m_IsCloseRequired)
    emit closeRequired();
}
