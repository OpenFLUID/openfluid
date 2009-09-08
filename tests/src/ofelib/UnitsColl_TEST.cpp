/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file UnitsColl_TEST.cpp
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
  openfluid::core::UnitsCollection* pUC = NULL;

  pUC = new openfluid::core::UnitsCollection();

  BOOST_REQUIRE_EQUAL(pUC->getList()->size(),0);
  BOOST_REQUIRE(pUC->getUnit(1) == NULL);

  delete pUC;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_populate)
{
  openfluid::core::UnitsCollection* pUC = NULL;

  pUC = new openfluid::core::UnitsCollection();

  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",1,1)),true);
  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",2,1)),true);
  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",5,1)),true);
  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",4,2)),true);
  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",17,1)),true);
  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",4,3)),false);
  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",17,3)),false);


  BOOST_REQUIRE_EQUAL(pUC->getList()->size(),5);

  BOOST_REQUIRE(pUC->getUnit(1) != NULL);
  BOOST_REQUIRE(pUC->getUnit(17) != NULL);
  BOOST_REQUIRE(pUC->getUnit(9) == NULL);

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

  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",1,1)),true);
  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",2,5)),true);
  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",5,2)),true);
  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",4,7)),true);
  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",17,3)),true);
  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",13,1)),true);
  BOOST_REQUIRE_EQUAL(pUC->addUnit(openfluid::core::Unit("Test",15,2)),true);

  pUC->sortByProcessOrder();


  LastOrd = 0;
  for (it=pUC->getList()->begin();it!=pUC->getList()->end();++it)
  {
    BOOST_REQUIRE_GE(it->getProcessOrder(),LastOrd);
    LastOrd = it->getProcessOrder();
  }


  delete pUC;
}

