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
 \file DomainUnitCreationDialog_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_DomainUnitAddEditDialog
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "DomainUnitAddEditDialog.hpp"
#include "DomainUnitRelationAddDialog.hpp"
#include "EngineProject.hpp"
#include "tests-config.hpp"
#include "EngineHelper.hpp"
#include <openfluid/guicommon/BuilderDescriptor.hpp>

// =====================================================================
// =====================================================================

struct init_Dialog
{
    DomainUnitAddEditDialogSub* mp_Dialog;
    DomainUnitRelationAddDialog* mp_AddDialog;
    EngineProject* mp_EngProject;

    init_Dialog()
    {
      BuilderTestHelper::getInstance()->initGtk();

      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.Primitives";
      mp_EngProject = new EngineProject(Path);

      mp_AddDialog = new DomainUnitRelationAddDialog();
      mp_Dialog = new DomainUnitAddEditDialogSub(*mp_AddDialog, mp_EngProject->getBuilderDesc().getDomain());
    }

    ~init_Dialog()
    {
      delete mp_AddDialog;
      delete mp_Dialog;
      delete mp_EngProject;
    }
};

BOOST_FIXTURE_TEST_SUITE(DomainUnitAddEditDialogTest, init_Dialog)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_initialize)
{
  openfluid::guicommon::BuilderDomain* Domain = &(mp_EngProject->getBuilderDesc().getDomain());

//  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getParentWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getChildWidget()->getUnits().size(),0);

  mp_Dialog->initCreationMode("TestUnits");

//  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getParentWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getChildWidget()->getUnits().size(),0);

  mp_Dialog->initCreationMode("ParentTestUnits");

//  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getParentWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getChildWidget()->getUnits().size(),0);

  mp_Dialog->setUnit(const_cast<openfluid::fluidx::UnitDescriptor*>(&Domain->getUnitDescriptor("TestUnits",4)));
  mp_Dialog->initEditionMode();

//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getParentUnits("ParentTestUnits")->size(),1);

//  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getParentWidget()->getUnits().size(),1);
//  BOOST_CHECK_EQUAL(mp_Dialog->getChildWidget()->getUnits().size(),0);
//
//  BOOST_CHECK_EQUAL((*mp_Dialog->getParentWidget()->getUnits().begin())->getClass(),"ParentTestUnits");
//  BOOST_CHECK_EQUAL((*mp_Dialog->getParentWidget()->getUnits().begin())->getID(),2);

  mp_Dialog->setUnit(const_cast<openfluid::fluidx::UnitDescriptor*>(&Domain->getUnitDescriptor("ParentTestUnits",2)));
  mp_Dialog->initEditionMode();

//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getChildrenUnits("TestUnits")->size(),4);

//  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getParentWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getChildWidget()->getUnits().size(),4);

}

//BOOST_AUTO_TEST_CASE(test_clearRelations)
//{
//  mp_Dialog->setUnit(mp_EngProject->getCoreRepository().getUnit("TestUnits",4));
//
//  mp_Dialog->clearAllRelations();
//
//  std::set<std::string> Classes = EngineHelper::getClassNames(&mp_EngProject->getCoreRepository());
//
//  for (std::set<std::string>::iterator it = Classes.begin(); it
//      != Classes.end(); ++it)
//  {
//    if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getFromUnits(*it) != NULL)
//    BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getFromUnits(*it)->size(),0);
//
//    if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits(*it) != NULL)
//    BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits(*it)->size(),0);
//
//    if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getParentUnits(*it) != NULL)
//    BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getParentUnits(*it)->size(),0);
//
//    if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getChildrenUnits(*it) != NULL)
//    BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getChildrenUnits(*it)->size(),0);
//
//    if(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getFromUnits(*it) != NULL)
//    BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getFromUnits(*it)->size(),0);
//
//    if(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getToUnits(*it) != NULL)
//    BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getToUnits(*it)->size(),0);
//
//    if(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getParentUnits(*it) != NULL)
//    BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getParentUnits(*it)->size(),0);
//
//    if(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getChildrenUnits(*it) != NULL)
//    BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getChildrenUnits(*it)->size(),3);
//  }
//
//}
//
//BOOST_AUTO_TEST_CASE(test_createRelations)
//{
//  mp_Dialog->setUnit(mp_EngProject->getCoreRepository().getUnit("TestUnits",4));
//
//  mp_Dialog->initEditionMode();
//  mp_Dialog->clearAllRelations();
//
//  mp_Dialog->createAllRelations();
//
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getFromUnits("TestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getFromUnits("TestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("TestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("TestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getParentUnits("TestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getParentUnits("TestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getChildrenUnits("TestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getChildrenUnits("TestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getFromUnits("ParentTestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getFromUnits("ParentTestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("ParentTestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("ParentTestUnits")->size(),0);
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getParentUnits("ParentTestUnits")->size(),1);
//
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getChildrenUnits("ParentTestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getChildrenUnits("ParentTestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getFromUnits("TestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getFromUnits("TestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getToUnits("TestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getToUnits("TestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getParentUnits("TestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getParentUnits("TestUnits")->size(),0);
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getChildrenUnits("TestUnits")->size(),4);
//
//  if(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getFromUnits("ParentTestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getFromUnits("ParentTestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getToUnits("ParentTestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getToUnits("ParentTestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getParentUnits("ParentTestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getParentUnits("ParentTestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getChildrenUnits("ParentTestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getChildrenUnits("ParentTestUnits")->size(),0);
//
//}
//
//BOOST_AUTO_TEST_CASE(test_addRelations)
//{
//  mp_Dialog->setUnit(mp_EngProject->getCoreRepository().getUnit("TestUnits",4));
//  mp_Dialog->initEditionMode();
//
//  std::list<openfluid::core::Unit*> UnitsToAdd;
//  UnitsToAdd.push_back(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2));
//  UnitsToAdd.push_back(mp_EngProject->getCoreRepository().getUnit("TestUnits",10));
//  UnitsToAdd.push_back(mp_EngProject->getCoreRepository().getUnit("TestUnits",11));
//
//  mp_Dialog->getToWidget()->addUnits(UnitsToAdd);
//
//  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(),3);
//  BOOST_CHECK_EQUAL(mp_Dialog->getParentWidget()->getUnits().size(),1);
//  BOOST_CHECK_EQUAL(mp_Dialog->getChildWidget()->getUnits().size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("TestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("TestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("ParentTestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("ParentTestUnits")->size(),0);
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getParentUnits("ParentTestUnits")->size(),1);
//
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",10)->getFromUnits("TestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",10)->getFromUnits("TestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",11)->getFromUnits("TestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",11)->getFromUnits("TestUnits")->size(),0);
//
//  if(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getFromUnits("TestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getFromUnits("TestUnits")->size(),0);
//
//  mp_Dialog->clearAllRelations();
//  mp_Dialog->createAllRelations();
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("TestUnits")->size(),2);
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("ParentTestUnits")->size(),1);
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getParentUnits("ParentTestUnits")->size(),1);
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",10)->getFromUnits("TestUnits")->size(),1);
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",11)->getFromUnits("TestUnits")->size(),1);
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getFromUnits("TestUnits")->size(),1);
//}
//
//BOOST_AUTO_TEST_CASE(test_addExistingRelation)
//{
//  mp_Dialog->setUnit(mp_EngProject->getCoreRepository().getUnit("TestUnits",4));
//  mp_Dialog->initEditionMode();
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getParentUnits("ParentTestUnits")->size(),1);
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getChildrenUnits("TestUnits")->size(),4);
//
//  std::list<openfluid::core::Unit*> UnitsToAdd;
//  UnitsToAdd.push_back(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2));
//
//  mp_Dialog->getParentWidget()->addUnits(UnitsToAdd);
//
//  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(),0);
//  BOOST_CHECK_EQUAL(mp_Dialog->getParentWidget()->getUnits().size(),1);
//  BOOST_CHECK_EQUAL(mp_Dialog->getChildWidget()->getUnits().size(),0);
//
//  mp_Dialog->clearAllRelations();
//  mp_Dialog->createAllRelations();
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getParentUnits("ParentTestUnits")->size(),1);
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getChildrenUnits("TestUnits")->size(),4);
//}
//
//BOOST_AUTO_TEST_CASE(test_addItself)
//{
//  mp_Dialog->setUnit(mp_EngProject->getCoreRepository().getUnit("TestUnits",4));
//  mp_Dialog->initEditionMode();
//
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getFromUnits("TestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getFromUnits("TestUnits")->size(),0);
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getFromUnits("TestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getFromUnits("TestUnits")->size(),0);
//
//  std::list<openfluid::core::Unit*> UnitsToAdd;
//  UnitsToAdd.push_back(mp_EngProject->getCoreRepository().getUnit("TestUnits",4));
//
//  mp_Dialog->getFromWidget()->addUnits(UnitsToAdd);
//
//  mp_Dialog->clearAllRelations();
//  mp_Dialog->createAllRelations();
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getFromUnits("TestUnits")->size(),1);
//  BOOST_CHECK_EQUAL((*mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getFromUnits("TestUnits")->begin())->getClass(),"TestUnits");
//  BOOST_CHECK_EQUAL((*mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getFromUnits("TestUnits")->begin())->getID(),4);
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("TestUnits")->size(),1);
//  BOOST_CHECK_EQUAL((*mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("TestUnits")->begin())->getClass(),"TestUnits");
//  BOOST_CHECK_EQUAL((*mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("TestUnits")->begin())->getID(),4);
//
//  mp_Dialog->setUnit(mp_EngProject->getCoreRepository().getUnit("TestUnits",4));
//  mp_Dialog->initEditionMode();
//  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(),1);
//  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(),1);
//}
//
//BOOST_AUTO_TEST_CASE(test_removeRelation)
//{
//  mp_Dialog->setUnit(mp_EngProject->getCoreRepository().getUnit("TestUnits",4));
//  mp_Dialog->initEditionMode();
//
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getParentUnits("ParentTestUnits")->size(),1);
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getChildrenUnits("TestUnits")->size(),4);
//
//  mp_Dialog->getParentWidget()->getTreeView()->get_selection()->select(mp_Dialog->getParentWidget()->getTreeModel()->children().begin());
//  mp_Dialog->getParentWidget()->onRemoveClicked();
//
//  mp_Dialog->clearAllRelations();
//  mp_Dialog->createAllRelations();
//
//  if(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getParentUnits("ParentTestUnits") != NULL)
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getParentUnits("ParentTestUnits")->size(),0);
//  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("ParentTestUnits",2)->getChildrenUnits("TestUnits")->size(),3);
//}
//
//BOOST_AUTO_TEST_CASE(test_addUnit)
//{
//  mp_Dialog->initCreationMode("");
//
//  mp_Dialog->getClassComboEntryText()->get_entry()->set_text("class A");
//  mp_Dialog->getIdSpinValue()->set_value(100);
//
//  std::list<openfluid::core::Unit*> UnitsToAdd;
//  UnitsToAdd.push_back(mp_EngProject->getCoreRepository().getUnit("TestUnits",4));
//
//  mp_Dialog->getFromWidget()->addUnits(UnitsToAdd);
//
//  mp_Dialog->createUnit();
//
//  mp_Dialog->clearAllRelations();
//  mp_Dialog->createAllRelations();
//
//  BOOST_CHECK_EQUAL((*mp_EngProject->getCoreRepository().getUnit("class A",100)->getFromUnits("TestUnits")->begin())->getID(),4);
//  BOOST_CHECK_EQUAL((*mp_EngProject->getCoreRepository().getUnit("TestUnits",4)->getToUnits("class A")->begin())->getID(),100);
//}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
