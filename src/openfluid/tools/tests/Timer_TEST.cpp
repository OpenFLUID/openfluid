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
  @file Timer_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_timer


#include <boost/test/unit_test.hpp>

#include <openfluid/tools/Timer.hpp>
#include <openfluid/tools/MiscHelpers.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_prettystring)
{
  std::cout << "------------------------------------" << std::endl;

  int MSecs, Seconds, Minutes, Hours, Days  = 0;

  openfluid::tools::Timer::splitDuration(0,Days,Hours,Minutes,Seconds,MSecs);
  BOOST_REQUIRE_EQUAL(Days,0);
  BOOST_REQUIRE_EQUAL(Hours,0);
  BOOST_REQUIRE_EQUAL(Minutes,0);
  BOOST_REQUIRE_EQUAL(Seconds,0);
  BOOST_REQUIRE_EQUAL(MSecs,0);

  openfluid::tools::Timer::splitDuration(86400000+3600000+60000+1000+1,Days,Hours,Minutes,Seconds,MSecs);
  BOOST_REQUIRE_EQUAL(Days,1);
  BOOST_REQUIRE_EQUAL(Hours,1);
  BOOST_REQUIRE_EQUAL(Minutes,1);
  BOOST_REQUIRE_EQUAL(Seconds,1);
  BOOST_REQUIRE_EQUAL(MSecs,1);


  // ---------------------------------------------------------------------


  for (auto D : {123456789,12345678,1234567,123456,12345,1234,123,12,1,0})
  {
    std::cout << D << "ms = " << openfluid::tools::Timer::getDurationAsPrettyString(D) << std::endl;
  }

  for (auto D : {86400000,3600000,60000,1000,86460001,3601000})
  {
    std::cout << D << "ms = " << openfluid::tools::Timer::getDurationAsPrettyString(D) << std::endl;
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  std::cout << "------------------------------------" << std::endl;

  openfluid::tools::Timer T;

  BOOST_REQUIRE_EQUAL(T.isRunning(),false);

  T.start();
  BOOST_REQUIRE_EQUAL(T.isRunning(),true);
  openfluid::tools::millisleep(50);
  T.stop();
  BOOST_REQUIRE_EQUAL(T.isRunning(),false);
  BOOST_REQUIRE(T.elapsed() >= 40);
  std::cout << T.elapsed() << std::endl;
  std::cout << T.elapsedAsPrettyString() << std::endl;

  T.reset();
  BOOST_REQUIRE_EQUAL(T.elapsed(),0);
  BOOST_REQUIRE_EQUAL(T.isRunning(),false);
  std::cout << T.elapsed() << std::endl;
  std::cout << T.elapsedAsPrettyString() << std::endl;
  T.start();
  BOOST_REQUIRE_EQUAL(T.isRunning(),true);
  T.stop();

  T.restart();
  BOOST_REQUIRE_EQUAL(T.isRunning(),true);
  openfluid::tools::millisleep(50);
  BOOST_REQUIRE(T.elapsed() >= 40);
  BOOST_REQUIRE_EQUAL(T.isRunning(),true);
  openfluid::tools::millisleep(150);
  BOOST_REQUIRE_EQUAL(T.isRunning(),true);
  openfluid::tools::millisleep(50);
  BOOST_REQUIRE(T.elapsed() >= 190);
  openfluid::tools::millisleep(50);
  T.stop();
  BOOST_REQUIRE_EQUAL(T.isRunning(),false);
  BOOST_REQUIRE(T.elapsed() >= 240);
  std::cout << T.elapsed() << std::endl;
  std::cout << T.elapsedAsPrettyString() << std::endl;

  openfluid::tools::Timer TAuto(true);
  BOOST_REQUIRE_EQUAL(TAuto.isRunning(),true);
  openfluid::tools::millisleep(50);
  BOOST_REQUIRE(TAuto.elapsed() >= 40);
  std::cout << TAuto.elapsedAsPrettyString() << std::endl;
  TAuto.stop();
  BOOST_REQUIRE_EQUAL(T.isRunning(),false);
}

