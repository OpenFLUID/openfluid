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
#include <iostream>

class OpenFLUIDBuddy
{
  protected:
    std::map<std::string,std::string> m_Options;

    std::map<std::string,std::string> m_RequiredOptionsHelp;
    std::map<std::string,std::string> m_OtherOptionsHelp;

    void setOptionIfNotSet(std::string OptionName, std::string OptionValue);

    std::string getYesNoFromOneZero(std::string VStr);

  public:
    OpenFLUIDBuddy();

    ~OpenFLUIDBuddy();

    bool parseOptions(std::string OptsStr);

    void printlnOptions();

    virtual bool run() = 0;

    void displayHelp();
};

#endif // __OFBUDDY_H__
