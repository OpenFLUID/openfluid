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
  @file GhostsHelpers.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_WARESDEV_GHOSTSHELPERS_HPP__
#define __OPENFLUID_WARESDEV_GHOSTSHELPERS_HPP__


#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/tools/FilesystemPath.hpp>


namespace openfluid { namespace waresdev {


std::string OPENFLUID_API getGhostSimulatorPath(const std::string& ParentPath, const openfluid::ware::WareID_t& ID);

openfluid::tools::Path OPENFLUID_API oldSignaturePath(const std::string& ParentPath, 
                                                      const openfluid::ware::WareID_t& ID);

bool OPENFLUID_API migrateGhostSimulator(const std::string& ParentPath, const openfluid::ware::WareID_t& ID);


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_GHOSTSHELPERS_HPP__ */
