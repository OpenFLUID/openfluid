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
  @file OutputsWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <QFileSystemModel>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QApplication>

#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include "ui_OutputsWidget.h"
#include "OutputsWidget.hpp"


OutputsWidget::OutputsWidget(QWidget* Parent, openfluid::fluidx::AdvancedFluidXDescriptor& AFXDesc):
  WorkspaceWidget(Parent,AFXDesc), ui(new Ui::OutputsWidget), mp_FSModel(new QFileSystemModel(this))
{
  ui->setupUi(this);

  QDir("").mkpath(QString(openfluid::base::ProjectManager::instance()->getOutputDir().c_str()));

  ui->OutputDirView->setModel(mp_FSModel);
  refreshOutputDir();

  connect(ui->OutputDirView,SIGNAL(doubleClicked(const QModelIndex&)),this,SLOT(tryToOpenFile(const QModelIndex&)));
  connect(ui->ClearButton,SIGNAL(clicked()),this,SLOT(clearOutputDir()));
  connect(ui->ExploreButton,SIGNAL(clicked()),this,SLOT(tryToExploreOutputDir()));

}


// =====================================================================
// =====================================================================


OutputsWidget::~OutputsWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void OutputsWidget::refreshOutputDir() const
{
  QString NativePath = QDir::toNativeSeparators(QString::fromStdString(openfluid::base::ProjectManager::instance()->getOutputDir()));

  ui->OutputDirLabel->setText(NativePath);
  ui->OutputDirLabel->setToolTip(NativePath);
  ui->OutputDirView->setRootIndex(mp_FSModel->setRootPath(QDir::fromNativeSeparators(NativePath)));
}


// =====================================================================
// =====================================================================


bool OutputsWidget::removeDirectory(QDir Dir)
{
  if (!Dir.exists())
    return false;

  foreach (QFileInfo FI, Dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System))
  {
    if ( FI.isSymLink() || FI.isFile() )
    {
      if (!Dir.remove(FI.absoluteFilePath()))
        return false;
    }
    else if (FI.isDir())
    {
      if (!removeDirectory(FI.filePath()))
        return false;
    }
  }

  if (!Dir.rmdir(Dir.absolutePath()))
    return false;

  return true;
}


// =====================================================================
// =====================================================================


void OutputsWidget::clearOutputDir()
{
  if (QMessageBox::warning(this,tr("Delete outputs contents"),
                           tr("This will delete all files and directories in the output directory.\n\nProceed anyway?"),
                           QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    QDir OutDir(QString::fromStdString(openfluid::base::ProjectManager::instance()->getOutputDir()));

    foreach (QFileInfo FI, OutDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System))
    {
      if (FI.isSymLink() || FI.isFile())
      {
        OutDir.remove(FI.absoluteFilePath());
      }
      else if (FI.isDir())
      {
        removeDirectory(FI.filePath());
      }
    }
  }
}


// =====================================================================
// =====================================================================


void OutputsWidget::tryToOpenFile(const QModelIndex& Index)
{
  QString SelectedFile = mp_FSModel->fileInfo(Index).absoluteFilePath();

  if (!SelectedFile.isEmpty())
  {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QDesktopServices::openUrl(QUrl::fromLocalFile(SelectedFile));
    QApplication::restoreOverrideCursor();
  }

}


// =====================================================================
// =====================================================================


void OutputsWidget::tryToExploreOutputDir()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  QDesktopServices::openUrl(QUrl::fromLocalFile(QString(openfluid::base::ProjectManager::instance()->getOutputDir().c_str())));
  QApplication::restoreOverrideCursor();
}
