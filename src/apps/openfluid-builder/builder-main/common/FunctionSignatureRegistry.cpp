/*
 This file is part of OpenFLUID software
 Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
 along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

 In addition, as a special exception, INRA gives You the additional right
 to dynamically link the code of OpenFLUID with code not covered
 under the GNU General Public License ("Non-GPL Code") and to distribute
 linked combinations including the two, subject to the limitations in this
 paragraph. Non-GPL Code permitted under this exception must only link to
 the code of OpenFLUID dynamically through the OpenFLUID libraries
 interfaces, and only for building OpenFLUID plugins. The files of
 Non-GPL Code may be link to the OpenFLUID libraries without causing the
 resulting work to be covered by the GNU General Public License. You must
 obey the GNU General Public License in all respects for all of the
 OpenFLUID code and other code used in conjunction with OpenFLUID
 except the Non-GPL Code covered by this exception. If you modify
 this OpenFLUID, you may extend this exception to your version of the file,
 but you are not obligated to do so. If you do not wish to provide this
 exception without modification, you must delete this exception statement
 from your version and license this OpenFLUID solely under the GPL without
 exception.


 == Other Usage ==

 Other Usage means a use of OpenFLUID that is inconsistent with the GPL
 license, and requires a written agreement between You and INRA.
 Licensees for Other Usage of OpenFLUID may use this file in accordance
 with the terms contained in the written agreement between You and INRA.
 */

/**
 \file FunctionSignatureRegistry.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "FunctionSignatureRegistry.hpp"

FunctionSignatureRegistry* FunctionSignatureRegistry::mp_Instance = 0;

// =====================================================================
// =====================================================================


FunctionSignatureRegistry::FunctionSignatureRegistry()
{
  addAGeneratorSignature(new FixedGeneratorSignature());
  addAGeneratorSignature(new RandomGeneratorSignature());
  addAGeneratorSignature(new InterpGeneratorSignature());
}

// =====================================================================
// =====================================================================


FunctionSignatureRegistry* FunctionSignatureRegistry::getInstance()
{
  if (!mp_Instance)
  {
    mp_Instance = new FunctionSignatureRegistry();
    mp_Instance->updatePluggableSignatures();
  }

  return mp_Instance;
}

// =====================================================================
// =====================================================================


void FunctionSignatureRegistry::addAPluggableSignature(
    openfluid::machine::SignatureItemInstance* Signature)
{
  Signature->ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;

  m_Signatures[openfluid::base::ModelItemDescriptor::PluggedFunction].push_back(
      Signature);
}

// =====================================================================
// =====================================================================


void FunctionSignatureRegistry::addAGeneratorSignature(
    openfluid::machine::SignatureItemInstance* Signature)
{
  m_Signatures[openfluid::base::ModelItemDescriptor::Generator].push_back(
      Signature);
}

// =====================================================================
// =====================================================================


void FunctionSignatureRegistry::updatePluggableSignatures()
{
  m_Signatures[openfluid::base::ModelItemDescriptor::PluggedFunction].clear();

  openfluid::machine::PluginManager::getInstance()->unloadAllPlugins();

  openfluid::machine::ArrayOfSignatureItemInstance Signatures =
      openfluid::machine::PluginManager::getInstance()->getAvailableFunctions();

  for (unsigned int i = 0; i < Signatures.size(); i++)
  {
    addAPluggableSignature( Signatures[i]);
  }
}

// =====================================================================
// =====================================================================


FunctionSignatureRegistry::FctSignaturesByType_t FunctionSignatureRegistry::getFctSignatures()
{
  return m_Signatures;
}

// =====================================================================
// =====================================================================


FunctionSignatureRegistry::FctSignatures_t FunctionSignatureRegistry::getGeneratorSignatures()
{
  return m_Signatures[openfluid::base::ModelItemDescriptor::Generator];
}

// =====================================================================
// =====================================================================


FunctionSignatureRegistry::FctSignatures_t FunctionSignatureRegistry::getPluggableSignatures()
{
  return m_Signatures[openfluid::base::ModelItemDescriptor::PluggedFunction];
}

// =====================================================================
// =====================================================================


openfluid::machine::SignatureItemInstance * FunctionSignatureRegistry::getEmptyPluggableSignature()
{
  openfluid::machine::SignatureItemInstance* Signature =
      new openfluid::machine::SignatureItemInstance();
  Signature->Signature = new openfluid::base::FunctionSignature();

  return Signature;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


void FunctionSignatureRegistrySub::addAPluggableSignature(
    openfluid::machine::SignatureItemInstance* Signature)
{
  FunctionSignatureRegistry::addAPluggableSignature(Signature);
}

// =====================================================================
// =====================================================================


void FunctionSignatureRegistrySub::addAGeneratorSignature(
    openfluid::machine::SignatureItemInstance* Signature)
{
  FunctionSignatureRegistry::addAGeneratorSignature(Signature);
}
