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

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_SimulOutSetsPresenter
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "SimulOutSetsComponent.hpp"
#include "SimulOutSetsModel.hpp"
#include "SimulOutSetsView.hpp"
#include "SimulOutSetsAdapterModel.hpp"
#include "SimulOutSetsColumns.hpp"
#include "EngineProject.hpp"
#include "tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_Presenter
{
    SimulOutSetsComponent* mp_Component;

    SimulOutSetsModel* mp_Model;
    SimulOutSetsViewSub* mp_View;
    SimulOutSetsAdapterModelSub * mp_AdapterModel;

    SimulOutSetsColumns m_Columns;

    init_Presenter()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Component = new SimulOutSetsComponent();
      mp_Model = mp_Component->getModel();
      mp_View = (SimulOutSetsViewSub*) (mp_Component->getView());
      mp_AdapterModel
          = (SimulOutSetsAdapterModelSub*) (mp_Component->getAdapterModel());
    }

    ~init_Presenter()
    {
      delete mp_Component;
    }
};

BOOST_FIXTURE_TEST_SUITE(SimulOutSetsPresenterTest, init_Presenter)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_getStringListFromVect)
{
  std::vector<int> Vect;

  Vect.push_back(9);
  Vect.push_back(8);
  Vect.push_back(7);
  Vect.push_back(6);

  BOOST_CHECK_EQUAL(mp_AdapterModel->getStringListFromVect(Vect),"9;8;7;6");
}

BOOST_AUTO_TEST_CASE(test_getStringListFromVectEmpty)
{
  std::vector<int> Vect;

  BOOST_CHECK_EQUAL(mp_AdapterModel->getStringListFromVect(Vect),"");
}

BOOST_AUTO_TEST_CASE(test_getStringListFromVectOneItem)
{
  std::vector<int> Vect;

  Vect.push_back(9);

  BOOST_CHECK_EQUAL(mp_AdapterModel->getStringListFromVect(Vect),"9");
}

BOOST_AUTO_TEST_CASE(test_getStringListFromStringSet)
{
  std::set<std::string> Set;

  BOOST_CHECK_EQUAL(mp_AdapterModel->getStringListFromStringSet(Set),"");

  Set.insert("VarA");

  BOOST_CHECK_EQUAL(mp_AdapterModel->getStringListFromStringSet(Set),"VarA");

  Set.insert("VarB[]");

  BOOST_CHECK_EQUAL(mp_AdapterModel->getStringListFromStringSet(Set),"VarA;VarB[]");
}

BOOST_AUTO_TEST_CASE(test_extractIDsString)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor(),
      p_EngProject->getCoreRepository(),
      *p_EngProject->getModelInstance());

  std::vector<int> Vect;

  BOOST_CHECK_EQUAL(mp_AdapterModel->extractIDsString(p_EngProject->getOutputDescriptor().getFileSets()[0].getSets()[0]),"*");
  BOOST_CHECK_EQUAL(mp_AdapterModel->extractIDsString(p_EngProject->getOutputDescriptor().getFileSets()[0].getSets()[1]),"1;5");

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_extractVariablesString)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor(),
      p_EngProject->getCoreRepository(),
      *p_EngProject->getModelInstance());

  std::vector<int> Vect;

  BOOST_CHECK_EQUAL(mp_AdapterModel->extractVariablesString(p_EngProject->getOutputDescriptor().getFileSets()[0].getSets()[0], p_EngProject->getModelInstance()),"*");

  p_EngProject->getOutputDescriptor().getFileSets()[0].getSets()[0].setAllVariables(false);
  p_EngProject->getOutputDescriptor().getFileSets()[0].getSets()[0].getVariables().push_back("var A");
  p_EngProject->getOutputDescriptor().getFileSets()[0].getSets()[0].getVariables().push_back("var X");
  p_EngProject->getOutputDescriptor().getFileSets()[0].getSets()[0].getVariables().push_back("var B");
  p_EngProject->getOutputDescriptor().getFileSets()[0].getSets()[0].getVariables().push_back("var Y");

  BOOST_CHECK_EQUAL(mp_AdapterModel->extractVariablesString(p_EngProject->getOutputDescriptor().getFileSets()[0].getSets()[0], p_EngProject->getModelInstance()),"var A;var X;var B;var Y");

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_SetOutDescriptor)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = new EngineProject(Path);

  p_EngProject->getOutputDescriptor().getFileSets().begin()->setName("Format #1");

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor(),
      p_EngProject->getCoreRepository(),
      *p_EngProject->getModelInstance());

  Gtk::TreeView* TreeView = mp_View->getTreeView();

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().size(),2);
  BOOST_CHECK_EQUAL(TreeView->get_columns().size(),6);

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().begin()->get_value(m_Columns.m_Name),"full");

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().begin()->get_value(m_Columns.m_UnitsClass),"TestUnits");

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().begin()->get_value(m_Columns.m_UnitsIDs),"*");

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().begin()->get_value(m_Columns.m_Vars),"*");

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().begin()->get_value(m_Columns.m_FormatName),"Format #1");

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().begin()->get_value(m_Columns.m_Precision),5);

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_selectSet)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor(),
      p_EngProject->getCoreRepository(),
      *p_EngProject->getModelInstance());

  BOOST_CHECK(mp_Model->getSelectedSet() == 0);

  mp_View->setSelectedRow(*mp_AdapterModel->getTreeModel()->children()[1]);

  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet(),&p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets()[1]);

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_deleteSet)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor(),
      p_EngProject->getCoreRepository(),
      *p_EngProject->getModelInstance());

  mp_View->setSelectedRow(*mp_AdapterModel->getTreeModel()->children()[1]);

  mp_Model->deleteSelectedSet();

  Gtk::TreeView* TreeView = mp_View->getTreeView();
  BOOST_CHECK_EQUAL(TreeView->get_model()->children().size(),1);

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_addSet)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = new EngineProject(Path);

  p_EngProject->getOutputDescriptor().getFileSets().begin()->setName("Format #1");

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor(),
      p_EngProject->getCoreRepository(),
      *p_EngProject->getModelInstance());

  Gtk::TreeView* TreeView = mp_View->getTreeView();

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().size(),2);

  openfluid::base::OutputSetDescriptor SetDesc;
  SetDesc.setName("ZZNew Set"); // ZZ for this set to be in last position
  SetDesc.setUnitsClass("TestUnits");
  SetDesc.getUnitsIDs().push_back(2);
  SetDesc.getUnitsIDs().push_back(4);
  SetDesc.getUnitsIDs().push_back(6);
  SetDesc.getVariables().push_back("var A");
  SetDesc.getVariables().push_back("var B");
  SetDesc.setPrecision(3);

  mp_Model->addSet(&SetDesc,"Format #1");

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().size(),3);

  Gtk::TreeModel::Children::iterator it = TreeView->get_model()->children().begin();
  std::advance(it,2);

  BOOST_CHECK_EQUAL(it->get_value(m_Columns.m_Name),"ZZNew Set");

  BOOST_CHECK_EQUAL(it->get_value(m_Columns.m_UnitsClass),"TestUnits");

  BOOST_CHECK_EQUAL(it->get_value(m_Columns.m_UnitsIDs),"2;4;6");

  BOOST_CHECK_EQUAL(it->get_value(m_Columns.m_Vars),"var A;var B");

  BOOST_CHECK_EQUAL(it->get_value(m_Columns.m_FormatName),"Format #1");

  BOOST_CHECK_EQUAL(it->get_value(m_Columns.m_Precision),3);

  delete p_EngProject;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END()
