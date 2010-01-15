/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file RunReader.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_runreader
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>

#include "tests-config.h"
#include "RunReader.h"

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  RunDescriptor RD;
  BOOST_REQUIRE_EQUAL(RD.getDeltaT(),-1);
  BOOST_REQUIRE(RD.getBeginDate() == openfluid::core::DateTime());
  BOOST_REQUIRE(RD.getEndDate() == openfluid::core::DateTime());
  BOOST_REQUIRE_EQUAL(RD.isProgressiveOutput(),false);
  BOOST_REQUIRE_EQUAL(RD.isSimulationID(),false);

  RunDescriptor RD2(55,openfluid::core::DateTime(2001,1,1,15,28,59),openfluid::core::DateTime(2001,1,31,0,0,0));
  BOOST_REQUIRE(RD2.getBeginDate() == openfluid::core::DateTime(2001,1,1,15,28,59));
  BOOST_REQUIRE(RD2.getEndDate() == openfluid::core::DateTime(2001,1,31,0,0,0));
  BOOST_REQUIRE_EQUAL(RD2.isProgressiveOutput(),false);
  BOOST_REQUIRE_EQUAL(RD2.isSimulationID(),false);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  RunDescriptor RD1;
  RunReader Reader1;

  RD1 = Reader1.readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/run1.xml").string());

  BOOST_REQUIRE_EQUAL(RD1.getDeltaT(),3600);
  BOOST_REQUIRE(RD1.getBeginDate() == openfluid::core::DateTime(2000,1,1,0,0,0));
  BOOST_REQUIRE(RD1.getEndDate()== openfluid::core::DateTime(2000,1,1,6,0,0));
  BOOST_REQUIRE_EQUAL(RD1.isSimulationID(),false);
  BOOST_REQUIRE_EQUAL(RD1.isProgressiveOutput(),false);


  RunDescriptor RD2;
  RunReader Reader2;

  RD2 = Reader2.readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/run2.xml").string());

  BOOST_REQUIRE_EQUAL(RD2.getDeltaT(),4753);
  BOOST_REQUIRE(RD2.getBeginDate() == openfluid::core::DateTime(1997,1,2,11,15,48));
  BOOST_REQUIRE(RD2.getEndDate() == openfluid::core::DateTime(2005,11,30,6,53,7));
  BOOST_REQUIRE_EQUAL(RD2.isSimulationID(),true);
  BOOST_REQUIRE_EQUAL(RD2.getSimulationID(),"testrun");
  BOOST_REQUIRE_EQUAL(RD2.isProgressiveOutput(),true);
  BOOST_REQUIRE_EQUAL(RD2.getProgressiveOutputPacket(),100);
  BOOST_REQUIRE_EQUAL(RD2.getProgressiveOutputKeep(),5);

}


// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_error_handling)
{
  RunDescriptor RunDesc;
  bool HasFailed;


  HasFailed = false;
  try
  {
    RunDesc = RunReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/rundoesnotexist.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    RunDesc = RunReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongrun1.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    RunDesc = RunReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongrun2.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    RunDesc = RunReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongrun3.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    RunDesc = RunReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongrun4.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    RunDesc = RunReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongrun5.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    RunDesc = RunReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongrun6.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    RunDesc = RunReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongrun7.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    RunDesc = RunReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongrun9.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    RunDesc = RunReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongrun10.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);



}

