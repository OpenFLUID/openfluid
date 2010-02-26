/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file SimReportWriter.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __SIMREPORTWRITER_H___
#define __SIMREPORTWRITER_H___

#include <string>

class SimulationReportWriter
{
  private:

  public:
    SimulationReportWriter();

    ~SimulationReportWriter();

    static void saveToFile(std::string FilePath,openfluid::base::SimulationInfo *SimInfo);

};



#endif /* __SIMREPORTWRITER_H___ */
