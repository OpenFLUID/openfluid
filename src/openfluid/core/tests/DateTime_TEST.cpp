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
  \file DateTime_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_datetime
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/core/DateTime.hpp>


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
  openfluid::core::RawTime_t RawTime;
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

  openfluid::core::RawTime_t DiffInSeconds;

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

  BOOST_REQUIRE_EQUAL(DT.setFromISOString("2009-09-09 16:36:25"),true);
  BOOST_REQUIRE_EQUAL(DT.getYear(),2009);
  BOOST_REQUIRE_EQUAL(DT.getMonth(),9);
  BOOST_REQUIRE_EQUAL(DT.getDay(),9);
  BOOST_REQUIRE_EQUAL(DT.getHour(),16);
  BOOST_REQUIRE_EQUAL(DT.getMinute(),36);
  BOOST_REQUIRE_EQUAL(DT.getSecond(),25);

  DTStr = DT.getAsISOString();
  BOOST_REQUIRE_EQUAL(DTStr,"2009-09-09 16:36:25");


  BOOST_REQUIRE_EQUAL(DT.setFromString("2013xx06xxx25","%Yxx%mxxx%d"),true);
  BOOST_REQUIRE_EQUAL(DT.getYear(),2013);
  BOOST_REQUIRE_EQUAL(DT.getMonth(),6);
  BOOST_REQUIRE_EQUAL(DT.getDay(),25);
  BOOST_REQUIRE_EQUAL(DT.getHour(),0);
  BOOST_REQUIRE_EQUAL(DT.getMinute(),0);
  BOOST_REQUIRE_EQUAL(DT.getSecond(),0);

  BOOST_REQUIRE_EQUAL(DT.setFromString("2013-08-06T17:37:53","%Y-%m-%dT%H:%M:%S"),true);
  BOOST_REQUIRE_EQUAL(DT.getYear(),2013);
  BOOST_REQUIRE_EQUAL(DT.getMonth(),8);
  BOOST_REQUIRE_EQUAL(DT.getDay(),6);
  BOOST_REQUIRE_EQUAL(DT.getHour(),17);
  BOOST_REQUIRE_EQUAL(DT.getMinute(),37);
  BOOST_REQUIRE_EQUAL(DT.getSecond(),53);

  BOOST_REQUIRE_EQUAL(DT.setFromString("25/06/2012 12h35m06s","%d/%m/%Y %Hh%Mm%Ss"),true);
  BOOST_REQUIRE_EQUAL(DT.getYear(),2012);
  BOOST_REQUIRE_EQUAL(DT.getMonth(),6);
  BOOST_REQUIRE_EQUAL(DT.getDay(),25);
  BOOST_REQUIRE_EQUAL(DT.getHour(),12);
  BOOST_REQUIRE_EQUAL(DT.getMinute(),35);
  BOOST_REQUIRE_EQUAL(DT.getSecond(),6);

  BOOST_REQUIRE_EQUAL(DT.setFromString("2013-06-25","%d %Hh%Mm%Ss"),false);
}
// =====================================================================
// =====================================================================
