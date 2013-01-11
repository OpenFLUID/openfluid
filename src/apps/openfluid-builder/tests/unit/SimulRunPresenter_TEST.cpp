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
 \file SimulRunPresenter_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_SimulRunPresenter
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "SimulRunComponent.hpp"
#include "SimulRunModel.hpp"
#include "SimulRunView.hpp"
#include "EngineProject.hpp"
#include "tests-config.hpp"

#include <openfluid/fluidx/RunDescriptor.hpp>

// =====================================================================
// =====================================================================

struct init_Presenter
{
    SimulRunComponent* mp_Component;

    SimulRunModelSub* mp_Model;
    SimulRunViewSub* mp_View;

    EngineProject* mp_EngProject;

    init_Presenter()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Component = new SimulRunComponent();
      mp_Model = (SimulRunModelSub*) mp_Component->getModel();
      mp_View = (SimulRunViewSub*) (mp_Component->getView());

      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.Primitives";

      openfluid::base::RuntimeEnvironment::getInstance()->setOutputDir(
          CONFIGTESTS_OUTPUT_DATA_DIR + "/SimulRunPresenter_TEST.OUT");
      mp_EngProject = new EngineProject(Path);

      mp_Model->setEngineRequirements(mp_EngProject->getRunDescriptor());
    }

    ~init_Presenter()
    {
      delete mp_Component;
      delete mp_EngProject;
    }
};

BOOST_FIXTURE_TEST_SUITE(SimulRunPresenterTest, init_Presenter)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_SetRunDescriptor)
{
  BOOST_CHECK_EQUAL(mp_View->getDelta(), 3600);
  BOOST_CHECK_EQUAL(mp_View->getBegin(), "2000-01-01 00:00:00");
  BOOST_CHECK_EQUAL(mp_View->getEnd(), "2000-01-01 06:00:00");
  BOOST_CHECK_EQUAL(mp_View->isValuesBuffSet(), false);
  BOOST_CHECK_EQUAL(mp_View->isValuesBuffSpinSensitive(), false);
  BOOST_CHECK_EQUAL(mp_View->getValuesBuff(), 1);
  BOOST_CHECK_EQUAL(mp_View->getFilesBuff(), 2);

  BOOST_CHECK_EQUAL(mp_Model->isValuesBuffSet(),false);
  BOOST_CHECK_EQUAL(mp_Model->getValuesBuff(),0);
  BOOST_CHECK_EQUAL(mp_EngProject->getRunDescriptor().isUserValuesBufferSize(),false);
  BOOST_CHECK_EQUAL(mp_EngProject->getRunDescriptor().getValuesBufferSize(),0);
}

BOOST_AUTO_TEST_CASE(test_ToggleValuesBuffIsSet)
{
  mp_View->setValuesBuffIsSet(true);

  BOOST_CHECK_EQUAL(mp_View->isValuesBuffSet(), true);
  BOOST_CHECK_EQUAL(mp_View->isValuesBuffSpinSensitive(), true);

  BOOST_CHECK_EQUAL(mp_Model->isValuesBuffSet(),true);
  BOOST_CHECK_EQUAL(mp_EngProject->getRunDescriptor().isUserValuesBufferSize(),true);

  mp_View->setValuesBuffIsSet(false);

  BOOST_CHECK_EQUAL(mp_View->isValuesBuffSet(), false);
  BOOST_CHECK_EQUAL(mp_View->isValuesBuffSpinSensitive(), false);

  BOOST_CHECK_EQUAL(mp_Model->isValuesBuffSet(),false);
  BOOST_CHECK_EQUAL(mp_EngProject->getRunDescriptor().isUserValuesBufferSize(),false);
}

BOOST_AUTO_TEST_CASE(test_changeValuesBuff)
{
  mp_View->setValuesBuffIsSet(true);
  mp_View->setValuesBuff(10);

  BOOST_CHECK_EQUAL(mp_View->isValuesBuffSet(), true);
  BOOST_CHECK_EQUAL(mp_View->isValuesBuffSpinSensitive(), true);
  BOOST_CHECK_EQUAL(mp_View->getValuesBuff(), 10);

  BOOST_CHECK_EQUAL(mp_Model->isValuesBuffSet(),true);
  BOOST_CHECK_EQUAL(mp_Model->getValuesBuff(),10);
  BOOST_CHECK_EQUAL(mp_EngProject->getRunDescriptor().isUserValuesBufferSize(),true);
  BOOST_CHECK_EQUAL(mp_EngProject->getRunDescriptor().getValuesBufferSize(), 10);

  mp_View->setValuesBuffIsSet(false);

  BOOST_CHECK_EQUAL(mp_View->isValuesBuffSet(), false);
  BOOST_CHECK_EQUAL(mp_View->isValuesBuffSpinSensitive(), false);

  BOOST_CHECK_EQUAL(mp_Model->isValuesBuffSet(),false);
  BOOST_CHECK_EQUAL(mp_EngProject->getRunDescriptor().isUserValuesBufferSize(),false);
}

BOOST_AUTO_TEST_CASE(test_setInvalidDateTimeValue)
{
  mp_View->setBegin("99/99/99");

  BOOST_CHECK_EQUAL(mp_View->getBeginBGColor(),Gdk::Color("red").to_string());

  mp_View->setEnd("99/99/99");

  BOOST_CHECK_EQUAL(mp_View->getEndBGColor(),Gdk::Color("red").to_string());
}

BOOST_AUTO_TEST_CASE(test_setUncoherentDateTimeValue)
{
  mp_View->setBegin("99/99/99");
  mp_View->setBegin("2012-11-10 01:23:45");

  BOOST_CHECK_EQUAL(mp_View->getBeginBGColor(),Gdk::Color("orange").to_string());

  mp_View->setEnd("99/99/99");
  mp_View->setEnd("2010-11-12 02:34:56");

  BOOST_CHECK_EQUAL(mp_View->getEndBGColor(),Gdk::Color("orange").to_string());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END()
