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
  \file Unit_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_unit
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/core/SpatialUnit.hpp>



BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::SpatialUnit TU("Test",25,3);

  BOOST_REQUIRE_EQUAL(TU.getClass(),"Test");
  BOOST_REQUIRE_EQUAL(TU.getID(),25);
  BOOST_REQUIRE_EQUAL(TU.getProcessOrder(),3);
  BOOST_REQUIRE(TU.fromSpatialUnits("TestFrom") == NULL);
  BOOST_REQUIRE(TU.toSpatialUnits("TestTo") == NULL);
  BOOST_REQUIRE(TU.parentSpatialUnits("TestParent") == NULL);
  BOOST_REQUIRE(TU.childSpatialUnits("TestChildren") == NULL);
  BOOST_REQUIRE_EQUAL(TU.events()->getCount(),0);
  BOOST_REQUIRE_EQUAL(TU.attributes()->isAttributeExist("testattr"),false);
  BOOST_REQUIRE_EQUAL(TU.variables()->isVariableExist("testvar"),false);


  openfluid::core::SpatialUnit* pTU = NULL;

  pTU = new openfluid::core::SpatialUnit("pTest",25,3);

  BOOST_REQUIRE_EQUAL(pTU->getClass(),"pTest");
  BOOST_REQUIRE_EQUAL(pTU->getID(),25);
  BOOST_REQUIRE_EQUAL(pTU->getProcessOrder(),3);
  BOOST_REQUIRE(pTU->fromSpatialUnits("pTestFrom") == NULL);
  BOOST_REQUIRE(pTU->toSpatialUnits("pTestTo") == NULL);
  BOOST_REQUIRE(pTU->parentSpatialUnits("pTestParent") == NULL);
  BOOST_REQUIRE(pTU->childSpatialUnits("pTestChildren") == NULL);
  BOOST_REQUIRE_EQUAL(pTU->events()->getCount(),0);
  BOOST_REQUIRE_EQUAL(pTU->attributes()->isAttributeExist("testattr"),false);
  BOOST_REQUIRE_EQUAL(pTU->variables()->isVariableExist("testvar"),false);

  delete pTU;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_from_to)
{
  const openfluid::core::UnitsPtrList_t* FromUnits;
  const openfluid::core::UnitsPtrList_t* ToUnits;
  openfluid::core::UnitsPtrList_t::const_iterator itUnits;
  unsigned int UnitsCount;


  openfluid::core::SpatialUnit TU("Test",35,17);

  TU.addFromUnit(new openfluid::core::SpatialUnit("FromTest",23,1));
  TU.addFromUnit(new openfluid::core::SpatialUnit("FromTest",2,5));
  TU.addFromUnit(new openfluid::core::SpatialUnit("FromTest",21,5));

  TU.addFromUnit(new openfluid::core::SpatialUnit("FromTest2",1,1));
  TU.addFromUnit(new openfluid::core::SpatialUnit("FromTest2",2,1));


  TU.addToUnit(new openfluid::core::SpatialUnit("ToTest",1,1));
  TU.addToUnit(new openfluid::core::SpatialUnit("ToTest",2,1));

  TU.addToUnit(new openfluid::core::SpatialUnit("ToTest2",1,1));
  TU.addToUnit(new openfluid::core::SpatialUnit("ToTest2",2,1));

  TU.addParentUnit(new openfluid::core::SpatialUnit("ParentTest",55,1));

  TU.addChildUnit(new openfluid::core::SpatialUnit("ChildrenTest",1,1));
  TU.addChildUnit(new openfluid::core::SpatialUnit("ChildrenTest",2,1));

  BOOST_REQUIRE(TU.fromSpatialUnits("FromTest") != NULL);
  BOOST_REQUIRE(TU.fromSpatialUnits("FromTest2") != NULL);
  BOOST_REQUIRE(TU.fromSpatialUnits("FromTest3") == NULL);
  BOOST_REQUIRE(TU.toSpatialUnits("FromTest") == NULL);

  BOOST_REQUIRE(TU.toSpatialUnits("ToTest") != NULL);
  BOOST_REQUIRE(TU.toSpatialUnits("ToTest2") != NULL);
  BOOST_REQUIRE(TU.toSpatialUnits("ToTest3") == NULL);
  BOOST_REQUIRE(TU.fromSpatialUnits("ToTest") == NULL);

  BOOST_REQUIRE(TU.parentSpatialUnits("ParentTest") != NULL);
  BOOST_REQUIRE(TU.childSpatialUnits("ChildrenTest") != NULL);
  BOOST_REQUIRE(TU.parentSpatialUnits("ToTest3") == NULL);
  BOOST_REQUIRE(TU.childSpatialUnits("ToTest") == NULL);


  BOOST_REQUIRE_EQUAL(TU.fromSpatialUnits("FromTest")->size(),3);
  BOOST_REQUIRE_EQUAL(TU.fromSpatialUnits("FromTest2")->size(),2);

  BOOST_REQUIRE_EQUAL(TU.fromSpatialUnits("FromTest")->front()->getID(),23);
  BOOST_REQUIRE_EQUAL(TU.fromSpatialUnits("FromTest")->back()->getID(),21);

  FromUnits = TU.fromSpatialUnits("FromTest");
  UnitsCount = 0;
  for (itUnits=FromUnits->begin();itUnits!=FromUnits->end();++itUnits)
  {
    BOOST_REQUIRE_EQUAL((*itUnits)->getClass(),"FromTest");
    BOOST_REQUIRE((*itUnits)->fromSpatialUnits("FakeFrom") == NULL);
    UnitsCount++;
  }
  BOOST_REQUIRE_EQUAL(UnitsCount,3);


  BOOST_REQUIRE_EQUAL(TU.toSpatialUnits("ToTest")->size(),2);
  BOOST_REQUIRE_EQUAL(TU.toSpatialUnits("ToTest2")->size(),2);

  BOOST_REQUIRE_EQUAL(TU.toSpatialUnits("ToTest")->front()->getID(),1);
  BOOST_REQUIRE_EQUAL(TU.toSpatialUnits("ToTest")->back()->getID(),2);

  ToUnits = TU.toSpatialUnits("ToTest");
  UnitsCount = 0;
  for (itUnits=ToUnits->begin();itUnits!=ToUnits->end();++itUnits)
  {
    BOOST_REQUIRE_EQUAL((*itUnits)->getClass(),"ToTest");
    BOOST_REQUIRE((*itUnits)->toSpatialUnits("FakeTo") == NULL);
    UnitsCount++;
  }
  BOOST_REQUIRE_EQUAL(UnitsCount,2);


  BOOST_REQUIRE_EQUAL(TU.parentSpatialUnits("ParentTest")->front()->getID(),55);

  BOOST_REQUIRE_EQUAL(TU.childSpatialUnits("ChildrenTest")->front()->getID(),1);
  BOOST_REQUIRE_EQUAL(TU.childSpatialUnits("ChildrenTest")->back()->getID(),2);


}



