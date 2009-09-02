/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file Func2DocBuddy.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __FUNC2DOCBUDDY_H___
#define __FUNC2DOCBUDDY_H___


#include "OFBuddy.h"

class Func2DocBuddy : public OpenFLUIDBuddy
{

  public:

    Func2DocBuddy();

    ~Func2DocBuddy();

    bool run();

};

#endif /* __FUNC2DOCBUDDY_H___ */
