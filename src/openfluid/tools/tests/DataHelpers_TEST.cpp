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
  @file DataHelpers_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_datahelpers
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/tools/DataHelpers.hpp>


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
  BOOST_REQUIRE_EQUAL(openfluid::tools::convertString(Str,&DoubleValue),true);
  BOOST_REQUIRE_CLOSE(openfluid::tools::convertString<double>(Str),0.01,0.0001);


  Str = "abcd";
  BOOST_REQUIRE_EQUAL(openfluid::tools::convertString(Str,&BoolValue),false);
  BOOST_REQUIRE_THROW(bool Test = openfluid::tools::convertString<bool>(Str),openfluid::base::FrameworkException);

  Str = "1";
  BOOST_REQUIRE_EQUAL(openfluid::tools::convertString(Str,&BoolValue),true);

  Str = "12a";
  BOOST_REQUIRE_EQUAL(openfluid::tools::convertString(Str,&UIntValue),false);

  Str = "1a2";
  BOOST_REQUIRE_EQUAL(openfluid::tools::convertString(Str,&UIntValue),false);


  Str = "a11";
  BOOST_REQUIRE_EQUAL(openfluid::tools::convertString(Str,&UIntValue),false);


  BOOST_REQUIRE_EQUAL(1,true);
  BOOST_REQUIRE_EQUAL(0,false);

  DoubleValue = 0.25;
  BOOST_REQUIRE_EQUAL(openfluid::tools::convertValue(DoubleValue,&Str),true);
  BOOST_REQUIRE_EQUAL(Str,"0.25");

  BOOST_REQUIRE_NO_THROW(Str = openfluid::tools::convertValue(DoubleValue));
  BOOST_REQUIRE_EQUAL(Str,"0.25");

  Str = "aaa;bbbb;ccccc";
  openfluid::tools::tokenizeString(Str,StrArray,";");
  BOOST_REQUIRE_EQUAL(StrArray.size(),3);
  BOOST_REQUIRE_EQUAL(StrArray[0],"aaa");
  BOOST_REQUIRE_EQUAL(StrArray[1],"bbbb");
  BOOST_REQUIRE_EQUAL(StrArray[2],"ccccc");
  openfluid::tools::tokenizeString(Str,StrArray,"-");
  BOOST_REQUIRE_EQUAL(StrArray.size(),1);

}


// =====================================================================
// =====================================================================

