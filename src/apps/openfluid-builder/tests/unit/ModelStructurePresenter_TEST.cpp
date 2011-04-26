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
#define BOOST_TEST_MODULE builder_unittest_ModelStructurePresenter
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "ModelStructureComponent.hpp"
#include "ModelStructureModel.hpp"
#include "ModelStructureView.hpp"
#include "EngineProject.hpp"
#include "tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_Presenter
{
    ModelStructureComponent* mp_Component;

    ModelStructureModel* mp_Model;
    ModelStructureViewSub* mp_View;

    init_Presenter()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Component = new ModelStructureComponent();
      mp_Model = mp_Component->getModel();
      mp_View = (ModelStructureViewSub*) (mp_Component->getView());
    }

    ~init_Presenter()
    {
      delete mp_Component;
    }
};

BOOST_FIXTURE_TEST_SUITE(ModelStructurePresenterTest, init_Presenter)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_SetModelInstance)
{
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),-1);
  BOOST_CHECK_EQUAL(mp_View->getRowNb(),0);

  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  std::string idA = "tests.primitives.prod";
  std::string idB = "tests.primitives.use";
  std::string idC = "tests.vector.prod";

  openfluid::machine::SignatureItemInstance FctSignature1 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idA);
  FctSignature1.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idB);
  FctSignature2.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature3 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idC);
  FctSignature3.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  mp_Model->appendFunction(FctSignature1);
  mp_Model->appendFunction(FctSignature2);
  mp_Model->appendFunction(FctSignature3);

  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),2);
  BOOST_CHECK_EQUAL(mp_View->getRowNb(),3);
  BOOST_CHECK_EQUAL(mp_View->getRowId(0),"1. " + idA);
  BOOST_CHECK_EQUAL(mp_View->getRowId(1),"2. " + idB);
  BOOST_CHECK_EQUAL(mp_View->getRowId(2),"3. " + idC);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_SelectAFunctionByApp)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  std::string idA = "tests.primitives.prod";
  std::string idB = "tests.primitives.use";
  std::string idC = "tests.vector.prod";

  openfluid::machine::SignatureItemInstance FctSignature1 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idA);
  FctSignature1.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idB);
  FctSignature2.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature3 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idC);
  FctSignature3.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  mp_Model->appendFunction(FctSignature1);
  mp_Model->appendFunction(FctSignature2);
  mp_Model->appendFunction(FctSignature3);

  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),2);

  mp_Model->requestSelectionByAppAt(0);
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),0);

  mp_Model->requestSelectionByAppAt(1);
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),1);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_AppendAFunction)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  // append a function
  std::string idA = "tests.primitives.prod";
  openfluid::machine::SignatureItemInstance FctSignature =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idA);
  FctSignature.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  mp_Model->appendFunction(FctSignature);

  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),0);
  BOOST_CHECK_EQUAL(mp_View->getRowNb(),1);
  BOOST_CHECK_EQUAL(mp_View->getRowId(0),"1. " + idA);
  BOOST_CHECK_EQUAL(mp_View->getSelectedRowPosition(),0);

  // append another function
  std::string idB = "tests.primitives.use";
  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idB);
  FctSignature2.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  mp_Model->appendFunction(FctSignature2);

  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),1);
  BOOST_CHECK_EQUAL(mp_View->getRowNb(),2);
  BOOST_CHECK_EQUAL(mp_View->getRowId(0),"1. " + idA);
  BOOST_CHECK_EQUAL(mp_View->getRowId(1),"2. " + idB);
  BOOST_CHECK_EQUAL(mp_View->getSelectedRowPosition(),1);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_RemoveFunctionAt)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  std::string idA = "tests.primitives.prod";
  std::string idB = "tests.primitives.use";
  std::string idC = "tests.vector.prod";

  openfluid::machine::SignatureItemInstance FctSignature1 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idA);
  FctSignature1.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idB);
  FctSignature2.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature3 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idC);
  FctSignature3.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  mp_Model->appendFunction(FctSignature1);
  mp_Model->appendFunction(FctSignature2);
  mp_Model->appendFunction(FctSignature3);

  //remove in the middle
  mp_Model->removeFunctionAt(1);

  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),1);
  BOOST_CHECK_EQUAL(mp_View->getRowNb(),2);
  BOOST_CHECK_EQUAL(mp_View->getRowId(0),"1. " + idA);
  BOOST_CHECK_EQUAL(mp_View->getRowId(1),"2. " + idC);

  //remove first
  mp_Model->removeFunctionAt(0);

  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),0);
  BOOST_CHECK_EQUAL(mp_View->getRowNb(),1);
  BOOST_CHECK_EQUAL(mp_View->getRowId(0),"1. " + idC);

  //remove the last one
  mp_Model->removeFunctionAt(0);

  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),-1);
  BOOST_CHECK_EQUAL(mp_View->getRowNb(),0);

  // add three new functions
  mp_Model->appendFunction(FctSignature1);
  mp_Model->appendFunction(FctSignature2);
  mp_Model->appendFunction(FctSignature3);

  // remove last
  mp_Model->removeFunctionAt(2);

  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),1);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_RemoveFirst)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  std::string idA = "tests.primitives.prod";
  std::string idB = "tests.primitives.use";
  std::string idC = "tests.vector.prod";

  openfluid::machine::SignatureItemInstance FctSignature1 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idA);
  FctSignature1.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idB);
  FctSignature2.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature3 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idC);
  FctSignature3.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  mp_Model->appendFunction(FctSignature1);
  mp_Model->appendFunction(FctSignature2);
  mp_Model->appendFunction(FctSignature3);

  //remove first
  mp_Model->removeFunctionAt(0);
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),0);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_MoveFunction)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  std::string idA = "tests.primitives.prod";
  std::string idB = "tests.primitives.use";
  std::string idC = "tests.vector.prod";

  openfluid::machine::SignatureItemInstance FctSignature1 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idA);
  FctSignature1.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idB);
  FctSignature2.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature3 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idC);
  FctSignature3.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  mp_Model->appendFunction(FctSignature1);
  mp_Model->appendFunction(FctSignature2);
  mp_Model->appendFunction(FctSignature3);

  // move to lower
  mp_Model->moveFunction(2,1);
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),1);
  BOOST_CHECK_EQUAL(mp_View->getRowId(0),"1. " + idA);
  BOOST_CHECK_EQUAL(mp_View->getRowId(1),"2. " + idC);
  BOOST_CHECK_EQUAL(mp_View->getRowId(2),"3. " + idB);

  // move to upper
  mp_Model->moveFunction(0,1);
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),1);
  BOOST_CHECK_EQUAL(mp_View->getRowId(0),"1. " + idC);
  BOOST_CHECK_EQUAL(mp_View->getRowId(1),"2. " + idA);
  BOOST_CHECK_EQUAL(mp_View->getRowId(2),"3. " + idB);

  // bad To index, throw "Bad indexes of items to move"
  BOOST_CHECK_THROW(mp_Model->moveFunction(0,3),openfluid::base::OFException);

  // bad From index, throw "Bad indexes of items to move"
  BOOST_CHECK_THROW(mp_Model->moveFunction(-1,1),openfluid::base::OFException);

  // does nothing
  mp_Model->moveFunction(1,1);
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),1);
  BOOST_CHECK_EQUAL(mp_View->getRowId(0),"1. " + idC);
  BOOST_CHECK_EQUAL(mp_View->getRowId(1),"2. " + idA);
  BOOST_CHECK_EQUAL(mp_View->getRowId(2),"3. " + idB);

  // move to top
  mp_Model->moveFunction(0,2);
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),2);
  BOOST_CHECK_EQUAL(mp_View->getRowId(0),"1. " + idA);
  BOOST_CHECK_EQUAL(mp_View->getRowId(1),"2. " + idB);
  BOOST_CHECK_EQUAL(mp_View->getRowId(2),"3. " + idC);

  // move to the bottom
  mp_Model->moveFunction(2,0);
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),0);
  BOOST_CHECK_EQUAL(mp_View->getRowId(0),"1. " + idC);
  BOOST_CHECK_EQUAL(mp_View->getRowId(1),"2. " + idA);
  BOOST_CHECK_EQUAL(mp_View->getRowId(2),"3. " + idB);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_SelectAFunction)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(*EngProject->getModelInstance(), &EngProject->getCoreRepository());

  std::string idA = "tests.primitives.prod";
  std::string idB = "tests.primitives.use";
  std::string idC = "tests.vector.prod";

  openfluid::machine::SignatureItemInstance FctSignature1 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idA);
  FctSignature1.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idB);
  FctSignature2.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature3 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idC);
  FctSignature3.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  mp_Model->appendFunction(FctSignature1);
  mp_Model->appendFunction(FctSignature2);
  mp_Model->appendFunction(FctSignature3);

  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),2);

  mp_View->requestSelectionAt(1);
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),1);

  mp_View->requestSelectionAt(0);
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),0);

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
  FctSignature1.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idB);
  FctSignature2.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature3 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idC);
  FctSignature3.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  mp_Model->appendFunction(FctSignature1);
  mp_Model->appendFunction(FctSignature2);
  mp_Model->appendFunction(FctSignature3);

  std::list<openfluid::machine::ModelItemInstance*>::const_iterator it;

  //select and move
  mp_Model->requestSelectionByAppAt(2);
  mp_Model->moveTowardTheBegin();
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),1);
  it = mp_Model->getModelInstance()->getItems().begin();
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idA);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idC);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idB);

  //select and move (circular)
  mp_Model->requestSelectionByAppAt(0);
  mp_Model->moveTowardTheBegin();
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),2);
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
  FctSignature1.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature2 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idB);
  FctSignature2.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  openfluid::machine::SignatureItemInstance FctSignature3 =
  *openfluid::machine::PluginManager::getInstance()->getPlugin(idC);
  FctSignature3.ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  mp_Model->appendFunction(FctSignature1);
  mp_Model->appendFunction(FctSignature2);
  mp_Model->appendFunction(FctSignature3);

  std::list<openfluid::machine::ModelItemInstance*>::const_iterator it;

  //select and move
  mp_Model->requestSelectionByAppAt(0);
  mp_Model->moveTowardTheEnd();
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),1);
  it = mp_Model->getModelInstance()->getItems().begin();
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idB);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idA);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idC);

  //select and move (circular)
  mp_Model->requestSelectionByAppAt(2);
  mp_Model->moveTowardTheEnd();
  BOOST_CHECK_EQUAL(mp_Model->getCurrentSelection(),0);
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

BOOST_AUTO_TEST_SUITE_END()

