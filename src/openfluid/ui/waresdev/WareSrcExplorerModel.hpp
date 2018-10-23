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
 @file WareSrcExplorerModel.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_UIWARESDEV_WARESRCEXPLORERMODEL_HPP__
#define __OPENFLUID_UIWARESDEV_WARESRCEXPLORERMODEL_HPP__


#include <QFileSystemModel>
#include <QFileSystemWatcher>

#include <openfluid/dllexport.hpp>
#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/utils/GitProxy.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API WareSrcExplorerModel: public QFileSystemModel
{
  Q_OBJECT


  private slots:

    void onGitIndexFileChanged(const QString& Path);

    void onGitDirObjectsChanged(const QString& Path);


  private:

    // set here to speed up display
    QMap<QString, QString> m_UserIcons;

    openfluid::waresdev::WareSrcManager* mp_Manager;

    /*
     * Map of PathInfo for each FilePath
     * Stored to avoid PathInfo object creation at each data() call.
     * Instead they are only created at each directoryLoaded() signal
     */
    QMap<QString, openfluid::waresdev::WareSrcManager::PathInfo> m_PathInfos;

    std::map<openfluid::utils::GitProxy::FileStatus, QString> m_IconByGitStatus =
        {
          { openfluid::utils::GitProxy::FileStatus::TRACKED, ":/ui/common/emblems/git_tracked.png" },
          { openfluid::utils::GitProxy::FileStatus::UNTRACKED, ":/ui/common/emblems/git_untracked.png" },
          { openfluid::utils::GitProxy::FileStatus::IGNORED, "" },
          { openfluid::utils::GitProxy::FileStatus::CONFLICT, ":/ui/common/emblems/git_conflict.png" },
          { openfluid::utils::GitProxy::FileStatus::ADDED, ":/ui/common/emblems/git_added.png" },
          { openfluid::utils::GitProxy::FileStatus::DELETED, "" },
          { openfluid::utils::GitProxy::FileStatus::MODIFIED, ":/ui/common/emblems/git_staged.png" }
        };

    QMap<QString, QString> m_GitBranchByWarePath;

    QMap<QString, QString> m_GitIconByWareFilePath;

    QList<QString> m_GitDirties;

    QFileSystemWatcher m_Watcher;

    void applyIconRecursively(const QString& CurrDir, const QString& IconPath);


  public slots:

    void onDirectoryLoaded(const QString& Path);


  public:

    explicit WareSrcExplorerModel(const QString& Path);

    QVariant data(const QModelIndex& Index, int Role) const;

    void updateGitStatusInfo(const QString& WarePath);

};


} } } // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESRCEXPLORERMODEL_HPP__ */
