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

      mp_Component = new DomainStructureComponent();
      mp_Model = (DomainStructureModelSub*) (mp_Component->getModel());
      mp_View = (DomainStructureViewSub*) (mp_Component->getView());
    }

    ~init_Presenter()
    {
      delete mp_Component;
    }
};

BOOST_FIXTURE_TEST_SUITE(DomainStructurePresenterTest, init_Presenter)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_constructors)
{
  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(),"");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(),0);
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(),"");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(),0);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(),-1);
}

BOOST_AUTO_TEST_CASE(test_SetEmptyCoreRepos)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(),"");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(),0);
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(),"");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(),0);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(),-1);
}

BOOST_AUTO_TEST_CASE(test_SetCoreRepos)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  unsigned int ClassCount = EngProject->getCoreRepository().getUnitsByClass()->size();
  unsigned int FirstClassUnitsCount = EngProject->getCoreRepository().getUnitsByClass()->begin()->second.getList()->size();
  std::string FirstClassName = EngProject->getCoreRepository().getUnitsByClass()->begin()->first;

  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(),"");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(),ClassCount);
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(),FirstClassName);
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(),FirstClassUnitsCount);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(),-1);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_addUnit)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  openfluid::core::Unit U("class A",100,2, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U);

  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(),"class A");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(),1);
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(),"class A");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(),1);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(),-1);

  openfluid::core::Unit U2("class B",200,3, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U2);

  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(),"class B");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(),2);
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(),"class B");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(),1);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(),-1);

  openfluid::core::Unit U3("class B",300,4, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U3);

  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(),"class B");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(),2);
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(),"class B");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(),2);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(),-1);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_selectClass)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  openfluid::core::Unit U("class A",100,2, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U);
  openfluid::core::Unit U2("class B",200,3, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U2);
  openfluid::core::Unit U3("class B",300,4, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U3);

  // select class B
  mp_View->selectClassWithIndex(1);

  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(),"class B");
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(),"class B");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(),2);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(),-1);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_selectUnit)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  openfluid::core::Unit U("class A",100,2, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U);
  openfluid::core::Unit U2("class B",200,3, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U2);
  openfluid::core::Unit U3("class B",300,4, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U3);

  // select U3
  mp_View->selectUnitWithIndex(1);

  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(),"class B");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(),2);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnit()->getID(),300);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(),"class B");
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(),300);

  // select U
  mp_View->selectClassWithIndex(0);
  mp_View->selectUnitWithIndex(0);

  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(),"class A");
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(),1);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnit()->getID(),100);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(),"class A");
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(),100);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_deleteSelectedUnit)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  openfluid::core::Unit U("class A",100,2, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U);
  openfluid::core::Unit U2("class B",200,3, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U2);
  openfluid::core::Unit U3("class B",300,4, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U3);

  // select U2
  mp_View->selectClassWithIndex(1);
  mp_View->selectUnitWithIndex(0);

  mp_Model->deleteSelectedUnit();

  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(),"class B");
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(),"class B");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(),2);
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(),1);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(),-1);

  // select U
  mp_View->selectClassWithIndex(0);
  mp_View->selectUnitWithIndex(0);

  mp_Model->deleteSelectedUnit();

  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(),"class B");
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(),"class B");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(),1);
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(),1);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(),-1);

  // select U3
  mp_View->selectUnitWithIndex(0);

  mp_Model->deleteSelectedUnit();

  BOOST_CHECK_EQUAL(mp_Model->getSelectedClass(),"");
  BOOST_CHECK_EQUAL(mp_View->getSelectedClassName(),"");
  BOOST_CHECK_EQUAL(mp_View->getClassesViewRowCount(),0);
  BOOST_CHECK_EQUAL(mp_View->getUnitsViewRowCount(),0);
  BOOST_CHECK_EQUAL(mp_View->getSelectedUnitId(),-1);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_alterUnit)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  openfluid::core::Unit U("class A",100,2, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U);
  openfluid::core::Unit U2("class B",200,3, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U2);
  openfluid::core::Unit U3("class B",300,4, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(&U3);

  // select U2
  mp_View->selectClassWithIndex(1);
  mp_View->selectUnitWithIndex(0);

  EngProject->getCoreRepository().getUnit("class B",200)->setProcessOrder(30);

  mp_Model->alterSelectedUnit();

  int DisplayedPcsOrder = 0;
  mp_View->getSelectedUnitIter()->get_value(1,DisplayedPcsOrder);
  BOOST_CHECK_EQUAL(DisplayedPcsOrder,30);

  delete EngProject;
}
// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END()

