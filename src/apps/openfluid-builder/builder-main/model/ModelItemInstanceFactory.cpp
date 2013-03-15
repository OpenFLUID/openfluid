///*
// This file is part of OpenFLUID software
// Copyright (c) 2007-2010 INRA-Montpellier SupAgro
//
//
// == GNU General Public License Usage ==
//
// OpenFLUID is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OpenFLUID is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.
//
// In addition, as a special exception, INRA gives You the additional right
// to dynamically link the code of OpenFLUID with code not covered
// under the GNU General Public License ("Non-GPL Code") and to distribute
// linked combinations including the two, subject to the limitations in this
// paragraph. Non-GPL Code permitted under this exception must only link to
// the code of OpenFLUID dynamically through the OpenFLUID libraries
// interfaces, and only for building OpenFLUID plugins. The files of
// Non-GPL Code may be link to the OpenFLUID libraries without causing the
// resulting work to be covered by the GNU General Public License. You must
// obey the GNU General Public License in all respects for all of the
// OpenFLUID code and other code used in conjunction with OpenFLUID
// except the Non-GPL Code covered by this exception. If you modify
// this OpenFLUID, you may extend this exception to your version of the file,
// but you are not obligated to do so. If you do not wish to provide this
// exception without modification, you must delete this exception statement
// from your version and license this OpenFLUID solely under the GPL without
// exception.
//
//
// == Other Usage ==
//
// Other Usage means a use of OpenFLUID that is inconsistent with the GPL
// license, and requires a written agreement between You and INRA.
// Licensees for Other Usage of OpenFLUID may use this file in accordance
// with the terms contained in the written agreement between You and INRA.
// */
//
///**
// \file ModelItemInstanceFactory.cpp
// \brief Implements ...
//
// \author Aline LIBRES <libres@supagro.inra.fr>
// */
//
//#include "ModelItemInstanceFactory.hpp"
//
//#include <openfluid/machine/ModelItemInstance.hpp>
//#include <openfluid/machine/FunctionPluginsManager.hpp>
//#include <openfluid/machine/Factory.hpp>
//#include <openfluid/machine/FixedGenerator.hpp>
//#include <openfluid/machine/RandomGenerator.hpp>
//#include <openfluid/machine/InterpGenerator.hpp>
//#include <openfluid/machine/InjectGenerator.hpp>
//#include <openfluid/fluidx/ModelItemDescriptor.hpp>
//#include <openfluid/tools/SwissTools.hpp>
//
//#include <openfluid/ware/GeneratorSignature.hpp>
//#include "ModelGeneratorCreationDialog.hpp"
//
//// =====================================================================
//// =====================================================================
//
//
//openfluid::machine::ModelItemInstance* ModelItemInstanceFactory::createPluggableItemFromSignature(
//    openfluid::machine::ModelItemSignatureInstance& Signature)
//{
//  openfluid::machine::ModelItemInstance* Item = 0;
//
//  if (!Signature.Signature)
//    throw openfluid::base::OFException("OpenFLUID Builder",
//        "ModelItemInstanceFactory::createPluggableItemFromSignature",
//        "Function Signature is not set. Creation is impossible.");
//  else
//  {
//    Item = openfluid::machine::FunctionPluginsManager::getInstance()->loadWareSignatureOnly(
//        Signature.Signature->ID);
//
//    if (Item)
//      Item->ItemType = openfluid::fluidx::ModelItemDescriptor::PluggedFunction;
//  }
//
//  return Item;
//}
//
//// =====================================================================
//// =====================================================================
//
//
//openfluid::machine::ModelItemInstance* ModelItemInstanceFactory::createGeneratorItemFromSignatureWithDialog(
//    openfluid::machine::ModelItemSignatureInstance& Signature,
//    openfluid::core::CoreRepository& CoreRepos,
//    openfluid::machine::ModelInstance* ModelInstance)
//{
//  ModelGeneratorCreationDialog Dialog(CoreRepos, ModelInstance);
//
//  std::map<std::string, std::string> GenInfo = Dialog.show();
//
//  if (GenInfo.size() == 3)
//    return ModelItemInstanceFactory::createGeneratorItemFromSignature(
//        Signature, GenInfo["varname"], GenInfo["classname"], GenInfo["varsize"]);
//  else
//    return (openfluid::machine::ModelItemInstance*) 0;
//}
//
//// =====================================================================
//// =====================================================================
//
//
//openfluid::machine::ModelItemInstance* ModelItemInstanceFactory::createGeneratorItemFromSignature(
//    openfluid::machine::ModelItemSignatureInstance& Signature, std::string VarName,
//    std::string ClassName, std::string VarSize)
//{
//  openfluid::machine::ModelItemInstance* Item = 0;
//
//  if (!Signature.Signature)
//  {
//    std::cerr
//        << "OpenFLUID Builder : ModelItemInstanceFactory::createGeneratorItemFromSignature : Function Signature is not set. Creation is impossible."
//        << std::endl;
//
//    return Item;
//  }
//
//  unsigned int VarSizeInt;
//  openfluid::tools::ConvertString(VarSize, &VarSizeInt);
//
//  openfluid::fluidx::GeneratorDescriptor::GeneratorMethod
//      Method =
//          (static_cast<openfluid::ware::GeneratorSignature*> (Signature.Signature))->m_GeneratorMethod;
//
//  openfluid::ware::GeneratorSignature* GeneratorSign = new openfluid::ware::GeneratorSignature(Method);
//
//  GeneratorSign->ID = openfluid::machine::Factory::buildGeneratorID(
//      VarName, (VarSizeInt > 1), ClassName);
//
//  GeneratorSign->HandledData.ProducedVars.push_back(
//       openfluid::ware::SignatureHandledTypedDataItem(VarName, ClassName, "", ""));
//
//  Item = new openfluid::machine::ModelItemInstance();
//  Item->Signature = GeneratorSign;
//  Item->Body = NULL;
//  Item->SDKCompatible = true;
//  Item->ItemType = openfluid::fluidx::ModelItemDescriptor::Generator;
//
//  Item->GeneratorInfo = new openfluid::machine::GeneratorExtraInfo();
//  Item->GeneratorInfo->VariableName = VarName;
//  Item->GeneratorInfo->UnitClass = ClassName;
//  Item->GeneratorInfo->VariableSize = VarSizeInt;
//  Item->GeneratorInfo->GeneratorMethod = Method;
//
//  return Item;
//}
//
//// =====================================================================
//// =====================================================================
//
//
