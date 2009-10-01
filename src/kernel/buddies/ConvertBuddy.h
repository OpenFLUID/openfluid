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
#include "openfluid-core.h"

#include <map>

class ConvertBuddy : public OpenFLUIDBuddy
{
  private:


    void convert_13_14();
    void convert_13_14_defs(std::string UnitClass);
    void convert_13_14_data();
    void convert_13_14_output();
    void convert_13_14_model_run_events();

    std::map<openfluid::core::UnitID_t,double> m_GUSurfaces;

  public:

    ConvertBuddy();

    ~ConvertBuddy();

    bool run();

};



#endif /* __CONVERTBUDDY_H___ */
