/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file SwissTools_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_swisstools
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include "openfluid-tools.h"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  std::string Str;
  bool BoolValue;
  double DoubleValue;
  std::vector<std::string> StrArray;

  Str = "0.01";
  BOOST_REQUIRE_EQUAL(openfluid::tools::ConvertString(Str,&DoubleValue),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::IsVeryClose(DoubleValue,0.01),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::IsVeryClose(DoubleValue,0.02),false);

  Str = "abcd";
  BOOST_REQUIRE_EQUAL(openfluid::tools::ConvertString(Str,&BoolValue),false);

  Str = "1";
  BOOST_REQUIRE_EQUAL(openfluid::tools::ConvertString(Str,&BoolValue),true);
  BOOST_REQUIRE_EQUAL(1,true);
  BOOST_REQUIRE_EQUAL(0,false);

  DoubleValue = 0.25;
  BOOST_REQUIRE_EQUAL(openfluid::tools::ConvertValue(DoubleValue,&Str),true);
  BOOST_REQUIRE_EQUAL(Str,"0.25");


  Str = "aaa;bbbb;ccccc";
  openfluid::tools::TokenizeString(Str,StrArray,";");
  BOOST_REQUIRE_EQUAL(StrArray.size(),3);
  BOOST_REQUIRE_EQUAL(StrArray[0],"aaa");
  BOOST_REQUIRE_EQUAL(StrArray[1],"bbbb");
  BOOST_REQUIRE_EQUAL(StrArray[2],"ccccc");
  openfluid::tools::TokenizeString(Str,StrArray,"-");
  BOOST_REQUIRE_EQUAL(StrArray.size(),1);

  BOOST_REQUIRE_EQUAL(openfluid::tools::WildcardMatching("*","foobar"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::WildcardMatching("*foobar","foobar"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::WildcardMatching("?foobar","foobar"),false);
  BOOST_REQUIRE_EQUAL(openfluid::tools::WildcardMatching("*bar","foobar"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::WildcardMatching("foo*","foobar"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::WildcardMatching("foo*","fobar"),false);
  BOOST_REQUIRE_EQUAL(openfluid::tools::WildcardMatching("foo?","foobar"),false);
  BOOST_REQUIRE_EQUAL(openfluid::tools::WildcardMatching("foo???","foobar"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::WildcardMatching("foo*foo","foobarfoo"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::WildcardMatching("foo*foo","foofoo"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::WildcardMatching("foo*foo","foobarfo"),false);
  BOOST_REQUIRE_EQUAL(openfluid::tools::WildcardMatching("foo?foo","foobarfoo"),false);
  BOOST_REQUIRE_EQUAL(openfluid::tools::WildcardMatching("foo???foo","foobarfoo"),true);

}

// =====================================================================
// =====================================================================

