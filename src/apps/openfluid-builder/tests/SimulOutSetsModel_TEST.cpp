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
#include "EngineProjectFactory.hpp"
#include "tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_Model
{
    SimulOutSetsModelSub* mp_Model;

    init_Model()
    {
      BuilderTestHelper::getInstance()->initGtk();

      openfluid::base::RuntimeEnvironment::getInstance()->addExtraPluginsPaths(CONFIGTESTS_OUTPUT_BINARY_DIR);

      mp_Model = new SimulOutSetsModelSub();
    }

    ~init_Model()
    {
      delete mp_Model;
    }
};

BOOST_FIXTURE_TEST_SUITE(SimulOutSetsModelTest, init_Model)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_setEngineRequirements)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());

  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().size(),2);

  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().begin()->first,"full");
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().begin()->second.first,"Format #1");
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().begin()->second.second.getName(),"full");
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().begin()->second.second.getUnitsClass(),"TestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().begin()->second.second.isAllUnits(),true);
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().begin()->second.second.isAllVectors(),true);
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().begin()->second.second.isAllScalars(),true);
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().begin()->second.second.getPrecision(),5);

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_getSelectedFileFormat)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());

  BOOST_CHECK(mp_Model->getSelectedSet() == 0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetFormatName(),"");

  mp_Model->setSelectedSetName("wrong set name");

  BOOST_CHECK(mp_Model->getSelectedSet() == 0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetFormatName(),"");

  mp_Model->setSelectedSetName("partial");

  openfluid::base::OutputSetDescriptor* EngineSetDesc = &(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets()[1]);

  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet(),EngineSetDesc);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetFormatName(),"Format #1");

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_deleteSelectedSet)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().size(),2);

  mp_Model->setSelectedSetName("partial");
  mp_Model->deleteSelectedSet();

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().size(),1);
  BOOST_CHECK(mp_Model->getSelectedSet() == 0);

  mp_Model->setSelectedSetName("wrong set name");
  mp_Model->deleteSelectedSet();

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().size(),1);
  BOOST_CHECK(mp_Model->getSelectedSet() == 0);

  mp_Model->setSelectedSetName("full");
  mp_Model->deleteSelectedSet();

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().size(),0);
  BOOST_CHECK(mp_Model->getSelectedSet() == 0);

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_addSet)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().size(),2);

  openfluid::base::OutputSetDescriptor SetDesc;
  SetDesc.setName("New Set");
  SetDesc.setUnitsClass("TestUnits");
  SetDesc.getUnitsIDs().push_back(2);
  SetDesc.getUnitsIDs().push_back(4);
  SetDesc.getUnitsIDs().push_back(6);
  SetDesc.getScalars().push_back("var A");
  SetDesc.getScalars().push_back("var B");
  SetDesc.setAllVectors(true);
  SetDesc.setPrecision(3);

  mp_Model->addSet(&SetDesc,"Format #1",0);

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets().size(),3);
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().size(),3);

  openfluid::base::OutputSetDescriptor EngineSetDesc = p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets()[2];

  BOOST_CHECK_EQUAL(EngineSetDesc.getName(),"New Set");
  BOOST_CHECK_EQUAL(EngineSetDesc.getUnitsClass(),"TestUnits");
  BOOST_CHECK_EQUAL(EngineSetDesc.getUnitsIDs().size(),3);
  BOOST_CHECK_EQUAL(EngineSetDesc.getScalars().size(),2);
  BOOST_CHECK_EQUAL(EngineSetDesc.isAllVectors(),true);
  BOOST_CHECK_EQUAL(EngineSetDesc.getPrecision(),3);

  BOOST_CHECK_EQUAL(mp_Model->getSetsByName()["New Set"].first,"Format #1");
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName()["New Set"].second.getName(),"New Set");
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName()["New Set"].second.getUnitsClass(),"TestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName()["New Set"].second.getUnitsIDs().size(),3);
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName()["New Set"].second.getScalars().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName()["New Set"].second.isAllVectors(),true);
  BOOST_CHECK_EQUAL(mp_Model->getSetsByName()["New Set"].second.getPrecision(),3);

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_updateSelectedSet)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());

  mp_Model->setSelectedSetName("partial");

  openfluid::base::OutputSetDescriptor* PartialSetDesc = &(p_EngProject->getOutputDescriptor().getFileSets().begin()->getSets()[1]);
  PartialSetDesc->setPrecision(7);
  PartialSetDesc->setUnitsClass("ParentTestUnits");
  PartialSetDesc->setAllUnits(true);
  PartialSetDesc->getScalars().clear();
  PartialSetDesc->getVectors().clear();

  mp_Model->updateSelectedSet("New Format");

  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet()->getName(),"partial");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet()->getPrecision(),7);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet()->getUnitsClass(),"ParentTestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet()->isAllUnits(),true);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet()->getScalars().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSet()->getVectors().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetFormatName(),"New Format");

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_updateFileFormats)
{
//  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
//  + "/OPENFLUID.IN.Primitives";
//  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);
//
//  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());
//  mp_Model->setSelectedSetName("partial");
//
//  SimulOutFilesModelImpl FilesModel;
//  FilesModel.setEngineRequirements(p_EngProject->getOutputDescriptor());
//  openfluid::base::OutputFilesDescriptor FileDesc;
//  openfluid::base::OutputSetDescriptor SetDesc1;
//  openfluid::base::OutputSetDescriptor SetDesc2;
//  openfluid::base::OutputSetDescriptor SetDesc3;
//  FileDesc.getSets().push_back(SetDesc1);
//  FileDesc.getSets().push_back(SetDesc2);
//  FileDesc.getSets().push_back(SetDesc3);
//  FilesModel.addFileFormat(&FileDesc,"New Format");
//
//  mp_Model->addSet()
//
//  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().size(),3);
//
//  mp_Model->updateFileFormats(FilesModel.getFilesFormatsByNameVect());
//
//  BOOST_CHECK_EQUAL(mp_Model->getSetsByName().size(),1);
//
//  delete p_EngProject;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
