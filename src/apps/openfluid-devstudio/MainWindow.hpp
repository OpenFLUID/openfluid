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

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_DEVSTUDIOAPP_MAINWINDOW_HPP__
#define __OPENFLUID_DEVSTUDIOAPP_MAINWINDOW_HPP__


#include <QMainWindow>
#include <QMap>

#include <openfluid/ui/common/OpenFLUIDSplashScreen.hpp>
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

    openfluid::ui::waresdev::WareSrcWidgetCollection* mp_WidgetsCollection;

    QMap<QString, QAction*> m_Actions;

    QMap<QString, QAction*> m_ExternalToolsActions;

    QList<QString> m_ExternalToolsOrder;

    void createLocalActions();

    void createMenus();

    void setWorkspaceDefaults();


  signals:
    void openExternalToolAsked(const QString& Command, const QString& FilePath);


  private slots:

    void onOpenExternalToolAsked();

    void onBuildOptionsMenuChanged();

    void onBuildOptionsToolbarChanged();

    void onQuitRequested();

    void onPreferencesAsked();

    void onImportWareSourcesAsked();

    void onExportWareSourcesAsked();

    void onDevDashboardAsked();

    void onAboutAsked();

    void onOnlineWebAsked();

    void onOnlineCommunityAsked();

    void setCurrentPath(const QString& Path);

    void updateSaveButtonsStatus(bool FileModified, bool FileOpen, bool WareModified);

    void onDeleteWareRequested();

    void onCloseAllWaresRequested();

    void updateExplorer();


  public:

    MainWindow(openfluid::ui::common::OpenFLUIDSplashScreen* Splash);

    ~MainWindow();

    void closeEvent(QCloseEvent* Event);
};


#endif /* __OPENFLUID_DEVSTUDIOAPP_MAINWINDOW_HPP__ */

