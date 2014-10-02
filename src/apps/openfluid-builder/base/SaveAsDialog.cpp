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
  \file SaveAsDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/ProjectManager.hpp>

#include "ui_SaveAsDialog.h"
#include "SaveAsDialog.hpp"
#include "builderconfig.hpp"

#include <QPushButton>
#include <QFileDialog>


SaveAsDialog::SaveAsDialog(QWidget* Parent) :
  OpenFLUIDDialog(Parent),ui(new Ui::SaveAsDialog)
{
  ui->setupUi(this);

  ui->DirectoryLabel->setText(QDir::toNativeSeparators(openfluid::base::PreferencesManager::getInstance()->getProjectsPath()));
  ui->ProjectNameEdit->setText(QString::fromStdString(openfluid::base::ProjectManager::getInstance()->getName()));

  projectChanged();

  connect(ui->BrowseButton,SIGNAL(clicked()),this,SLOT(browseClicked()));
  connect(ui->ProjectNameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(projectChanged()));

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  adjustSize();
}


// =====================================================================
// =====================================================================


SaveAsDialog::~SaveAsDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void SaveAsDialog::browseClicked()
{
  QString SelectedDir = QFileDialog::getExistingDirectory(QApplication::activeWindow(),tr("Select directory"),
                                                          ui->DirectoryLabel->text());
  if (!SelectedDir.isEmpty())
  {
    ui->DirectoryLabel->setText(SelectedDir);
    projectChanged();
  }
}


// =====================================================================
// =====================================================================


void SaveAsDialog::projectChanged()
{
  ui->ProjectNameEdit->setText(ui->ProjectNameEdit->text().replace(QRegExp("[^\\w]"),"_"));

  m_PrjDir.setPath(QDir::fromNativeSeparators(ui->DirectoryLabel->text()+"/"+ui->ProjectNameEdit->text()));

  checkGlobal();
}


// =====================================================================
// =====================================================================


void SaveAsDialog::checkGlobal()
{
  if (openfluid::base::ProjectManager::isProject(m_PrjDir.path().toStdString()))
    setMessage(tr("Project already exists"));
  else if (m_PrjDir.exists())
    setMessage(tr("Project directory already exist"));
  else
    setMessage();
}


// =====================================================================
// =====================================================================


void SaveAsDialog::setMessage(const QString& Msg)
{
  if (Msg.isEmpty())
  {
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(BUILDER_DIALOGBANNER_BGCOLOR));
    ui->MessageLabel->setText(tr("Save project as..."));
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


QString SaveAsDialog::getProjectName() const
{
  return ui->ProjectNameEdit->text();
}


// =====================================================================
// =====================================================================


QString SaveAsDialog::getProjectFullPath() const
{
  return m_PrjDir.path();
}
