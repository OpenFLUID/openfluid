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


// =====================================================================
// =====================================================================


ModelFactory::ModelFactory(openfluid::core::CoreRepository* CoreData,
                           openfluid::base::ExecutionMessages* ExecMsgs,
                           openfluid::base::FunctionEnvironment* FuncEnv)
{
  mp_CoreData = CoreData;
  mp_ExecMsgs = ExecMsgs;
  mp_FuncEnv = FuncEnv;
}


// =====================================================================
// =====================================================================


ModelInstance* ModelFactory::buildInstanceFromDescriptor(const ModelDescriptor& Descriptor) const
{

  throw openfluid::base::OFException("under construction");
  return NULL;
}
