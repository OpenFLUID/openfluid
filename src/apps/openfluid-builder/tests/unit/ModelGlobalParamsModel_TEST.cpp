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
#include "EngineProject.hpp"
#include "tests-config.hpp"
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>

// =====================================================================
// =====================================================================

struct init_Model
{
    ModelGlobalParamsModel* mp_Model;
    EngineProject* mp_EngProject;

    init_Model()
    {
      BuilderTestHelper::getInstance()->initGtk();

      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.Primitives";
      mp_EngProject = new EngineProject(Path);

      mp_Model = new ModelGlobalParamsModelImpl(mp_EngProject->getAdvancedDesc().getModel());

      mp_Model->update();
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

BOOST_AUTO_TEST_CASE(test_globalparams_management)
{
  // initial state

  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),6);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),0);

  // adding a global parameter (from ModelInstance)

  mp_EngProject->getAdvancedDesc().getModel().setGlobalParameter("longparam","123");
  mp_Model->update();

  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),5);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed()["longparam"].second,"123");

  std::set<std::string> Globals = mp_Model->getGloballyNotUsed();

  BOOST_CHECK(Globals.find("longparam") == Globals.end());

  // adding a second global parameter (from User)

  mp_Model->fromUserGloballyUsedSet("strparam");
  mp_Model->update();

  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),4);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed()["longparam"].second,"123");
  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed()["strparam"].second,"");

  Globals = mp_Model->getGloballyNotUsed();

  BOOST_CHECK(Globals.find("longparam") == Globals.end());
  BOOST_CHECK(Globals.find("strparam") == Globals.end());

  // changing a global parameter value

  mp_Model->setGlobalValue("strparam","abc");
  mp_Model->update();

  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed()["strparam"].second,"abc");

  // removing a global parameter (from ModelInstance)

  mp_EngProject->getAdvancedDesc().getModel().eraseGlobalParameter("longparam");
  mp_Model->update();

  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),5);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed()["strparam"].second,"abc");

  Globals = mp_Model->getGloballyNotUsed();

  BOOST_CHECK(Globals.find("longparam") != Globals.end());
  BOOST_CHECK(Globals.find("strparam") == Globals.end());

  // removing last global parameter (from User)

  mp_Model->fromUserGloballyUsedUnset("strparam");
  mp_Model->update();

  BOOST_CHECK_EQUAL(mp_Model->getGloballyNotUsed().size(),6);
  BOOST_CHECK_EQUAL(mp_Model->getGloballyUsed().size(),0);

  Globals = mp_Model->getGloballyNotUsed();

  BOOST_CHECK(Globals.find("longparam") != Globals.end());
  BOOST_CHECK(Globals.find("strparam") != Globals.end());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
