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
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>

// =====================================================================
// =====================================================================

struct init_Model
{
    DomainIDataModelImpl* mp_Model;
    EngineProject* mp_EngProject;
    openfluid::fluidx::AdvancedDomainDescriptor* mp_Domain;

    init_Model()
    {
      BuilderTestHelper::getInstance()->initGtk();

      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.BuilderUnitTesting";
      mp_EngProject = new EngineProject(Path);

      mp_Domain = &mp_EngProject->getAdvancedDesc().getDomain();

      mp_Model = new DomainIDataModelImpl(*mp_Domain);
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
  BOOST_CHECK_EQUAL(mp_Model->getClass(), "");

  mp_Model->setClass("TestUnits");

  BOOST_CHECK_EQUAL(mp_Model->getClass(), "TestUnits");
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), false);

  mp_Model->setClass("ParentTestUnits");

  BOOST_CHECK_EQUAL(mp_Model->getClass(), "ParentTestUnits");
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), true);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_removeData)
{
  mp_Model->setClass("TestUnits");

  std::set<std::string> IDataNames = mp_Domain->getInputDataNames("TestUnits");

  BOOST_CHECK_EQUAL(IDataNames.count("indataA"), true);
  BOOST_CHECK_EQUAL(IDataNames.count("indataB"), true);
  BOOST_CHECK_EQUAL(IDataNames.count("indataC"), true);
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), false);

  mp_Model->removeData("indataB");
  IDataNames = mp_Domain->getInputDataNames("TestUnits");

  BOOST_CHECK_EQUAL(IDataNames.count("indataA"), true);
  BOOST_CHECK_EQUAL(IDataNames.count("indataB"), false);
  BOOST_CHECK_EQUAL(IDataNames.count("indataC"), true);
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), false);

  BOOST_CHECK_THROW(mp_Model->removeData("wrongIndata"),
                    openfluid::base::OFException);
  IDataNames = mp_Domain->getInputDataNames("TestUnits");

  BOOST_CHECK_EQUAL(IDataNames.count("indataA"), true);
  BOOST_CHECK_EQUAL(IDataNames.count("indataB"), false);
  BOOST_CHECK_EQUAL(IDataNames.count("indataC"), true);
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), false);

  mp_Model->removeData("indataC");
  IDataNames = mp_Domain->getInputDataNames("TestUnits");

  BOOST_CHECK_EQUAL(IDataNames.count("indataA"), true);
  BOOST_CHECK_EQUAL(IDataNames.count("indataB"), false);
  BOOST_CHECK_EQUAL(IDataNames.count("indataC"), false);
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), false);

  mp_Model->removeData("indataA");
  IDataNames = mp_Domain->getInputDataNames("TestUnits");

  BOOST_CHECK_EQUAL(IDataNames.count("indataA"), false);
  BOOST_CHECK_EQUAL(IDataNames.count("indataB"), false);
  BOOST_CHECK_EQUAL(IDataNames.count("indataC"), false);
  BOOST_CHECK_EQUAL(mp_Model->isEmptyDataList(), true);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_addData)
{
  mp_Model->setClass("TestUnits");

  std::set<std::string> IDataNames = mp_Domain->getInputDataNames("TestUnits");

  BOOST_CHECK_EQUAL(IDataNames.count("NewIndata"), false);

  mp_Model->addData("NewIndata", "DefaultVal");
  IDataNames = mp_Domain->getInputDataNames("TestUnits");

  BOOST_CHECK_EQUAL(IDataNames.count("NewIndata"), true);
  BOOST_CHECK_EQUAL(mp_Domain->getInputData("TestUnits",1,"NewIndata"),
                    "DefaultVal");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_changeDataName)
{
  mp_Model->setClass("TestUnits");

  std::string ValOrigine = mp_Domain->getInputData("TestUnits", 6, "indataB");

  mp_Model->changeDataName("indataB", "newDataB");

  std::set<std::string> IDataNames = mp_Domain->getInputDataNames("TestUnits");

  BOOST_CHECK_EQUAL(IDataNames.count("indataB"), false);
  BOOST_CHECK_EQUAL(IDataNames.count("newDataB"), true);

  BOOST_CHECK_EQUAL(mp_Domain->getInputData("TestUnits",6,"newDataB"),
                    ValOrigine);
}
// =====================================================================
// =====================================================================
BOOST_AUTO_TEST_SUITE_END();
