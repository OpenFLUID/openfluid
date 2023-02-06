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
  @file WareSrcHelpers.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
 */


#include <set>
#include <vector>
#include <string>
#include <algorithm>

#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/waresdev/WareSrcEnquirer.hpp>
#include <openfluid/waresdev/WareSrcHelpers.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace waresdev {


bool IsCppFile(const openfluid::tools::FilesystemPath& FileObj)
{
  return (FileObj.isFile() && 
          (std::find(std::begin(CppFilesExt), std::end(CppFilesExt), FileObj.extension()) != std::end(CppFilesExt)));
}


// =====================================================================
// =====================================================================


bool IsCMakeFile(const openfluid::tools::FilesystemPath& FileObj)
{
  return (FileObj.isFile() && 
          (FileObj.filename() == openfluid::config::WARESDEV_SRC_CMAKESTDFILE || FileObj.extension() == "cmake"));
}


// =====================================================================
// =====================================================================


bool hasUserAccess(const std::string& UserName, const std::set<std::string>& ROUsers, 
                   const std::set<std::string>& RWUsers)
{
  std::set<std::string> Users = ROUsers;
  Users.insert(RWUsers.begin(), RWUsers.end());
  return Users.count("*") || Users.count(UserName); 
}


// =====================================================================
// =====================================================================


bool isWareInCurrentWorkspace(const std::string& WarePath)
{
  return openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(WarePath).IsInCurrentWorkspace;
}


// =====================================================================
// =====================================================================


std::map<std::string,std::string> initializeConfigureVariables()
{
  std::map<std::string,std::string> Vars;

  const char* ChOpenFLUIDInstallPrefix = std::getenv("OPENFLUID_INSTALL_PREFIX");
  if (ChOpenFLUIDInstallPrefix)
  {
    Vars["OpenFLUID_DIR"] = 
      openfluid::tools::Filesystem::joinPath({std::string(ChOpenFLUIDInstallPrefix),"lib","cmake"});
  }

  return Vars;
}


} } // namespaces
