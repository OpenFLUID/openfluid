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
 \file WareSrcExplorer.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef __OPENFLUID_UIWARESDEV_WARESRCEXPLORER_HPP__
#define __OPENFLUID_UIWARESDEV_WARESRCEXPLORER_HPP__

#include <QTreeView>

#include <openfluid/waresdev/WareSrcManager.hpp>

namespace openfluid { namespace ui { namespace waresdev {


class WareSrcExplorerModel;


class WareSrcExplorer: public QTreeView
{
  Q_OBJECT

  private:

    WareSrcExplorerModel* mp_Model;

    QString getCurrentDir();

  private slots:

    void onDoubleClicked(const QModelIndex& Index);

    void onClicked(const QModelIndex& Index);

    void onCurrentChanged(const QModelIndex& Current);

    void onOpenExplorerAsked();

    void onOpenTerminalAsked();

    void scrollToCurrent();

  public:

    WareSrcExplorer(QWidget* Parent = 0);

    ~WareSrcExplorer();

    void configure(const QString& TopDirectoryPath, bool WithContextMenu);

    void setCurrentPath(const QString& Path);

    QString getCurrentPath();

  protected:

    void showEvent(QShowEvent * event);

  signals:

    void doubleClicked(const QString& FilePath);

    void clicked(const QString& FilePath);

    void currentChanged(const QString& FilePath);

    void openExplorerAsked(const QString& FilePath);

    void openTerminalAsked(const QString& FilePath);
};

} } } // namespaces

#endif /* __OPENFLUID_UIWARESDEV_WARESRCEXPLORER_HPP__ */
