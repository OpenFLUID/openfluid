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
  @file FilesystemPath_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_filesystempath


#include <set>
#include <iostream>

#include <boost/test/unit_test.hpp>

#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/global.hpp>

#include "FilesystemCommon.hpp"
#include "tests-config.hpp"


// =====================================================================
// =====================================================================


const std::string FSInputPath = CONFIGTESTS_INPUT_MISCDATA_DIR+"/FilesystemPath";
const std::string FSOutputPath = CONFIGTESTS_OUTPUT_DATA_DIR+"/FilesystemPath";


BOOST_AUTO_TEST_CASE(init)
{
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(FSOutputPath).removeDirectory());
  BOOST_REQUIRE(!openfluid::tools::Path(FSOutputPath).exists());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(std_filesystem)
{
  const std::vector<std::string> PathsList = {
    "/path/to/file.md",
    "C:/path/to/file.md",
    "C:\\path\\to\\file.md",
    //---
    "/path/to///file.md",
    "C:/path////to/file.md",
    "C:\\path\\\\\\to\\file.md",
    "////path/to/folder////"
  };

  std::cout << "===== std::filesystem::path explorer =====" << std::endl;
  for (const auto& PathStr: PathsList)
  {
    std::filesystem::path P(PathStr);
    std::cout << "Path : " << PathStr << std::endl;
    std::cout << "  .string() : " << P.string() << std::endl;
    std::cout << "  .generic_string() : " << P.generic_string() << std::endl;
    std::cout << "  .lexically_normal().string() : " << P.lexically_normal().string() << std::endl;
    auto PPref = P; std::cout << "  .make_preferred().string() : " << PPref.make_preferred().string() << std::endl;
    std::cout << "  .parent_path().string() : " << P.parent_path().string() << std::endl;
    std::cout << "  .stem().string() : " << P.stem().string() << std::endl;
    std::cout << "  .filename().string() : " << P.filename().string() << std::endl;
    std::cout << "  .extension().string() : " << P.extension().string() << std::endl;
  }
  std::cout << "========================================" << std::endl;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_path_operations)
{
  std::cout << "System path separator is " << openfluid::tools::FilesystemPath::separator() << std::endl;
  std::cout << "System paths list separator is " << openfluid::tools::Path::listSeparator() << std::endl;


  // -------------------------------------------


  for (const auto& Test : JoinedPaths)
  {
    BOOST_REQUIRE_EQUAL(openfluid::tools::FilesystemPath(Test.Arguments).toGeneric(),Test.Expected);
  }


  // -------------------------------------------


  for (const auto& Test : SplittedPaths)
  {
    const auto SplittedPath = openfluid::tools::FilesystemPath(Test.Arguments).split();
    BOOST_REQUIRE_EQUAL(SplittedPath.size(),Test.Expected.size());
    BOOST_REQUIRE_EQUAL_COLLECTIONS(SplittedPath.begin(),SplittedPath.end(),Test.Expected.begin(),Test.Expected.end());
  }


  // -------------------------------------------


  for (const auto& Test : ContainedPaths)
  {
    BOOST_REQUIRE(openfluid::tools::FilesystemPath(Test.first).contains(Test.second));
  }


  // -------------------------------------------


  for (const auto& Test : UnixPathsWithTrailing)
  {
    BOOST_REQUIRE_EQUAL(openfluid::tools::Path::removeTrailingSeparators(Test.Arguments),Test.Expected);
  }

  for (const auto& Test : WinPathsWithTrailing)
  {
    BOOST_REQUIRE_EQUAL(openfluid::tools::Path::removeTrailingSeparators(Test.Arguments.first,Test.Arguments.second),
                        Test.Expected);
  }


  // -------------------------------------------


#if defined(OPENFLUID_OS_WINDOWS)

  for (const auto& Test : WinPathsToGeneric)
  {
    BOOST_REQUIRE_EQUAL(openfluid::tools::Path(Test.Arguments).toGeneric(),Test.Expected);
  }

    for (const auto& Test : WinPathsToNative)
  {
    BOOST_REQUIRE_EQUAL(openfluid::tools::Path(Test.Arguments).toNative(),Test.Expected);
  }

#elif defined(OPENFLUID_OS_UNIX)  

  for (const auto& Test : UnixPathsToGeneric)
  {
    BOOST_REQUIRE_EQUAL(openfluid::tools::Path(Test.Arguments).toGeneric(),Test.Expected);
  }

  for (const auto& Test : UnixPathsToGenericClean)
  {
    BOOST_REQUIRE_EQUAL(openfluid::tools::Path(Test.Arguments).toGenericClean(),Test.Expected);
  }

  for (const auto& Test : UnixPathsToNative)
  {
    BOOST_REQUIRE_EQUAL(openfluid::tools::Path(Test.Arguments).toNative(),Test.Expected);
  }

#endif

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_names_operations)
{

  for (const auto& Test : PathsComponents)
  {
    openfluid::tools::Path P(Test.Arguments);
    std::cout << P.toGeneric() << std::endl;
    BOOST_REQUIRE_EQUAL(P.filename(),Test.Expected[0]);
    BOOST_REQUIRE_EQUAL(P.dirname(),Test.Expected[1]);
    BOOST_REQUIRE_EQUAL(P.basename(),Test.Expected[2]);
    BOOST_REQUIRE_EQUAL(P.extension(),Test.Expected[3]);
    BOOST_REQUIRE_EQUAL(P.minimalBasename(),Test.Expected[4]);
    BOOST_REQUIRE_EQUAL(P.completeExtension(),Test.Expected[5]);
  }

  BOOST_REQUIRE_EQUAL(openfluid::tools::Path("/my/path/myarchive.tar.gz").minimalBasename()+"."+
                      openfluid::tools::Path("/my/path/myarchive.tar.gz").completeExtension(),
                      openfluid::tools::Path("/my/path/myarchive.tar.gz").basename()+"."+
                      openfluid::tools::Path("/my/path/myarchive.tar.gz").extension());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_path_relative)
{

  for (const auto& Test : RelativePaths)
  {
    BOOST_REQUIRE_EQUAL(openfluid::tools::Path(Test.Arguments.first).relativeTo(Test.Arguments.second),Test.Expected);
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_test_operations)
{
  for (const auto& Test : IsDirPaths)
  {
    BOOST_REQUIRE_EQUAL(openfluid::tools::FilesystemPath(Test.Arguments).isDirectory(),Test.Expected);
  }

  for (const auto& Test : IsFilePaths)
  {
    BOOST_REQUIRE_EQUAL(openfluid::tools::FilesystemPath(Test.Arguments).isFile(),Test.Expected);
  }

  for (const auto& Test : ExistsPaths)
  {
    BOOST_REQUIRE_EQUAL(openfluid::tools::FilesystemPath(Test.Arguments).exists(),Test.Expected);
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_dirfiles_operations)
{

  // Directories

  {
    const auto MadeDir = openfluid::tools::FilesystemPath({FSOutputPath,"dirfiles","MadeDir"});
    BOOST_REQUIRE(MadeDir.makeDirectory());
    BOOST_REQUIRE(MadeDir.isDirectory());
    BOOST_REQUIRE(!MadeDir.isFile());
    BOOST_REQUIRE(MadeDir.exists());
    BOOST_REQUIRE(!MadeDir.removeFile());
    BOOST_REQUIRE(MadeDir.isDirectory());
    BOOST_REQUIRE(MadeDir.removeDirectory());    
    BOOST_REQUIRE(!MadeDir.isDirectory());
    BOOST_REQUIRE(!MadeDir.isFile());
    BOOST_REQUIRE(!MadeDir.exists());
  }

  {
    const auto MadeDir = openfluid::tools::FilesystemPath({FSOutputPath,"dirfiles","MadeDir","MadeSubDir"});
    BOOST_REQUIRE(MadeDir.makeDirectory());
    BOOST_REQUIRE(MadeDir.isDirectory());
    BOOST_REQUIRE(openfluid::tools::FilesystemPath({FSOutputPath,"dirfiles","MadeDir"}).isDirectory());    
    BOOST_REQUIRE(!MadeDir.isFile());
    BOOST_REQUIRE(MadeDir.exists());
    BOOST_REQUIRE(MadeDir.removeDirectory());
    BOOST_REQUIRE(!MadeDir.isDirectory());
    BOOST_REQUIRE(!MadeDir.isFile());
    BOOST_REQUIRE(!MadeDir.exists());
    BOOST_REQUIRE(openfluid::tools::FilesystemPath({FSOutputPath,"dirfiles","MadeDir"}).isDirectory()); 
    BOOST_REQUIRE(openfluid::tools::FilesystemPath({FSOutputPath,"dirfiles","MadeDir"}).remove()); 
    BOOST_REQUIRE(!openfluid::tools::FilesystemPath({FSOutputPath,"dirfiles","MadeDir"}).isDirectory());    
  }

  
  // Files

  {
    const auto MadeFile = openfluid::tools::FilesystemPath({FSOutputPath,"dirfiles","MadeFile.txt"});
    BOOST_REQUIRE(MadeFile.makeFile());
    BOOST_REQUIRE(!MadeFile.isDirectory());
    BOOST_REQUIRE(MadeFile.isFile());
    BOOST_REQUIRE(MadeFile.exists());
    BOOST_REQUIRE(!MadeFile.removeDirectory());
    BOOST_REQUIRE(MadeFile.removeFile());
    BOOST_REQUIRE(!MadeFile.isDirectory());
    BOOST_REQUIRE(!MadeFile.isFile());
    BOOST_REQUIRE(!MadeFile.exists());
    BOOST_REQUIRE(MadeFile.makeFile());
    BOOST_REQUIRE(!MadeFile.isDirectory());
    BOOST_REQUIRE(MadeFile.isFile());
    BOOST_REQUIRE(MadeFile.exists());
    BOOST_REQUIRE(MadeFile.remove());
    BOOST_REQUIRE(!MadeFile.isDirectory());
    BOOST_REQUIRE(!MadeFile.isFile());
    BOOST_REQUIRE(!MadeFile.exists());
  }


  // With subpaths

  {
    const auto MadeSubDir = openfluid::tools::FilesystemPath({FSOutputPath,"dirfileswithsub"});
    BOOST_REQUIRE(MadeSubDir.makeDirectory("MadeDir"));
    BOOST_REQUIRE(MadeSubDir.isDirectory());
    BOOST_REQUIRE(MadeSubDir.isDirectory("MadeDir"));
    BOOST_REQUIRE(MadeSubDir.makeDirectory("MadeDir/ReMadeDir"));
    BOOST_REQUIRE(MadeSubDir.isDirectory("MadeDir/ReMadeDir"));
    BOOST_REQUIRE(MadeSubDir.exists("MadeDir/ReMadeDir"));
    BOOST_REQUIRE(MadeSubDir.makeFile("MadeDir/ReMadeDir/SubFile.txt"));
    BOOST_REQUIRE(MadeSubDir.isFile("MadeDir/ReMadeDir/SubFile.txt"));
    BOOST_REQUIRE(!MadeSubDir.isDirectory("MadeDir/ReMadeDir/SubFile.txt"));
    BOOST_REQUIRE(MadeSubDir.exists("MadeDir/ReMadeDir/SubFile.txt"));
    BOOST_REQUIRE(!MadeSubDir.removeDirectory("MadeDir/ReMadeDir/SubFile.txt"));
    BOOST_REQUIRE(MadeSubDir.removeFile("MadeDir/ReMadeDir/SubFile.txt"));
    BOOST_REQUIRE(!MadeSubDir.exists("MadeDir/ReMadeDir/SubFile.txt"));
    BOOST_REQUIRE(!MadeSubDir.remove("MadeDir"));
    BOOST_REQUIRE(!MadeSubDir.isFile("MadeDir"));
    BOOST_REQUIRE(!MadeSubDir.isDirectory("MadeDir"));
    BOOST_REQUIRE(!MadeSubDir.exists("MadeDir"));
  }

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_fromstdpath)
{
  auto P = openfluid::tools::FilesystemPath::fromStdPath(std::filesystem::path(FSOutputPath));

  std::cout << "fromStdPath() : " << P.toGeneric() << std::endl;
  BOOST_REQUIRE_EQUAL(P.toGeneric(),FSOutputPath);
}

