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
#define BOOST_TEST_MODULE builder_unittest_ModelStructureModel
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "ModelStructureModel.hpp"
#include "EngineProject.hpp"
#include "tests-config.hpp"
#include "ModelItemInstanceFactory.hpp"

#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/PluginManager.hpp>

// =====================================================================
// =====================================================================

struct init_Presenter
{
    ModelStructureModelImpl* mp_Model;

    init_Presenter()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Model = new ModelStructureModelImpl();
    }

    ~init_Presenter()
    {
      delete mp_Model;
    }
};

BOOST_FIXTURE_TEST_SUITE(ModelStructureModelTest, init_Presenter)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_getModelInstance)
{
  BOOST_CHECK_THROW(mp_Model->getModelInstance(),openfluid::base::OFException);

  EngineProject* EngProject = new EngineProject();
  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  BOOST_CHECK_EQUAL(mp_Model->getModelInstance(),EngProject->getModelInstance());

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_setEmptyModelInstance)
{
  EngineProject* EngProject = new EngineProject();
  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  BOOST_CHECK_EQUAL(mp_Model->getFctCount(),0);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_setNonEmptyModelInstance)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = new EngineProject(Path);
  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  BOOST_CHECK_EQUAL(mp_Model->getFctCount(),2);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_AppendAnUnavailableFunction)
{
  EngineProject* EngProject = new EngineProject();
  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  openfluid::machine::ModelItemInstance* Item = 0;

  mp_Model->appendFunction(Item);

  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),-1);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_AppendARegularFunction)
{
  EngineProject* EngProject = new EngineProject();
  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  openfluid::machine::SignatureItemInstance FctSignature =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(
      "tests.primitives.use");

  openfluid::machine::ModelItemInstance* Item = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature);

  mp_Model->appendFunction(Item);

  BOOST_CHECK_EQUAL(mp_Model->getFctCount(),1);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_RemoveAFunction)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  openfluid::machine::SignatureItemInstance FctSignature =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(
      "tests.vector.prod");
  openfluid::machine::ModelItemInstance* Item = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature);

  mp_Model->appendFunction(Item);

  //remove in the middle
  mp_Model->removeFunctionAt(1);

  BOOST_CHECK_EQUAL(mp_Model->getFctCount(),2);

  //remove first
  mp_Model->removeFunctionAt(0);

  BOOST_CHECK_EQUAL(mp_Model->getFctCount(),1);

  //remove the last one
  mp_Model->removeFunctionAt(0);

  BOOST_CHECK_EQUAL(mp_Model->getFctCount(),0);

  // add three new functions
  openfluid::machine::SignatureItemInstance FctSignature1 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(
      "tests.vector.prod");
  openfluid::machine::ModelItemInstance* Item1 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature1);

  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(
      "tests.vector.use");
  openfluid::machine::ModelItemInstance* Item2 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature2);

  openfluid::machine::SignatureItemInstance FctSignature3 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(
      "tests.primitives.prod");
  openfluid::machine::ModelItemInstance* Item3 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature3);

  mp_Model->appendFunction(Item1);
  mp_Model->appendFunction(Item2);
  mp_Model->appendFunction(Item3);

  // remove last
  mp_Model->removeFunctionAt(2);

  BOOST_CHECK_EQUAL(mp_Model->getFctCount(),2);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_MoveAFunction)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  std::string idA = "tests.primitives.prod";
  std::string idB = "tests.primitives.use";
  std::string idC = "tests.vector.prod";

  openfluid::machine::SignatureItemInstance FctSignature1 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idA);
  openfluid::machine::ModelItemInstance* Item1 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature1);

  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idB);
  openfluid::machine::ModelItemInstance* Item2 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature2);

  openfluid::machine::SignatureItemInstance FctSignature3 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idC);
  openfluid::machine::ModelItemInstance* Item3 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature3);

  mp_Model->appendFunction(Item1);
  mp_Model->appendFunction(Item2);
  mp_Model->appendFunction(Item3);

  std::list<openfluid::machine::ModelItemInstance*>::const_iterator it;

  // move to lower
  mp_Model->moveFunction(2,1);
  it = mp_Model->getModelInstance()->getItems().begin();
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idA);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idC);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idB);

  // move to upper
  mp_Model->moveFunction(0,1);
  it = mp_Model->getModelInstance()->getItems().begin();
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idC);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idA);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idB);

  // bad To index, throw "Bad indexes of items to move"
  BOOST_CHECK_THROW(mp_Model->moveFunction(0,3),openfluid::base::OFException);

  // bad From index, throw "Bad indexes of items to move"
  BOOST_CHECK_THROW(mp_Model->moveFunction(-1,1),openfluid::base::OFException);

  // does nothing
  mp_Model->moveFunction(1,1);
  it = mp_Model->getModelInstance()->getItems().begin();
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idC);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idA);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idB);

  // move to top
  mp_Model->moveFunction(0,2);
  it = mp_Model->getModelInstance()->getItems().begin();
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idA);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idB);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idC);

  // move to the bottom
  mp_Model->moveFunction(2,0);
  it = mp_Model->getModelInstance()->getItems().begin();
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idC);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idA);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idB);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_moveTowardTheBegin)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  std::string idA = "tests.primitives.prod";
  std::string idB = "tests.primitives.use";
  std::string idC = "tests.vector.prod";

  openfluid::machine::SignatureItemInstance FctSignature1 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idA);
  openfluid::machine::ModelItemInstance* Item1 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature1);

  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idB);
  openfluid::machine::ModelItemInstance* Item2 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature2);

  openfluid::machine::SignatureItemInstance FctSignature3 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idC);
  openfluid::machine::ModelItemInstance* Item3 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature3);

  mp_Model->appendFunction(Item1);
  mp_Model->appendFunction(Item2);
  mp_Model->appendFunction(Item3);

  std::list<openfluid::machine::ModelItemInstance*>::const_iterator it;

  //select and move
  mp_Model->setCurrentSelectionByUserAt(2);
  mp_Model->moveTowardTheBegin();
  it = mp_Model->getModelInstance()->getItems().begin();
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idA);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idC);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idB);

  //select and move (circular)
  mp_Model->setCurrentSelectionByUserAt(0);
  mp_Model->moveTowardTheBegin();
  it = mp_Model->getModelInstance()->getItems().begin();
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idC);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idB);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idA);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_moveTowardTheEnd)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  std::string idA = "tests.primitives.prod";
  std::string idB = "tests.primitives.use";
  std::string idC = "tests.vector.prod";

  openfluid::machine::SignatureItemInstance FctSignature1 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idA);
  openfluid::machine::ModelItemInstance* Item1 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature1);

  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idB);
  openfluid::machine::ModelItemInstance* Item2 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature2);

  openfluid::machine::SignatureItemInstance FctSignature3 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idC);
  openfluid::machine::ModelItemInstance* Item3 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature3);

  mp_Model->appendFunction(Item1);
  mp_Model->appendFunction(Item2);
  mp_Model->appendFunction(Item3);

  std::list<openfluid::machine::ModelItemInstance*>::const_iterator it;

  //select and move
  mp_Model->setCurrentSelectionByUserAt(0);
  mp_Model->moveTowardTheEnd();
  it = mp_Model->getModelInstance()->getItems().begin();
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idB);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idA);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idC);

  //select and move (circular)
  mp_Model->setCurrentSelectionByUserAt(2);
  mp_Model->moveTowardTheEnd();
  it = mp_Model->getModelInstance()->getItems().begin();
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idC);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idB);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idA);

  delete EngProject;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_getCurrentSelection)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  mp_Model->setCurrentSelectionByUserAt(1);

  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelectionSignature()->Signature->ID,"tests.primitives.use");

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_requestSelectionByApp)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  std::string idA = "tests.primitives.prod";
  std::string idB = "tests.primitives.use";
  std::string idC = "tests.vector.prod";

  openfluid::machine::SignatureItemInstance FctSignature1 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idA);
  openfluid::machine::ModelItemInstance* Item1 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature1);

  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idB);
  openfluid::machine::ModelItemInstance* Item2 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature2);

  openfluid::machine::SignatureItemInstance FctSignature3 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idC);
  openfluid::machine::ModelItemInstance* Item3 = ModelItemInstanceFactory::createPluggableItemFromSignature(FctSignature3);

  mp_Model->appendFunction(Item1);
  mp_Model->appendFunction(Item2);
  mp_Model->appendFunction(Item3);

  mp_Model->requestSelectionByApp(idB);
  BOOST_CHECK_EQUAL(mp_Model->getAppRequestedSelection(),1);

  mp_Model->requestSelectionByApp("idD");
  BOOST_CHECK_EQUAL(mp_Model->getAppRequestedSelection(),-1);

  mp_Model->requestSelectionByApp(idC);
  BOOST_CHECK_EQUAL(mp_Model->getAppRequestedSelection(),2);

  mp_Model->requestSelectionByApp(idA);
  BOOST_CHECK_EQUAL(mp_Model->getAppRequestedSelection(),0);

  delete EngProject;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();

