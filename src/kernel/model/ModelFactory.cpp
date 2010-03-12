/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ModelFactory.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "ModelFactory.h"
#include "FunctionDescriptor.h"
#include "GeneratorDescriptor.h"
#include "generators/FixedGenerator.h"
#include "generators/RandomGenerator.h"
#include "generators/InterpGenerator.h"

// =====================================================================
// =====================================================================


ModelFactory::ModelFactory()
{
  mp_CoreData = openfluid::core::CoreRepository::getInstance();
  mp_ExecMsgs = openfluid::base::ExecutionMessages::getInstance();
}


// =====================================================================
// =====================================================================


const ModelInstance* ModelFactory::buildInstanceFromDescriptor(const ModelDescriptor& Descriptor) const
{

  ModelDescriptor::ModelDescription_t::const_iterator it;
  ModelInstance* MInstance = new ModelInstance();
  ModelItemInstance* IInstance;

  for (it=Descriptor.getItems().begin();it!=Descriptor.getItems().end();++it)
  {
    if ((*it)->isType(ModelItemDescriptor::NoModelItemType))
      throw openfluid::base::OFException("kernel","ModelFactory::buildInstanceFromDescriptor","unknown model item type");

    if ((*it)->isType(ModelItemDescriptor::PluggedFunction))
    {
      // instanciation of a pluggeg simulation function using the plugin manager
      IInstance = PluginManager::getInstance()->getPlugin(((FunctionDescriptor*)(*it))->getFileID(),mp_ExecMsgs,mp_CoreData);
      IInstance->Params = (*it)->getParameters();
      MInstance->appendItem(IInstance);
    }

    if ((*it)->isType(ModelItemDescriptor::Generator))
    {
      // instanciation of a data generator
      GeneratorDescriptor* GenDesc = (GeneratorDescriptor*)(*it);
      Generator* GenInstance = NULL;

      IInstance = new ModelItemInstance();
      IInstance->SDKCompatible = true;

      openfluid::base::FunctionSignature* Signature = new openfluid::base::FunctionSignature();

      Signature->ID = "(generator)";

      std::string VarName = GenDesc->getVariableName();
      if (GenDesc->isVectorVariable()) VarName = VarName + "[]";

      Signature->ID = "(generator)"+VarName;
      Signature->HandledData.ProducedVars.push_back(openfluid::base::SignatureHandledDataItem(VarName,GenDesc->getUnitClass(),"",""));

      if (GenDesc->getGeneratorMethod() == GeneratorDescriptor::Fixed)
        GenInstance = new FixedGenerator();

      if (GenDesc->getGeneratorMethod() == GeneratorDescriptor::Random)
        GenInstance = new RandomGenerator();

      if (GenDesc->getGeneratorMethod() == GeneratorDescriptor::Interp)
        GenInstance = new InterpGenerator();

      if (GenInstance == NULL)
        throw openfluid::base::OFException("kernel","ModelFactory::buildInstanceFromDescriptor","unknown generator type");

      GenInstance->setDataRepository(mp_CoreData);
      GenInstance->setExecutionMessages(mp_ExecMsgs);
      GenInstance->setFunctionEnvironment(new openfluid::base::FunctionEnvironment);
      GenInstance->setDescriptor(*GenDesc);
      IInstance->Function = GenInstance;
      IInstance->Signature = Signature;

      MInstance->appendItem(IInstance);
    }

  }

  return MInstance;
}
