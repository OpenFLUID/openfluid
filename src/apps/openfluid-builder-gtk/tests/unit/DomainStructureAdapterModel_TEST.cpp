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
 \file DomainStructureAdapterModel_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_DomainStructureAdapterModel
#include <boost/test/unit_test.hpp>
//
//#include "BuilderTestHelper.hpp"
//#include "DomainStructureAdapterModel.hpp"
//#include "DomainStructureColumns.hpp"
//#include "EngineProject.hpp"
//#include "BuilderClassListColumns.hpp"
//#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
//
//// =====================================================================
//// =====================================================================
//
//struct init_AdapterModel
//{
//    DomainStructureAdapterModelSub* mp_AdapterModel;
//    DomainStructureColumns m_Columns;
//    BuilderClassListColumns m_ClassColumns;
//
//    init_AdapterModel()
//    {
//      BuilderTestHelper::getInstance()->initGtk();
//      mp_AdapterModel = new DomainStructureAdapterModelSub(m_Columns);
//    }
//
//    ~init_AdapterModel()
//    {
//      delete mp_AdapterModel;
//    }
//};
//
//BOOST_FIXTURE_TEST_SUITE(DomainStructureAdapterModelTest, init_AdapterModel)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_constructor)
{
//  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores().empty(), true);
//  BOOST_CHECK_EQUAL(mp_AdapterModel->getClassStore()->isEmpty(), true);
//  BOOST_CHECK_EQUAL(mp_AdapterModel->getRequestedSelectedClass(), "");
//  BOOST_CHECK(mp_AdapterModel->getRequestedClassSelection() == 0);
}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(test_setEmptyDomainStructure)
//{
//  EngineProject* EngProject = new EngineProject();
//
//  mp_AdapterModel->setDomainStructure(
//      EngProject->getAdvancedDesc().getDomain().getUnitsByIdByClass());
//
//  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores().empty(), true);
//  BOOST_CHECK_EQUAL(mp_AdapterModel->getClassStore()->isEmpty(), true);
//  BOOST_CHECK_EQUAL(mp_AdapterModel->getRequestedSelectedClass(), "");
//  BOOST_CHECK(mp_AdapterModel->getRequestedClassSelection() == 0);
//}
//
//// =====================================================================
//// =====================================================================
//
////BOOST_AUTO_TEST_CASE(test_setRequestedClassSelectionOnEmptyDomain)
////{
////  BOOST_CHECK_EQUAL(mp_AdapterModel->getRequestedSelectedClass(),"");
////
////  mp_AdapterModel->setRequestedClassSelection("");
////  BOOST_CHECK_EQUAL(mp_AdapterModel->getRequestedSelectedClass(),"");
////
////  mp_AdapterModel->setRequestedClassSelection("class A");
////  BOOST_CHECK_EQUAL(mp_AdapterModel->getRequestedSelectedClass(),"");
////}
//
////BOOST_AUTO_TEST_CASE(test_setRequestedClassSelectionOnNotEmptyDomain)
////{
////  EngineProject* EngProject = new EngineProject();
////
////  openfluid::core::Unit U("class A",100,2, openfluid::core::Unit::SIMULATION);
////  EngProject->getCoreRepository().addUnit(U);
////  openfluid::core::Unit U2("class B",200,3, openfluid::core::Unit::SIMULATION);
////  EngProject->getCoreRepository().addUnit(U2);
////  openfluid::core::Unit U3("class B",300,4, openfluid::core::Unit::SIMULATION);
////  EngProject->getCoreRepository().addUnit(U3);
////
////  openfluid::core::UnitsListByClassMap_t UnitListByClass = *(EngProject->getCoreRepository().getUnitsByClass());
////
////  // empty if setRequestedClassSelection is not set
////  mp_AdapterModel->setDomainStructure(UnitListByClass);
////  BOOST_CHECK_EQUAL(mp_AdapterModel->getRequestedSelectedClass(),"");
////
////  // first class selection if setRequestedClassSelection and existing selection is not valid
////  mp_AdapterModel->setRequestedClassSelection("");
////  BOOST_CHECK_EQUAL(mp_AdapterModel->getRequestedSelectedClass(),"class A");
////
////  // asked class selection if setRequestedClassSelection is valid
////  mp_AdapterModel->setRequestedClassSelection("class B");
////  BOOST_CHECK_EQUAL(mp_AdapterModel->getRequestedSelectedClass(),"class B");
////
////  // last selected class if setRequestedClassSelection is not valid and existing selection is still valid
////  mp_AdapterModel->setRequestedClassSelection("");
////  BOOST_CHECK_EQUAL(mp_AdapterModel->getRequestedSelectedClass(),"class B");
////}
//
//// =====================================================================
//// =====================================================================
//
//BOOST_AUTO_TEST_CASE(test_setDomainStructure)
//{
//  EngineProject* EngProject = new EngineProject();
//
//  openfluid::fluidx::AdvancedDomainDescriptor* Domain = &(EngProject->getAdvancedDesc().getDomain());
//
//  Domain->addUnit(createAUnitDesc("class A", 100, 2));
//  Domain->addUnit(createAUnitDesc("class B", 200, 3));
//  Domain->addUnit(createAUnitDesc("class B", 300, 4));
//
//  mp_AdapterModel->setDomainStructure(Domain->getUnitsByIdByClass());
//
//  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores().size(), 2);
//  BOOST_CHECK_EQUAL(mp_AdapterModel->getClassStore()->isEmpty(), false);
//  BOOST_CHECK_EQUAL(mp_AdapterModel->getRequestedSelectedClass(), "class A");
//  BOOST_CHECK_EQUAL(
//      mp_AdapterModel->getRequestedClassSelection()->get_value(m_ClassColumns.m_Class),
//      "class A");
//  BOOST_CHECK(mp_AdapterModel->getRequestedClassSelection());
//}
//// =====================================================================
//// =====================================================================
//BOOST_AUTO_TEST_SUITE_END();
