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
  @file InternalLogging_TEST.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_binding


#include <vector>
#include <string>

#include <boost/test/unit_test.hpp>

#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/utils/InternalLogger.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_logging_setup)
{
  openfluid::utils::log::error("TEST", "foo");
  BOOST_CHECK(!openfluid::utils::LoggingSystem::instance()->isActivated());

  openfluid::utils::log::setup();
  std::string PathDefault = openfluid::utils::LoggingSystem::instance()->getLogPath();
  BOOST_CHECK(openfluid::utils::LoggingSystem::instance()->isActivated());
  openfluid::utils::LoggingSystem::instance()->close();

  openfluid::utils::log::setup(true, "./bar");
  std::string PathCustom = openfluid::utils::LoggingSystem::instance()->getLogPath();
  BOOST_CHECK(PathDefault != PathCustom);
  openfluid::utils::LoggingSystem::instance()->close();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_logging_output)
{
  openfluid::utils::log::setup();
  openfluid::utils::log::error("TEST", "foo");
  BOOST_CHECK(openfluid::utils::LoggingSystem::instance()->getLastLog() == "");

  // check redirection to vector of logs
  openfluid::utils::LoggingSystem::instance()->setOutput(false);
  openfluid::utils::log::error("TEST", "foo");
  BOOST_CHECK(openfluid::utils::LoggingSystem::instance()->getLastLog() == "foo");

  // check log file output content
  openfluid::utils::LoggingSystem::instance()->setOutput(true);
  std::string ErrorContent = "foo written";
  openfluid::utils::log::error("TEST", ErrorContent);
  openfluid::utils::LoggingSystem::instance()->close();
  std::string LogPath = openfluid::utils::LoggingSystem::instance()->getLogPath();
  std::string LogContent = openfluid::tools::Filesystem::readFile(LogPath);
  BOOST_CHECK(LogContent.find(ErrorContent) != std::string::npos);
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  openfluid::base::Environment::init();

  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
