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
  \file AddDatastoreItemDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_EditDatastoreItemDialog.h"
#include "AddDatastoreItemDialog.hpp"
#include <openfluid/ui/config.hpp>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/tools/GDALHelpers.hpp>

#include <gdal.h>
#include <ogr_api.h>

#include <QPushButton>
#include <QFileDialog>



AddDatastoreItemDialog::AddDatastoreItemDialog(const QStringList& ExistingUnitsClass,
  const QStringList& ExistingIDs, QWidget* Parent):
  openfluid::ui::common::OpenFLUIDDialog(Parent),ui(new(Ui::EditDatastoreItemDialog)),
  m_ExistingIDs(ExistingIDs)
{
  ui->setupUi(this);

  GDALAllRegister();
  OGRRegisterAll();

  m_OGRFormatsStr = openfluid::tools::getOGRGDALFormatsForQFileDialogs(openfluid::tools::getOGRFilesDriversForOpenFLUID(),
                                                                          tr("All vector files"));

  m_GDALFormatsStr = openfluid::tools::getOGRGDALFormatsForQFileDialogs(openfluid::tools::getGDALFilesDriversForOpenFLUID(),
                                                                        tr("All raster files"));

  setMessage();

  ui->StaticIDLabel->setVisible(false);
  ui->GeovectorRadioButton->setChecked(true);
  ui->CopyRadioButton->setChecked(true);

  connect(ui->IDEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->UnitsClassCheckBox,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->UnitsClassEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->GeovectorRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->GeorasterRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->CopyRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->CopySubdirRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->CopySubdirEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));

  connect(ui->GeovectorBrowseButton,SIGNAL(clicked()),this,SLOT(selectVectorFile()));
  connect(ui->GeorasterBrowseButton,SIGNAL(clicked()),this,SLOT(selectRasterFile()));

  // completer for units classes
  if (!ExistingUnitsClass.isEmpty())
  {
    mp_Completer = new QCompleter(ExistingUnitsClass, this);
    mp_Completer->setCaseSensitivity(Qt::CaseInsensitive);
    mp_Completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->UnitsClassEdit->setCompleter(mp_Completer);

    connect(mp_Completer,SIGNAL(activated(const QString&)),this,SLOT(checkGlobal()));
  }

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  checkGlobal();
}


// =====================================================================
// =====================================================================


AddDatastoreItemDialog::~AddDatastoreItemDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void AddDatastoreItemDialog::checkGlobal()
{
  ui->IDEdit->setText(ui->IDEdit->text().replace(QRegExp("[^\\w]"),"_"));

  ui->GeovectorEdit->setEnabled(ui->GeovectorRadioButton->isChecked());
  ui->GeovectorBrowseButton->setEnabled(ui->GeovectorRadioButton->isChecked());

  ui->GeorasterEdit->setEnabled(ui->GeorasterRadioButton->isChecked());
  ui->GeorasterBrowseButton->setEnabled(ui->GeorasterRadioButton->isChecked());

  ui->DataDestGroupBox->setEnabled(!isSourceFileInInputDataset());

  if (ui->IDEdit->text().isEmpty())
    setMessage(tr("Item ID cannot be empty"));
  else if (m_ExistingIDs.contains(ui->IDEdit->text()))
    setMessage(tr("Item ID already exists"));
  else if (ui->UnitsClassCheckBox->isChecked() && ui->UnitsClassEdit->text().isEmpty())
    setMessage(tr("Units class cannot be empty"));
  else if (ui->GeovectorRadioButton->isChecked() && ui->GeovectorEdit->text().isEmpty())
    setMessage(tr("No geovector source file selected"));
  else if (ui->GeorasterRadioButton->isChecked() && ui->GeorasterEdit->text().isEmpty())
    setMessage(tr("No georaster source file selected"));
  else if (ui->CopySubdirRadioButton->isChecked() && ui->CopySubdirEdit->text().isEmpty())
    setMessage(tr("Subdirectory for data destination is empty"));
  else if (ui->CopySubdirRadioButton->isChecked() &&
           (ui->CopySubdirEdit->text().startsWith("/") || ui->CopySubdirEdit->text().startsWith("\\")))
    setMessage(tr("Wrong subdirectory name for data destination"));
  else
    setMessage();
}


// =====================================================================
// =====================================================================


void AddDatastoreItemDialog::setMessage(const QString& Msg)
{
  if (Msg.isEmpty())
  {
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR));
    ui->MessageLabel->setText(tr("Add of a datastore item"));
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
  else
  {
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(openfluid::ui::config::DIALOGBANNER_WARNBGCOLOR));
    ui->MessageLabel->setText(Msg);
  }
}


// =====================================================================
// =====================================================================


void AddDatastoreItemDialog::selectVectorFile()
{
  QString SelectedFile =
      QFileDialog::getOpenFileName(this,
                                   tr("Select file"),
                                   "",
                                   m_OGRFormatsStr);

  if (!SelectedFile.isEmpty())
    ui->GeovectorEdit->setText(SelectedFile);

  checkGlobal();
}


// =====================================================================
// =====================================================================


void AddDatastoreItemDialog::selectRasterFile()
{

  QString SelectedFile =
      QFileDialog::getOpenFileName(this,
                                   tr("Select file"),
                                   "",
                                   m_GDALFormatsStr);

  if (!SelectedFile.isEmpty())
    ui->GeorasterEdit->setText(SelectedFile);

  checkGlobal();
}



// =====================================================================
// =====================================================================


QString AddDatastoreItemDialog::getItemID() const
{
  return ui->IDEdit->text();
}


// =====================================================================
// =====================================================================


bool AddDatastoreItemDialog::isUnitsClass() const
{
  return ui->UnitsClassCheckBox->isChecked();
}


// =====================================================================
// =====================================================================


QString AddDatastoreItemDialog::getUnitsClass() const
{
  return ui->UnitsClassEdit->text();
}


// =====================================================================
// =====================================================================


openfluid::core::UnstructuredValue::UnstructuredType AddDatastoreItemDialog::getItemType() const
{
  if (ui->GeovectorRadioButton->isChecked())
    return openfluid::core::UnstructuredValue::GeoVectorValue;
  else
    return openfluid::core::UnstructuredValue::GeoRasterValue;
}


// =====================================================================
// =====================================================================


QString AddDatastoreItemDialog::getSourceFilePath() const
{
  if (ui->GeovectorRadioButton->isChecked())
    return ui->GeovectorEdit->text();
  else
    return ui->GeorasterEdit->text();
}


// =====================================================================
// =====================================================================


bool AddDatastoreItemDialog::isSourceFileInInputDataset() const
{
  QString SourceFile(getSourceFilePath());

  if (SourceFile.isEmpty())
    return false;

  QString InputPath = QString::fromStdString(openfluid::base::RuntimeEnvironment::getInstance()->getInputDir());

  if (QDir::fromNativeSeparators(SourceFile).startsWith(QDir::fromNativeSeparators(InputPath)))
    return true;

  return false;
}


// =====================================================================
// =====================================================================


QString AddDatastoreItemDialog::getCopySubdir() const
{
  if (ui->CopyRadioButton->isChecked())
    return "";
  else
    return ui->CopySubdirEdit->text();
}

