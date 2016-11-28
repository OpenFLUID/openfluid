/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/



/**
  @file FileHelpers_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_filehelpers
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <tests-config.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{

  openfluid::tools::copyDirectoryContentsRecursively(CONFIGTESTS_INPUT_MISCDATA_DIR+"/Filesystem",
                                                     CONFIGTESTS_OUTPUT_DATA_DIR+"/FileHelpers/dircopycontents");

  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                     "/FileHelpers/dircopycontents/README"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                     "/FileHelpers/dircopycontents/README.TOO"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                     "/FileHelpers/dircopycontents/subdir/another_subdir/README"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                     "/FileHelpers/dircopycontents/subdir2/README.TXT"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                          "/FileHelpers/dircopycontents/subdir/another_subdir"));

  openfluid::tools::emptyDirectoryRecursively(CONFIGTESTS_OUTPUT_DATA_DIR+"/FileHelpers/dircopycontents");

  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/FileHelpers/dircopycontents"));

  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                      "/FileHelpers/dircopycontents/README"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                      "/FileHelpers/dircopycontents/README.TOO"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                      "/FileHelpers/dircopycontents/subdir/another_subdir/README"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                      "/FileHelpers/dircopycontents/subdir2/README.TXT"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                           "/FileHelpers/dircopycontents/subdir/another_subdir"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                              "/FileHelpers/dircopycontents"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/FileHelpers/dircopycontents"));

}

