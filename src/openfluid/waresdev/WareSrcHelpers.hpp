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
  @file WareSrcHelpers.hpp

  @author Armel THÖNI <armel.thoni@inra.fr>
  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
 */


#ifndef __OPENFLUID_WARESDEV_WARESRCHELPERS_HPP__
#define __OPENFLUID_WARESDEV_WARESRCHELPERS_HPP__


#include <set>
#include <vector>
#include <string>
#include <regex>

#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace waresdev {


class OPENFLUID_API WarePurgeHandler
{

  private:

    std::string m_BuildDirRegexStr = "_build-";
    std::string ModeArg = "(release|debug)";
    std::string VersionArg = "[0-9]+\\.[0-9]+";
    std::string VersionMajorMinorStr;
    std::string CurrentVersionRegex;
    

  public:

    WarePurgeHandler(bool CurrentVersion, bool OtherVersions, bool ReleaseMode, bool DebugMode)
    {

      VersionMajorMinorStr = openfluid::base::Environment::getVersionMajorMinor();
      CurrentVersionRegex = openfluid::tools::replace(VersionMajorMinorStr, ".", "\\.");

      if (CurrentVersion && !OtherVersions)  // Current version only
      {
        VersionArg = CurrentVersionRegex;
      }
      else if (!CurrentVersion && OtherVersions)  // Other versions only
      {
        VersionArg = "(?!"+CurrentVersionRegex+")[0-9]+\\.[0-9]+";
      }

      if (ReleaseMode && !DebugMode)  // Release mode only
      {
        ModeArg = "release";
      }
      else if (!ReleaseMode && DebugMode) // Debug mode only
      {
        ModeArg = "debug";
      }

      m_BuildDirRegexStr = m_BuildDirRegexStr + ModeArg + "-" + VersionArg;
    }


    // =====================================================================
    // =====================================================================


    void purge(openfluid::tools::FilesystemPath WarePath, 
               std::function<void(std::string, std::string)> WriteMessageFunc,
               std::function<void(bool)> EmitSignal)
    { 
      std::regex FilterRegExp(m_BuildDirRegexStr);

      if(!WarePath.exists())
      {
        WriteMessageFunc(WarePath.toGeneric() + ": Ware path does not exists ", "Error");
        return;
      }

      for (const auto& Entry : std::filesystem::directory_iterator(WarePath.stdPath()))
      {
        auto EntryFSP = openfluid::tools::FilesystemPath(Entry.path().string());
        if (std::regex_match(EntryFSP.filename(), FilterRegExp)) 
        {
          EntryFSP.removeDirectory();

          if(EntryFSP.isDirectory())
          {
            WriteMessageFunc(EntryFSP.filename() + ": Error ", "Error");
            EmitSignal(false);
          }
          else
          {
            WriteMessageFunc(EntryFSP.filename() + ": Deleted ", "Success");
            EmitSignal(true);
          }
        }
      }
    }

    ~WarePurgeHandler()
    {

    }
};


// TODO reuse this wherever possible 
inline static const std::vector<std::string> CppFilesExt = {"cpp","hpp","cc","hh","cxx","hxx","C","H","h"};


// =====================================================================
// =====================================================================


/**
  Returns true if the given file path is a C++ source file (based on its extension)
  @param[in] FileObj The file path
  @return true if the file extension is a C++ extension
*/
bool OPENFLUID_API IsCppFile(const openfluid::tools::FilesystemPath& FileObj);


// =====================================================================
// =====================================================================


/**
  Returns true if the given file path is a CMake file (based on its name and/or extension)
  @param[in] FileObj The file path
  @return true if the file a CMake file
*/
bool OPENFLUID_API IsCMakeFile(const openfluid::tools::FilesystemPath& FileObj);


// =====================================================================
// =====================================================================


/**
  Checks if a given user can do read operations on a ware based on the RO/RW lists
  @param[in] UserName the current user to check
  @param[in] IsLoggedIn true if logged in to hub
  @param[in] ROUsers the set of users with read only access
  @param[in] RWUsers the set of users with read+write access
  @return true if user is allowed to access the corresponding ware
*/
bool OPENFLUID_API hasUserAccess(const std::string& UserName, bool IsLoggedIn, 
                                 const std::set<std::string>& ROUsers, 
                                 const std::set<std::string>& RWUsers={});


// =====================================================================
// =====================================================================


/**
  Checks if a given ware id is found in current workspace.
  Wrapper of WareSrcEnquirer function
  @param[in] WarePath the ware path
  @return true if ware found in workspace
*/
bool OPENFLUID_API isWareInCurrentWorkspace(const std::string& WarePath);


// =====================================================================
// =====================================================================


/**
  Initializes a map of CMake variables according to the OpenFLUID installation context
  @return a map of variables
*/
std::map<std::string,std::string> OPENFLUID_API initializeConfigureVariables();


} } // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESRCHELPERS_HPP__ */
