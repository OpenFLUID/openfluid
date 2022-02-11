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
  @file FilesystemCommon.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_TOOLSTESTS_FILESYSTEMCOMMON_HPP__
#define __OPENFLUID_TOOLSTESTS_FILESYSTEMCOMMON_HPP__


#include "tests-config.hpp"


// =====================================================================
// =====================================================================


template<class A,class E>
struct TestInfo
{
  A Arguments; 
  E Expected;
};

template<class A,class E>
using TestInfoList = std::vector<TestInfo<A,E>>;

template<class A>
using BoolTestInfoList = std::vector<A>;


// =====================================================================
// =====================================================================


const TestInfoList<std::vector<std::string>,std::string> JoinedPaths = {
  {{"/my/joined","path/myfile.txt"},"/my/joined/path/myfile.txt"},
  {{"/my/joined",""},"/my/joined/"},
  {{"","path/myfile.txt"},"/path/myfile.txt"},
  {{"",""},"/"}
};

// -----------------

const TestInfoList<std::string,std::vector<std::string>> SplittedPaths = {
  {"/my/path/myfile.txt",{"my","path","myfile.txt"}},
  {"/my/path/mydir/mysubdir/",{"my","path","mydir","mysubdir"}},
  {"//my///path////mydir////",{"my","path","mydir"}}
};

// -----------------

const BoolTestInfoList<std::pair<std::string,std::string>> ContainedPaths = {
  {"/my/path1","/my/path1/subdir"},
  {"/my/path2//","/my/path2/subdir"},
  {"/my/path3","/my/path3/subdir//"},
  {"/my/path4","/my/path4"},
  {"/my/path5/","/my/path5"},
  {"/my/path6","/my/path6/"},
  {"/my/path7","/my/path7/subdir/subsubdir/subsubsubdir/////"}
};

// -----------------

const TestInfoList<std::string,std::string> UnixPathsWithTrailing = {
  {"/","/"},
  {"///","/"},
  {"/my/path1/","/my/path1"},
  {"/my/path2","/my/path2"},
  {"/my/path3//","/my/path3"},
  {"/my/path4///////////////////","/my/path4"},
  {"my/path5/","my/path5"},
  {"my/path6","my/path6"},
  {"my/path7//","my/path7"},
  {"my/path8///////////////////","my/path8"}
};

const TestInfoList<std::pair<std::string,char>,std::string> WinPathsWithTrailing = {
  {{"c:\\my\\path1",'\\'},"c:\\my\\path1"},
  {{"c:\\my\\path2\\",'\\'},"c:\\my\\path2"},
  {{"c:\\my\\path3\\\\\\\\\\",'\\'},"c:\\my\\path3"},
  {{"c:/my/path4/////",'/'},"c:/my/path4"},
  {{"my\\path5\\\\\\\\\\",'\\'},"my\\path5"},
  {{"e:\\\\\\",'\\'},"e:\\"},
  {{"S:\\\\\\",'\\'},"S:\\"}
};


// -----------------

const TestInfoList<std::string,std::string> UnixPathsToGeneric = {
  {"/my/path1","/my/path1"},
  {"/my/path2/","/my/path2/"},
  {"/my\\\\path3/","/my\\\\path3/"},
  {"my\\\\path4/\\\\","my\\\\path4/\\\\"}
};

const TestInfoList<std::string,std::string> UnixPathsToGenericClean = {
  {"/my/path1","/my/path1"},
  {"/my/path2/","/my/path2"},
  {"/my\\\\path3//","/my\\\\path3"},
  {"my\\\\path4//\\\\","my\\\\path4/\\\\"}
};

const TestInfoList<std::string,std::string> UnixPathsToNative = {
  {"/my/path1","/my/path1"},
  {"/my\\\\path2//","/my\\\\path2//"},
  {"my\\\\path3//\\\\","my\\\\path3//\\\\"}
};

const TestInfoList<std::string,std::string> WinPathsToGeneric = {
  {"/my/path1","/my/path1"},
  {"/my\\\\path2//","/my/path2/"},
  {"my\\\\path3//\\\\","my/path3/"},
  {"a:/my/path4/","a:/my/path4/"},
  {"A:/my\\\\path5//","A:/my/path5/"}
};

const TestInfoList<std::string,std::string> WinPathsToGenericClean = {
  {"/my/path1","/my/path1"},
  {"/my\\\\path2//","/my/path2"},
  {"my\\\\path3//\\\\","my/path3"},
  {"a:/my/path4/","a:/my/path4"},
  {"A:/my\\\\path5//","A:/my/path5"}
};

const TestInfoList<std::string,std::string> WinPathsToNative = {
  {"c:/my/path1","c:\\my\\path1"},
  {"c:/my\\path2","c:\\my\\path2"}
};

// -----------------

const TestInfoList<std::string,std::vector<std::string>> PathsComponents = {
  // filename, dirname, basename, extension, minimalBasename, completeExtension
  {"/my/path/myfile.txt",{"myfile.txt","/my/path","myfile","txt","myfile","txt"}},
  {"/my/other/path",{"path","/my/other","path","","path",""}},
  {"/my/path/",{"","/my/path","","","",""}},
  {"/my/path/myarchive.tar.gz",{"myarchive.tar.gz","/my/path","myarchive.tar","gz","myarchive","tar.gz"}}/*,
  {"file:///url/like/path/file.txt",{"file.txt","file:/url/like/path","file","txt","file","txt"}},
  {"http://url/like/path/file.txt",{"file.txt","http:/url/like/path","file","txt","file","txt"}}*/
};

// -----------------

const TestInfoList<std::pair<std::string,std::string>,std::string> RelativePaths = {
  {{"/path/to/directory","/path/to"},"directory"},
  {{"/path/to/directory/subdirectory","/path/to"},"directory/subdirectory"}
};

// -----------------

const TestInfoList<std::string,bool> IsDirPaths = {
  {CONFIGTESTS_BINARY_DIR,true},
  {CONFIGTESTS_SRC_DIR+"/CMakeLists.txt",false}
};

const TestInfoList<std::string,bool> IsFilePaths = {
  {CONFIGTESTS_BINARY_DIR,false},
  {CONFIGTESTS_SRC_DIR+"/CMakeLists.txt",true}
};

const TestInfoList<std::string,bool> ExistsPaths = {
  {CONFIGTESTS_BINARY_DIR,true},
  {CONFIGTESTS_SRC_DIR+"/CMakeLists.txt",true},
  {CONFIGTESTS_SRC_DIR+"/doesnotexists.txt",false},
};


#endif /* __OPENFLUID_TOOLSTESTS_FILESYSTEMCOMMON_HPP__ */
