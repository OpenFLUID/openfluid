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
 \file WareSrcExplorer.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "WareSrcExplorer.hpp"

#include <QAction>

#include "WareSrcExplorerModel.hpp"


WareSrcExplorer::WareSrcExplorer(QWidget* Parent) :
    QTreeView(Parent), mp_Model(0)
{
  connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this,
          SLOT(onDoubleClicked(const QModelIndex&)));

  connect(this, SIGNAL(clicked(const QModelIndex&)), this,
          SLOT(onClicked(const QModelIndex&)));

  QAction* OpenExplorerAction = new QAction("Open a file explorer", this);
  connect(OpenExplorerAction, SIGNAL(triggered()), this, SLOT(onOpenExplorerAsked()));
  addAction(OpenExplorerAction);

  QAction* OpenTerminaAction = new QAction("Open a terminal", this);
  connect(OpenTerminaAction, SIGNAL(triggered()), this, SLOT(onOpenTerminalAsked()));
  addAction(OpenTerminaAction);

  setContextMenuPolicy(Qt::ActionsContextMenu);
}


// =====================================================================
// =====================================================================


WareSrcExplorer::~WareSrcExplorer()
{

}


// =====================================================================
// =====================================================================


void WareSrcExplorer::setType(
    openfluid::waresdev::WareSrcManager::WareType Type)
{
  m_WareType = Type;

  QString Path = openfluid::waresdev::WareSrcManager::instance()
      ->getWareTypePath(m_WareType);

  mp_Model = new WareSrcExplorerModel(Path);

  setModel(mp_Model);

  hideColumn(1);
  hideColumn(2);
  hideColumn(3);

  setRootIndex(mp_Model->index(Path));
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onDoubleClicked(const QModelIndex& Index)
{
  emit openAsked(mp_Model->filePath(Index));
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onClicked(const QModelIndex& Index)
{
  emit setCurrentAsked(mp_Model->filePath(Index));
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

    return mp_Model->fileInfo(currentIndex()).dir().absolutePath();
  }

  return "";
}


// =====================================================================
// =====================================================================


