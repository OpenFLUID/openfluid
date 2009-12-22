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


void ModelItemDescriptor::setParameter(openfluid::core::FuncParamKey_t Key, openfluid::core::FuncParamKey_t Value)
{
  m_Params[Key] = Value;
}


// =====================================================================
// =====================================================================


openfluid::core::FuncParamsMap_t ModelItemDescriptor::getParameters() const
{
  return m_Params;
}

// =====================================================================
// =====================================================================


bool ModelItemDescriptor::isType(ModelItemType MIType) const
{
  return (m_ModelItemType == MIType);
}
