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
 \file ModelItemInstanceFactory.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelItemInstanceFactory.hpp"

#include "GeneratorSignature.hpp"
#include "ModelGeneratorCreationDialog.hpp"

// =====================================================================
// =====================================================================


openfluid::machine::ModelItemInstance* ModelItemInstanceFactory::createPluggableItemFromSignature(
    openfluid::machine::SignatureItemInstance& Signature)
{
  openfluid::machine::ModelItemInstance* Item = 0;

  if (!Signature.Signature)
    throw openfluid::base::OFException("OpenFLUID Builder",
        "ModelItemInstanceFactory::createPluggableItemFromSignature",
        "Function Signature is not set. Creation is impossible.");
  else
  {
    Item = openfluid::machine::PluginManager::getInstance()->getPlugin(
        Signature.Signature->ID);

    if (Item)
      Item->ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;
  }

  return Item;
}

// =====================================================================
// =====================================================================


openfluid::machine::ModelItemInstance* ModelItemInstanceFactory::createGeneratorItemFromSignatureWithDialog(
    openfluid::machine::SignatureItemInstance& Signature,
    openfluid::core::CoreRepository& CoreRepos,
    openfluid::machine::ModelInstance* ModelInstance)
{
  ModelGeneratorCreationDialog Dialog(CoreRepos, ModelInstance);

  std::map<std::string, std::string> GenInfo = Dialog.show();

  if (GenInfo.size() == 3)
    return ModelItemInstanceFactory::createGeneratorItemFromSignature(
        Signature, GenInfo["varname"], GenInfo["classname"], GenInfo["varsize"]);
  else
    return (openfluid::machine::ModelItemInstance*) 0;
}

// =====================================================================
// =====================================================================


openfluid::machine::ModelItemInstance* ModelItemInstanceFactory::createGeneratorItemFromSignature(
    openfluid::machine::SignatureItemInstance& Signature, std::string VarName,
    std::string ClassName, std::string VarSize)
{
  openfluid::machine::ModelItemInstance* Item = 0;

  if (!Signature.Signature)
  {
    std::cerr
        << "OpenFLUID Builder : ModelItemInstanceFactory::createGeneratorItemFromSignature : Function Signature is not set. Creation is impossible."
        << std::endl;

    return Item;
  }

  unsigned int VarSizeInt;
  openfluid::tools::ConvertString(VarSize, &VarSizeInt);

  std::string VarSizedName = VarName;
  if (VarSizeInt > 1)
    VarSizedName.append("[]");

  openfluid::base::GeneratorDescriptor::GeneratorMethod
      Method =
          (static_cast<GeneratorSignature*> (Signature.Signature))->m_GeneratorMethod;

  GeneratorSignature* GeneratorSign = new GeneratorSignature(Method);

  GeneratorSign->ID = openfluid::machine::Factory::buildGeneratorID(
      VarSizedName, (VarSizeInt > 1), ClassName);

  GeneratorSign->HandledData.ProducedVars.push_back(
      openfluid::base::SignatureHandledDataItem(VarSizedName, ClassName, "", ""));

  openfluid::machine::Generator* GeneratorFunction = 0;

  switch (Method)
  {
    case openfluid::base::GeneratorDescriptor::Fixed:
      GeneratorFunction = new openfluid::machine::FixedGenerator();
      break;

    case openfluid::base::GeneratorDescriptor::Random:
      GeneratorFunction = new openfluid::machine::RandomGenerator();
      break;

    case openfluid::base::GeneratorDescriptor::Interp:
      GeneratorFunction = new openfluid::machine::InterpGenerator();
      break;

    default:
      std::cerr
          << "OpenFLUID Builder : ModelItemInstanceFactory::createGeneratorItemFromSignature : bad ModelItemDescriptor type"
          << std::endl;

      return Item;
      break;
  }

  GeneratorFunction->setInfos(VarName, ClassName, Method, VarSizeInt);

  Item = new openfluid::machine::ModelItemInstance();

  Item->Signature = GeneratorSign;

  Item->Function = GeneratorFunction;

  Item->SDKCompatible = true;

  Item->ItemType = openfluid::base::ModelItemDescriptor::Generator;

  return Item;
}

// =====================================================================
// =====================================================================


