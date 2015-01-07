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
  \file UnitsColl_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_unitscoll
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/core/UnitsCollection.hpp>
#include <openfluid/core/SpatialUnit.hpp>


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::UnitsCollection* pUC = NULL;

  pUC = new openfluid::core::UnitsCollection();

  BOOST_REQUIRE_EQUAL(pUC->list()->size(),0);
  BOOST_REQUIRE(pUC->spatialUnit(1) == NULL);

  delete pUC;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_populate)
{
  openfluid::core::UnitsCollection* pUC = NULL;

  pUC = new openfluid::core::UnitsCollection();

  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",1,1)) != NULL);
  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",2,1)) != NULL);
  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",5,1)) != NULL);
  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",4,2)) != NULL);
  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",17,1)) != NULL);
  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",4,3)) == NULL);
  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",17,3)) == NULL);


  BOOST_REQUIRE_EQUAL(pUC->list()->size(),5);

  BOOST_REQUIRE(pUC->spatialUnit(1) != NULL);
  BOOST_REQUIRE(pUC->spatialUnit(17) != NULL);
  BOOST_REQUIRE(pUC->spatialUnit(9) == NULL);

  delete pUC;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_sortbypcsorder)
{
  openfluid::core::UnitsCollection* pUC = NULL;
  openfluid::core::PcsOrd_t LastOrd;
  openfluid::core::UnitsList_t::iterator it;

  pUC = new openfluid::core::UnitsCollection();

  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",1,1)) != NULL);
  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",2,5)) != NULL);
  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",5,2)) != NULL);
  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",4,7)) != NULL);
  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",17,3)) != NULL);
  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",13,1)) != NULL);
  BOOST_REQUIRE(pUC->addSpatialUnit(openfluid::core::SpatialUnit("Test",15,2)) != NULL);

  pUC->sortByProcessOrder();


  LastOrd = 0;
  for (it=pUC->list()->begin();it!=pUC->list()->end();++it)
  {
    BOOST_REQUIRE_GE(it->getProcessOrder(),LastOrd);
    LastOrd = it->getProcessOrder();
  }


  delete pUC;
}

