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
 \file ProjectCoordinator_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_ProjectCoordinator
#include <boost/test/unit_test.hpp>

//#include <openfluid/base/OutputDescriptor.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include "EngineProject.hpp"
#include "ProjectExplorerComponent.hpp"
#include "ProjectExplorerModel.hpp"
#include "ProjectExplorerView.hpp"
#include "ProjectWorkspace.hpp"
#include "ProjectDashboard.hpp"
#include "ProjectCoordinator.hpp"
#include "BuilderTestHelper.hpp"
#include "tests-config.hpp"

struct init_Coordinator
{
    EngineProject* mp_EngProject;
    ProjectExplorerComponentSub* mp_ExplorerComponent;
    ProjectExplorerModel* mp_ExplorerModel;
    ProjectWorkspaceSub* mp_Workspace;
    ProjectDashboard* mp_ProjectDashboard;
    ProjectCoordinatorSub* mp_Coordinator;

    openfluid::fluidx::AdvancedFluidXDescriptor* mp_AdvancedDesc;

    std::string m_ModelPageName;
    std::string m_DomainPageName;
    std::string m_RunPageName;
//    std::string m_OutputsPageName;

    std::string m_TestUnitsClassPageName;
    std::string m_ParentTestUnitsClassPageName;

//    std::string m_FullSetPageName;
//    std::string m_PartialSetPageName;

    init_Coordinator()
    {
      BuilderTestHelper::getInstance()->initGtk();

      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.Primitives";
      mp_EngProject = new EngineProject(Path);

      mp_AdvancedDesc = &mp_EngProject->getAdvancedDesc();

      mp_ExplorerComponent = new ProjectExplorerComponentSub(*mp_AdvancedDesc);
      mp_ExplorerModel = mp_ExplorerComponent->getModel();

      mp_Workspace = new ProjectWorkspaceSub();

      mp_ProjectDashboard = new ProjectDashboard();

      mp_Coordinator = new ProjectCoordinatorSub(*mp_ExplorerModel,
                                                 *mp_Workspace, *mp_EngProject,
                                                 *mp_ProjectDashboard);

      m_ModelPageName = mp_Coordinator->getModelPageName();
      m_DomainPageName = mp_Coordinator->getDomainPageName();
      m_RunPageName = mp_Coordinator->getRunPageName();
//      m_OutputsPageName = mp_Coordinator->getOutputsPageName();

      m_TestUnitsClassPageName = mp_Coordinator->constructClassPageName(
          "TestUnits");
      m_ParentTestUnitsClassPageName = mp_Coordinator->constructClassPageName(
          "ParentTestUnits");

//      m_FullSetPageName = mp_Coordinator->constructSetPageName("full");
//      m_PartialSetPageName = mp_Coordinator->constructSetPageName("partial");
    }

    ~init_Coordinator()
    {
      delete mp_Coordinator;
      delete mp_ProjectDashboard;
      delete mp_ExplorerComponent;
      delete mp_EngProject;
      delete mp_Workspace;
    }
};

BOOST_FIXTURE_TEST_SUITE(ProjectCoordinatorTest, init_Coordinator)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_whenActivationChanged)
{
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 0);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(), "");

  mp_ExplorerModel->setActivatedElements(
      std::make_pair(ProjectExplorerCategories::EXPLORER_MODEL, ""));
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 1);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(), m_ModelPageName);

  mp_ExplorerModel->setActivatedElements(
      std::make_pair(ProjectExplorerCategories::EXPLORER_MODEL, ""));
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 1);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(), m_ModelPageName);

  mp_Workspace->removePage(m_ModelPageName);
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 0);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(), "");

  mp_ExplorerModel->setActivatedElements(
      std::make_pair(ProjectExplorerCategories::EXPLORER_CLASS, "TestUnits"));
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 1);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),
                    m_TestUnitsClassPageName);

  mp_ExplorerModel->setActivatedElements(
      std::make_pair(ProjectExplorerCategories::EXPLORER_DOMAIN, ""));
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 2);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(), m_DomainPageName);

  mp_Workspace->removePage(m_DomainPageName);
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 1);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),
                    m_TestUnitsClassPageName);

  mp_Workspace->removePage(m_TestUnitsClassPageName);
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 0);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(), "");

//  mp_ExplorerModel->setActivatedElements(std::make_pair(ProjectExplorerCategories::EXPLORER_OUTPUTS,""));
//  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(),1);
//  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),m_OutputsPageName);

  mp_ExplorerModel->setActivatedElements(
      std::make_pair(ProjectExplorerCategories::EXPLORER_RUN, ""));
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 1);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(), m_RunPageName);

//  mp_Workspace->setCurrentPage(m_OutputsPageName);
//  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),m_OutputsPageName);

//  mp_ExplorerModel->setActivatedElements(std::make_pair(ProjectExplorerCategories::EXPLORER_SET,"full"));
//  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(),3);
//  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),m_FullSetPageName);

  mp_Workspace->removePage(m_RunPageName);
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 0);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(), "");

//  mp_Workspace->removePage(m_FullSetPageName);
//  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(),1);
//  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),m_OutputsPageName);

//  mp_Workspace->removePage(m_OutputsPageName);
//  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(),0);
//  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),"");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_removeDeletedClassPages)
{
  openfluid::fluidx::AdvancedDomainDescriptor* Domain = &mp_AdvancedDesc->getDomain();

  mp_ExplorerModel->setActivatedElements(
      std::make_pair(ProjectExplorerCategories::EXPLORER_CLASS, "TestUnits"));
  mp_ExplorerModel->setActivatedElements(
      std::make_pair(ProjectExplorerCategories::EXPLORER_CLASS,
                     "ParentTestUnits"));
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 2);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),
                    m_ParentTestUnitsClassPageName);

  Domain->deleteUnit("ParentTestUnits", 1);
  Domain->deleteUnit("ParentTestUnits", 2);

  mp_Coordinator->removeDeletedClassPages();
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 1);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),
                    m_TestUnitsClassPageName);

  for (int i = 1; i < 13; i++)
    Domain->deleteUnit("TestUnits", i);

  mp_Coordinator->removeDeletedClassPages();
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 0);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(), "");
}

//BOOST_AUTO_TEST_CASE(test_removeDeletedSetPages)
//{
//  mp_ExplorerModel->setActivatedElements(std::make_pair(ProjectExplorerCategories::EXPLORER_SET,"partial"));
//  mp_ExplorerModel->setActivatedElements(std::make_pair(ProjectExplorerCategories::EXPLORER_SET,"full"));
//  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(),2);
//  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),m_FullSetPageName);
//
//  for (unsigned int i = 0; i < mp_EngProject->getOutputDescriptor().getFileSets().size(); i++)
//  {
//    for (unsigned int j = 0; j < mp_EngProject->getOutputDescriptor().getFileSets()[i].getSets().size(); j++)
//    {
//      if (mp_EngProject->getOutputDescriptor().getFileSets()[i].getSets()[j].getName()
//          == "partial")
//      {
//        mp_EngProject->getOutputDescriptor().getFileSets()[i].getSets().erase(
//            mp_EngProject->getOutputDescriptor().getFileSets()[i].getSets().begin() + j);
//      }
//    }
//  }
//
//  mp_Coordinator->removeDeletedSetPages();
//  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(),1);
//  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),m_FullSetPageName);
//}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_whenModelChanged)
{
  Gtk::TreeModel::Children ModelRows =
      mp_ExplorerComponent->getViewSub()->getTreeView()->get_model()->children()[0].children();

//  mp_ExplorerModel->setActivatedElements(std::make_pair(ProjectExplorerCategories::EXPLORER_SET,"partial"));
//  mp_ExplorerModel->setActivatedElements(std::make_pair(ProjectExplorerCategories::EXPLORER_SET,"full"));
  mp_ExplorerModel->setActivatedElements(
      std::make_pair(ProjectExplorerCategories::EXPLORER_CLASS, "TestUnits"));
  mp_ExplorerModel->setActivatedElements(
      std::make_pair(ProjectExplorerCategories::EXPLORER_CLASS,
                     "ParentTestUnits"));
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 2);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),
                    m_ParentTestUnitsClassPageName);
  BOOST_CHECK_EQUAL(ModelRows.size(), 2);

  mp_EngProject->getAdvancedDesc().getModel().removeItem(1);

  mp_Coordinator->whenModelChanged();
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 2);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),
                    m_ParentTestUnitsClassPageName);
  BOOST_CHECK_EQUAL(ModelRows.size(), 1);

  mp_EngProject->getAdvancedDesc().getModel().removeItem(0);

  mp_Coordinator->whenModelChanged();
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 2);
//  BOOST_CHECK_EQUAL(mp_Workspace->existsPageName(m_FullSetPageName),false);
//  BOOST_CHECK_EQUAL(mp_Workspace->existsPageName(m_PartialSetPageName),false);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),
                    m_ParentTestUnitsClassPageName);
  BOOST_CHECK_EQUAL(ModelRows.size(), 0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_whenDomainChanged)
{
  openfluid::fluidx::AdvancedDomainDescriptor* Domain = &mp_AdvancedDesc->getDomain();

  Gtk::TreeModel::Children DomainRows =
      mp_ExplorerComponent->getViewSub()->getTreeView()->get_model()->children()[1].children();

//  mp_ExplorerModel->setActivatedElements(std::make_pair(ProjectExplorerCategories::EXPLORER_SET,"partial"));
//  mp_ExplorerModel->setActivatedElements(std::make_pair(ProjectExplorerCategories::EXPLORER_SET,"full"));
  mp_ExplorerModel->setActivatedElements(
      std::make_pair(ProjectExplorerCategories::EXPLORER_CLASS, "TestUnits"));
  mp_ExplorerModel->setActivatedElements(
      std::make_pair(ProjectExplorerCategories::EXPLORER_CLASS,
                     "ParentTestUnits"));
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 2);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),
                    m_ParentTestUnitsClassPageName);
  BOOST_CHECK_EQUAL(DomainRows.size(), 2);

  Domain->deleteUnit("TestUnits", 1);
  Domain->deleteUnit("TestUnits", 5);

  mp_Coordinator->whenDomainChanged();
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 2);
//  BOOST_CHECK_EQUAL(mp_Workspace->existsPageName(m_PartialSetPageName),false);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),
                    m_ParentTestUnitsClassPageName);
  BOOST_CHECK_EQUAL(DomainRows.size(), 2);

  Domain->deleteUnit("ParentTestUnits", 1);
  Domain->deleteUnit("ParentTestUnits", 2);

  mp_Coordinator->whenDomainChanged();
  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(), 1);
  BOOST_CHECK_EQUAL(
      mp_Workspace->existsPageName(m_ParentTestUnitsClassPageName), false);
  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),
                    m_TestUnitsClassPageName);
  BOOST_CHECK_EQUAL(DomainRows.size(), 1);
}
//BOOST_AUTO_TEST_CASE(test_whenOutChanged)
//{
//  mp_ExplorerModel->setActivatedElements(std::make_pair(ProjectExplorerCategories::EXPLORER_SET,"partial"));
//  mp_ExplorerModel->setActivatedElements(std::make_pair(ProjectExplorerCategories::EXPLORER_SET,"full"));
//  mp_ExplorerModel->setActivatedElements(std::make_pair(ProjectExplorerCategories::EXPLORER_CLASS,"TestUnits"));
//  mp_ExplorerModel->setActivatedElements(std::make_pair(ProjectExplorerCategories::EXPLORER_CLASS,"ParentTestUnits"));
//  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(),4);
//  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),m_ParentTestUnitsClassPageName);
//
//  for (unsigned int i = 0; i < mp_EngProject->getOutputDescriptor().getFileSets().size(); i++)
//  {
//    for (unsigned int j = 0; j < mp_EngProject->getOutputDescriptor().getFileSets()[i].getSets().size(); j++)
//    {
//      if (mp_EngProject->getOutputDescriptor().getFileSets()[i].getSets()[j].getName()
//          == "partial")
//      {
//        mp_EngProject->getOutputDescriptor().getFileSets()[i].getSets().erase(
//            mp_EngProject->getOutputDescriptor().getFileSets()[i].getSets().begin() + j);
//      }
//    }
//  }
//
//  mp_Coordinator->whenOutChanged();
//  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(),3);
//  BOOST_CHECK_EQUAL(mp_Workspace->existsPageName(m_PartialSetPageName),false);
//  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),m_ParentTestUnitsClassPageName);
//
//  mp_EngProject->getOutputDescriptor().getFileSets()[0].getSets().erase(
//      mp_EngProject->getOutputDescriptor().getFileSets()[0].getSets().begin());
//
//  mp_Coordinator->whenOutChanged();
//  BOOST_CHECK_EQUAL(mp_Workspace->getPagesCount(),2);
//  BOOST_CHECK_EQUAL(mp_Workspace->existsPageName(m_FullSetPageName),false);
//  BOOST_CHECK_EQUAL(mp_Workspace->getCurrentPageName(),m_ParentTestUnitsClassPageName);
//}
// =====================================================================
// =====================================================================
BOOST_AUTO_TEST_SUITE_END();
