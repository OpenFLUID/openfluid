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
 \file SimulOutFileDescPresenter_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_SimulOutFileDescPresenter
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "SimulOutFileDescComponent.hpp"
#include "SimulOutFileDescModel.hpp"
#include "SimulOutFileDescView.hpp"
#include "EngineProjectFactory.hpp"
#include "SimulOutFilesModel.hpp"
#include "tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_Presenter
{
    SimulOutFileDescComponent* mp_Component;

    SimulOutFileDescModelSub* mp_Model;
    SimulOutFileDescView* mp_View;
    EngineProject* mp_EngProject;

    init_Presenter()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Component = new SimulOutFileDescComponent();
      mp_Model = (SimulOutFileDescModelSub*) (mp_Component->getModel());
      mp_View = (SimulOutFileDescView*) (mp_Component->getView());

      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.Primitives";
      mp_EngProject = EngineProjectFactory::createEngineProject(Path);

      mp_Model->setEngineRequirements(mp_EngProject->getOutputDescriptor());
    }

    ~init_Presenter()
    {
      delete mp_Component;
      delete mp_EngProject;
    }
};

BOOST_FIXTURE_TEST_SUITE(SimulOutFileDescPresenterTest, init_Presenter)

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_initializeForCreation)
{
  SimulOutFilesModelImpl OutFilesModel;
  OutFilesModel.setEngineRequirements(mp_EngProject->getOutputDescriptor());

  mp_Model->initialize(0,"A Name",OutFilesModel.getFilesFormatsByNameVect());

  BOOST_CHECK_EQUAL(mp_Model->getChars().size(),7);
  BOOST_CHECK_EQUAL(mp_Model->getDateFormats().size(),3);

  openfluid::base::OutputFilesDescriptor FileDesc;
  BOOST_CHECK_EQUAL(mp_Model->getColSeparator(),FileDesc.getColSeparator());
  BOOST_CHECK_EQUAL(mp_Model->getDateFormat(),FileDesc.getDateFormat());
  BOOST_CHECK_EQUAL(mp_Model->getCommentChar(),FileDesc.getCommentChar());
  BOOST_CHECK_EQUAL(mp_Model->getFormatName(),"A Name");

  BOOST_CHECK_EQUAL(mp_View->getColSep(),FileDesc.getColSeparator());
  BOOST_CHECK_EQUAL(mp_View->getDateFormat(),FileDesc.getDateFormat());
  BOOST_CHECK_EQUAL(mp_View->getCommentChar(),FileDesc.getCommentChar());
  BOOST_CHECK_EQUAL(mp_View->getFormatName(),"A Name");
}

BOOST_AUTO_TEST_CASE(test_initializeForEdition)
{
  SimulOutFilesModelImpl OutFilesModel;
  OutFilesModel.setEngineRequirements(mp_EngProject->getOutputDescriptor());

  openfluid::base::OutputFilesDescriptor* FileDesc = &(*mp_EngProject->getOutputDescriptor().getFileSets().begin());

  mp_Model->initialize(FileDesc,"A Name",OutFilesModel.getFilesFormatsByNameVect());

  BOOST_CHECK_EQUAL(mp_Model->getColSeparator(),FileDesc->getColSeparator());
  BOOST_CHECK_EQUAL(mp_Model->getDateFormat(),FileDesc->getDateFormat());
  BOOST_CHECK_EQUAL(mp_Model->getCommentChar(),FileDesc->getCommentChar());
  BOOST_CHECK_EQUAL(mp_Model->getFormatName(),"A Name");
}

BOOST_AUTO_TEST_CASE(test_changeValues)
{
  SimulOutFilesModelImpl OutFilesModel;
  OutFilesModel.setEngineRequirements(mp_EngProject->getOutputDescriptor());

  openfluid::base::OutputFilesDescriptor* FileDesc = &(*mp_EngProject->getOutputDescriptor().getFileSets().begin());

  mp_Model->initialize(FileDesc,"A Name",OutFilesModel.getFilesFormatsByNameVect());

  mp_View->setColSep("---");
  mp_View->setDateFormat("abcd");
  mp_View->setCommentChar("***");
  mp_View->setFormatName("New Name");

  BOOST_CHECK_EQUAL(FileDesc->getColSeparator(),"---");
  BOOST_CHECK_EQUAL(FileDesc->getDateFormat(),"abcd");
  BOOST_CHECK_EQUAL(FileDesc->getCommentChar(),"***");

  BOOST_CHECK_EQUAL(mp_Model->getColSeparator(),"---");
  BOOST_CHECK_EQUAL(mp_Model->getDateFormat(),"abcd");
  BOOST_CHECK_EQUAL(mp_Model->getCommentChar(),"***");
  BOOST_CHECK_EQUAL(mp_Model->getFormatName(),"New Name");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END()
