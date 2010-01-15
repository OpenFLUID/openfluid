/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file RunReader.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __RUNREADER_H___
#define __RUNREADER_H___


#include "run/RunDescriptor.h"


class RunReader
{
  private:

  public:

    RunReader();

    ~RunReader();

    RunDescriptor readFromFile(std::string RunFilePath);


};


#endif /* __RUNREADER_H___ */
