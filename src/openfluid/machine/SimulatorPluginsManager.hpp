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
  @file SimulatorPluginsManager.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_MACHINE_SIMULATORPLUGINSMANAGER_HPP__
#define __OPENFLUID_MACHINE_SIMULATORPLUGINSMANAGER_HPP__

#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/machine/WarePluginsManager.hpp>
#include <openfluid/base/RuntimeEnv.hpp>


namespace openfluid { namespace machine {

class ModelItemSignatureInstance;
class ModelItemInstance;


// =====================================================================
// =====================================================================

class OPENFLUID_API SimulatorPluginsManager : public WarePluginsManager<ModelItemSignatureInstance,ModelItemInstance,
                                                         openfluid::ware::GetPluggableSimulatorSignatureProc,
                                                         openfluid::ware::GetPluggableSimulatorBodyProc>
{

  private:

    static SimulatorPluginsManager* mp_Singleton;

    SimulatorPluginsManager() : WarePluginsManager<ModelItemSignatureInstance,
                                                   ModelItemInstance,
                                                   openfluid::ware::GetPluggableSimulatorSignatureProc,
                                                   openfluid::ware::GetPluggableSimulatorBodyProc>()
    { };


  public:

    static SimulatorPluginsManager* instance()
    {
      if (mp_Singleton == NULL) mp_Singleton = new SimulatorPluginsManager();
       return mp_Singleton;
    }


    // =====================================================================
    // =====================================================================


    std::string getPluginFullPath(const std::string& Filename)
    {
      return openfluid::base::RuntimeEnvironment::instance()->getSimulatorPluginFullPath(Filename);
    }


    // =====================================================================
    // =====================================================================


    std::vector<std::string> getPluginsSearchPaths()
    {
      return openfluid::base::RuntimeEnvironment::instance()->getSimulatorsPluginsPaths();
    }


    // =====================================================================
    // =====================================================================


    std::string getPluginFilenameSuffix()
    {
      return openfluid::config::SIMULATORS_PLUGINS_SUFFIX;
    }

};


} }  // namespaces

#endif /* __OPENFLUID_MACHINE_SIMULATORPLUGINSMANAGER_HPP__ */
