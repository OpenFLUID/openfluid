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
 @file WareExplorerDialog.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/ui/waresdev/WareExplorerDialog.hpp>

#include "ui_WareExplorerDialog.h"

#include <QFileInfo>
#include <QMessageBox>

#include <openfluid/ui/config.hpp>

namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareExplorerDialog::WareExplorerDialog(QWidget* Parent, const QString& TopDirectoryPath, const QString& CurrentPath,
                                       const QString& Title, const QString& DefaultMessage,
                                       const QString& AcceptButtonLabel) :
    QDialog(Parent), ui(new Ui::WareExplorerDialog), mp_AcceptButton(0), m_DefaulMessage(DefaultMessage),
    m_TopDir(QDir(TopDirectoryPath))
{
  ui->setupUi(this);

  setWindowTitle(Title);

  ui->Filepath_lineEdit->setFocus();

  QRegExp FilepathRx(QString("[a-zA-Z0-9._%1-]+").arg(QDir::separator()));
  ui->Filepath_lineEdit->setValidator(new QRegExpValidator(FilepathRx, this));
  ui->Filepath_lineEdit->setToolTip(
      tr("Accepts only letters, digits, dashes ('-'), underscores ('_'), dots ('.') and folder separators ('%1').").arg(
          QDir::separator()));
  ui->Filepath_lineEdit->setPlaceholderText(openfluid::ui::config::PLACEHOLDER_REQUIRED);

  ui->WareExplorer->configure(TopDirectoryPath, false);
  ui->ParentDir_label->setText(QDir::toNativeSeparators(TopDirectoryPath));

  ui->WareExplorer->setCurrentPath(CurrentPath.isEmpty() ? TopDirectoryPath : CurrentPath);

  mp_Manager = openfluid::waresdev::WareSrcManager::instance();

  mp_AcceptButton = ui->buttonBox->button(QDialogButtonBox::Open);
  if (mp_AcceptButton)
    mp_AcceptButton->setText(AcceptButtonLabel);
}


// =====================================================================
// =====================================================================


WareExplorerDialog::~WareExplorerDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


QString WareExplorerDialog::getOpenWarePath(QWidget* Parent, const QString& TopDirectoryPath, const QString& Title,
                                            const QString& CurrentPath)
{
  WareExplorerDialog Dialog(Parent, TopDirectoryPath, CurrentPath, Title.isEmpty() ? "Open ware" : Title,
                            tr("Choose the directory of the ware to open"), tr("Open"));
  Dialog.setOpenWareMode();

  if (Dialog.exec())
    return Dialog.getSelectedPath();

  return "";
}


// =====================================================================
// =====================================================================


QString WareExplorerDialog::getOpenFilePath(QWidget* Parent, const QString& TopDirectoryPath,
                                            const QString& CurrentPath)
{
  WareExplorerDialog Dialog(Parent, TopDirectoryPath, CurrentPath, tr("Open file"), tr("Choose the file to open"),
                            tr("Open"));
  Dialog.setOpenFileMode();

  if (Dialog.exec())
    return Dialog.getSelectedPath();

  return "";
}


// =====================================================================
// =====================================================================


QString WareExplorerDialog::getSaveFilePath(QWidget* Parent, const QString& TopDirectoryPath,
                                            const QString& CurrentPath)
{
  WareExplorerDialog Dialog(
      Parent, TopDirectoryPath, CurrentPath, tr("Save as"),
      tr("Save \"%1\" as...").arg(QDir::toNativeSeparators(QDir(TopDirectoryPath).relativeFilePath(CurrentPath))),
      tr("Save"));
  Dialog.setSaveFileMode();

  if (Dialog.exec())
  {
    QString NewPath = Dialog.getCompleteFilePath();

    int Res = QMessageBox::Ok;

    if (QFileInfo(NewPath).exists())
      Res = QMessageBox::warning(&Dialog, tr("Save as"), tr("This file already exists.\n"
                                                            "Do you want to replace it?"),
                                 QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);

    return Res == QMessageBox::Ok ? NewPath : "";
  }

  return "";
}


// =====================================================================
// =====================================================================


QString WareExplorerDialog::getCreateFilePath(QWidget* Parent, const QString& TopDirectoryPath,
                                              const QString& CurrentPath)
{
  WareExplorerDialog Dialog(Parent, TopDirectoryPath, CurrentPath, tr("New file"), tr("Define the file to create"),
                            tr("OK"));
  Dialog.setCreateFileMode();

  if (Dialog.exec())
    return Dialog.getCompleteFilePath();

  return "";
}


// =====================================================================
// =====================================================================


QString WareExplorerDialog::getCreateFolderPath(QWidget* Parent, const QString& TopDirectoryPath,
                                                const QString& CurrentPath)
{
  WareExplorerDialog Dialog(Parent, TopDirectoryPath, CurrentPath, tr("New folder"), tr("Define the folder to create"),
                            tr("OK"));
  Dialog.setCreateFolderMode();

  if (Dialog.exec())
    return Dialog.getCompleteFilePath();

  return "";
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setOpenWareMode()
{
  ui->Filepath_widget->setVisible(false);

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)), this,
          SLOT(onCurrentChangedOpenWareMode(const QString&)));

  onCurrentChangedOpenWareMode(ui->WareExplorer->getCurrentPath());
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setOpenFileMode()
{
  ui->Filepath_widget->setVisible(false);

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)), this,
          SLOT(onCurrentChangedOpenFileMode(const QString&)));

  connect(ui->WareExplorer, SIGNAL(doubleClicked(const QString&)), this, SLOT(onDoubleClickedFileMode()));

  onCurrentChangedOpenFileMode(ui->WareExplorer->getCurrentPath());
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setSaveFileMode()
{
  connect(ui->Filepath_lineEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(onTextChangedSaveMode(const QString&)));

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)), this,
          SLOT(onCurrentChangedSaveCreateMode(const QString&)));

  onTextChangedSaveMode(ui->Filepath_lineEdit->text());
  onCurrentChangedSaveCreateMode(ui->WareExplorer->getCurrentPath());
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setCreateFileMode()
{
  connect(ui->Filepath_lineEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(onTextChangedCreateFileMode(const QString&)));

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)), this,
          SLOT(onCurrentChangedSaveCreateMode(const QString&)));

  onTextChangedCreateFileMode(ui->Filepath_lineEdit->text());
  onCurrentChangedSaveCreateMode(ui->WareExplorer->getCurrentPath());

  ui->Filepath_lineEdit->setText("file.txt");
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setCreateFolderMode()
{
  connect(ui->Filepath_lineEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(onTextChangedCreateFileMode(const QString&)));

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)), this,
          SLOT(onCurrentChangedSaveCreateMode(const QString&)));

  onTextChangedCreateFolderMode(ui->Filepath_lineEdit->text());
  onCurrentChangedSaveCreateMode(ui->WareExplorer->getCurrentPath());

  ui->Filepath_lineEdit->setText("new_folder");
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onCurrentChangedOpenWareMode(const QString& Path)
{
  setStatus(mp_Manager->getPathInfo(Path).m_isAWare ? "" : "You must select a ware directory");
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onCurrentChangedOpenFileMode(const QString& Path)
{
  setStatus(QFileInfo(Path).isFile() ? "" : "You must select a file");
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onCurrentChangedSaveCreateMode(const QString& Path)
{
  ui->Filepath_lineEdit->setText(QDir::toNativeSeparators(m_TopDir.relativeFilePath(Path)));
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onTextChangedSaveMode(const QString& Text)
{
  QString Msg = "";
  if (Text.isEmpty())
    Msg = "You must enter a file path";
  else if (QFileInfo(getCompleteFilePath()).isDir())
    Msg = "You must enter the path of a file";
  else if (m_TopDir.relativeFilePath(QDir::fromNativeSeparators(Text)).startsWith(".."))
    Msg = "The file path must be below the parent directory";

  setStatus(Msg);
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onTextChangedCreateFileMode(const QString& Text)
{
  QString Msg = "";
  if (Text.isEmpty())
    Msg = "You must enter a file path";
  else if (QFile(getCompleteFilePath()).exists())
    Msg = "You must enter the path of a file that doesn't already exist";
  else if (m_TopDir.relativeFilePath(QDir::fromNativeSeparators(Text)).startsWith(".."))
    Msg = "The file path must be below the parent directory";

  setStatus(Msg);
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onTextChangedCreateFolderMode(const QString& Text)
{
  QString Msg = "";
  if (Text.isEmpty())
    Msg = "You must enter a folder path";
  else if (QFile(getCompleteFilePath()).exists())
    Msg = "You must enter the path of a folder that doesn't already exist";
  else if (m_TopDir.relativeFilePath(QDir::fromNativeSeparators(Text)).startsWith(".."))
    Msg = "The folder path must be below the parent directory";

  setStatus(Msg);
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onDoubleClickedFileMode()
{
  if (mp_AcceptButton && mp_AcceptButton->isEnabled())
    accept();
}


// =====================================================================
// =====================================================================

QString WareExplorerDialog::getSelectedPath()
{
  return ui->WareExplorer->getCurrentPath();
}


// =====================================================================
// =====================================================================


QString WareExplorerDialog::getCompleteFilePath()
{
  return m_TopDir.absoluteFilePath(QDir::fromNativeSeparators(ui->Filepath_lineEdit->text()));
}


// =====================================================================
// =====================================================================

void WareExplorerDialog::setStatus(const QString WarningMsg)
{
  bool Ok = WarningMsg.isEmpty();

  ui->MessageLabel->setText(Ok ? m_DefaulMessage : WarningMsg);

  ui->MessageFrame->setStyleSheet(
      QString("background-color: %1;").arg(
          Ok ? openfluid::ui::config::DIALOGBANNER_BGCOLOR : openfluid::ui::config::DIALOGBANNER_WARNBGCOLOR));

  if (mp_AcceptButton)
    mp_AcceptButton->setEnabled(Ok);
}


// =====================================================================
// =====================================================================


} } }  // namespaces
