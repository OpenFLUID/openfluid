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
  @file SimulatorRegistry.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/fluidx/WareDescriptor.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/machine/GeneratorSignature.hpp>
#include <openfluid/utils/InternalLogger.hpp>


namespace openfluid { namespace machine {


OPENFLUID_SINGLETON_INITIALIZATION(SimulatorRegistry)


SimulatorRegistry::SimulatorRegistry():
  WareRegistry<openfluid::ware::SimulatorSignature>()
{

}


// =====================================================================
// =====================================================================


bool SimulatorRegistry::addWare(const openfluid::ware::WareID_t& ID)
{
  auto Man = openfluid::machine::SimulatorPluginsManager::instance();

  return WareRegistry<openfluid::ware::SimulatorSignature>::add(Man->loadPlugin(ID));
}


// =====================================================================
// =====================================================================


// TODO enable/disable discovering ghosts using a dedicated argument
void SimulatorRegistry::discoverWares(const std::string IDPattern)
{
  discoverWares(true,IDPattern);
}


// =====================================================================
// =====================================================================


void SimulatorRegistry::discoverWares(bool WithGhosts, const std::string IDPattern)
{
  auto Man = openfluid::machine::SimulatorPluginsManager::instance();

  clearWares();
  Man->unloadAll();

  auto CollectedPlugins = Man->loadPlugins(IDPattern);
  for (auto& Item : CollectedPlugins)
  {
    WareRegistry<openfluid::ware::SimulatorSignature>::add(std::move(Item));
  }

  if (WithGhosts)
  {
    auto CollectedGhosts = Man->getAvailableGhosts(IDPattern);
    for (auto& Item : CollectedGhosts)
    {
      WareRegistry<openfluid::ware::SimulatorSignature>::add(std::move(Item));
    }
  }
}


// =====================================================================
// =====================================================================


// TODO check if another returned value should be better
openfluid::ware::WareID_t SimulatorRegistry::addGenerator(const GeneratorSpecs& Specs)
{
  if (Specs.Method != openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::NONE &&
      (Specs.VarTriplets.size() > 0))
  {
    
    auto Container = createGeneratorContainer();
    auto Signature = new openfluid::machine::GeneratorSignature(Specs.Method,
                                                                Specs.VarTriplets,
                                                                Specs.VariableType,
                                                                Specs.VariableDimensions);  

    Container.setSignature(Signature);
    Container.validate();

    auto it = m_Generators.find(Container.signature()->ID);
    if (it != m_Generators.end())
    {
      m_Generators.erase(it);
    }
    m_Generators.emplace(Container.signature()->ID,std::move(Container));

    return Signature->ID;
  }

  openfluid::utils::log::warning("Generator", "A ware without ID have been created");

  return openfluid::ware::WareID_t();
}


// =====================================================================
// =====================================================================


const WareContainer<openfluid::ware::SimulatorSignature>& 
SimulatorRegistry::generatorContainer(const openfluid::ware::WareID_t& ID) const
{
  auto it = m_Generators.find(ID);

  if (it != m_Generators.end())
  {
    return it->second;
  }

  return m_InvalidWareContainer;
}


// =====================================================================
// =====================================================================


const WareContainer<openfluid::ware::SimulatorSignature>& 
SimulatorRegistry::wareOrGeneratorContainer(const openfluid::ware::WareID_t& ID) const
{
  auto itS = m_AvailableWares.find(ID);

  if (itS != m_AvailableWares.end())
  {
    return itS->second;
  }
  else
  {
    auto itG = m_Generators.find(ID);

    if (itG != m_Generators.end())
    {
      return itG->second;
    }
  }

  return m_InvalidWareContainer;
}


} } //namespaces
