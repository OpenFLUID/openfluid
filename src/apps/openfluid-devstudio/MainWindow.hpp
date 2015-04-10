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
 @file MainWindow.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef __OPENFLUID_DEVSTUDIOAPP_MAINWINDOW_HPP__
#define __OPENFLUID_DEVSTUDIOAPP_MAINWINDOW_HPP__

#include <QMainWindow>
#include <QMap>

#include <openfluid/ui/waresdev/WareSrcWidgetCollection.hpp>
#include <openfluid/ui/waresdev/WareSrcToolbar.hpp>
#include <openfluid/dllexport.hpp>


namespace Ui {
class MainWindow;
}

class QAction;


namespace openfluid { namespace ui { namespace waresdev {
class WareSrcWidget;
} } }


class OPENFLUID_API MainWindow: public QMainWindow
{
  Q_OBJECT

  private:

    Ui::MainWindow* ui;

    openfluid::ui::waresdev::WareSrcToolbar* mp_Toolbar;

    openfluid::ui::waresdev::WareSrcWidgetCollection* mp_Collection;

    QMap<QString, QAction*> m_Actions;

    void createLocalActions();

    void createMenus();

    void setWorkspaceDefaults();

  private slots:

    void showNotYetImplemented();

    void onQuitRequested();

    void onAboutAsked();

    void onOnlineWebAsked();

    void onOnlineCommunityAsked();

    void setCurrentPath(const QString& Path);

  public:

    MainWindow();

    ~MainWindow();

    void closeEvent(QCloseEvent* Event);
};

#endif /* __OPENFLUID_DEVSTUDIOAPP_MAINWINDOW_HPP__ */
