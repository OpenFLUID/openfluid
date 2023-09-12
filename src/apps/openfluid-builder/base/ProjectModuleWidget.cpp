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
  @file ProjectModuleWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/ui/builderext/PluggableModalExtension.hpp>
#include <openfluid/ui/builderext/PluggableModelessExtension.hpp>
#include <openfluid/ui/builderext/PluggableWorkspaceExtension.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/machine/ObserverRegistry.hpp>
#include <openfluid/ui/common/EditSignatureDialog.hpp>
#include <openfluid/ui/common/PreferencesDialog.hpp>
#include <openfluid/waresdev/WareSrcEnquirer.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>
#include <openfluid/waresdev/GhostsHelpers.hpp>
#include <openfluid/ui/QtHelpers.hpp>
#include <openfluid/tools/FilesystemPath.hpp>

#include "ProjectModuleWidget.hpp"
#include "AppTools.hpp"
#include "ProjectCentral.hpp"
#include "SaveAsDialog.hpp"
#include "EditProjectPropertiesDialog.hpp"

#include "ExtensionsRegistry.hpp"
#include "WaresTranslationsRegistry.hpp"

#include "DashboardFrame.hpp"

#include "ProjectModuleWidget.hpp"
#include "ModelWidget.hpp"
#include "SpatialDomainWidget.hpp"
#include "MonitoringWidget.hpp"
#include "DatastoreWidget.hpp"
#include "RunConfigurationWidget.hpp"
#include "OutputsWidget.hpp"


constexpr unsigned int BUILDER_WARE_WATCHERS_DELAY = 2000;
constexpr unsigned int BUILDER_INPUTDIR_WATCHERS_DELAY = 2000;


// =====================================================================
// =====================================================================


ProjectModuleWidget::ProjectModuleWidget(const QString& ProjectPath, QWidget* Parent):
  AbstractModuleWidget(Parent),
  ui(new Ui::ProjectModuleWidget), mp_WorkspaceTabWidget(nullptr), mp_DashboardFrame(nullptr),
  m_ProjectPath(ProjectPath), mp_ProjectCentral(nullptr)
{
  ui->setupUi(this);

  mp_WorkspaceTabWidget = new WorkspaceTabWidget(this);
  layout()->addWidget(mp_WorkspaceTabWidget);

  mp_WareSrcCollection = new openfluid::ui::waresdev::WareSrcWidgetCollection(mp_WorkspaceTabWidget, true);

  connect(mp_WareSrcCollection, SIGNAL(buildLaunched(openfluid::ware::WareType, const QString&)),
          this, SLOT(onBuildLaunched(openfluid::ware::WareType, const QString&)));

  connect(mp_WareSrcCollection, SIGNAL(buildFinished(openfluid::ware::WareType, const QString&)),
          this, SLOT(onBuildFinished(openfluid::ware::WareType, const QString&)));

  mp_ProjectCentral = ProjectCentral::initInstance(ProjectPath);

  // watcher for simulators, with delay for ui update using timer
  mp_SimulatorsPlugsWatcher = new QFileSystemWatcher(this);

  mp_SimulatorsPlugsUpdateTimer = new QTimer(this);
  mp_SimulatorsPlugsUpdateTimer->setInterval(BUILDER_WARE_WATCHERS_DELAY);
  mp_SimulatorsPlugsUpdateTimer->setSingleShot(true);

  connect(mp_SimulatorsPlugsWatcher,SIGNAL(directoryChanged(const QString&)),
          mp_SimulatorsPlugsUpdateTimer,SLOT(start()));
  connect(mp_SimulatorsPlugsUpdateTimer,SIGNAL(timeout()),SLOT(updateSimulatorsWares()));

  // watcher for observers, with delay for ui update using timer
  mp_ObserversPlugsWatcher = new QFileSystemWatcher(this);

  mp_ObserversPlugsUpdateTimer = new QTimer(this);
  mp_ObserversPlugsUpdateTimer->setInterval(BUILDER_WARE_WATCHERS_DELAY);
  mp_ObserversPlugsUpdateTimer->setSingleShot(true);

  connect(mp_ObserversPlugsWatcher,SIGNAL(directoryChanged(const QString&)),
          mp_ObserversPlugsUpdateTimer,SLOT(start()));
  connect(mp_ObserversPlugsUpdateTimer,SIGNAL(timeout()),SLOT(updateObserversWares()));

  updateWaresWatchersPaths();

  // watcher for input directory
  mp_InputDirWatcher = new QFileSystemWatcher(this);
  mp_InputDirUpdateTimer = new QTimer(this);
  mp_InputDirUpdateTimer->setInterval(BUILDER_INPUTDIR_WATCHERS_DELAY);
  mp_InputDirUpdateTimer->setSingleShot(true);
  connect(mp_InputDirWatcher,SIGNAL(directoryChanged(const QString&)),mp_InputDirUpdateTimer,SLOT(start()));
  connect(mp_InputDirUpdateTimer,SIGNAL(timeout()),SLOT(checkInputDir()));

  resetInputDirWatcher();

  mp_ModelTab = new ModelWidget(this,mp_ProjectCentral->descriptors());
  connect(mp_ModelTab,SIGNAL(changed(openfluid::ui::builderext::FluidXUpdateFlags::Flags)),
          this,SLOT(dispatchChanges(openfluid::ui::builderext::FluidXUpdateFlags::Flags)));
  connect(mp_ModelTab,SIGNAL(srcEditAsked(const QString&,openfluid::ware::WareType,bool)),
          this,SLOT(whenSrcEditAsked(const QString&,openfluid::ware::WareType,bool)));
  connect(mp_ModelTab,SIGNAL(srcGenerateAsked(const QString&)),this,SLOT(whenSrcGenerateAsked(const QString&)));

  mp_SpatialTab = new SpatialDomainWidget(this,mp_ProjectCentral->descriptors());
  connect(mp_SpatialTab,SIGNAL(changed(openfluid::ui::builderext::FluidXUpdateFlags::Flags)),
          this,SLOT(dispatchChanges(openfluid::ui::builderext::FluidXUpdateFlags::Flags)));

  mp_DatastoreTab = new DatastoreWidget(this,mp_ProjectCentral->descriptors());
  connect(mp_DatastoreTab,SIGNAL(changed(openfluid::ui::builderext::FluidXUpdateFlags::Flags)),
          this,SLOT(dispatchChanges(openfluid::ui::builderext::FluidXUpdateFlags::Flags)));

  mp_MonitoringTab = new MonitoringWidget(this,mp_ProjectCentral->descriptors());
  connect(mp_MonitoringTab,SIGNAL(changed(openfluid::ui::builderext::FluidXUpdateFlags::Flags)),
          this,SLOT(dispatchChanges(openfluid::ui::builderext::FluidXUpdateFlags::Flags)));
  connect(mp_MonitoringTab,SIGNAL(srcEditAsked(const QString&,openfluid::ware::WareType,bool)),
          this,SLOT(whenSrcEditAsked(const QString&,openfluid::ware::WareType,bool)));

  mp_RunConfigTab = new RunConfigurationWidget(this,mp_ProjectCentral->descriptors());
  connect(mp_RunConfigTab,SIGNAL(changed(openfluid::ui::builderext::FluidXUpdateFlags::Flags)),
          this,SLOT(dispatchChanges(openfluid::ui::builderext::FluidXUpdateFlags::Flags)));

  mp_OutputsTab = new OutputsWidget(this,mp_ProjectCentral->descriptors());


  addWorkspaceTab(mp_ModelTab,tr("Model"));
  addWorkspaceTab(mp_SpatialTab,tr("Spatial domain"));
  addWorkspaceTab(mp_DatastoreTab,tr("Datastore"));
  addWorkspaceTab(mp_MonitoringTab,tr("Monitoring"));
  addWorkspaceTab(mp_RunConfigTab,tr("Simulation configuration"));
  addWorkspaceTab(mp_OutputsTab,tr("Outputs browser"));
}


// =====================================================================
// =====================================================================


ProjectModuleWidget::~ProjectModuleWidget()
{
  ExtensionsRegistry::instance()->releaseAllFeatureExtensions();

  ProjectCentral::kill();
  mp_SimulatorsPlugsWatcher->deleteLater();
  mp_ObserversPlugsWatcher->deleteLater();
  mp_InputDirWatcher->deleteLater();

  delete ui;
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::updateWaresWatchersPaths()
{
  QStringList Paths;


  // simulators

  if (!mp_SimulatorsPlugsWatcher->directories().isEmpty())
  {
    mp_SimulatorsPlugsWatcher->removePaths(mp_SimulatorsPlugsWatcher->directories());
  }

  if (openfluid::base::PreferencesManager::instance()->isBuilderWaresWatchersActive())
  {
    Paths << openfluid::ui::toQStringList(openfluid::base::Environment::getSimulatorsDirs())
          << openfluid::ui::toQStringList(openfluid::base::Environment::getExtraSimulatorsDirs());

    Paths.removeDuplicates();

    for (QString P : Paths)
    {
      if (QDir(P).exists())
      {
        mp_SimulatorsPlugsWatcher->addPath(P);
      }
    }
  }


  // observers

  Paths.clear();

  if (!mp_ObserversPlugsWatcher->directories().isEmpty())
  {
    mp_ObserversPlugsWatcher->removePaths(mp_ObserversPlugsWatcher->directories());
  }

  if (openfluid::base::PreferencesManager::instance()->isBuilderWaresWatchersActive())
  {

    Paths << openfluid::ui::toQStringList(openfluid::base::Environment::getObserversDirs())
          << openfluid::ui::toQStringList(openfluid::base::Environment::getExtraObserversDirs());

    Paths.removeDuplicates();

    for (QString P : Paths)
    {
      if (QDir(P).exists())
      {
        mp_ObserversPlugsWatcher->addPath(P);
      }
    }
  }
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::disableInputDirWatcher()
{
  mp_InputDirWatcher->removePaths(mp_InputDirWatcher->directories());
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::resetInputDirWatcher()
{
  mp_InputDirWatcher->addPath(m_ProjectPath+"/IN");
}


// =====================================================================
// =====================================================================


QWidget* ProjectModuleWidget::dockWidgetRebuilt(QWidget* Parent)
{
  if (mp_DashboardFrame != nullptr)
  {
    delete mp_DashboardFrame;
    mp_DashboardFrame = nullptr;
  }

  mp_DashboardFrame = new DashboardFrame(mp_ProjectCentral,Parent);

  return mp_DashboardFrame;
}


// =====================================================================
// =====================================================================


bool ProjectModuleWidget::whenQuitAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


bool ProjectModuleWidget::whenNewAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


bool ProjectModuleWidget::whenOpenAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


bool ProjectModuleWidget::whenReloadAsked()
{
  if (QMessageBox::question(QApplication::activeWindow(),tr("Reload project"),
                              tr("Reloading project from disk will overwrite all unsaved modifications if any.")+
                              "\n\n"+
                              tr("Proceed anyway?"),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    emit savePerformed();

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenSaveAsked()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  disableInputDirWatcher();
  if (mp_ProjectCentral->save())
  {
    emit savePerformed();
  }
  resetInputDirWatcher();
  QApplication::restoreOverrideCursor();
}


// =====================================================================
// =====================================================================


bool ProjectModuleWidget::whenSaveAsAsked()
{
  SaveAsDialog SaveAsDlg(QApplication::activeWindow());

  if (SaveAsDlg.exec() == QDialog::Accepted)
  {
    disableInputDirWatcher();
    if (mp_ProjectCentral->saveAs(SaveAsDlg.getProjectName(),
                                  SaveAsDlg.getProjectFullPath()))
    {
      m_ProjectPath = SaveAsDlg.getProjectFullPath();
      mp_DashboardFrame->refreshProjectInfos();
      mp_OutputsTab->refreshOutputDir();
      resetInputDirWatcher();
      emit savePerformed();
      return true;
    }
    resetInputDirWatcher();
  }
  return false;
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenPropertiesAsked()
{
  EditProjectPropertiesDialog EditDlg(QApplication::activeWindow());

  if (EditDlg.exec() == QDialog::Accepted)
  {
    openfluid::base::RunContextManager::instance()->setProjectDescription(EditDlg.getDescription().toStdString());
    openfluid::base::RunContextManager::instance()->setProjectAuthors(EditDlg.getAuthors().toStdString());
    openfluid::base::RunContextManager::instance()->saveProject();
  }

}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenExploreAsked()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  QDesktopServices::openUrl(QUrl::fromLocalFile(
      QString(openfluid::base::RunContextManager::instance()->getProjectPath().c_str())));
  QApplication::restoreOverrideCursor();
}


// =====================================================================
// =====================================================================


bool ProjectModuleWidget::whenCloseAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


bool ProjectModuleWidget::whenPreferencesAsked()
{
  bool WaresWatchingUpdated = false;

  openfluid::ui::common::PreferencesDialog PrefsDlg(QApplication::activeWindow(),
    openfluid::ui::common::PreferencesDialog::DisplayMode::BUILDER);

  connect(&PrefsDlg, SIGNAL(applyTextEditorSettingsAsked()), this, SLOT(updateWareSrcEditorsSettings()));

  PrefsDlg.exec();


  openfluid::base::PreferencesManager* PrefsMgr =
    openfluid::base::PreferencesManager::instance();

  if (PrefsDlg.isSimPathsChanged())
  {
    auto ExtraPaths = PrefsMgr->getBuilderExtraSimulatorsPaths();

    openfluid::base::Environment::resetExtraSimulatorsDirs();
    for (const auto& P : ExtraPaths)
    {
      openfluid::base::Environment::addExtraSimulatorsDirs(P);
    }

    updateWaresWatchersPaths();
    WaresWatchingUpdated = true;

    updateSimulatorsWares();
  }

  if (PrefsDlg.isObsPathsChanged())
  {
    auto ExtraPaths = PrefsMgr->getBuilderExtraObserversPaths();

    openfluid::base::Environment::resetExtraObserversDirs();
    for (const auto& P : ExtraPaths)
    {
      openfluid::base::Environment::addExtraObserversDirs(P);
    }

    updateWaresWatchersPaths();
    WaresWatchingUpdated = true;

    updateObserversWares();
  }

  if (PrefsDlg.isWaresWatchingChanged() && !WaresWatchingUpdated)
  {
    updateWaresWatchersPaths();

    if (PrefsMgr->isBuilderWaresWatchersActive())
    {
      // update wares when re-enabling watching
      updateSimulatorsWares();
      updateObserversWares();
    }
  }

  emit refreshWaresEnabled(!PrefsMgr->isBuilderWaresWatchersActive());

  if(PrefsDlg.isTextEditorSettingsChanged())
  {
    updateWareSrcEditorsSettings();
  }

  return PrefsDlg.isRestartRequired();
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenRecentProjectsActionsChanged()
{

}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenRunAsked()
{
  // Get run mode
  std::string ModeStr =
    openfluid::base::RunContextManager::instance()
      ->getProjectContextValue("/builder/simulation/mode").get<std::string>("default");
  ProjectCentral::RunMode Mode = ProjectCentral::getRunModeValue(QString::fromStdString(ModeStr));

  // Save if automatic or specific run mode
  if (openfluid::base::PreferencesManager::instance()->isBuilderAutomaticSaveBeforeRun() || 
      Mode != ProjectCentral::RunMode::DEFAULT)
  {
    whenSaveAsked();
  }

  emit simulationStarted();


  // pausing plugs watchers

  QStringList SimPlugsPaths = mp_SimulatorsPlugsWatcher->directories();
  mp_SimulatorsPlugsWatcher->removePaths(SimPlugsPaths);

  QStringList ObsPlugsPaths = mp_ObserversPlugsWatcher->directories();
  mp_ObserversPlugsWatcher->removePaths(ObsPlugsPaths);


  mp_ProjectCentral->run(Mode);


  // resuming plugs watchers

  mp_SimulatorsPlugsWatcher->addPaths(SimPlugsPaths);
  mp_ObserversPlugsWatcher->addPaths(ObsPlugsPaths);

  // following two lines are added in conjunction with removal of equivalent lines in ProjectCentral::run()
  updateSimulatorsWares();
  updateObserversWares();

  emit simulationFinished();
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenRunModeAsked(ProjectCentral::RunMode Mode)
{
  openfluid::base::RunContextManager::instance()
    ->setProjectContextValue("/builder/simulation","mode",ProjectCentral::getRunModeStr(Mode).toStdString());
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenExtensionAsked(const QString& ID)
{
  // TODO provide simulators and observers registries to extensions?
  // for model and monitoring handling for example

  openfluid::ware::WareID_t WareID = ID.toStdString();

  ExtensionsRegistry* ExtReg = ExtensionsRegistry::instance();

  if (ExtReg->isFeatureExtensionRegistered(WareID))
  {
    if (!ExtReg->isFeatureExtensionActive(WareID))
    {
      if (ExtReg->getExtensionMode(WareID) == openfluid::builderext::ExtensionMode::MODAL)
      {
        openfluid::ui::builderext::PluggableModalExtension* ExtModal =
            (openfluid::ui::builderext::PluggableModalExtension*)(ExtReg->instanciateFeatureExtension(WareID));
        ExtModal->setParent(QApplication::activeWindow(),Qt::Dialog);
        ExtModal->setModal(true);

        // TODO set correct extension configuration
        ExtModal->setConfiguration(openfluid::ware::WareParams_t());
        ExtModal->setFluidXDescriptor(&(mp_ProjectCentral->descriptors()));

        connect(ExtModal,SIGNAL(fluidxChanged(openfluid::ui::builderext::FluidXUpdateFlags::Flags)),
                this,SLOT(dispatchChangesFromExtension(openfluid::ui::builderext::FluidXUpdateFlags::Flags)));
        connect(this,SIGNAL(fluidxChanged(openfluid::ui::builderext::FluidXUpdateFlags::Flags)),
                ExtModal,SLOT(update(openfluid::ui::builderext::FluidXUpdateFlags::Flags)));
        connect(this,SIGNAL(simulationStarted()),ExtModal,SLOT(manageSimulationStart()));
        connect(this,SIGNAL(simulationFinished()),ExtModal,SLOT(manageSimulationFinish()));

        if (ExtModal->initialize())
        {
          ExtModal->update(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_ALL);
          ExtModal->exec();
        }

        ExtReg->releaseFeatureExtension(ExtModal);
        ExtModal->deleteLater();
      }
      else if (ExtReg->getExtensionMode(WareID) == openfluid::builderext::ExtensionMode::MODELESS)
      {
        openfluid::ui::builderext::PluggableModelessExtension* ExtModeless =
            (openfluid::ui::builderext::PluggableModelessExtension*)(ExtReg->instanciateFeatureExtension(WareID));
        ExtModeless->setParent(QApplication::activeWindow(),Qt::Dialog);
        ExtModeless->setModal(false);

        // TODO set correct extension configuration
        ExtModeless->setConfiguration(openfluid::ware::WareParams_t());
        ExtModeless->setFluidXDescriptor(&(mp_ProjectCentral->descriptors()));

        connect(ExtModeless,SIGNAL(finished(int)),this, SLOT(releaseModelessExtension()));
        connect(ExtModeless,SIGNAL(fluidxChanged(openfluid::ui::builderext::FluidXUpdateFlags::Flags)),
                this,SLOT(dispatchChangesFromExtension(openfluid::ui::builderext::FluidXUpdateFlags::Flags)));
        connect(this,SIGNAL(fluidxChanged(openfluid::ui::builderext::FluidXUpdateFlags::Flags)),
                ExtModeless,SLOT(update(openfluid::ui::builderext::FluidXUpdateFlags::Flags)));
        connect(this,SIGNAL(simulationStarted()),ExtModeless,SLOT(manageSimulationStart()));

        connect(this,SIGNAL(simulationFinished()),ExtModeless,SLOT(manageSimulationFinish()));

        if (ExtModeless->initialize())
        {
          ExtModeless->update(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_ALL);
          ExtModeless->show();
        }
        else
        {
          releaseModelessExtension(ExtModeless);
        }
      }
      else if (ExtReg->getExtensionMode(WareID) == openfluid::builderext::ExtensionMode::WORKSPACE)
      {
        openfluid::ui::builderext::PluggableWorkspaceExtension* ExtWork =
            (openfluid::ui::builderext::PluggableWorkspaceExtension*)(ExtReg->instanciateFeatureExtension(WareID));
        ExtWork->setProperty("ID",QString::fromStdString(WareID));

        // TODO set correct extension configuration

        ExtWork->setConfiguration(openfluid::ware::WareParams_t());
        ExtWork->setFluidXDescriptor(&(mp_ProjectCentral->descriptors()));

        connect(ExtWork,SIGNAL(fluidxChanged(openfluid::ui::builderext::FluidXUpdateFlags::Flags)),
                this,SLOT(dispatchChangesFromExtension(openfluid::ui::builderext::FluidXUpdateFlags::Flags)));
        connect(this,SIGNAL(fluidxChanged(openfluid::ui::builderext::FluidXUpdateFlags::Flags)),
                ExtWork,SLOT(update(openfluid::ui::builderext::FluidXUpdateFlags::Flags)));
        connect(this,SIGNAL(simulationStarted()),ExtWork,SLOT(manageSimulationStart()));
        connect(this,SIGNAL(simulationFinished()),ExtWork,SLOT(manageSimulationFinish()));

        if (ExtWork->initialize())
        {
          const auto& Ext = ExtReg->registeredFeatureExtensions().at(WareID);

          QString TabText = WaresTranslationsRegistry::instance()
            ->tryTranslate(QString::fromStdString(Ext.Container.getPath()),
                           "signature",QString::fromStdString(Ext.Container.signature()->MenuText));

          // Replace empty menu text by extension ID
          TabText = QString::fromStdString(openfluid::tools::replaceEmptyString(TabText.toStdString(),WareID));

          ExtWork->update(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_ALL);
          this->addWorkspaceExtensionTab(ExtWork,TabText);
        }
        else
        {
          // destruction of the extension
          ExtensionsRegistry::instance()->releaseFeatureExtension(ExtWork);
          ExtWork->deleteLater();
        }
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


void ProjectModuleWidget::whenWaresRefreshAsked()
{
  updateSimulatorsWares();
  updateObserversWares();
}


// =====================================================================
// =====================================================================


bool ProjectModuleWidget::whenOpenExampleAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenSrcEditAsked(const QString& ID,openfluid::ware::WareType WType,bool Ghost)
{
  if (Ghost)
  {
    const auto& Container = openfluid::machine::SimulatorRegistry::instance()->wareContainer(ID.toStdString());

    if (Container.isValid() && Container.isGhost())
    {
      openfluid::ui::common::EditSignatureDialog Dlg(this);
      Dlg.initialize(*(Container.signature()));
      if (Dlg.exec() == QDialog::Accepted)
      {
        Dlg.exportSignature(QString::fromStdString(Container.getPath()));
        updateSimulatorsWares();
      }
    }
    else
    {
      QMessageBox::critical(QApplication::activeWindow(),
                            tr("Ghost simulator edition error"),
                            tr("Unable to find ghost simulator"),
                            QMessageBox::Close);
    }
  }
  else
  {
    QString ErrMsg;

    QString Path = QString::fromStdString(openfluid::waresdev::WareSrcEnquirer::findWarePath(WType,ID.toStdString()));

    if(!Path.isEmpty())
    {
      addWorkspaceWareSrcTab(Path);
    }
    else
    {
      QMessageBox::critical(QApplication::activeWindow(),
                            tr("Source code edition error"),
                            tr("Cannot edit source code of %1").arg(ID),
                            QMessageBox::Close);
    }
  }
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenSrcGenerateAsked(const QString& ID)
{
  openfluid::ware::SimulatorSignature Signature;
  std::string FileFullPath;

  const auto& Container = openfluid::machine::SimulatorRegistry::instance()->wareContainer(ID.toStdString());

  if (Container.isValid() && Container.isGhost())
  {
    mp_WareSrcCollection->newSimulatorFromGhost(*(Container.signature().get()));
  }
  else
  {
    QMessageBox::critical(QApplication::activeWindow(),
                          tr("Source code generation error"),
                          tr("Unable to find ghost simulator"),
                          QMessageBox::Close);
  }
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenNewSimulatorSrcAsked()
{
  mp_WareSrcCollection->newSimulator();
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenNewGhostSimulatorAsked()
{
  QStringList ExistingIDs;

  for (const auto& C : openfluid::machine::SimulatorRegistry::instance()->availableWares())
  {
    if (C.second.isValid() && C.second.isGhost())
    {
      ExistingIDs.append(QString::fromStdString(C.second.signature()->ID));
    }
  }

  openfluid::ui::common::EditSignatureDialog Dlg(this);
  Dlg.initialize(openfluid::ware::SimulatorSignature(),ExistingIDs);

  if (Dlg.exec() == QDialog::Accepted)
  {
    openfluid::ware::SimulatorSignature Signature = Dlg.getSignature();
    auto GhostFilePath = 
      openfluid::waresdev::getGhostSimulatorPath(openfluid::base::Environment::getDefaultSimulatorsDirs().front(),
      Signature.ID);
    openfluid::waresdev::SimulatorSignatureSerializer().writeToJSONFile(Signature,GhostFilePath);
    updateSimulatorsWares();
  }
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenOpenSimulatorSrcAsked()
{
  mp_WareSrcCollection->openSimulator();
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenNewObserverSrcAsked()
{
  mp_WareSrcCollection->newObserver();
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenOpenObserverSrcAsked()
{
  mp_WareSrcCollection->openObserver();
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::whenLaunchDevStudioAsked()
{
  launchDevStudio();
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::dispatchChanges(openfluid::ui::builderext::FluidXUpdateFlags::Flags UpdateFlags)
{
  if (UpdateFlags.testFlag(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_ALL))
  {
    mp_SpatialTab->refresh();
    mp_DatastoreTab->refresh();
  }
  else
  {
    if (UpdateFlags.testFlag(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_DATASTORE))
    {
      mp_SpatialTab->refreshMap();
    }
  }

  emit fluidxChanged(UpdateFlags);

  doCheck();
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::dispatchChangesFromExtension(openfluid::ui::builderext::FluidXUpdateFlags::Flags UpdateFlags)
{

  if (UpdateFlags.testFlag(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_SPATIALATTRS) ||
      UpdateFlags.testFlag(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_SPATIALSTRUCT))
  {
    mp_SpatialTab->refresh();
  }


  if (UpdateFlags.testFlag(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_DATASTORE))
  {
    mp_DatastoreTab->refresh();
  }


  dispatchChanges(UpdateFlags);
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::releaseModelessExtension(openfluid::ui::builderext::PluggableModelessExtension* Sender)
{
  if (Sender == nullptr)
  {
    Sender = (openfluid::ui::builderext::PluggableModelessExtension*)(QObject::sender());
  }

  if (Sender)
  {
    ExtensionsRegistry::instance()->releaseFeatureExtension(Sender);
    Sender->deleteLater();
  }
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::updateSimulatorsWares()
{
  emit runEnabled(false);

  mp_ModelTab->prepareWaresUpdate();
  openfluid::machine::SimulatorRegistry::instance()->discoverWares();
  mp_ModelTab->updateWares();

  doCheck();
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::updateObserversWares()
{
  emit runEnabled(false);

  mp_MonitoringTab->prepareWaresUpdate();
  openfluid::machine::ObserverRegistry::instance()->discoverWares();
  mp_MonitoringTab->updateWares();

  doCheck();
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::checkInputDir()
{
  doCheck();
}


// =====================================================================
// =====================================================================


bool ProjectModuleWidget::isOkForSimulation() const
{
  return mp_ProjectCentral->checkInfos()->isOKForSimulation();
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::doCheck()
{
  mp_ProjectCentral->check();
  mp_DashboardFrame->refresh();

  emit runEnabled(mp_ProjectCentral->checkInfos()->isOKForSimulation());
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::onBuildLaunched(openfluid::ware::WareType /*Type*/, const QString& /*ID*/)
{
  emit runEnabled(false);
  m_ActiveBuilds++;
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::onBuildFinished(openfluid::ware::WareType /*Type*/, const QString& /*ID*/)
{
  // TODO add guards? m_ActiveBuilds should be >= 1 before decreasing

  m_ActiveBuilds--;

  if (!m_ActiveBuilds)
  {
    updateSimulatorsWares();
    updateObserversWares();
  }
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::addWorkspaceTab(QWidget* Tab, const QString& Label)
{
  mp_WorkspaceTabWidget->addWorkspaceTab(Tab,Label);
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::addWorkspaceExtensionTab(QWidget* Tab, const QString& Label)
{
  mp_WorkspaceTabWidget->addWorkspaceTab(Tab,Label,true);
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::addWorkspaceWareSrcTab(const QString& Path)
{
  if (!mp_WareSrcCollection->openPath(Path))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Unable to add ware source tab, path not in known places");
  }
}


// =====================================================================
// =====================================================================


void ProjectModuleWidget::updateWareSrcEditorsSettings()
{
  mp_WareSrcCollection->updateEditorsSettings();
}

