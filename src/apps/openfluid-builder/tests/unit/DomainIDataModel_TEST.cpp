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
 \file DomainIDataModel_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_DomainIDataModel
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "DomainIDataModel.hpp"
#include "EngineProject.hpp"
#include "tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_Model
{
    DomainIDataModelImpl* mp_Model;
    EngineProject* mp_EngProject;

    init_Model()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Model = new DomainIDataModelImpl();
      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.Primitives";
      mp_EngProject = new EngineProject(Path);

      mp_Model->setEngineRequirements(mp_EngProject->getCoreRepository());
    }

    ~init_Model()
    {
      delete mp_Model;
      delete mp_EngProject;
    }
};

BOOST_FIXTURE_TEST_SUITE(DomainIDataModelTest, init_Model)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_getUnitsColl)
{
  BOOST_CHECK(mp_Model->getUnitsCollection() == 0);

  mp_Model->setClass("TestUnits");

  BOOST_CHECK_EQUAL(mp_Model->getUnitsCollection()->getList()->size(),mp_EngProject->getCoreRepository().getUnits("TestUnits")->getList()->size());
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), false);

  mp_Model->setClass("ParentTestUnits");

  BOOST_CHECK_EQUAL(mp_Model->getUnitsCollection()->getList()->size(),mp_EngProject->getCoreRepository().getUnits("ParentTestUnits")->getList()->size());
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), true);
}

BOOST_AUTO_TEST_CASE(test_removeData)
{
  mp_Model->setClass("TestUnits");

  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataA"),true);
  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataB"),true);
  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataC"),true);
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), false);

  mp_Model->removeData("indataB");

  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataA"),true);
  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataB"),false);
  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataC"),true);
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), false);

  mp_Model->removeData("wrongIndata");

  //does nothing
  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataA"),true);
  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataB"),false);
  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataC"),true);
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), false);

  mp_Model->removeData("indataC");

  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataA"),true);
  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataB"),false);
  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataC"),false);
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), false);

  mp_Model->removeData("indataA");

  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataA"),false);
  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataB"),false);
  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataC"),false);
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), true);
}

BOOST_AUTO_TEST_CASE(test_addData)
{
  mp_Model->setClass("TestUnits");

  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("NewIndata"),false);

  mp_Model->addData(std::make_pair("NewIndata","DefaultVal"));

  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("NewIndata"),true);
  std::string Val;
  mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->getValue("NewIndata",&Val);
  BOOST_CHECK_EQUAL(Val,"DefaultVal");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
