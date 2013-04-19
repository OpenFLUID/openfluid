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
  \file EventsColl_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_eventscoll
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/core/EventsColl.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::EventsCollection EvColl, EvColl2;
  BOOST_REQUIRE_EQUAL(EvColl.getCount(),0);

  EvColl.getEventsBetween(openfluid::core::DateTime(1979,1,1,0,0,0),openfluid::core::DateTime(2019,1,1,0,0,0),EvColl2);
  BOOST_REQUIRE_EQUAL(EvColl2.getCount(),0);
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{

  openfluid::core::EventsCollection EvColl, EvColl2, EvColl3;
  openfluid::core::Event Ev;

  Ev = openfluid::core::Event(openfluid::core::DateTime(1999,1,1,6,0,0));
  Ev.addInfo("test1","1");
  Ev.addInfo("test2","2");
  EvColl.addEvent(Ev);

  Ev = openfluid::core::Event(openfluid::core::DateTime(2003,2,5,6,0,0));
  Ev.addInfo("test11","11");
  Ev.addInfo("test22","22");
  EvColl.addEvent(Ev);

  Ev = openfluid::core::Event(openfluid::core::DateTime(2023,2,5,6,0,0));
  Ev.addInfo("test111","111");
  Ev.addInfo("test222","222");
  Ev.addInfo("test333","333");
  EvColl.addEvent(Ev);

  Ev = openfluid::core::Event(openfluid::core::DateTime(2010,7,31,16,30,0));
  Ev.addInfo("specialthing","wedding");
  EvColl.addEvent(Ev);


  BOOST_REQUIRE_EQUAL(EvColl.getEventsList()->size(),4);

  EvColl2.clear();
  EvColl.getEventsBetween(openfluid::core::DateTime(1979,1,1,0,0,0),openfluid::core::DateTime(2019,1,1,0,0,0),EvColl2);
  BOOST_REQUIRE_EQUAL(EvColl2.getCount(),3);

  EvColl2.clear();
  EvColl.getEventsBetween(openfluid::core::DateTime(2010,1,1,0,0,0),openfluid::core::DateTime(2010,12,31,23,59,59),EvColl2);
  BOOST_REQUIRE_EQUAL(EvColl2.getCount(),1);

  BOOST_REQUIRE_EQUAL(EvColl2.getEventsList()->front().isInfoEqual("specialthing","wedding"),true);
  BOOST_REQUIRE_EQUAL(EvColl2.getEventsList()->front().isInfoExist("test333"),false);



  Ev = openfluid::core::Event(openfluid::core::DateTime(1999,1,1,6,0,0));
  Ev.addInfo("test1","1");
  Ev.addInfo("test2","2");
  EvColl3.addEvent(Ev);

  Ev = openfluid::core::Event(openfluid::core::DateTime(2003,2,5,6,0,0));
  Ev.addInfo("test11","11");
  Ev.addInfo("test22","22");
  EvColl3.addEvent(Ev);

  Ev = openfluid::core::Event(openfluid::core::DateTime(2023,2,5,6,0,0));
  Ev.addInfo("test111","111");
  Ev.addInfo("test222","222");
  Ev.addInfo("test333","333");
  EvColl3.addEvent(Ev);

  Ev = openfluid::core::Event(openfluid::core::DateTime(2010,7,31,16,30,0));
  Ev.addInfo("specialthing","wedding");
  EvColl3.addEvent(Ev);

  BOOST_REQUIRE_EQUAL(EvColl3.getCount(),4);

  EvColl3.clear();
  BOOST_REQUIRE_EQUAL(EvColl3.getCount(),0);
}

// =====================================================================
// =====================================================================
