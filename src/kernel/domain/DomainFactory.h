/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file DomainFactory.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#ifndef __DOMAINFACTORY_H___
#define __DOMAINFACTORY_H___


#include "DomainDescriptor.h"

class DomainFactory
{
  private:

  public:

    DomainFactory();

    void buildDomainFromDescriptor(DomainDescriptor& Descriptor);

};


#endif /* __DOMAINFACTORY_H___ */
