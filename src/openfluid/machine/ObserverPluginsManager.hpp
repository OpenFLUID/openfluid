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
  \file ObserverPluginsManager.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OBSERVERPLUGINSMANAGER_HPP__
#define __OBSERVERPLUGINSMANAGER_HPP__

#include <openfluid/ware/PluggableObserver.hpp>
#include <openfluid/ware/ObserverSignature.hpp>
#include <openfluid/machine/WarePluginsManager.hpp>
#include <openfluid/base/RuntimeEnv.hpp>


namespace openfluid { namespace machine {

class ObserverSignatureInstance;
class ObserverInstance;


// =====================================================================
// =====================================================================

class DLLEXPORT ObserverPluginsManager : public WarePluginsManager<ObserverSignatureInstance,ObserverInstance,
                                                         openfluid::ware::GetPluggableObserverSignatureProc,
                                                         openfluid::ware::GetPluggableObserverBodyProc>
{

  private:

    static ObserverPluginsManager* mp_Singleton;

    ObserverPluginsManager() : WarePluginsManager()
    { };


  public:

    static ObserverPluginsManager* getInstance()
    {
      if (mp_Singleton == NULL) mp_Singleton = new ObserverPluginsManager();
       return mp_Singleton;
    }


    // =====================================================================
    // =====================================================================


    std::string getPluginFullPath(const std::string& Filename)
    {
      return openfluid::base::RuntimeEnvironment::getInstance()->getObserverPluginFullPath(Filename);
    }


    // =====================================================================
    // =====================================================================


    std::vector<std::string> getPluginsSearchPaths()
    {
      return openfluid::base::RuntimeEnvironment::getInstance()->getObserversPluginsPaths();
    }


    // =====================================================================
    // =====================================================================


    std::string getPluginFilenameSuffix()
    {
      return openfluid::config::OBSERVERS_PLUGINS_SUFFIX;
    }

};


} }  // namespaces


#endif /* __OBSERVERPLUGINSMANAGER_HPP__ */
