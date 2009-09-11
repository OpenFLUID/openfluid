/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file DateTime_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_unit
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::DateTime DT(2009,9,8,13,11,25);

  BOOST_REQUIRE_EQUAL(DT.getYear(),2009);
  BOOST_REQUIRE_EQUAL(DT.getMonth(),9);
  BOOST_REQUIRE_EQUAL(DT.getDay(),8);
  BOOST_REQUIRE_EQUAL(DT.getHour(),13);
  BOOST_REQUIRE_EQUAL(DT.getMinute(),11);
  BOOST_REQUIRE_EQUAL(DT.getSecond(),25);
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_rawtime)
{
  openfluid::core::DateTime DT,DT2;
  openfluid::core::rawtime_t RawTime;
  const unsigned int CasesCount = 4;


  int DTCases[CasesCount][6] = {
      {2009,9,8,13,19,25},
      {0,0,0,0,0,0},
      {1999,12,31,23,59,59},
      {2000,0,0,0,0,0}
    };


  for (unsigned int i=0;i<CasesCount;i++)
  {

    DT.set(DTCases[i][0],DTCases[i][1],DTCases[i][2],DTCases[i][3],DTCases[i][4],DTCases[i][5]);
    RawTime = DT.getRawTime();
    DT2.set(RawTime);

    BOOST_REQUIRE_EQUAL(DT2.getYear(),DT.getYear());
    BOOST_REQUIRE_EQUAL(DT2.getMonth(),DT.getMonth());
    BOOST_REQUIRE_EQUAL(DT2.getDay(),DT.getDay());
    BOOST_REQUIRE_EQUAL(DT2.getHour(),DT.getHour());
    BOOST_REQUIRE_EQUAL(DT2.getMinute(),DT.getMinute());
    BOOST_REQUIRE_EQUAL(DT2.getSecond(),DT.getSecond());
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::DateTime DT(2008,12,30,13,11,25);

  openfluid::core::DateTime DTResult;

  openfluid::core::rawtime_t DiffInSeconds;

  DTResult = DT + 37;
  BOOST_REQUIRE_EQUAL(DTResult.getYear(),2008);
  BOOST_REQUIRE_EQUAL(DTResult.getMonth(),12);
  BOOST_REQUIRE_EQUAL(DTResult.getDay(),30);
  BOOST_REQUIRE_EQUAL(DTResult.getHour(),13);
  BOOST_REQUIRE_EQUAL(DTResult.getMinute(),12);
  BOOST_REQUIRE_EQUAL(DTResult.getSecond(),2);

  DTResult = DTResult + openfluid::core::DateTime::Minute();
  BOOST_REQUIRE_EQUAL(DTResult.getYear(),2008);
  BOOST_REQUIRE_EQUAL(DTResult.getMonth(),12);
  BOOST_REQUIRE_EQUAL(DTResult.getDay(),30);
  BOOST_REQUIRE_EQUAL(DTResult.getHour(),13);
  BOOST_REQUIRE_EQUAL(DTResult.getMinute(),13);
  BOOST_REQUIRE_EQUAL(DTResult.getSecond(),2);

  DTResult = DTResult + openfluid::core::DateTime::Minutes(9);
  BOOST_REQUIRE_EQUAL(DTResult.getYear(),2008);
  BOOST_REQUIRE_EQUAL(DTResult.getMonth(),12);
  BOOST_REQUIRE_EQUAL(DTResult.getDay(),30);
  BOOST_REQUIRE_EQUAL(DTResult.getHour(),13);
  BOOST_REQUIRE_EQUAL(DTResult.getMinute(),22);
  BOOST_REQUIRE_EQUAL(DTResult.getSecond(),2);

  DTResult = DTResult + openfluid::core::DateTime::Hour();
  BOOST_REQUIRE_EQUAL(DTResult.getYear(),2008);
  BOOST_REQUIRE_EQUAL(DTResult.getMonth(),12);
  BOOST_REQUIRE_EQUAL(DTResult.getDay(),30);
  BOOST_REQUIRE_EQUAL(DTResult.getHour(),14);
  BOOST_REQUIRE_EQUAL(DTResult.getMinute(),22);
  BOOST_REQUIRE_EQUAL(DTResult.getSecond(),2);

  DTResult = DTResult + openfluid::core::DateTime::Hours(30);
  BOOST_REQUIRE_EQUAL(DTResult.getYear(),2008);
  BOOST_REQUIRE_EQUAL(DTResult.getMonth(),12);
  BOOST_REQUIRE_EQUAL(DTResult.getDay(),31);
  BOOST_REQUIRE_EQUAL(DTResult.getHour(),20);
  BOOST_REQUIRE_EQUAL(DTResult.getMinute(),22);
  BOOST_REQUIRE_EQUAL(DTResult.getSecond(),2);

  DTResult = DTResult - openfluid::core::DateTime::Day();
  BOOST_REQUIRE_EQUAL(DTResult.getYear(),2008);
  BOOST_REQUIRE_EQUAL(DTResult.getMonth(),12);
  BOOST_REQUIRE_EQUAL(DTResult.getDay(),30);
  BOOST_REQUIRE_EQUAL(DTResult.getHour(),20);
  BOOST_REQUIRE_EQUAL(DTResult.getMinute(),22);
  BOOST_REQUIRE_EQUAL(DTResult.getSecond(),2);

  DTResult = DTResult + openfluid::core::DateTime::Days(5);
  BOOST_REQUIRE_EQUAL(DTResult.getYear(),2009);
  BOOST_REQUIRE_EQUAL(DTResult.getMonth(),1);
  BOOST_REQUIRE_EQUAL(DTResult.getDay(),4);
  BOOST_REQUIRE_EQUAL(DTResult.getHour(),20);
  BOOST_REQUIRE_EQUAL(DTResult.getMinute(),22);
  BOOST_REQUIRE_EQUAL(DTResult.getSecond(),2);

  DiffInSeconds = DTResult.diffInSeconds(DT);
  BOOST_REQUIRE_EQUAL(DiffInSeconds,457837);

  DiffInSeconds = DT.diffInSeconds(DTResult);
  BOOST_REQUIRE_EQUAL(DiffInSeconds,-457837);

  BOOST_REQUIRE_EQUAL(openfluid::core::DateTime::isLeapYear(2004),true);

  BOOST_REQUIRE_EQUAL(openfluid::core::DateTime::isLeapYear(1903),false);

  BOOST_REQUIRE_EQUAL(openfluid::core::DateTime::isValidDateTime(2004,2,29,13,2,2),true);
  BOOST_REQUIRE_EQUAL(openfluid::core::DateTime::isValidDateTime(2003,2,29,13,2,2),false);
  BOOST_REQUIRE_EQUAL(openfluid::core::DateTime::isValidDateTime(2004,4,31,13,2,2),false);
  BOOST_REQUIRE_EQUAL(openfluid::core::DateTime::isValidDateTime(2004,1,1,13,72,2),false);
  BOOST_REQUIRE_EQUAL(openfluid::core::DateTime::isValidDateTime(2004,1,1,13,12,2),true);

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_tofromstring)
{
  openfluid::core::DateTime DT;
  std::string DTStr;

  DT.setFromISOString("2009-09-09 16:36:25");
  BOOST_REQUIRE_EQUAL(DT.getYear(),2009);
  BOOST_REQUIRE_EQUAL(DT.getMonth(),9);
  BOOST_REQUIRE_EQUAL(DT.getDay(),9);
  BOOST_REQUIRE_EQUAL(DT.getHour(),16);
  BOOST_REQUIRE_EQUAL(DT.getMinute(),36);
  BOOST_REQUIRE_EQUAL(DT.getSecond(),25);

  DTStr = DT.getAsISOString();
  BOOST_REQUIRE_EQUAL(DTStr,"2009-09-09 16:36:25");
}
// =====================================================================
// =====================================================================
