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
  @file ExceptionContext_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_exceptioncontext
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/base/ExceptionContext.hpp>
#include <iostream>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::ExceptionContext Ctxt;

  BOOST_REQUIRE(Ctxt.empty());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::base::ExceptionContext Ctxt;
  BOOST_REQUIRE_EQUAL(Ctxt.size(),0);

  Ctxt.addInfos({{"codeloc","here"},{"source","test"}});
  BOOST_REQUIRE_EQUAL(Ctxt.size(),2);

  Ctxt.addInfos({{"key1","k1"},{"key2","k2"}}).addInfos({{"key3","k3"}});
  BOOST_REQUIRE_EQUAL(Ctxt.size(),5);

  std::cout << "Contents: " << Ctxt.toString() << std::endl;

  Ctxt.clear();
  Ctxt.addStage("INITIALIZERUN").addSpatialUnit("TU#147").addTimeIndex(123).addTimeIndex(155);
  std::cout << "Contents: " << Ctxt.toString() << std::endl;

}
