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
  \file MarketClient_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_distributionbindings
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <openfluid/tools/DistributionBindings.hpp>


#include <iostream>

#include <tests-config.hpp>

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

  DistriTables.build(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.Generators").string(),
                                                      "sourcesinject.xml","distri.dat");

  BOOST_REQUIRE_EQUAL(DistriTables.SourcesTable["1"],boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.Generators/source3.dat").string());
  BOOST_REQUIRE_EQUAL(DistriTables.SourcesTable["2"],boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.Generators/source4.dat").string());

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



