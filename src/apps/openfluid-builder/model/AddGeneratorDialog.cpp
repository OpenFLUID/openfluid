/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file AddGeneratorDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/base/ProjectManager.hpp>

#include "ui_AddGeneratorDialog.h"
#include "AddGeneratorDialog.hpp"
#include "builderconfig.hpp"

#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>


AddGeneratorDialog::AddGeneratorDialog(const QStringList& UnitsClasses, QWidget* Parent) :
  OpenFLUIDDialog(Parent),ui(new Ui::AddGeneratorDialog)
{
  ui->setupUi(this);

  ui->MessageLabel->setText(tr("Add generator"));

  connect(ui->FixedRadioButton,SIGNAL(toggled(bool)),this,SLOT(switchGeneratorOptions()));
  connect(ui->RandomRadioButton,SIGNAL(toggled(bool)),this,SLOT(switchGeneratorOptions()));
  connect(ui->InterpRadioButton,SIGNAL(toggled(bool)),this,SLOT(switchGeneratorOptions()));
  connect(ui->InjectRadioButton,SIGNAL(toggled(bool)),this,SLOT(switchGeneratorOptions()));

  connect(ui->VectorRadioButton,SIGNAL(toggled(bool)),ui->VectorSpinBox,SLOT(setEnabled(bool)));

  connect(ui->VarNameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));

  connect(ui->SourcesBrowseButton,SIGNAL(clicked()),this,SLOT(selectSourcesFile()));
  connect(ui->DistriBrowseButton,SIGNAL(clicked()),this,SLOT(selectDistriFile()));

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  switchGeneratorOptions();

  m_Method = openfluid::fluidx::GeneratorDescriptor::Fixed;

  ui->FixedRadioButton->setChecked(true);
  ui->DoubleRadioButton->setChecked(true);
  ui->VectorSpinBox->setEnabled(false);
  ui->UnitsClassComboBox->addItems(UnitsClasses);

  checkGlobal();
}


// =====================================================================
// =====================================================================


AddGeneratorDialog::~AddGeneratorDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void AddGeneratorDialog::setMessage(const QString& Msg)
{
  if (Msg.isEmpty())
  {
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(BUILDER_DIALOGBANNER_BGCOLOR));
    ui->MessageLabel->setText(tr("Add generator"));
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


void AddGeneratorDialog::switchGeneratorOptions()
{
  if (ui->FixedRadioButton->isChecked())
  {
    m_Method = openfluid::fluidx::GeneratorDescriptor::Fixed;
    ui->OptionsWidget->setCurrentIndex(0);
  }
  else if (ui->RandomRadioButton->isChecked())
  {
    m_Method = openfluid::fluidx::GeneratorDescriptor::Random;
    ui->OptionsWidget->setCurrentIndex(1);
  }
  else if (ui->InterpRadioButton->isChecked())
  {
    m_Method = openfluid::fluidx::GeneratorDescriptor::Interp;
    ui->OptionsWidget->setCurrentIndex(2);
  }
  else if (ui->InjectRadioButton->isChecked())
  {
    m_Method = openfluid::fluidx::GeneratorDescriptor::Inject;
    ui->OptionsWidget->setCurrentIndex(2);
  }
}


// =====================================================================
// =====================================================================


void AddGeneratorDialog::checkGlobal()
{
  if (ui->VarNameEdit->text().isEmpty())
    setMessage(tr("Variable name cannot be empty"));
  else if (ui->UnitsClassComboBox->currentText().isEmpty())
    setMessage(tr("Unit class cannot be empty"));
  else
    setMessage();
}


// =====================================================================
// =====================================================================


QString AddGeneratorDialog::getVariableName() const
{
  return ui->VarNameEdit->text();
}


// =====================================================================
// =====================================================================


QString AddGeneratorDialog::getUnitClass() const
{
  return ui->UnitsClassComboBox->currentText();
}


// =====================================================================
// =====================================================================


unsigned int AddGeneratorDialog::getVariableSize() const
{
  if (ui->DoubleRadioButton->isChecked())
    return 1;
  else return ui->VectorSpinBox->value();
}


// =====================================================================
// =====================================================================


openfluid::ware::WareParams_t AddGeneratorDialog::getParams() const
{
  openfluid::ware::WareParams_t Params;

  if (m_Method == openfluid::fluidx::GeneratorDescriptor::Fixed)
  {
    if (!ui->ValueEdit->text().isEmpty()) Params["fixedvalue"] = ui->ValueEdit->text().toStdString();
  }
  else if (m_Method == openfluid::fluidx::GeneratorDescriptor::Random)
  {
    if (!ui->MinValueEdit->text().isEmpty()) Params["min"] = ui->MinValueEdit->text().toStdString();
    if (!ui->MaxValueEdit->text().isEmpty()) Params["max"] = ui->MaxValueEdit->text().toStdString();
  }
  else if (m_Method == openfluid::fluidx::GeneratorDescriptor::Interp)
  {
    if (!ui->SourcesFileEdit->text().isEmpty()) Params["sources"] = ui->SourcesFileEdit->text().toStdString();
    if (!ui->DistriFileEdit->text().isEmpty()) Params["distribution"] = ui->DistriFileEdit->text().toStdString();
  }
  else if (m_Method == openfluid::fluidx::GeneratorDescriptor::Inject)
  {
    if (!ui->SourcesFileEdit->text().isEmpty()) Params["sources"] = ui->SourcesFileEdit->text().toStdString();
    if (!ui->DistriFileEdit->text().isEmpty()) Params["distribution"] = ui->DistriFileEdit->text().toStdString();
  }

  return Params;
}


// =====================================================================
// =====================================================================


void AddGeneratorDialog::selectSourcesFile()
{
  QString CurrentDir = QString::fromStdString(openfluid::base::ProjectManager::getInstance()->getInputDir());

  QString FileName = QFileDialog::getOpenFileName(this,
                                                  tr("Select source file"),
                                                  CurrentDir,
                                                  tr("All files (*.*)"));
  if (FileName.startsWith(CurrentDir))
  {
    ui->SourcesFileEdit->setText(FileName.remove(0,CurrentDir.length()+1));
  }
  else
  {
    QMessageBox::critical(QApplication::activeWindow(),"OpenFLUID-Builder",tr("Choosen file is not located in the input dataset of the project"),QMessageBox::Close);
  }

}


// =====================================================================
// =====================================================================


void AddGeneratorDialog::selectDistriFile()
{
  QString CurrentDir = QString::fromStdString(openfluid::base::ProjectManager::getInstance()->getInputDir());

  QString FileName = QFileDialog::getOpenFileName(this,
                                                  tr("Select distribution file"),
                                                  CurrentDir,
                                                  tr("All files (*.*)"));
  if (FileName.startsWith(CurrentDir))
  {
    ui->DistriFileEdit->setText(FileName.remove(0,CurrentDir.length()+1));
  }
  else
  {
    QMessageBox::critical(QApplication::activeWindow(),"OpenFLUID-Builder",tr("Choosen file is not located in the input dataset of the project"),QMessageBox::Close);
  }

}


