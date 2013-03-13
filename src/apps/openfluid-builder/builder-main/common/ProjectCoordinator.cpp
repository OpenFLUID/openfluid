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
 \file ProjectCoordinator.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ProjectCoordinator.hpp"

#include <giomm/file.h>

#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/builderext/WorkspaceTab.hpp>
#include <openfluid/builderext/ModelessWindow.hpp>

#include "ProjectExplorerModel.hpp"
#include "ProjectWorkspace.hpp"
#include "EngineProject.hpp"
#include "BuilderModuleFactory.hpp"
#include "ProjectDashboard.hpp"
#include <openfluid/guicommon/FunctionSignatureRegistry.hpp>
#include "BuilderExtensionsManager.hpp"

#include "ModelStructureModule.hpp"
#include "DomainClassModule.hpp"
//#include "ResultsSetModule.hpp"

#include <openfluid/guicommon/BuilderDescriptor.hpp>

// =====================================================================
// =====================================================================

ProjectCoordinator::ProjectCoordinator(ProjectExplorerModel& ExplorerModel,
                                       ProjectWorkspace& Workspace,
                                       EngineProject& TheEngineProject,
                                       ProjectDashboard& TheProjectDashboard) :
    m_ExplorerModel(ExplorerModel), m_Workspace(Workspace), m_EngineProject(
        TheEngineProject), m_ProjectDashboard(TheProjectDashboard), m_HasRun(
        false), m_ModelPageName(_("Model")), m_DomainPageName(
        _("Spatial domain")), m_RunPageName(_("Run configuration")), m_MonitoringPageName(
        _("Monitoring")), m_FileMonitorHasChanged(false), m_FileMonitorHasToDisplay(
        true)
{
  mp_ModuleFactory = new BuilderModuleFactory(m_EngineProject);

  m_ExplorerModel.signal_ActivationChanged().connect(
      sigc::mem_fun(*this, &ProjectCoordinator::whenActivationChanged));
  m_Workspace.signal_PageRemoved().connect(
      sigc::mem_fun(*this, &ProjectCoordinator::whenPageRemoved));

  m_EngineProject.signal_RunStarted().connect(
      sigc::mem_fun(*this, &ProjectCoordinator::whenRunStarted));
  m_EngineProject.signal_RunStopped().connect(
      sigc::mem_fun(*this, &ProjectCoordinator::whenRunStopped));

  mp_FileMonitorDialog = new Gtk::MessageDialog("", false,
                                                Gtk::MESSAGE_QUESTION,
                                                Gtk::BUTTONS_OK_CANCEL, false);
  mp_FileMonitorDialog->signal_response().connect(
      sigc::mem_fun(*this, &ProjectCoordinator::whenUpdatePluginsAsked));

  updatePluginPathsMonitors();

}

// =====================================================================
// =====================================================================

sigc::signal<void, bool> ProjectCoordinator::signal_CheckHappened()
{
  return m_signal_CheckHappened;
}

// =====================================================================
// =====================================================================

sigc::signal<void> ProjectCoordinator::signal_ChangeHappened()
{
  return m_signal_ChangeHappened;
}

// =====================================================================
// =====================================================================

ProjectCoordinator::~ProjectCoordinator()
{
  // clearing tabs

  for (std::map<std::string, openfluid::guicommon::ProjectWorkspaceModule*>::iterator it =
      m_ModulesByPageNameMap.begin(); it != m_ModulesByPageNameMap.end(); ++it)
  {
    if (m_TabExtensionIdByNameMap.count(it->first))
    {
      BuilderExtensionsManager::getInstance()->deletePluggableExtension(
          m_TabExtensionIdByNameMap[it->first]);
    }
    else
    {
      delete it->second;
      it->second = 0;
    }
  }
  m_ModulesByPageNameMap.clear();

  // clearing modeless extensions

  for (std::map<std::string, openfluid::builderext::ModelessWindow*>::iterator it =
      m_ModelessWindowsExtensionsMap.begin();
      it != m_ModelessWindowsExtensionsMap.end(); ++it)
  {
    it->second->onProjectClosed();
    BuilderExtensionsManager::getInstance()->deletePluggableExtension(
        it->first);
  }

  delete mp_FileMonitorDialog;
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenActivationChanged()
{
  std::string PageName = "";
  openfluid::guicommon::ProjectWorkspaceModule* Module;

  switch (m_ExplorerModel.getActivatedElement().first)
  {
    case ProjectExplorerCategories::EXPLORER_MODEL:
      PageName = m_ModelPageName;
      if (m_Workspace.existsPageName(PageName))
        Module = m_ModulesByPageNameMap[PageName];
      else
      {
        Module =
            static_cast<openfluid::guicommon::ProjectWorkspaceModule*>(mp_ModuleFactory->createModelStructureModule());

        Module->signal_ModuleChanged().connect(
            sigc::mem_fun(*this, &ProjectCoordinator::whenModelChanged));

        addModuleToWorkspace(PageName, *Module);
      }
      if (m_ExplorerModel.getActivatedElement().second != "")
        (static_cast<ModelStructureModule*>(Module))->setCurrentFunction(
            m_ExplorerModel.getActivatedElement().second);
      break;

    case ProjectExplorerCategories::EXPLORER_DOMAIN:
      PageName = m_DomainPageName;
      if (!m_Workspace.existsPageName(PageName))
      {
        Module =
            static_cast<openfluid::guicommon::ProjectWorkspaceModule*>(mp_ModuleFactory->createDomainStructureModule());

        Module->signal_ModuleChanged().connect(
            sigc::mem_fun(*this, &ProjectCoordinator::whenDomainChanged));

        addModuleToWorkspace(PageName, *Module);
      }
      break;

    case ProjectExplorerCategories::EXPLORER_CLASS:
      PageName = constructClassPageName(
          m_ExplorerModel.getActivatedElement().second);
      if (!m_Workspace.existsPageName(PageName))
      {
        Module =
            static_cast<openfluid::guicommon::ProjectWorkspaceModule*>(mp_ModuleFactory->createDomainClassModule());

        Module->signal_ModuleChanged().connect(
            sigc::mem_fun(*this, &ProjectCoordinator::whenClassChanged));

        (static_cast<DomainClassModule*>(Module))->setSelectedClassFromApp(
            m_ExplorerModel.getActivatedElement().second);

        m_ClassPageNames.insert(PageName);

        addModuleToWorkspace(PageName, *Module);
      }
      break;

    case ProjectExplorerCategories::EXPLORER_RUN:
      PageName = m_RunPageName;
      if (!m_Workspace.existsPageName(PageName))
      {
        Module =
            static_cast<openfluid::guicommon::ProjectWorkspaceModule*>(mp_ModuleFactory->createSimulationRunModule());

        Module->signal_ModuleChanged().connect(
            sigc::mem_fun(*this, &ProjectCoordinator::whenRunChanged));

        addModuleToWorkspace(PageName, *Module);
      }
      break;

    case ProjectExplorerCategories::EXPLORER_MONITORING:
      PageName = m_MonitoringPageName;
      if (!m_Workspace.existsPageName(PageName))
      {
        Module =
            static_cast<openfluid::guicommon::ProjectWorkspaceModule*>(mp_ModuleFactory->createMonitoringModule());

        Module->signal_ModuleChanged().connect(
            sigc::mem_fun(*this, &ProjectCoordinator::whenMonitoringChanged));

        addModuleToWorkspace(PageName, *Module);
      }
      break;

    case ProjectExplorerCategories::EXPLORER_NONE:
    default:
      break;
  }

  m_Workspace.setCurrentPage(PageName);

}

// =====================================================================
// =====================================================================

std::string ProjectCoordinator::constructClassPageName(std::string ClassName)
{
  return Glib::ustring::compose(_("%1 [%2]"), _("Spatial data"), ClassName);
}

// =====================================================================
// =====================================================================

//std::string ProjectCoordinator::constructSetPageName(std::string SetName)
//{
//  return Glib::ustring::compose(_("%1 [%2]"), _("Results"), SetName);
//}

// =====================================================================
// =====================================================================

void ProjectCoordinator::addModuleToWorkspace(
    std::string PageName, openfluid::guicommon::ProjectWorkspaceModule& Module)
{
  m_ModulesByPageNameMap[PageName] = &Module;
  m_Workspace.appendPage(PageName, Module.composeAndGetAsWidget());
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenModelChanged()
{
//  Glib::ustring OutputConsistencyMessage =
//      m_EngineProject.checkOutputsConsistency();
//
//  if (!OutputConsistencyMessage.empty())
//    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
//        Glib::ustring::compose(_(
//            "This change leads OpenFLUID to delete:\n%1"),
//            OutputConsistencyMessage));

  computeModelChanges();
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::updateWorkspaceModules()
{
  for (std::map<std::string, openfluid::guicommon::ProjectWorkspaceModule*>::iterator it =
      m_ModulesByPageNameMap.begin(); it != m_ModulesByPageNameMap.end(); ++it)
  {
    it->second->update();
  }
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::checkProject()
{
  bool GlobalState;

  m_ProjectDashboard.setCheckInfo(m_EngineProject.check(GlobalState));

  m_signal_CheckHappened.emit(GlobalState);
}

// =====================================================================
// =====================================================================

//void ProjectCoordinator::updateResults()
//{
//  if (m_HasRun)
//    m_ExplorerModel.updateResultsAsked(true);
//}

// =====================================================================
// =====================================================================

//TODO allow the user to cancel deletion
void ProjectCoordinator::whenDomainChanged()
{
//  Glib::ustring OutputConsistencyMessage =
//      m_EngineProject.checkOutputsConsistency();
//
//  if (!OutputConsistencyMessage.empty())
//    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
//        Glib::ustring::compose(_(
//            "This change leads OpenFLUID to delete:\n%1"),
//            OutputConsistencyMessage));

  computeDomainChanges();
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::removeDeletedClassPages()
{
  std::set<std::string> ClassNames;

  std::set<std::string> DomainClassNames =
      m_EngineProject.getBuilderDesc().getDomain().getClassNames();

  std::set<std::string>::iterator it;

  for (it = DomainClassNames.begin(); it != DomainClassNames.end(); ++it)
    ClassNames.insert(constructClassPageName(*it));

  std::set<std::string> ClassPageNames = m_ClassPageNames;
  for (it = ClassPageNames.begin(); it != ClassPageNames.end(); ++it)
  {
    if (!ClassNames.count(*it))
      m_Workspace.removePage(*it);
  }
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenClassChanged()
{
  updateModelessWindowsExtensions();

  checkProject();

  m_signal_ChangeHappened.emit();
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenRunChanged()
{
  updateModelessWindowsExtensions();

//  updateResults();

  m_ExplorerModel.updateSimulationAsked();

  updateWorkspaceModules();

  checkProject();

  m_signal_ChangeHappened.emit();
}

// =====================================================================
// =====================================================================

//void ProjectCoordinator::whenOutChanged()
//{
//  updateModelessWindowsExtensions();
//
////  updateResults();
//
////  removeDeletedSetPages();
//
//  updateWorkspaceModules();
//
//  checkProject();
//
//  m_signal_ChangeHappened.emit();
//}

// =====================================================================
// =====================================================================

void ProjectCoordinator::computeModelChanges()
{
  updateModelessWindowsExtensions();

//  updateResults();

  m_ExplorerModel.updateModelAsked();

//  removeDeletedSetPages();

  updateWorkspaceModules();

  checkProject();

  m_signal_ChangeHappened.emit();
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::computeDomainChanges()
{
  updateModelessWindowsExtensions();

//  updateResults();
  m_ExplorerModel.updateDomainAsked();

  removeDeletedClassPages();
//  removeDeletedSetPages();

  updateWorkspaceModules();

  checkProject();

  m_signal_ChangeHappened.emit();
}

// =====================================================================
// =====================================================================

//void ProjectCoordinator::removeDeletedSetPages()
//{
//  std::set<std::string> SetNames;
//
//  for (unsigned int i = 0; i
//      < m_EngineProject.getOutputDescriptor().getFileSets().size(); i++)
//  {
//    for (unsigned int j = 0; j
//        < m_EngineProject.getOutputDescriptor().getFileSets()[i].getSets().size(); j++)
//    {
//      SetNames.insert(
//          constructSetPageName(
//              m_EngineProject.getOutputDescriptor().getFileSets()[i].getSets()[j].getName()));
//    }
//  }
//
//  std::set<std::string> SetPageNames = m_SetPageNames;
//  for (std::set<std::string>::iterator it = SetPageNames.begin(); it
//      != SetPageNames.end(); ++it)
//  {
//    if (!SetNames.count(*it))
//      m_Workspace.removePage(*it);
//  }
//}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenMonitoringChanged()
{
  m_signal_ChangeHappened.emit();
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenMapViewChanged()
{
//  std::cout << "MapView has changed" << std::endl;
  // check if update of Domain is sufficient
  //  updateWorkspaceModules();
  //  updateResults();
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenPageRemoved(std::string RemovedPageName)
{
  openfluid::guicommon::ProjectWorkspaceModule* ModuleToDelete =
      m_ModulesByPageNameMap[RemovedPageName];

  if (m_TabExtensionIdByNameMap.count(RemovedPageName))
    BuilderExtensionsManager::getInstance()->deletePluggableExtension(
        m_TabExtensionIdByNameMap[RemovedPageName]);
  else
    delete ModuleToDelete;

  m_ModulesByPageNameMap.erase(RemovedPageName);

  // update Class Pages
  if (m_ClassPageNames.count(RemovedPageName))
    m_ClassPageNames.erase(RemovedPageName);

  // update Set Pages
//  if (m_SetPageNames.count(RemovedPageName))
//    m_SetPageNames.erase(RemovedPageName);
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::onDirMonitorChanged(
    const Glib::RefPtr<Gio::File>& /*File*/,
    const Glib::RefPtr<Gio::File>& /*OtherFile*/,
    Gio::FileMonitorEvent /*EventType*/)
{
  if (!m_FileMonitorHasToDisplay)
    m_FileMonitorHasChanged = true;
  else if (!mp_FileMonitorDialog->is_popup())
  {
    std::string Msg =
        _( "Changes occur in the functions list.") + std::string(
            _("\nDo you want to reload it?\n(if not, it's at your own risk, you have to manually reload the simulation functions list)"));
    mp_FileMonitorDialog->set_message(Msg);
    mp_FileMonitorDialog->show_all();
  }
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenUpdatePluginsAsked(int ResponseId)
{
  mp_FileMonitorDialog->hide();

  if (ResponseId != Gtk::RESPONSE_OK)
    return;

  std::string MissingFunctionsStr = "";

  std::list<openfluid::fluidx::ModelItemDescriptor*> ModifiedFunctions =
      m_EngineProject.getBuilderDesc().getModel().checkAndGetModifiedModel(
          MissingFunctionsStr);

  if (!MissingFunctionsStr.empty())
  {
    m_EngineProject.getBuilderDesc().getModel().setItems(ModifiedFunctions);

    Glib::ustring Msg =
        Glib::ustring::compose(
            _("These plugin file(s) are no more available:\n%1\n\n"
                "Corresponding simulation functions have been removed from the model.\n"),
            MissingFunctionsStr);

    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(Msg);
  }

  // to clear and re-create all FctParamsComponents with right function signatures,
  // and update AddFctDialog with available signatures
  if (m_Workspace.existsPageName(m_ModelPageName))
  {
    (static_cast<ModelStructureModule*>(m_ModulesByPageNameMap[m_ModelPageName]))->updateWithFctParamsComponents();
  }
  else
  {
    ModelStructureModule * TempModelModule =
        static_cast<ModelStructureModule*>(mp_ModuleFactory->createModelStructureModule());
    TempModelModule->updateWithFctParamsComponents();
    delete TempModelModule;
  }

  m_ExplorerModel.updateModelAsked();

  updateWorkspaceModules();

  checkProject();

  m_signal_ChangeHappened.emit();
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenMapViewAsked()
{
//  std::string PageName = "MapView";
//  openfluid::guicommon::ProjectWorkspaceModule* Module;
//
//  if (m_Workspace.existsPageName(PageName))
//    Module = m_ModulesByPageNameMap[PageName];
//  else
//  {
//    Module =
//        static_cast<openfluid::guicommon::ProjectWorkspaceModule*>(mp_ModuleFactory->createMapViewModule());
//
//    Module->signal_ModuleChanged().connect(
//        sigc::mem_fun(*this, &ProjectCoordinator::whenMapViewChanged));
//
//    addModuleToWorkspace(PageName, *Module);
//  }
//
//  m_Workspace.setCurrentPage(PageName);
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::setFileMonitorDisplayState(bool HasToDisplay)
{
  m_FileMonitorHasToDisplay = HasToDisplay;

  if (m_FileMonitorHasToDisplay)
  {
    if (m_FileMonitorHasChanged && !mp_FileMonitorDialog->is_popup())
    {
      std::string Msg =
          _( "Changes occur in the functions list while simulation was running.") + std::string(
              _("\nDo you want to reload it?\n(if not, it's at your own risk, you have to manually reload the simulation functions)"));
      mp_FileMonitorDialog->set_message(Msg);
      mp_FileMonitorDialog->show_all();
    }

    m_FileMonitorHasChanged = false;
  }
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::updatePluginPathsMonitors()
{
  m_DirMonitors.clear();

  std::vector<std::string> PluginPaths =
      openfluid::base::RuntimeEnvironment::getInstance()->getFunctionsPluginsPaths();

  for (unsigned int i = 0; i < PluginPaths.size(); i++)
  {
    Glib::RefPtr<Gio::File> ItemFile = Gio::File::create_for_path(
        PluginPaths[i]);

    Glib::RefPtr<Gio::FileMonitor> DirMonitor = ItemFile->monitor_directory();

    DirMonitor->signal_changed().connect(
        sigc::mem_fun(*this, &ProjectCoordinator::onDirMonitorChanged));

    m_DirMonitors.push_back(DirMonitor);
  }
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::launchExtension(std::string ExtensionID)
{
  ExtensionContainer* ExtCont =
      BuilderExtensionsManager::getInstance()->getExtensionContainer(
          ExtensionID);

  if (!ExtCont)
    return;

  std::string ExtID = ExtCont->Infos.ID;
  openfluid::builderext::PluggableBuilderExtension::ExtensionType ExtType =
      ExtCont->Infos.Type;

  // Extension is already instantiated
  if (ExtCont->Extension)
  {
    if (ExtType == openfluid::builderext::PluggableBuilderExtension::WorkspaceTab)
      m_Workspace.setCurrentPage(ExtCont->Infos.ShortName);
    else if (ExtType
        == openfluid::builderext::PluggableBuilderExtension::ModelessWindow)
    {
      Gtk::Window * ExtWindow =
          dynamic_cast<Gtk::Window*>(ExtCont->Extension->getExtensionAsWidget());
      if (ExtWindow)
        ExtWindow->present();
    }
    return;
  }

  try
  {
    if (!ExtCont->instantiateExt())
      return;
  }
  catch (openfluid::base::OFException& e)
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(e.what());
    return;
  }

  openfluid::builderext::PluggableBuilderExtension* Ext = ExtCont->Extension;

  Ext->setBuilderDescriptor(m_EngineProject.getBuilderDesc());

  Ext->signal_ChangedOccurs().connect(
      sigc::mem_fun(*this, &ProjectCoordinator::whenExtensionChanged));

  // checking if extension is ready to be executed
  if (!Ext->isReadyForShowtime())
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(
        "Extension is not ready!");
    return;
  }

  if (ExtType == openfluid::builderext::PluggableBuilderExtension::WorkspaceTab)
  {
    std::string PageName = ExtCont->Infos.ShortName;

    openfluid::builderext::WorkspaceTab* Tab =
        static_cast<openfluid::builderext::WorkspaceTab*>(Ext);

    Tab->update();

    addModuleToWorkspace(PageName, *Tab);

    m_TabExtensionIdByNameMap[PageName] = ExtID;

    m_Workspace.setCurrentPage(PageName);
  }
  else
  {
    if ((ExtCont->Infos.Type
        == openfluid::builderext::PluggableBuilderExtension::ModelessWindow))
    {
      openfluid::builderext::ModelessWindow* ModelessWin =
          static_cast<openfluid::builderext::ModelessWindow*>(Ext);

      ModelessWin->signal_Hidden().connect(
          sigc::bind<std::string>(
              sigc::mem_fun(
                  *this,
                  &ProjectCoordinator::whenModelessWindowExtensionHidden),
              ExtID));

      m_ModelessWindowsExtensionsMap[ExtID] = ModelessWin;

      Ext->show();
    }
    else
    {
      Ext->show();
      ExtCont->deleteExt();
    }

  }

}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenExtensionChanged()
{
//  Glib::ustring OutputConsistencyMessage =
//      m_EngineProject.checkOutputsConsistency();
//
//  if (!OutputConsistencyMessage.empty())
//    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
//        Glib::ustring::compose(_(
//            "This change leads OpenFLUID to delete:\n%1"),
//            OutputConsistencyMessage));

  updateModelessWindowsExtensions();

//  updateResults();

  m_ExplorerModel.updateDomainAsked();

  m_ExplorerModel.updateModelAsked();

  m_ExplorerModel.updateSimulationAsked();

  removeDeletedClassPages();

//  removeDeletedSetPages();

  updateWorkspaceModules();

  checkProject();

  m_signal_ChangeHappened.emit();
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenModelessWindowExtensionHidden(std::string ExtID)
{
  BuilderExtensionsManager::getInstance()->deletePluggableExtension(ExtID);
  m_ModelessWindowsExtensionsMap.erase(ExtID);
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::updateModelessWindowsExtensions()
{
  for (std::map<std::string, openfluid::builderext::ModelessWindow*>::iterator it =
      m_ModelessWindowsExtensionsMap.begin();
      it != m_ModelessWindowsExtensionsMap.end(); ++it)
    it->second->onRefresh();
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenRunStarted()
{
  for (std::map<std::string, openfluid::builderext::ModelessWindow*>::iterator it =
      m_ModelessWindowsExtensionsMap.begin();
      it != m_ModelessWindowsExtensionsMap.end(); ++it)
    it->second->onRunStarted();
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenRunStopped()
{
  m_HasRun = true;

  whenDomainChanged(); // for functions that create units

//  m_ExplorerModel.updateResultsAsked(false);

  for (std::map<std::string, openfluid::builderext::ModelessWindow*>::iterator it =
      m_ModelessWindowsExtensionsMap.begin();
      it != m_ModelessWindowsExtensionsMap.end(); ++it)
    it->second->onRunStopped();
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::updateMonitoringModule()
{
  std::map<std::string, openfluid::guicommon::ProjectWorkspaceModule*>::iterator it =
      m_ModulesByPageNameMap.find(m_MonitoringPageName);

  if (it != m_ModulesByPageNameMap.end())
    it->second->update();

  m_signal_ChangeHappened.emit();
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

ProjectCoordinatorSub::ProjectCoordinatorSub(
    ProjectExplorerModel& ExplorerModel, ProjectWorkspace& Workspace,
    EngineProject& TheEngineProject, ProjectDashboard& TheProjectDashboard) :
    ProjectCoordinator(ExplorerModel, Workspace, TheEngineProject,
                       TheProjectDashboard)
{
}

// =====================================================================
// =====================================================================

void ProjectCoordinatorSub::whenModelChanged()
{
//  Glib::ustring OutputConsistencyMessage =
//      m_EngineProject.checkOutputsConsistency();
//
//  if (!OutputConsistencyMessage.empty())
//    std::cout << OutputConsistencyMessage << std::endl;

  computeModelChanges();
}

// =====================================================================
// =====================================================================

void ProjectCoordinatorSub::whenDomainChanged()
{
//  Glib::ustring OutputConsistencyMessage =
//      m_EngineProject.checkOutputsConsistency();
//
//  if (!OutputConsistencyMessage.empty())
//    std::cout << OutputConsistencyMessage << std::endl;

  computeDomainChanges();
}

// =====================================================================
// =====================================================================

