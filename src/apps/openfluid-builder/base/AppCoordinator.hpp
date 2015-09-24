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
  @file AppCoordinator.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_APPCOORDINATOR_HPP__
#define __OPENFLUID_BUILDERAPP_APPCOORDINATOR_HPP__

#include <QObject>
#include <QDockWidget>

#include "NewProjectDialog.hpp"

class MainWindow;
class AppActions;
class AbstractModule;

class AppCoordinator : public QObject
{
  Q_OBJECT

  private:

    MainWindow& m_MainWindow;

    AppActions& m_Actions;

    QDockWidget* mp_DockWidget;

    AbstractModule* mp_CurrentModule;

    void unsetCurrentModule();

    void setCurrentModule(AbstractModule* Module);

    bool setProjectModule(const QString& ProjectPath);

    bool createProject(const QString& Name, const QString& Path, const QString& Description, const QString& Authors,
                       NewProjectDialog::ImportType IType, const QString& ISource);

    void openProject(const QString& Name, const QString& Path);

    bool closeProject();


  private slots:

    void whenQuitAsked();

    void whenNewAsked();

    void whenOpenAsked();

    void whenReloadAsked();

    void whenOpenRecentAsked();

    void whenSaveAsked();

    void whenSaveAsAsked();

    void whenCloseAsked();

    void whenPropertiesAsked();

    void whenPreferencesAsked();

    void whenWaresRefreshAsked();

    void whenRunAsked();

    void whenExtensionAsked();

    void whenViewDashboardAsked();

    void whenViewToolbarAsked();

    void whenViewRestoreAsked();

    void whenMarketAsked();

    void whenOnlineWebAsked();

    void whenOnlineCommunityAsked();

    void whenOpenExampleAsked();

    void whenEmailAsked();

    void whenRestoreExamplesAsked();

    void whenAboutAsked();

    void whenNewSimulatorSrcAsked();

    void whenNewGhostSimulatorAsked();

    void whenOpenSimulatorSrcAsked();

    void whenNewObserverSrcAsked();

    void whenOpenObserverSrcAsked();

    void whenLaunchDevStudioAsked();

    void enableSave();

    void disableSave();

    void saveDockArea(Qt::DockWidgetArea Area);

    void enableRun(bool Enabled);


  public:

    AppCoordinator(MainWindow& MainWin, AppActions& Actions);

    ~AppCoordinator();

    void connectExtensions();

    void setHomeModule();

};


#endif /* __OPENFLUID_BUILDERAPP_APPCOORDINATOR_HPP__ */
