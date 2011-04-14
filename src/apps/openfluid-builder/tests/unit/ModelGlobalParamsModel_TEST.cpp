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
 \file ModelGlobalParamsModel_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_ModelGlobalParamsModel
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "ModelGlobalParamsModel.hpp"
#include "EngineProjectFactory.hpp"
#include "tests-config.hpp"
#include <openfluid/machine.hpp>

// =====================================================================
// =====================================================================

struct init_Model
{
    ModelGlobalParamsModelSub* mp_Model;
    EngineProject* mp_EngProject;

    init_Model()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Model = new ModelGlobalParamsModelSub();
      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.Primitives";
      mp_EngProject = EngineProjectFactory::createEngineProject(Path);
    }

    ~init_Model()
    {
      delete mp_Model;
      delete mp_EngProject;
    }
};

BOOST_FIXTURE_TEST_SUITE(ModelGlobalParamsModelTest, init_Model)

// =====================================================================
// =====================================================================
BOOST_AUTO_TEST_CASE(test_dispatchParamWithNoExistingUsedParam)
{
  std::vector<std::string> IncomingParams;
  IncomingParams.push_back("A");
  IncomingParams.push_back("B");
  IncomingParams.push_back("C");

  mp_Model->clearGloballyNotUsed();
  mp_Model->clearTempNewGloballyUsed();
  mp_Model->clearGloballyNoMoreUsed();
  for(unsigned int i=0; i<IncomingParams.size(); i++)
  {
    mp_Model->dispatchParam(IncomingParams[i]);
  }

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),3);
  BOOST_CHECK_EQUAL(mp_Model->getTempNewGloballyUsed().size(),0);
}

BOOST_AUTO_TEST_CASE(test_dispatchParamWithExistingUsedParams)
{
  std::set<std::string> NotUsed;
  NotUsed.insert("A");
  NotUsed.insert("C");

  std::map<std::string,std::string> Used;
  Used["B"] = "valB";
  Used["E"] = "valE";

  mp_Model->setGloballyNotUsed(NotUsed);
  mp_Model->setGloballyUsed(Used);

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getTempNewGloballyUsed().size(),0);

  std::vector<std::string> IncomingParams;
  IncomingParams.push_back("C");
  IncomingParams.push_back("B");
  IncomingParams.push_back("D");

  mp_Model->clearGloballyNotUsed();
  mp_Model->clearTempNewGloballyUsed();
  mp_Model->clearGloballyNoMoreUsed();
  for(unsigned int i=0; i<IncomingParams.size(); i++)
  {
    mp_Model->dispatchParam(IncomingParams[i]);
  }

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getTempNewGloballyUsed().size(),1);
}

BOOST_AUTO_TEST_CASE(test_simulUpdateWithNoExistingUsedParam)
{
  std::vector<std::string> IncomingParams;
  IncomingParams.push_back("A");
  IncomingParams.push_back("B");
  IncomingParams.push_back("C");

  mp_Model->clearGloballyNotUsed();
  mp_Model->clearTempNewGloballyUsed();
  mp_Model->clearGloballyNoMoreUsed();
  for(unsigned int i=0; i<IncomingParams.size(); i++)
  {
    mp_Model->dispatchParam(IncomingParams[i]);
  }
  mp_Model->afterDispatch();

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),3);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNoMoreUsed().size(),0);
}

BOOST_AUTO_TEST_CASE(test_simulUpdateWithExistingUsedParams)
{
  std::set<std::string> NotUsed;
  NotUsed.insert("A");
  NotUsed.insert("C");

  std::map<std::string,std::string> Used;
  Used["B"] = "valB";
  Used["E"] = "valE";

  mp_Model->setGloballyNotUsed(NotUsed);
  mp_Model->setGloballyUsed(Used);

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNoMoreUsed().size(),0);

  std::vector<std::string> IncomingParams;
  IncomingParams.push_back("C");
  IncomingParams.push_back("B");
  IncomingParams.push_back("D");

  mp_Model->clearGloballyNotUsed();
  mp_Model->clearTempNewGloballyUsed();
  mp_Model->clearGloballyNoMoreUsed();
  for(unsigned int i=0; i<IncomingParams.size(); i++)
  {
    mp_Model->dispatchParam(IncomingParams[i]);
  }
  mp_Model->afterDispatch();

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().begin()->first,"B");
  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().begin()->second,"valB");

  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),2);
  std::set<std::string>::iterator it = mp_Model->getGloballyNotUsed().begin();
  BOOST_CHECK_EQUAL(*it,"C");
  it++;
  BOOST_CHECK_EQUAL(*it,"D");

  BOOST_CHECK_EQUAL(mp_Model->getGloballyNoMoreUsed().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNoMoreUsed()[0],"E");
}

BOOST_AUTO_TEST_CASE(test_setEngineRequirement)
{
  mp_Model->setEngineRequirements(*mp_EngProject->getModelInstance());

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),6);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNoMoreUsed().size(),0);
}

BOOST_AUTO_TEST_CASE(test_updateWithNoExistingUsedParam)
{
  mp_Model->setEngineRequirements(*mp_EngProject->getModelInstance());

  mp_EngProject->getModelInstance()->deleteItem(1);
  mp_Model->update();

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNoMoreUsed().size(),0);

  mp_EngProject->getModelInstance()->appendItem(
      openfluid::machine::PluginManager::getInstance()->getPlugin("tests.primitives.use"));
  mp_Model->update();

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),6);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNoMoreUsed().size(),0);
}

BOOST_AUTO_TEST_CASE(test_fromUserGloballyUsedSet)
{
  mp_Model->setEngineRequirements(*mp_EngProject->getModelInstance());

  std::string ParamUnit = mp_Model->fromUserGloballyUsedSet("longparam");

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),5);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNoMoreUsed().size(),0);
  BOOST_CHECK_EQUAL(ParamUnit,"-");

  ParamUnit = mp_Model->fromUserGloballyUsedSet("doublearrayparam");

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),4);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNoMoreUsed().size(),0);
  BOOST_CHECK_EQUAL(ParamUnit,"-");
}

BOOST_AUTO_TEST_CASE(test_fromUserGloballyUsedUnset)
{
  mp_Model->setEngineRequirements(*mp_EngProject->getModelInstance());

  mp_Model->fromUserGloballyUsedSet("longparam");
  mp_Model->fromUserGloballyUsedSet("doublearrayparam");
  mp_Model->fromUserGloballyUsedSet("strparam");

  mp_Model->fromUserGloballyUsedUnset("doublearrayparam");

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),4);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNoMoreUsed().size(),0);

  mp_Model->fromUserGloballyUsedUnset("longparam");

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),5);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNoMoreUsed().size(),0);

  mp_Model->fromUserGloballyUsedUnset("strparam");

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),6);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyNoMoreUsed().size(),0);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
