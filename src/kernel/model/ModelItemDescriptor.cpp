/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ModelItemDescriptor.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "openfluid-base.h"
#include "ModelItemDescriptor.h"


// =====================================================================
// =====================================================================


ModelItemDescriptor::ModelItemDescriptor()
{
  m_ModelItemType = NoModelItemType;
}

// =====================================================================
// =====================================================================


ModelItemDescriptor::~ModelItemDescriptor()
{
}


// =====================================================================
// =====================================================================


void ModelItemDescriptor::setParameter(const openfluid::core::FuncParamKey_t& Key, const openfluid::core::FuncParamKey_t& Value)
{
  m_Params[Key] = Value;
}


// =====================================================================
// =====================================================================


void ModelItemDescriptor::setParameters(const openfluid::core::FuncParamsMap_t& Params)
{
  openfluid::core::FuncParamsMap_t::const_iterator it;

  for (it=Params.begin();it!=Params.end();++it)
  {
    m_Params[(*it).first] = (*it).second;
  }
}

// =====================================================================
// =====================================================================


openfluid::core::FuncParamsMap_t ModelItemDescriptor::getParameters()
{
  return m_Params;
}

// =====================================================================
// =====================================================================


bool ModelItemDescriptor::isType(ModelItemType MIType) const
{
  return (m_ModelItemType == MIType);
}
