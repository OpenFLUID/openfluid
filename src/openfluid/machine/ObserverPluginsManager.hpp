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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_MACHINE_OBSERVERPLUGINSMANAGER_HPP__
#define __OPENFLUID_MACHINE_OBSERVERPLUGINSMANAGER_HPP__


#include <openfluid/ware/PluggableObserver.hpp>
#include <openfluid/ware/ObserverSignature.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/WarePluginsManager.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/utils/SingletonMacros.hpp>


namespace openfluid { namespace machine {


class OPENFLUID_API ObserverPluginsManager : 
  public WarePluginsManager<openfluid::ware::ObserverSignature,openfluid::ware::PluggableObserver,
                            openfluid::ware::GetPluggableObserverSignatureProc,
                            openfluid::ware::GetPluggableObserverBodyProc>
{

  OPENFLUID_SINGLETON_DEFINITION(ObserverPluginsManager)


  private:

    ObserverPluginsManager() : WarePluginsManager<openfluid::ware::ObserverSignature,openfluid::ware::PluggableObserver,
                                                  openfluid::ware::GetPluggableObserverSignatureProc,
                                                  openfluid::ware::GetPluggableObserverBodyProc>()
    { }


    ~ObserverPluginsManager()
    { }


    // =====================================================================
    // =====================================================================


    WareContainer<openfluid::ware::ObserverSignature> createContainer() const
    {
      return WareContainer<openfluid::ware::ObserverSignature>(openfluid::ware::WareType::OBSERVER);
    }


    // =====================================================================
    // =====================================================================


  public:

    std::string getPluginFullPath(const std::string& Filename) const
    {
      return openfluid::base::Environment::getObserverFullPath(Filename);
    }


    // =====================================================================
    // =====================================================================


    std::vector<std::string> getPluginsSearchPaths() const
    {
      return openfluid::base::Environment::getObserversDirs();
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
