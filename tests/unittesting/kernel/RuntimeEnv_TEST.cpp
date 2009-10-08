/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file RuntimeEnv_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_runtimeenv
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include "RuntimeEnv.h"
#include "openfluid-base.h"
#include <wx/app.h>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  if (!wxInitialize())
  {
    BOOST_FAIL("Error initializing wx libs");
  }

  RuntimeEnvironment RunEnv;

  BOOST_REQUIRE_NE(RunEnv.getInputDir(),"");
  BOOST_REQUIRE_NE(RunEnv.getOutputDir(),"");
  BOOST_REQUIRE_NE(RunEnv.getTempDir(),"");
  BOOST_REQUIRE_GT(RunEnv.getPluginsPaths().size(),0);
  BOOST_REQUIRE_EQUAL(RunEnv.isClearOutputDir(),false);
  BOOST_REQUIRE_EQUAL(RunEnv.isVerboseRun(),false);
  BOOST_REQUIRE_EQUAL(RunEnv.isQuietRun(),false);
  BOOST_REQUIRE_EQUAL(RunEnv.isWriteResults(),true);
  BOOST_REQUIRE_EQUAL(RunEnv.isWriteSimReport(),true);
  BOOST_REQUIRE_EQUAL(RunEnv.isProgressiveOutput(),false);
  BOOST_REQUIRE_EQUAL(RunEnv.isCheckVarNames(),true);
  BOOST_REQUIRE_NE(RunEnv.getSimulationID(),"");

  wxUninitialize();
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  if (!wxInitialize())
  {
    BOOST_FAIL("Error initializing wx libs");
  }

  RuntimeEnvironment RunEnv;

  RunEnv.setInputDir("/foo/bar");
  RunEnv.setOutputDir("/bar/foo/bar");
  RunEnv.setOutputDir("/bar/foo/bar");
  RunEnv.addExtraPluginsPaths("/bar/foo/foo/bar");
  RunEnv.addExtraPluginsPaths("/bar/foo/foo/bar/bar/bar");
  BOOST_REQUIRE_EQUAL(RunEnv.getInputDir(),"/foo/bar");
  BOOST_REQUIRE_EQUAL(RunEnv.getOutputDir(),"/bar/foo/bar");
  BOOST_REQUIRE_NE(RunEnv.getTempDir(),"");
  BOOST_REQUIRE_GT(RunEnv.getPluginsPaths().size(),2);


  RunEnv.setClearOutputDir(true);
  BOOST_REQUIRE_EQUAL(RunEnv.isClearOutputDir(),true);

  RunEnv.setVerboseRun(true);
  BOOST_REQUIRE_EQUAL(RunEnv.isVerboseRun(),true);
  BOOST_REQUIRE_EQUAL(RunEnv.isQuietRun(),false);

  RunEnv.setVerboseRun(false);
  BOOST_REQUIRE_EQUAL(RunEnv.isVerboseRun(),false);
  BOOST_REQUIRE_EQUAL(RunEnv.isQuietRun(),false);

  RunEnv.setVerboseRun(true);
  RunEnv.setQuietRun(true);
  BOOST_REQUIRE_EQUAL(RunEnv.isVerboseRun(),false);
  BOOST_REQUIRE_EQUAL(RunEnv.isQuietRun(),true);

  RunEnv.setVerboseRun(false);
  RunEnv.setQuietRun(false);
  BOOST_REQUIRE_EQUAL(RunEnv.isVerboseRun(),false);
  BOOST_REQUIRE_EQUAL(RunEnv.isQuietRun(),false);

  RunEnv.setWriteResults(false);
  BOOST_REQUIRE_EQUAL(RunEnv.isWriteResults(),false);

  RunEnv.setWriteSimReport(false);
  BOOST_REQUIRE_EQUAL(RunEnv.isWriteSimReport(),false);

  RunEnv.setCheckVarNames(false);
  BOOST_REQUIRE_EQUAL(RunEnv.isCheckVarNames(),false);

  RunEnv.setProgressiveOutputKeep(23);
  RunEnv.setProgressiveOutputPacket(2345);
  BOOST_REQUIRE_EQUAL(RunEnv.isProgressiveOutput(),true);
  BOOST_REQUIRE_EQUAL(RunEnv.getProgressiveOutputPacket(),2345);
  BOOST_REQUIRE_EQUAL(RunEnv.getProgressiveOutputKeep(),23);

  RunEnv.setSimulationID("THESIMID-2");
  BOOST_REQUIRE_EQUAL(RunEnv.getSimulationID(),"THESIMID-2");




  openfluid::base::FunctionEnvironment* FuncEnv;
  bool BoolValue;
  std::string StrValue;

  FuncEnv = RunEnv.getFunctionEnvironment();
  BOOST_REQUIRE(FuncEnv != NULL);

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("wrong.fake",&StrValue),false);
  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("fake.wrong",&BoolValue),false);

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("dir.input",&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,RunEnv.getInputDir());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("dir.output",&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,RunEnv.getOutputDir());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("dir.temp",&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,RunEnv.getTempDir());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("mode.clearoutputdir",&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,RunEnv.isClearOutputDir());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("mode.quiet",&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,RunEnv.isQuietRun());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("mode.verbose",&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,RunEnv.isVerboseRun());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("mode.saveresults",&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,RunEnv.isWriteResults());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("mode.writereport",&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,RunEnv.isWriteSimReport());



  wxUninitialize();
}

// =====================================================================
// =====================================================================
