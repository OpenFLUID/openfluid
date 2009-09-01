/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OFBuddy.h
  \brief header of OpenFLUIDBuddy class ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#ifndef __OFBUDDY_H__
#define __OFBUDDY_H__

#include <vector>
#include <string>
#include <map>


class OpenFLUIDBuddy
{
  private:
    std::map<std::string,std::string> m_Options;

  public:
    OpenFLUIDBuddy();

    ~OpenFLUIDBuddy();

    bool parseOptions(std::string OptsStr);

    virtual bool runBuddy() = 0;

};

#endif // __OFBUDDY_H__
