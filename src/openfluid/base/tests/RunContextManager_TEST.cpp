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
#define BOOST_TEST_MODULE unittest_runcontextmanager


#include <ctime>
#include <chrono>

#include <boost/test/unit_test.hpp>

#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/global.hpp>

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
#if !defined(OPENFLUID_OS_WINDOWS)
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getInputDir(),"/foo/bar");
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getOutputDir(),"/bar/baz");
#endif
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
  std::string Prj1Dir = CONFIGTESTS_OUTPUT_DATA_DIR+"/RunContextManager/prj1";

  if (openfluid::tools::Filesystem::isDirectory(Prj1Dir))
  {
    openfluid::tools::Filesystem::removeDirectory(Prj1Dir);
  }


  BOOST_CHECK_EQUAL(openfluid::base::RunContextManager::isProject(Prj1Dir),false);

  openfluid::base::RunContextManager::instance()->createProject(Prj1Dir,
                                                                "Test project","This is a test project",
                                                                "John Doe", false);

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

  BOOST_CHECK(!openfluid::base::RunContextManager::instance()->projectContainsDeprecatedFile(Prj1Dir));

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

  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->isProjectIncrementalOutputDir(),true);
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getOutputDir(),Prj1Dir+"/OUT_"+Now);
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectName(), "Modified Test project");
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectDescription(),
                      "This is a modified test project");
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->getProjectAuthors(),"John Doe, Tom Morello");
  BOOST_CHECK_NE(openfluid::base::RunContextManager::instance()->getProjectCreationDate(),
                 openfluid::base::RunContextManager::instance()->getProjectLastModDate());
  openfluid::base::RunContextManager::instance()->closeProject();

  BOOST_CHECK(openfluid::base::RunContextManager::isProject(Prj1Dir));
  BOOST_REQUIRE_EQUAL(openfluid::base::RunContextManager::instance()->isProjectOpen(), false);

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_project_fileformat_conversion_simple)
{
  std::string OrigConfPrjDir = CONFIGTESTS_INPUT_MISCDATA_DIR+"/RunContextManager/SimpleConf";
  std::string ConfPrjDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/RunContextManager/SimpleConf";

  if (openfluid::tools::Filesystem::isDirectory(ConfPrjDir))
  {
    openfluid::tools::Filesystem::removeDirectory(ConfPrjDir);
  }
  openfluid::tools::Filesystem::copyDirectory(OrigConfPrjDir,ConfPrjDir);

  
  auto CtxtMan = openfluid::base::RunContextManager::instance();

  BOOST_REQUIRE(CtxtMan->openProject(ConfPrjDir));
  BOOST_CHECK_EQUAL(CtxtMan->isProjectOpen(),true);
  BOOST_CHECK_EQUAL(CtxtMan->getProjectPath(),ConfPrjDir);
  BOOST_CHECK_EQUAL(CtxtMan->getInputDir(),ConfPrjDir+"/IN");
  BOOST_CHECK_EQUAL(CtxtMan->getOutputDir(),ConfPrjDir+"/OUT");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectName(),"Former Test project");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectDescription(),"This is a former project file for tests");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectAuthors(),"Nina Simone, John Coltrane");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectCreationDate(),"20210610T140435");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectLastModDate(),"20210610T140436");
  BOOST_CHECK_EQUAL(CtxtMan->isProjectIncrementalOutputDir(),true);

  openfluid::base::RunContextManager::instance()->closeProject();

  BOOST_CHECK(openfluid::base::RunContextManager::isProject(ConfPrjDir));
  BOOST_CHECK(openfluid::base::RunContextManager::instance()->projectContainsDeprecatedFile(ConfPrjDir));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_project_fileformat_conversion_complex)
{
  std::string OrigConfPrjDir = CONFIGTESTS_INPUT_MISCDATA_DIR+"/RunContextManager/ComplexConf";
  std::string ConfPrjDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/RunContextManager/ComplexConf";

  if (openfluid::tools::Filesystem::isDirectory(ConfPrjDir))
  {
    openfluid::tools::Filesystem::removeDirectory(ConfPrjDir);
  }

  openfluid::tools::Filesystem::copyDirectory(OrigConfPrjDir,ConfPrjDir);


  auto CtxtMan = openfluid::base::RunContextManager::instance();

  BOOST_REQUIRE(CtxtMan->openProject(ConfPrjDir));
  BOOST_CHECK_EQUAL(CtxtMan->isProjectOpen(),true);
  BOOST_CHECK_EQUAL(CtxtMan->getProjectPath(),ConfPrjDir);
  BOOST_CHECK_EQUAL(CtxtMan->getInputDir(),ConfPrjDir+"/IN");
  BOOST_CHECK_EQUAL(CtxtMan->getOutputDir(),ConfPrjDir+"/OUT");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectName(),"Coding Heroes");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectDescription(),
                    "Example of OpenFLUID Project with bla bla bla. This is bla bla bla...");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectAuthors(),"Alan Turing, Katherine Johnson, Margaret Hamilton");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectCreationDate(),"20150924T101640");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectLastModDate(),"20150924T101640");
  BOOST_CHECK_EQUAL(CtxtMan->isProjectIncrementalOutputDir(),false);
  BOOST_CHECK_EQUAL(CtxtMan->getProjectContextValue(
    "/builder/model/graphicalview/positions/examples.apollo.ballistics").get<std::string>(),
    "@Point(-484 -201)");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectContextValue(
    "/builder/model/graphicalview/positions/examples.engine.fuel-management").get<std::string>(),
    "@Point(-346 -395)");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectContextValue(
    "/builder/model/graphicalview/color_mode").get<std::string>(),"BACKGROUND");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectContextValue(
    "/builder/model/colors","examples.earth.gravitation-system").get<std::string>(),"#aaffff");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectContextValue(
    "/builder/model/colors","examples.rocket.control").get<std::string>(),"#00aa00");
  BOOST_CHECK_EQUAL(CtxtMan->getProjectContextValue("/builder/runconfig/maxthreads").get<int>(),16);
  BOOST_CHECK(!CtxtMan->getProjectContextValue("/builder/runconfig/profiling").get<bool>());
  BOOST_CHECK(CtxtMan->getProjectContextValue("/builder/runconfig/clear_outdir").get<bool>());
  BOOST_CHECK_EQUAL(CtxtMan->getProjectContextValue("/builder/simulation/mode").get<std::string>(),"cli");
  BOOST_CHECK_EQUAL(
    CtxtMan->getProjectContextValue("/builder/spatial/unitsclasses/styles/AU/fillcolor").get<std::string>(),
    "#5386b0"
  );
  BOOST_CHECK_EQUAL(
    CtxtMan->getProjectContextValue("/builder/spatial/unitsclasses/styles/AU/linecolor").get<std::string>(),
    "#3ac599"
  );
  BOOST_CHECK_EQUAL(
    CtxtMan->getProjectContextValue("/builder/spatial/unitsclasses/styles/AU/linewidth").get<int>(),3
  );
  BOOST_CHECK_EQUAL(
    CtxtMan->getProjectContextValue("/builder/spatial/unitsclasses/styles/LU/fillcolor").get<std::string>(),
    "#55aa00"
  );
  BOOST_CHECK_EQUAL(
    CtxtMan->getProjectContextValue("/builder/spatial/unitsclasses/styles/LU/linecolor").get<std::string>(),
    "#005500"
  );
  BOOST_CHECK_EQUAL(
    CtxtMan->getProjectContextValue("/builder/spatial/unitsclasses/styles/LU/linewidth").get<int>(),1);
  auto ClassesOrder = 
    CtxtMan->getProjectContextValue("/builder/spatial/unitsclasses/order").get<std::vector<std::string>>();
  BOOST_CHECK_EQUAL(ClassesOrder[0],"LU");
  BOOST_CHECK_EQUAL(ClassesOrder[1],"AU");

  openfluid::base::RunContextManager::instance()->closeProject();
  BOOST_CHECK(openfluid::base::RunContextManager::isProject(ConfPrjDir));
  BOOST_CHECK(openfluid::base::RunContextManager::instance()->projectContainsDeprecatedFile(ConfPrjDir));
}
