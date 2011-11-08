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
#include <openfluid/tools.hpp>
#include <openfluid/debug.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  std::string Str;
  bool BoolValue;
  unsigned int UIntValue;
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

  Str = "12a";
  BOOST_REQUIRE_EQUAL(openfluid::tools::ConvertString(Str,&UIntValue),false);

  Str = "1a2";
  BOOST_REQUIRE_EQUAL(openfluid::tools::ConvertString(Str,&UIntValue),false);


  Str = "a11";
  BOOST_REQUIRE_EQUAL(openfluid::tools::ConvertString(Str,&UIntValue),false);


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


  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.6.1","1.6.1"),0);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.6.1","1.6.1",false),0);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.6.1~alpha8","1.6.1"),-1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.6.1~alpha8","1.6.1~alpha1"),1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.6.1~alpha8","1.6.1~alpha1", false),0);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.6.1~alpha8","1.6.1~rc1"),-1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.6.1~alpha8","1.6.1~rc1",false),0);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.6.1~rc1","1.6.1~alpha8"),1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.6.1~rc1","1.6.1~alpha8",false),0);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.6.11","1.6.3"),1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.0.0","1.6.3"),-1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.7.0","1.6.3"),1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.a.0","1.6.3"),-2);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.6.1~rc1","1.6.1~RC1"),0);
  BOOST_REQUIRE_EQUAL(openfluid::tools::CompareVersions("1.6.1~rc1","1.6.1+18"),-2);

}


// =====================================================================
// =====================================================================

