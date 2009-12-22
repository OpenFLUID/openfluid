/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ModelItemDescriptor.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __MODELITEMDESCRIPTOR_H___
#define __MODELITEMDESCRIPTOR_H___

#include "openfluid-core.h"


class ModelItemDescriptor
{
  public:
    enum ModelItemType { NoModelItemType, PluggedFunction, Generator };

  protected:

    ModelItemType m_ModelItemType;
    openfluid::core::FuncParamsMap_t m_Params;  // Function parameters set

  public:

    ModelItemDescriptor();

    virtual ~ModelItemDescriptor();

    void setParameter(const openfluid::core::FuncParamKey_t& Key, const openfluid::core::FuncParamKey_t& Value);

    void setParameters(const openfluid::core::FuncParamsMap_t& Params);

    openfluid::core::FuncParamsMap_t getParameters() const;

    bool isType(ModelItemType MIType) const;
};


#endif /* __MODELITEMDESCRIPTOR_H___ */
