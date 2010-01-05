/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file Variable_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_variable
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::Variables<double> Vars;
  double Value;

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo"),false);
  BOOST_REQUIRE_EQUAL(Vars.getVariableValuesCount("foo"),-1);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",0,&Value),false);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::Variables<double> Vars;
  double Value;

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo"),false);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",0),false);
  BOOST_REQUIRE_EQUAL(Vars.getVariableValuesCount("foo"),-1);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",0,&Value),false);

  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",0.0),false);

  BOOST_REQUIRE_EQUAL(Vars.createVariable("foo"),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo"),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",0),false);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",0.0),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",1.0),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",2.0),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",3.0),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("foo",4.0),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",0),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",4),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",5),false);
  BOOST_REQUIRE_EQUAL(Vars.getVariableValuesCount("foo"),5);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",0,&Value),true);
  BOOST_REQUIRE_CLOSE(Value,0.0,0.001);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",3,&Value),true);
  BOOST_REQUIRE_CLOSE(Value,3.0,0.001);
  BOOST_REQUIRE_EQUAL(Vars.modifyValue("foo",3,34.5),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",3,&Value),true);
  BOOST_REQUIRE_CLOSE(Value,34.5,0.001);

  BOOST_REQUIRE_EQUAL(Vars.createVariable("bar"),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",1000.1),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",1001.1),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",1002.1),true);
  BOOST_REQUIRE_EQUAL(Vars.appendValue("bar",1003.1),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",0,&Value),true);
  BOOST_REQUIRE_CLOSE(Value,1000.1,0.001);
  BOOST_REQUIRE_EQUAL(Vars.getValue("bar",2,&Value),true);
  BOOST_REQUIRE_CLOSE(Value,1002.1,0.001);

  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo"),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("bar"),true);

  BOOST_REQUIRE_EQUAL(Vars.releaseMemory(2),true);
  BOOST_REQUIRE_EQUAL(Vars.isVariableExist("foo",0),true);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",0,&Value),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",2,&Value),false);
  BOOST_REQUIRE_EQUAL(Vars.getValue("foo",3,&Value),true);
  BOOST_REQUIRE_CLOSE(Value,34.5,0.001);
}

// =====================================================================
// =====================================================================
