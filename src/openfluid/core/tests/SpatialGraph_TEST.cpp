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
  @file SpatialGraph_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_spatialgraph


#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/core/SpatialGraph.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::SpatialGraph* SGraph;

  SGraph = new openfluid::core::SpatialGraph();

  delete SGraph;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::SpatialGraph* SGraph;
  int i, PcsOrder;
  openfluid::core::UnitsCollection* UnitsColl;
  openfluid::core::UnitsList_t::iterator UnitsIt, PrevUnitsIt;
  openfluid::core::SpatialUnit* U;


  SGraph = new openfluid::core::SpatialGraph();

  for (i=1;i<=250;i++)
  {
    PcsOrder = (i%7)+1;
    SGraph->addUnit(openfluid::core::SpatialUnit("UnitClassA",i,PcsOrder));
  }

  for (i=1;i<=7325;i++)
  {
    PcsOrder = (i%31)+1;
    SGraph->addUnit(openfluid::core::SpatialUnit("UnitClassB",i,PcsOrder));
  }

  SGraph->sortUnitsByProcessOrder();

  // *** Units count and process order
  UnitsColl = SGraph->spatialUnits("UnitClassA");
  BOOST_REQUIRE_EQUAL(UnitsColl->list()->size(),250);

  PrevUnitsIt = UnitsColl->list()->begin();
  for (UnitsIt = UnitsColl->list()->begin(); UnitsIt != UnitsColl->list()->end();++UnitsIt)
  {

    if (UnitsIt != UnitsColl->list()->begin())
    {
      BOOST_REQUIRE_GE(UnitsIt->getProcessOrder(),PrevUnitsIt->getProcessOrder());
    }
    PrevUnitsIt = UnitsIt;
  }

  UnitsColl = SGraph->spatialUnits("UnitClassB");
  BOOST_REQUIRE_EQUAL(UnitsColl->list()->size(),7325);

  PrevUnitsIt = UnitsColl->list()->begin();
  for (UnitsIt = UnitsColl->list()->begin(); UnitsIt != UnitsColl->list()->end();++UnitsIt)
  {

    if (UnitsIt != UnitsColl->list()->begin())
    {
      BOOST_REQUIRE_GE(UnitsIt->getProcessOrder(),PrevUnitsIt->getProcessOrder());
    }
    PrevUnitsIt = UnitsIt;
  }


  // *** existing classes
  BOOST_REQUIRE_EQUAL(SGraph->isUnitsClassExist("UnitClassA"),true);
  BOOST_REQUIRE_EQUAL(SGraph->isUnitsClassExist("UnitClassB"),true);
  BOOST_REQUIRE_EQUAL(SGraph->isUnitsClassExist("WrongClass"),false);


  // *** existing units
  BOOST_REQUIRE(SGraph->spatialUnit("UnitClassA",17) != nullptr);
  BOOST_REQUIRE(SGraph->spatialUnit("UnitClassA",1000) == nullptr);
  U = SGraph->spatialUnit("UnitClassA",17);
  BOOST_REQUIRE_EQUAL(U->getClass(),"UnitClassA");
  BOOST_REQUIRE_EQUAL(U->getID(),17);

  BOOST_REQUIRE(SGraph->spatialUnit("UnitClassB",1333) != nullptr);
  BOOST_REQUIRE(SGraph->spatialUnit("UnitClassB",10000) == nullptr);
  U = SGraph->spatialUnit("UnitClassB",1333);
  BOOST_REQUIRE_EQUAL(U->getClass(),"UnitClassB");
  BOOST_REQUIRE_EQUAL(U->getID(),1333);

  BOOST_REQUIRE(SGraph->spatialUnit("WrongClass",1) == nullptr);


  SGraph->clearUnits();
  BOOST_REQUIRE_EQUAL(SGraph->spatialUnits("UnitClassA")->list()->size(),0);
  BOOST_REQUIRE_EQUAL(SGraph->spatialUnits("UnitClassB")->list()->size(),0);

  delete SGraph;
}

