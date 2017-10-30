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
  @file EditSetDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QPushButton>

#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/ui/config.hpp>

#include "ui_EditSetDialog.h"
#include "EditSetDialog.hpp"


EditSetDialog::EditSetDialog(const QStringList& SetNames,
                             const QStringList& FormatNames, const QStringList& ClassNames,
                             QWidget* Parent):
  openfluid::ui::common::OpenFLUIDDialog(Parent),
  ui(new Ui::EditSetDialog), m_ExistingSetsNames(SetNames)
{
  ui->setupUi(this);

  ui->FormatComboBox->addItems(FormatNames);
  ui->UnitsClassComboBox->addItems(ClassNames);

  ui->AllUnitsRadioButton->setChecked(true);
  ui->AllVariablesRadioButton->setChecked(true);

  ui->SetNameEdit->setPlaceholderText(getPlaceholderRequired());

  connect(ui->SetNameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->UnitsClassComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(checkGlobal()));
  connect(ui->UnitsClassComboBox,SIGNAL(editTextChanged(const QString &)),this,SLOT(checkGlobal()));
  connect(ui->SelectedUnitsRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->SelectedUnitsTextEdit,SIGNAL(textChanged()),this,SLOT(checkGlobal()));
  connect(ui->SelectedVariablesRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->SelectedVariablesTextEdit,SIGNAL(textChanged()),this,SLOT(checkGlobal()));

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  checkGlobal();
}


// =====================================================================
// =====================================================================


EditSetDialog::~EditSetDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void EditSetDialog::checkGlobal()
{
  bool ValidLists = true;

  ui->SelectedUnitsTextEdit->setEnabled(ui->SelectedUnitsRadioButton->isChecked());
  ui->SelectedVariablesTextEdit->setEnabled(ui->SelectedVariablesRadioButton->isChecked());

  if (ui->SelectedUnitsRadioButton->isChecked())
  {
    QRegExp Exp("^[0-9]+(;[0-9]+)*$");

    ValidLists = !ui->SelectedUnitsTextEdit->toPlainText().isEmpty() &&
                 Exp.exactMatch(ui->SelectedUnitsTextEdit->toPlainText());
  }

  if (ValidLists && ui->SelectedVariablesRadioButton->isChecked())
  {
    QStringList VarNames = ui->SelectedVariablesTextEdit->toPlainText().split(";");

    for (auto& Name : VarNames)
      ValidLists = ValidLists && openfluid::tools::isValidVariableName(Name.toStdString());
  }

  bool InvalidName = ui->SetNameEdit->text().isEmpty() ||
                    !openfluid::tools::isValidAlphaNumName(ui->SetNameEdit->text().toStdString());

  bool ExistingSet = m_ExistingSetsNames.contains(ui->SetNameEdit->text());

  if (InvalidName || ExistingSet ||
      ui->UnitsClassComboBox->currentText().isEmpty() || !ValidLists)
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  else
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}


// =====================================================================
// =====================================================================


void EditSetDialog::initialize(const QString& Name, const QString& Format,
                               const QString& UnitsClass, const QString& UnitsIDs,
                               const QString& Vars)
{
  ui->SetNameEdit->setText(Name);
  ui->FormatComboBox->setCurrentIndex(ui->FormatComboBox->findText(Format));
  ui->UnitsClassComboBox->lineEdit()->setText(UnitsClass);

  if (UnitsIDs == "*")
    ui->AllUnitsRadioButton->setChecked(true);
  else
  {
    ui->SelectedUnitsRadioButton->setChecked(true);
    ui->SelectedUnitsTextEdit->setPlainText(UnitsIDs);
  }

  if (Vars == "*")
    ui->AllVariablesRadioButton->setChecked(true);
  else
  {
    ui->SelectedVariablesRadioButton->setChecked(true);
    ui->SelectedVariablesTextEdit->setPlainText(Vars);
  }

  checkGlobal();
}


// =====================================================================
// =====================================================================


openfluid::ware::WareParams_t EditSetDialog::getSetParams()
{
  std::string ParamsRoot = "set."+ui->SetNameEdit->text().toStdString()+".";

  std::string FormatStr = ui->FormatComboBox->currentText().toStdString();
  std::string UnitsClassStr = ui->UnitsClassComboBox->currentText().toStdString();

  std::string UnitsIDsStr = "*";
  if (ui->SelectedUnitsRadioButton->isChecked())
    UnitsIDsStr = ui->SelectedUnitsTextEdit->toPlainText().trimmed().toStdString();

  std::string VarsStr = "*";
  if (ui->SelectedVariablesRadioButton->isChecked())
    VarsStr = ui->SelectedVariablesTextEdit->toPlainText().trimmed().toStdString();

  return openfluid::ware::WareParams_t({{ParamsRoot+"format",FormatStr},
                                        {ParamsRoot+"unitsclass",UnitsClassStr},
                                        {ParamsRoot+"unitsIDs",UnitsIDsStr},
                                        {ParamsRoot+"vars",VarsStr}});
}


