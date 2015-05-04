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
 @file WareSrcExplorer.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/ui/waresdev/WareSrcExplorer.hpp>

#include <QAction>
#include <QMenu>
#include <QMessageBox>

#include <openfluid/ui/waresdev/WareSrcExplorerModel.hpp>
#include <openfluid/ui/waresdev/NewSrcFileAssistant.hpp>
#include <openfluid/ui/waresdev/WareExplorerDialog.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcExplorer::WareSrcExplorer(QWidget* Parent) :
    QTreeView(Parent), mp_Model(0)
{
  connect(this, SIGNAL(activated(const QModelIndex&)), this, SLOT(onDoubleClicked(const QModelIndex&)));

  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onClicked(const QModelIndex&)));
}


// =====================================================================
// =====================================================================


WareSrcExplorer::~WareSrcExplorer()
{

}


// =====================================================================
// =====================================================================


void WareSrcExplorer::configure(const QString& TopDirectoryPath, bool WithContextMenu)
{
  m_TopDirectoryPath = TopDirectoryPath;

  mp_Model = new WareSrcExplorerModel(m_TopDirectoryPath);

  setModel(mp_Model);

  hideColumn(1);
  hideColumn(2);
  hideColumn(3);

  setRootIndex(mp_Model->index(m_TopDirectoryPath));

  connect(selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this,
          SLOT(onCurrentChanged(const QModelIndex&)));
  connect(mp_Model, SIGNAL(directoryLoaded(const QString&)), this, SLOT(scrollToCurrent()));

  if (WithContextMenu)
  {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(onCustomContextMenuRequested(const QPoint&)));
  }
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onCustomContextMenuRequested(const QPoint& Point)
{
  QMenu Menu;

  Menu.addAction(tr("New file"), this, SLOT(onNewFileAsked()));
  Menu.addAction(tr("New folder"), this, SLOT(onNewFolderAsked()));

  Menu.addSeparator();

  Menu.addAction(tr("Delete ware"), this, SIGNAL(deleteWareAsked()));
  QAction* DeleteFileAction = Menu.addAction(tr("Delete file"), this, SLOT(onDeleteFileAsked()));
  if (currentIndex().isValid() && mp_Model->isDir(currentIndex()))
    DeleteFileAction->setEnabled(false);

  Menu.addSeparator();

  Menu.addAction(tr("Open a terminal"), this, SLOT(onOpenTerminalAsked()));
  Menu.addAction(tr("Open a file explorer"), this, SLOT(onOpenExplorerAsked()));

  Menu.exec(viewport()->mapToGlobal(Point));
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onDoubleClicked(const QModelIndex& Index)
{
  emit doubleClicked(mp_Model->filePath(Index));
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onClicked(const QModelIndex& Index)
{
  emit clicked(mp_Model->filePath(Index));
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onCurrentChanged(const QModelIndex& Current)
{
  emit currentChanged(mp_Model->filePath(Current));
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onOpenExplorerAsked()
{
  emit openExplorerAsked(getCurrentDir());
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onOpenTerminalAsked()
{
  emit openTerminalAsked(getCurrentDir());
}


// =====================================================================
// =====================================================================


QString WareSrcExplorer::getCurrentDir()
{
  if (currentIndex().isValid())
  {
    if (mp_Model->isDir(currentIndex()))
      return mp_Model->filePath(currentIndex());

    return mp_Model->fileInfo(currentIndex()).absolutePath();
  }

  return "";
}


// =====================================================================
// =====================================================================


QString WareSrcExplorer::getCurrentPath()
{
  if (currentIndex().isValid())
    return mp_Model->filePath(currentIndex());

  return "";
}


// =====================================================================
// =====================================================================

bool WareSrcExplorer::setCurrentPath(const QString& Path)
{
  if (Path.contains(m_TopDirectoryPath))
  {
    setCurrentIndex(mp_Model->index(Path));
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::scrollToCurrent()
{
  scrollTo(currentIndex());
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::emitDataChanged()
{
  mp_Model->emitDataChanged();
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onNewFileAsked()
{
  if (!currentIndex().isValid())
    return;

  openfluid::waresdev::WareSrcManager::PathInfo PInfo = openfluid::waresdev::WareSrcManager::instance()->getPathInfo(
      mp_Model->filePath(currentIndex()));

  NewSrcFileAssistant Assistant(
      openfluid::waresdev::WareSrcContainer(PInfo.m_AbsolutePathOfWare, PInfo.m_WareType, PInfo.m_WareName), this);
  Assistant.exec();

  QString NewFilePath = Assistant.getNewFilePath();

  if (!NewFilePath.isEmpty())
    emit openPathAsked(NewFilePath);
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onNewFolderAsked()
{
  if (!currentIndex().isValid())
    return;

  QString CurrentPath = mp_Model->filePath(currentIndex());

  QString WarePath = openfluid::waresdev::WareSrcManager::instance()->getPathInfo(CurrentPath).m_AbsolutePathOfWare;

  QString NewPath = WareExplorerDialog::getCreateFolderPath(this, WarePath, CurrentPath);

  if (!NewPath.isEmpty())
  {
    QDir(WarePath).mkdir(NewPath);
    setCurrentPath(NewPath);
  }
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onDeleteFileAsked()
{
  if (!currentIndex().isValid())
    return;

  QString CurrentPath = mp_Model->filePath(currentIndex());

  if (QMessageBox::warning(this, tr("Delete file"), tr("Are you sure you want to delete \"%1\"?").arg(CurrentPath),
                           QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel)
      == QMessageBox::Cancel)
    return;

  if (QDir().remove(CurrentPath))
    emit fileDeleted(CurrentPath);
  else
    QMessageBox::critical(0, tr("Error"), tr("Unable to remove the file \"%1\"").arg(CurrentPath));
}


// =====================================================================
// =====================================================================


} } }  // namespaces
