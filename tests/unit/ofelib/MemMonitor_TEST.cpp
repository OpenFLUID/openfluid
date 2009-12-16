/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file MemMonitor_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_memmonitor
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::MemoryMonitor *MM;

  MM = openfluid::core::MemoryMonitor::getInstance();

  BOOST_REQUIRE_EQUAL(MM->getPacket(),0);
  BOOST_REQUIRE_EQUAL(MM->getKeep(),0);
  BOOST_REQUIRE_EQUAL(MM->getLastMemoryRelease(),-1);
}

// =====================================================================
// =====================================================================


void check_operation_withparams(unsigned int Packet, unsigned int Keep, unsigned int StepsCount)
{
  openfluid::core::MemoryMonitor *MM;

  MM = openfluid::core::MemoryMonitor::getInstance();
  MM->setPacketAndKeep(Packet,Keep);
  MM->setLastMemoryRelease(Keep-1);

  BOOST_REQUIRE_EQUAL(MM->getPacket(),Packet);
  BOOST_REQUIRE_EQUAL(MM->getKeep(),Keep);
  BOOST_REQUIRE_EQUAL(MM->getLastMemoryRelease(),-1);

  for (unsigned int i = 0; i < StepsCount; i++)
  {
    if (MM->isMemReleaseStep(i))
    {
      BOOST_REQUIRE_EQUAL(i-MM->getLastMemoryRelease(),MM->getPacket()+MM->getKeep());
      BOOST_REQUIRE_NE(MM->getLastMemoryRelease(),i-MM->getKeep());
      MM->setLastMemoryRelease(i);
      BOOST_REQUIRE_EQUAL(MM->getLastMemoryRelease(),i-MM->getKeep());
    }
    else
    {
      BOOST_REQUIRE_NE(i-MM->getLastMemoryRelease(),MM->getPacket()+MM->getKeep());
    }
  }
}

BOOST_AUTO_TEST_CASE(check_operations)
{
  check_operation_withparams(500,10,10000);
  check_operation_withparams(1,1,10000);
  check_operation_withparams(5000,100,100);
}

// =====================================================================
// =====================================================================
