/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file SSerieValues_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_sserievalues
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::StepSerieOfValues<double> SSerie;

  BOOST_REQUIRE_EQUAL(SSerie.getNextStep(),0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::StepSerieOfValues<double> SSerie;
  double Value;

  BOOST_REQUIRE_EQUAL(SSerie.getNextStep(),0);

  BOOST_REQUIRE_EQUAL(SSerie.appendValue(1.1),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(2.2),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(3.3),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(4.4),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(5.5),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(6.6),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(7.7),true);

  BOOST_REQUIRE_EQUAL(SSerie.getNextStep(),7);

  BOOST_REQUIRE_EQUAL(SSerie.getValue(7,&Value),false);

  BOOST_REQUIRE_EQUAL(SSerie.getValue(3,&Value),true);
  BOOST_REQUIRE_CLOSE(Value,4.4,0.001);

  BOOST_REQUIRE_EQUAL(SSerie.modifyValue(3,44.0),true);
  BOOST_REQUIRE_EQUAL(SSerie.getValue(3,&Value),true);
  BOOST_REQUIRE_CLOSE(Value,44.0,0.001);

  BOOST_REQUIRE_EQUAL(SSerie.deleteValues(3),true);
  BOOST_REQUIRE_EQUAL(SSerie.getValue(3,&Value),false);
  BOOST_REQUIRE_NE(SSerie.getValue(3,&Value),true);

  BOOST_REQUIRE_EQUAL(SSerie.appendValue(10.1),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(20.2),true);
  BOOST_REQUIRE_EQUAL(SSerie.appendValue(30.3),true);

  BOOST_REQUIRE_EQUAL(SSerie.deleteValues(8),true);

}

// =====================================================================
// =====================================================================
