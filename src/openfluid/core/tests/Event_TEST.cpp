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
  @file Event_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_event


#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <openfluid/core/Event.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::Event Ev;
  BOOST_REQUIRE_EQUAL(Ev.getInfosCount(),0);

  openfluid::core::Event Ev2(openfluid::core::DateTime(2009,9,10,13,55,7));
  BOOST_REQUIRE_EQUAL(Ev2.getInfosCount(),0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_infosoperations)
{
  openfluid::core::Event Ev(openfluid::core::DateTime(2009,9,10,13,55,7));
  std::string StrInfo;
  long LongInfo;
  double DoubleInfo;
  openfluid::core::DoubleValue DoubleValueInfo;


  Ev.addInfo("test1","value");
  Ev.addInfo("test2","18");
  Ev.addInfo("test3","25.2");

  BOOST_REQUIRE_EQUAL(Ev.getInfosCount(),3);

  BOOST_REQUIRE_EQUAL(Ev.isInfoExist("test1"),true);
  BOOST_REQUIRE_EQUAL(Ev.isInfoExist("test2"),true);
  BOOST_REQUIRE_EQUAL(Ev.isInfoExist("test3"),true);
  BOOST_REQUIRE_EQUAL(Ev.isInfoExist("test4"),false);
  BOOST_REQUIRE_EQUAL(Ev.isInfoExist("test"),false);

  BOOST_REQUIRE_EQUAL(Ev.getInfoAsString("test1",StrInfo),true);
  BOOST_REQUIRE_EQUAL(StrInfo,"value");
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsString("test2",StrInfo),true);
  BOOST_REQUIRE_EQUAL(StrInfo,"18");
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsString("test3",StrInfo),true);
  BOOST_REQUIRE_EQUAL(StrInfo,"25.2");
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsString("test",StrInfo),false);

  BOOST_REQUIRE_EQUAL(Ev.getInfoAsLong("test1",LongInfo),false);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsLong("test2",LongInfo),true);
  BOOST_REQUIRE_EQUAL(LongInfo,18);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsLong("test3",LongInfo),false);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsLong("test",LongInfo),false);

  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDouble("test1",DoubleInfo),false);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDouble("test2",DoubleInfo),true);
  BOOST_REQUIRE_CLOSE(DoubleInfo,18.0,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDouble("test3",DoubleInfo),true);
  BOOST_REQUIRE_CLOSE(DoubleInfo,25.2,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDouble("test",DoubleInfo),false);

  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDoubleValue("test1",DoubleValueInfo),false);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDoubleValue("test2",DoubleValueInfo),true);
  BOOST_REQUIRE_CLOSE(DoubleValueInfo.get(),18.0,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDoubleValue("test3",DoubleValueInfo),true);
  BOOST_REQUIRE_CLOSE(DoubleValueInfo.get(),25.2,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDoubleValue("test",DoubleValueInfo),false);
}


// =====================================================================
// =====================================================================

