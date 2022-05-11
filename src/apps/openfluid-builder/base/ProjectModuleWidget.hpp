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
  @file ProjectModuleWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_BUILDERAPP_PROJECTMODULEWIDGET_HPP__
#define __OPENFLUID_BUILDERAPP_PROJECTMODULEWIDGET_HPP__


#include <QString>
#include <QDockWidget>
#include <QFileSystemWatcher>
#include <QTimer>

#include <openfluid/builderext/FluidXUpdateFlags.hpp>
#include <openfluid/builderext/PluggableBuilderExtension.hpp>
#include <openfluid/ui/waresdev/WareSrcWidgetCollection.hpp>
#include <openfluid/ui/waresdev/WareSrcWidget.hpp>

#include "ui_ProjectModuleWidget.h"
#include "ProjectModuleWidget.hpp"
#include "AbstractModuleWidget.hpp"
#include "WorkspaceTabWidget.hpp"
#include "ExtensionsRegistry.hpp"


class ProjectCentral;
class DashboardFrame;
class ProjectWidget;
class ModelWidget;
class SpatialDomainWidget;
class MonitoringWidget;
class DatastoreWidget;
class RunConfigurationWidget;
class OutputsWidget;


namespace openfluid {
  namespace builderext {
    class PluggableModelessExtension;
  }
  namespace ware {
    class SimulatorSignature;
  }
  namespace ui {
    namespace waresdev {
      class WareSrcWidgetCollection;
    }
  }
}

namespace Ui
{
  class ProjectModuleWidget;
}


// =====================================================================
// =====================================================================


class ProjectModuleWidget : public AbstractModuleWidget
{
  Q_OBJECT;


  private slots:

    void releaseModelessExtension(openfluid::builderext::PluggableModelessExtension* Sender = nullptr);

    void dispatchChanges(openfluid::builderext::FluidXUpdateFlags::Flags UpdateFlags);

    void dispatchChangesFromExtension(openfluid::builderext::FluidXUpdateFlags::Flags UpdateFlags);

    void updateSimulatorsWares();

    void updateObserversWares();

    void checkInputDir();

    void onBuildLaunched(openfluid::ware::WareType Type, const QString& ID);

    void onBuildFinished(openfluid::ware::WareType Type, const QString& ID);


  private:

    Ui::ProjectModuleWidget* ui;

    WorkspaceTabWidget* mp_WorkspaceTabWidget;

    DashboardFrame* mp_DashboardFrame;

    ModelWidget* mp_ModelTab;

    SpatialDomainWidget* mp_SpatialTab;

    DatastoreWidget* mp_DatastoreTab;

    MonitoringWidget* mp_MonitoringTab;

    RunConfigurationWidget* mp_RunConfigTab;

    OutputsWidget* mp_OutputsTab;

    QString m_ProjectPath;

    ProjectCentral* mp_ProjectCentral;

    QFileSystemWatcher* mp_SimulatorsPlugsWatcher;

    QFileSystemWatcher* mp_ObserversPlugsWatcher;

    QFileSystemWatcher* mp_InputDirWatcher;

    QTimer* mp_SimulatorsPlugsUpdateTimer;

    QTimer* mp_ObserversPlugsUpdateTimer;

    QTimer* mp_InputDirUpdateTimer;

    openfluid::ui::waresdev::WareSrcWidgetCollection* mp_WareSrcCollection;

    unsigned int m_ActiveBuilds = 0;

    void updateWaresWatchersPaths();

    void doCheck();

    void disableInputDirWatcher();

    void resetInputDirWatcher();

    void addWorkspaceTab(QWidget* Tab, const QString& Label);

    void addWorkspaceExtensionTab(QWidget* Tab, const QString& Label);

    void addWorkspaceWareSrcTab(const QString& Path);


  signals:

    void fluidxChanged(openfluid::builderext::FluidXUpdateFlags::Flags UpdateFlags);

    void savePerformed();

    void simulationStarted();

    void simulationFinished();

    void runEnabled(bool Enabled);

    void refreshWaresEnabled(bool Enabled);


  public slots:

    void whenSrcEditAsked(const QString& ID,openfluid::ware::WareType WType,bool Ghost);

    void whenSrcGenerateAsked(const QString& ID);

    void updateWareSrcEditorsSettings();


  public:

    ProjectModuleWidget(const QString& ProjectPath, QWidget* Parent = nullptr);

    ~ProjectModuleWidget();

    QWidget* dockWidgetRebuilt(QWidget* Parent);

    bool whenQuitAsked();

    bool whenNewAsked();

    bool whenOpenAsked();

    bool whenReloadAsked();

    void whenSaveAsked();

    bool whenSaveAsAsked();

    void whenPropertiesAsked();

    void whenExploreAsked();

    bool whenCloseAsked();

    bool whenPreferencesAsked();

    void whenRecentProjectsActionsChanged();

    void whenRunAsked();

    void whenRunModeAsked(ProjectCentral::RunMode Mode);

    void whenExtensionAsked(const QString& ID);

    void whenWaresRefreshAsked();

    bool whenOpenExampleAsked();

    void whenNewSimulatorSrcAsked();

    void whenNewGhostSimulatorAsked();

    void whenOpenSimulatorSrcAsked();

    void whenNewObserverSrcAsked();

    void whenOpenObserverSrcAsked();

    void whenLaunchDevStudioAsked();

    bool isOkForSimulation() const;

};


#endif /* __OPENFLUID_BUILDERAPP_PROJECTMODULEWIDGET_HPP__ */

