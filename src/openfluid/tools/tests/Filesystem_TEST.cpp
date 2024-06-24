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
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_filesystem


#include <set>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <boost/test/unit_test.hpp>

#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/global.hpp>

#include "FilesystemCommon.hpp"
#include "tests-config.hpp"


// =====================================================================
// =====================================================================


const std::string FSInputPath = CONFIGTESTS_INPUT_MISCDATA_DIR+"/Filesystem";
const std::string FSOutputPath = CONFIGTESTS_OUTPUT_DATA_DIR+"/Filesystem";


BOOST_AUTO_TEST_CASE(init)
{
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(FSOutputPath).removeDirectory());
  BOOST_REQUIRE(!openfluid::tools::FilesystemPath(FSOutputPath).exists());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_system_paths)
{
  auto hP = openfluid::tools::Filesystem::homePath();
  auto tP = openfluid::tools::Filesystem::tempPath();

  std::cout << "homePath(): " << hP << std::endl;
  std::cout << "tempPath(): " << tP << std::endl;

  BOOST_CHECK(hP.size() > 0);
  BOOST_CHECK(tP.size() > 0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_path_operations)
{

  for (const auto& Test : JoinedPaths)
  {
    BOOST_REQUIRE_EQUAL(openfluid::tools::Filesystem::joinPath(Test.Arguments),Test.Expected);
  }

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_path_absolute)
{

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


BOOST_AUTO_TEST_CASE(check_dirfiles_operations)
{
  const auto WorkDir = FSOutputPath+"/dirfiles"; 

  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).makeDirectory());


  // Files

  BOOST_REQUIRE(openfluid::tools::Filesystem::copyFile(CONFIGTESTS_SRC_DIR+"/CMakeLists.txt",
                                              WorkDir+"/CMakeLists_copied.txt"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("CMakeLists_copied.txt"));
  
  BOOST_REQUIRE(openfluid::tools::Filesystem::renameFile(WorkDir+"/CMakeLists_copied.txt",
                                                         WorkDir+"/CMakeLists_renamed.txt"));  
  BOOST_REQUIRE(!openfluid::tools::FilesystemPath(WorkDir).isFile("CMakeLists_copied.txt"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("CMakeLists_renamed.txt"));

  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).makeDirectory("ForRenamed"));
  BOOST_REQUIRE(openfluid::tools::Filesystem::renameFile(WorkDir+"/CMakeLists_renamed.txt",
                                                         WorkDir+"/ForRenamed/CMakeLists_renamed.txt"));
  BOOST_REQUIRE(!openfluid::tools::FilesystemPath(WorkDir).isFile("CMakeLists_Renamed.txt"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("ForRenamed/CMakeLists_renamed.txt"));

  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).removeFile("ForRenamed/CMakeLists_renamed.txt"));
  BOOST_REQUIRE(!openfluid::tools::FilesystemPath(WorkDir).isFile("ForRenamed/CMakeLists_renamed.txt"));


  // Directories

  BOOST_REQUIRE(openfluid::tools::Filesystem::copyDirectory(FSInputPath,WorkDir+"/DirCopyContent"));

  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopyContent/README"));

  BOOST_REQUIRE(!openfluid::tools::Filesystem::copyDirectory(FSInputPath,WorkDir+"/DirCopyContent"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::copyDirectory(FSInputPath,WorkDir+"/DirCopyContent",false, true));

  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopyContent/README"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopyContent/README.TOO"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopyContent/subdir/another_subdir/README"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopyContent/subdir2/README.TXT"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isDirectory("DirCopyContent/subdir/another_subdir"));

  openfluid::tools::Filesystem::emptyDirectory(WorkDir+"/DirCopyContent");

  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isDirectory("DirCopyContent"));

  BOOST_REQUIRE(!openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopyContent/README"));
  BOOST_REQUIRE(!openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopyContent/README.TOO"));
  BOOST_REQUIRE(!openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopyContent/subdir/another_subdir/README"));
  BOOST_REQUIRE(!openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopyContent/subdir2/README.TXT"));
  BOOST_REQUIRE(!openfluid::tools::FilesystemPath(WorkDir).isDirectory("DirCopyContent/subdir/another_subdir"));

  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).removeDirectory("DirCopyContent"));
  BOOST_REQUIRE(!openfluid::tools::FilesystemPath(WorkDir).isDirectory("DirCopyContent"));


  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).removeDirectory("DirCopy"));
  BOOST_REQUIRE(!openfluid::tools::FilesystemPath(WorkDir).isDirectory("DirCopy"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::copyDirectory(FSInputPath,WorkDir+"/DirCopy",true));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopy/Filesystem/README"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopy/Filesystem/README.TOO"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopy/Filesystem/subdir/another_subdir/README"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isDirectory("DirCopy/Filesystem/subdir/another_subdir"));


  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).removeDirectory("DirCopyContent"));
  BOOST_REQUIRE(!openfluid::tools::FilesystemPath(WorkDir).isDirectory("DirCopyContent"));

  BOOST_REQUIRE(openfluid::tools::Filesystem::copyDirectory(FSInputPath,WorkDir+"/DirCopyContent"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopyContent/README"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopyContent/README.TOO"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("DirCopyContent/subdir/another_subdir/README"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isDirectory("DirCopyContent/subdir/another_subdir"));


  openfluid::tools::FilesystemPath WorkDirFSP = openfluid::tools::FilesystemPath(WorkDir);
  BOOST_REQUIRE(WorkDirFSP.makeDirectory("_testDir"));
  BOOST_REQUIRE(WorkDirFSP.makeDirectory(".hidden"));
  BOOST_REQUIRE(WorkDirFSP.makeFile(".hidden/hiddenFile.txt"));
  BOOST_REQUIRE(WorkDirFSP.makeFile("file.json"));

  openfluid::tools::Filesystem::emptyDirectory(WorkDir, {openfluid::tools::Path({WorkDir, ".*"}).toGeneric(), 
                                                         openfluid::tools::Path({WorkDir, "_*"}).toGeneric()});

  BOOST_REQUIRE(WorkDirFSP.isDirectory(".hidden"));
  BOOST_REQUIRE(WorkDirFSP.isDirectory("_testDir"));

  BOOST_REQUIRE(WorkDirFSP.isFile(".hidden/hiddenFile.txt"));
  BOOST_REQUIRE(!WorkDirFSP.isFile("file.json"));

  //   merge dir content with existing

  //     emulate existing content
  std::filesystem::create_directory(WorkDir+"/ExistingDir");
  std::filesystem::create_directory(WorkDir+"/ExistingDir/subdir");
  std::ofstream(WorkDir+"/ExistingDir/EXISTING.txt").flush();
  std::ofstream(WorkDir+"/ExistingDir/subdir/SUBEXISTING.txt").flush();

  //    adding directory content
  BOOST_REQUIRE(openfluid::tools::Filesystem::copyDirectoryContent(FSInputPath,WorkDir+"/ExistingDir"));

  //    checking previous files
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("ExistingDir/EXISTING.txt"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("ExistingDir/subdir/SUBEXISTING.txt"));
  //    checking copied files
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("ExistingDir/README.TOO"));
  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).isFile("ExistingDir/subdir/another_subdir/README"));
  
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_unique_operations)
{

  const auto WorkDir = FSOutputPath+"/unique"; 

  // Unique subdirs and files

  std::set<std::string> UniqueDirs;
  std::string Dir;

  BOOST_REQUIRE(openfluid::tools::FilesystemPath(WorkDir).makeDirectory("MadeDirForUnique"));

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
