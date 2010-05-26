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
  \file SSerieValues_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_sserievalues
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <openfluid/core.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::StepSerieOfValues<double> SSerie;

  BOOST_REQUIRE_EQUAL(SSerie.getNextStep(),0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::StepSerieOfValues<double> SSerie;
  double Value;

  BOOST_REQUIRE_EQUAL(SSerie.getNextStep(),0);

  BOOST_REQUIRE_EQUAL(SSerie.appendValue(1.1),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(2.2),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(3.3),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(4.4),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(5.5),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(6.6),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(7.7),true);

  BOOST_REQUIRE_EQUAL(SSerie.getNextStep(),7);

  BOOST_REQUIRE_EQUAL(SSerie.getValue(7,&Value),false);

  BOOST_REQUIRE_EQUAL(SSerie.getValue(3,&Value),true);
  BOOST_REQUIRE_CLOSE(Value,4.4,0.001);

  BOOST_REQUIRE_EQUAL(SSerie.modifyValue(3,44.0),true);
  BOOST_REQUIRE_EQUAL(SSerie.getValue(3,&Value),true);
  BOOST_REQUIRE_CLOSE(Value,44.0,0.001);

  BOOST_REQUIRE_EQUAL(SSerie.deleteValues(3),true);
  BOOST_REQUIRE_EQUAL(SSerie.getValue(3,&Value),false);
  BOOST_REQUIRE_NE(SSerie.getValue(3,&Value),true);

  BOOST_REQUIRE_EQUAL(SSerie.appendValue(10.1),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(20.2),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(30.3),true);

  BOOST_REQUIRE_EQUAL(SSerie.deleteValues(8),true);

}

// =====================================================================
// =====================================================================
