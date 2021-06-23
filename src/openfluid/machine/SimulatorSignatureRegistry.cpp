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
  @file SimulatorSignatureRegistry.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/fluidx/WareDescriptor.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/ware/GeneratorSignature.hpp>


namespace openfluid { namespace machine {


OPENFLUID_SINGLETON_INITIALIZATION(SimulatorSignatureRegistry)


SimulatorSignatureRegistry::SimulatorSignatureRegistry():
  WareSignatureRegistry<ModelItemSignatureInstance>()
{
  openfluid::machine::ModelItemSignatureInstance* FixedSignature = new openfluid::machine::ModelItemSignatureInstance();
  FixedSignature->Verified = true;
  FixedSignature->Signature =
    new openfluid::ware::GeneratorSignature(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED);
  addGeneratorSignature(FixedSignature);
  m_GenSignatures[openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED] = FixedSignature;

  openfluid::machine::ModelItemSignatureInstance* RandomSignature =
    new openfluid::machine::ModelItemSignatureInstance();
  RandomSignature->Verified = true;
  RandomSignature->Signature =
    new openfluid::ware::GeneratorSignature(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM);
  addGeneratorSignature(RandomSignature);
  m_GenSignatures[openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM] = RandomSignature;

  openfluid::machine::ModelItemSignatureInstance* InterpSignature =
    new openfluid::machine::ModelItemSignatureInstance();
  InterpSignature->Verified = true;
  InterpSignature->Signature =
    new openfluid::ware::GeneratorSignature(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP);
  addGeneratorSignature(InterpSignature);
  m_GenSignatures[openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP] = InterpSignature;

  openfluid::machine::ModelItemSignatureInstance* InjectSignature =
    new openfluid::machine::ModelItemSignatureInstance();
  InjectSignature->Verified = true;
  InjectSignature->Signature =
    new openfluid::ware::GeneratorSignature(openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INJECT);
  addGeneratorSignature(InjectSignature);
  m_GenSignatures[openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INJECT] = InjectSignature;

  update();
}


// =====================================================================
// =====================================================================


SimulatorSignatureRegistry::~SimulatorSignatureRegistry()
{

}


// =====================================================================
// =====================================================================


void SimulatorSignatureRegistry::addSimulatorSignature(openfluid::machine::ModelItemSignatureInstance* Signature)
{
  if (Signature->Signature)
  {
    Signature->ItemType = openfluid::ware::WareType::SIMULATOR;

    m_SimSignatures[openfluid::ware::WareType::SIMULATOR][Signature->Signature->ID] = Signature;
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
        "trying to register a simulator signature with no signature instantiated");
  }
}


// =====================================================================
// =====================================================================


void SimulatorSignatureRegistry::addGeneratorSignature(openfluid::machine::ModelItemSignatureInstance* Signature)
{
  if (Signature->Signature)
  {
    Signature->ItemType = openfluid::ware::WareType::GENERATOR;

    m_SimSignatures[openfluid::ware::WareType::GENERATOR][Signature->Signature->ID] = Signature;
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
        "Trying to register a generator signature with no signature instantiated");
  }
}


// =====================================================================
// =====================================================================


void SimulatorSignatureRegistry::update()
{
  unloadAll();
  
  // searching for pluggable simulators
  const std::vector<openfluid::machine::ModelItemSignatureInstance*> Signatures =
      openfluid::machine::SimulatorPluginsManager::instance()->getAvailableWaresSignatures().availablePlugins();

  for (auto SignInst: Signatures)
  {
    if (SignInst->Signature)
    {
      addSimulatorSignature(SignInst);
    }
  }

  openfluid::machine::SimulatorPluginsManager::instance()->unloadAllWares();

  // searching for ghosts simulators

  std::vector<openfluid::machine::ModelItemSignatureInstance*> GhostsSignatures =
        openfluid::machine::SimulatorPluginsManager::instance()->getAvailableGhostsSignatures();

  for (auto SignInst: GhostsSignatures)
  {
    if (SignInst->Signature &&
        m_SimSignatures[openfluid::ware::WareType::SIMULATOR].find(SignInst->Signature->ID)
          == m_SimSignatures[openfluid::ware::WareType::SIMULATOR].end())
    {
      addSimulatorSignature(SignInst);
    }
  }
}


// =====================================================================
// =====================================================================


const SimulatorSignatureRegistry::SimSignaturesByTypeByName_t& SimulatorSignatureRegistry::getSimSignatures() const
{
  return m_SimSignatures;
}


// =====================================================================
// =====================================================================


const SimulatorSignatureRegistry::SimSignaturesByName_t& SimulatorSignatureRegistry::getGeneratorSignatures() const
{
  return m_SimSignatures.at(openfluid::ware::WareType::GENERATOR);
}


// =====================================================================
// =====================================================================


const SimulatorSignatureRegistry::SimSignaturesByName_t& SimulatorSignatureRegistry::getSimulatorSignatures() const
{
  return m_SimSignatures.at(openfluid::ware::WareType::SIMULATOR);
}


// =====================================================================
// =====================================================================


bool SimulatorSignatureRegistry::isSimulatorAvailable(const openfluid::ware::WareID_t& ID) const
{
  return m_SimSignatures.at(openfluid::ware::WareType::SIMULATOR).count(ID);
}


// =====================================================================
// =====================================================================


const openfluid::machine::ModelItemSignatureInstance*
  SimulatorSignatureRegistry::signature(const openfluid::ware::WareID_t& ID) const
{
  if (isSimulatorAvailable(ID))
  {
    return m_SimSignatures.at(openfluid::ware::WareType::SIMULATOR).at(ID);
  }

  if (m_SimSignatures.at(openfluid::ware::WareType::GENERATOR).count(ID))
  {
    return m_SimSignatures.at(openfluid::ware::WareType::GENERATOR).at(ID);
  }

  return nullptr;
}


// =====================================================================
// =====================================================================


const ModelItemSignatureInstance*
  SimulatorSignatureRegistry::signature(openfluid::fluidx::ModelItemDescriptor* Item) const
{
  openfluid::ware::WareID_t ItemID = "";

  if (Item->isType(openfluid::ware::WareType::SIMULATOR))
  {
    ItemID = (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(Item))->getID();
  }
  else if (Item->isType(openfluid::ware::WareType::GENERATOR))
  {
    ItemID = openfluid::fluidx::GeneratorDescriptor::getGeneratorName(
                 (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(Item))->getGeneratorMethod());
  }

  return signature(ItemID);
}


// =====================================================================
// =====================================================================


const ModelItemSignatureInstance*
  SimulatorSignatureRegistry::signature(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method) const
{
  return m_GenSignatures.at(Method);
}


// =====================================================================
// =====================================================================


void SimulatorSignatureRegistry::unloadAll()
{
  for (auto Sign : m_SimSignatures[openfluid::ware::WareType::SIMULATOR])
  {
    delete Sign.second;
  }
  
  m_SimSignatures[openfluid::ware::WareType::SIMULATOR].clear();

  openfluid::machine::SimulatorPluginsManager::instance()->unloadAllWares();
}


} } //namespaces
