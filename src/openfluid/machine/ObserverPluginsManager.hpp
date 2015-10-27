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
  @file ObserverPluginsManager.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_MACHINE_OBSERVERPLUGINSMANAGER_HPP__
#define __OPENFLUID_MACHINE_OBSERVERPLUGINSMANAGER_HPP__


#include <openfluid/ware/PluggableObserver.hpp>
#include <openfluid/ware/ObserverSignature.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/WarePluginsManager.hpp>
#include <openfluid/base/RuntimeEnv.hpp>


namespace openfluid { namespace machine {


class OPENFLUID_API ObserverPluginsManager : public WarePluginsManager<ObserverSignatureInstance,ObserverInstance,
                                                         openfluid::ware::GetPluggableObserverSignatureProc,
                                                         openfluid::ware::GetPluggableObserverBodyProc>
{

  private:

    static ObserverPluginsManager* mp_Singleton;

    ObserverPluginsManager() : WarePluginsManager<ObserverSignatureInstance,
                                                  ObserverInstance,
                                                  openfluid::ware::GetPluggableObserverSignatureProc,
                                                  openfluid::ware::GetPluggableObserverBodyProc>()
    { };


  public:

    static ObserverPluginsManager* instance()
    {
      if (mp_Singleton == nullptr) mp_Singleton = new ObserverPluginsManager();
       return mp_Singleton;
    }


    // =====================================================================
    // =====================================================================


    std::string getPluginFullPath(const std::string& Filename) const
    {
      return openfluid::base::RuntimeEnvironment::instance()->getObserverPluginFullPath(Filename);
    }


    // =====================================================================
    // =====================================================================


    std::vector<std::string> getPluginsSearchPaths() const
    {
      return openfluid::base::RuntimeEnvironment::instance()->getObserversPluginsPaths();
    }


    // =====================================================================
    // =====================================================================


    std::string getPluginFilenameSuffix() const
    {
      return openfluid::config::OBSERVERS_PLUGINS_SUFFIX;
    }

};


} }  // namespaces


#endif /* __OPENFLUID_MACHINE_OBSERVERPLUGINSMANAGER_HPP__ */
