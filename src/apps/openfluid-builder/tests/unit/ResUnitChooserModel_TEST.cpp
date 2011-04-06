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
 \file ResUnitChooserModel_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_ResUnitChooserModel
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "ResUnitChooserModel.hpp"
#include "EngineProjectFactory.hpp"
#include "tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_Model
{
    ResUnitChooserModelSub* mp_Model;
    EngineProject* mp_EngProject;

    init_Model()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Model = new ResUnitChooserModelSub();

      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.Primitives";
      mp_EngProject = EngineProjectFactory::createEngineProject(Path);

      mp_Model->setEngineRequirements(mp_EngProject->getOutputDescriptor(),
          mp_EngProject->getCoreRepository(),
          *mp_EngProject->getModelInstance());
    }

    ~init_Model()
    {
      delete mp_Model;
      delete mp_EngProject;
    }
};

BOOST_FIXTURE_TEST_SUITE(ResUnitChooserModelTest, init_Model)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_setEngineRequirements)
{
  BOOST_CHECK(mp_Model->getSelectedSetDescriptor() == 0);
  BOOST_CHECK_EQUAL(mp_Model->getClassName(),"");
  BOOST_CHECK_EQUAL(mp_Model->getIDs().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getScalars().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getVectors().size(),0);
  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);
}

BOOST_AUTO_TEST_CASE(test_setExtractSet)
{
  mp_Model->setSetAndClassName("full","TestUnits");
  mp_Model->extractSet();

  BOOST_CHECK(mp_Model->getSelectedSetDescriptor() == &(mp_EngProject->getOutputDescriptor().getFileSets()[0].getSets())[0]);
}

BOOST_AUTO_TEST_CASE(test_setExtractIdInfo)
{
  mp_Model->setSetAndClassName("full","TestUnits");
  mp_Model->extractSet();
  mp_Model->extractIdsInfo();

  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetDescriptor()->getName(),"full");
  BOOST_CHECK_EQUAL(mp_Model->getClassName(),"TestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetDescriptor()->isAllUnits(),true);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetDescriptor()->getUnitsIDs().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getIDs().size(),12);

  mp_Model->setSetAndClassName("partial","TestUnits");
  mp_Model->extractSet();
  mp_Model->extractIdsInfo();

  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetDescriptor()->getName(),"partial");
  BOOST_CHECK_EQUAL(mp_Model->getClassName(),"TestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetDescriptor()->isAllUnits(),false);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetDescriptor()->getUnitsIDs().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getIDs().size(),2);
}

BOOST_AUTO_TEST_CASE(test_setExtractVarsInfo)
{
  mp_Model->setSetAndClassName("full","TestUnits");
  mp_Model->extractSet();
  mp_Model->extractVarsInfo();

  BOOST_CHECK_EQUAL(mp_Model->getScalars().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getVectors().size(),1);

  mp_Model->setSetAndClassName("partial","TestUnits");
  mp_Model->extractSet();
  mp_Model->extractVarsInfo();

  BOOST_CHECK_EQUAL(mp_Model->getScalars().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getVectors().size(),1);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
