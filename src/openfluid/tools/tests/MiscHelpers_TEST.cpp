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
  @file MiscHelpers_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_mischelpers


#include <boost/test/unit_test.hpp>

#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/global.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{

  BOOST_REQUIRE_EQUAL(openfluid::tools::matchWithWildcard("*","foobar"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::matchWithWildcard("*foobar","foobar"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::matchWithWildcard("?foobar","foobar"),false);
  BOOST_REQUIRE_EQUAL(openfluid::tools::matchWithWildcard("*bar","foobar"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::matchWithWildcard("foo*","foobar"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::matchWithWildcard("foo*","fobar"),false);
  BOOST_REQUIRE_EQUAL(openfluid::tools::matchWithWildcard("foo?","foobar"),false);
  BOOST_REQUIRE_EQUAL(openfluid::tools::matchWithWildcard("foo???","foobar"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::matchWithWildcard("foo*foo","foobarfoo"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::matchWithWildcard("foo*foo","foofoo"),true);
  BOOST_REQUIRE_EQUAL(openfluid::tools::matchWithWildcard("foo*foo","foobarfo"),false);
  BOOST_REQUIRE_EQUAL(openfluid::tools::matchWithWildcard("foo?foo","foobarfoo"),false);
  BOOST_REQUIRE_EQUAL(openfluid::tools::matchWithWildcard("foo???foo","foobarfoo"),true);


  // ---------------------------------------------------------------------


  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.6.1","1.6.1"),0);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.6.1","1.6.1",false),0);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.6.1~alpha8","1.6.1"),-1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.6.1~alpha8","1.6.1~alpha1"),1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.6.1~alpha8","1.6.1~alpha1", false),0);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.6.1~alpha8","1.6.1~rc1"),-1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.6.1~alpha8","1.6.1~rc1",false),0);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.6.1~rc1","1.6.1~alpha8"),1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.6.1~rc1","1.6.1~alpha8",false),0);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.6.11","1.6.3"),1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.0.0","1.6.3"),-1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.7.0","1.6.3"),1);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.a.0","1.6.3"),-2);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.6.1~rc1","1.6.1~RC1"),0);
  BOOST_REQUIRE_EQUAL(openfluid::tools::compareVersions("1.6.1~rc1","1.6.1+18"),-2);


  // ---------------------------------------------------------------------


  std::cout << openfluid::tools::getNowAsString("%Y%m%dT%H%M%S") << std::endl;
  std::cout << openfluid::tools::getNowAsString("%Y-%m-%d %H:%M") << std::endl;


  // ---------------------------------------------------------------------


  for (int i =0; i<10; i++)
  {
    std::cout << "10 chars: " << openfluid::tools::generatePseudoUniqueIdentifier(10) << std::endl;
  }

  for (int i =0; i<8; i++)
  {
    std::cout << (2*(i+1)) << " chars: " << openfluid::tools::generatePseudoUniqueIdentifier(2*(i+1)) << std::endl;
  }


  // ---------------------------------------------------------------------


  BOOST_REQUIRE_EQUAL(openfluid::tools::escapePattern("txt"),"txt");
  BOOST_REQUIRE_EQUAL(openfluid::tools::escapePattern(".md"),"\\.md");
  BOOST_REQUIRE_EQUAL(openfluid::tools::escapePattern("_ofware20111-sim.dll"),"_ofware20111\\-sim\\.dll");
  BOOST_REQUIRE_EQUAL(openfluid::tools::escapePattern("_ofware20200-obs.so"),"_ofware20200\\-obs\\.so");
  BOOST_REQUIRE_EQUAL(openfluid::tools::escapePattern("+.?{}[]\\\\-_"),"\\+\\.\\?\\{\\}\\[\\]\\\\\\-_");
}

