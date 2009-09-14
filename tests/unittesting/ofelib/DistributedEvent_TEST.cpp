/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


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
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::DistributedEvent Ev;
  BOOST_REQUIRE_EQUAL(Ev.getInfosCount(),0);

  openfluid::core::DistributedEvent Ev2(openfluid::core::DateTime(2009,9,10,13,55,7));
  BOOST_REQUIRE_EQUAL(Ev2.getInfosCount(),0);
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_infosoperations)
{
  openfluid::core::DistributedEvent Ev(openfluid::core::DateTime(2009,9,10,13,55,7));
  std::string StrInfo;
  long LongInfo;
  double DoubleInfo;
  openfluid::core::ScalarValue ScalarValueInfo;


  Ev.addInfo("test1","value");
  Ev.addInfo("test2","18");
  Ev.addInfo("test3","25.2");

  BOOST_REQUIRE_EQUAL(Ev.getInfosCount(),3);

  BOOST_REQUIRE_EQUAL(Ev.isInfoExists("test1"),true);
  BOOST_REQUIRE_EQUAL(Ev.isInfoExists("test2"),true);
  BOOST_REQUIRE_EQUAL(Ev.isInfoExists("test3"),true);
  BOOST_REQUIRE_EQUAL(Ev.isInfoExists("test4"),false);
  BOOST_REQUIRE_EQUAL(Ev.isInfoExists("test"),false);

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
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsLong("test3",&LongInfo),true);
  BOOST_REQUIRE_EQUAL(LongInfo,25);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsLong("test",&LongInfo),false);

  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDouble("test1",&DoubleInfo),false);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDouble("test2",&DoubleInfo),true);
  BOOST_REQUIRE_CLOSE(DoubleInfo,18.0,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDouble("test3",&DoubleInfo),true);
  BOOST_REQUIRE_CLOSE(DoubleInfo,25.2,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsDouble("test",&DoubleInfo),false);

  BOOST_REQUIRE_EQUAL(Ev.getInfoAsScalarValue("test1",&ScalarValueInfo),false);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsScalarValue("test2",&ScalarValueInfo),true);
  BOOST_REQUIRE_CLOSE(ScalarValueInfo,18.0,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsScalarValue("test3",&ScalarValueInfo),true);
  BOOST_REQUIRE_CLOSE(ScalarValueInfo,25.2,0.1);
  BOOST_REQUIRE_EQUAL(Ev.getInfoAsScalarValue("test",&ScalarValueInfo),false);
}

// =====================================================================
// =====================================================================

