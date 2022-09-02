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
 */


#ifndef __OPENFLUID_WARESDEV_WARESRCHELPERS_HPP__
#define __OPENFLUID_WARESDEV_WARESRCHELPERS_HPP__


#include <set>

#include <openfluid/waresdev/WareSrcEnquirer.hpp>


namespace openfluid { namespace waresdev {


/**
  Checks if a given user can do read operations on a ware based on the RO/RW lists
  @param[in] UserName the current user to check
  @param[in] ROUsers the set of users with read only access
  @param[in] RWUsers the set of users with read+write access
  @return true if user is allowed to access the corresponding ware
*/
inline bool hasUserAccess(const std::string& UserName, const std::set<std::string>& ROUsers, 
                   const std::set<std::string>& RWUsers={})
{
  std::set<std::string> Users = ROUsers;
  Users.insert(RWUsers.begin(), RWUsers.end());
  return Users.count("*") || Users.count(UserName); 
}


/**
  Checks if a given ware id is found in current workspace.
  Wrapper of WareSrcEnquirer function
  @param[in] WarePath the ware path
  @return true if ware found in workspace
*/
inline bool isWareInCurrentWorkspace(const std::string& WarePath)
{
  return openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(WarePath).IsInCurrentWorkspace;
}

} } // namespaces

#endif /* __OPENFLUID_WARESDEV_WARESRCHELPERS_HPP__ */
