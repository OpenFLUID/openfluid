/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file GeneratorDescriptor.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __GENERATORDESCRIPTOR_H___
#define __GENERATORDESCRIPTOR_H___

#include "ModelItemDescriptor.h"


class GeneratorDescriptor : public ModelItemDescriptor
{
  public:
    enum GeneratorMethod { NoGenMethod, Fixed, Random, Interp };

  private:

    openfluid::core::VariableName_t m_VarName; // Generators only

    openfluid::core::UnitClass_t m_UnitClass; // Generators only

    GeneratorMethod m_GenMethod; // Generators only

  public:

    GeneratorDescriptor(openfluid::core::VariableName_t VarName, openfluid::core::UnitClass_t UnitClass,
                        GeneratorMethod GenMethod);

    openfluid::core::VariableName_t getVariableName() const;

    openfluid::core::UnitClass_t getUnitClass() const;

    GeneratorMethod getGeneratorMethod() const;
};


#endif /* __GENERATORDESCRIPTOR_H___ */
