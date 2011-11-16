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
  \file DistributedEvent_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
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
  double ScalarValueInfo;
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

  BOOST_REQUIRE_EQUAL(Ev.getInfoAsString("test1",&StrInfo),true);
  BOOST_REQUIRE_EQUAL(StrInfo,"value");
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsString("test2",&StrInfo),true);
  BOOST_REQUIRE_EQUAL(StrInfo,"18");
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsString("test3",&StrInfo),true);
  BOOST_REQUIRE_EQUAL(StrInfo,"25.2");
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsString("test",&StrInfo),false);

  BOOST_REQUIRE_EQUAL(Ev.getInfoAsLong("test1",&LongInfo),false);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsLong("test2",&LongInfo),true);
  BOOST_REQUIRE_EQUAL(LongInfo,18);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsLong("test3",&LongInfo),false);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsLong("test",&LongInfo),false);

  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDouble("test1",&DoubleInfo),false);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDouble("test2",&DoubleInfo),true);
  BOOST_REQUIRE_CLOSE(DoubleInfo,18.0,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDouble("test3",&DoubleInfo),true);
  BOOST_REQUIRE_CLOSE(DoubleInfo,25.2,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDouble("test",&DoubleInfo),false);

  // test for compatibility
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsScalarValue("test1",&ScalarValueInfo),false);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsScalarValue("test2",&ScalarValueInfo),true);
  BOOST_REQUIRE_CLOSE(ScalarValueInfo,18.0,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsScalarValue("test3",&ScalarValueInfo),true);
  BOOST_REQUIRE_CLOSE(ScalarValueInfo,25.2,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsScalarValue("test",&ScalarValueInfo),false);

  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDoubleValue("test1",DoubleValueInfo),false);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDoubleValue("test2",DoubleValueInfo),true);
  BOOST_REQUIRE_CLOSE(DoubleValueInfo.get(),18.0,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDoubleValue("test3",DoubleValueInfo),true);
  BOOST_REQUIRE_CLOSE(DoubleValueInfo.get(),25.2,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDoubleValue("test",DoubleValueInfo),false);
}

// =====================================================================
// =====================================================================

