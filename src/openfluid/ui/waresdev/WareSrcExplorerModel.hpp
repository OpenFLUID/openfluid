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

#include <openfluid/waresdev/WareSrcManager.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class WareSrcExplorerModel: public QFileSystemModel
{
  Q_OBJECT

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

  public:

    WareSrcExplorerModel(const QString& Path);

    QVariant data(const QModelIndex& Index, int Role) const;

    void emitDataChanged();

  private slots:

    void onDirectoryLoaded(const QString& Path);
};

} } }  // namespaces

#endif /* __OPENFLUID_UIWARESDEV_WARESRCEXPLORERMODEL_HPP__ */
