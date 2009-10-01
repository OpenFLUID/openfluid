/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file NewDataBuddy.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __NEWDATABUDDY_H___
#define __NEWDATABUDDY_H___

#include "OFBuddy.h"

class NewDataBuddy : public OpenFLUIDBuddy
{
  private:

  public:

    NewDataBuddy();

    ~NewDataBuddy();

    bool run();

};



#endif /* __NEWDATABUDDY_H___ */
