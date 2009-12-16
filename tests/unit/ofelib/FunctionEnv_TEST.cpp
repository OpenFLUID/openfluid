/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file FunctionEnv_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_functionenv
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include "openfluid-core.h"
#include "openfluid-base.h"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::FunctionEnvironment FuncEnv;
  bool BoolValue;
  std::string StrValue;

  BOOST_REQUIRE_EQUAL(FuncEnv.getValue("fakebool",&BoolValue),false);
  BOOST_REQUIRE_EQUAL(FuncEnv.getValue("fakestring",&StrValue),false);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::base::FunctionEnvironment FuncEnv;
  bool BoolValue;
  std::string StrValue;



  BOOST_REQUIRE_EQUAL(FuncEnv.setValue("bool1",true),true);
  BOOST_REQUIRE_EQUAL(FuncEnv.getValue("bool1",&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,true);

  BOOST_REQUIRE_EQUAL(FuncEnv.setValue("bool1",false),true);
  BOOST_REQUIRE_EQUAL(FuncEnv.getValue("bool1",&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,false);

  BOOST_REQUIRE_EQUAL(FuncEnv.setValue("bool2",false),true);
  BOOST_REQUIRE_EQUAL(FuncEnv.getValue("bool2",&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,false);

  BOOST_REQUIRE_EQUAL(FuncEnv.getValue("fakebool",&BoolValue),false);



  BOOST_REQUIRE_EQUAL(FuncEnv.setValue("str1",std::string("first string")),true);
  BOOST_REQUIRE_EQUAL(FuncEnv.getValue("str1",&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,"first string");

  BOOST_REQUIRE_EQUAL(FuncEnv.setValue("str1",std::string("string one")),true);
  BOOST_REQUIRE_EQUAL(FuncEnv.getValue("str1",&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,"string one");

  BOOST_REQUIRE_EQUAL(FuncEnv.setValue("str2",std::string("string 2")),true);
  BOOST_REQUIRE_EQUAL(FuncEnv.getValue("str2",&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,"string 2");

  BOOST_REQUIRE_EQUAL(FuncEnv.getValue("fakestring",&StrValue),false);

}

// =====================================================================
// =====================================================================
