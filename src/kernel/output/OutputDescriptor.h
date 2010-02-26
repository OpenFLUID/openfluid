/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OutputDescriptor.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OUTPUTDESCRIPTOR_H__
#define __OUTPUTDESCRIPTOR_H__

#include <vector>
#include "OutputFilesDescriptor.h"

class OutputDescriptor
{
  private:

    std::vector<OutputFilesDescriptor> m_FileSets;

  public:

    OutputDescriptor();

    ~OutputDescriptor();

    std::vector<OutputFilesDescriptor>& getFileSets() {return m_FileSets; };

    void fillUnitsPtr();


};

#endif /* __OUTPUTDESCRIPTOR_H__ */
