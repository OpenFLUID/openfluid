/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


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
