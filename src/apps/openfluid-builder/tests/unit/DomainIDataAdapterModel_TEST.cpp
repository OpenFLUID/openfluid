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
 \file DomainIDataAdapterModel_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_DomainIDataAdapterModel
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "DomainIDataAdapterModel.hpp"
#include "EngineProject.hpp"
#include "tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_Model
{
    DomainIDataAdapterModelSub* mp_AdapterModel;
    EngineProject* mp_EngProject;

    init_Model()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_AdapterModel = new DomainIDataAdapterModelSub();

      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.Primitives";
      mp_EngProject = new EngineProject(Path);

      mp_AdapterModel->setCoreRepository(&mp_EngProject->getCoreRepository());
    }

    ~init_Model()
    {
      delete mp_AdapterModel;
      delete mp_EngProject;
    }
};

BOOST_FIXTURE_TEST_SUITE(DomainIDataAdapterModelTest, init_Model)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_extractClassNames)
{
  mp_AdapterModel->extractClassNames();

  BOOST_CHECK_EQUAL(mp_AdapterModel->getClassNames().size(),2);
  BOOST_CHECK(std::find(mp_AdapterModel->getClassNames().begin(),mp_AdapterModel->getClassNames().end(),"ParentTestUnits") != mp_AdapterModel->getClassNames().end());
  BOOST_CHECK(std::find(mp_AdapterModel->getClassNames().begin(),mp_AdapterModel->getClassNames().end(),"TestUnits") != mp_AdapterModel->getClassNames().end());
}

BOOST_AUTO_TEST_CASE(test_createDataColumns)
{
  mp_AdapterModel->extractClassNames();

  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassColumns().size(),0);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores().size(),0);

  mp_AdapterModel->createDataColumnsAndStoreForClass("ParentTestUnits");

  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassColumns().size(),1);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores().size(),1);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassColumns()["ParentTestUnits"]->getByTitleColumns().size(),0);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores()["ParentTestUnits"]->get_n_columns(),1);

  mp_AdapterModel->createDataColumnsAndStoreForClass("TestUnits");

  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassColumns().size(),2);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassColumns()["ParentTestUnits"]->getByTitleColumns().size(),0);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores()["ParentTestUnits"]->get_n_columns(),1);
  std::map<std::string, Gtk::TreeModelColumn<std::string>* > IDataMap = mp_AdapterModel->getByClassColumns()["TestUnits"]->getByTitleColumns();
  BOOST_CHECK_EQUAL(IDataMap.size(),3);
  BOOST_CHECK(IDataMap.find("indataA") != IDataMap.end());
  BOOST_CHECK(IDataMap.find("indataB") != IDataMap.end());
  BOOST_CHECK(IDataMap.find("indataC") != IDataMap.end());
  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores().size(),2);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores()["TestUnits"]->get_n_columns(),4);
}

BOOST_AUTO_TEST_CASE(test_populateStore)
{
  mp_AdapterModel->extractClassNames();
  mp_AdapterModel->createDataColumnsAndStoreForClass("ParentTestUnits");
  mp_AdapterModel->populateStoreForClass("ParentTestUnits");
  mp_AdapterModel->createDataColumnsAndStoreForClass("TestUnits");
  mp_AdapterModel->populateStoreForClass("TestUnits");

  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores()["ParentTestUnits"]->children().size(),2);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores()["TestUnits"]->children().size(),12);
  std::map<std::string,Gtk::TreeModelColumn<std::string>* > TestUnitsColumns = mp_AdapterModel->getByClassColumns()["TestUnits"]->getByTitleColumns();
  int FirstUnitId = mp_AdapterModel->getByClassUnitsStores()["TestUnits"]->children()[0].get_value(*mp_AdapterModel->getByClassColumns()["TestUnits"]->getIdColumn());
  openfluid::core::Unit* FirstUnit = mp_EngProject->getCoreRepository().getUnit("TestUnits",FirstUnitId);
  std::string DataVal = "";
  FirstUnit->getInputData()->getValue("indataA",&DataVal);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores()["TestUnits"]->children()[0].get_value(*TestUnitsColumns["indataA"]),DataVal);
  FirstUnit->getInputData()->getValue("indataB",&DataVal);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores()["TestUnits"]->children()[0].get_value(*TestUnitsColumns["indataB"]),DataVal);
  FirstUnit->getInputData()->getValue("indataC",&DataVal);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getByClassUnitsStores()["TestUnits"]->children()[0].get_value(*TestUnitsColumns["indataC"]),DataVal);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
