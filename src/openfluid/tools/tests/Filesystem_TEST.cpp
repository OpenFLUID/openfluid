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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_filesystem


#include <set>
#include <iostream>
#include <filesystem>

#include <boost/test/unit_test.hpp>

#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/global.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


const std::string FSInputPath = CONFIGTESTS_INPUT_MISCDATA_DIR+"/Filesystem";
const std::string FSOutputPath = CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem";


BOOST_AUTO_TEST_CASE(init)
{
  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(FSOutputPath));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::exists(FSOutputPath));
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
  std::cout << "System path separator is " << openfluid::tools::Filesystem::pathSeparator() << std::endl;
  std::cout << "System paths list separator is " << openfluid::tools::Filesystem::pathsListSeparator() << std::endl;


  // -------------------------------------------


  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::joinPath({"/my/joined","path/myfile.txt"}),
                      "/my/joined/path/myfile.txt");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::joinPath({"/my/joined",""}),
                        "/my/joined/");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::joinPath({"","path/myfile.txt"}),
                        "/path/myfile.txt");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::joinPath({"",""}),
                        "/");


  // -------------------------------------------


  // Unix style
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::removeTrailingSeparators("/"),"/");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::removeTrailingSeparators("///"),"/");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::removeTrailingSeparators("/my/path/"),"/my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::removeTrailingSeparators("/my/path"),"/my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::removeTrailingSeparators("/my/path//"),"/my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::removeTrailingSeparators("/my/path///////////////////"),"/my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::removeTrailingSeparators("my/path/"),"my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::removeTrailingSeparators("my/path"),"my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::removeTrailingSeparators("my/path//"),"my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::removeTrailingSeparators("my/path///////////////////"),"my/path");

  // Windows style
  BOOST_REQUIRE_EQUAL(
    openfluid::tools::Filesystem::removeTrailingSeparators("c:\\my\\path",'\\'),"c:\\my\\path"
  );
  BOOST_REQUIRE_EQUAL(
    openfluid::tools::Filesystem::removeTrailingSeparators("c:\\my\\path\\",'\\'),"c:\\my\\path"
  );
  BOOST_REQUIRE_EQUAL(
    openfluid::tools::Filesystem::removeTrailingSeparators("c:\\my\\path\\\\\\\\\\",'\\'),"c:\\my\\path"
  );
  BOOST_REQUIRE_EQUAL(
    openfluid::tools::Filesystem::removeTrailingSeparators("c:/my/path/////",'/'),"c:/my/path"
  );
  BOOST_REQUIRE_EQUAL(
    openfluid::tools::Filesystem::removeTrailingSeparators("my\\path\\\\\\\\\\",'\\'),"my\\path"
  );
  BOOST_REQUIRE_EQUAL(
    openfluid::tools::Filesystem::removeTrailingSeparators("e:\\\\\\",'\\'),"e:\\"
  );
  BOOST_REQUIRE_EQUAL(
    openfluid::tools::Filesystem::removeTrailingSeparators("S:\\\\\\",'\\'),"S:\\"
  );


  // -------------------------------------------


#if defined(OPENFLUID_OS_WINDOWS)

  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::toGenericPath("/my/path"),"/my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::toGenericPath("/my\\\\path//"),"/my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::toGenericPath("my\\\\path//\\\\"),"my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::toGenericPath("a:/my/path/"),"a:/my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::toGenericPath("A:/my\\\\path//"),"A:/my/path");  

  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::toNativePath("c:/my/path"),"c:\\my\\path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::toNativePath("c:/my\\\\path//"),"c:\\my\\path");

#elif defined(OPENFLUID_OS_UNIX)  

  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::toGenericPath("/my/path"),"/my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::toGenericPath("/my\\\\path//"),"/my\\\\path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::toGenericPath("my\\\\path//\\\\"),"my\\\\path/\\\\");

  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::toNativePath("/my/path"),"/my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::toNativePath("/my\\\\path//"),"/my\\\\path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::toNativePath("my\\\\path//\\\\"),"my\\\\path/\\\\");

#endif
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_names_operations)
{
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::filename("/my/path/myfile.txt"),"myfile.txt");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::filename("/my/other/path"),"path");

  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::dirname("/my/path/myfile.txt"),"/my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::dirname("/my/path/"),"/my/path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::dirname("/my/path"),"/my");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::dirname("/my/path.txt"),"/my");

  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::filename("file://url/like/path/file.txt"),"file.txt");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::filename("http://url/like/path/file.txt"),"file.txt");


  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::basename("/my/path/myfile.txt"),"myfile");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::basename("/my/other/path"),"path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::basename("/my/path/myarchive.tar.gz"),"myarchive.tar");
 
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::extension("/my/path/myfile.txt"),"txt");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::extension("/my/other/path"),"");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::extension("/my/path/myarchive.tar.gz"),"gz");

  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::minimalBasename("/my/other/path"),"path");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::minimalBasename("/my/path/myfile.txt"),"myfile");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::minimalBasename("/my/path/myarchive.tar.gz"),"myarchive");

  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::completeExtension("/my/other/path"),"");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::completeExtension("/my/path/myfile.txt"),"txt");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::completeExtension("/my/path/myarchive.tar.gz"),"tar.gz");


  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::minimalBasename("/my/path/myarchive.tar.gz")+"."+
                      openfluid::tools::Filesystem::completeExtension("/my/path/myarchive.tar.gz"),
                      openfluid::tools::Filesystem::basename("/my/path/myarchive.tar.gz")+"."+
                      openfluid::tools::Filesystem::extension("/my/path/myarchive.tar.gz"));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_path_clean_absolute_relative)
{
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::cleanPath("/my/path/to/directory/"),"/my/path/to/directory");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::cleanPath("/my/path//.//to/directory/"),"/my/path/to/directory");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::cleanPath("my/path//.//to/directory/"),"my/path/to/directory");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::cleanPath("my/path//.//to/../../directory///"),"my/directory");

#if !defined(OPENFLUID_OS_WINDOWS)
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::absolutePath("/my/path/to/directory/"),"/my/path/to/directory");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::absolutePath("/my/path/to/directory"),"/my/path/to/directory");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::absolutePath("/my/path/to/file.txt"),"/my/path/to/file.txt");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::absolutePath("/my/long/../path/to/file.txt"),
                                                                 "/my/path/to/file.txt");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::absolutePath("/"),"/");
  BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::absolutePath(""),"");
#endif

  std::cout << openfluid::tools::Filesystem::absolutePath("path/to/directory") << std::endl;
  BOOST_REQUIRE(
    openfluid::tools::Filesystem::absolutePath("path/to/directory")
      .rfind(openfluid::tools::Filesystem::currentPath(),0) == 0
  );
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_test_operations)
{
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_BINARY_DIR));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(CONFIGTESTS_BINARY_DIR));
  BOOST_REQUIRE(openfluid::tools::Filesystem::exists(CONFIGTESTS_BINARY_DIR));
  
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(CONFIGTESTS_SRC_DIR+"/CMakeLists.txt"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(CONFIGTESTS_SRC_DIR+"/CMakeLists.txt"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::exists(CONFIGTESTS_SRC_DIR+"/CMakeLists.txt"));

  BOOST_REQUIRE(!openfluid::tools::Filesystem::exists(CONFIGTESTS_SRC_DIR+"/doesnotexist.txt"));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_dirfiles_operations)
{
  const auto WorkDir = FSOutputPath+"/dirfiles"; 

  // Directories

  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(WorkDir+"/MadeDir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(WorkDir+"/MadeDir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(WorkDir+"/MadeDir"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(WorkDir+"/MadeDir"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(WorkDir+"/MadeDir/MadeSubdir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(WorkDir+"/MadeDir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(WorkDir+"/MadeDir/MadeSubdir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(WorkDir+"/MadeDir/MadeSubdir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(WorkDir+"/MadeDir"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(WorkDir+"/MadeDir/MadeSubdir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(WorkDir+"/MadeDir"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(WorkDir+"/MadeDir"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(WorkDir+"/MadeDir/MadeSubdir"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(WorkDir+"/MadeDir"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(WorkDir+"/MadeDir"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(WorkDir+"/MadeDir"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(WorkDir+"/MadeDir"));


  // Files

  BOOST_REQUIRE(openfluid::tools::Filesystem::copyFile(CONFIGTESTS_SRC_DIR+"/CMakeLists.txt",
                                              WorkDir+"/CMakeLists_copied.txt"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/CMakeLists_copied.txt"));
  
  BOOST_REQUIRE(openfluid::tools::Filesystem::renameFile(WorkDir+"/CMakeLists_copied.txt",
                                                         WorkDir+"/CMakeLists_renamed.txt"));  
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(WorkDir+"/CMakeLists_copied.txt"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/CMakeLists_renamed.txt"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(WorkDir+"/ForRenamed"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::renameFile(WorkDir+"/CMakeLists_renamed.txt",
                                                         WorkDir+"/ForRenamed/CMakeLists_renamed.txt"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(WorkDir+"/CMakeLists_Renamed.txt"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/ForRenamed/CMakeLists_renamed.txt"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::removeFile(WorkDir+"/ForRenamed/CMakeLists_renamed.txt"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(WorkDir+"/ForRenamed/CMakeLists_renamed.txt"));


  // directory copies

  BOOST_REQUIRE(openfluid::tools::Filesystem::copyDirectory(FSInputPath,WorkDir+"/DirCopyContent"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopyContent/README"));

  BOOST_REQUIRE(!openfluid::tools::Filesystem::copyDirectory(FSInputPath,WorkDir+"/DirCopyContent"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::copyDirectory(FSInputPath,WorkDir+"/DirCopyContent",false, true));

  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopyContent/README"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopyContent/README.TOO"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopyContent/subdir/another_subdir/README"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopyContent/subdir2/README.TXT"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(WorkDir+"/DirCopyContent/subdir/another_subdir"));

  openfluid::tools::Filesystem::emptyDirectory(WorkDir+"/DirCopyContent");

  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(WorkDir+"/DirCopyContent"));

  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopyContent/README"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopyContent/README.TOO"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopyContent/subdir/another_subdir/README"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopyContent/subdir2/README.TXT"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(WorkDir+"/DirCopyContent/subdir/another_subdir"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(WorkDir+"/DirCopyContent"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(WorkDir+"/DirCopyContent"));


  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(WorkDir+"/DirCopy"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(WorkDir+"/DirCopy"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::copyDirectory(FSInputPath,WorkDir+"/DirCopy",true));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopy/Filesystem/README"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopy/Filesystem/README.TOO"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopy/Filesystem/subdir/another_subdir/README"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(WorkDir+"/DirCopy/Filesystem/subdir/another_subdir"));


  BOOST_REQUIRE(openfluid::tools::Filesystem::removeDirectory(WorkDir+"/DirCopyContent"));
  BOOST_REQUIRE(!openfluid::tools::Filesystem::isDirectory(WorkDir+"/DirCopyContent"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::copyDirectory(FSInputPath,WorkDir+"/DirCopyContent"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopyContent/README"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopyContent/README.TOO"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isFile(WorkDir+"/DirCopyContent/subdir/another_subdir/README"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::isDirectory(WorkDir+"/DirCopyContent/subdir/another_subdir"));

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_unique_operations)
{

  const auto WorkDir = FSOutputPath+"/unique"; 

  // Unique subdirs and files

  std::set<std::string> UniqueDirs;
  std::string Dir;

  BOOST_REQUIRE(openfluid::tools::Filesystem::makeDirectory(WorkDir+"/MadeDirForUnique"));

  Dir = openfluid::tools::Filesystem::makeUniqueSubdirectory(WorkDir+"//MadeDirForUnique","uniquedir");
  std::cout << "Unique dir: " << Dir << std::endl;
  BOOST_REQUIRE(!Dir.empty());
  UniqueDirs.insert(Dir);
  BOOST_REQUIRE_EQUAL(UniqueDirs.size(),1);

  Dir = openfluid::tools::Filesystem::makeUniqueSubdirectory(WorkDir+"//MadeDirForUnique","uniquedir");
  BOOST_REQUIRE(!Dir.empty());
  UniqueDirs.insert(Dir);
  BOOST_REQUIRE_EQUAL(UniqueDirs.size(),2);

  Dir = openfluid::tools::Filesystem::makeUniqueSubdirectory(WorkDir+"//MadeDirForUnique","uniquedir");
  BOOST_REQUIRE(!Dir.empty());
  UniqueDirs.insert(Dir);
  BOOST_REQUIRE_EQUAL(UniqueDirs.size(),3);


  std::string File;

  File = openfluid::tools::Filesystem::makeUniqueFile(WorkDir+"/MadeDirForUnique","uniquefile.sh");
  BOOST_REQUIRE(!File.empty());

  File = openfluid::tools::Filesystem::makeUniqueFile(WorkDir+"/MadeDirForUnique","uniquefile.sh");
  BOOST_REQUIRE(!File.empty());

  File = openfluid::tools::Filesystem::makeUniqueFile(WorkDir+"/MadeDirForUnique","uniquefile.tar.gz");
  BOOST_REQUIRE(!File.empty());

  File = openfluid::tools::Filesystem::makeUniqueFile(WorkDir+"/MadeDirForUnique","uniquefile");
  BOOST_REQUIRE(!File.empty());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_findfiles_operations)
{
  // Only filenames

  const auto FoundFiles = openfluid::tools::Filesystem::findFiles(FSInputPath);
  const auto ExpFiles = {"file1.md","file1_sufx1.md","file1_sufx1.txt","file1_sufx2.md","file1_sufx2.txt","file1.txt",
                         "file2.md","file2_sufx2.md","README","README.TOO"};

  BOOST_REQUIRE_EQUAL(FoundFiles.size(),ExpFiles.size());

  for (const auto& Exp : ExpFiles)
  {
    BOOST_REQUIRE(std::find(FoundFiles.begin(),FoundFiles.end(),Exp) != FoundFiles.end());
  }


  // Full paths

  const auto FoundFilesWithPath = 
    openfluid::tools::Filesystem::findFiles(FSInputPath,true);
  std::vector<std::string> ExpFilesWithPath;
  for (const auto& Exp: ExpFiles)
  {
    ExpFilesWithPath.push_back(FSInputPath+"/"+Exp);
  }

  BOOST_REQUIRE_EQUAL(FoundFilesWithPath.size(),ExpFilesWithPath.size());

  for (const auto& Exp : ExpFilesWithPath)
  {
    BOOST_REQUIRE(std::find(FoundFilesWithPath.begin(),FoundFilesWithPath.end(),Exp) != FoundFilesWithPath.end());
  }


  // Only filenames, filtered by pattern

  const auto FoundFilesPattern = 
    openfluid::tools::Filesystem::findFiles(FSInputPath,false,".*sufx.*");
  const auto ExpFilesPattern = {"file1_sufx1.md","file1_sufx1.txt","file1_sufx2.md","file1_sufx2.txt","file2_sufx2.md"};

  BOOST_REQUIRE_EQUAL(FoundFilesPattern.size(),ExpFilesPattern.size());

  for (const auto& Exp : ExpFilesPattern)
  {
    BOOST_REQUIRE(std::find(FoundFilesPattern.begin(),FoundFilesPattern.end(),Exp) != FoundFilesPattern.end());
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_findfilesext_operations)
{
  // Only filenames

  const auto FoundFiles = 
    openfluid::tools::Filesystem::findFilesByExtension(FSInputPath,"md");
  const auto ExpFiles = {"file1.md","file1_sufx1.md","file1_sufx2.md","file2.md","file2_sufx2.md"};

  BOOST_REQUIRE_EQUAL(FoundFiles.size(),ExpFiles.size());

  for (const auto& Exp : ExpFiles)
  {
    BOOST_REQUIRE(std::find(FoundFiles.begin(),FoundFiles.end(),Exp) != FoundFiles.end());
  }


  // Full paths

  const auto FoundFilesWithPath = 
    openfluid::tools::Filesystem::findFilesByExtension(FSInputPath,"md",true);
  std::vector<std::string> ExpFilesWithPath;
  for (const auto& Exp: ExpFiles)
  {
    ExpFilesWithPath.push_back(FSInputPath+"/"+Exp);
  }

  BOOST_REQUIRE_EQUAL(FoundFilesWithPath.size(),ExpFilesWithPath.size());

  for (const auto& Exp : ExpFilesWithPath)
  {
    BOOST_REQUIRE(std::find(FoundFilesWithPath.begin(),FoundFilesWithPath.end(),Exp) != FoundFilesWithPath.end());
  }


  // Only filenames, with dot in ext

  const auto FoundFilesDot = 
    openfluid::tools::Filesystem::findFilesByExtension(FSInputPath,".md",false,true);

  BOOST_REQUIRE_EQUAL(FoundFilesDot.size(),ExpFiles.size());

  for (const auto& Exp : ExpFiles)
  {
    BOOST_REQUIRE(std::find(FoundFilesDot.begin(),FoundFilesDot.end(),Exp) != FoundFilesDot.end());
  }

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_findfilesextsuffix_operations)
{
  // Only filenames

  const auto FoundFiles = openfluid::tools::Filesystem::findFilesBySuffixAndExtension(FSInputPath,"sufx2","md");
  const auto ExpFiles = {"file1_sufx2.md","file2_sufx2.md"};

  BOOST_REQUIRE_EQUAL(FoundFiles.size(),ExpFiles.size());

  for (const auto& Exp : ExpFiles)
  {
    BOOST_REQUIRE(std::find(FoundFiles.begin(),FoundFiles.end(),Exp) != FoundFiles.end());
  }


  // Full paths

  const auto FoundFilesWithPath = 
    openfluid::tools::Filesystem::findFilesBySuffixAndExtension(FSInputPath,"sufx2","md",true);
  std::vector<std::string> ExpFilesWithPath;
  for (const auto& Exp: ExpFiles)
  {
    ExpFilesWithPath.push_back(FSInputPath+"/"+Exp);
  }

  BOOST_REQUIRE_EQUAL(FoundFilesWithPath.size(),ExpFilesWithPath.size());

  for (const auto& Exp : ExpFilesWithPath)
  {
    BOOST_REQUIRE(std::find(FoundFilesWithPath.begin(),FoundFilesWithPath.end(),Exp) != FoundFilesWithPath.end());
  }


  // Only filenames, with dot in ext

  const auto FoundFilesDot = 
    openfluid::tools::Filesystem::findFilesBySuffixAndExtension(FSInputPath,"sufx2",".md",false,true);

  BOOST_REQUIRE_EQUAL(FoundFilesDot.size(),ExpFiles.size());

  for (const auto& Exp : ExpFiles)
  {
    BOOST_REQUIRE(std::find(FoundFilesDot.begin(),FoundFilesDot.end(),Exp) != FoundFilesDot.end());
  }

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_finddirs_operations)
{
  // Only dirs names

  const auto FoundDirs = openfluid::tools::Filesystem::findDirectories(FSInputPath);
  const auto ExpDirs = {"subdir", "subdir2"};

  BOOST_REQUIRE_EQUAL(FoundDirs.size(),ExpDirs.size());

  for (const auto& Exp : ExpDirs)
  {
    BOOST_REQUIRE(std::find(FoundDirs.begin(),FoundDirs.end(),Exp) != FoundDirs.end());
  }


  // Full paths

  const auto FoundDirsWithPath = openfluid::tools::Filesystem::findDirectories(FSInputPath,true);
  std::vector<std::string> ExpDirsWithPath;
  for (const auto& Exp: ExpDirs)
  {
    ExpDirsWithPath.push_back(FSInputPath+"/"+Exp);
  }

  BOOST_REQUIRE_EQUAL(FoundDirsWithPath.size(),ExpDirsWithPath.size());

  for (const auto& Exp : ExpDirsWithPath)
  {
    BOOST_REQUIRE(std::find(FoundDirsWithPath.begin(),FoundDirsWithPath.end(),Exp) != FoundDirsWithPath.end());
  }


  // Only dirs names, filtered by pattern

  const auto FoundDirsPattern = openfluid::tools::Filesystem::findDirectories(FSInputPath,false,".*dir.+");
  const auto ExpDirsPattern = {"subdir2"};

  BOOST_REQUIRE_EQUAL(FoundDirsPattern.size(),ExpDirsPattern.size());

  for (const auto& Exp : ExpDirsPattern)
  {
    BOOST_REQUIRE(std::find(FoundDirsPattern.begin(),FoundDirsPattern.end(),Exp) != FoundDirsPattern.end());
  }

}
