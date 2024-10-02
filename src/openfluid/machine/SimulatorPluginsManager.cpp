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
  @file SimulatorPluginsManager.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/utils/InternalLogger.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>


namespace openfluid { namespace machine {


OPENFLUID_SINGLETON_INITIALIZATION(SimulatorPluginsManager)


// =====================================================================
// =====================================================================


std::vector<WareContainer<openfluid::ware::SimulatorSignature>>
SimulatorPluginsManager::getAvailableGhosts(const std::string& IDPattern) const
{
  std::vector<WareContainer<openfluid::ware::SimulatorSignature>> Containers;
  std::vector<std::string> PluginsPaths = getPluginsSearchPaths();
  std::vector<std::string> GhostsFiles;
  std::vector<std::string> TmpFiles;
  unsigned int i,j;

  for (i=0;i<PluginsPaths.size();i++)
  {
    TmpFiles = openfluid::tools::Filesystem::findFilesBySuffixAndExtension(PluginsPaths[i],
                                                                           openfluid::config::SIMULATORS_GHOSTS_SUFFIX,
                                                                           openfluid::config::GHOSTS_EXT,
                                                                           true,true);
    for (j=0;j<TmpFiles.size();j++)
    {
      GhostsFiles.push_back(TmpFiles[j]);
    }
  }

  for (i=0;i<GhostsFiles.size();i++)
  {
    try
    {
      openfluid::ware::SimulatorSignature* TmpSignature = new openfluid::ware::SimulatorSignature();
      
      *TmpSignature = openfluid::waresdev::SimulatorSignatureSerializer().readFromJSONFile(GhostsFiles[i]);
      if (IDPattern.empty() || openfluid::tools::matchWithWildcard(IDPattern,TmpSignature->ID))
      {
        auto CurrentGhost = createContainer();
        CurrentGhost.setGhost();
        CurrentGhost.setPath(GhostsFiles[i]);
        CurrentGhost.setSignature(TmpSignature);
        CurrentGhost.validate();
        Containers.push_back(std::move(CurrentGhost));
      }
      else
      {
        delete TmpSignature;
      }

    }
    catch (std::exception& E)
    {
      openfluid::utils::log::error("Ghosts", E.what());
    }
   
  }

  return Containers;
}


} }  // namespaces
