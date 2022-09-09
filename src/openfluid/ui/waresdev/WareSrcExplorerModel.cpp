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
 @file WareSrcExplorerModel.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <QFont>
#include <QPainter>

#include <openfluid/ui/waresdev/WareSrcUIContainer.hpp>
#include <openfluid/ui/waresdev/WareSrcFiletypeManager.hpp>
#include <openfluid/ui/waresdev/WareSrcExplorerModel.hpp>
#include <openfluid/utilsq/GitProxy.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcExplorerModel::WareSrcExplorerModel(const QString& Path) :
    QFileSystemModel()
{
  m_UserIcons = openfluid::ui::waresdev::WareSrcFiletypeManager::instance()->getIconsByFileExtensionList();

  connect(this, SIGNAL(directoryLoaded(const QString&)), this, SLOT(onDirectoryLoaded(const QString&)));

  connect(&m_Watcher, SIGNAL(fileChanged(const QString&)), this, SLOT(onGitIndexFileChanged(const QString&)));
  connect(&m_Watcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(onGitDirObjectsChanged(const QString&)));

  setRootPath(Path);
}


// =====================================================================
// =====================================================================


void WareSrcExplorerModel::onDirectoryLoaded(const QString& Path)
{
  QModelIndex Parent = index(Path);
  auto Info = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(Path.toStdString());


  // fetching path info of direct children
  for (int Row = 0; Row < rowCount(Parent); ++Row)
  {
    QString ChildPath = filePath(index(Row, 0, Parent));

    m_PathInfos[ChildPath] = openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(ChildPath.toStdString());
  }


  // fetching git info
  if (Path == rootPath() && openfluid::utils::GitProxy::isAvailable())
  {
    // we get branch name for direct children that are tracked wares
    // (we have to do it here because onDirectoryLoaded is not called if the ware is not expanded)
    openfluid::utils::GitProxy Git;
    for (const auto& ChildPathInfo : m_PathInfos)
    {
      if (ChildPathInfo.IsWareDirectory)
      {
        QString WarePath = QString::fromStdString(ChildPathInfo.AbsolutePath);

        openfluid::utils::GitProxy::TreeStatusInfo TreeStatus = Git.status(WarePath);

        if (TreeStatus.m_IsGitTracked)
        {
          m_GitBranchByWarePath[WarePath] = TreeStatus.m_BranchName;

          // we add the ware dir to the watcher right now, because in case of a cloned ware,
          // the ware directory is created with a temporary branch name, so display will have to be updated
          QString DirToWatch = QString("%1/.git/objects").arg(WarePath);
          if (QFile(DirToWatch).exists() && !m_Watcher.directories().contains(DirToWatch))
          {
            m_Watcher.addPath(DirToWatch);
          }
        }
      }
    }
  }
  else if (Info.IsWareDirectory)
  {
    updateGitStatusInfo(Path);
  }
  else
  {
    for (const auto& WarePathsTracked : m_GitBranchByWarePath.keys())
    {
      // if Path is a sub dir of a tracked ware, we reload git status on this ware
      if (Path.startsWith(WarePathsTracked) && Path != WarePathsTracked)
      {
        updateGitStatusInfo(QString::fromStdString(Info.AbsoluteWarePath));
      }
    }
  }

}


// =====================================================================
// =====================================================================


void WareSrcExplorerModel::updateGitStatusInfo(const QString& WarePath)
{
  if (!openfluid::utils::GitProxy::isAvailable())
  {
    return;
  }

  openfluid::utils::GitProxy Git;
  openfluid::utils::GitProxy::TreeStatusInfo TreeStatus = Git.status(WarePath);

  if (TreeStatus.m_IsGitTracked)
  {
    // cleaning git info

    QMap<QString, QString>::iterator it = m_GitIconByWareFilePath.begin();
    while (it != m_GitIconByWareFilePath.end())
    {
      if (it.key().startsWith(WarePath))
      {
        it = m_GitIconByWareFilePath.erase(it);
      }
      else
      {
        ++it;
      }
    }

    QStringList::iterator it2 = m_GitDirties.begin();
    while (it2 != m_GitDirties.end())
    {
      if (it2->startsWith(WarePath))
      {
        it2 = m_GitDirties.erase(it2);
      }
      else
      {
        ++it2;
      }
    }

    it = m_GitBranchByWarePath.begin();
    while (it != m_GitBranchByWarePath.end())
    {
      if (it.key().startsWith(WarePath))
      {
        it = m_GitBranchByWarePath.erase(it);
      }
      else
      {
        ++it;
      }
    }


    // watching git changes

    QString FileToWatch = QString("%1/.git/index").arg(WarePath);
    if (QFile(FileToWatch).exists() && !m_Watcher.files().contains(FileToWatch))
    {
      m_Watcher.addPath(FileToWatch);
    }

    QString DirToWatch = QString("%1/.git/objects").arg(WarePath);
    if (QFile(DirToWatch).exists() && !m_Watcher.directories().contains(DirToWatch))
    {
      m_Watcher.addPath(DirToWatch);
    }


    m_GitBranchByWarePath[WarePath] = TreeStatus.m_BranchName;


    // setting default with TRACKED

    applyIconRecursively(WarePath, m_IconByGitStatus[openfluid::utils::GitProxy::FileStatus::TRACKED]);


    // parsing status outputs

    QMap<QString, openfluid::utils::GitProxy::FileStatusInfo> FileStatus = TreeStatus.m_FileStatusByTreePath;
    for (QMap<QString, openfluid::utils::GitProxy::FileStatusInfo>::iterator it = FileStatus.begin();
         it != FileStatus.end(); ++it)
    {
      QString AbsolutePath = QString("%1/%2").arg(WarePath).arg(it.key());

      if (it.value().m_IsDirty)
      {
        m_GitDirties << AbsolutePath;
      }

      m_GitIconByWareFilePath[AbsolutePath] = m_IconByGitStatus[it.value().m_IndexStatus];

      if (AbsolutePath.endsWith("/"))
      {
        AbsolutePath.chop(1);

        applyIconRecursively(AbsolutePath, m_IconByGitStatus[openfluid::utils::GitProxy::FileStatus::UNTRACKED]);
      }
    }
  }

  emit dataChanged(index(0, 0), index(rowCount(), 0));
}


// =====================================================================
// =====================================================================


void WareSrcExplorerModel::applyIconRecursively(const QString& CurrDir, const QString& IconPath)
{
  QDir Dir(CurrDir);

  if (Dir.dirName().startsWith("_build") || Dir.dirName() == ".git")
  {
    return;
  }

  for (const auto& SubFile : Dir.entryList(QDir::Files))
  {
    m_GitIconByWareFilePath[QString("%1/%2").arg(CurrDir).arg(SubFile)] = IconPath;
  }  

  for (const auto& SubDir : Dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
  {
    applyIconRecursively(QString("%1/%2").arg(CurrDir).arg(SubDir), IconPath);
  }  
}


// =====================================================================
// =====================================================================


QVariant WareSrcExplorerModel::data(const QModelIndex& Index, int Role) const
  {
  if (!Index.isValid())
  {
    return QFileSystemModel::data(Index, Role);
  }

  if (Role == Qt::ForegroundRole && fileName(Index).startsWith("_build"))
  {
    return QVariant(QColor("#A3A3A3"));
  }

  if (Role == Qt::FontRole && !isDir(Index) && fileInfo(Index).suffix() == "cpp")
  {
    auto PInfo = m_PathInfos.value(filePath(Index));

    auto Container = WareSrcUIContainer(QString::fromStdString(PInfo.AbsoluteWarePath),
                                        PInfo.WareType,
                                        QString::fromStdString(PInfo.WareDirName));

    if (Container.getMainCppPath() == filePath(Index).toStdString())
    {
      QFont Font;
      Font.setBold(true);
      return Font;
    }
  }

  if (Role == Qt::DisplayRole && m_GitDirties.contains(filePath(Index)))
  {
    return QVariant(QString("> %1").arg(QFileSystemModel::data(Index, Role).toString()));
  }

  if (Role == Qt::DisplayRole && m_GitBranchByWarePath.contains(filePath(Index)))
  {
    return QVariant(QString("%1  [%2]").arg(QFileSystemModel::data(Index, Role).toString())
                                       .arg(m_GitBranchByWarePath[filePath(Index)]));
  }

  if (Role == QFileSystemModel::FileIconRole)
  {
    if (m_PathInfos.value(filePath(Index)).IsWareDirectory)
    {
      if (m_GitBranchByWarePath.contains(filePath(Index)))
      {
        QPixmap Base(":/ui/common/filetypes/waredir.png");
        QPixmap Overlay(":/ui/common/emblems/git_linked.png");

        QPainter painter(&Base);
        painter.drawPixmap(Base.width()*0.4, Base.height()*0.4, Base.width()*0.6, Base.height()*0.6, Overlay);
        return QIcon(Base);
      }

      return QIcon(":/ui/common/filetypes/waredir.png");
    }

    if (!isDir(Index))
    {
      QString FilePath = filePath(Index);
      QString FileName = QFileInfo(FilePath).fileName();
      QString BaseIconPath = ":/ui/common/filetypes/notype.png";

      for (QMap<QString, QString>::const_iterator it = m_UserIcons.begin(); it != m_UserIcons.end(); ++it)
      {
        if (QDir::match(it.key(), FileName))
        {
          BaseIconPath = it.value();
          break;
        }
      }

      QString DecoratorPath = m_GitIconByWareFilePath.value(FilePath, "");
      if (!DecoratorPath.isEmpty())
      {
        QPixmap Base(BaseIconPath);
        QPixmap Overlay(DecoratorPath);

        QPainter painter(&Base);
        painter.drawPixmap(Base.width() / 2, Base.height() / 2, Base.width() / 2, Base.height() / 2, Overlay);
        return QIcon(Base);
      }

      return QIcon(BaseIconPath);
    }
  }

  return QFileSystemModel::data(Index, Role);
}


// =====================================================================
// =====================================================================


void WareSrcExplorerModel::onGitIndexFileChanged(const QString& Path)
{
  QString WarePath = Path;
  WarePath.chop(11); // removing "/.git/index" at the end

  updateGitStatusInfo(WarePath);
}


// =====================================================================
// =====================================================================


void WareSrcExplorerModel::onGitDirObjectsChanged(const QString& Path)
{
  QString WarePath = Path;
  WarePath.chop(13); // removing "/.git/objects" at the end

  updateGitStatusInfo(WarePath);
}


// =====================================================================
// =====================================================================


} } } // namespaces
