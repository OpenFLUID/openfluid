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


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareExplorerDialog::WareExplorerDialog(QWidget* Parent, const QString& TopDirectoryPath, const QString& CurrentPath) :
    QDialog(Parent), ui(new Ui::WareExplorerDialog), mp_AcceptButton(0)
{
  ui->setupUi(this);

  QRegExp Rx("[a-zA-Z0-9.-]+");
  QValidator* Validator = new QRegExpValidator(Rx, this);
  ui->Filename_lineEdit->setValidator(Validator);

  ui->WareExplorer->configure(TopDirectoryPath, false);

  QString CurrentPathToSet = CurrentPath.isEmpty() ? TopDirectoryPath : CurrentPath;

  ui->WareExplorer->setCurrentPath(CurrentPathToSet);
  ui->Filename_lineEdit->setText(QFileInfo(CurrentPathToSet).fileName());

  mp_Manager = openfluid::waresdev::WareSrcManager::instance();

  mp_AcceptButton = ui->buttonBox->button(QDialogButtonBox::Open);
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
  WareExplorerDialog Dialog(Parent, TopDirectoryPath, CurrentPath);
  Dialog.setOpenWareMode();
  if (!Title.isEmpty())
    Dialog.setWindowTitle(Title);

  if (Dialog.exec())
    return Dialog.getSelectedPath();

  return "";
}


// =====================================================================
// =====================================================================


QString WareExplorerDialog::getOpenFilePath(QWidget* Parent, const QString& TopDirectoryPath,
                                            const QString& CurrentPath)
{
  WareExplorerDialog Dialog(Parent, TopDirectoryPath, CurrentPath);
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
  WareExplorerDialog Dialog(Parent, TopDirectoryPath, CurrentPath);
  Dialog.setSaveFileMode();

  if (Dialog.exec())
  {
    QFileInfo Info(Dialog.getSelectedPath());

    QDir Dir;
    QString NewPath;

    if (Info.isFile())
      Dir = Info.absoluteDir();
    else
      Dir.setPath(Info.filePath());

    NewPath = Dir.filePath(Dialog.getEditedFilename());

    int Res = QMessageBox::Ok;

    if (QFileInfo(NewPath).exists())
      Res = QMessageBox::warning(Parent, tr("Save as"), tr("This file already exists.\n"
                                                           "Do you want to replace it?"),
                                 QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);

    return Res == QMessageBox::Ok ? NewPath : "";
  }

  return "";
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::setOpenWareMode()
{
  setWindowTitle(tr("Open ware"));

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
  setWindowTitle(tr("Open file"));

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
  setWindowTitle(tr("Save as"));

  mp_AcceptButton->setText(tr("Save"));

  connect(ui->Filename_lineEdit, SIGNAL(textChanged ( const QString & )), this, SLOT(onTextChanged(const QString&)));

  connect(ui->WareExplorer, SIGNAL(currentChanged(const QString&)), this,
          SLOT(onCurrentChangedSaveFileMode(const QString&)));

  onTextChanged(ui->Filename_lineEdit->text());
  onCurrentChangedSaveFileMode(ui->WareExplorer->getCurrentPath());
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onCurrentChangedOpenWareMode(const QString& Path)
{
  if (mp_AcceptButton)
    mp_AcceptButton->setEnabled(mp_Manager->getPathInfo(Path).m_isAWare);
}


// =====================================================================
// =====================================================================


void WareExplorerDialog::onCurrentChangedOpenFileMode(const QString& Path)
{
  if (mp_AcceptButton)
    mp_AcceptButton->setEnabled(QFileInfo(Path).isFile());

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


void WareExplorerDialog::onTextChanged(const QString & text)
{
  if (mp_AcceptButton)
    mp_AcceptButton->setEnabled(!text.isEmpty());
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


} } }  // namespaces
