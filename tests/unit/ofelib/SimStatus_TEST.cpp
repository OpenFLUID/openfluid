/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file SimStatus_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_simstatus
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::SimulationStatus SimStatus(openfluid::core::DateTime(2000,1,1,0,0,0),
                                    openfluid::core::DateTime(2001,1,1,0,0,0),
                                    60);


  BOOST_REQUIRE(SimStatus.getStartTime() == openfluid::core::DateTime(2000,1,1,0,0,0));
  BOOST_REQUIRE(SimStatus.getEndTime() == openfluid::core::DateTime(2001,1,1,0,0,0));
  BOOST_REQUIRE_EQUAL(SimStatus.getTimeStep(),60);


  BOOST_REQUIRE_EQUAL(SimStatus.isFirstStep(),true);
  BOOST_REQUIRE_EQUAL(SimStatus.isLastStep(),false);
  BOOST_REQUIRE_EQUAL(SimStatus.getCurrentStep(),0);
  BOOST_REQUIRE(SimStatus.getCurrentTime() == openfluid::core::DateTime(2000,1,1,0,0,0));

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::base::SimulationStatus SimStatus(openfluid::core::DateTime(2000,1,1,0,0,0),
                                      openfluid::core::DateTime(2001,1,1,0,0,0),
                                      76);

  openfluid::core::DateTime PrevTime;
  openfluid::core::TimeStep_t PrevStep;

  do
  {
    if (!SimStatus.isFirstStep())
    {
      BOOST_REQUIRE(SimStatus.getCurrentTime() == (PrevTime + SimStatus.getTimeStep()));
      BOOST_REQUIRE(SimStatus.getCurrentStep() == (PrevStep + 1));
    }

    PrevTime = SimStatus.getCurrentTime();
    PrevStep = SimStatus.getCurrentStep();

    if (SimStatus.isLastStep())
    {
      BOOST_REQUIRE_EQUAL(SimStatus.getCurrentStep(),SimStatus.getStepsCount()-1);
      BOOST_REQUIRE((SimStatus.getCurrentTime()+SimStatus.getTimeStep()) >= SimStatus.getEndTime());
    }

  } while (SimStatus.switchToNextStep());

}

// =====================================================================
// =====================================================================
