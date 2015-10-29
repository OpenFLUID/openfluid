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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/GhostSimulatorFileIO.hpp>


namespace openfluid { namespace machine {


std::vector<ModelItemSignatureInstance*>
SimulatorPluginsManager::getAvailableGhostsSignatures(const std::string& /*Pattern*/) const
{
  std::vector<ModelItemSignatureInstance*> PluginsContainers;
  std::vector<std::string> PluginsPaths = getPluginsSearchPaths();
  std::vector<std::string> GhostsFiles;
  std::vector<std::string> TmpFiles;
  unsigned int i,j;

  for (i=0;i<PluginsPaths.size();i++)
  {
    TmpFiles = openfluid::tools::findFilesBySuffixAndExtension(PluginsPaths[i],
                                                               openfluid::config::SIMULATORS_GHOSTS_SUFFIX,
                                                               openfluid::config::GHOSTS_EXT,true,true);
    for (j=0;j<TmpFiles.size();j++)
      GhostsFiles.push_back(TmpFiles[j]);
  }


  for (i=0;i<GhostsFiles.size();i++)
  {
    std::unique_ptr<openfluid::ware::SimulatorSignature> TmpSignature(new openfluid::ware::SimulatorSignature());
    if (openfluid::machine::GhostSimulatorFileIO::loadFromFile(GhostsFiles[i],*TmpSignature))
    {
      ModelItemSignatureInstance* CurrentGhost = new ModelItemSignatureInstance();
      CurrentGhost->Ghost = true;
      CurrentGhost->FileFullPath = GhostsFiles[i];
      CurrentGhost->Signature = std::move(TmpSignature);
      PluginsContainers.push_back(CurrentGhost);
    }
  }

  return PluginsContainers;
}


} }  // namespaces
