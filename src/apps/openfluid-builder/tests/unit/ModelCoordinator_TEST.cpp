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
 \file ModelCoordinator_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_ModelCoordinator
#include <boost/test/unit_test.hpp>

//#include "BuilderModuleFactory.hpp"
#include "BuilderTestHelper.hpp"
#include "TestModelInstanceWrapper.hpp"

//#include "ModelAvailFctComponent.hpp"
#include "ModelAvailFctModel.hpp"
//#include "ModelAvailFctView.hpp"
//#include "ModelAvailFctAdapterModel.hpp"
//#include "ModelAvailFctAdapter.hpp"
//#include "ModelAvailFctPresenter.hpp"

#include "ModelFctDetailModel.hpp"
#include "ModelStructureModel.hpp"
#include "BuilderListToolBox.hpp"
#include "ModelCoordinator.hpp"

#include "FunctionSignatureRegistry.hpp"
#include "BuilderModelModule.hpp"

struct init_Coordinator
{
    BuilderModelModuleSub* mp_ModelModule;

    ModelAvailFctModel* mp_AvailFctModel;
    ModelFctDetailModel* mp_FctDetailModel;
    ModelStructureModel* mp_StructureModel;
    BuilderListToolBox* mp_StructureListToolBox;
    ModelCoordinator* mp_Coordinator;

    init_Coordinator()
    {
      BuilderTestHelper::getInstance()->initGtk();

//      mp_ModelModule = BuilderModuleFactory::createModelModuleSub();
      BuilderListToolBoxFactory TBFactory;
      mp_ModelModule = new BuilderModelModuleSub(TBFactory);

      mp_AvailFctModel = mp_ModelModule->getAvailFctMVPModel();
      mp_FctDetailModel = mp_ModelModule->getModelFctDetailMVPModel();
      mp_StructureModel = mp_ModelModule->getModelStructureMVPModel();
      mp_StructureListToolBox = mp_ModelModule->getModelStructureListToolBox();
      mp_Coordinator = mp_ModelModule->getCoordinator();
    }

    ~init_Coordinator()
    {
      delete mp_ModelModule;
    }
};

BOOST_FIXTURE_TEST_SUITE(ModelCoordinatorTest, init_Coordinator)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_initialStateWithEmptyModel)
{
  BOOST_CHECK_EQUAL(mp_AvailFctModel->isASignatureSelected(), false);
  BOOST_CHECK(mp_FctDetailModel->getFctDisplayed() == 0);
  BOOST_CHECK_EQUAL(mp_StructureModel->getFctCount(), 0);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isAddCommandAvailable(),false);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isRemoveCommandAvailable(),false);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isUpCommandAvailable(),false);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isDownCommandAvailable(),false);

  // init signatures
  FunctionSignatureRegistry* Signatures = FunctionSignatureRegistry::getInstance();
  mp_Coordinator->setSignatures(*Signatures);

  BOOST_CHECK_EQUAL(mp_AvailFctModel->isASignatureSelected(), true);
  BOOST_CHECK(mp_FctDetailModel->getFctDisplayed() == Signatures->getGeneratorSignatures()[0]);
  BOOST_CHECK_EQUAL(mp_StructureModel->getFctCount(), 0);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isAddCommandAvailable(),true);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isRemoveCommandAvailable(),false);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isUpCommandAvailable(),false);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isDownCommandAvailable(),false);

  // set Engine elements
  TestModelInstanceWrapper EmptyModelInstanceWrapper;
  mp_Coordinator->setEngineRequirements(*EmptyModelInstanceWrapper.getModelInstance());

  BOOST_CHECK_EQUAL(mp_AvailFctModel->isASignatureSelected(), true);
  BOOST_CHECK(mp_FctDetailModel->getFctDisplayed() == Signatures->getGeneratorSignatures()[0]);
  BOOST_CHECK_EQUAL(mp_StructureModel->getFctCount(), 0);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isAddCommandAvailable(),true);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isRemoveCommandAvailable(),false);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isUpCommandAvailable(),false);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isDownCommandAvailable(),false);

  delete Signatures;
}

BOOST_AUTO_TEST_CASE(test_initialStateWithNotEmptyModel)
{
  // init signatures
  FunctionSignatureRegistry* Signatures = FunctionSignatureRegistry::getInstance();
  mp_Coordinator->setSignatures(*Signatures);

  // set Engine elements
  TestModelInstanceWrapper OneItemModelInstanceWrapper(1);
  mp_Coordinator->setEngineRequirements(*OneItemModelInstanceWrapper.getModelInstance());

  BOOST_CHECK_EQUAL(mp_AvailFctModel->isASignatureSelected(), true);
  BOOST_CHECK(mp_FctDetailModel->getFctDisplayed() == Signatures->getGeneratorSignatures()[0]);
  BOOST_CHECK_EQUAL(mp_StructureModel->getFctCount(), 1);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isAddCommandAvailable(),true);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isRemoveCommandAvailable(),true);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isUpCommandAvailable(),false);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isDownCommandAvailable(),false);

  delete Signatures;
}

BOOST_AUTO_TEST_CASE(test_addAFunction)
{
  // init signatures
  FunctionSignatureRegistry* Signatures = FunctionSignatureRegistry::getInstance();
  mp_Coordinator->setSignatures(*Signatures);

  // set Engine elements
  TestModelInstanceWrapper EmptyModelInstanceWrapper;
  mp_Coordinator->setEngineRequirements(*EmptyModelInstanceWrapper.getModelInstance());

  // add a first function
  mp_StructureListToolBox->signal_AddCommandAsked().emit();

  BOOST_CHECK_EQUAL(mp_AvailFctModel->isASignatureSelected(), true);
  BOOST_CHECK(mp_FctDetailModel->getFctDisplayed() == Signatures->getGeneratorSignatures()[0]);
  BOOST_CHECK_EQUAL(mp_StructureModel->getFctCount(), 1);
  BOOST_CHECK_EQUAL(mp_StructureModel->getCurrentSelection(), 0);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isAddCommandAvailable(),true);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isRemoveCommandAvailable(),true);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isUpCommandAvailable(),false);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isDownCommandAvailable(),false);

  // add a second function
  mp_StructureListToolBox->signal_AddCommandAsked().emit();

  BOOST_CHECK_EQUAL(mp_AvailFctModel->isASignatureSelected(), true);
  BOOST_CHECK(mp_FctDetailModel->getFctDisplayed() == Signatures->getGeneratorSignatures()[0]);
  BOOST_CHECK_EQUAL(mp_StructureModel->getFctCount(), 2);
  BOOST_CHECK_EQUAL(mp_StructureModel->getCurrentSelection(), 1);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isAddCommandAvailable(),true);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isRemoveCommandAvailable(),true);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isUpCommandAvailable(),true);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isDownCommandAvailable(),true);

  delete Signatures;
}

BOOST_AUTO_TEST_CASE(test_removeAFunction)
{
  // init signatures
  FunctionSignatureRegistry* Signatures = FunctionSignatureRegistry::getInstance();
  mp_Coordinator->setSignatures(*Signatures);

  // set Engine elements
  TestModelInstanceWrapper OneItemModelInstanceWrapper(1);
  mp_Coordinator->setEngineRequirements(*OneItemModelInstanceWrapper.getModelInstance());

  // remove the function
  mp_StructureListToolBox->signal_RemoveCommandAsked().emit();

  BOOST_CHECK_EQUAL(mp_AvailFctModel->isASignatureSelected(), true);
  BOOST_CHECK(mp_FctDetailModel->getFctDisplayed() == Signatures->getGeneratorSignatures()[0]);
  BOOST_CHECK_EQUAL(mp_StructureModel->getFctCount(), 0);
  BOOST_CHECK_EQUAL(mp_StructureModel->getCurrentSelection(), -1);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isAddCommandAvailable(),true);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isRemoveCommandAvailable(),false);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isUpCommandAvailable(),false);
  BOOST_CHECK_EQUAL(mp_StructureListToolBox->isDownCommandAvailable(),false);

  delete Signatures;
}

BOOST_AUTO_TEST_CASE(test_moveTowardTheBegin)
{
  // set Engine elements
  TestModelInstanceWrapper ThreeItemsModelInstanceWrapper(3);
  mp_Coordinator->setEngineRequirements(*ThreeItemsModelInstanceWrapper.getModelInstance());

  std::string idA = ThreeItemsModelInstanceWrapper.getItemIdAt(0);
  std::string idB = ThreeItemsModelInstanceWrapper.getItemIdAt(1);
  std::string idC = ThreeItemsModelInstanceWrapper.getItemIdAt(2);

  // select and emit
  mp_StructureModel->requestSelectionByAppAt(2);
  mp_StructureListToolBox->signal_UpCommandAsked().emit();

  std::list<openfluid::machine::ModelItemInstance*>::const_iterator it;
  it = mp_StructureModel->getModelInstance()->getItems().begin();
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idA);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idC);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idB);
}

BOOST_AUTO_TEST_CASE(test_moveTowardTheEnd)
{
  // set Engine elements
  TestModelInstanceWrapper ThreeItemsModelInstanceWrapper(3);
  mp_Coordinator->setEngineRequirements(*ThreeItemsModelInstanceWrapper.getModelInstance());

  std::string idA = ThreeItemsModelInstanceWrapper.getItemIdAt(0);
  std::string idB = ThreeItemsModelInstanceWrapper.getItemIdAt(1);
  std::string idC = ThreeItemsModelInstanceWrapper.getItemIdAt(2);

  // select and emit
  mp_StructureModel->requestSelectionByAppAt(1);
  mp_StructureListToolBox->signal_DownCommandAsked().emit();

  std::list<openfluid::machine::ModelItemInstance*>::const_iterator it;
  it = mp_StructureModel->getModelInstance()->getItems().begin();
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idA);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idC);
  it++;
  BOOST_CHECK_EQUAL((*it)->Signature->ID, idB);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
