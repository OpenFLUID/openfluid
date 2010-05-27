/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


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
#include <tests-config.hpp>
#include <openfluid/machine/DynamicLib.hpp>
#include <openfluid/base.hpp>

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::machine::DynamicLib DLib("");

  BOOST_REQUIRE_EQUAL(DLib.isLoaded(),false);
  BOOST_REQUIRE_EQUAL(DLib.getLibName(),"");

  openfluid::machine::DynamicLib DLib2("foo/bar");

  BOOST_REQUIRE_EQUAL(DLib2.isLoaded(),false);
  BOOST_REQUIRE_EQUAL(DLib2.getLibName(),boost::filesystem::path("foo/bar").string());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  std::string LibToLoad = CONFIGTESTS_OUTPUT_BINARY_DIR+"/tests.fakefunction"+CONFIGTESTS_PLUGINS_EXT;
  openfluid::base::GetSDKVersionProc SDKProc;
  openfluid::base::GetSignatureProc SignProc;
  openfluid::base::GetPluggableFunctionProc FuncProc;

  openfluid::base::PluggableFunction* PlugFunc;
  openfluid::base::FunctionSignature* PlugSignature;

  openfluid::machine::DynamicLib DLib(LibToLoad);

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

  BOOST_REQUIRE_EQUAL(SDKProc(),CONFIGTESTS_FULL_VERSION);

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
