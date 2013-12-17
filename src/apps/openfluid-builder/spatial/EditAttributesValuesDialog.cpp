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
  \file EditAttributesValuesDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_EditAttributesValuesDialog.h"
#include "EditAttributesValuesDialog.hpp"

#include "builderconfig.hpp"

#include <QPushButton>
#include <QDoubleValidator>


EditAttributesValuesDialog::EditAttributesValuesDialog(QWidget* Parent):
  OpenFLUIDDialog(Parent), ui(new Ui::EditAttributesValuesDialog)
{

  ui->setupUi(this);

  ui->ReplaceRadioButton->setChecked(true);
  ui->ReplaceEdit->setFocus();

  ui->MultEdit->setValidator(new QDoubleValidator(ui->MultEdit));
  ui->AddEdit->setValidator(new QDoubleValidator(ui->AddEdit));

  connect(ui->ReplaceRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->ReplaceEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->MultRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->MultEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->AddRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->AddEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));

  connect(ui->ReplaceEdit,SIGNAL(textEdited(const QString&)),this,SLOT(adaptSelection()));
  connect(ui->MultEdit,SIGNAL(textEdited(const QString&)),this,SLOT(adaptSelection()));
  connect(ui->AddEdit,SIGNAL(textEdited(const QString&)),this,SLOT(adaptSelection()));

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  checkGlobal();
}


// =====================================================================
// =====================================================================


EditAttributesValuesDialog::~EditAttributesValuesDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void EditAttributesValuesDialog::checkGlobal()
{
  if (ui->ReplaceRadioButton->isChecked() && ui->ReplaceEdit->text().isEmpty())
    setMessage(tr("Replacement value cannot be empty"));
  else if (ui->MultRadioButton->isChecked() && ui->MultEdit->text().isEmpty())
    setMessage(tr("Multiply factor cannot be empty"));
  else if (ui->AddRadioButton->isChecked() && ui->AddEdit->text().isEmpty())
    setMessage(tr("Added value cannot be empty"));
  else
    setMessage();
}


// =====================================================================
// =====================================================================


void EditAttributesValuesDialog::adaptSelection()
{
  QLineEdit* Sender = (QLineEdit*)(QObject::sender());
  if (Sender != NULL)
  {
    if (Sender == ui->ReplaceEdit)
      ui->ReplaceRadioButton->setChecked(true);
    else     if (Sender == ui->MultEdit)
      ui->MultRadioButton->setChecked(true);
    else     if (Sender == ui->AddEdit)
      ui->AddRadioButton->setChecked(true);
  }
}


// =====================================================================
// =====================================================================


void EditAttributesValuesDialog::setMessage(const QString& Msg)
{
  if (Msg.isEmpty())
  {
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(BUILDER_DIALOGBANNER_BGCOLOR));
    ui->MessageLabel->setText(tr("Edit values of selected attributes"));
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


EditAttributesValuesDialog::EditMode EditAttributesValuesDialog::getEditMode() const
{
  if (ui->ReplaceRadioButton->isChecked())
    return EDIT_REPLACE;

  if (ui->MultRadioButton->isChecked())
    return EDIT_MULTIPLY;

  if (ui->AddRadioButton->isChecked())
    return EDIT_ADD;

  return EDIT_UNKNOWN;
}


// =====================================================================
// =====================================================================


QString EditAttributesValuesDialog::getReplaceValue() const
{
  return ui->ReplaceEdit->text();
}


// =====================================================================
// =====================================================================


double EditAttributesValuesDialog::getMultiplyValue() const
{
  return ui->MultEdit->text().toDouble();
}


// =====================================================================
// =====================================================================


double EditAttributesValuesDialog::getAddValue() const
{
  return ui->AddEdit->text().toDouble();
}
