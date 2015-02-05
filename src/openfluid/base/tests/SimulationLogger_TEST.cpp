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
  @file SimulationLogger_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_simlogger
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/base/SimulationLogger.hpp>
#include <tests-config.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::SimulationLogger SimLog(CONFIGTESTS_OUTPUT_DATA_DIR+"/checksimlog1.log");

  BOOST_REQUIRE_EQUAL(SimLog.getWarningsCount(),0);
  BOOST_REQUIRE_EQUAL(SimLog.isWarningFlag(),false);

  SimLog.addInfo("Hello World!");

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::base::SimulationLogger* SimLog =
      new openfluid::base::SimulationLogger(CONFIGTESTS_OUTPUT_DATA_DIR+"/checksimlog2.log");

  SimLog->addWarning("Sender",1,"Warning message #1");
  BOOST_REQUIRE_EQUAL(SimLog->getWarningsCount(),1);
  BOOST_REQUIRE_EQUAL(SimLog->isWarningFlag(),true);
  SimLog->resetWarningFlag();
  BOOST_REQUIRE_EQUAL(SimLog->isWarningFlag(),false);
  BOOST_REQUIRE_EQUAL(SimLog->getWarningsCount(),1);

  SimLog->addWarning("Sender","Warning message #2");
  SimLog->addWarning("Sender",std::string("Source"),1,"Warning message #3");
  BOOST_REQUIRE_EQUAL(SimLog->getWarningsCount(),3);
  BOOST_REQUIRE_EQUAL(SimLog->isWarningFlag(),true);
  SimLog->resetWarningFlag();
  BOOST_REQUIRE_EQUAL(SimLog->isWarningFlag(),false);
  BOOST_REQUIRE_EQUAL(SimLog->getWarningsCount(),3);
  BOOST_REQUIRE_EQUAL(SimLog->getWarningsCount(),3);
  BOOST_REQUIRE_EQUAL(SimLog->isWarningFlag(),false);

  SimLog->addWarning("Sender","Source","Warning message #4");
  BOOST_REQUIRE_EQUAL(SimLog->getWarningsCount(),4);
  BOOST_REQUIRE_EQUAL(SimLog->isWarningFlag(),true);

  SimLog->addMessage("Sender","Message #1");

  SimLog->addInfo("Info #1");

  delete SimLog;

}
