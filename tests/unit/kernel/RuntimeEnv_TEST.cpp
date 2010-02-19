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

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{


  BOOST_REQUIRE_NE(RuntimeEnvironment::getInstance()->getInputDir(),"");
  BOOST_REQUIRE_NE(RuntimeEnvironment::getInstance()->getOutputDir(),"");
  BOOST_REQUIRE_NE(RuntimeEnvironment::getInstance()->getTempDir(),"");
  BOOST_REQUIRE_GT(RuntimeEnvironment::getInstance()->getPluginsPaths().size(),0);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isClearOutputDir(),false);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isVerboseRun(),false);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isQuietRun(),false);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isWriteResults(),true);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isWriteSimReport(),true);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isProgressiveOutput(),false);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isCheckVarNames(),true);
  BOOST_REQUIRE_NE(RuntimeEnvironment::getInstance()->getSimulationID(),"");

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{

  RuntimeEnvironment::getInstance()->setInputDir("/foo/bar");
  RuntimeEnvironment::getInstance()->setOutputDir("/bar/foo/bar");
  RuntimeEnvironment::getInstance()->setOutputDir("/bar/foo/bar");
  RuntimeEnvironment::getInstance()->addExtraPluginsPaths("/bar/foo/foo/bar");
  RuntimeEnvironment::getInstance()->addExtraPluginsPaths("/bar/foo/foo/bar/bar/bar");
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->getInputDir(),"/foo/bar");
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->getOutputDir(),"/bar/foo/bar");
  BOOST_REQUIRE_NE(RuntimeEnvironment::getInstance()->getTempDir(),"");
  BOOST_REQUIRE_GT(RuntimeEnvironment::getInstance()->getPluginsPaths().size(),2);


  RuntimeEnvironment::getInstance()->setClearOutputDir(true);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isClearOutputDir(),true);

  RuntimeEnvironment::getInstance()->setVerboseRun(true);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isVerboseRun(),true);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isQuietRun(),false);

  RuntimeEnvironment::getInstance()->setVerboseRun(false);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isVerboseRun(),false);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isQuietRun(),false);

  RuntimeEnvironment::getInstance()->setVerboseRun(true);
  RuntimeEnvironment::getInstance()->setQuietRun(true);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isVerboseRun(),false);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isQuietRun(),true);

  RuntimeEnvironment::getInstance()->setVerboseRun(false);
  RuntimeEnvironment::getInstance()->setQuietRun(false);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isVerboseRun(),false);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isQuietRun(),false);

  RuntimeEnvironment::getInstance()->setWriteResults(false);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isWriteResults(),false);

  RuntimeEnvironment::getInstance()->setWriteSimReport(false);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isWriteSimReport(),false);

  RuntimeEnvironment::getInstance()->setCheckVarNames(false);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isCheckVarNames(),false);

  RuntimeEnvironment::getInstance()->setProgressiveOutputKeep(23);
  RuntimeEnvironment::getInstance()->setProgressiveOutputPacket(2345);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->isProgressiveOutput(),true);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->getProgressiveOutputPacket(),2345);
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->getProgressiveOutputKeep(),23);

  RuntimeEnvironment::getInstance()->setSimulationID("THESIMID-2");
  BOOST_REQUIRE_EQUAL(RuntimeEnvironment::getInstance()->getSimulationID(),"THESIMID-2");




  openfluid::base::FunctionEnvironment* FuncEnv;
  bool BoolValue;
  std::string StrValue;

  FuncEnv = RuntimeEnvironment::getInstance()->getFunctionEnvironment();
  BOOST_REQUIRE(FuncEnv != NULL);

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("wrong.fake",&StrValue),false);
  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("fake.wrong",&BoolValue),false);

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("dir.input",&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,RuntimeEnvironment::getInstance()->getInputDir());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("dir.output",&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,RuntimeEnvironment::getInstance()->getOutputDir());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("dir.temp",&StrValue),true);
  BOOST_REQUIRE_EQUAL(StrValue,RuntimeEnvironment::getInstance()->getTempDir());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("mode.clearoutputdir",&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,RuntimeEnvironment::getInstance()->isClearOutputDir());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("mode.quiet",&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,RuntimeEnvironment::getInstance()->isQuietRun());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("mode.verbose",&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,RuntimeEnvironment::getInstance()->isVerboseRun());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("mode.saveresults",&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,RuntimeEnvironment::getInstance()->isWriteResults());

  BOOST_REQUIRE_EQUAL(FuncEnv->getValue("mode.writereport",&BoolValue),true);
  BOOST_REQUIRE_EQUAL(BoolValue,RuntimeEnvironment::getInstance()->isWriteSimReport());

}

// =====================================================================
// =====================================================================
