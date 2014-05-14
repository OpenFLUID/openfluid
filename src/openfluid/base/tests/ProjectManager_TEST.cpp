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
  \file ProjectManager_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_prjman

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem.hpp>

#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/tools/SwissTools.hpp>
#include <tests-config.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::ProjectManager::getInstance();

  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->isOpened(), false);
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->getPath(), "");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  std::string Prj1Dir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/prjmanager/prj1").string();

  if (boost::filesystem::is_directory(boost::filesystem::path(Prj1Dir)))
    boost::filesystem::remove_all(boost::filesystem::path(Prj1Dir));


  BOOST_CHECK_EQUAL(openfluid::base::ProjectManager::isProject(Prj1Dir),false);

  openfluid::base::ProjectManager::getInstance()->create(Prj1Dir,"Test project","This is a test project","John Doe", false);

  BOOST_CHECK_EQUAL(openfluid::base::ProjectManager::isProject(Prj1Dir),true);
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->isOpened(), true);
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->getPath(), Prj1Dir);
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->getInputDir(), boost::filesystem::path(Prj1Dir+"/IN").string());
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->getOutputDir(), boost::filesystem::path(Prj1Dir+"/OUT").string());
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->isIncrementalOutputDir(),false);
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->getName(), "Test project");
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->getDescription(), "This is a test project");
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->getAuthors(),"John Doe");
  BOOST_CHECK_EQUAL(openfluid::base::ProjectManager::getInstance()->getCreationDate(),openfluid::base::ProjectManager::getInstance()->getLastModDate());

  openfluid::base::ProjectManager::getInstance()->setIncrementalOutputDir(true);
  openfluid::base::ProjectManager::getInstance()->setName("Modified Test project");
  openfluid::base::ProjectManager::getInstance()->setDescription("This is a modified test project");
  openfluid::base::ProjectManager::getInstance()->setAuthors("John Doe, Tom Morello");

  openfluid::tools::Sleep(1000000);

  openfluid::base::ProjectManager::getInstance()->save();
  openfluid::base::ProjectManager::getInstance()->close();

  BOOST_CHECK_EQUAL(openfluid::base::ProjectManager::isProject(Prj1Dir),true);
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->isOpened(), false);
  BOOST_REQUIRE(openfluid::base::ProjectManager::getInstance()->getPath().empty());

  openfluid::base::ProjectManager::getInstance()->open(Prj1Dir);
  std::string Now = boost::posix_time::to_iso_string(
      boost::posix_time::second_clock::local_time());
  Now[8] = '-';
  openfluid::base::ProjectManager::getInstance()->updateOutputDir();
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->isOpened(), true);
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->getPath(), Prj1Dir);
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->getInputDir(), boost::filesystem::path(Prj1Dir+"/IN").string());

  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->getOutputDir(), boost::filesystem::path(Prj1Dir+"/OUT_"+Now).string());
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->isIncrementalOutputDir(),true);
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->getName(), "Modified Test project");
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->getDescription(), "This is a modified test project");
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->getAuthors(),"John Doe, Tom Morello");
  BOOST_CHECK_NE(openfluid::base::ProjectManager::getInstance()->getCreationDate(),openfluid::base::ProjectManager::getInstance()->getLastModDate());
  openfluid::base::ProjectManager::getInstance()->close();

  BOOST_CHECK_EQUAL(openfluid::base::ProjectManager::isProject(Prj1Dir),true);
  BOOST_REQUIRE_EQUAL(openfluid::base::ProjectManager::getInstance()->isOpened(), false);

}

// =====================================================================
// =====================================================================



