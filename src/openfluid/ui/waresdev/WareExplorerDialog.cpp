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


#ifndef QT_VERSION_MAJOR
#pragma message "Qt version not found in source"
#else
#pragma message "Qt version found in source"
#endif


#include <QFileInfo>
#include <QMessageBox>

#include <openfluid/ui/config.hpp>
#include <openfluid/ui/waresdev/WareExplorerDialog.hpp>

#include "ui_WareExplorerDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


WareExplorerDialog::WareExplorerDialog(QWidget* Parent, const QString& TopDirectoryPath, const QString& CurrentPath,
                                       const QString& Title, const QString& DefaultMessage,
                                       const QString& AcceptButtonLabel) :
  openfluid::ui::common::MessageDialog(Parent),
  ui(new Ui::WareExplorerDialog), mp_AcceptButton(nullptr),
  m_TopDir(QDir(TopDirectoryPath))
{
  ui->setupUi(this);

  setupMessageUi(DefaultMessage, QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  setWindowTitle(Title);

  ui->FilepathEdit->setFocus();

#if (QT_VERSION_MAJOR < 6)
  QRegExp FilepathRx(QString("[a-zA-Z0-9._%1-]+").arg(QDir::separator()));
  ui->FilepathEdit->setValidator(new QRegExpValidator(FilepathRx, this));
#else
  QRegularExpression FilepathRx(QString("[a-zA-Z0-9._%1-]+").arg(QDir::separator()));
  ui->FilepathEdit->setValidator(new QRegularExpressionValidator(FilepathRx, this));
#endif
  ui->FilepathEdit->setToolTip(
      tr("Accepts only letters, digits, dashes ('-'), underscores ('_'), dots ('.') and paths separators ('%1').")
      .arg(QDir::separator()));

  // "required" placeholder
  ui->FilepathEdit->setPlaceholderText(getPlaceholderRequired());

  ui->WareExplorer->configure(TopDirectoryPath, false);
  ui->ParentDir_label->setText(QDir::toNativeSeparators(TopDirectoryPath));

  ui->WareExplorer->setCurrentPath(CurrentPath.isEmpty() ? TopDirectoryPath : CurrentPath);

  mp_AcceptButton = buttonOfButtonBox(QDialogButtonBox::Ok);
  if (mp_AcceptButton)
  {
    mp_AcceptButton->setText(AcceptButtonLabel);
  }
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
  WareExplorerDialog Dialog(Parent, TopDirectoryPath, CurrentPath,
                            Title.isEmpty() ? tr("Open ware") : Title,
                            tr("Open a ware"), tr("Open"));
  Dialog.setOpenWareMode();

  if (Dialog.exec())
  {
    return Dialog.getSelectedPath();
  }

  return "";
}


// =====================================================================
// =====================================================================


QString WareExplorerDialog::getOpenFilePath(QWidget* Parent, const QString& TopDirectoryPath,
  const QString& CurrentPath)
{
  WareExplorerDialog Dialog(Parent, TopDirectoryPath, CurrentPath, tr("Open file"),
                            tr("Open a file"),
                            tr("Open"));
  Dialog.setOpenFileMode();

  if (Dialog.exec())
  {
    return Dialog.getSelectedPath();
  }

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
    {
      Res = QMessageBox::warning(&Dialog, tr("Save as"), tr("This file already exists.\n"
                                                            "Do you want to replace it?"),
                                 QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
    }

    return Res == QMessageBox::Ok ? NewPath : "";
  }

  return "";
}


// =====================================================================
// =====================================================================


QString WareExplorerDialog::getCreateFilePath(QWidget* Parent, const QString& TopDirectoryPath,
  const QString& CurrentPath)
{
  WareExplorerDialog Dialog(Parent, TopDirectoryPath, CurrentPath, tr("New file"),
                            tr("Create a file"),
                            tr("OK"));
  Dialog.setCreateFileMode(QDir(TopDirectoryPath).relativeFilePath(CurrentPath));

  if (Dialog.exec())
  {
    return Dialog.getCompleteFilePath();
  }

  return "";
}


// =====================================================================
// =====================================================================


QString WareExplorerDialog::getCreateFolderPath(QWidget* Parent, const QString& TopDirectoryPath,
  const QString& CurrentPath)
{
  WareExplorerDialog Dialog(Parent, TopDirectoryPath, CurrentPath, tr("New folder"),
                            tr("Create a folder"),
                            tr("OK"));
  Dialog.setCreateFolderMode();

  if (Dialog.exec())
  {
    return Dialog.getCompleteFilePath();
  }

  return "";
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setOpenWareMode()
{
  ui->FilepathWidget->setVisible(false);

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)),
          this, SLOT(onCurrentChangedOpenWareMode(const QString&)));

  onCurrentChangedOpenWareMode(ui->WareExplorer->getCurrentPath());
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setOpenFileMode()
{
  ui->FilepathWidget->setVisible(false);

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)),
          this, SLOT(onCurrentChangedOpenFileMode(const QString&)));

  connect(ui->WareExplorer, SIGNAL(doubleClicked(const QString&)),
          this, SLOT(onDoubleClickedFileMode()));

  onCurrentChangedOpenFileMode(ui->WareExplorer->getCurrentPath());
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setSaveFileMode()
{
  connect(ui->FilepathEdit, SIGNAL(textChanged(const QString &)),
          this, SLOT(onTextChangedSaveMode(const QString&)));

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)),
          this, SLOT(onCurrentChangedSaveCreateMode(const QString&)));

  onTextChangedSaveMode(ui->FilepathEdit->text());
  onCurrentChangedSaveCreateMode(ui->WareExplorer->getCurrentPath());
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setCreateFileMode(const QString& CurrentPath)
{
  connect(ui->FilepathEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(onTextChangedCreateFileMode(const QString&)));

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)), this,
          SLOT(onCurrentChangedSaveCreateMode(const QString&)));

  ui->FilepathEdit->setText(CurrentPath);
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setCreateFolderMode()
{
  connect(ui->FilepathEdit, SIGNAL(textChanged(const QString &)),
          this, SLOT(onTextChangedCreateFileMode(const QString&)));

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)),
          this, SLOT(onCurrentChangedSaveCreateMode(const QString&)));

  onTextChangedCreateFolderMode(ui->FilepathEdit->text());
  onCurrentChangedSaveCreateMode(ui->WareExplorer->getCurrentPath());

  ui->FilepathEdit->setText("new_folder");
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onCurrentChangedOpenWareMode(const QString& Path)
{
  setMessage(openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(Path.toStdString()).IsWareDirectory ? 
             "" : tr("No ware directory selected"));
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onCurrentChangedOpenFileMode(const QString& Path)
{
  setMessage(QFileInfo(Path).isFile() ? "" : tr("No file selected"));
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onCurrentChangedSaveCreateMode(const QString& Path)
{
  ui->FilepathEdit->setText(QDir::toNativeSeparators(m_TopDir.relativeFilePath(Path)));
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onTextChangedSaveMode(const QString& Text)
{
  QString Msg = "";
  if (Text.isEmpty())
  {
    Msg = tr("No file path");
  }
  else if (QFileInfo(getCompleteFilePath()).isDir())
  {
    Msg = tr("Path is not a file");
  }  
  else if (m_TopDir.relativeFilePath(QDir::fromNativeSeparators(Text)).startsWith(".."))
  {
    Msg = tr("File path is not inside the parent directory");
  }

  setMessage(Msg);
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onTextChangedCreateFileMode(const QString& Text)
{
  QString Msg = "";
  if (Text.isEmpty())
  {
    Msg = tr("No file path");
  }
  else if (QFile(getCompleteFilePath()).exists())
  {
    Msg = tr("File already exists");
  }  
  else if (m_TopDir.relativeFilePath(QDir::fromNativeSeparators(Text)).startsWith(".."))
  {
    Msg = tr("File path is not inside the parent directory");
  }

  setMessage(Msg);
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onTextChangedCreateFolderMode(const QString& Text)
{
  QString Msg = "";
  if (Text.isEmpty())
  {
    Msg = tr("No folder path");
  }
  else if (QFile(getCompleteFilePath()).exists())
  {
    Msg = tr("Folder already exists");
  }  
  else if (m_TopDir.relativeFilePath(QDir::fromNativeSeparators(Text)).startsWith(".."))
  {
    Msg = tr("Folder path is not inside the parent directory");
  }

  setMessage(Msg);
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onDoubleClickedFileMode()
{
  if (mp_AcceptButton && mp_AcceptButton->isEnabled())
  {
    accept();
  }
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
  return m_TopDir.absoluteFilePath(QDir::fromNativeSeparators(ui->FilepathEdit->text()));
}


} } } // namespaces
