/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


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
#include "openfluid-core.h"



BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::Unit TU("Test",25,3);

  BOOST_REQUIRE_EQUAL(TU.getClass(),"Test");
  BOOST_REQUIRE_EQUAL(TU.getID(),25);
  BOOST_REQUIRE_EQUAL(TU.getProcessOrder(),3);
  BOOST_REQUIRE(TU.getFromUnits("TestFrom") == NULL);
  BOOST_REQUIRE(TU.getToUnits("TestTo") == NULL);
  BOOST_REQUIRE_EQUAL(TU.getEvents()->getCount(),0);
  BOOST_REQUIRE_EQUAL(TU.getInputData()->isDataExist("testidata"),false);
  BOOST_REQUIRE_EQUAL(TU.getScalarVariables()->isVariableExist("testsvar"),false);
  BOOST_REQUIRE_EQUAL(TU.getVectorVariables()->isVariableExist("testvvar"),false);


  openfluid::core::Unit* pTU = NULL;

  pTU = new openfluid::core::Unit("pTest",25,3);

  BOOST_REQUIRE_EQUAL(pTU->getClass(),"pTest");
  BOOST_REQUIRE_EQUAL(pTU->getID(),25);
  BOOST_REQUIRE_EQUAL(pTU->getProcessOrder(),3);
  BOOST_REQUIRE(pTU->getFromUnits("pTestFrom") == NULL);
  BOOST_REQUIRE(pTU->getToUnits("pTestTo") == NULL);
  BOOST_REQUIRE_EQUAL(pTU->getEvents()->getCount(),0);
  BOOST_REQUIRE_EQUAL(pTU->getInputData()->isDataExist("testidata"),false);
  BOOST_REQUIRE_EQUAL(pTU->getScalarVariables()->isVariableExist("testsvar"),false);
  BOOST_REQUIRE_EQUAL(pTU->getVectorVariables()->isVariableExist("testvvar"),false);

  delete pTU;

}



// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_from_to)
{

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




  BOOST_REQUIRE(TU.getFromUnits("FromTest") != NULL);
  BOOST_REQUIRE(TU.getFromUnits("FromTest2") != NULL);
  BOOST_REQUIRE(TU.getFromUnits("FromTest3") == NULL);
  BOOST_REQUIRE(TU.getToUnits("FromTest") == NULL);

  BOOST_REQUIRE(TU.getToUnits("ToTest") != NULL);
  BOOST_REQUIRE(TU.getToUnits("ToTest2") != NULL);
  BOOST_REQUIRE(TU.getToUnits("ToTest3") == NULL);
  BOOST_REQUIRE(TU.getFromUnits("ToTest") == NULL);

  BOOST_REQUIRE_EQUAL(TU.getFromUnits("FromTest")->size(),3);
  BOOST_REQUIRE_EQUAL(TU.getFromUnits("FromTest2")->size(),2);

  BOOST_REQUIRE_EQUAL(TU.getFromUnits("FromTest")->front()->getID(),23);
  BOOST_REQUIRE_EQUAL(TU.getFromUnits("FromTest")->back()->getID(),21);

  BOOST_REQUIRE_EQUAL(TU.getToUnits("ToTest")->size(),2);
  BOOST_REQUIRE_EQUAL(TU.getToUnits("ToTest2")->size(),2);

  BOOST_REQUIRE_EQUAL(TU.getToUnits("ToTest")->front()->getID(),1);
  BOOST_REQUIRE_EQUAL(TU.getToUnits("ToTest")->back()->getID(),2);

}



