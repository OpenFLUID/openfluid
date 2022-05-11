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
  @file ObserverRegistry.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Aline LIBRES <aline.libres@gmail.com>
*/


#include <openfluid/machine/ObserverRegistry.hpp>
#include <openfluid/machine/ObserverPluginsManager.hpp>
#include <openfluid/machine/ObserverInstance.hpp>


namespace openfluid { namespace machine {


OPENFLUID_SINGLETON_INITIALIZATION(ObserverRegistry)


ObserverRegistry::ObserverRegistry() : WareRegistry<openfluid::ware::ObserverSignature>()
{

}


// =====================================================================
// =====================================================================


bool ObserverRegistry::addWare(const openfluid::ware::WareID_t& ID)
{
  auto Man = openfluid::machine::ObserverPluginsManager::instance();

  return WareRegistry<openfluid::ware::ObserverSignature>::add(Man->loadPlugin(ID));
}


// =====================================================================
// =====================================================================


void ObserverRegistry::discoverWares(const std::string IDPattern)
{
  auto Man = openfluid::machine::ObserverPluginsManager::instance();

  clearWares();
  Man->unloadAll();

  std::vector<WareContainer<openfluid::ware::ObserverSignature>> Collected = Man->loadPlugins(IDPattern);

  for (auto& Item : Collected)
  {
    WareRegistry<openfluid::ware::ObserverSignature>::add(std::move(Item));
  }
}


} } /* namespaces */

