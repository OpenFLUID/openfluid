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
#include <openfluid/core/ValuesBuffer.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::ValuesBuffer<double> VBuffer;

  BOOST_REQUIRE_EQUAL(VBuffer.getNextStep(),0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{

  openfluid::core::ValuesBufferProperties::setBufferSize(5);
  openfluid::core::ValuesBuffer<double> VBuffer;
  double Value;

  BOOST_REQUIRE_EQUAL(VBuffer.getNextStep(),0);

  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(1.1),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(2.2),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(3.3),true);

  // tstep    0   1   2
  // buffer |1.1|2.2|3.3|

  BOOST_REQUIRE_EQUAL(VBuffer.getValue(2,&Value),true);
  BOOST_REQUIRE_CLOSE(Value,3.3,0.001);

  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(4.4),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(5.5),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(6.6),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(7.7),true);

  // tstep    2   3   4   5   6
  // buffer |3.3|4.4|5.5|6.6|7.7|

  BOOST_REQUIRE_EQUAL(VBuffer.getNextStep(),7);

  BOOST_REQUIRE_EQUAL(VBuffer.getCurrentValue(&Value),true);
  BOOST_REQUIRE_CLOSE(Value,7.7,0.001);

  BOOST_REQUIRE_EQUAL(VBuffer.getValue(0,&Value),false);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(7,&Value),false);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(1,&Value),false);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(2,&Value),true);


  BOOST_REQUIRE_EQUAL(VBuffer.getValue(3,&Value),true);
  BOOST_REQUIRE_CLOSE(Value,4.4,0.001);

  BOOST_REQUIRE_EQUAL(VBuffer.modifyValue(3,44.0),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(3,&Value),true);
  BOOST_REQUIRE_CLOSE(Value,44.0,0.001);


/*  BOOST_REQUIRE_EQUAL(VBuffer.deleteValues(3),true);
  BOOST_REQUIRE_EQUAL(VBuffer.getValue(3,&Value),false);
  BOOST_REQUIRE_NE(VBuffer.getValue(3,&Value),true);*/

  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(10.1),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(20.2),true);
  BOOST_REQUIRE_EQUAL(VBuffer.appendValue(30.3),true);

//  BOOST_REQUIRE_EQUAL(VBuffer.deleteValues(8),true);

}

// =====================================================================
// =====================================================================
