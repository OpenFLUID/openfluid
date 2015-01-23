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


SimulatorSignatureRegistry::SimulatorSignatureRegistry()
{
  openfluid::machine::ModelItemSignatureInstance* FixedSignature =
      new openfluid::machine::ModelItemSignatureInstance();
  FixedSignature->Verified = true;
  FixedSignature->Signature = new openfluid::ware::GeneratorSignature(
      openfluid::fluidx::GeneratorDescriptor::Fixed);
  addAGeneratorSignature(FixedSignature);
  m_GenSignatures[openfluid::fluidx::GeneratorDescriptor::Fixed] = FixedSignature;

  openfluid::machine::ModelItemSignatureInstance* RandomSignature =
      new openfluid::machine::ModelItemSignatureInstance();
  RandomSignature->Verified = true;
  RandomSignature->Signature = new openfluid::ware::GeneratorSignature(
      openfluid::fluidx::GeneratorDescriptor::Random);
  addAGeneratorSignature(RandomSignature);
  m_GenSignatures[openfluid::fluidx::GeneratorDescriptor::Random] = RandomSignature;

  openfluid::machine::ModelItemSignatureInstance* InterpSignature =
      new openfluid::machine::ModelItemSignatureInstance();
  InterpSignature->Verified = true;
  InterpSignature->Signature = new openfluid::ware::GeneratorSignature(
      openfluid::fluidx::GeneratorDescriptor::Interp);
  addAGeneratorSignature(InterpSignature);
  m_GenSignatures[openfluid::fluidx::GeneratorDescriptor::Interp] = InterpSignature;

  openfluid::machine::ModelItemSignatureInstance* InjectSignature =
      new openfluid::machine::ModelItemSignatureInstance();
  InjectSignature->Verified = true;
  InjectSignature->Signature = new openfluid::ware::GeneratorSignature(
      openfluid::fluidx::GeneratorDescriptor::Inject);
  addAGeneratorSignature(InjectSignature);
  m_GenSignatures[openfluid::fluidx::GeneratorDescriptor::Inject] = InjectSignature;

  updatePluggableSignatures();
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


void SimulatorSignatureRegistry::addAPluggableSignature(
    openfluid::machine::ModelItemSignatureInstance* Signature)
{
  if (Signature->Signature)
  {
    Signature->ItemType =
        openfluid::fluidx::ModelItemDescriptor::PluggedSimulator;

    m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::PluggedSimulator][Signature->Signature->ID] =
        Signature;
  }
  else
    throw openfluid::base::FrameworkException(
        "SimulatorSignatureRegistry::addAPluggableSignature",
        "trying to register a PluggableSignature with no SimulatorSignature instancied");
}


// =====================================================================
// =====================================================================


void SimulatorSignatureRegistry::addAGeneratorSignature(
    openfluid::machine::ModelItemSignatureInstance* Signature)
{
  if (Signature->Signature)
  {
    Signature->ItemType = openfluid::fluidx::ModelItemDescriptor::Generator;

    m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::Generator][Signature->Signature->ID] =
        Signature;
  }
  else
    throw openfluid::base::FrameworkException(
        "SimulatorSignatureRegistry::addAGeneratorSignature",
        "trying to register a GeneratorSignature with no SimulatorSignature instancied");
}


// =====================================================================
// =====================================================================


void SimulatorSignatureRegistry::updatePluggableSignatures()
{
  m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::PluggedSimulator].clear();

  openfluid::machine::SimulatorPluginsManager::instance()->unloadAllWares();

  std::vector<openfluid::machine::ModelItemSignatureInstance*> Signatures =
      openfluid::machine::SimulatorPluginsManager::instance()->getAvailableWaresSignatures();

  for (unsigned int i = 0; i < Signatures.size(); i++)
  {
    if (Signatures[i]->Signature)
      addAPluggableSignature(Signatures[i]);
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


SimulatorSignatureRegistry::SimSignaturesByName_t SimulatorSignatureRegistry::getPluggableSignatures()
{
  return m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::PluggedSimulator];
}


// =====================================================================
// =====================================================================


ModelItemSignatureInstance * SimulatorSignatureRegistry::getEmptyPluggableSignature()
{
  openfluid::machine::ModelItemSignatureInstance* Signature =
      new openfluid::machine::ModelItemSignatureInstance();
  Signature->Signature = new openfluid::ware::SimulatorSignature();

  return Signature;
}


// =====================================================================
// =====================================================================


bool SimulatorSignatureRegistry::isPluggableSimulatorAvailable(
    std::string SimulatorID)
{
  return m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::PluggedSimulator].count(
      SimulatorID);
}


// =====================================================================
// =====================================================================


openfluid::machine::ModelItemSignatureInstance* SimulatorSignatureRegistry::signatureItemInstance(
    std::string SimulatorID)
{
  if (isPluggableSimulatorAvailable(SimulatorID))
    return m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::PluggedSimulator][SimulatorID];

  if (m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::Generator].count(
      SimulatorID))
    return m_SimSignatures[openfluid::fluidx::ModelItemDescriptor::Generator][SimulatorID];

  return (openfluid::machine::ModelItemSignatureInstance*) 0;
}


// =====================================================================
// =====================================================================


ModelItemSignatureInstance* SimulatorSignatureRegistry::signatureItemInstance(
    openfluid::fluidx::ModelItemDescriptor* Item)
{
  std::string ItemID = "";

  if (Item->isType(openfluid::fluidx::WareDescriptor::PluggedSimulator))
    ItemID =
        (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(Item))->getID();
  else if (Item->isType(openfluid::fluidx::WareDescriptor::Generator))
    ItemID =
        openfluid::fluidx::GeneratorDescriptor::getGeneratorName(
            (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(Item))->getGeneratorMethod());

  return signatureItemInstance(ItemID);
}


// =====================================================================
// =====================================================================


ModelItemSignatureInstance*
  SimulatorSignatureRegistry::signatureItemInstance(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method)
{
  return m_GenSignatures[Method];
}


// =====================================================================
// =====================================================================


void SimulatorSignatureRegistry::unloadAllSimulators()
{
  openfluid::machine::SimulatorPluginsManager::instance()->unloadAllWares();
}



} } //namespaces
