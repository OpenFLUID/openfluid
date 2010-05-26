/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


/**
  \file ModelFactory.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/machine/ModelFactory.hpp>
#include <openfluid/base/FunctionDescriptor.hpp>
#include <openfluid/base/GeneratorDescriptor.hpp>
#include <openfluid/machine/FixedGenerator.hpp>
#include <openfluid/machine/RandomGenerator.hpp>
#include <openfluid/machine/InterpGenerator.hpp>


namespace openfluid { namespace machine {


// =====================================================================
// =====================================================================


ModelFactory::ModelFactory()
{
  mp_CoreData = openfluid::core::CoreRepository::getInstance();
  mp_ExecMsgs = openfluid::base::ExecutionMessages::getInstance();
}


// =====================================================================
// =====================================================================


const ModelInstance* ModelFactory::buildInstanceFromDescriptor(const openfluid::base::ModelDescriptor& Descriptor) const
{

  openfluid::base::ModelDescriptor::ModelDescription_t::const_iterator it;
  ModelInstance* MInstance = new ModelInstance();
  ModelItemInstance* IInstance;

  for (it=Descriptor.getItems().begin();it!=Descriptor.getItems().end();++it)
  {
    if ((*it)->isType(openfluid::base::ModelItemDescriptor::NoModelItemType))
      throw openfluid::base::OFException("kernel","ModelFactory::buildInstanceFromDescriptor","unknown model item type");

    if ((*it)->isType(openfluid::base::ModelItemDescriptor::PluggedFunction))
    {
      // instanciation of a pluggeg simulation function using the plugin manager
      IInstance = PluginManager::getInstance()->getPlugin(((openfluid::base::FunctionDescriptor*)(*it))->getFileID(),mp_ExecMsgs,mp_CoreData);
      IInstance->Params = (*it)->getParameters();
      MInstance->appendItem(IInstance);
    }

    if ((*it)->isType(openfluid::base::ModelItemDescriptor::Generator))
    {
      // instanciation of a data generator
      openfluid::base::GeneratorDescriptor* GenDesc = (openfluid::base::GeneratorDescriptor*)(*it);
      Generator* GenInstance = NULL;

      IInstance = new ModelItemInstance();
      IInstance->SDKCompatible = true;

      openfluid::base::FunctionSignature* Signature = new openfluid::base::FunctionSignature();

      Signature->ID = "(generator)";

      std::string VarName = GenDesc->getVariableName();
      if (GenDesc->isVectorVariable()) VarName = VarName + "[]";

      Signature->ID = "(generator)"+VarName;
      Signature->HandledData.ProducedVars.push_back(openfluid::base::SignatureHandledDataItem(VarName,GenDesc->getUnitClass(),"",""));

      if (GenDesc->getGeneratorMethod() == openfluid::base::GeneratorDescriptor::Fixed)
        GenInstance = new FixedGenerator();

      if (GenDesc->getGeneratorMethod() == openfluid::base::GeneratorDescriptor::Random)
        GenInstance = new RandomGenerator();

      if (GenDesc->getGeneratorMethod() == openfluid::base::GeneratorDescriptor::Interp)
      {
        Signature->HandledData.RequiredExtraFiles.push_back(GenDesc->getParameters()["sources"]);
        Signature->HandledData.RequiredExtraFiles.push_back(GenDesc->getParameters()["distribution"]);
        GenInstance = new InterpGenerator();
      }

      if (GenInstance == NULL)
        throw openfluid::base::OFException("kernel","ModelFactory::buildInstanceFromDescriptor","unknown generator type");

      GenInstance->setDataRepository(mp_CoreData);
      GenInstance->setExecutionMessages(mp_ExecMsgs);
      GenInstance->setFunctionEnvironment(openfluid::base::RuntimeEnvironment::getInstance()->getFunctionEnvironment());
      GenInstance->setDescriptor(*GenDesc);
      IInstance->Function = GenInstance;
      IInstance->Signature = Signature;

      MInstance->appendItem(IInstance);
    }

  }

  return MInstance;
}


} } //namespaces


