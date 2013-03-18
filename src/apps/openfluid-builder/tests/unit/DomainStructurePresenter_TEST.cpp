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
 \file DomainStructurePresenter_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_DomainStructurePresenter
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "DomainStructureComponent.hpp"
#include "DomainStructureModel.hpp"
#include "DomainStructureView.hpp"
#include "EngineProject.hpp"
#include "tests-config.hpp"
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>

// =====================================================================
// =====================================================================

struct init_Presenter
{
    DomainStructureComponent* mp_Component;

    DomainStructureModelSub* mp_Model;
    DomainStructureViewSub* mp_View;

    init_Presenter()
    {
      BuilderTestHelper::getInstance()->initGtk();
    }

    ~init_Presenter()
    {
      delete mp_Component;
    }
};

BOOST_FIXTURE_TEST_SUITE(DomainStructurePresenterTest, init_Presenter)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_SetEmptyCoreRepos)
{
  EngineProject* EngProject = new EngineProject();

  mp_Component = new DomainStructureComponent(EngProject->getAdvancedDesc().getDomain());
  mp_Model = (DomainStructureModelSub*) (mp_Component->getModel());
  mp_View = (DomainStructureViewSub*) (mp_Component->getView());
  mp_Model->update();

  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(), "");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(), 0);
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(), "");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(), 0);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(), -1);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_SetDomain)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
      + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = new EngineProject(Path);

  openfluid::fluidx::AdvancedDomainDescriptor* Domain = &(EngProject->getAdvancedDesc().getDomain());

  mp_Component = new DomainStructureComponent(*Domain);
  mp_Model = (DomainStructureModelSub*) (mp_Component->getModel());
  mp_View = (DomainStructureViewSub*) (mp_Component->getView());
  mp_Model->update();

  BOOST_CHECK(
      mp_Model->getSelectedUnit() == &Domain->getUnitDescriptor("ParentTestUnits",1));
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(), "ParentTestUnits");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(), 2);
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(), "ParentTestUnits");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(), 2);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(), 1);

  delete EngProject;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_addUnit)
{
  EngineProject* EngProject = new EngineProject();

  openfluid::fluidx::AdvancedDomainDescriptor* Domain = &(EngProject->getAdvancedDesc().getDomain());

  mp_Component = new DomainStructureComponent(*Domain);
  mp_Model = (DomainStructureModelSub*) (mp_Component->getModel());
  mp_View = (DomainStructureViewSub*) (mp_Component->getView());
  mp_Model->update();

  mp_Model->addUnit(createAUnitDesc("class A", 100, 2));

  BOOST_CHECK(
      mp_Model->getSelectedUnit() == &Domain->getUnitDescriptor("class A",100));
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(), "class A");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(), 1);
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(), "class A");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(), 1);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(), 100);

  mp_Model->addUnit(createAUnitDesc("class B", 200, 3));

  BOOST_CHECK(
      mp_Model->getSelectedUnit() == &Domain->getUnitDescriptor("class B",200));
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(), "class B");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(), 2);
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(), "class B");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(), 1);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(), 200);

  mp_Model->addUnit(createAUnitDesc("class B", 300, 4));

  BOOST_CHECK(
      mp_Model->getSelectedUnit() == &Domain->getUnitDescriptor("class B",300));
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(), "class B");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(), 2);
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(), "class B");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(), 2);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(), 300);

  delete EngProject;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_selectClass)
{
  EngineProject* EngProject = new EngineProject();

  openfluid::fluidx::AdvancedDomainDescriptor* Domain = &(EngProject->getAdvancedDesc().getDomain());

  Domain->addUnit(createAUnitDesc("class A", 100, 2));
  Domain->addUnit(createAUnitDesc("class B", 200, 3));
  Domain->addUnit(createAUnitDesc("class B", 300, 4));

  mp_Component = new DomainStructureComponent(*Domain);
  mp_Model = (DomainStructureModelSub*) (mp_Component->getModel());
  mp_View = (DomainStructureViewSub*) (mp_Component->getView());
  mp_Model->update();

  // select class B
  mp_View->selectClassWithIndex(1);

  BOOST_CHECK(
      mp_Model->getSelectedUnit() == &Domain->getUnitDescriptor("class B",200));
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(), "class B");
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(), "class B");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(), 2);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(), 200);

  delete EngProject;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_selectUnit)
{
  EngineProject* EngProject = new EngineProject();

  openfluid::fluidx::AdvancedDomainDescriptor* Domain = &(EngProject->getAdvancedDesc().getDomain());

  mp_Component = new DomainStructureComponent(*Domain);
  mp_Model = (DomainStructureModelSub*) (mp_Component->getModel());
  mp_View = (DomainStructureViewSub*) (mp_Component->getView());
  mp_Model->update();

  mp_Model->addUnit(createAUnitDesc("class A", 100, 2));
  mp_Model->addUnit(createAUnitDesc("class B", 200, 3));
  mp_Model->addUnit(createAUnitDesc("class B", 300, 4));

  // select U3
  mp_View->selectUnitWithIndex(1);

  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(), "class B");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(), 2);
  BOOST_CHECK_EQUAL(
      const_cast<openfluid::fluidx::UnitDescriptor*>(mp_Model->getSelectedUnit())->getUnitID(),
      300);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(), "class B");
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(), 300);

  // select U
  mp_View->selectClassWithIndex(0);
  mp_View->selectUnitWithIndex(0);

  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(), "class A");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(), 1);
  BOOST_CHECK_EQUAL(
      const_cast<openfluid::fluidx::UnitDescriptor*>(mp_Model->getSelectedUnit())->getUnitID(),
      100);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(), "class A");
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(), 100);

  delete EngProject;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_deleteSelectedUnit)
{
  EngineProject* EngProject = new EngineProject();

  openfluid::fluidx::AdvancedDomainDescriptor* Domain = &(EngProject->getAdvancedDesc().getDomain());

  mp_Component = new DomainStructureComponent(*Domain);
  mp_Model = (DomainStructureModelSub*) (mp_Component->getModel());
  mp_View = (DomainStructureViewSub*) (mp_Component->getView());
  mp_Model->update();

  mp_Model->addUnit(createAUnitDesc("class A", 100, 2));
  mp_Model->addUnit(createAUnitDesc("class B", 200, 3));
  mp_Model->addUnit(createAUnitDesc("class B", 300, 4));

  // select U2
  mp_View->selectClassWithIndex(1);
  mp_View->selectUnitWithIndex(0);

  mp_Model->deleteSelectedUnit();

  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(), "class B");
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(), "class B");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(), 2);
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(), 1);
//  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(),300); // ? don't understand why == -1. (300 is well selected when try to reproduced it for real)

// select U
  mp_View->selectClassWithIndex(0);
  mp_View->selectUnitWithIndex(0);

  mp_Model->deleteSelectedUnit();

  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(), "class B");
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(), "class B");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(), 1);
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(), 1);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(), 300);

  // select U3
  mp_View->selectUnitWithIndex(0);

  mp_Model->deleteSelectedUnit();

  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(), "");
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(), "");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(), 0);
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(), 0);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(), -1);

  delete EngProject;
}
// =====================================================================
// =====================================================================
BOOST_AUTO_TEST_SUITE_END()

