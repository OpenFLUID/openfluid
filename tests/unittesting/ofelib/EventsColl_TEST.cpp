/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


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
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::EventsCollection EvColl, EvColl2;
  BOOST_REQUIRE_EQUAL(EvColl.getCount(),0);

  EvColl.getEventsBetween(openfluid::core::DateTime(1979,1,1,0,0,0),openfluid::core::DateTime(2019,1,1,0,0,0),&EvColl2);
  BOOST_REQUIRE_EQUAL(EvColl2.getCount(),0);
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{

  openfluid::core::EventsCollection EvColl, EvColl2;
  openfluid::core::Event* Ev;

  Ev = new openfluid::core::Event(openfluid::core::DateTime(1999,1,1,6,0,0));
  Ev->addInfo("test1","1");
  Ev->addInfo("test2","2");
  EvColl.addEvent(Ev);

  Ev = new openfluid::core::Event(openfluid::core::DateTime(2003,2,5,6,0,0));
  Ev->addInfo("test11","11");
  Ev->addInfo("test22","22");
  EvColl.addEvent(Ev);

  Ev = new openfluid::core::Event(openfluid::core::DateTime(2023,2,5,6,0,0));
  Ev->addInfo("test111","111");
  Ev->addInfo("test222","222");
  Ev->addInfo("test333","333");
  EvColl.addEvent(Ev);

  Ev = new openfluid::core::Event(openfluid::core::DateTime(2010,7,31,16,30,0));
  Ev->addInfo("specialthing","wedding");
  EvColl.addEvent(Ev);


  BOOST_REQUIRE_EQUAL(EvColl.getEventsList()->size(),4);

  EvColl2.clear();
  EvColl.getEventsBetween(openfluid::core::DateTime(1979,1,1,0,0,0),openfluid::core::DateTime(2019,1,1,0,0,0),&EvColl2);
  BOOST_REQUIRE_EQUAL(EvColl2.getCount(),3);

  EvColl2.clear();
  EvColl.getEventsBetween(openfluid::core::DateTime(2010,1,1,0,0,0),openfluid::core::DateTime(2010,12,31,23,59,59),&EvColl2);
  BOOST_REQUIRE_EQUAL(EvColl2.getCount(),1);

  BOOST_REQUIRE_EQUAL(EvColl2.getEventsList()->front()->isInfoEqual("specialthing","wedding"),true);
  BOOST_REQUIRE_EQUAL(EvColl2.getEventsList()->front()->isInfoExist("test333"),false);



}

// =====================================================================
// =====================================================================
