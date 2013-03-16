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
#include "tests-config.hpp"
#include "EngineHelper.hpp"
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>

// =====================================================================
// =====================================================================

struct init_Dialog
{
    DomainUnitAddEditDialogSub* mp_Dialog;
    DomainUnitRelationAddDialog* mp_AddDialog;
    openfluid::fluidx::AdvancedDomainDescriptor* mp_Domain;
    openfluid::fluidx::FluidXDescriptor* mp_FXDesc;

    init_Dialog()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_FXDesc = new openfluid::fluidx::FluidXDescriptor(0);
      mp_FXDesc->loadFromDirectory(
          CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

      mp_Domain = new openfluid::fluidx::AdvancedDomainDescriptor(
          mp_FXDesc->getDomainDescriptor());

      mp_AddDialog = new DomainUnitRelationAddDialog(*mp_Domain);
      mp_Dialog = new DomainUnitAddEditDialogSub(*mp_AddDialog, *mp_Domain);
    }

    ~init_Dialog()
    {
      delete mp_AddDialog;
      delete mp_Dialog;
      delete mp_Domain;
    }
};

BOOST_FIXTURE_TEST_SUITE(DomainUnitAddEditDialogTest, init_Dialog)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_initialize)
{
  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getParentWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getChildWidget()->getUnits().size(), 0);

  mp_Dialog->initCreationMode("unitsA");

  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getParentWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getChildWidget()->getUnits().size(), 0);

  mp_Dialog->initCreationMode("unitsB");

  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getParentWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getChildWidget()->getUnits().size(), 0);

  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsB", 11)));

  mp_Dialog->initEditionMode();

  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t A7 = std::make_pair("unitsA", 7);
  openfluid::core::UnitClassID_t B1 = std::make_pair("unitsB", 1);
  openfluid::core::UnitClassID_t B2 = std::make_pair("unitsB", 2);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  std::list<openfluid::core::UnitClassID_t> Widget_Froms =
      mp_Dialog->getFromWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Tos =
      mp_Dialog->getToWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Parents =
      mp_Dialog->getParentWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Children =
      mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 1);
  BOOST_CHECK(std::count(Widget_Froms.begin(),Widget_Froms.end(),A3));

  BOOST_CHECK_EQUAL(Widget_Tos.size(), 1);
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),B3));

  BOOST_CHECK_EQUAL(Widget_Parents.size(), 1);
  BOOST_CHECK(std::count(Widget_Parents.begin(),Widget_Parents.end(),P1));

  BOOST_CHECK_EQUAL(Widget_Children.size(), 0);

  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsP", 1)));
  mp_Dialog->initEditionMode();

  Widget_Froms = mp_Dialog->getFromWidget()->getUnits();
  Widget_Tos = mp_Dialog->getToWidget()->getUnits();
  Widget_Parents = mp_Dialog->getParentWidget()->getUnits();
  Widget_Children = mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 0);

  BOOST_CHECK_EQUAL(Widget_Tos.size(), 0);

  BOOST_CHECK_EQUAL(Widget_Parents.size(), 0);

  BOOST_CHECK_EQUAL(Widget_Children.size(), 2);
  BOOST_CHECK(std::count(Widget_Children.begin(),Widget_Children.end(),B11));
  BOOST_CHECK(std::count(Widget_Children.begin(),Widget_Children.end(),A3));

  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsB", 3)));
  mp_Dialog->initEditionMode();

  Widget_Froms = mp_Dialog->getFromWidget()->getUnits();
  Widget_Tos = mp_Dialog->getToWidget()->getUnits();
  Widget_Parents = mp_Dialog->getParentWidget()->getUnits();
  Widget_Children = mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 3);
  BOOST_CHECK(std::count(Widget_Froms.begin(),Widget_Froms.end(),B11));
  BOOST_CHECK(std::count(Widget_Froms.begin(),Widget_Froms.end(),A7));
  BOOST_CHECK(std::count(Widget_Froms.begin(),Widget_Froms.end(),B1));

  BOOST_CHECK_EQUAL(Widget_Tos.size(), 1);
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),B2));

  BOOST_CHECK_EQUAL(Widget_Parents.size(), 0);

  BOOST_CHECK_EQUAL(Widget_Children.size(), 0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_clearRelations)
{
  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsB", 11)));

  mp_Dialog->clearAllRelations();

  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t A7 = std::make_pair("unitsA", 7);
  openfluid::core::UnitClassID_t B1 = std::make_pair("unitsB", 1);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getParentWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getChildWidget()->getUnits().size(), 0);

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsFromOf(B11).size(), 0);
  BOOST_CHECK_EQUAL(mp_Domain->getUnitsToOf(B11).size(), 0);
  BOOST_CHECK_EQUAL(mp_Domain->getUnitsParentsOf(B11).size(), 0);
  BOOST_CHECK_EQUAL(mp_Domain->getUnitsChildrenOf(B11).size(), 0);

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsToOf(A3).size(), 0);

  std::list<openfluid::core::UnitClassID_t> FromsOfB3 =
      mp_Domain->getUnitsFromOf(B3);
  BOOST_CHECK_EQUAL(FromsOfB3.size(), 2);
  BOOST_CHECK(!std::count(FromsOfB3.begin(),FromsOfB3.end(),B11));
  BOOST_CHECK(std::count(FromsOfB3.begin(),FromsOfB3.end(),A7));
  BOOST_CHECK(std::count(FromsOfB3.begin(),FromsOfB3.end(),B1));

  std::list<openfluid::core::UnitClassID_t> ChildrenOfP1 =
      mp_Domain->getUnitsChildrenOf(P1);
  BOOST_CHECK_EQUAL(ChildrenOfP1.size(), 1);
  BOOST_CHECK(!std::count(ChildrenOfP1.begin(),ChildrenOfP1.end(),B11));
  BOOST_CHECK(std::count(ChildrenOfP1.begin(),ChildrenOfP1.end(),A3));
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_createRelations)
{
  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsB", 11)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t A7 = std::make_pair("unitsA", 7);
  openfluid::core::UnitClassID_t B1 = std::make_pair("unitsB", 1);
  openfluid::core::UnitClassID_t B2 = std::make_pair("unitsB", 2);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  std::list<openfluid::core::UnitClassID_t> Widget_Froms =
      mp_Dialog->getFromWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Tos =
      mp_Dialog->getToWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Parents =
      mp_Dialog->getParentWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Children =
      mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 1);
  BOOST_CHECK(std::count(Widget_Froms.begin(),Widget_Froms.end(),A3));

  BOOST_CHECK_EQUAL(Widget_Tos.size(), 1);
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),B3));

  BOOST_CHECK_EQUAL(Widget_Parents.size(), 1);
  BOOST_CHECK(std::count(Widget_Parents.begin(),Widget_Parents.end(),P1));

  BOOST_CHECK_EQUAL(Widget_Children.size(), 0);

  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsP", 1)));
  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  Widget_Froms = mp_Dialog->getFromWidget()->getUnits();
  Widget_Tos = mp_Dialog->getToWidget()->getUnits();
  Widget_Parents = mp_Dialog->getParentWidget()->getUnits();
  Widget_Children = mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 0);

  BOOST_CHECK_EQUAL(Widget_Tos.size(), 0);

  BOOST_CHECK_EQUAL(Widget_Parents.size(), 0);

  BOOST_CHECK_EQUAL(Widget_Children.size(), 2);
  BOOST_CHECK(std::count(Widget_Children.begin(),Widget_Children.end(),B11));
  BOOST_CHECK(std::count(Widget_Children.begin(),Widget_Children.end(),A3));

  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsB", 3)));
  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  Widget_Froms = mp_Dialog->getFromWidget()->getUnits();
  Widget_Tos = mp_Dialog->getToWidget()->getUnits();
  Widget_Parents = mp_Dialog->getParentWidget()->getUnits();
  Widget_Children = mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 3);
  BOOST_CHECK(std::count(Widget_Froms.begin(),Widget_Froms.end(),B11));
  BOOST_CHECK(std::count(Widget_Froms.begin(),Widget_Froms.end(),A7));
  BOOST_CHECK(std::count(Widget_Froms.begin(),Widget_Froms.end(),B1));

  BOOST_CHECK_EQUAL(Widget_Tos.size(), 1);
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),B2));

  BOOST_CHECK_EQUAL(Widget_Parents.size(), 0);

  BOOST_CHECK_EQUAL(Widget_Children.size(), 0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_addRelations_To)
{
  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsB", 11)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  openfluid::core::UnitClassID_t A1 = std::make_pair("unitsA", 1);
  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t A5 = std::make_pair("unitsA", 5);
  openfluid::core::UnitClassID_t A7 = std::make_pair("unitsA", 7);
  openfluid::core::UnitClassID_t B1 = std::make_pair("unitsB", 1);
  openfluid::core::UnitClassID_t B2 = std::make_pair("unitsB", 2);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  std::list<openfluid::core::UnitClassID_t> UnitsToAdd;
  UnitsToAdd.push_back(A5);
  UnitsToAdd.push_back(B2);
  mp_Dialog->getToWidget()->addUnits(UnitsToAdd);

  std::list<openfluid::core::UnitClassID_t> Widget_Froms =
      mp_Dialog->getFromWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Tos =
      mp_Dialog->getToWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Parents =
      mp_Dialog->getParentWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Children =
      mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 1);
  BOOST_CHECK(std::count(Widget_Froms.begin(),Widget_Froms.end(),A3));

  BOOST_CHECK_EQUAL(Widget_Tos.size(), 3);
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),B3));
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),A5));
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),B2));

  BOOST_CHECK_EQUAL(Widget_Parents.size(), 1);
  BOOST_CHECK(std::count(Widget_Parents.begin(),Widget_Parents.end(),P1));

  BOOST_CHECK_EQUAL(Widget_Children.size(), 0);

  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  std::list<openfluid::core::UnitClassID_t> FromsOfB11 =
      mp_Domain->getUnitsFromOf(B11);
  BOOST_CHECK_EQUAL(FromsOfB11.size(), 1);
  BOOST_CHECK(std::count(FromsOfB11.begin(),FromsOfB11.end(),A3));

  std::list<openfluid::core::UnitClassID_t> ToOfB11 = mp_Domain->getUnitsToOf(
      B11);
  BOOST_CHECK_EQUAL(ToOfB11.size(), 3);
  BOOST_CHECK(std::count(ToOfB11.begin(),ToOfB11.end(),B3));
  BOOST_CHECK(std::count(ToOfB11.begin(),ToOfB11.end(),A5));
  BOOST_CHECK(std::count(ToOfB11.begin(),ToOfB11.end(),B2));

  std::list<openfluid::core::UnitClassID_t> ParentsOfB11 =
      mp_Domain->getUnitsParentsOf(B11);
  BOOST_CHECK_EQUAL(ParentsOfB11.size(), 1);
  BOOST_CHECK(std::count(ParentsOfB11.begin(),ParentsOfB11.end(),P1));

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsChildrenOf(B11).size(), 0);

  std::list<openfluid::core::UnitClassID_t> FromsOfA5 =
      mp_Domain->getUnitsFromOf(A5);
  BOOST_CHECK_EQUAL(FromsOfA5.size(), 1);
  BOOST_CHECK(std::count(FromsOfA5.begin(),FromsOfA5.end(),B11));

  std::list<openfluid::core::UnitClassID_t> FromsOfB2 =
      mp_Domain->getUnitsFromOf(B2);
  BOOST_CHECK_EQUAL(FromsOfB2.size(), 3);
  BOOST_CHECK(std::count(FromsOfB2.begin(),FromsOfB2.end(),B11));
  BOOST_CHECK(std::count(FromsOfB2.begin(),FromsOfB2.end(),A1));
  BOOST_CHECK(std::count(FromsOfB2.begin(),FromsOfB2.end(),B3));

  std::list<openfluid::core::UnitClassID_t> FromsOfB3 =
      mp_Domain->getUnitsFromOf(B3);
  BOOST_CHECK_EQUAL(FromsOfB3.size(), 3);
  BOOST_CHECK(std::count(FromsOfB3.begin(),FromsOfB3.end(),B11));
  BOOST_CHECK(std::count(FromsOfB3.begin(),FromsOfB3.end(),A7));
  BOOST_CHECK(std::count(FromsOfB3.begin(),FromsOfB3.end(),B1));
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_addRelations_From)
{
  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsA", 5)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  openfluid::core::UnitClassID_t A1 = std::make_pair("unitsA", 1);
  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t A5 = std::make_pair("unitsA", 5);
  openfluid::core::UnitClassID_t A7 = std::make_pair("unitsA", 7);
  openfluid::core::UnitClassID_t B1 = std::make_pair("unitsB", 1);
  openfluid::core::UnitClassID_t B2 = std::make_pair("unitsB", 2);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  std::list<openfluid::core::UnitClassID_t> UnitsToAdd;
  UnitsToAdd.push_back(B11);
  mp_Dialog->getFromWidget()->addUnits(UnitsToAdd);
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsB", 2)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  UnitsToAdd.clear();
  UnitsToAdd.push_back(B11);
  mp_Dialog->getFromWidget()->addUnits(UnitsToAdd);
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsB", 11)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  std::list<openfluid::core::UnitClassID_t> Widget_Froms =
      mp_Dialog->getFromWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Tos =
      mp_Dialog->getToWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Parents =
      mp_Dialog->getParentWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Children =
      mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 1);
  BOOST_CHECK(std::count(Widget_Froms.begin(),Widget_Froms.end(),A3));

  BOOST_CHECK_EQUAL(Widget_Tos.size(), 3);
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),B3));
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),A5));
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),B2));

  BOOST_CHECK_EQUAL(Widget_Parents.size(), 1);
  BOOST_CHECK(std::count(Widget_Parents.begin(),Widget_Parents.end(),P1));

  BOOST_CHECK_EQUAL(Widget_Children.size(), 0);

  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  std::list<openfluid::core::UnitClassID_t> FromsOfB11 =
      mp_Domain->getUnitsFromOf(B11);
  BOOST_CHECK_EQUAL(FromsOfB11.size(), 1);
  BOOST_CHECK(std::count(FromsOfB11.begin(),FromsOfB11.end(),A3));

  std::list<openfluid::core::UnitClassID_t> ToOfB11 = mp_Domain->getUnitsToOf(
      B11);
  BOOST_CHECK_EQUAL(ToOfB11.size(), 3);
  BOOST_CHECK(std::count(ToOfB11.begin(),ToOfB11.end(),B3));
  BOOST_CHECK(std::count(ToOfB11.begin(),ToOfB11.end(),A5));
  BOOST_CHECK(std::count(ToOfB11.begin(),ToOfB11.end(),B2));

  std::list<openfluid::core::UnitClassID_t> ParentsOfB11 =
      mp_Domain->getUnitsParentsOf(B11);
  BOOST_CHECK_EQUAL(ParentsOfB11.size(), 1);
  BOOST_CHECK(std::count(ParentsOfB11.begin(),ParentsOfB11.end(),P1));

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsChildrenOf(B11).size(), 0);

  std::list<openfluid::core::UnitClassID_t> FromsOfA5 =
      mp_Domain->getUnitsFromOf(A5);
  BOOST_CHECK_EQUAL(FromsOfA5.size(), 1);
  BOOST_CHECK(std::count(FromsOfA5.begin(),FromsOfA5.end(),B11));

  std::list<openfluid::core::UnitClassID_t> FromsOfB2 =
      mp_Domain->getUnitsFromOf(B2);
  BOOST_CHECK_EQUAL(FromsOfB2.size(), 3);
  BOOST_CHECK(std::count(FromsOfB2.begin(),FromsOfB2.end(),B11));
  BOOST_CHECK(std::count(FromsOfB2.begin(),FromsOfB2.end(),A1));
  BOOST_CHECK(std::count(FromsOfB2.begin(),FromsOfB2.end(),B3));

  std::list<openfluid::core::UnitClassID_t> FromsOfB3 =
      mp_Domain->getUnitsFromOf(B3);
  BOOST_CHECK_EQUAL(FromsOfB3.size(), 3);
  BOOST_CHECK(std::count(FromsOfB3.begin(),FromsOfB3.end(),B11));
  BOOST_CHECK(std::count(FromsOfB3.begin(),FromsOfB3.end(),A7));
  BOOST_CHECK(std::count(FromsOfB3.begin(),FromsOfB3.end(),B1));
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_addRelations_Parent)
{
  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsB", 11)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  std::list<openfluid::core::UnitClassID_t> UnitsToAdd;
  UnitsToAdd.push_back(A3);
  mp_Dialog->getParentWidget()->addUnits(UnitsToAdd);

  std::list<openfluid::core::UnitClassID_t> Widget_Froms =
      mp_Dialog->getFromWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Tos =
      mp_Dialog->getToWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Parents =
      mp_Dialog->getParentWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Children =
      mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 1);
  BOOST_CHECK(std::count(Widget_Froms.begin(),Widget_Froms.end(),A3));

  BOOST_CHECK_EQUAL(Widget_Tos.size(), 1);
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),B3));

  BOOST_CHECK_EQUAL(Widget_Parents.size(), 2);
  BOOST_CHECK(std::count(Widget_Parents.begin(),Widget_Parents.end(),P1));
  BOOST_CHECK(std::count(Widget_Parents.begin(),Widget_Parents.end(),A3));

  BOOST_CHECK_EQUAL(Widget_Children.size(), 0);

  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  std::list<openfluid::core::UnitClassID_t> FromsOfB11 =
      mp_Domain->getUnitsFromOf(B11);
  BOOST_CHECK_EQUAL(FromsOfB11.size(), 1);
  BOOST_CHECK(std::count(FromsOfB11.begin(),FromsOfB11.end(),A3));

  std::list<openfluid::core::UnitClassID_t> ToOfB11 = mp_Domain->getUnitsToOf(
      B11);
  BOOST_CHECK_EQUAL(ToOfB11.size(), 1);
  BOOST_CHECK(std::count(ToOfB11.begin(),ToOfB11.end(),B3));

  std::list<openfluid::core::UnitClassID_t> ParentsOfB11 =
      mp_Domain->getUnitsParentsOf(B11);
  BOOST_CHECK_EQUAL(ParentsOfB11.size(), 2);
  BOOST_CHECK(std::count(ParentsOfB11.begin(),ParentsOfB11.end(),P1));
  BOOST_CHECK(std::count(ParentsOfB11.begin(),ParentsOfB11.end(),A3));

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsChildrenOf(B11).size(), 0);

  std::list<openfluid::core::UnitClassID_t> ChildrenOfA3 =
      mp_Domain->getUnitsChildrenOf(A3);
  BOOST_CHECK_EQUAL(ChildrenOfA3.size(), 1);
  BOOST_CHECK(std::count(ChildrenOfA3.begin(),ChildrenOfA3.end(),B11));
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_addRelations_Children)
{
  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsA", 3)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  std::list<openfluid::core::UnitClassID_t> UnitsToAdd;
  UnitsToAdd.push_back(B11);
  mp_Dialog->getChildWidget()->addUnits(UnitsToAdd);
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsB", 11)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  std::list<openfluid::core::UnitClassID_t> Widget_Froms =
      mp_Dialog->getFromWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Tos =
      mp_Dialog->getToWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Parents =
      mp_Dialog->getParentWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Children =
      mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 1);
  BOOST_CHECK(std::count(Widget_Froms.begin(),Widget_Froms.end(),A3));

  BOOST_CHECK_EQUAL(Widget_Tos.size(), 1);
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),B3));

  BOOST_CHECK_EQUAL(Widget_Parents.size(), 2);
  BOOST_CHECK(std::count(Widget_Parents.begin(),Widget_Parents.end(),P1));
  BOOST_CHECK(std::count(Widget_Parents.begin(),Widget_Parents.end(),A3));

  BOOST_CHECK_EQUAL(Widget_Children.size(), 0);

  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  std::list<openfluid::core::UnitClassID_t> FromsOfB11 =
      mp_Domain->getUnitsFromOf(B11);
  BOOST_CHECK_EQUAL(FromsOfB11.size(), 1);
  BOOST_CHECK(std::count(FromsOfB11.begin(),FromsOfB11.end(),A3));

  std::list<openfluid::core::UnitClassID_t> ToOfB11 = mp_Domain->getUnitsToOf(
      B11);
  BOOST_CHECK_EQUAL(ToOfB11.size(), 1);
  BOOST_CHECK(std::count(ToOfB11.begin(),ToOfB11.end(),B3));

  std::list<openfluid::core::UnitClassID_t> ParentsOfB11 =
      mp_Domain->getUnitsParentsOf(B11);
  BOOST_CHECK_EQUAL(ParentsOfB11.size(), 2);
  BOOST_CHECK(std::count(ParentsOfB11.begin(),ParentsOfB11.end(),P1));
  BOOST_CHECK(std::count(ParentsOfB11.begin(),ParentsOfB11.end(),A3));

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsChildrenOf(B11).size(), 0);

  std::list<openfluid::core::UnitClassID_t> ChildrenOfA3 =
      mp_Domain->getUnitsChildrenOf(A3);
  BOOST_CHECK_EQUAL(ChildrenOfA3.size(), 1);
  BOOST_CHECK(std::count(ChildrenOfA3.begin(),ChildrenOfA3.end(),B11));
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_addExistingRelation)
{
  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsA", 3)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  std::list<openfluid::core::UnitClassID_t> UnitsToAdd;
  UnitsToAdd.push_back(P1);
  mp_Dialog->getParentWidget()->addUnits(UnitsToAdd);
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(), 1);
  std::list<openfluid::core::UnitClassID_t> Widget_Parents =
      mp_Dialog->getParentWidget()->getUnits();
  BOOST_CHECK_EQUAL(Widget_Parents.size(), 1);
  BOOST_CHECK(std::count(Widget_Parents.begin(),Widget_Parents.end(), P1));
  BOOST_CHECK_EQUAL(mp_Dialog->getChildWidget()->getUnits().size(), 0);

  std::list<openfluid::core::UnitClassID_t> ParentsOfA3 =
      mp_Domain->getUnitsParentsOf(A3);
  BOOST_CHECK_EQUAL(ParentsOfA3.size(), 1);
  BOOST_CHECK(std::count(ParentsOfA3.begin(), ParentsOfA3.end(),P1));
  BOOST_CHECK_EQUAL(mp_Domain->getUnitsChildrenOf(A3).size(), 0);

  std::list<openfluid::core::UnitClassID_t> ChildrenOfP1 =
      mp_Domain->getUnitsChildrenOf(P1);
  BOOST_CHECK_EQUAL(ChildrenOfP1.size(), 2);
  BOOST_CHECK(std::count(ChildrenOfP1.begin(), ChildrenOfP1.end(),A3));
  BOOST_CHECK(std::count(ChildrenOfP1.begin(), ChildrenOfP1.end(),B11));
  BOOST_CHECK_EQUAL(mp_Domain->getUnitsParentsOf(P1).size(), 0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_addItself)
{
  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsA", 3)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  std::list<openfluid::core::UnitClassID_t> UnitsToAdd;
  UnitsToAdd.push_back(A3);
  mp_Dialog->getParentWidget()->addUnits(UnitsToAdd);
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  std::list<openfluid::core::UnitClassID_t> ParentsOfA3 =
      mp_Domain->getUnitsParentsOf(A3);
  BOOST_CHECK_EQUAL(ParentsOfA3.size(), 2);
  BOOST_CHECK(std::count(ParentsOfA3.begin(), ParentsOfA3.end(),P1));
  BOOST_CHECK(std::count(ParentsOfA3.begin(), ParentsOfA3.end(),A3));
  std::list<openfluid::core::UnitClassID_t> ChildrenOfA3 =
      mp_Domain->getUnitsChildrenOf(A3);
  BOOST_CHECK_EQUAL(ChildrenOfA3.size(), 1);
  BOOST_CHECK(std::count(ChildrenOfA3.begin(),ChildrenOfA3.end(), A3));

  std::list<openfluid::core::UnitClassID_t> ChildrenOfP1 =
      mp_Domain->getUnitsChildrenOf(P1);
  BOOST_CHECK_EQUAL(ChildrenOfP1.size(), 2);
  BOOST_CHECK(std::count(ChildrenOfP1.begin(), ChildrenOfP1.end(),A3));
  BOOST_CHECK(std::count(ChildrenOfP1.begin(), ChildrenOfP1.end(),B11));
  BOOST_CHECK_EQUAL(mp_Domain->getUnitsParentsOf(P1).size(), 0);

  //need reinit for widgets to update
  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsA", 3)));
  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  BOOST_CHECK_EQUAL(mp_Dialog->getFromWidget()->getUnits().size(), 0);
  BOOST_CHECK_EQUAL(mp_Dialog->getToWidget()->getUnits().size(), 1);
  std::list<openfluid::core::UnitClassID_t> Widget_Parents =
      mp_Dialog->getParentWidget()->getUnits();
  BOOST_CHECK_EQUAL(Widget_Parents.size(), 2);
  BOOST_CHECK(std::count(Widget_Parents.begin(),Widget_Parents.end(), P1));
  BOOST_CHECK(std::count(Widget_Parents.begin(),Widget_Parents.end(), A3));
  std::list<openfluid::core::UnitClassID_t> Widget_Children =
      mp_Dialog->getChildWidget()->getUnits();
  BOOST_CHECK_EQUAL(Widget_Children.size(), 1);
  BOOST_CHECK(std::count(Widget_Children.begin(),Widget_Children.end(), A3));

  ParentsOfA3 = mp_Domain->getUnitsParentsOf(A3);
  BOOST_CHECK_EQUAL(ParentsOfA3.size(), 2);
  BOOST_CHECK(std::count(ParentsOfA3.begin(), ParentsOfA3.end(),P1));
  BOOST_CHECK(std::count(ParentsOfA3.begin(), ParentsOfA3.end(),A3));
  ChildrenOfA3 = mp_Domain->getUnitsChildrenOf(A3);
  BOOST_CHECK_EQUAL(ChildrenOfA3.size(), 1);
  BOOST_CHECK(std::count(ChildrenOfA3.begin(),ChildrenOfA3.end(), A3));

  ChildrenOfP1 = mp_Domain->getUnitsChildrenOf(P1);
  BOOST_CHECK_EQUAL(ChildrenOfP1.size(), 2);
  BOOST_CHECK(std::count(ChildrenOfP1.begin(), ChildrenOfP1.end(),A3));
  BOOST_CHECK(std::count(ChildrenOfP1.begin(), ChildrenOfP1.end(),B11));
  BOOST_CHECK_EQUAL(mp_Domain->getUnitsParentsOf(P1).size(), 0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_removeRelation_To)
{
  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsA", 3)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  mp_Dialog->getToWidget()->getTreeView()->get_selection()->select(
      mp_Dialog->getToWidget()->getTreeModel()->children().begin());
  mp_Dialog->getToWidget()->onRemoveClicked();

  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  std::list<openfluid::core::UnitClassID_t> Widget_Froms =
      mp_Dialog->getFromWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Tos =
      mp_Dialog->getToWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Parents =
      mp_Dialog->getParentWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Children =
      mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 0);
  BOOST_CHECK_EQUAL(Widget_Tos.size(), 0);
  BOOST_CHECK_EQUAL(Widget_Parents.size(), 1);
  BOOST_CHECK(std::count(Widget_Parents.begin(),Widget_Parents.end(),P1));
  BOOST_CHECK_EQUAL(Widget_Children.size(), 0);

  std::list<openfluid::core::UnitClassID_t> FromsOfB11 =
      mp_Domain->getUnitsFromOf(B11);
  BOOST_CHECK_EQUAL(FromsOfB11.size(), 0);

  std::list<openfluid::core::UnitClassID_t> ToOfB11 = mp_Domain->getUnitsToOf(
      B11);
  BOOST_CHECK_EQUAL(ToOfB11.size(), 1);
  BOOST_CHECK(std::count(ToOfB11.begin(),ToOfB11.end(),B3));

  std::list<openfluid::core::UnitClassID_t> ParentsOfB11 =
      mp_Domain->getUnitsParentsOf(B11);
  BOOST_CHECK_EQUAL(ParentsOfB11.size(), 1);
  BOOST_CHECK(std::count(ParentsOfB11.begin(),ParentsOfB11.end(),P1));

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsChildrenOf(B11).size(), 0);

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsToOf(A3).size(), 0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_removeRelation_From)
{
  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsB", 11)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  mp_Dialog->getFromWidget()->getTreeView()->get_selection()->select(
      mp_Dialog->getFromWidget()->getTreeModel()->children().begin());
  mp_Dialog->getFromWidget()->onRemoveClicked();

  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  std::list<openfluid::core::UnitClassID_t> Widget_Froms =
      mp_Dialog->getFromWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Tos =
      mp_Dialog->getToWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Parents =
      mp_Dialog->getParentWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Children =
      mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 0);
  BOOST_CHECK_EQUAL(Widget_Tos.size(), 1);
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),B3));
  BOOST_CHECK_EQUAL(Widget_Parents.size(), 1);
  BOOST_CHECK(std::count(Widget_Parents.begin(),Widget_Parents.end(),P1));
  BOOST_CHECK_EQUAL(Widget_Children.size(), 0);

  std::list<openfluid::core::UnitClassID_t> FromsOfB11 =
      mp_Domain->getUnitsFromOf(B11);
  BOOST_CHECK_EQUAL(FromsOfB11.size(), 0);

  std::list<openfluid::core::UnitClassID_t> ToOfB11 = mp_Domain->getUnitsToOf(
      B11);
  BOOST_CHECK_EQUAL(ToOfB11.size(), 1);
  BOOST_CHECK(std::count(ToOfB11.begin(),ToOfB11.end(),B3));

  std::list<openfluid::core::UnitClassID_t> ParentsOfB11 =
      mp_Domain->getUnitsParentsOf(B11);
  BOOST_CHECK_EQUAL(ParentsOfB11.size(), 1);
  BOOST_CHECK(std::count(ParentsOfB11.begin(),ParentsOfB11.end(),P1));

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsChildrenOf(B11).size(), 0);

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsToOf(A3).size(), 0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_removeRelations_Parent)
{
  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsB", 11)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  mp_Dialog->getParentWidget()->getTreeView()->get_selection()->select(
      mp_Dialog->getParentWidget()->getTreeModel()->children().begin());
  mp_Dialog->getParentWidget()->onRemoveClicked();

  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  std::list<openfluid::core::UnitClassID_t> Widget_Froms =
      mp_Dialog->getFromWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Tos =
      mp_Dialog->getToWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Parents =
      mp_Dialog->getParentWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Children =
      mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 1);
  BOOST_CHECK(std::count(Widget_Froms.begin(),Widget_Froms.end(),A3));

  BOOST_CHECK_EQUAL(Widget_Tos.size(), 1);
  BOOST_CHECK(std::count(Widget_Tos.begin(),Widget_Tos.end(),B3));

  BOOST_CHECK_EQUAL(Widget_Parents.size(), 0);

  BOOST_CHECK_EQUAL(Widget_Children.size(), 0);

  std::list<openfluid::core::UnitClassID_t> FromsOfB11 =
      mp_Domain->getUnitsFromOf(B11);
  BOOST_CHECK_EQUAL(FromsOfB11.size(), 1);
  BOOST_CHECK(std::count(FromsOfB11.begin(),FromsOfB11.end(),A3));

  std::list<openfluid::core::UnitClassID_t> ToOfB11 = mp_Domain->getUnitsToOf(
      B11);
  BOOST_CHECK_EQUAL(ToOfB11.size(), 1);
  BOOST_CHECK(std::count(ToOfB11.begin(),ToOfB11.end(),B3));

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsParentsOf(B11).size(), 0);

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsChildrenOf(B11).size(), 0);

  std::list<openfluid::core::UnitClassID_t> ChildrenOfP1 =
      mp_Domain->getUnitsChildrenOf(P1);
  BOOST_CHECK_EQUAL(ChildrenOfP1.size(), 1);
  BOOST_CHECK(std::count(ChildrenOfP1.begin(),ChildrenOfP1.end(),A3));
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_removeRelations_Children)
{
  mp_Dialog->setUnit(
      const_cast<openfluid::fluidx::UnitDescriptor*>(&mp_Domain->getUnitDescriptor(
          "unitsP", 1)));

  mp_Dialog->initEditionMode();
  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  mp_Dialog->getChildWidget()->getTreeView()->get_selection()->select(
      mp_Dialog->getChildWidget()->getTreeModel()->children().begin().operator ++());
  mp_Dialog->getChildWidget()->onRemoveClicked();

  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  openfluid::core::UnitClassID_t A3 = std::make_pair("unitsA", 3);
  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t P1 = std::make_pair("unitsP", 1);

  std::list<openfluid::core::UnitClassID_t> Widget_Froms =
      mp_Dialog->getFromWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Tos =
      mp_Dialog->getToWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Parents =
      mp_Dialog->getParentWidget()->getUnits();
  std::list<openfluid::core::UnitClassID_t> Widget_Children =
      mp_Dialog->getChildWidget()->getUnits();

  BOOST_CHECK_EQUAL(Widget_Froms.size(), 0);

  BOOST_CHECK_EQUAL(Widget_Tos.size(), 0);

  BOOST_CHECK_EQUAL(Widget_Parents.size(), 0);

  BOOST_CHECK_EQUAL(Widget_Children.size(), 1);
  BOOST_CHECK(std::count(Widget_Children.begin(),Widget_Children.end(),A3));

  std::list<openfluid::core::UnitClassID_t> FromsOfB11 =
      mp_Domain->getUnitsFromOf(B11);
  BOOST_CHECK_EQUAL(FromsOfB11.size(), 1);
  BOOST_CHECK(std::count(FromsOfB11.begin(),FromsOfB11.end(),A3));

  std::list<openfluid::core::UnitClassID_t> ToOfB11 = mp_Domain->getUnitsToOf(
      B11);
  BOOST_CHECK_EQUAL(ToOfB11.size(), 1);
  BOOST_CHECK(std::count(ToOfB11.begin(),ToOfB11.end(),B3));

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsParentsOf(B11).size(), 0);

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsChildrenOf(B11).size(), 0);

  std::list<openfluid::core::UnitClassID_t> ChildrenOfP1 =
      mp_Domain->getUnitsChildrenOf(P1);
  BOOST_CHECK_EQUAL(ChildrenOfP1.size(), 1);
  BOOST_CHECK(std::count(ChildrenOfP1.begin(),ChildrenOfP1.end(),A3));
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_addUnit)
{
  mp_Dialog->initCreationMode("");

  mp_Dialog->getClassComboEntryText()->get_entry()->set_text("unitsZ");
  mp_Dialog->getIdSpinValue()->set_value(100);

  openfluid::core::UnitClassID_t B3 = std::make_pair("unitsB", 3);
  openfluid::core::UnitClassID_t B11 = std::make_pair("unitsB", 11);
  openfluid::core::UnitClassID_t Z100 = std::make_pair("unitsZ", 100);

  std::list<openfluid::core::UnitClassID_t> UnitsToAdd;
  UnitsToAdd.push_back(B11);

  mp_Dialog->getFromWidget()->addUnits(UnitsToAdd);

  mp_Dialog->createUnit();
  mp_Domain->addUnit(mp_Dialog->getUnit());

  mp_Dialog->clearAllRelations();
  mp_Dialog->createAllRelations();

  std::list<openfluid::core::UnitClassID_t> FromsOfZ100 =
      mp_Domain->getUnitsFromOf(Z100);
  BOOST_CHECK_EQUAL(FromsOfZ100.size(), 1);
  BOOST_CHECK(std::count(FromsOfZ100.begin(),FromsOfZ100.end(),B11));

  BOOST_CHECK_EQUAL(mp_Domain->getUnitsToOf(Z100).size(), 0);
  BOOST_CHECK_EQUAL(mp_Domain->getUnitsParentsOf(Z100).size(), 0);
  BOOST_CHECK_EQUAL(mp_Domain->getUnitsChildrenOf(Z100).size(), 0);

  std::list<openfluid::core::UnitClassID_t> TosOfB11 = mp_Domain->getUnitsToOf(
      B11);
  BOOST_CHECK_EQUAL(TosOfB11.size(), 2);
  BOOST_CHECK(std::count(TosOfB11.begin(),TosOfB11.end(),B3));
  BOOST_CHECK(std::count(TosOfB11.begin(),TosOfB11.end(),Z100));

}
// =====================================================================
// =====================================================================
BOOST_AUTO_TEST_SUITE_END();
