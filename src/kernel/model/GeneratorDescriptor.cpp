/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file GeneratorDescriptor.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "GeneratorDescriptor.h"

// =====================================================================
// =====================================================================

GeneratorDescriptor::GeneratorDescriptor(openfluid::core::VariableName_t VarName,
                                         openfluid::core::UnitClass_t UnitClass,
                                         GeneratorMethod GenMethod)
                    : ModelItemDescriptor()
{
  m_ModelItemType = Generator;
  m_VarName = VarName;
  m_UnitClass = UnitClass;
  m_GenMethod = GenMethod;
}


// =====================================================================
// =====================================================================


openfluid::core::VariableName_t GeneratorDescriptor::getVariableName() const
{
  return m_VarName;
}


// =====================================================================
// =====================================================================


openfluid::core::UnitClass_t GeneratorDescriptor::getUnitClass() const
{
  return m_UnitClass;
}


// =====================================================================
// =====================================================================


GeneratorDescriptor::GeneratorMethod GeneratorDescriptor::getGeneratorMethod() const
{
  return m_GenMethod;
}

