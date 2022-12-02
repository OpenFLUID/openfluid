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

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QMessageBox>
#include <QGuiApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QInputDialog>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/ui/waresdev/WareSrcExplorer.hpp>
#include <openfluid/ui/waresdev/WareSrcExplorerModel.hpp>
#include <openfluid/ui/waresdev/WareExplorerDialog.hpp>
#include <openfluid/ui/waresdev/WareGitDialog.hpp>
#include <openfluid/ui/common/DefaultAction.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/waresdev/FragmentsSrcImportDialog.hpp>


namespace openfluid { namespace ui { namespace waresdev {


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
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            SLOT(onCustomContextMenuRequested(const QPoint&)));
  }
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onCustomContextMenuRequested(const QPoint& Point)
{
  QMenu Menu;

  Menu.addAction(tr("New file"), this, SLOT(onNewFileAsked()));
  Menu.addAction(tr("New folder"), this, SLOT(onNewFolderAsked()));
  if (currentIndex().isValid() && mp_Model->isFragment(currentIndex()))
  {
    Menu.addAction(tr("Remove this fragment"), this, SLOT(onFragmentRemovalAsked()));
  }
  else
  {
    Menu.addAction(tr("Add fragment"), this, SLOT(onNewFragmentAsked()));
  }

  Menu.addSeparator();

  Menu.addAction(tr("Delete ware"), this, SIGNAL(deleteWareAsked()));
  QAction* DeleteFileAction = Menu.addAction(tr("Delete file"), this, SLOT(onDeleteFileAsked()));
  if (currentIndex().isValid() && mp_Model->isDir(currentIndex()))
  {
    DeleteFileAction->setEnabled(false);
  }

  Menu.addSeparator();

  Menu.addAction(tr("Open a terminal"), this, SLOT(onOpenTerminalAsked()));
  Menu.addAction(tr("Open a file explorer"), this, SLOT(onOpenExplorerAsked()));

  // Custom tools menu
  QMenu ExtToolMenu;
  ExtToolMenu.setTitle("Open in external tool");
  ExtToolMenu.setEnabled(false);

  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();
  std::list<openfluid::base::PreferencesManager::ExternalTool_t> ExternalTools; 
  openfluid::base::PreferencesManager::ExternalToolContext ExternalToolsCtxt;

  if (currentIndex().isValid() && mp_Model->isDir(currentIndex()))
  {
    ExternalToolsCtxt = openfluid::base::PreferencesManager::ExternalToolContext::WARE;
  }
  else
  {
    ExternalToolsCtxt = openfluid::base::PreferencesManager::ExternalToolContext::FILE;
  }
  
  ExternalTools = PrefMgr->getWaresdevExternalToolsInContext(ExternalToolsCtxt);

  QMap<QString, QAction*> ExternalToolsActions;
  for (auto const& Tool : ExternalTools)
  {
    QString ToolName = QString::fromStdString(Tool.Name);
    ExtToolMenu.setEnabled(true);
    ExternalToolsActions[ToolName] = new openfluid::ui::common::DefaultAction(ToolName, this);
    ExternalToolsActions[ToolName]->setData(QString::fromStdString(Tool.getCommand(ExternalToolsCtxt)));
    ExtToolMenu.addAction(ExternalToolsActions[ToolName]);
    connect(ExternalToolsActions[ToolName], SIGNAL(triggered()), this, SLOT(onOpenExternalToolAsked()));
  }
  Menu.addMenu(&ExtToolMenu);

  Menu.addSeparator();

  Menu.addAction(tr("Copy full path"), this, SLOT(onCopyFullPathAsked()));
  Menu.addAction(tr("Copy relative path"), this, SLOT(onCopyRelativePathAsked()));

  Menu.addSeparator();

  QMenu GitMenu;
  GitMenu.setTitle("Git");

  QAction* InitAction = new QAction("Init", this);
  QAction* StatusAction = new QAction("Status", this);
  QAction* LogAction = new QAction("Log", this);
  GitMenu.addAction(InitAction);
  GitMenu.addAction(StatusAction);
  GitMenu.addAction(LogAction);
  connect(InitAction, SIGNAL(triggered()), this, SLOT(onGitInitAsked()));
  connect(StatusAction, SIGNAL(triggered()), this, SLOT(onGitStatusAsked()));
  connect(LogAction, SIGNAL(triggered()), this, SLOT(onGitLogAsked()));
  
  Menu.addMenu(&GitMenu);

  if (currentIndex().data(Qt::DisplayRole).toString().contains("[") && openfluid::utils::GitProxy::isAvailable())
  {
    InitAction->setEnabled(false);
  }
  else
  {
    StatusAction->setEnabled(false);
    LogAction->setEnabled(false);
  }

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


void WareSrcExplorer::onOpenExternalToolAsked()
{
  QAction* Sender = (QAction*)(QObject::sender());
  if (Sender != nullptr)
  {
    emit openExternalToolAsked(Sender->data().toString(), getCurrentPath());
  }
  
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onCopyFullPathAsked()
{
  QClipboard *Clip = QGuiApplication::clipboard();
  Clip->setText(getCurrentPath());
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onCopyRelativePathAsked()
{  
  QClipboard *Clip = QGuiApplication::clipboard();
  Clip->setText(QDir(m_TopDirectoryPath).relativeFilePath(getCurrentPath()));
}


// =====================================================================
// =====================================================================


QString WareSrcExplorer::getCurrentDir()
{
  if (currentIndex().isValid())
  {
    if (mp_Model->isDir(currentIndex()))
    {
      return mp_Model->filePath(currentIndex());
    }

    return mp_Model->fileInfo(currentIndex()).absolutePath();
  }

  return "";
}


// =====================================================================
// =====================================================================


QString WareSrcExplorer::getCurrentPath()
{
  if (currentIndex().isValid())
  {
    return mp_Model->filePath(currentIndex());
  }

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


void WareSrcExplorer::onNewFileAsked()
{
  if (!currentIndex().isValid())
  {
    return;
  }

  auto PInfo = 
    openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(mp_Model->filePath(currentIndex()).toStdString());

  openfluid::ui::common::createNewFile(this,QString::fromStdString(PInfo.AbsoluteWarePath));
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onNewFolderAsked()
{
  if (!currentIndex().isValid())
  {
    return;
  }

  QString CurrentPath = mp_Model->filePath(currentIndex());

  QString WarePath = 
    QString::fromStdString(
      openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(CurrentPath.toStdString()).AbsoluteWarePath
    );

  QString NewPath = WareExplorerDialog::getCreateFolderPath(this, WarePath, CurrentPath);
  if (!NewPath.isEmpty())
  {
    QDir(WarePath).mkdir(NewPath);
    setCurrentPath(NewPath);
  }
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onNewFragmentAsked()
{
  if (!openfluid::utils::GitProxy::isAvailable())
  {
    QMessageBox::warning(this, tr("Import not available"), tr("Git program can not be found."));
    return;
  }
  if (!currentIndex().isValid())
  {
    return;
  }

  QString CurrentPath = mp_Model->filePath(currentIndex());

  QString WarePath = 
    QString::fromStdString(
      openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(CurrentPath.toStdString()).AbsoluteWarePath
    );

  GitUIProxy Git;
  bool HasWareVersionControl = Git.status(WarePath).m_IsGitTracked;

  openfluid::ui::waresdev::FragmentsSrcImportDialog Dialog(this, WarePath, HasWareVersionControl);
  Dialog.exec();
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onFragmentRemovalAsked()
{
  //TOIMPL: since have effects on global window, redirect all actions to it through a
  // "fragmentRemovalAsked" signal? (changing action from SLOT to SIGNAL?)
  // (suivre logique de "deleteWareAsked")
  QString CurrentPath = mp_Model->filePath(currentIndex());
  openfluid::ui::waresdev::GitUIProxy Git;

  openfluid::waresdev::WareSrcEnquirer::WarePathInfo WareInfo = 
    openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(CurrentPath.toStdString());

  QString WarePath = 
    QString::fromStdString(
      WareInfo.AbsoluteWarePath
    );

  QString FragmentName = QDir(CurrentPath).dirName();

  QString SubPath = 
    QString::fromStdString(
      WareInfo.RelativePathToWare
    );
  if (QMessageBox::warning(this, tr("Delete fragment"), tr("Are you sure you want to delete the fragment \"%1\"?\n"
        "Any open file of the fragment will be closed and deleted, even unsaved ones.").arg(FragmentName),
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Cancel)
  {
    return;
  }
  
  // TODO add progress bar for delete operations
  if (Git.status(WarePath).m_IsGitTracked && Git.status(CurrentPath).m_IsGitTracked)
  {
    // IF WARE AND FRAGMENT ARE UNDER VERSION CONTROL AND SUBMODULE HAS BEEN USED: Removing through Git proxy
    openfluid::ui::waresdev::WareGitDialog Dialog;
    Dialog.setWindowTitle("Remove fragment submodule");
    std::pair<bool, QString> Result = Git.removeSubmodule(WarePath, SubPath);
    emit folderDeleted(WarePath, CurrentPath, false);
    Dialog.setContent(Result.second);
    Dialog.exec();
  }
  else
  {
    // IF WARE NOT UNDER VERSION CONTROL: Simply removing the folder
    if (QDir(CurrentPath).removeRecursively())
    {
      emit folderDeleted(WarePath, CurrentPath, false);
      // TODO check unsaved files beforehand to allow cancelling fragment removal if wanted
      QMessageBox::information(0, tr("Fragment removal"), tr("Fragment \"%1\" removed.").arg(FragmentName));
    }
    else
    {
      QMessageBox::critical(0, tr("Error"), tr("Unable to remove the fragment \"%1\"").arg(SubPath));
    }
  }

  QDir FragmentsDir(QString::fromStdString(openfluid::tools::FilesystemPath({WareInfo.AbsoluteWarePath, openfluid::config::WARESDEV_SRC_DIR, openfluid::config::WARESDEV_FRAGMENTS_DIR}).toGeneric()));
  if (FragmentsDir.exists() && FragmentsDir.isEmpty())
  {
    FragmentsDir.removeRecursively();
  }
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onDeleteFileAsked()
{
  if (!currentIndex().isValid())
  {
    return;
  }

  QString CurrentPath = mp_Model->filePath(currentIndex());

  if (QMessageBox::warning(this, tr("Delete file"), tr("Are you sure you want to delete \"%1\"?").arg(CurrentPath),
                           QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Cancel)
  {
    return;
  }

  if (QDir().remove(CurrentPath))
  {
    emit fileDeleted(CurrentPath);
  }
  else
  {
    QMessageBox::critical(0, tr("Error"), tr("Unable to remove the file \"%1\"").arg(CurrentPath));
  }
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onGitInitAsked()
{
  GitUIProxy Git;

  QString CurrentPath = mp_Model->filePath(currentIndex());

  QString WarePath = 
    QString::fromStdString(
      openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(CurrentPath.toStdString()).AbsoluteWarePath
    );

  openfluid::ui::waresdev::WareGitDialog Dialog;
  Dialog.setWindowTitle("git init");

  Dialog.setContent(Git.init(WarePath));
  Dialog.exec();

  mp_Model->updateGitStatusInfo(CurrentPath);
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onGitStatusAsked()
{
  GitUIProxy Git;

  openfluid::ui::waresdev::WareGitDialog Dialog;
  Dialog.setWindowTitle("git status");
  Dialog.setContent(Git.statusHtml(mp_Model->filePath(currentIndex()), true));
  Dialog.exec();
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onGitLogAsked()
{
  GitUIProxy Git;

  openfluid::ui::waresdev::WareGitDialog Dialog;
  Dialog.setWindowTitle("git log");
  Dialog.setContent(Git.logHtml(mp_Model->filePath(currentIndex()), true));
  Dialog.exec();
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::updateExplorerModel(const QString& Path)
{
  mp_Model->updateGitStatusInfo(Path);
}


// =====================================================================
// =====================================================================


} } } // namespaces
