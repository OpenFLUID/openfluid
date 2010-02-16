/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file RandomGenerator.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __RANDOMGENERATOR_H___
#define __RANDOMGENERATOR_H___


#include "Generator.h"

class RandomGenerator : public Generator
{
  private:
    openfluid::core::ScalarValue m_Min;
    openfluid::core::ScalarValue m_Max;

  public:

    RandomGenerator();

    ~RandomGenerator();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif /* __RANDOMGENERATOR_H___ */
