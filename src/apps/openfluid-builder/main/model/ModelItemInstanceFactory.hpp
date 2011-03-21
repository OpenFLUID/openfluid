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

#ifndef MODELITEMINSTANCEFACTORY_HPP_
#define MODELITEMINSTANCEFACTORY_HPP_

#include <gtkmm.h>

#include <openfluid/machine.hpp>

class ModelItemInstanceFactory
{
  protected:
    static openfluid::machine::ModelItemInstance* createPluggableItemFromSignature(
        openfluid::machine::SignatureItemInstance& Signature)
    {
      openfluid::machine::ModelItemInstance* Item =
          openfluid::machine::PluginManager::getInstance()->getPlugin(
              Signature.Signature->ID);
      Item->ItemType = openfluid::base::ModelItemDescriptor::PluggedFunction;
      return Item;
    }
    static openfluid::machine::Generator* createGeneratorFunctionFromId(
        Glib::ustring Id)
    {
      openfluid::machine::Generator* GeneratorFunction = 0;
      if (Id.find("(generator) Fixed") != std::string::npos)
        GeneratorFunction = new openfluid::machine::FixedGenerator();
      else if (Id.find("(generator) Random") != std::string::npos)
        GeneratorFunction = new openfluid::machine::RandomGenerator();
      else if (Id.find("(generator) Interp") != std::string::npos)
        GeneratorFunction = new openfluid::machine::InterpGenerator();
      else
        throw openfluid::base::OFException("OpenFLUID Builder",
            "ModelItemInstanceFactory::createGeneratorFunction",
            "unknown generator type");
      return GeneratorFunction;
    }
    static openfluid::machine::ModelItemInstance* createGeneratorItemFromSignature(
        openfluid::machine::SignatureItemInstance& Signature)
    {
      openfluid::machine::ModelItemInstance* Item =
          new openfluid::machine::ModelItemInstance();
      Item->SDKCompatible = true;
      Item->ItemType = openfluid::base::ModelItemDescriptor::Generator;
      Item->Signature = Signature.Signature;
      openfluid::machine::Generator* GeneratorFunction = ModelItemInstanceFactory::
          createGeneratorFunctionFromId(Signature.Signature->ID);
      Item->Function = GeneratorFunction;
      return Item;
    }
  public:
    static openfluid::machine::ModelItemInstance* createModelItemInstanceFromSignature(
        openfluid::machine::SignatureItemInstance& Signature)
    {
      openfluid::machine::ModelItemInstance* Item = 0;
      if (!Signature.Signature)
        throw openfluid::base::OFException("OpenFLUID Builder",
            "ModelItemInstanceFactory::createModelItemInstance",
            "Function Signature is not set. Creation is impossible.");
      else
      {
        if (Signature.Signature->ID.find("(generator)") != std::string::npos)
          Item = ModelItemInstanceFactory::createGeneratorItemFromSignature(Signature);
        else
          Item = ModelItemInstanceFactory::createPluggableItemFromSignature(Signature);
      }
      return Item;
    }
};

//class ModelItemInstanceFactorySub: ModelItemInstanceFactory
//{
//  public:
//    ModelItemInstanceFactorySub(openfluid::base::ExecutionMessages& ExecMsgs,
//        openfluid::core::CoreRepository& CoreRepos) :
//      ModelItemInstanceFactory(ExecMsgs, CoreRepos)
//    {
//    }
//    openfluid::machine::ModelItemInstance* createModelItemInstance(
//        openfluid::machine::SignatureItemInstance& Signature)
//    {
//      return ModelItemInstanceFactory::createModelItemInstanceFromSignature(
//          Signature);
//    }
//    openfluid::machine::ModelItemInstance* getResultOfcreateGenerator(
//        openfluid::machine::SignatureItemInstance& Signature)
//    {
//      return ModelItemInstanceFactory::createGeneratorItemFromSignature(
//          Signature);
//    }
//};
#endif /* MODELITEMINSTANCEFACTORY_HPP_ */
