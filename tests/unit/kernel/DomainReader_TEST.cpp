/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file DomainReader.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_domainreader
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>

#include "tests-config.h"
#include "DomainReader.h"

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  DomainReader DM;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{

  openfluid::core::CoreRepository* Data = openfluid::core::CoreRepository::getInstance();

  DomainReader DR;

  DR.loadDomainFromDirectory(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/domain").string(),Data);


  BOOST_REQUIRE_EQUAL(Data->isUnitsClassExist("UnitsA"),true);
  BOOST_REQUIRE_EQUAL(Data->isUnitsClassExist("UnitsB"),true);
  BOOST_REQUIRE_EQUAL(Data->isUnitsClassExist("UnitsAA"),false);

  BOOST_REQUIRE_EQUAL(Data->getUnits("UnitsA")->getList()->size(),8);
  BOOST_REQUIRE_EQUAL(Data->getUnits("UnitsB")->getList()->size(),5);
  BOOST_REQUIRE_EQUAL(Data->getUnits("UnitsABC")->getList()->size(),0);


  BOOST_FAIL("under construction");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_error_handling)
{
  BOOST_FAIL("under construction");
}
