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
#include <QDir>
#include <QMessageBox>

#include <openfluid/ui/config.hpp>

namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareExplorerDialog::WareExplorerDialog(QWidget* Parent, const QString& TopDirectoryPath, const QString& CurrentPath,
                                       const QString& Title, const QString& DefaultMessage,
                                       const QString& AcceptButtonLabel) :
    QDialog(Parent), ui(new Ui::WareExplorerDialog), mp_AcceptButton(0), m_DefaulMessage(DefaultMessage)
{
  ui->setupUi(this);

  setWindowTitle(Title);

  setStatus("");

  QRegExp Rx("[a-zA-Z0-9._-]+");
  QValidator* Validator = new QRegExpValidator(Rx, this);
  ui->Filename_lineEdit->setValidator(Validator);

  ui->WareExplorer->configure(TopDirectoryPath, false);

  QString CurrentPathToSet = CurrentPath.isEmpty() ? TopDirectoryPath : CurrentPath;

  ui->WareExplorer->setCurrentPath(CurrentPathToSet);

  QFileInfo CurrentPathToSetFI(CurrentPathToSet);
  ui->Filename_lineEdit->setText(CurrentPathToSetFI.isFile() ? CurrentPathToSetFI.fileName() : "");

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
  WareExplorerDialog Dialog(Parent, TopDirectoryPath, CurrentPath, tr("Save as"), tr("Define the file to save as"),
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


void WareExplorerDialog::setOpenWareMode()
{
  ui->Directoryname_label->setVisible(false);
  ui->Directory_label->setVisible(false);
  ui->Filename_label->setVisible(false);
  ui->Filename_lineEdit->setVisible(false);

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)), this,
          SLOT(onCurrentChangedOpenWareMode(const QString&)));

  onCurrentChangedOpenWareMode(ui->WareExplorer->getCurrentPath());
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setOpenFileMode()
{
  ui->Directoryname_label->setVisible(false);
  ui->Filename_label->setVisible(false);
  ui->Directory_label->setVisible(false);
  ui->Filename_lineEdit->setVisible(false);

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)), this,
          SLOT(onCurrentChangedOpenFileMode(const QString&)));

  onCurrentChangedOpenFileMode(ui->WareExplorer->getCurrentPath());
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setSaveFileMode()
{
  connect(ui->Filename_lineEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(onTextChangedCheckEmpty(const QString&)));

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)), this,
          SLOT(onCurrentChangedSaveFileMode(const QString&)));

  onTextChangedCheckEmpty(ui->Filename_lineEdit->text());
  onCurrentChangedSaveFileMode(ui->WareExplorer->getCurrentPath());
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setCreateFileMode()
{
  connect(ui->Filename_lineEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(onTextChangedCheckExists(const QString&)));

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)), this,
          SLOT(onCurrentChangedSaveFileMode(const QString&)));

  onTextChangedCheckExists(ui->Filename_lineEdit->text());
  onCurrentChangedSaveFileMode(ui->WareExplorer->getCurrentPath());
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


void WareExplorerDialog::onCurrentChangedSaveFileMode(const QString& Path)
{
  QFileInfo Info(Path);

  if (Info.isFile())
  {
    ui->Directory_label->setText(Info.absolutePath());
    ui->Filename_lineEdit->setText(Info.fileName());
  }
  else
    ui->Directory_label->setText(Info.absoluteFilePath());
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onTextChangedCheckEmpty(const QString& Text)
{
  setStatus(Text.isEmpty() ? "You must enter a file name" : "");
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onTextChangedCheckExists(const QString& Text)
{
  QString Msg = "";
  if (Text.isEmpty())
    Msg = "You must enter a file name";
  else if (QFile(getCompleteFilePath()).exists())
    Msg = "You must enter the name of a file that doesn't already exist";

  setStatus(Msg);
}


// =====================================================================
// =====================================================================

QString WareExplorerDialog::getSelectedPath()
{
  return ui->WareExplorer->getCurrentPath();
}


// =====================================================================
// =====================================================================


QString WareExplorerDialog::getEditedFilename()
{
  return ui->Filename_lineEdit->text();
}


// =====================================================================
// =====================================================================


QString WareExplorerDialog::getCompleteFilePath()
{
  QFileInfo Info(getSelectedPath());

  QDir Dir;
  QString NewPath;

  if (Info.isFile())
    Dir = Info.absoluteDir();
  else
    Dir.setPath(Info.filePath());

  return Dir.filePath(getEditedFilename());
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
