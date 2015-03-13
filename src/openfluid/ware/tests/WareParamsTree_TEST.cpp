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
  @file WareParamsTree_TEST.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_wareparamstree
#include <boost/test/unit_test.hpp>


#include <openfluid/ware/WareParamsTree.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_constructor)
{
  openfluid::ware::WareParamsTree WPT;
  BOOST_REQUIRE_EQUAL(WPT.root().size(),1);

  openfluid::ware::WareParams_t Params;

  openfluid::ware::WareParamsTree WPTEmpty(Params);
  BOOST_REQUIRE_EQUAL(WPTEmpty.root().size(),1);

  Params["test.a.p1"] = openfluid::core::StringValue("ap1");
  Params["test.a.p2"] = openfluid::core::StringValue("ap2");
  Params["test.b.p1"] = openfluid::core::StringValue("bp1");
  Params["test.b.p2"] = openfluid::core::StringValue("bp2");
  Params["test.c.p1"] = openfluid::core::StringValue("cp1");

  openfluid::ware::WareParamsTree WPTInit(Params);
  WPTInit.root().writeToStreamHierarchically(std::cout);
  WPTInit.root().writeToStreamFlat(std::cout);
  BOOST_REQUIRE_EQUAL(WPTInit.root().size(),10);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::ware::WareParams_t Params;

  Params["test.a.p1"] = openfluid::core::StringValue("ap1");
  Params["test.a.p2"] = openfluid::core::StringValue("ap2");
  Params["test.b.p1"] = openfluid::core::StringValue("bp1");
  Params["test.b.p2"] = openfluid::core::StringValue("bp2");
  Params["test.c.p1"] = openfluid::core::StringValue("cp1");
  Params["another.test.a.p1"] = openfluid::core::StringValue("atap1");
  Params["another.test.b.p1"] = openfluid::core::StringValue("atbp1");
  Params["another.test.b.p2"] = openfluid::core::StringValue("atbp2");

  std::cout << "----------------" << std::endl;

  openfluid::ware::WareParamsTree WPT;
  WPT.setParams(Params);
  WPT.root().writeToStreamHierarchically(std::cout);
  std::cout << std::endl;
  BOOST_REQUIRE_EQUAL(WPT.root().size(),17);


  std::cout << "----------------" << std::endl;

  BOOST_REQUIRE_EQUAL(WPT.getValueUsingFullKey("another.test.b.p1").get(),"atbp1");
  BOOST_CHECK_THROW(WPT.getValueUsingFullKey("this.does.not.exist"),openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(WPT.getValueUsingFullKey("another.test.does.not.exist"),openfluid::base::FrameworkException);
  BOOST_REQUIRE_EQUAL(WPT.getValueUsingFullKey("this.does.not.exist.either","yes!").get(),"yes!");
}




