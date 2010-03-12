/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ModelFactory.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */
#ifndef __MODELFACTORY_H__
#define __MODELFACTORY_H__

#include "openfluid-core.h"
#include "openfluid-base.h"
#include "ModelDescriptor.h"
#include "ModelInstance.h"
#include "PluginManager.h"

class ModelFactory
{
  private:
    openfluid::core::CoreRepository* mp_CoreData;
    openfluid::base::ExecutionMessages* mp_ExecMsgs;

  public:

    ModelFactory();

    const ModelInstance* buildInstanceFromDescriptor(const ModelDescriptor& Descriptor) const;

};

#endif /* __MODELFACTORY_H__ */
