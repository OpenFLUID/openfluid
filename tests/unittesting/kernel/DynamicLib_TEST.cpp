/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file DynamicLib_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_dynamiclib
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include "tests-config.h"
#include "DynamicLib.h"
#include "openfluid-base.h"

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  DynamicLib DLib("");

  BOOST_REQUIRE_EQUAL(DLib.isLoaded(),false);
  BOOST_REQUIRE_EQUAL(DLib.getLibName(),"");

  DynamicLib DLib2("foo/bar");

  BOOST_REQUIRE_EQUAL(DLib2.isLoaded(),false);
  BOOST_REQUIRE_EQUAL(DLib2.getLibName(),boost::filesystem::path("foo/bar").string());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  std::string LibToLoad = TESTS_OUTPUT_BINARY_DIR+"/tests.fakefunction"+OPENFLUID_PLUGINS_EXT;
  openfluid::base::GetSDKVersionProc SDKProc;
  openfluid::base::GetSignatureProc SignProc;
  openfluid::base::GetPluggableFunctionProc FuncProc;

  openfluid::base::PluggableFunction* PlugFunc;
  openfluid::base::FunctionSignature* PlugSignature;

  DynamicLib DLib(LibToLoad);

  BOOST_REQUIRE_EQUAL(DLib.getLibName(),boost::filesystem::path(LibToLoad).string());

  DLib.load();

  BOOST_REQUIRE_EQUAL(DLib.isLoaded(),true);

  BOOST_REQUIRE_EQUAL(DLib.hasSymbol("foobar"),false);
  BOOST_REQUIRE_EQUAL(DLib.hasSymbol(PLUGSDKVERSION_PROC_NAME),true);
  BOOST_REQUIRE_EQUAL(DLib.hasSymbol(PLUGSIGNATURE_PROC_NAME),true);
  BOOST_REQUIRE_EQUAL(DLib.hasSymbol(PLUGFUNCTION_PROC_NAME),true);


  SDKProc = (openfluid::base::GetSDKVersionProc)DLib.getSymbol(PLUGSDKVERSION_PROC_NAME);
  SignProc = (openfluid::base::GetSignatureProc)DLib.getSymbol(PLUGSIGNATURE_PROC_NAME);
  FuncProc = (openfluid::base::GetPluggableFunctionProc)DLib.getSymbol(PLUGFUNCTION_PROC_NAME);

  BOOST_REQUIRE_EQUAL(SDKProc(),FULL_VERSION);

  PlugSignature = SignProc();
  PlugFunc = FuncProc();

  BOOST_REQUIRE(PlugSignature != NULL);
  BOOST_REQUIRE(PlugFunc != NULL);

  BOOST_REQUIRE_EQUAL(PlugSignature->ID,"tests.fakefunction");

  DLib.unload();

  BOOST_REQUIRE_EQUAL(DLib.isLoaded(),false);


}

// =====================================================================
// =====================================================================
