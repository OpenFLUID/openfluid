/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file Vector_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_vector
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::Vector<double> V1,V4;
  openfluid::core::Vector<double> V2(5);
  openfluid::core::Vector<double> V3(50000,12.3);

  BOOST_REQUIRE_EQUAL(V1.size(),0);
  BOOST_REQUIRE_EQUAL(V2.size(),5);
  BOOST_REQUIRE_EQUAL(V3.size(),50000);

  BOOST_REQUIRE_CLOSE(V3.at(17564),12.3,0.0001);
  BOOST_REQUIRE_CLOSE(V3.at(0),12.3,0.0001);
  BOOST_REQUIRE_CLOSE(V3.at(49999),12.3,0.0001);

  V4 = openfluid::core::Vector<double>(V3);
  BOOST_REQUIRE_CLOSE(V4.at(17564),12.3,0.0001);
  BOOST_REQUIRE_CLOSE(V4.at(0),12.3,0.0001);
  BOOST_REQUIRE_CLOSE(V4.at(49999),12.3,0.0001);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operationss)
{
  openfluid::core::Vector<double> V1(10,12.3);
  openfluid::core::Vector<double> V2,V3;

  BOOST_REQUIRE_EQUAL(V1.getSize(),10);

  V1[1] = -5.18;
  V1.setElement(3,12345.6789);
  BOOST_REQUIRE_CLOSE(V1[1],-5.18,0.0001);
  BOOST_REQUIRE_CLOSE(V1.at(3),12345.6789,0.0001);

  V2.setData(V1.getData(),V1.getSize());
  BOOST_REQUIRE_EQUAL(V2.getSize(),10);
  BOOST_REQUIRE_CLOSE(V2.at(1),-5.18,0.0001);
  BOOST_REQUIRE_CLOSE(V2.getElement(3),12345.6789,0.0001);

  V3 = V2;
  BOOST_REQUIRE_EQUAL(V3.getSize(),10);
  BOOST_REQUIRE_CLOSE(V3.at(1),-5.18,0.0001);
  BOOST_REQUIRE_CLOSE(V3.getElement(3),12345.6789,0.0001);

  V3.clear();
  BOOST_REQUIRE_EQUAL(V3.getSize(),0);
}

// =====================================================================
// =====================================================================

