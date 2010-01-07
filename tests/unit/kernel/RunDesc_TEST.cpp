/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file RunDesc_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_rundesc
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include "run/RunDescriptor.h"

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  RunDescriptor RunDesc(57,openfluid::core::DateTime(2010,1,7,16,24,13),openfluid::core::DateTime(2010,1,9,12,0,0));

  BOOST_REQUIRE_EQUAL(RunDesc.getDeltaT(),57);

  BOOST_REQUIRE_EQUAL(RunDesc.getBeginDate().getDay(),7);
  BOOST_REQUIRE_EQUAL(RunDesc.getBeginDate().getHour(),16);
  BOOST_REQUIRE_EQUAL(RunDesc.getEndDate().getDay(),9);
  BOOST_REQUIRE_EQUAL(RunDesc.getEndDate().getHour(),12);
  BOOST_REQUIRE_EQUAL(RunDesc.getEndDate().getMinute(),0);
  BOOST_REQUIRE_EQUAL(RunDesc.getEndDate().getSecond(),0);
  BOOST_REQUIRE_EQUAL(RunDesc.isProgressiveOutput(),false);
  BOOST_REQUIRE_EQUAL(RunDesc.isSimulationID(),false);
  BOOST_REQUIRE_EQUAL(RunDesc.getSimulationID(),"");

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  RunDescriptor RunDesc(57,openfluid::core::DateTime(2010,1,7,16,24,13),openfluid::core::DateTime(2010,1,9,12,0,0));

  RunDesc.setSimulationID("testID");
  RunDesc.setProgressiveOutput(20,5);

  BOOST_REQUIRE_EQUAL(RunDesc.getDeltaT(),57);
  BOOST_REQUIRE_EQUAL(RunDesc.getBeginDate().getDay(),7);
  BOOST_REQUIRE_EQUAL(RunDesc.getBeginDate().getHour(),16);
  BOOST_REQUIRE_EQUAL(RunDesc.getEndDate().getDay(),9);
  BOOST_REQUIRE_EQUAL(RunDesc.getEndDate().getHour(),12);
  BOOST_REQUIRE_EQUAL(RunDesc.getEndDate().getMinute(),0);
  BOOST_REQUIRE_EQUAL(RunDesc.getEndDate().getSecond(),0);
  BOOST_REQUIRE_EQUAL(RunDesc.isProgressiveOutput(),true);
  BOOST_REQUIRE_EQUAL(RunDesc.getProgressiveOutputPacket(),20);
  BOOST_REQUIRE_EQUAL(RunDesc.getProgressiveOutputKeep(),5);
  BOOST_REQUIRE_EQUAL(RunDesc.isSimulationID(),true);
  BOOST_REQUIRE_EQUAL(RunDesc.getSimulationID(),"testID");

  RunDesc.setSimulationID("gotcha");
  RunDesc.setProgressiveOutput(1179,53);

  BOOST_REQUIRE_EQUAL(RunDesc.isProgressiveOutput(),true);
  BOOST_REQUIRE_EQUAL(RunDesc.getProgressiveOutputPacket(),1179);
  BOOST_REQUIRE_EQUAL(RunDesc.getProgressiveOutputKeep(),53);
  BOOST_REQUIRE_EQUAL(RunDesc.isSimulationID(),true);
  BOOST_REQUIRE_EQUAL(RunDesc.getSimulationID(),"gotcha");

  RunDesc.setSimulationID("");
  RunDesc.setProgressiveOutput(0,0);

  BOOST_REQUIRE_EQUAL(RunDesc.isProgressiveOutput(),false);
  BOOST_REQUIRE_EQUAL(RunDesc.isSimulationID(),false);

}

// =====================================================================
// =====================================================================
