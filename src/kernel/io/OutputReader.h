/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OutputReader.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OUTPUTREADER_H___
#define __OUTPUTREADER_H___

#include "output/OutputDescriptor.h"

class OutputReader
{
  private:

  public:

    OutputReader();

    ~OutputReader();

    OutputDescriptor readFromFile(std::string OutputFilePath);
};


#endif /* __OUTPUTREADER_H___ */
