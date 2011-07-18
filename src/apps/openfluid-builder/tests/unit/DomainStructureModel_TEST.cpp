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
 \file DomainStructureModel_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_DomainStructureModel
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "DomainStructureModel.hpp"
#include "EngineProject.hpp"
#include "tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_Model
{
    DomainStructureModelSub* mp_Model;

    init_Model()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Model = new DomainStructureModelSub();
    }

    ~init_Model()
    {
      delete mp_Model;
    }
};

BOOST_FIXTURE_TEST_SUITE(DomainStructureModelTest, init_Model)

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_constructor)
{
  BOOST_CHECK_THROW(mp_Model->getCoreRepos(),openfluid::base::OFException);
  BOOST_CHECK_EQUAL(mp_Model->isEmpty(),true);
  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().empty(),true);
  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_setCoreReposEmpty)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  BOOST_CHECK(mp_Model->getCoreRepos() == &(EngProject->getCoreRepository()));
  BOOST_CHECK_EQUAL(mp_Model->isEmpty(),true);
  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().empty(),true);

  delete EngProject;
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_setCoreReposNonEmpty)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  unsigned int ClassCount = EngProject->getCoreRepository().getUnitsByClass()->size();
  unsigned int FirstClassUnitsCount = EngProject->getCoreRepository().getUnitsByClass()->begin()->second.getList()->size();

  BOOST_CHECK(mp_Model->getCoreRepos() == &(EngProject->getCoreRepository()));
  BOOST_CHECK_EQUAL(mp_Model->isEmpty(),false);
  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().size(), ClassCount);
  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().begin()->second.getList()->size(), FirstClassUnitsCount);

  delete EngProject;
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_setSelectedUnit)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  std::string FirstClassName = EngProject->getCoreRepository().getUnitsByClass()->begin()->first;
  int FirstClassFirstId = EngProject->getCoreRepository().getUnitsByClass()->begin()->second.getList()->begin()->getID();

  mp_Model->setCurrentSelectionByUser(std::make_pair(FirstClassName,FirstClassFirstId));

  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnit()->getClass(),FirstClassName);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnit()->getID(),FirstClassFirstId);

  delete EngProject;
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_setSelectedUnitWithWrongParams)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = new EngineProject(Path);

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  std::string FirstClassName = EngProject->getCoreRepository().getUnitsByClass()->begin()->first;

  mp_Model->setCurrentSelectionByUser(std::make_pair("wrong class",100));
  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);

  mp_Model->setCurrentSelectionByUser(std::make_pair(FirstClassName,-1));
  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);

  delete EngProject;
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_addUnit)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  openfluid::core::Unit* U = new openfluid::core::Unit("class A",100,2, openfluid::core::Unit::DESCRIPTOR);
  mp_Model->addUnit(U);

  BOOST_CHECK_EQUAL(mp_Model->isEmpty(),false);
  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().size(), 1);
  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().begin()->first, "class A");
  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().begin()->second.getList()->size(), 1);
  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().begin()->second.getList()->begin()->getID(), 100);
  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().begin()->second.getList()->begin()->getProcessOrder(), 2);

  BOOST_CHECK_EQUAL((*EngProject->getCoreRepository().getUnitsGlobally()->begin())->getClass(),"class A");

  delete EngProject;
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_addUnits)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  // addUnits
  openfluid::core::Unit* U = new openfluid::core::Unit("class A",100,2, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(U);
  openfluid::core::Unit* U2 = new openfluid::core::Unit("class B",200,3, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(U2);
  openfluid::core::Unit* U3 = new openfluid::core::Unit("class B",300,4, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(U3);

  openfluid::core::UnitsList_t ListA = *(mp_Model->getUnitListByClass()["class A"].getList());
  openfluid::core::UnitsList_t ListB = *(mp_Model->getUnitListByClass()["class B"].getList());
  openfluid::core::UnitsList_t::iterator itA = ListA.begin();
  openfluid::core::UnitsList_t::iterator itB = ListB.begin();

  BOOST_CHECK_EQUAL(mp_Model->isEmpty(),false);
  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().size(), 2);

  BOOST_CHECK_EQUAL(ListA.size(),1);
  BOOST_CHECK_EQUAL(itA->getID(),100);
  BOOST_CHECK_EQUAL(itA->getProcessOrder(),2);

  BOOST_CHECK_EQUAL(ListB.size(),2);
  BOOST_CHECK_EQUAL(itB->getID(),200);
  BOOST_CHECK_EQUAL(itB->getProcessOrder(),3);
  itB ++;
  BOOST_CHECK_EQUAL(itB->getID(),300);
  BOOST_CHECK_EQUAL(itB->getProcessOrder(),4);

  delete EngProject;
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_deleteUnit)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  // addUnits
  openfluid::core::Unit* U = new openfluid::core::Unit("class A",100,2, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(U);
  openfluid::core::Unit* U2 = new openfluid::core::Unit("class A",200,3, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(U2);

  mp_Model->deleteUnit(EngProject->getCoreRepository().getUnit("class A",100));

  openfluid::core::UnitsList_t ListA = *(mp_Model->getUnitListByClass()["class A"].getList());
  openfluid::core::UnitsList_t::iterator itA = ListA.begin();

  BOOST_CHECK_EQUAL(mp_Model->isEmpty(),false);
  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().size(), 1);

  BOOST_CHECK_EQUAL(ListA.size(),1);
  BOOST_CHECK_EQUAL(itA->getID(),200);
  BOOST_CHECK_EQUAL(itA->getProcessOrder(),3);

  delete EngProject;
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_deleteLastUnitOfClass)
{
  EngineProject* EngProject = new EngineProject();

  mp_Model->setEngineRequirements(EngProject->getCoreRepository());

  // addUnits
  openfluid::core::Unit* U = new openfluid::core::Unit("class A",100,2, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(U);
  openfluid::core::Unit* U2 = new openfluid::core::Unit("class A",200,3, openfluid::core::Unit::SIMULATION);
  mp_Model->addUnit(U2);

  mp_Model->deleteUnit(EngProject->getCoreRepository().getUnit("class A",100));
  mp_Model->deleteUnit(EngProject->getCoreRepository().getUnit("class A",200));

  BOOST_CHECK_EQUAL(mp_Model->isEmpty(),true);
  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().empty(), true);

  delete EngProject;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
