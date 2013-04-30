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

//#include "BuilderTestHelper.hpp"
//#include "DomainStructureModel.hpp"
//#include "EngineProject.hpp"
//#include "tests-config.hpp"
//#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>

// =====================================================================
// =====================================================================
//
//struct init_Model
//{
//    DomainStructureModelSub* mp_Model;
//
//    init_Model()
//    {
//      BuilderTestHelper::getInstance()->initGtk();
//    }
//
//    ~init_Model()
//    {
//      delete mp_Model;
//    }
//};
//
//BOOST_FIXTURE_TEST_SUITE(DomainStructureModelTest, init_Model)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_setDomainEmpty)
{
//  EngineProject* EngProject = new EngineProject();
//
//  mp_Model = new DomainStructureModelSub(EngProject->getAdvancedDesc().getDomain());
//
//  BOOST_CHECK_EQUAL(mp_Model->isEmpty(), true);
//  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().empty(), true);
//  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);
//
//  delete EngProject;
}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(test_setDomainNonEmpty)
//{
//  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
//      + "/OPENFLUID.IN.BuilderUnitTesting";
//  EngineProject* EngProject = new EngineProject(Path);
//
//  mp_Model = new DomainStructureModelSub(EngProject->getAdvancedDesc().getDomain());
//
//  BOOST_CHECK_EQUAL(mp_Model->isEmpty(), false);
//  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().size(), 2);
//  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().begin()->second.size(), 2);
//
//  delete EngProject;
//}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(test_setSelectedUnit)
//{
//  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
//      + "/OPENFLUID.IN.BuilderUnitTesting";
//  EngineProject* EngProject = new EngineProject(Path);
//
//  mp_Model = new DomainStructureModelSub(EngProject->getAdvancedDesc().getDomain());
//
//  mp_Model->setCurrentSelectionByUser(std::make_pair("TestUnits", 1));
//
//  BOOST_CHECK_EQUAL(
//      const_cast<openfluid::fluidx::UnitDescriptor*>(mp_Model->getSelectedUnit())->getUnitClass(),
//      "TestUnits");
//  BOOST_CHECK_EQUAL(
//      const_cast<openfluid::fluidx::UnitDescriptor*>(mp_Model->getSelectedUnit())->getUnitID(),
//      1);
//
//  delete EngProject;
//}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(test_setSelectedUnitWithWrongParams)
//{
//  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
//      + "/OPENFLUID.IN.BuilderUnitTesting";
//  EngineProject* EngProject = new EngineProject(Path);
//
//  mp_Model = new DomainStructureModelSub(EngProject->getAdvancedDesc().getDomain());
//
//  mp_Model->setCurrentSelectionByUser(std::make_pair("wrong class", 100));
//  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);
//
//  mp_Model->setCurrentSelectionByUser(std::make_pair("TestUnits", -1));
//  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);
//
//  delete EngProject;
//}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(test_deleteUnit)
//{
//  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
//      + "/OPENFLUID.IN.BuilderUnitTesting";
//  EngineProject* EngProject = new EngineProject(Path);
//
//  openfluid::fluidx::AdvancedDomainDescriptor* Domain = &(EngProject->getAdvancedDesc().getDomain());
//
//  mp_Model = new DomainStructureModelSub(EngProject->getAdvancedDesc().getDomain());
//
//  mp_Model->deleteUnit(&Domain->getUnitDescriptor("ParentTestUnits", 1));
//
//  BOOST_CHECK_EQUAL(mp_Model->isEmpty(), false);
//  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().size(), 2);
//
//  BOOST_CHECK_EQUAL(Domain->getIDsOfClass("ParentTestUnits").size(), 1);
//  BOOST_CHECK_EQUAL(*Domain->getIDsOfClass("ParentTestUnits").begin(), 2);
//
//  delete EngProject;
//}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(test_deleteLastUnitOfClass)
//{
//  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
//      + "/OPENFLUID.IN.BuilderUnitTesting";
//  EngineProject* EngProject = new EngineProject(Path);
//
//  openfluid::fluidx::AdvancedDomainDescriptor* Domain = &(EngProject->getAdvancedDesc().getDomain());
//
//  mp_Model = new DomainStructureModelSub(EngProject->getAdvancedDesc().getDomain());
//
//  mp_Model->deleteUnit(&Domain->getUnitDescriptor("ParentTestUnits", 1));
//  mp_Model->deleteUnit(&Domain->getUnitDescriptor("ParentTestUnits", 2));
//
//  BOOST_CHECK_EQUAL(mp_Model->getUnitListByClass().size(), 1);
//
//  delete EngProject;
//}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_SUITE_END();
