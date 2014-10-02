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
  \file ProjectModule.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __PROJECTMODULE_HPP__
#define __PROJECTMODULE_HPP__


#include "AbstractModule.hpp"

#include <openfluid/builderext/FluidXUpdateFlags.hpp>

#include <QString>
#include <QDockWidget>
#include <QFileSystemWatcher>
#include <QTimer>


class ProjectCentral;

class DashboardFrame;

class ProjectWidget;
class ModelWidget;
class SpatialDomainWidget;
class MonitoringWidget;
class DatastoreWidget;
class RunConfigurationWidget;
class OutputsWidget;


namespace openfluid { namespace builderext {
  class PluggableModelessExtension;
} }


class ProjectModule : public AbstractModule
{
  Q_OBJECT;

  private:

    ProjectWidget* mp_MainWidget;

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

    void updateWaresWatchersPaths();

    void doCheck();

    void disableInputDirWatcher();

    void resetInputDirWatcher();


  signals:

    void fluidxChanged(openfluid::builderext::FluidXUpdateFlags::Flags UpdateFlags);

    void savePerformed();

    void simulationStarted();

    void simulationFinished();

    void runEnabled(bool Enabled);

    void refreshWaresEnabled(bool Enabled);


  private slots:

    void releaseModelessExtension(openfluid::builderext::PluggableModelessExtension* Sender = NULL);

    void dispatchChanges(openfluid::builderext::FluidXUpdateFlags::Flags UpdateFlags);

    void dispatchChangesFromExtension(openfluid::builderext::FluidXUpdateFlags::Flags UpdateFlags);

    void updateSimulatorsWares();

    void updateObserversWares();

    void checkInputDir();


  public:

    ProjectModule(const QString& ProjectPath);

    ~ProjectModule();

    QWidget* getMainWidget(QWidget* Parent);

    QWidget* getDockWidget(QWidget* Parent);

    bool whenQuitAsked();

    bool whenNewAsked();

    bool whenOpenAsked();

    bool whenReloadAsked();

    void whenSaveAsked();

    bool whenSaveAsAsked();

    void whenPropertiesAsked();

    bool whenCloseAsked();

    void whenPreferencesAsked();

    void whenRecentProjectsActionsChanged();

    void whenRunAsked();

    void whenExtensionAsked(const QString& ID);

    void whenMarketAsked();

    void whenWaresRefreshAsked();

    bool whenOpenExampleAsked();

    bool isOkForSimulation() const;

};


#endif /* __PROJECTMODULE_HPP__ */
