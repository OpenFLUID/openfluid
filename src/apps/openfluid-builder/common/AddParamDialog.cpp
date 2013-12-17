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
  \file AddParamDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include "ui_AddParamDialog.h"
#include "AddParamDialog.hpp"
#include "builderconfig.hpp"

#include <QPushButton>


AddParamDialog::AddParamDialog(const QStringList& ExistingParams,const QStringList& CompletingParams,
                               QWidget* Parent):
  OpenFLUIDDialog(Parent),ui(new(Ui::AddParamDialog)),
  mp_Completer(NULL),
  m_ExistingParams(ExistingParams), m_CompletingParams(CompletingParams)
{
  ui->setupUi(this);

  ui->MessageLabel->setText(tr("Add parameter"));

  if (!CompletingParams.isEmpty())
  {
    mp_Completer = new QCompleter(CompletingParams, this);
    mp_Completer->setCaseSensitivity(Qt::CaseInsensitive);
    mp_Completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->NameEdit->setCompleter(mp_Completer);

    connect(mp_Completer,SIGNAL(activated(const QString&)),this,SLOT(checkGlobal()));
  }

  connect(ui->NameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  checkGlobal();
}


// =====================================================================
// =====================================================================


AddParamDialog::~AddParamDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void AddParamDialog::checkGlobal()
{
  if (ui->NameEdit->text().isEmpty())
    setMessage(tr("Parameter name cannot be empty"));
  else if (m_ExistingParams.contains(ui->NameEdit->text()))
    setMessage(tr("Parameter already exists"));
  else
    setMessage();
}


// =====================================================================
// =====================================================================


void AddParamDialog::setMessage(const QString& Msg)
{
  if (Msg.isEmpty())
  {
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(BUILDER_DIALOGBANNER_BGCOLOR));
    ui->MessageLabel->setText(tr("Add parameter"));
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
  else
  {
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(BUILDER_DIALOGBANNER_WARNBGCOLOR));
    ui->MessageLabel->setText(Msg);
  }
}


// =====================================================================
// =====================================================================


QString AddParamDialog::getParamName() const
{
  return ui->NameEdit->text();
}


// =====================================================================
// =====================================================================


QString AddParamDialog::getParamValue() const
{
  return ui->ValueEdit->text();
}
