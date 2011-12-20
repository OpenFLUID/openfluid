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
#define BOOST_TEST_MODULE builder_unittest_SimulOutSetsModel
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "SimulOutSetsModel.hpp"
#include "EngineProject.hpp"
#include "tests-config.hpp"

#include <openfluid/base/OutputDescriptor.hpp>
#include <openfluid/base/OutputSetDescriptor.hpp>

// =====================================================================
// =====================================================================

struct init_Model
{
    SimulOutSetsModel* mp_Model;

    init_Model()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Model = new SimulOutSetsModelImpl();
    }

    ~init_Model()
    {
      delete mp_Model;
    }
};

BOOST_FIXTURE_TEST_SUITE(SimulOutSetsModelTest, init_Model)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_getSelectedFileFormat)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor(), p_EngProject->getCoreRepository(),*p_EngProject->getModelInstance() );

  BOOST_CHECK(mp_Model->getSelectedSet() == 0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetName(),"");

  mp_Model->setSelectedSetName("wrong set name");

  BOOST_CHECK(mp_Model->getSelectedSet() == 0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetName(),"");

  mp_Model->setSelectedSetName("partial");

  openfluid::base::OutputSetDescriptor* EngineSetDesc = &(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets()[1]);

  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet(),EngineSetDesc);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetName(),"partial");

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_deleteSelectedSet)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor(), p_EngProject->getCoreRepository(),*p_EngProject->getModelInstance() );

  mp_Model->setSelectedSetName("partial");
  mp_Model->deleteSelectedSet();

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets().size(),1);
  BOOST_CHECK(mp_Model->getSelectedSet() == 0);

  mp_Model->setSelectedSetName("wrong set name");
  mp_Model->deleteSelectedSet();

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets().size(),1);
  BOOST_CHECK(mp_Model->getSelectedSet() == 0);

  mp_Model->setSelectedSetName("full");
  mp_Model->deleteSelectedSet();

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets().size(),0);
  BOOST_CHECK(mp_Model->getSelectedSet() == 0);

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_addSet)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor(), p_EngProject->getCoreRepository(),*p_EngProject->getModelInstance() );

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets().size(),2);

  p_EngProject->getOutputDescriptor().getFileSets().begin()->setName("Format #1");

  openfluid::base::OutputSetDescriptor SetDesc;
  SetDesc.setName("New Set");
  SetDesc.setUnitsClass("TestUnits");
  SetDesc.getUnitsIDs().push_back(2);
  SetDesc.getUnitsIDs().push_back(4);
  SetDesc.getUnitsIDs().push_back(6);
  SetDesc.getVariables().push_back("var A");
  SetDesc.getVariables().push_back("var B");
  SetDesc.setPrecision(3);

  mp_Model->addSet(&SetDesc,"Format #1");

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets().size(),3);

  openfluid::base::OutputSetDescriptor EngineSetDesc = p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets()[2];

  BOOST_CHECK_EQUAL(EngineSetDesc.getName(),"New Set");
  BOOST_CHECK_EQUAL(EngineSetDesc.getUnitsClass(),"TestUnits");
  BOOST_CHECK_EQUAL(EngineSetDesc.getUnitsIDs().size(),3);
  BOOST_CHECK_EQUAL(EngineSetDesc.getVariables().size(),2);
  BOOST_CHECK_EQUAL(EngineSetDesc.getPrecision(),3);

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_updateSelectedSet)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor(), p_EngProject->getCoreRepository(),*p_EngProject->getModelInstance() );

  p_EngProject->getOutputDescriptor().getFileSets().begin()->setName("Format #1");

  mp_Model->setSelectedSetName("partial");

  openfluid::base::OutputSetDescriptor PartialSetDesc = p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets()[1];
  PartialSetDesc.setName("new set name");
  PartialSetDesc.setPrecision(7);
  PartialSetDesc.setUnitsClass("ParentTestUnits");
  PartialSetDesc.setAllUnits(true);
  PartialSetDesc.getVariables().clear();

  mp_Model->updateSelectedSet(&PartialSetDesc,"Format #1");

  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet()->getName(),"new set name");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet()->getPrecision(),7);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet()->getUnitsClass(),"ParentTestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet()->isAllUnits(),true);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet()->getVariables().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetName(),"new set name");

  delete p_EngProject;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
