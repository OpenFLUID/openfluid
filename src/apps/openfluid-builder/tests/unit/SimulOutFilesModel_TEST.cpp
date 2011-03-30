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
 \file SimulOutFilesModel_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_SimulOutFilesModel
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "SimulOutFilesModel.hpp"
#include "EngineProjectFactory.hpp"
#include "tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_Model
{
    SimulOutFilesModelSub* mp_Model;

    init_Model()
    {
      BuilderTestHelper::getInstance()->initGtk();

      openfluid::base::RuntimeEnvironment::getInstance()->addExtraPluginsPaths(CONFIGTESTS_OUTPUT_BINARY_DIR);

      mp_Model = new SimulOutFilesModelSub();
    }

    ~init_Model()
    {
      delete mp_Model;
    }
};

BOOST_FIXTURE_TEST_SUITE(SimulOutFilesModelTest, init_Model)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_setEngineRequirements)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());

  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].first,"Format #1");
}

BOOST_AUTO_TEST_CASE(test_getSelectedFileFormat)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());

  mp_Model->setSelectedFileFormatIndex(0);

  openfluid::base::OutputFilesDescriptor* EngineFileDesc = &(*p_EngProject->getOutputDescriptor().getFileSets().begin());

  BOOST_CHECK_EQUAL(mp_Model->getSelectedFileFormat(),EngineFileDesc);
  BOOST_CHECK_EQUAL(mp_Model->getSelectedFileFormatName(),"Format #1");

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_deleteSelectedFileFormat)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect().size(),1);

  mp_Model->setSelectedFileFormatIndex(0);
  mp_Model->deleteSelectedFileFormatConfirmed();

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect().size(),0);

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_deleteSelectedFileFormat2Formats)
{
//  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
//  + "/OPENFLUID.IN.Primitives";
//  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);
//
//  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());
//
//  openfluid::base::OutputFilesDescriptor FileDesc;
//  mp_Model->addFileFormat(&FileDesc,"New Format");
//
//
//
//  openfluid::base::OutputSetDescriptor SetDesc1;
//  openfluid::base::OutputSetDescriptor SetDesc2;
//  openfluid::base::OutputSetDescriptor SetDesc3;
//  FileDesc.getSets().push_back(SetDesc1);
//  FileDesc.getSets().push_back(SetDesc2);
//  FileDesc.getSets().push_back(SetDesc3);
//
//
//
//  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().size(),2);
//  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect().size(),2);
//  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].first,"Format #1");
//  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].second.getSets().size(),2);
//  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[1].first,"New Format");
//  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[1].second.getSets().size(),3);
//
//  mp_Model->setSelectedFileFormatIndex(0);
//  mp_Model->deleteSelectedFileFormatConfirmed();
//
//  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().size(),1);
//  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect().size(),1);
//  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].first,"New Format");
//  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].second.getSets().size(),3);
//
//  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_updateFileFormats)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());

  openfluid::base::OutputFilesDescriptor FileDesc;
  openfluid::base::OutputSetDescriptor SetDesc1;
  openfluid::base::OutputSetDescriptor SetDesc2;
  openfluid::base::OutputSetDescriptor SetDesc3;
  FileDesc.getSets().push_back(SetDesc1);
  FileDesc.getSets().push_back(SetDesc2);
  FileDesc.getSets().push_back(SetDesc3);

  mp_Model->addFileFormat(&FileDesc,"New Format");

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].first,"Format #1");
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].second.getSets().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[1].first,"New Format");
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[1].second.getSets().size(),3);

  mp_Model->setSelectedFileFormatIndex(0);
  mp_Model->deleteSelectedFileFormatConfirmed();

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].first,"New Format");
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].second.getSets().size(),3);

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_addFileFormatToEmptyFilesSet)
{
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject();
  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().size(),0);
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect().size(),0);

  openfluid::base::OutputFilesDescriptor FileDesc;
  FileDesc.setColSeparator("---");
  FileDesc.setDateFormat("abcd");
  FileDesc.setCommentChar("***");

  mp_Model->addFileFormat(&FileDesc,"A Name");

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect().size(),1);

  openfluid::base::OutputFilesDescriptor EngineFileDesc = *p_EngProject->getOutputDescriptor().getFileSets().begin();

  BOOST_CHECK_EQUAL(EngineFileDesc.getColSeparator(),"---");
  BOOST_CHECK_EQUAL(EngineFileDesc.getDateFormat(),"abcd");
  BOOST_CHECK_EQUAL(EngineFileDesc.getCommentChar(),"***");
  BOOST_CHECK_EQUAL(EngineFileDesc.getSets().size(),0);

  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].second.getColSeparator(),"---");
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].second.getDateFormat(),"abcd");
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].second.getCommentChar(),"***");
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].first,"A Name");

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_addFileFormatToNotEmptyFilesSet)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect().size(),1);

  openfluid::base::OutputFilesDescriptor FileDesc;
  FileDesc.setColSeparator("---");
  FileDesc.setDateFormat("abcd");
  FileDesc.setCommentChar("***");

  mp_Model->addFileFormat(&FileDesc,"A Name");

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().size(),2);
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect().size(),2);

  std::vector<openfluid::base::OutputFilesDescriptor>::iterator it = p_EngProject->getOutputDescriptor().getFileSets().begin();
  it ++;

  BOOST_CHECK_EQUAL(it->getColSeparator(),"---");
  BOOST_CHECK_EQUAL(it->getDateFormat(),"abcd");
  BOOST_CHECK_EQUAL(it->getCommentChar(),"***");
  BOOST_CHECK_EQUAL(it->getSets().size(),0);

  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[1].second.getColSeparator(),"---");
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[1].second.getDateFormat(),"abcd");
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[1].second.getCommentChar(),"***");
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[1].first,"A Name");

  delete p_EngProject;
}

BOOST_AUTO_TEST_CASE(test_updateSelectedFileFormat)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* p_EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_Model->setEngineRequirements(p_EngProject->getOutputDescriptor());

  openfluid::base::OutputFilesDescriptor* EngineFileDesc = &(*p_EngProject->getOutputDescriptor().getFileSets().begin());

  EngineFileDesc->setColSeparator("---");
  EngineFileDesc->setDateFormat("abcd");
  EngineFileDesc->setCommentChar("***");

  mp_Model->setSelectedFileFormatIndex(0);
  mp_Model->updateSelectedFileFormat("New Name");

  BOOST_CHECK_EQUAL(p_EngProject->getOutputDescriptor().getFileSets().size(),1);
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect().size(),1);

  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].second.getColSeparator(),"---");
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].second.getDateFormat(),"abcd");
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].second.getCommentChar(),"***");
  BOOST_CHECK_EQUAL(mp_Model->getFilesFormatsByNameVect()[0].first,"New Name");

  delete p_EngProject;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
