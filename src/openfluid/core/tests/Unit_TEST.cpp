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
#include <openfluid/core/Unit.hpp>



BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::Unit TU("Test",25,3);

  BOOST_REQUIRE_EQUAL(TU.getClass(),"Test");
  BOOST_REQUIRE_EQUAL(TU.getID(),25);
  BOOST_REQUIRE_EQUAL(TU.getProcessOrder(),3);
  BOOST_REQUIRE(TU.getFromUnits("TestFrom") == NULL);
  BOOST_REQUIRE(TU.getToUnits("TestTo") == NULL);
  BOOST_REQUIRE(TU.getParentUnits("TestParent") == NULL);
  BOOST_REQUIRE(TU.getChildrenUnits("TestChildren") == NULL);
  BOOST_REQUIRE_EQUAL(TU.getEvents()->getCount(),0);
  BOOST_REQUIRE_EQUAL(TU.getAttributes()->isAttributeExist("testattr"),false);
  BOOST_REQUIRE_EQUAL(TU.getVariables()->isVariableExist("testvar"),false);


  openfluid::core::Unit* pTU = NULL;

  pTU = new openfluid::core::Unit("pTest",25,3);

  BOOST_REQUIRE_EQUAL(pTU->getClass(),"pTest");
  BOOST_REQUIRE_EQUAL(pTU->getID(),25);
  BOOST_REQUIRE_EQUAL(pTU->getProcessOrder(),3);
  BOOST_REQUIRE(pTU->getFromUnits("pTestFrom") == NULL);
  BOOST_REQUIRE(pTU->getToUnits("pTestTo") == NULL);
  BOOST_REQUIRE(pTU->getParentUnits("pTestParent") == NULL);
  BOOST_REQUIRE(pTU->getChildrenUnits("pTestChildren") == NULL);
  BOOST_REQUIRE_EQUAL(pTU->getEvents()->getCount(),0);
  BOOST_REQUIRE_EQUAL(pTU->getAttributes()->isAttributeExist("testattr"),false);
  BOOST_REQUIRE_EQUAL(pTU->getVariables()->isVariableExist("testvar"),false);

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


  openfluid::core::Unit TU("Test",35,17);

  TU.addFromUnit(new openfluid::core::Unit("FromTest",23,1));
  TU.addFromUnit(new openfluid::core::Unit("FromTest",2,5));
  TU.addFromUnit(new openfluid::core::Unit("FromTest",21,5));

  TU.addFromUnit(new openfluid::core::Unit("FromTest2",1,1));
  TU.addFromUnit(new openfluid::core::Unit("FromTest2",2,1));


  TU.addToUnit(new openfluid::core::Unit("ToTest",1,1));
  TU.addToUnit(new openfluid::core::Unit("ToTest",2,1));

  TU.addToUnit(new openfluid::core::Unit("ToTest2",1,1));
  TU.addToUnit(new openfluid::core::Unit("ToTest2",2,1));

  TU.addParentUnit(new openfluid::core::Unit("ParentTest",55,1));

  TU.addChildUnit(new openfluid::core::Unit("ChildrenTest",1,1));
  TU.addChildUnit(new openfluid::core::Unit("ChildrenTest",2,1));

  BOOST_REQUIRE(TU.getFromUnits("FromTest") != NULL);
  BOOST_REQUIRE(TU.getFromUnits("FromTest2") != NULL);
  BOOST_REQUIRE(TU.getFromUnits("FromTest3") == NULL);
  BOOST_REQUIRE(TU.getToUnits("FromTest") == NULL);

  BOOST_REQUIRE(TU.getToUnits("ToTest") != NULL);
  BOOST_REQUIRE(TU.getToUnits("ToTest2") != NULL);
  BOOST_REQUIRE(TU.getToUnits("ToTest3") == NULL);
  BOOST_REQUIRE(TU.getFromUnits("ToTest") == NULL);

  BOOST_REQUIRE(TU.getParentUnits("ParentTest") != NULL);
  BOOST_REQUIRE(TU.getChildrenUnits("ChildrenTest") != NULL);
  BOOST_REQUIRE(TU.getParentUnits("ToTest3") == NULL);
  BOOST_REQUIRE(TU.getChildrenUnits("ToTest") == NULL);


  BOOST_REQUIRE_EQUAL(TU.getFromUnits("FromTest")->size(),3);
  BOOST_REQUIRE_EQUAL(TU.getFromUnits("FromTest2")->size(),2);

  BOOST_REQUIRE_EQUAL(TU.getFromUnits("FromTest")->front()->getID(),23);
  BOOST_REQUIRE_EQUAL(TU.getFromUnits("FromTest")->back()->getID(),21);

  FromUnits = TU.getFromUnits("FromTest");
  UnitsCount = 0;
  for (itUnits=FromUnits->begin();itUnits!=FromUnits->end();++itUnits)
  {
    BOOST_REQUIRE_EQUAL((*itUnits)->getClass(),"FromTest");
    BOOST_REQUIRE((*itUnits)->getFromUnits("FakeFrom") == NULL);
    UnitsCount++;
  }
  BOOST_REQUIRE_EQUAL(UnitsCount,3);


  BOOST_REQUIRE_EQUAL(TU.getToUnits("ToTest")->size(),2);
  BOOST_REQUIRE_EQUAL(TU.getToUnits("ToTest2")->size(),2);

  BOOST_REQUIRE_EQUAL(TU.getToUnits("ToTest")->front()->getID(),1);
  BOOST_REQUIRE_EQUAL(TU.getToUnits("ToTest")->back()->getID(),2);

  ToUnits = TU.getToUnits("ToTest");
  UnitsCount = 0;
  for (itUnits=ToUnits->begin();itUnits!=ToUnits->end();++itUnits)
  {
    BOOST_REQUIRE_EQUAL((*itUnits)->getClass(),"ToTest");
    BOOST_REQUIRE((*itUnits)->getToUnits("FakeTo") == NULL);
    UnitsCount++;
  }
  BOOST_REQUIRE_EQUAL(UnitsCount,2);


  BOOST_REQUIRE_EQUAL(TU.getParentUnits("ParentTest")->front()->getID(),55);

  BOOST_REQUIRE_EQUAL(TU.getChildrenUnits("ChildrenTest")->front()->getID(),1);
  BOOST_REQUIRE_EQUAL(TU.getChildrenUnits("ChildrenTest")->back()->getID(),2);


}



