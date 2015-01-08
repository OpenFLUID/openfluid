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
  @file NewProjectDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <QFileDialog>
#include <QMessageBox>
#include <QFileSystemModel>

#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/base/PreferencesManager.hpp>

#include "ui_NewProjectDialog.h"
#include "NewProjectDialog.hpp"
#include <openfluid/ui/config.hpp>



NewProjectDialog::NewProjectDialog(QWidget *Parent):
  openfluid::ui::common::OpenFLUIDDialog(Parent), ui(new Ui::NewProjectDialog), mp_DirectoryModel(new QFileSystemModel(this))
{
  ui->setupUi(this);

  ui->WorkdirLabel->setText(QDir::toNativeSeparators(openfluid::base::PreferencesManager::instance()->getProjectsPath()));


  connect(ui->WorkdirButton,SIGNAL(clicked()),this,SLOT(onWorkdirButtonClicked()));
  connect(ui->ProjectButton,SIGNAL(clicked()),this,SLOT(onProjectButtonClicked()));
  connect(ui->DatadirButton,SIGNAL(clicked()),this,SLOT(onDatadirButtonClicked()));

  connect(ui->NameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(onGlobalCheck()));
  connect(ui->DataGroupBox,SIGNAL(toggled(bool)),this,SLOT(onGlobalCheck()));
  connect(ui->ProjectRadioButton,SIGNAL(toggled(bool)),this,SLOT(onGlobalCheck()));
  connect(ui->DirectoryRadioButton,SIGNAL(toggled(bool)),this,SLOT(onGlobalCheck()));

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  ui->DataGroupBox->setChecked(false);

  onGlobalCheck();
}


// =====================================================================
// =====================================================================


NewProjectDialog::~NewProjectDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void NewProjectDialog::onGlobalCheck()
{
  ui->NameEdit->setText(ui->NameEdit->text().replace(QRegExp("[^\\w]"),"_"));

  if (ui->NameEdit->text().isEmpty())
  {
    setMessage(tr("Project name cannot be empty"));
  }
  else if (QDir(ui->WorkdirLabel->text()+"/"+ui->NameEdit->text()).exists())
  {
    setMessage(tr("Project already exists"));
  }
  else if (ui->DataGroupBox->isChecked() && ui->ProjectRadioButton->isChecked() && ui->ProjectLabel->text() == "(none)")
  {
    setMessage(tr("Imported project is not selected"));
  }
  else if (ui->DataGroupBox->isChecked() && ui->DirectoryRadioButton->isChecked() && ui->DirectoryLabel->text() == "(none)")
  {
    setMessage(tr("Imported data directory is not selected"));
  }
  else
    setMessage();

}


// =====================================================================
// =====================================================================


void NewProjectDialog::setMessage(const QString& Msg)
{
  if (Msg.isEmpty())
  {
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR));
    ui->MessageLabel->setText(tr("Creation of a new OpenFLUID project"));
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
  else
  {
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->MessageFrame->setStyleSheet(QString("background-color: %1")
                                    .arg(openfluid::ui::config::DIALOGBANNER_WARNBGCOLOR));
    ui->MessageLabel->setText(Msg);
  }
}


// =====================================================================
// =====================================================================


void NewProjectDialog::onWorkdirButtonClicked()
{
  QString SelectedDir = QFileDialog::getExistingDirectory(this,tr("Select working directory"));
  if (SelectedDir !=  "")
  {
    QString NativePath = QDir::toNativeSeparators(SelectedDir);

    ui->WorkdirLabel->setText(NativePath);
    ui->WorkdirLabel->setToolTip(NativePath);

    onGlobalCheck();
  }
}


// =====================================================================
// =====================================================================


void NewProjectDialog::onProjectButtonClicked()
{
  QString SelectedDir = QFileDialog::getExistingDirectory(this,tr("Select project"));
  if (SelectedDir !=  "")
  {
    QString NativePath = QDir::toNativeSeparators(SelectedDir);

    if (openfluid::base::ProjectManager::isProject(SelectedDir.toStdString()))
    {
      ui->ProjectLabel->setText(NativePath);
      ui->ProjectLabel->setToolTip(NativePath);
      ui->ProjectLabel->setStyleSheet("font: normal;");
      onGlobalCheck();
    }
    else
      QMessageBox::critical(this,
                            tr("Project error"),
                            tr("%1\n\nis not a valid OpenFLUID project").arg(NativePath));
  }
}


// =====================================================================
// =====================================================================


void NewProjectDialog::onDatadirButtonClicked()
{
  QString SelectedDir = QFileDialog::getExistingDirectory(this,tr("Select directory"));
  if (SelectedDir !=  "")
  {
    QString NativePath = QDir::toNativeSeparators(SelectedDir);

    ui->DirectoryLabel->setText(NativePath);
    ui->DirectoryLabel->setToolTip(NativePath);
    ui->DirectoryLabel->setStyleSheet("font: normal;");
    onGlobalCheck();

    mp_DirectoryModel->setRootPath(NativePath);
    ui->DirectoryView->setModel(mp_DirectoryModel);
    ui->DirectoryView->setRootIndex(mp_DirectoryModel->setRootPath(NativePath));
  }
}


// =====================================================================
// =====================================================================


QString NewProjectDialog::getProjectName() const
{
  return ui->NameEdit->text();
}


// =====================================================================
// =====================================================================


QString NewProjectDialog::getProjectFullPath() const
{
  return QDir::fromNativeSeparators(QDir(getWorkingDir()+"/"+getProjectName()).path());
}


// =====================================================================
// =====================================================================


QString NewProjectDialog::getWorkingDir() const
{
  return ui->WorkdirLabel->text();
}


// =====================================================================
// =====================================================================


QString NewProjectDialog::getProjectDescription() const
{
  return ui->DescriptionEdit->toPlainText();
}


// =====================================================================
// =====================================================================


QString NewProjectDialog::getProjectAuthors() const
{
  return ui->AuthorsEdit->text();
}


// =====================================================================
// =====================================================================


NewProjectDialog::ImportType NewProjectDialog::getImportType() const
{
  if (ui->DataGroupBox->isChecked())
  {
    if (ui->ProjectRadioButton->isChecked()) return IMPORT_PROJECT;
    else return IMPORT_DIRECTORY;
  }
  else return IMPORT_NONE;
}


// =====================================================================
// =====================================================================


QString NewProjectDialog::getImportSource() const
{
  if (getImportType() == IMPORT_PROJECT) return ui->ProjectLabel->text();
  if (getImportType() == IMPORT_DIRECTORY) return ui->DirectoryLabel->text();
  return "";
}

