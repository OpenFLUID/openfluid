/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ModelItemInstance.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __MODELITEMINSTANCE_H___
#define __MODELITEMINSTANCE_H___

#include <string>

#include "openfluid-base.h"
#include "openfluid-core.h"

struct ModelItemInstance
{

  std::string Filename;
  bool SDKCompatible;
  openfluid::core::FuncParamsMap_t Params;
  openfluid::base::FunctionSignature* Signature;
  openfluid::base::PluggableFunction* Function;

  ModelItemInstance()
  {
    Filename = "";
    Signature = NULL;
    Function = NULL;
    SDKCompatible = false;
  }

};


#endif /* __MODELITEMINSTANCE_H___ */
