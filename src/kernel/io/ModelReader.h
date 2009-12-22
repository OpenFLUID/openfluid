/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ModelReader.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __MODELREADER_H___
#define __MODELREADER_H___

#include <libxml/tree.h>

#include "ModelDescriptor.h"
#include "openfluid-core.h"

class ModelReader
{
  private:

    openfluid::core::FuncParamsMap_t extractParamsFromNode(xmlNodePtr NodePtr);

  public:

    ModelReader();

    ~ModelReader();

    ModelDescriptor readFromFile(std::string ModelFilePath);

};


#endif /* __MODELREADER_H___ */
