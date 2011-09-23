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
  \file InputData_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_inputdata
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <openfluid/core.hpp>
#include <vector>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::InputData IData;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::InputData IData;
  double Value;
  std::vector<openfluid::core::InputDataName_t> Names;

  BOOST_REQUIRE_EQUAL(IData.setValue("idata_1",openfluid::core::DoubleValue(2.0)),true);
  BOOST_REQUIRE_EQUAL(IData.setValue("idata_2",openfluid::core::DoubleValue(3.2)),true);
  BOOST_REQUIRE_EQUAL(IData.setValue("idata_3",openfluid::core::DoubleValue(4.3)),true);
  BOOST_REQUIRE_EQUAL(IData.setValue("idata_3",openfluid::core::DoubleValue(2.0)),false);

  BOOST_REQUIRE_EQUAL(IData.getValueAsDouble("idata_1",&Value),true);
  BOOST_REQUIRE_CLOSE(Value,2.0,0.001);

  BOOST_REQUIRE_EQUAL(IData.getValueAsDouble("idata_2",&Value),true);
  BOOST_REQUIRE_CLOSE(Value,3.2,0.001);

  BOOST_REQUIRE_EQUAL(IData.getValueAsDouble("idata_3",&Value),true);
  BOOST_REQUIRE_CLOSE(Value,4.3,0.001);

  BOOST_REQUIRE_EQUAL(IData.getValueAsDouble("idata_4",&Value),false);

  Names = IData.getInputDataNames();
  BOOST_REQUIRE_EQUAL(Names.size(),3);



  openfluid::core::InputData IDataStr;
  std::string IDValue;
  double DoubleValue;

  BOOST_REQUIRE_EQUAL(IDataStr.setValue("idata_1","CODEA"),true);
  BOOST_REQUIRE_EQUAL(IDataStr.setValue("idata_3","3.5"),true);
  BOOST_REQUIRE_EQUAL(IDataStr.setValue("idata_3","CODEC"),false);

  BOOST_REQUIRE_EQUAL(IDataStr.getValue("idata_1",&IDValue),true);
  BOOST_REQUIRE_EQUAL(IDValue,"CODEA");

  BOOST_REQUIRE_EQUAL(IDataStr.getValue("idata_2",&IDValue),false);

  BOOST_REQUIRE_EQUAL(IDataStr.getValue("idata_3",&IDValue),true);
  BOOST_REQUIRE_EQUAL(IDValue,"3.5");
  BOOST_REQUIRE_EQUAL(IDataStr.getValueAsDouble("idata_3",&DoubleValue),true);
  BOOST_REQUIRE_CLOSE(DoubleValue,3.5,0.001);

  BOOST_REQUIRE_EQUAL(IDataStr.getValue("idata_4",&IDValue),false);

  Names = IDataStr.getInputDataNames();
  BOOST_REQUIRE_EQUAL(Names.size(),2);

}
