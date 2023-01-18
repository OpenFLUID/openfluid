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
 */


#ifndef __OPENFLUID_WARESDEV_WARESRCHELPERS_HPP__
#define __OPENFLUID_WARESDEV_WARESRCHELPERS_HPP__


#include <set>
#include <vector>
#include <string>

#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace waresdev {


// TOIMPL reuse this wherever possible 
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
  Tries to detect the OpenFLUID compatibility version of the sources. Currently, 
  it tries to distinguish version 2.2.x from versions below.
  @param[in] PathObj The path to the sources to detect
  @return The numeric representation of the detected OpenFLUID version (see below), 0 if version cannot be detected.
          Returned value  | OpenFLUID version
          --------------- | -----------------
          202000          | 2.2.0
          201000          | 2.1.x or below
*/
unsigned int OPENFLUID_API tryDetectWareSrcVersion(const openfluid::tools::FilesystemPath& PathObj);


// =====================================================================
// =====================================================================


/**
  Checks if a given user can do read operations on a ware based on the RO/RW lists
  @param[in] UserName the current user to check
  @param[in] ROUsers the set of users with read only access
  @param[in] RWUsers the set of users with read+write access
  @return true if user is allowed to access the corresponding ware
*/
bool OPENFLUID_API hasUserAccess(const std::string& UserName, const std::set<std::string>& ROUsers, 
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
