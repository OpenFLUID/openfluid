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
 \file ProjectExplorerPresenter_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_ProjectExplorerPresenter
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "ProjectExplorerComponent.hpp"
#include "ProjectExplorerModel.hpp"
#include "ProjectExplorerView.hpp"
#include "EngineProject.hpp"
#include "tests-config.hpp"

#include <openfluid/fluidx/FluidXDescriptor.hpp>

// =====================================================================
// =====================================================================

struct init_Presenter
{
    ProjectExplorerColumns m_Columns;

    init_Presenter()
    {
      BuilderTestHelper::getInstance()->initGtk();
    }
};

BOOST_FIXTURE_TEST_SUITE(ProjectExplorerPresenterTest, init_Presenter)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_setEmptyEngineRequirements)
{
  EngineProject* EngProject = new EngineProject();

  ProjectExplorerComponent* Component = new ProjectExplorerComponent(
      EngProject->getAdvancedDesc());
  ProjectExplorerModelImpl* Model =
      (ProjectExplorerModelImpl*) (Component->getModel());
  ProjectExplorerViewSub* View =
      (ProjectExplorerViewSub*) (Component->getView());

  BOOST_CHECK_EQUAL(Model->getActivatedElement().first,
                    ProjectExplorerCategories::EXPLORER_NONE);
  BOOST_CHECK_EQUAL(Model->getActivatedElement().second, "");

  Gtk::TreeView* TreeView = View->getTreeView();

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().size(), 4);

  delete EngProject;
  delete Component;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_setNotEmptyEngineRequirements)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
      + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = new EngineProject(Path);

  ProjectExplorerComponent* Component = new ProjectExplorerComponent(
      EngProject->getAdvancedDesc());
  ProjectExplorerModelImpl* Model =
      (ProjectExplorerModelImpl*) (Component->getModel());
  ProjectExplorerViewSub* View =
      (ProjectExplorerViewSub*) (Component->getView());

  BOOST_CHECK_EQUAL(Model->getActivatedElement().first,
                    ProjectExplorerCategories::EXPLORER_NONE);
  BOOST_CHECK_EQUAL(Model->getActivatedElement().second, "");

  Gtk::TreeView* TreeView = View->getTreeView();

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().size(), 4);

  delete EngProject;
  delete Component;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_activateRows)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
      + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = new EngineProject(Path);

  ProjectExplorerComponent* Component = new ProjectExplorerComponent(
      EngProject->getAdvancedDesc());
  ProjectExplorerModelImpl* Model =
      (ProjectExplorerModelImpl*) (Component->getModel());
  ProjectExplorerViewSub* View =
      (ProjectExplorerViewSub*) (Component->getView());

  Gtk::TreeView* TreeView = View->getTreeView();

  // Model activated
  TreeView->row_activated(
      TreeView->get_model()->get_path(TreeView->get_model()->children()[0]),
      *TreeView->get_column(0));

  BOOST_CHECK_EQUAL(Model->getActivatedElement().first,
                    ProjectExplorerCategories::EXPLORER_MODEL);
  BOOST_CHECK_EQUAL(Model->getActivatedElement().second, "");

  // Domain activated
  TreeView->row_activated(
      TreeView->get_model()->get_path(TreeView->get_model()->children()[1]),
      *TreeView->get_column(0));

  BOOST_CHECK_EQUAL(Model->getActivatedElement().first,
                    ProjectExplorerCategories::EXPLORER_DOMAIN);
  BOOST_CHECK_EQUAL(Model->getActivatedElement().second, "");

  // Simulation activated
  TreeView->row_activated(
      TreeView->get_model()->get_path(TreeView->get_model()->children()[2]),
      *TreeView->get_column(0));

  BOOST_CHECK_EQUAL(Model->getActivatedElement().first,
                    ProjectExplorerCategories::EXPLORER_RUN);
  BOOST_CHECK_EQUAL(Model->getActivatedElement().second, "");

  // Monitoring activated
  TreeView->row_activated(
       TreeView->get_model()->get_path(TreeView->get_model()->children()[3]),
       *TreeView->get_column(0));

   BOOST_CHECK_EQUAL(Model->getActivatedElement().first,
                     ProjectExplorerCategories::EXPLORER_MONITORING);
   BOOST_CHECK_EQUAL(Model->getActivatedElement().second, "");

// First Function activated
  TreeView->row_activated(
      TreeView->get_model()->get_path(
          TreeView->get_model()->children()[0]->children()[0]),
      *TreeView->get_column(0));

  BOOST_CHECK_EQUAL(Model->getActivatedElement().first,
                    ProjectExplorerCategories::EXPLORER_MODEL);
  BOOST_CHECK_EQUAL(Model->getActivatedElement().second,
                    "tests.primitives.prod");

  // First Class activated
  TreeView->row_activated(
      TreeView->get_model()->get_path(
          TreeView->get_model()->children()[1]->children()[0]),
      *TreeView->get_column(0));

  BOOST_CHECK_EQUAL(Model->getActivatedElement().first,
                    ProjectExplorerCategories::EXPLORER_CLASS);
  BOOST_CHECK_EQUAL(Model->getActivatedElement().second, "ParentTestUnits");

  // Run activated
  TreeView->row_activated(
      TreeView->get_model()->get_path(
          TreeView->get_model()->children()[2]->children()[0]),
      *TreeView->get_column(0));

  BOOST_CHECK_EQUAL(Model->getActivatedElement().first,
                    ProjectExplorerCategories::EXPLORER_RUN);
  BOOST_CHECK_EQUAL(Model->getActivatedElement().second, "");

  // Run Info activated
  TreeView->row_activated(
      TreeView->get_model()->get_path(
          TreeView->get_model()->children()[2]->children()[0]->children()[0]),
      *TreeView->get_column(0));

  BOOST_CHECK_EQUAL(Model->getActivatedElement().first,
                    ProjectExplorerCategories::EXPLORER_RUN);
  BOOST_CHECK_EQUAL(Model->getActivatedElement().second, "");

  delete EngProject;
  delete Component;
}
// =====================================================================
// =====================================================================
BOOST_AUTO_TEST_SUITE_END()

