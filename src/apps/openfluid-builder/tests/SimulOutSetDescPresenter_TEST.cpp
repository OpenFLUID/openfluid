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
#define BOOST_TEST_MODULE builder_unittest_SimulOutSetDescPresenter
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "SimulOutSetDescComponent.hpp"
#include "SimulOutSetDescModel.hpp"
#include "SimulOutSetDescView.hpp"
#include "EngineProjectFactory.hpp"
#include "SimulOutFilesModel.hpp"
#include "SimulOutSetsModel.hpp"
#include "tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_Presenter
{
    SimulOutSetDescComponent* mp_Component;

    SimulOutSetDescModelSub* mp_Model;
    SimulOutSetDescViewSub* mp_View;
    EngineProject* mp_EngProject;

    init_Presenter()
    {
      BuilderTestHelper::getInstance()->initGtk();

      openfluid::base::RuntimeEnvironment::getInstance()->addExtraPluginsPaths(CONFIGTESTS_OUTPUT_BINARY_DIR);

      mp_Component = new SimulOutSetDescComponent();
      mp_Model = (SimulOutSetDescModelSub*) (mp_Component->getModel());
      mp_View = (SimulOutSetDescViewSub*) (mp_Component->getView());

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

BOOST_FIXTURE_TEST_SUITE(SimulOutSetDescPresenterTest, init_Presenter)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_extractClassesAndIDs)
{
  mp_Model->extractClassesAndIDs();

  BOOST_CHECK_EQUAL(mp_Model->getClasses().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getByClassIDs().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getByClassIDs()["ParentTestUnits"].size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getByClassIDs()["TestUnits"].size(),12);
}

BOOST_AUTO_TEST_CASE(test_extractVars)
{
  mp_Model->extractVars();

  BOOST_CHECK_EQUAL(mp_Model->getByClassVars().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getByClassVars()["TestUnits"].size(),2);

  BOOST_CHECK(std::find(mp_Model->getByClassVars()["TestUnits"].begin(),mp_Model->getByClassVars()["TestUnits"].end(),
          "tests.vector[]") != mp_Model->getByClassVars()["TestUnits"].end());
  BOOST_CHECK(std::find(mp_Model->getByClassVars()["TestUnits"].begin(),mp_Model->getByClassVars()["TestUnits"].end(),
          "tests.scalar") != mp_Model->getByClassVars()["TestUnits"].end());

  BOOST_CHECK_EQUAL(mp_Model->getByClassScalarVars().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getByClassVectorVars().size(),1);
}

BOOST_AUTO_TEST_CASE(test_extractFormatsNames)
{
  SimulOutFilesModelImpl OutFilesModel;
  OutFilesModel.setEngineRequirements(mp_EngProject->getOutputDescriptor());

  BOOST_CHECK_EQUAL(OutFilesModel.getFilesFormatsByNameVect().size(),1);

  mp_Model->setFilesFormatsByNameVect(OutFilesModel.getFilesFormatsByNameVect());
  mp_Model->extractFormatNames();

  BOOST_CHECK_EQUAL(mp_Model->getFormatNames().size(),1);
}

BOOST_AUTO_TEST_CASE(test_initializeForCreation)
{
  SimulOutFilesModelImpl OutFilesModel;
  OutFilesModel.setEngineRequirements(mp_EngProject->getOutputDescriptor());
  SimulOutSetsModelImpl OutSetsModel;
  OutSetsModel.setEngineRequirements(mp_EngProject->getOutputDescriptor());

  mp_Model->initialize(0,"",OutFilesModel.getFilesFormatsByNameVect(),OutSetsModel.getSetsByName());

  BOOST_CHECK_EQUAL(mp_Model->getName(),"");
  BOOST_CHECK_EQUAL(mp_Model->getClass(),"ParentTestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getPrecision(),5);
  BOOST_CHECK_EQUAL(mp_Model->getFormatName(),"Format #1");
  BOOST_CHECK_EQUAL(mp_Model->getIDs().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getVars().size(),0);

  BOOST_CHECK_EQUAL(mp_View->getName(),"");
  BOOST_CHECK_EQUAL(mp_View->getClass(),"ParentTestUnits");
  BOOST_CHECK_EQUAL(mp_View->getPrecision(),5);
  BOOST_CHECK_EQUAL(mp_View->getFormatName(),"Format #1");
  BOOST_CHECK_EQUAL(mp_View->getIDs().size(),0);
  BOOST_CHECK_EQUAL(mp_View->getVars().size(),0);

  BOOST_CHECK_EQUAL(mp_View->getIDsTreeView()->get_selection()->count_selected_rows(),0);
  BOOST_CHECK_EQUAL(mp_View->getVarsTreeView()->get_selection()->count_selected_rows(),0);
}

BOOST_AUTO_TEST_CASE(test_initializeForEdition)
{
  SimulOutFilesModelImpl OutFilesModel;
  OutFilesModel.setEngineRequirements(mp_EngProject->getOutputDescriptor());
  SimulOutSetsModelImpl OutSetsModel;
  OutSetsModel.setEngineRequirements(mp_EngProject->getOutputDescriptor());

  openfluid::base::OutputSetDescriptor* SetDesc = &mp_EngProject->getOutputDescriptor().getFileSets().begin()->getSets()[0];
  mp_Model->initialize(SetDesc,"Format #1",OutFilesModel.getFilesFormatsByNameVect(),OutSetsModel.getSetsByName());

  BOOST_CHECK_EQUAL(mp_Model->getName(),"full");
  BOOST_CHECK_EQUAL(mp_Model->getClass(),"TestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getPrecision(),5);
  BOOST_CHECK_EQUAL(mp_Model->getFormatName(),"Format #1");
  BOOST_CHECK_EQUAL(mp_Model->getIDs().size(),12);
  BOOST_CHECK_EQUAL(mp_Model->getVars().size(),2);

  BOOST_CHECK_EQUAL(mp_View->getName(),"full");
  BOOST_CHECK_EQUAL(mp_View->getClass(),"TestUnits");
  BOOST_CHECK_EQUAL(mp_View->getPrecision(),5);
  BOOST_CHECK_EQUAL(mp_View->getFormatName(),"Format #1");
  BOOST_CHECK_EQUAL(mp_View->getIDs().size(),12);
  BOOST_CHECK_EQUAL(mp_View->getVars().size(),2);

  BOOST_CHECK_EQUAL(mp_View->getIDsTreeView()->get_selection()->count_selected_rows(),12);
  BOOST_CHECK_EQUAL(mp_View->getVarsTreeView()->get_selection()->count_selected_rows(),2);

  SetDesc = &mp_EngProject->getOutputDescriptor().getFileSets().begin()->getSets()[1];
  mp_Model->initialize(SetDesc,"Format #1",OutFilesModel.getFilesFormatsByNameVect(),OutSetsModel.getSetsByName());

  BOOST_CHECK_EQUAL(mp_Model->getName(),"partial");
  BOOST_CHECK_EQUAL(mp_Model->getClass(),"TestUnits");
  BOOST_CHECK_EQUAL(mp_Model->getPrecision(),5);
  BOOST_CHECK_EQUAL(mp_Model->getFormatName(),"Format #1");
  BOOST_CHECK_EQUAL(mp_Model->getIDs().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getVars().size(),2);

  BOOST_CHECK_EQUAL(mp_View->getName(),"partial");
  BOOST_CHECK_EQUAL(mp_View->getClass(),"TestUnits");
  BOOST_CHECK_EQUAL(mp_View->getPrecision(),5);
  BOOST_CHECK_EQUAL(mp_View->getFormatName(),"Format #1");
  BOOST_CHECK_EQUAL(mp_View->getIDs().size(),2);
  BOOST_CHECK_EQUAL(mp_View->getVars().size(),2);
}

BOOST_AUTO_TEST_CASE(test_changePrecisionValue)
{
  SimulOutFilesModelImpl OutFilesModel;
  OutFilesModel.setEngineRequirements(mp_EngProject->getOutputDescriptor());
  SimulOutSetsModelImpl OutSetsModel;
  OutSetsModel.setEngineRequirements(mp_EngProject->getOutputDescriptor());

  openfluid::base::OutputSetDescriptor* SetDesc = &mp_EngProject->getOutputDescriptor().getFileSets().begin()->getSets()[0];
  mp_Model->initialize(SetDesc,"Format #1",OutFilesModel.getFilesFormatsByNameVect(),OutSetsModel.getSetsByName());

  mp_View->setPrecision(7);

  BOOST_CHECK_EQUAL(SetDesc->getPrecision(),7);

  BOOST_CHECK_EQUAL(mp_Model->getPrecision(),7);
}

BOOST_AUTO_TEST_CASE(test_changeFormatName)
{
  SimulOutFilesModelImpl OutFilesModel;
  OutFilesModel.setEngineRequirements(mp_EngProject->getOutputDescriptor());
  SimulOutSetsModelImpl OutSetsModel;
  OutSetsModel.setEngineRequirements(mp_EngProject->getOutputDescriptor());

  OutFilesModel.addFileFormat(new openfluid::base::OutputFilesDescriptor,"New Format");

  openfluid::base::OutputSetDescriptor* SetDesc = &mp_EngProject->getOutputDescriptor().getFileSets().begin()->getSets()[0];
  mp_Model->initialize(SetDesc,"Format #1",OutFilesModel.getFilesFormatsByNameVect(),OutSetsModel.getSetsByName());

  mp_View->setFormatName("New Format");

  BOOST_CHECK_EQUAL(mp_Model->getFormatName(),"New Format");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END()
