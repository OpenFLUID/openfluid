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

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_ResChooserPresenter
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "ResChooserComponent.hpp"
#include "ResChooserModel.hpp"
#include "ResChooserView.hpp"
#include "EngineProjectFactory.hpp"
#include "tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_Presenter
{
    ResChooserComponent* mp_Component;

    ResChooserModelSub* mp_Model;
    ResChooserViewSub* mp_View;
    EngineProject* mp_EngProject;

    init_Presenter()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Component = new ResChooserComponent();
      mp_Model = (ResChooserModelSub*) (mp_Component->getModel());
      mp_View = (ResChooserViewSub*) (mp_Component->getView());

      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.Primitives";
      mp_EngProject = EngineProjectFactory::createEngineProject(Path);

      mp_Model->setEngineRequirements(mp_EngProject->getOutputDescriptor(),
          mp_EngProject->getCoreRepository(),
          *mp_EngProject->getModelInstance());
    }

    ~init_Presenter()
    {
      delete mp_Component;
      delete mp_EngProject;
    }
};

BOOST_FIXTURE_TEST_SUITE(ResChooserPresenterTest, init_Presenter)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_extractSets)
{
  BOOST_CHECK_EQUAL(mp_Model->getBySetNameClassName().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getBySetNameIDs().size(),0);

  mp_Model->extractSets();

  BOOST_CHECK_EQUAL(mp_Model->getBySetNameClassName().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getBySetNameClassName()["full"],"TestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getBySetNameClassName()["partial"],"TestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getBySetNameIDs().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getBySetNameIDs()["full"].size(),12);
  BOOST_CHECK_EQUAL(mp_Model->getBySetNameIDs()["partial"].size(),2);
}

BOOST_AUTO_TEST_CASE(test_initialize)
{
  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetName(),"");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnitInfo().first,"");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnitInfo().second,-1);

  BOOST_CHECK_EQUAL(mp_View->getSetName(),"");
  BOOST_CHECK_EQUAL(mp_View->getID(),-1);
  BOOST_CHECK_EQUAL(mp_View->getClassName(),"");

  mp_Model->initialize();

  BOOST_CHECK_EQUAL(mp_View->getBySetNameClassNames().size(),2);
  BOOST_CHECK_EQUAL(mp_View->getBySetNameIDsListStores().size(),2);
  BOOST_CHECK_EQUAL(mp_View->getBySetNameClassNames()["full"],"TestUnits");
  BOOST_CHECK_EQUAL(mp_View->getBySetNameClassNames()["partial"],"TestUnits");
  BOOST_CHECK_EQUAL(mp_View->getBySetNameIDsListStores()["full"]->children().size(),12);
  BOOST_CHECK_EQUAL(mp_View->getBySetNameIDsListStores()["partial"]->children().size(),2);

  unsigned int IDValue = 0;
  mp_View->getBySetNameIDsListStores()["full"]->children()[0].get_value(0,IDValue);
  BOOST_CHECK_EQUAL(IDValue,1);

  BOOST_CHECK_EQUAL(mp_View->getSetName(),"full");
  BOOST_CHECK_EQUAL(mp_View->getID(),1);
  BOOST_CHECK_EQUAL(mp_View->getClassName(),"TestUnits");

  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetName(),"full");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnitInfo().first,"TestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnitInfo().second,1);
}

BOOST_AUTO_TEST_CASE(test_getters)
{
  BOOST_CHECK(mp_Model->getSelectedSetDescriptor() == 0);
  BOOST_CHECK(mp_Model->getSelectedUnit() == 0);

  mp_Model->initialize();

  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetName(),"full");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnitInfo().first,"TestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnitInfo().second,1);

  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetDescriptor(), &(mp_EngProject->getOutputDescriptor().getFileSets()[0].getSets()[0]));
  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnit(),mp_EngProject->getCoreRepository().getUnit("TestUnits",1));
}

BOOST_AUTO_TEST_CASE(changeSetValue)
{
  mp_Model->initialize();

  mp_View->selectSetName("partial");

  BOOST_CHECK_EQUAL(mp_View->getSetName(),"partial");
  BOOST_CHECK_EQUAL(mp_View->getID(),1);
  BOOST_CHECK_EQUAL(mp_View->getClassName(),"TestUnits");

  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetName(),"partial");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnitInfo().first,"TestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnitInfo().second,1);
}

BOOST_AUTO_TEST_CASE(changeIdValue)
{
  mp_Model->initialize();

  mp_View->selectId(1);

  BOOST_CHECK_EQUAL(mp_View->getSetName(),"full");
  //  BOOST_CHECK_EQUAL(mp_View->getID(),2);
  BOOST_CHECK_EQUAL(mp_View->getClassName(),"TestUnits");

  BOOST_CHECK_EQUAL(mp_Model->getSelectedSetName(),"full");
  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnitInfo().first,"TestUnits");
  //  BOOST_CHECK_EQUAL(mp_Model->getSelectedUnitInfo().second,2);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END()

