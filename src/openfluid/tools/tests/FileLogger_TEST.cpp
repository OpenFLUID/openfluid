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
  @file FileLogger_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_filelogger


#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/tools/FileLogger.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations_1)
{
  openfluid::tools::FileLogger Log;

  Log.init(CONFIGTESTS_OUTPUT_DATA_DIR+"/filelogger1.log");

  Log.add(openfluid::tools::FileLogger::LOG_INFO,"test","info 1");
  Log.add(openfluid::tools::FileLogger::LOG_DEBUG,"test","debug 1");
  Log.add(openfluid::tools::FileLogger::LOG_INFO,"test","info 2");
  Log.add(openfluid::tools::FileLogger::LOG_WARNING,"test","warning 1");
  Log.add(openfluid::tools::FileLogger::LOG_ERROR,"test","error");

  BOOST_REQUIRE_EQUAL(Log.getWarningsCount(),1);
  BOOST_REQUIRE_EQUAL(Log.getInfosCount(),2);
  BOOST_REQUIRE(Log.isError());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations_2)
{
  openfluid::tools::FileLogger Log;

  Log.init(CONFIGTESTS_OUTPUT_DATA_DIR+"/filelogger2.log");

  Log.add(openfluid::tools::FileLogger::LOG_INFO,"test","info 1");
  Log.add(openfluid::tools::FileLogger::LOG_DEBUG,"test","debug 1");
  Log.add(openfluid::tools::FileLogger::LOG_INFO,"test","info 2");
  Log.add(openfluid::tools::FileLogger::LOG_WARNING,"test","warning");
  Log.add(openfluid::tools::FileLogger::LOG_WARNING,"test","warning");
  Log.add(openfluid::tools::FileLogger::LOG_WARNING,"test","warning");
  Log.add(openfluid::tools::FileLogger::LOG_WARNING,"test","warning");
  Log.add(openfluid::tools::FileLogger::LOG_WARNING,"test","warning");

  BOOST_REQUIRE_EQUAL(Log.getWarningsCount(),5);
  BOOST_REQUIRE_EQUAL(Log.getInfosCount(),2);
  BOOST_REQUIRE(!Log.isError());
}

