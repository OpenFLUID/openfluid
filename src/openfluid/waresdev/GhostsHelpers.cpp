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
  @file GhostsHelpers.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <openfluid/waresdev/GhostsHelpers.hpp>
#include <openfluid/waresdev/SimulatorSignatureXMLReader.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/utils/InternalLogger.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace waresdev {


std::string OPENFLUID_API getGhostSimulatorPath(const std::string& ParentPath, const openfluid::ware::WareID_t& ID)
{
  return openfluid::tools::Path(
    {ParentPath,ID+openfluid::config::SIMULATORS_GHOSTS_SUFFIX+openfluid::config::GHOSTS_EXT}).toGeneric();
}


// =====================================================================
// =====================================================================


openfluid::tools::Path oldSignaturePath(const std::string& ParentPath, const openfluid::ware::WareID_t& ID)
{
  return openfluid::tools::Path({ParentPath,ID+".xml.old"});
}


// =====================================================================
// =====================================================================


bool migrateGhostSimulator(const std::string& ParentPath, const openfluid::ware::WareID_t& ID)
{
  auto OldPathObj = openfluid::tools::Path({ParentPath,ID+".xml"});

  if (!OldPathObj.isFile())
  {
    openfluid::utils::log::error("Ghost migration", 
                                 "Path is not a file: " + OldPathObj.toNative());
    return false;
  }
  openfluid::ware::SimulatorSignature Sign;
  if (SimulatorSignatureXMLReader::loadFromFile(OldPathObj.toGeneric(),Sign))
  {
    try
    {
      SimulatorSignatureSerializer().writeToJSONFile(Sign,getGhostSimulatorPath(ParentPath,ID));
      const auto OldPathRenamed = oldSignaturePath(ParentPath, ID).toNative();
      if (!openfluid::tools::Filesystem::renameFile(OldPathObj.toNative(), OldPathRenamed))
      {
        openfluid::utils::log::error("Ghost migration", 
          "Rename failed from "+OldPathObj.toNative()+" to "+OldPathRenamed);
      }
      return true;
    }
    catch(const std::exception& e)
    {
      std::cout << "Error during ghost simulator JSON write: " << e.what() << std::endl;
      openfluid::utils::log::error("Ghost migration", 
                                    "Error during ghost simulator JSON write: " + std::string(e.what()));
      return false;
    }
  }
  openfluid::utils::log::error("Ghost migration", 
                               "XML loading failure for file " + OldPathObj.toGeneric());
  return false;
}


} }  // namespaces
