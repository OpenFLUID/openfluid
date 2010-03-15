/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file Generator.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __GENERATOR_H___
#define __GENERATOR_H___

#include "openfluid-base.h"
#include "openfluid-core.h"
#include "model/GeneratorDescriptor.h"


class Generator : public openfluid::base::PluggableFunction
{
  protected:

    GeneratorDescriptor m_GenDesc;

  public:

    Generator();

    ~Generator();

    void setDescriptor(GeneratorDescriptor GenDesc) { m_GenDesc = GenDesc; };

    bool initParams(openfluid::core::FuncParamsMap_t Params) { return true; };

    bool prepareData()  { return true; };

};


#endif /* __GENERATOR_H___ */
