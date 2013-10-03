/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file ProjectModule.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <QApplication>
#include <QMessageBox>
#include <QMainWindow>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/guicommon/PreferencesManager.hpp>

#include <openfluid/builderext/PluggableModalExtension.hpp>
#include <openfluid/builderext/PluggableModelessExtension.hpp>
#include <openfluid/builderext/PluggableWorkspaceExtension.hpp>

#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/machine/ObserverSignatureRegistry.hpp>

#include "ProjectCentral.hpp"
#include "ProjectModule.hpp"
#include "PreferencesDialog.hpp"

#include "ExtensionsRegistry.hpp"

#include "DashboardWidget.hpp"

#include "ProjectWidget.hpp"
#include "ModelWidget.hpp"
#include "SpatialDomainWidget.hpp"
#include "MonitoringWidget.hpp"
#include "DatastoreWidget.hpp"
#include "RunConfigurationWidget.hpp"
#include "OutputsWidget.hpp"

#include "AppTools.hpp"

#include <iostream>


// =====================================================================
// =====================================================================


ProjectModule::ProjectModule(const QString& ProjectPath):
AbstractModule(), mp_MainWidget(NULL), mp_DashboardWidget(NULL), m_ProjectPath(ProjectPath), mp_ProjectCentral(NULL)
{
  mp_ProjectCentral = new ProjectCentral(ProjectPath);

  mp_SimulatorsPlugsWatcher= new QFileSystemWatcher(this);
  mp_ObserversPlugsWatcher = new QFileSystemWatcher(this);

  connect(mp_SimulatorsPlugsWatcher,SIGNAL(directoryChanged(const QString&)),this,SLOT(updateSimulatorsWares()));
  connect(mp_ObserversPlugsWatcher,SIGNAL(directoryChanged(const QString&)),this,SLOT(updateObserversWares()));

  updateWatchersPaths();
}


// =====================================================================
// =====================================================================


ProjectModule::~ProjectModule()
{
  delete mp_ProjectCentral;
  mp_SimulatorsPlugsWatcher->deleteLater();
  mp_ObserversPlugsWatcher->deleteLater();
}


// =====================================================================
// =====================================================================


void ProjectModule::updateWatchersPaths()
{
  QStringList Paths;

  // simulators

  if (!mp_SimulatorsPlugsWatcher->directories().isEmpty())
    mp_SimulatorsPlugsWatcher->removePaths(mp_SimulatorsPlugsWatcher->directories());

  if (openfluid::guicommon::PreferencesManager::getInstance()->getWaresWatcher())
  {
    Paths << StringVectorToQStringList(openfluid::base::RuntimeEnvironment::getInstance()->getSimulatorsPluginsPaths())
            << StringVectorToQStringList(openfluid::base::RuntimeEnvironment::getInstance()->getExtraSimulatorsPluginsPaths());

    Paths.removeDuplicates();

    foreach (QString P,Paths)
    {
      if (QDir(P).exists())
        mp_SimulatorsPlugsWatcher->addPath(P);
    }
  }

  // observers

  Paths.clear();

  if (!mp_ObserversPlugsWatcher->directories().isEmpty())
    mp_ObserversPlugsWatcher->removePaths(mp_ObserversPlugsWatcher->directories());

  if (openfluid::guicommon::PreferencesManager::getInstance()->getWaresWatcher())
  {

    Paths << StringVectorToQStringList(openfluid::base::RuntimeEnvironment::getInstance()->getObserversPluginsPaths())
                  << StringVectorToQStringList(openfluid::base::RuntimeEnvironment::getInstance()->getExtraObserversPluginsPaths());

    Paths.removeDuplicates();

    foreach (QString P,Paths)
    {
      if (QDir(P).exists())
        mp_ObserversPlugsWatcher->addPath(P);
    }
  }
}


// =====================================================================
// =====================================================================


QWidget* ProjectModule::getMainWidget(QWidget* Parent)
{
  if (mp_MainWidget != NULL)
  {
    delete mp_MainWidget;
    mp_MainWidget = NULL;
  }

  mp_MainWidget = new ProjectWidget(Parent);


  mp_ModelTab = new ModelWidget(NULL,mp_ProjectCentral->getAdvancedDescriptors());
  connect(this,SIGNAL(modelChanged()),mp_ModelTab,SLOT(refresh()));
  connect(mp_ModelTab,SIGNAL(changed()),this,SLOT(dispatchChanges()));

  mp_SpatialTab = new SpatialDomainWidget(NULL,mp_ProjectCentral->getAdvancedDescriptors());
  connect(this,SIGNAL(spatialChanged()),mp_SpatialTab,SLOT(refresh()));
  connect(mp_SpatialTab,SIGNAL(changed()),this,SLOT(dispatchChanges()));

  mp_DatastoreTab = new DatastoreWidget(NULL,mp_ProjectCentral->getAdvancedDescriptors());
  connect(this,SIGNAL(datastoreChanged()),mp_DatastoreTab,SLOT(refresh()));
  connect(mp_DatastoreTab,SIGNAL(changed()),this,SLOT(dispatchChanges()));

  mp_MonitoringTab = new MonitoringWidget(NULL,mp_ProjectCentral->getAdvancedDescriptors());
  connect(this,SIGNAL(monitoringChanged()),mp_MonitoringTab,SLOT(refresh()));
  connect(mp_MonitoringTab,SIGNAL(changed()),this,SLOT(dispatchChanges()));

  mp_RunConfigTab = new RunConfigurationWidget(NULL,mp_ProjectCentral->getAdvancedDescriptors());
  connect(this,SIGNAL(runconfigChanged()),mp_RunConfigTab,SLOT(refresh()));
  connect(mp_RunConfigTab,SIGNAL(changed()),this,SLOT(dispatchChanges()));

  mp_OutputsTab = new OutputsWidget(NULL,mp_ProjectCentral->getAdvancedDescriptors());


  mp_MainWidget->addWorkspaceTab(mp_ModelTab,tr("Model"));
  mp_MainWidget->addWorkspaceTab(mp_SpatialTab,tr("Spatial domain"));
  mp_MainWidget->addWorkspaceTab(mp_DatastoreTab,tr("Datastore"));
  mp_MainWidget->addWorkspaceTab(mp_MonitoringTab,tr("Monitoring"));
  mp_MainWidget->addWorkspaceTab(mp_RunConfigTab,tr("Simulation configuration"));
  mp_MainWidget->addWorkspaceTab(mp_OutputsTab,tr("Outputs browser"));

  return mp_MainWidget;
}


// =====================================================================
// =====================================================================


QWidget* ProjectModule::getDockWidget(QWidget* Parent)
{
  if (mp_DashboardWidget != NULL)
  {
    delete mp_DashboardWidget;
    mp_DashboardWidget = NULL;
  }

  mp_DashboardWidget = new DashboardWidget(Parent, mp_ProjectCentral);

  return mp_DashboardWidget;
}


// =====================================================================
// =====================================================================


bool ProjectModule::whenQuitAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  return true;
}


// =====================================================================
// =====================================================================


bool ProjectModule::whenNewAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  QMessageBox::critical(QApplication::activeWindow(),QString(__PRETTY_FUNCTION__),QString("not implemented"),QMessageBox::Close);
  return false;
}

// =====================================================================
// =====================================================================


bool ProjectModule::whenOpenAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  QMessageBox::critical(QApplication::activeWindow(),QString(__PRETTY_FUNCTION__),QString("not implemented"),QMessageBox::Close);
  return false;
}


// =====================================================================
// =====================================================================


void ProjectModule::whenSaveAsked()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  if (mp_ProjectCentral->save())
  {
    emit savePerformed();
  }
  QApplication::restoreOverrideCursor();
}


// =====================================================================
// =====================================================================


void ProjectModule::whenSaveAsAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  QMessageBox::critical(QApplication::activeWindow(),QString(__PRETTY_FUNCTION__),QString("not implemented"),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void ProjectModule::whenPropertiesAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  QMessageBox::critical(QApplication::activeWindow(),QString(__PRETTY_FUNCTION__),QString("not implemented"),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


bool ProjectModule::whenCloseAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


void ProjectModule::whenPreferencesAsked()
{
  bool WaresWatchingUpdated = false;

  PreferencesDialog PrefsDlg(QApplication::activeWindow());
  PrefsDlg.exec();

  openfluid::guicommon::PreferencesManager* PrefsMgr =
    openfluid::guicommon::PreferencesManager::getInstance();

  if (PrefsDlg.isSimPathsChanged())
  {
    QStringList ExtraPaths = PrefsMgr->getExtraSimulatorsPaths();

    openfluid::base::RuntimeEnvironment::getInstance()->resetExtraSimulatorsPluginsPaths();
    for (int i=0;i<ExtraPaths.size(); i++)
      openfluid::base::RuntimeEnvironment::getInstance()->addExtraSimulatorsPluginsPaths(ExtraPaths[i].toStdString());

    updateWatchersPaths();
    WaresWatchingUpdated = true;

    updateSimulatorsWares();
  }

  if (PrefsDlg.isObsPathsChanged())
  {
    QStringList ExtraPaths = PrefsMgr->getExtraObserversPaths();

    openfluid::base::RuntimeEnvironment::getInstance()->resetExtraObserversPluginsPaths();
    for (int i=0;i<ExtraPaths.size(); i++)
      openfluid::base::RuntimeEnvironment::getInstance()->addExtraObserversPluginsPaths(ExtraPaths[i].toStdString());

    updateWatchersPaths();
    WaresWatchingUpdated = true;

    updateObserversWares();
  }

  if (PrefsDlg.isWaresWatchingChanged() && !WaresWatchingUpdated)
  {
    updateWatchersPaths();

    if (PrefsMgr->getWaresWatcher())
    {
      // update wares when re-enabling watching
      updateSimulatorsWares();
      updateObserversWares();
    }
  }
}


// =====================================================================
// =====================================================================


void ProjectModule::whenRunAsked()
{
  emit simulationStarted();
  mp_ProjectCentral->run();
  emit simulationFinished();
}


// =====================================================================
// =====================================================================


void ProjectModule::whenExtensionAsked(const QString& ID)
{
  // TODO provide simulators and observers registries to extensions?
  // for model and monitoring handling for example

  openfluid::ware::WareID_t WareID = ID.toStdString();

  ExtensionsRegistry* ExtReg = ExtensionsRegistry::getInstance();

  if (ExtReg->isExtensionRegistered(WareID))
  {
    if (!ExtReg->isExtensionActive(WareID))
    {
      if (ExtReg->getExtensionType(WareID) == openfluid::builderext::TYPE_MODAL)
      {
        openfluid::builderext::PluggableModalExtension* ExtModal =
            (openfluid::builderext::PluggableModalExtension*)(ExtReg->instanciateExtension(WareID));
        ExtModal->setParent(QApplication::activeWindow(),Qt::Dialog);
        ExtModal->setModal(true);
        // TODO set correct extension configuration
        ExtModal->setConfiguration(openfluid::ware::WareParams_t());
        ExtModal->setFluidXDescriptor(&(mp_ProjectCentral->getAdvancedDescriptors()));
        connect(ExtModal,SIGNAL(fluidxChanged()),this,SLOT(dispatchChangesFromExtension()));
        connect(this,SIGNAL(fluidxChanged()),ExtModal,SLOT(update()));
        connect(this,SIGNAL(simulationStarted()),ExtModal,SLOT(manageSimulationStart()));
        connect(this,SIGNAL(simulationFinished()),ExtModal,SLOT(manageSimulationFinish()));
        ExtModal->update();
        ExtModal->exec();
        ExtReg->releaseExtension(ExtModal);
        ExtModal->deleteLater();
      }
      else if (ExtReg->getExtensionType(WareID) == openfluid::builderext::TYPE_MODELESS)
      {
        openfluid::builderext::PluggableModelessExtension* ExtModeless =
            (openfluid::builderext::PluggableModelessExtension*)(ExtReg->instanciateExtension(WareID));
        ExtModeless->setParent(QApplication::activeWindow(),Qt::Dialog);
        ExtModeless->setModal(false);
        // TODO set correct extension configuration
        ExtModeless->setConfiguration(openfluid::ware::WareParams_t());
        ExtModeless->setFluidXDescriptor(&(mp_ProjectCentral->getAdvancedDescriptors()));
        connect(ExtModeless,SIGNAL(finished(int)),this, SLOT(releaseModelessExtension()));
        connect(ExtModeless,SIGNAL(fluidxChanged()),this,SLOT(dispatchChangesFromExtension()));
        connect(this,SIGNAL(fluidxChanged()),ExtModeless,SLOT(update()));
        connect(this,SIGNAL(simulationStarted()),ExtModeless,SLOT(manageSimulationStart()));
        connect(this,SIGNAL(simulationFinished()),ExtModeless,SLOT(manageSimulationFinish()));
        ExtModeless->update();
        ExtModeless->show();
      }
      else if (ExtReg->getExtensionType(WareID) == openfluid::builderext::TYPE_WORKSPACE)
      {
        openfluid::builderext::PluggableWorkspaceExtension* ExtWork =
            (openfluid::builderext::PluggableWorkspaceExtension*)(ExtReg->instanciateExtension(WareID));
        ExtWork->setProperty("ID",QString::fromStdString(WareID));
        // TODO set correct extension configuration
        ExtWork->setConfiguration(openfluid::ware::WareParams_t());
        ExtWork->setFluidXDescriptor(&(mp_ProjectCentral->getAdvancedDescriptors()));
        connect(ExtWork,SIGNAL(fluidxChanged()),this,SLOT(dispatchChangesFromExtension()));
        connect(this,SIGNAL(fluidxChanged()),ExtWork,SLOT(update()));
        connect(this,SIGNAL(simulationStarted()),ExtWork,SLOT(manageSimulationStart()));
        connect(this,SIGNAL(simulationFinished()),ExtWork,SLOT(manageSimulationFinish()));
        ExtWork->update();
        mp_MainWidget->addWorkspaceExtensionTab(ExtWork,ExtReg->getRegisteredExtensions()->at(WareID)->Signature->MenuText);
      }
      else
      {
        QMessageBox::critical(QApplication::activeWindow(),
                              tr("Extension error"),
                              tr("Unknown extension type.\nExtension cannot be instantiated."),
                              QMessageBox::Close);
      }
    }
    else
    {
      QMessageBox::warning(QApplication::activeWindow(),
                           tr("Extension warning"),
                           tr("Extension is already active."),
                           QMessageBox::Close);

    }
  }
  else
  {
    QMessageBox::critical(QApplication::activeWindow(),
                          tr("Extension error"),
                          tr("Extension is not registered.\nExtension cannot be instantiated."),
                          QMessageBox::Close);
  }
}


// =====================================================================
// =====================================================================


void ProjectModule::whenMarketAsked()
{

}


// =====================================================================
// =====================================================================


void ProjectModule::whenRefreshAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  QMessageBox::critical(QApplication::activeWindow(),QString(__PRETTY_FUNCTION__),QString("not implemented"),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


bool ProjectModule::whenOpenExampleAsked()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  QMessageBox::critical(QApplication::activeWindow(),QString(__PRETTY_FUNCTION__),QString("not implemented"),QMessageBox::Close);
  return false;
}


// =====================================================================
// =====================================================================


void ProjectModule::dispatchChanges()
{
  emit fluidxChanged();
}


// =====================================================================
// =====================================================================


void ProjectModule::dispatchChangesFromExtension()
{
  emit fluidxChanged();
}


// =====================================================================
// =====================================================================


void ProjectModule::releaseModelessExtension()
{
  openfluid::builderext::PluggableModelessExtension* Sender = (openfluid::builderext::PluggableModelessExtension*)(QObject::sender());

  if (Sender)
  {
    ExtensionsRegistry::getInstance()->releaseExtension(Sender);
    Sender->deleteLater();
  }
}


// =====================================================================
// =====================================================================


void ProjectModule::updateSimulatorsWares()
{
  openfluid::machine::SimulatorSignatureRegistry::getInstance()->updatePluggableSignatures();

  mp_ModelTab->updateWares();
}


// =====================================================================
// =====================================================================


void ProjectModule::updateObserversWares()
{
  openfluid::machine::ObserverSignatureRegistry::getInstance()->update();
  mp_MonitoringTab->updateWares();
}

