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
  @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
*/


#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/fluidx/WareDescriptor.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/ware/GeneratorSignature.hpp>


namespace openfluid { namespace machine {


SimulatorSignatureRegistry* SimulatorSignatureRegistry::mp_Instance = 0;


// =====================================================================
// =====================================================================


SimulatorSignatureRegistry::SimulatorSignatureRegistry():
  WareSignatureRegistry<ModelItemSignatureInstance>()
{
  openfluid::machine::ModelItemSignatureInstance* FixedSignature = new openfluid::machine::ModelItemSignatureInstance();
  FixedSignature->Verified = true;
  FixedSignature->Signature = new openfluid::ware::GeneratorSignature(openfluid::fluidx::GeneratorDescriptor::Fixed);
  addGeneratorSignature(FixedSignature);
  m_GenSignatures[openfluid::fluidx::GeneratorDescriptor::Fixed] = FixedSignature;

  openfluid::machine::ModelItemSignatureInstance* RandomSignature =
      new openfluid::machine::ModelItemSignatureInstance();
  RandomSignature->Verified = true;
  RandomSignature->Signature = new openfluid::ware::GeneratorSignature(openfluid::fluidx::GeneratorDescriptor::Random);
  addGeneratorSignature(RandomSignature);
  m_GenSignatures[openfluid::fluidx::GeneratorDescriptor::Random] = RandomSignature;

  openfluid::machine::ModelItemSignatureInstance* InterpSignature =
      new openfluid::machine::ModelItemSignatureInstance();
  InterpSignature->Verified = true;
  InterpSignature->Signature = new openfluid::ware::GeneratorSignature(openfluid::fluidx::GeneratorDescriptor::Interp);
  addGeneratorSignature(InterpSignature);
  m_GenSignatures[openfluid::fluidx::GeneratorDescriptor::Interp] = InterpSignature;

  openfluid::machine::ModelItemSignatureInstance* InjectSignature =
      new openfluid::machine::ModelItemSignatureInstance();
  InjectSignature->Verified = true;
  InjectSignature->Signature = new openfluid::ware::GeneratorSignature(openfluid::fluidx::GeneratorDescriptor::Inject);
  addGeneratorSignature(InjectSignature);
  m_GenSignatures[openfluid::fluidx::GeneratorDescriptor::Inject] = InjectSignature;

  update();
}


// =====================================================================
// =====================================================================


SimulatorSignatureRegistry* SimulatorSignatureRegistry::instance()
{
  if (!mp_Instance)
    mp_Instance = new SimulatorSignatureRegistry();

  return mp_Instance;
}


// =====================================================================
// =====================================================================


void SimulatorSignatureRegistry::addSimulatorSignature(
    openfluid::machine::ModelItemSignatureInstance* Signature)
{
  if (Signature->Signature)
  {
    Signature->ItemType = openfluid::fluidx::ModelItemDescriptor::PluggedSimulator;

    m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::PluggedSimulator][Signature->Signature->ID] = Signature;
  }
  else
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
        "trying to register a simulator signature with no signature instantiated");
}


// =====================================================================
// =====================================================================


void SimulatorSignatureRegistry::addGeneratorSignature(
    openfluid::machine::ModelItemSignatureInstance* Signature)
{
  if (Signature->Signature)
  {
    Signature->ItemType = openfluid::fluidx::ModelItemDescriptor::Generator;

    m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::Generator][Signature->Signature->ID] =
        Signature;
  }
  else
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
        "Trying to register a generator signature with no signature instantiated");
}


// =====================================================================
// =====================================================================


void SimulatorSignatureRegistry::update()
{
  m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::PluggedSimulator].clear();

  openfluid::machine::SimulatorPluginsManager::instance()->unloadAllWares();

  std::vector<openfluid::machine::ModelItemSignatureInstance*> Signatures =
      openfluid::machine::SimulatorPluginsManager::instance()->getAvailableWaresSignatures();

  for (unsigned int i = 0; i < Signatures.size(); i++)
  {
    if (Signatures[i]->Signature)
      addSimulatorSignature(Signatures[i]);
  }

  openfluid::machine::SimulatorPluginsManager::instance()->unloadAllWares();
}


// =====================================================================
// =====================================================================


SimulatorSignatureRegistry::SimSignaturesByTypeByName_t SimulatorSignatureRegistry::getSimSignatures()
{
  return m_SimSignatures;
}


// =====================================================================
// =====================================================================


SimulatorSignatureRegistry::SimSignaturesByName_t SimulatorSignatureRegistry::getGeneratorSignatures()
{
  return m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::Generator];
}


// =====================================================================
// =====================================================================


SimulatorSignatureRegistry::SimSignaturesByName_t SimulatorSignatureRegistry::getSimulatorSignatures()
{
  return m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::PluggedSimulator];
}


// =====================================================================
// =====================================================================


ModelItemSignatureInstance * SimulatorSignatureRegistry::getEmptyModelItemSignature()
{
  openfluid::machine::ModelItemSignatureInstance* Signature =
      new openfluid::machine::ModelItemSignatureInstance();
  Signature->Signature = new openfluid::ware::SimulatorSignature();

  return Signature;
}


// =====================================================================
// =====================================================================


bool SimulatorSignatureRegistry::isSimulatorAvailable(const openfluid::ware::WareID_t& ID)
{
  return m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::PluggedSimulator].count(ID);
}


// =====================================================================
// =====================================================================


const openfluid::machine::ModelItemSignatureInstance*
  SimulatorSignatureRegistry::signature(const openfluid::ware::WareID_t& ID)
{
  if (isSimulatorAvailable(ID))
    return m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::PluggedSimulator][ID];

  if (m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::Generator].count(ID))
    return m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::Generator][ID];

  return (openfluid::machine::ModelItemSignatureInstance*) 0;
}


// =====================================================================
// =====================================================================


const ModelItemSignatureInstance*
  SimulatorSignatureRegistry::signature(openfluid::fluidx::ModelItemDescriptor* Item)
{
  openfluid::ware::WareID_t ItemID = "";

  if (Item->isType(openfluid::fluidx::WareDescriptor::PluggedSimulator))
    ItemID =
        (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(Item))->getID();
  else if (Item->isType(openfluid::fluidx::WareDescriptor::Generator))
    ItemID =
        openfluid::fluidx::GeneratorDescriptor::getGeneratorName(
            (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(Item))->getGeneratorMethod());

  return signature(ItemID);
}


// =====================================================================
// =====================================================================


const ModelItemSignatureInstance*
  SimulatorSignatureRegistry::signature(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method)
{
  return m_GenSignatures[Method];
}


// =====================================================================
// =====================================================================


void SimulatorSignatureRegistry::unloadAll()
{
  openfluid::machine::SimulatorPluginsManager::instance()->unloadAllWares();
}



} } //namespaces
