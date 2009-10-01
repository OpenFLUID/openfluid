/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ConvertBuddy.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __CONVERTBUDDY_H___
#define __CONVERTBUDDY_H___

#include "OFBuddy.h"

class ConvertBuddy : public OpenFLUIDBuddy
{
  private:

  public:

    ConvertBuddy();

    ~ConvertBuddy();

    bool run();

};



#endif /* __CONVERTBUDDY_H___ */
