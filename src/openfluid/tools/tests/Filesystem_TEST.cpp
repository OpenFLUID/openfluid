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
  @file Filesystem_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_filesystem
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <tests-config.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_names_operations)
{
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::filename("/my/path/myfile.txt"),"myfile.txt");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::filename("/my/other/path"),"path");

  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::basename("/my/path/myfile.txt"),"myfile");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::basename("/my/other/path"),"path");

  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::extension("/my/path/myfile.txt"),"txt");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::extension("/my/other/path"),"");

  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::dirname("/my/path/myfile.txt"),"/my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::dirname("/my/path/"),"/my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::dirname("/my/path"),"/my");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::dirname("/my/path.txt"),"/my");

  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::filename("file://url/like/path/file.txt"),"file.txt");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::filename("http://url/like/path/file.txt"),"file.txt");
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_test_operations)
{
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_BINARY_DIR));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(CONFIGTESTS_SRC_DIR+"/CMakeLists.txt"));

  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(CONFIGTESTS_BINARY_DIR));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_SRC_DIR+"/CMakeLists.txt"));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_readwrite_operations)
{
  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem"));


  // Directories

  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDir"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDir"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                            "/Filesystem/MadeDir/MadeSubdir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                          "/Filesystem/MadeDir/MadeSubdir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                              "/Filesystem/MadeDir/MadeSubdir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDir"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                           "/Filesystem/MadeDir/MadeSubdir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDir"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDir"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                            "/Filesystem/MadeDir/MadeSubdir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDir"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDir"));


  // Files

  BOOST_REQUIRE(openfluid::tools::Filesystem::copyFile(CONFIGTESTS_SRC_DIR+"/CMakeLists.txt",
                                                       CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/CMakeLists.txt"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/CMakeLists.txt"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::removeFile(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/CMakeLists.txt"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/CMakeLists.txt"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::copyDirectory(CONFIGTESTS_INPUT_MISCDATA_DIR+"/Filesystem",
                                                            CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/dircopy"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                     "/Filesystem/dircopy/Filesystem/README"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                     "/Filesystem/dircopy/Filesystem/README.TOO"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                     "/Filesystem/dircopy/Filesystem/subdir/another_subdir/README"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                          "/Filesystem/dircopy/Filesystem/subdir/another_subdir"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/dircopy"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/dircopy"));


  // Unique subdirs

  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                            "/Filesystem/MadeDirForUnique"));

  BOOST_REQUIRE_EQUAL(
    openfluid::tools::Filesystem::makeUniqueSubdirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDirForUnique",
                                                         "Unique"),
    CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDirForUnique/Unique"
  );

  BOOST_REQUIRE_EQUAL(
    openfluid::tools::Filesystem::makeUniqueSubdirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDirForUnique",
                                                         "Unique"),
    CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDirForUnique/Unique1"
  );

  BOOST_REQUIRE_EQUAL(
    openfluid::tools::Filesystem::makeUniqueSubdirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDirForUnique",
                                                         "Unique"),
    CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDirForUnique/Unique2"
  );

  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                                "/Filesystem/MadeDirForUnique/Unique1"));

  BOOST_REQUIRE_EQUAL(
    openfluid::tools::Filesystem::makeUniqueSubdirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDirForUnique",
                                                         "Unique"),
    CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDirForUnique/Unique1"
  );

  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+
                                                                "/Filesystem/MadeDirForUnique/Unique"));

  BOOST_REQUIRE_EQUAL(
    openfluid::tools::Filesystem::makeUniqueSubdirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDirForUnique",
                                                         "Unique"),
    CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem/MadeDirForUnique/Unique"
  );
}

