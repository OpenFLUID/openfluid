/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file DomainReader.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __DOMAINREADER_H___
#define __DOMAINREADER_H___


#include "openfluid-core.h"

class DomainReader
{
  private:

    void loadFromFile(std::string Filename, std::list<openfluid::core::UnitsLink_t>* ToUnitsList);

  public:

    DomainReader();

    ~DomainReader();

    void loadDomainFromDirectory(std::string DirPath, openfluid::core::CoreRepository* CoreData);


};



#endif /* __DOMAINREADER_H___ */
