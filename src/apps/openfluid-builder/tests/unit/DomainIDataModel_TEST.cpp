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
    DomainIDataModelSub* mp_Model;
    EngineProject* mp_EngProject;

    init_Model()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Model = new DomainIDataModelSub();
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

BOOST_AUTO_TEST_CASE(test_replaceDataValue)
{
  std::pair<std::string,int> UnitInfo = std::make_pair("TestUnits",10);
  std::pair<std::string,std::string> DataInfo = std::make_pair("indataB","CODE00");

  mp_Model->replaceDataValue(UnitInfo, DataInfo);

  std::string Value = "";
  mp_EngProject->getCoreRepository().getUnit(UnitInfo.first,UnitInfo.second)->getInputData()->getValue(DataInfo.first,&Value);
  BOOST_CHECK_EQUAL(Value,"CODE00");
}

BOOST_AUTO_TEST_CASE(test_removeData)
{
  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataB"),true);

  mp_Model->setCurrentClassSelectionByUser("TestUnits");
  mp_Model->removeData("indataB");

  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataB"),false);
}

BOOST_AUTO_TEST_CASE(test_addData)
{
  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataTEST"),false);

  mp_Model->setCurrentClassSelectionByUser("TestUnits");
  mp_Model->addData(/*"TestUnits",*/"indataTEST");

  BOOST_CHECK_EQUAL(mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->isDataExist("indataTEST"),true);
  std::string Value = "non empty";
  mp_EngProject->getCoreRepository().getUnit("TestUnits",1)->getInputData()->getValue("indataTEST",&Value);
  BOOST_CHECK_EQUAL(Value,"");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
