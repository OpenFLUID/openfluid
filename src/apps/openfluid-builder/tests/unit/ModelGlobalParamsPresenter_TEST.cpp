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
#define BOOST_TEST_MODULE builder_unittest_ModelGlobalParamsPresenter
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "ModelGlobalParamsComponent.hpp"
#include "ModelGlobalParamsModel.hpp"
#include "ModelGlobalParamsView.hpp"
#include "ModelGlobalParamRow.hpp"
#include "EngineProject.hpp"
#include "tests-config.hpp"
#include <openfluid/machine.hpp>

// =====================================================================
// =====================================================================

struct init_Presenter
{
    ModelGlobalParamsComponent* mp_Component;

    ModelGlobalParamsModel* mp_Model;
    ModelGlobalParamsViewSub* mp_View;

    EngineProject* mp_EngProject;

    init_Presenter()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Component = new ModelGlobalParamsComponent();

      mp_Model = mp_Component->getModel();
      mp_View
          = static_cast<ModelGlobalParamsViewSub*> (mp_Component->getView());

      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.Primitives";
      mp_EngProject = new EngineProject(Path);
    }

    ~init_Presenter()
    {
      delete mp_Component;
      delete mp_EngProject;
    }
};

BOOST_FIXTURE_TEST_SUITE(ModelGlobalParamsModelTest, init_Presenter)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_globalparams_management)
{
  // initial state

  mp_Model->setEngineRequirements(*mp_EngProject->getModelInstance());

  BOOST_CHECK_EQUAL(mp_View->getByParamNameParamRow().size(),0);
  BOOST_CHECK_EQUAL(mp_View->getCombo()->get_model()->children().size(),6);

  // adding a global parameter (from Model)

  mp_EngProject->getModelInstance()->setGlobalParameter("longparam","123");
  mp_Model->update();

  BOOST_CHECK_EQUAL(mp_View->getByParamNameParamRow().size(),1);
  BOOST_CHECK_EQUAL(mp_View->getCombo()->get_model()->children().size(),5);
  BOOST_CHECK_EQUAL(mp_View->getByParamNameParamRow()["longparam"]->getValue(),"123");

  // adding a second global parameter (from View)

  mp_View->getCombo()->set_active_text("strparam");
  mp_View->getAddButton()->clicked();

  BOOST_CHECK_EQUAL(mp_View->getByParamNameParamRow().size(),2);
  BOOST_CHECK_EQUAL(mp_View->getCombo()->get_model()->children().size(),4);
  BOOST_CHECK_EQUAL(mp_View->getByParamNameParamRow()["strparam"]->getValue(),"");
  BOOST_CHECK_EQUAL(mp_EngProject->getModelInstance()->getGlobalParameters().size(),2);
  BOOST_CHECK_EQUAL(mp_EngProject->getModelInstance()->getGlobalParameters()["strparam"].get(),"");

  // changing a global parameter value

  mp_View->getByParamNameParamRow()["strparam"]->setValue("abc");

  BOOST_CHECK_EQUAL(mp_View->getByParamNameParamRow()["strparam"]->getValue(),"abc");
  BOOST_CHECK_EQUAL(mp_EngProject->getModelInstance()->getGlobalParameters()["strparam"].get(),"abc");

  // removing a global parameter (from Model)

  mp_EngProject->getModelInstance()->getGlobalParameters().erase("longparam");
  mp_Model->update();

  BOOST_CHECK_EQUAL(mp_View->getByParamNameParamRow().size(),1);
  BOOST_CHECK_EQUAL(mp_View->getCombo()->get_model()->children().size(),5);

  // removing last global parameter (from View)

  mp_View->getByParamNameParamRow()["strparam"]->signal_removeAsked().emit("strparam");

  BOOST_CHECK_EQUAL(mp_View->getByParamNameParamRow().size(),0);
  BOOST_CHECK_EQUAL(mp_View->getCombo()->get_model()->children().size(),6);
  BOOST_CHECK_EQUAL(mp_EngProject->getModelInstance()->getGlobalParameters().size(),0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
