/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file NewFuncBuddy.h
  \brief Header of the "new function" buddy

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __NEWFUNCBUDDY_H___
#define __NEWFUNCBUDDY_H___

#include "OFBuddy.h"

class NewFunctionBuddy : public OpenFLUIDBuddy
{
  private:
    void writeFunctionCPP();

  public:

    NewFunctionBuddy();

    ~NewFunctionBuddy();

    bool run();

};

#endif /* __NEWFUNCBUDDY_H___ */
