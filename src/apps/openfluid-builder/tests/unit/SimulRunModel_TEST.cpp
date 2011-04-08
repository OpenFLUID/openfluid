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
 \file SimulRunModel_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_SimulRunModel
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "SimulRunModel.hpp"
#include "EngineProjectFactory.hpp"
#include "tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_Model
{
    SimulRunModelSub* mp_Model;

    init_Model()
    {
      BuilderTestHelper::getInstance()->initGtk();

      openfluid::base::RuntimeEnvironment::getInstance()->setOutputDir(
          CONFIGTESTS_OUTPUT_DATA_DIR + "/SimulRunModel_TEST.OUT");

      mp_Model = new SimulRunModelSub();
    }

    ~init_Model()
    {
      delete mp_Model;
    }
};

BOOST_FIXTURE_TEST_SUITE(ModelStructureModelTest, init_Model)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_setRunDescriptor)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getRunDescriptor());

  BOOST_CHECK_EQUAL(p_EngProject->getRunDescriptor().isUserValuesBufferSize(),false);

  BOOST_CHECK_EQUAL(mp_Model->getDelta(), 3600);
  BOOST_CHECK_EQUAL(mp_Model->isBeginValid(), true);
  BOOST_CHECK_EQUAL(mp_Model->isEndValid(), true);
  BOOST_CHECK_EQUAL(mp_Model->getBegin(), "2000-01-01 00:00:00");
  BOOST_CHECK_EQUAL(mp_Model->getEnd(), "2000-01-01 06:00:00");
  BOOST_CHECK_EQUAL(mp_Model->isValuesBuffSet(), false);
  BOOST_CHECK_EQUAL(mp_Model->getValuesBuff(), 0);
  BOOST_CHECK_EQUAL(mp_Model->getFilesBuff(), 2);

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_setInvalidDateTime)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getRunDescriptor());

  mp_Model->setBegin("99/99/99");

  BOOST_CHECK_EQUAL(mp_Model->isBeginValid(), false);
  BOOST_CHECK_EQUAL(mp_Model->getBegin(), "2000-01-01 00:00:00");

  mp_Model->setEnd("99/99/99");

  BOOST_CHECK_EQUAL(mp_Model->isEndValid(), false);
  BOOST_CHECK_EQUAL(mp_Model->getEnd(), "2000-01-01 06:00:00");

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_setValidDateTime)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getRunDescriptor());

  mp_Model->setBegin("2012-11-10 01:23:45");

  BOOST_CHECK_EQUAL(mp_Model->isBeginValid(), true);
  BOOST_CHECK_EQUAL(mp_Model->getBegin(), "2012-11-10 01:23:45");
  BOOST_CHECK_EQUAL(p_EngProject->getRunDescriptor().getBeginDate().getAsISOString(), "2012-11-10 01:23:45");

  mp_Model->setEnd("2010-11-12 02:34:56");

  BOOST_CHECK_EQUAL(mp_Model->isEndValid(), true);
  BOOST_CHECK_EQUAL(mp_Model->getEnd(), "2010-11-12 02:34:56");
  BOOST_CHECK_EQUAL(p_EngProject->getRunDescriptor().getEndDate().getAsISOString(), "2010-11-12 02:34:56");

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_setValuesBuff)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getRunDescriptor());

  BOOST_CHECK_EQUAL(mp_Model->isValuesBuffSet(), false);
  BOOST_CHECK_EQUAL(p_EngProject->getRunDescriptor().isUserValuesBufferSize(),false);

  mp_Model->setValuesBuff(5);

  BOOST_CHECK_EQUAL(mp_Model->isValuesBuffSet(), true);
  BOOST_CHECK_EQUAL(mp_Model->getValuesBuff(), 5);
  BOOST_CHECK_EQUAL(p_EngProject->getRunDescriptor().isUserValuesBufferSize(),true);
  BOOST_CHECK_EQUAL(p_EngProject->getRunDescriptor().getValuesBufferSize(),5);

  mp_Model->setValuesBuffIsSet(false);

  BOOST_CHECK_EQUAL(mp_Model->isValuesBuffSet(), false);
  BOOST_CHECK_EQUAL(p_EngProject->getRunDescriptor().isUserValuesBufferSize(),false);

  mp_Model->setValuesBuffIsSet(true);

  BOOST_CHECK_EQUAL(mp_Model->isValuesBuffSet(), true);
  BOOST_CHECK_EQUAL(mp_Model->getValuesBuff(), 5);
  BOOST_CHECK_EQUAL(p_EngProject->getRunDescriptor().isUserValuesBufferSize(),true);
  BOOST_CHECK_EQUAL(p_EngProject->getRunDescriptor().getValuesBufferSize(),5);
}

BOOST_AUTO_TEST_CASE(test_setOtherValues)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getRunDescriptor());

  mp_Model->setDelta(360);
  mp_Model->setFilesBuff(4);

  BOOST_CHECK_EQUAL(mp_Model->getDelta(), 360);
  BOOST_CHECK_EQUAL(mp_Model->getFilesBuff(), 4);
  BOOST_CHECK_EQUAL(p_EngProject->getRunDescriptor().getDeltaT(),360);
  BOOST_CHECK_EQUAL(p_EngProject->getRunDescriptor().getFilesBufferSizeInKB(),4);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
