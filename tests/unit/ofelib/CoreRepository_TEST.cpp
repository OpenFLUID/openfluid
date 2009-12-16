/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file CoreRepository_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_corerepository
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::CoreRepository* Repos;
  openfluid::core::MemoryMonitor* MemMon;

  Repos = openfluid::core::CoreRepository::getInstance();
  MemMon = openfluid::core::MemoryMonitor::getInstance();
  MemMon->setPacketAndKeep(500,10);
  Repos->setMemoryMonitor(MemMon);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::CoreRepository* Repos;
  openfluid::core::MemoryMonitor* MemMon;
  int i, PcsOrder;
  openfluid::core::UnitsCollection* UnitsColl;
  openfluid::core::UnitsList_t::iterator UnitsIt, PrevUnitsIt;
  openfluid::core::Unit* U;


  Repos = openfluid::core::CoreRepository::getInstance();
  MemMon = openfluid::core::MemoryMonitor::getInstance();
  MemMon->setPacketAndKeep(500,10);
  Repos->setMemoryMonitor(MemMon);

  for (i=1;i<=250;i++)
  {
    PcsOrder = (i%7)+1;
    Repos->addUnit(openfluid::core::Unit("UnitClassA",i,PcsOrder));
  }

  for (i=1;i<=7325;i++)
  {
    PcsOrder = (i%31)+1;
    Repos->addUnit(openfluid::core::Unit("UnitClassB",i,PcsOrder));
  }

  Repos->sortUnitsByProcessOrder();

  // *** Units count and process order
  UnitsColl = Repos->getUnits("UnitClassA");
  BOOST_REQUIRE_EQUAL(UnitsColl->getList()->size(),250);

  PrevUnitsIt = UnitsColl->getList()->begin();
  for (UnitsIt = UnitsColl->getList()->begin(); UnitsIt != UnitsColl->getList()->end();++UnitsIt)
  {

    if (UnitsIt != UnitsColl->getList()->begin())
    {
      BOOST_REQUIRE_GE(UnitsIt->getProcessOrder(),PrevUnitsIt->getProcessOrder());
    }
    PrevUnitsIt = UnitsIt;
  }

  UnitsColl = Repos->getUnits("UnitClassB");
  BOOST_REQUIRE_EQUAL(UnitsColl->getList()->size(),7325);

  PrevUnitsIt = UnitsColl->getList()->begin();
  for (UnitsIt = UnitsColl->getList()->begin(); UnitsIt != UnitsColl->getList()->end();++UnitsIt)
  {

    if (UnitsIt != UnitsColl->getList()->begin())
    {
      BOOST_REQUIRE_GE(UnitsIt->getProcessOrder(),PrevUnitsIt->getProcessOrder());
    }
    PrevUnitsIt = UnitsIt;
  }


  // *** existing classes
  BOOST_REQUIRE_EQUAL(Repos->isUnitsClassExist("UnitClassA"),true);
  BOOST_REQUIRE_EQUAL(Repos->isUnitsClassExist("UnitClassB"),true);
  BOOST_REQUIRE_EQUAL(Repos->isUnitsClassExist("WrongClass"),false);


  // *** existing units
  BOOST_REQUIRE(Repos->getUnit("UnitClassA",17) != NULL);
  BOOST_REQUIRE(Repos->getUnit("UnitClassA",1000) == NULL);
  U = Repos->getUnit("UnitClassA",17);
  BOOST_REQUIRE_EQUAL(U->getClass(),"UnitClassA");
  BOOST_REQUIRE_EQUAL(U->getID(),17);

  BOOST_REQUIRE(Repos->getUnit("UnitClassB",1333) != NULL);
  BOOST_REQUIRE(Repos->getUnit("UnitClassB",10000) == NULL);
  U = Repos->getUnit("UnitClassB",1333);
  BOOST_REQUIRE_EQUAL(U->getClass(),"UnitClassB");
  BOOST_REQUIRE_EQUAL(U->getID(),1333);

  BOOST_REQUIRE(Repos->getUnit("WrongClass",1) == NULL);
}

// =====================================================================
// =====================================================================
