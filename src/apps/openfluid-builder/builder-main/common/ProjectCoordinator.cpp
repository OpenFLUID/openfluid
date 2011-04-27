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

#include "ProjectExplorerModel.hpp"
#include "ProjectWorkspace.hpp"
#include "EngineProject.hpp"
#include "BuilderModuleFactory.hpp"
#include "ProjectDashboard.hpp"

#include "ModelStructureModule.hpp"
#include "DomainClassModule.hpp"
#include "ResultsSetModule.hpp"

#include <boost/foreach.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include "BuilderPretestInfo.hpp"

// =====================================================================
// =====================================================================


ProjectCoordinator::ProjectCoordinator(ProjectExplorerModel& ExplorerModel,
    ProjectWorkspace& Workspace, EngineProject& TheEngineProject,
    ProjectDashboard& TheProjectDashboard) :
  m_ExplorerModel(ExplorerModel), m_Workspace(Workspace), m_EngineProject(
      TheEngineProject), m_ProjectDashboard(TheProjectDashboard)
{
  mp_ModuleFactory = new BuilderModuleFactory(m_EngineProject);

  m_ExplorerModel.setEngineRequirements(*m_EngineProject.getModelInstance(),
      *m_EngineProject.getSimBlob());

  m_ExplorerModel.signal_ActivationChanged().connect(sigc::mem_fun(*this,
      &ProjectCoordinator::whenActivationChanged));
  m_Workspace.signal_PageRemoved().connect(sigc::mem_fun(*this,
      &ProjectCoordinator::whenPageRemoved));
}

// =====================================================================
// =====================================================================


sigc::signal<void, bool> ProjectCoordinator::signal_CheckHappened()
{
  return m_signal_CheckHappened;
}

// =====================================================================
// =====================================================================


ProjectCoordinator::~ProjectCoordinator()
{
  // TODO Auto-generated destructor stub
}

// =====================================================================
// =====================================================================


void ProjectCoordinator::whenActivationChanged()
{
  std::string PageName = "";
  ProjectWorkspaceModule* Module;

  switch (m_ExplorerModel.getActivatedElement().first)
  {
    case ProjectExplorerCategories::EXPLORER_MODEL:
      PageName = _("Model");
      if (m_Workspace.existsPageName(PageName))
      {
        Module = m_ModulesByPageNameMap[PageName];
      } else
      {
        Module
            = (ProjectWorkspaceModule*) mp_ModuleFactory->createModelStructureModule();

        Module->signal_ModuleChanged().connect(sigc::mem_fun(*this,
            &ProjectCoordinator::whenModelChanged));
        Module->initialize();

        m_ModulesByPageNameMap[PageName] = Module;
        m_Workspace.appendPage(PageName, Module->composeAndGetAsWidget());
      }
      if (m_ExplorerModel.getActivatedElement().second != "")
        ((ModelStructureModule*) Module)->setCurrentFunction(
            m_ExplorerModel.getActivatedElement().second);
      break;
    case ProjectExplorerCategories::EXPLORER_DOMAIN:
      PageName = _("Domain");
      if (m_Workspace.existsPageName(PageName))
      {
        Module = m_ModulesByPageNameMap[PageName];
      } else
      {
        Module
            = (ProjectWorkspaceModule*) mp_ModuleFactory->createDomainStructureModule();

        Module->signal_ModuleChanged().connect(sigc::mem_fun(*this,
            &ProjectCoordinator::whenDomainChanged));
        Module->initialize();

        m_ModulesByPageNameMap[PageName] = Module;
        m_Workspace.appendPage(PageName, Module->composeAndGetAsWidget());
      }
      break;
    case ProjectExplorerCategories::EXPLORER_CLASS:
      PageName = m_ExplorerModel.getActivatedElement().second;
      if (m_Workspace.existsPageName(PageName))
      {
        Module = m_ModulesByPageNameMap[PageName];
      } else
      {
        Module
            = (ProjectWorkspaceModule*) mp_ModuleFactory->createDomainClassModule();

        Module->signal_ModuleChanged().connect(sigc::mem_fun(*this,
            &ProjectCoordinator::whenClassChanged));
        Module->initialize();

        ((DomainClassModule*) Module)->setSelectedClassFromApp(
            m_ExplorerModel.getActivatedElement().second);

        m_ModulesByPageNameMap[PageName] = Module;
        m_ClassPageNames.push_back(PageName);
        m_Workspace.appendPage(PageName, Module->composeAndGetAsWidget());
      }
      break;
    case ProjectExplorerCategories::EXPLORER_RUN:
      PageName = _("Run");
      if (m_Workspace.existsPageName(PageName))
      {
        Module = m_ModulesByPageNameMap[PageName];
      } else
      {
        Module
            = (ProjectWorkspaceModule*) mp_ModuleFactory->createSimulationRunModule();

        Module->signal_ModuleChanged().connect(sigc::mem_fun(*this,
            &ProjectCoordinator::whenRunChanged));
        Module->initialize();

        m_ModulesByPageNameMap[PageName] = Module;
        m_Workspace.appendPage(PageName, Module->composeAndGetAsWidget());
      }
      break;
    case ProjectExplorerCategories::EXPLORER_OUTPUTS:
      PageName = _("Outputs");
      if (m_Workspace.existsPageName(PageName))
      {
        Module = m_ModulesByPageNameMap[PageName];
      } else
      {
        Module
            = (ProjectWorkspaceModule*) mp_ModuleFactory->createSimulationOutModule();

        Module->signal_ModuleChanged().connect(sigc::mem_fun(*this,
            &ProjectCoordinator::whenOutChanged));
        Module->initialize();

        m_ModulesByPageNameMap[PageName] = Module;
        m_Workspace.appendPage(PageName, Module->composeAndGetAsWidget());
      }
      break;
    case ProjectExplorerCategories::EXPLORER_SET:
      PageName = m_ExplorerModel.getActivatedElement().second;
      if (m_Workspace.existsPageName(PageName))
      {
        Module = m_ModulesByPageNameMap[PageName];
      } else
      {
        Module
            = (ProjectWorkspaceModule*) mp_ModuleFactory->createResultsSetModule();

        Module->initialize();

        ((ResultsSetModule*) Module)->setSelectedSetFromApp(
            m_ExplorerModel.getActivatedElement().second);

        m_ModulesByPageNameMap[PageName] = Module;
        m_SetPageNames.push_back(PageName);
        m_Workspace.appendPage(PageName, Module->composeAndGetAsWidget());
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


void ProjectCoordinator::whenModelChanged()
{
  m_ExplorerModel.updateModelAsked();

  updateWorkspaceModules();

  checkProject();
}

// =====================================================================
// =====================================================================


void ProjectCoordinator::updateWorkspaceModules()
{
  for (std::map<std::string, ProjectWorkspaceModule*>::iterator it =
      m_ModulesByPageNameMap.begin(); it != m_ModulesByPageNameMap.end(); ++it)
  {
    it->second->update();
  }
}

// =====================================================================
// =====================================================================


void ProjectCoordinator::checkProject()
{
  BuilderPretestInfo CheckInfo;

  m_EngineProject.check(CheckInfo);

  CheckInfo.addBuilderInfo(m_EngineProject.getModelInstance(),
      m_EngineProject.getSimBlob());

  m_ProjectDashboard.setCheckInfo(CheckInfo);

  m_signal_CheckHappened.emit(CheckInfo.getGlobalCheckState());
}

// =====================================================================
// =====================================================================


void ProjectCoordinator::whenDomainChanged()
{
  m_ExplorerModel.updateDomainAsked();

  std::vector<std::string> PagesToDelete = getClassPagesToDelete();
  BOOST_FOREACH(std::string PageToDelete,PagesToDelete)
{  m_Workspace.removePage(PageToDelete);
}

updateWorkspaceModules();

checkProject();

}

// =====================================================================
// =====================================================================


std::vector<std::string> ProjectCoordinator::getClassPagesToDelete()
{
  std::vector<std::string> ClassNames;

  for (openfluid::core::UnitsListByClassMap_t::const_iterator it =
      m_EngineProject.getCoreRepository().getUnitsByClass()->begin(); it
      != m_EngineProject.getCoreRepository().getUnitsByClass()->end(); ++it)
  {
    if (!it->second.getList()->empty())
      ClassNames.push_back(it->first);
  }

  std::sort(ClassNames.begin(), ClassNames.end());
  std::sort(m_ClassPageNames.begin(), m_ClassPageNames.end());

  //are in 1st but not in 2d
  std::vector<std::string> PagesToDelete;
  std::set_difference(m_ClassPageNames.begin(), m_ClassPageNames.end(),
      ClassNames.begin(), ClassNames.end(), std::back_inserter(PagesToDelete));

  return PagesToDelete;
}

// =====================================================================
// =====================================================================

void ProjectCoordinator::whenClassChanged()
{
  checkProject();
}

// =====================================================================
// =====================================================================


void ProjectCoordinator::whenRunChanged()
{
  m_ExplorerModel.updateSimulationAsked();

  updateWorkspaceModules();
}

// =====================================================================
// =====================================================================


void ProjectCoordinator::whenOutChanged()
{
  m_ExplorerModel.updateResultsAsked();

  std::vector<std::string> PagesToDelete = getSetPagesToDelete();
  BOOST_FOREACH(std::string PageToDelete,PagesToDelete)
{  m_Workspace.removePage(PageToDelete);
}

updateWorkspaceModules();

checkProject();
}

// =====================================================================
// =====================================================================


std::vector<std::string> ProjectCoordinator::getSetPagesToDelete()
{
  std::vector<std::string> SetNames;

  for (unsigned int i = 0; i
      < m_EngineProject.getOutputDescriptor().getFileSets().size(); i++)
  {
    for (unsigned int j = 0; j
        < m_EngineProject.getOutputDescriptor().getFileSets()[i].getSets().size(); j++)
    {
      SetNames.push_back(
          m_EngineProject.getOutputDescriptor().getFileSets()[i].getSets()[j].getName());
    }
  }

  std::sort(SetNames.begin(), SetNames.end());
  std::sort(m_SetPageNames.begin(), m_SetPageNames.end());

  //are in 1st but not in 2d
  std::vector<std::string> PagesToDelete;
  std::set_difference(m_SetPageNames.begin(), m_SetPageNames.end(),
      SetNames.begin(), SetNames.end(), std::back_inserter(PagesToDelete));

  return PagesToDelete;
}

// =====================================================================
// =====================================================================


void ProjectCoordinator::whenResultsChanged()
{

}

// =====================================================================
// =====================================================================


void ProjectCoordinator::whenPageRemoved(std::string RemovedPageName)
{
  // update Class Pages
  std::vector<std::string>::iterator it = std::find(m_ClassPageNames.begin(),
      m_ClassPageNames.end(), RemovedPageName);

  if (it != m_ClassPageNames.end())
  {
    m_ClassPageNames.erase(it);
    m_ModulesByPageNameMap.erase(RemovedPageName);
  }

  // update Set Pages
  it = std::find(m_SetPageNames.begin(), m_SetPageNames.end(), RemovedPageName);

  if (it != m_SetPageNames.end())
  {
    m_SetPageNames.erase(it);
    m_ModulesByPageNameMap.erase(RemovedPageName);
  }

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
  ProjectCoordinator::whenModelChanged();
}

// =====================================================================
// =====================================================================


std::vector<std::string> ProjectCoordinatorSub::getWorkspacePagesToDelete()
{
  return ProjectCoordinator::getClassPagesToDelete();
}
