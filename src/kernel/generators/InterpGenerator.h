/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file InterpGenerator.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __INTERPGENERATOR_H___
#define __INTERPGENERATOR_H___


#include "Generator.h"
#include "openfluid-tools.h"

class InterpGenerator : public Generator
{
  private:
    openfluid::tools::DistributeInterpolate m_DataPool;

    bool m_IsMin;
    bool m_IsMax;

    double m_Min;
    double m_Max;

  public:

    InterpGenerator();

    ~InterpGenerator();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif /* __INTERPGENERATOR_H___ */
