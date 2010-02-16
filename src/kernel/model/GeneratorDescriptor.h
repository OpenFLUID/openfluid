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

    openfluid::core::VariableName_t m_VarName;

    openfluid::core::UnitClass_t m_UnitClass;

    GeneratorMethod m_GenMethod;

    unsigned int m_VarSize;

  public:

    GeneratorDescriptor();

    GeneratorDescriptor(openfluid::core::VariableName_t VarName, openfluid::core::UnitClass_t UnitClass,
                        GeneratorMethod GenMethod, unsigned int VarSize=1);

    openfluid::core::VariableName_t getVariableName() const;

    openfluid::core::UnitClass_t getUnitClass() const;

    GeneratorMethod getGeneratorMethod() const;

    bool isVectorVariable() const { return (m_VarSize > 1); };

    bool isScalarVariable() const { return (!isVectorVariable()); };

    unsigned int getVariableSize() const { return m_VarSize; };


};


#endif /* __GENERATORDESCRIPTOR_H___ */
