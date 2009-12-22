/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file FunctionDescriptor.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __FUNCTIONDESCRIPTOR_H___
#define __FUNCTIONDESCRIPTOR_H___

#include "openfluid-core.h"
#include "ModelItemDescriptor.h"

class FunctionDescriptor : public ModelItemDescriptor
{

  private:

    std::string m_FileID;  // (filename without ext)

  public:

    FunctionDescriptor(std::string FileID);

    std::string getFileID() const;

};

#endif /* __FUNCTIONDESCRIPTOR_H___ */
