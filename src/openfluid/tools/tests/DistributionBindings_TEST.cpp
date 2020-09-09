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
  @file DistributionBindings_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_distributionbindings


#include <iostream>

#include <boost/test/unit_test.hpp>

#include <openfluid/tools/DistributionBindings.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::tools::DistributionTables DistriTables;

  openfluid::tools::DistributionBindings DistriBindings(DistriTables);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{

  openfluid::tools::DistributionTables DistriTables;

  DistriTables.build(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.Generators",
                     "sourcesinject.xml","distri.dat");

  BOOST_REQUIRE_EQUAL(DistriTables.SourcesTable["1"],
                      CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.Generators/source3.dat");
  BOOST_REQUIRE_EQUAL(DistriTables.SourcesTable["2"],
                      CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.Generators/source4.dat");

  BOOST_REQUIRE_EQUAL(DistriTables.UnitsTable[1],"1");
  BOOST_REQUIRE_EQUAL(DistriTables.UnitsTable[2],"2");
  BOOST_REQUIRE_EQUAL(DistriTables.UnitsTable[3],"1");
  BOOST_REQUIRE_EQUAL(DistriTables.UnitsTable[5],"1");
  BOOST_REQUIRE_EQUAL(DistriTables.UnitsTable[4],"2");

  // ===============


  openfluid::tools::DistributionBindings DistriBindings(DistriTables);
  bool ValueFound = false;
  openfluid::core::DoubleValue Value(0);

//  std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  DistriBindings.displayBindings();

//  std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  DistriBindings.advanceToTime(openfluid::core::DateTime(2000,1,1,0,6,0));

//  std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  ValueFound = DistriBindings.getValue(1,openfluid::core::DateTime(2000,1,1,0,6,0),Value);
  BOOST_REQUIRE(ValueFound);
  BOOST_REQUIRE_CLOSE(Value.get(),6.0,0.0001);

  ValueFound = DistriBindings.getValue(2,openfluid::core::DateTime(2000,1,1,0,6,0),Value);
  BOOST_REQUIRE(ValueFound);
  BOOST_REQUIRE_CLOSE(Value.get(),0.0,0.0001);

  ValueFound = DistriBindings.getValue(5,openfluid::core::DateTime(2000,1,1,0,6,0),Value);
  BOOST_REQUIRE(ValueFound);
  BOOST_REQUIRE_CLOSE(Value.get(),6.0,0.0001);

  ValueFound = DistriBindings.getValue(2,openfluid::core::DateTime(2013,1,1,0,6,0),Value);
  BOOST_REQUIRE(!ValueFound);

  openfluid::core::DateTime NextDT;

  BOOST_REQUIRE(DistriBindings.advanceToNextTimeAfter(openfluid::core::DateTime(2000,1,1,0,6,0),NextDT));
  BOOST_REQUIRE(NextDT == openfluid::core::DateTime(2000,1,1,0,7,0));

  ValueFound = DistriBindings.getValue(5,openfluid::core::DateTime(2000,1,1,0,7,0),Value);
  BOOST_REQUIRE(ValueFound);
  BOOST_REQUIRE_CLOSE(Value.get(),7.0,0.0001);

}

