/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file RunContextManager_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_prjman


#include <ctime>
#include <chrono>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->isProjectOpen(), false);
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectPath(), "");
  BOOST_REQUIRE(!openfluid::base::RunContextManager::instance()->getInputDir().empty());
  BOOST_REQUIRE(!openfluid::base::RunContextManager::instance()->getOutputDir().empty());

  BOOST_REQUIRE(!openfluid::base::RunContextManager::instance()->isClearOutputDir());
  BOOST_REQUIRE(!openfluid::base::RunContextManager::instance()->isValuesBufferUserSize());

  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getWaresMaxNumThreads(),
                      openfluid::base::RunContextManager::instance()->getIdealThreadCount());
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->extraProperties().size(),0);
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getWaresEnvironment().size(),4);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->isProjectOpen(), false);

  openfluid::base::RunContextManager::instance()->setInputDir("/foo/bar");

  openfluid::base::RunContextManager::instance()->setOutputDir("/bar/baz");

  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getInputDir(),"/foo/bar");
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getOutputDir(),"/bar/baz");
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->isProjectOpen(),false);
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectPath(), "");

  openfluid::base::RunContextManager::instance()->setClearOutputDir(true);
  openfluid::base::RunContextManager::instance()->setValuesBufferUserSize(15);
  BOOST_REQUIRE(openfluid::base::RunContextManager::instance()->isClearOutputDir());
  BOOST_REQUIRE(openfluid::base::RunContextManager::instance()->isValuesBufferUserSize());
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getValuesBufferUserSize(),15);

  openfluid::base::RunContextManager::instance()->setClearOutputDir(false);
  openfluid::base::RunContextManager::instance()->unsetValuesBufferUserSize();
  BOOST_REQUIRE(!openfluid::base::RunContextManager::instance()->isClearOutputDir());
  BOOST_REQUIRE(!openfluid::base::RunContextManager::instance()->isValuesBufferUserSize());
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getValuesBufferUserSize(),0);

  openfluid::base::RunContextManager::instance()->closeProject();
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->isProjectOpen(),false);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_project_operations)
{
  std::string Prj1Dir = CONFIGTESTS_OUTPUT_DATA_DIR+"/prjmanager/prj1";

  if (openfluid::tools::Filesystem::isDirectory(Prj1Dir))
  {
    openfluid::tools::Filesystem::removeDirectory(Prj1Dir);
  }


  BOOST_CHECK_EQUAL(openfluid::base::RunContextManager::isProject(Prj1Dir),false);

  openfluid::base::RunContextManager::instance()->createProject(Prj1Dir,"Test project",
                                                      "This is a test project","John Doe", false);

  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::isProject(Prj1Dir),true);
  BOOST_REQUIRE(openfluid::base::RunContextManager::instance()->isProjectOpen());
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectPath(), Prj1Dir);
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getInputDir(),Prj1Dir+"/IN");
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getOutputDir(),Prj1Dir+"/OUT");
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->isProjectIncrementalOutputDir(),false);
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectName(),"Test project");
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectDescription(),"This is a test project");
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectAuthors(),"John Doe");
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectCreationDate(),
                      openfluid::base::RunContextManager::instance()->getProjectLastModDate());

  openfluid::base::RunContextManager::instance()->setProjectIncrementalOutputDir(true);
  openfluid::base::RunContextManager::instance()->setProjectName("Modified Test project");
  openfluid::base::RunContextManager::instance()->setProjectDescription("This is a modified test project");
  openfluid::base::RunContextManager::instance()->setProjectAuthors("John Doe, Tom Morello");

  openfluid::tools::millisleep(1000);

  openfluid::base::RunContextManager::instance()->saveProject();
  openfluid::base::RunContextManager::instance()->closeProject();

  BOOST_CHECK_EQUAL(openfluid::base::RunContextManager::isProject(Prj1Dir),true);
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->isProjectOpen(), false);
  BOOST_REQUIRE(openfluid::base::RunContextManager::instance()->getProjectPath().empty());

  openfluid::base::RunContextManager::instance()->openProject(Prj1Dir);

  std::chrono::system_clock::time_point TimePoint = std::chrono::system_clock::now();
  std::time_t Time = std::chrono::system_clock::to_time_t(TimePoint);

  char NowChar[16];
  std::strftime(NowChar, sizeof(NowChar),"%Y%m%d-%H%M%S", std::localtime(&Time));
  std::string Now(NowChar);

  openfluid::base::RunContextManager::instance()->updateProjectOutputDir();
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->isProjectOpen(), true);
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectPath(), Prj1Dir);
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getInputDir(),Prj1Dir+"/IN");

  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getOutputDir(),Prj1Dir+"/OUT_"+Now);
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->isProjectIncrementalOutputDir(),true);
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectName(), "Modified Test project");
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectDescription(),
                      "This is a modified test project");
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectAuthors(),"John Doe, Tom Morello");
  BOOST_CHECK_NE(openfluid::base::RunContextManager::instance()->getProjectCreationDate(),
                 openfluid::base::RunContextManager::instance()->getProjectLastModDate());
  openfluid::base::RunContextManager::instance()->closeProject();

  BOOST_CHECK_EQUAL(openfluid::base::RunContextManager::isProject(Prj1Dir),true);
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->isProjectOpen(), false);

}

