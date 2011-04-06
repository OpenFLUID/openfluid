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
 \file ProjectExplorerAdapterModel_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_ProjectExplorerAdapterModel
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "ProjectExplorerAdapterModel.hpp"
#include "ProjectExplorerColumns.hpp"
#include "EngineProjectFactory.hpp"
#include "tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_AdapterModel
{
    ProjectExplorerAdapterModelSub* mp_AdapterModel;
    ProjectExplorerColumns m_Columns;

    init_AdapterModel()
    {
      BuilderTestHelper::getInstance()->initGtk();
      mp_AdapterModel = new ProjectExplorerAdapterModelSub();
    }

    ~init_AdapterModel()
    {
      delete mp_AdapterModel;
    }
};

BOOST_FIXTURE_TEST_SUITE(ProjectExplorerAdapterModelTest, init_AdapterModel)

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_constructor)
{
  BOOST_CHECK_EQUAL(mp_AdapterModel->getTreeModel()->children().size(),4);
}

BOOST_AUTO_TEST_CASE(test_updateModel)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_AdapterModel->setModelInstance(EngProject->getModelInstance());

  BOOST_CHECK_EQUAL(mp_AdapterModel->getTreeModel()->children().size(),4);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getTreeModel()->children()[0].children().size(),0);

  mp_AdapterModel->updateModel();

  BOOST_CHECK_EQUAL(mp_AdapterModel->getTreeModel()->children().size(),4);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getTreeModel()->children()[0].children().size(),2);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_updateDomain)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_AdapterModel->setSimulationBlob(EngProject->getSimBlob());

  BOOST_CHECK_EQUAL(mp_AdapterModel->getTreeModel()->children().size(),4);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getTreeModel()->children()[1].children().size(),0);

  mp_AdapterModel->updateDomain();

  BOOST_CHECK_EQUAL(mp_AdapterModel->getTreeModel()->children().size(),4);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getTreeModel()->children()[1].children().size(),2);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_updateRunInfo)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_AdapterModel->setSimulationBlob(EngProject->getSimBlob());

  Glib::RefPtr<BuilderTreeStore> BuilderStore =
  Glib::RefPtr<BuilderTreeStore>::cast_static(mp_AdapterModel->getTreeModel());

  Gtk::TreeRow RunInfoRow = BuilderStore->getRowFromRowRef(*mp_AdapterModel->getRunInfoRowRef());

  std::string RunInfoName = RunInfoRow[m_Columns.m_Display];

  BOOST_CHECK_EQUAL(RunInfoName,mp_AdapterModel->generateRunInfoStr("","",1));

  mp_AdapterModel->updateRunInfo();

  std::string RunInfoStr = mp_AdapterModel->generateRunInfoStr(EngProject->getRunDescriptor().getBeginDate().getAsISOString(),
      EngProject->getRunDescriptor().getEndDate().getAsISOString(),
      EngProject->getRunDescriptor().getDeltaT());

  std::string NewRunInfoName = RunInfoRow[m_Columns.m_Display];

  BOOST_CHECK_EQUAL(NewRunInfoName,RunInfoStr);

  delete EngProject;
}

BOOST_AUTO_TEST_CASE(test_updateResults)
{
  std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
  + "/OPENFLUID.IN.Primitives";
  EngineProject* EngProject = EngineProjectFactory::createEngineProject(Path);

  mp_AdapterModel->setSimulationBlob(EngProject->getSimBlob());

  BOOST_CHECK_EQUAL(mp_AdapterModel->getTreeModel()->children().size(),4);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getTreeModel()->children()[3].children().size(),0);

  mp_AdapterModel->updateResults();

  BOOST_CHECK_EQUAL(mp_AdapterModel->getTreeModel()->children().size(),4);
  BOOST_CHECK_EQUAL(mp_AdapterModel->getTreeModel()->children()[3].children().size(),2);

  delete EngProject;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
