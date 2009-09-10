/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OFBuddy.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "OFBuddy.h"
#include "AppTools.h"


OpenFLUIDBuddy::OpenFLUIDBuddy()
{

}


// =====================================================================
// =====================================================================


OpenFLUIDBuddy::~OpenFLUIDBuddy()
{

}


// =====================================================================
// =====================================================================


void OpenFLUIDBuddy::setOptionIfNotSet(std::string OptionName, std::string OptionValue)
{
  if (m_Options.find(OptionName) == m_Options.end())
  {
    m_Options[OptionName] = OptionValue;
  }
}

// =====================================================================
// =====================================================================


std::string OpenFLUIDBuddy::getYesNoFromOneZero(std::string VStr)
{
  if (VStr == "1") return "yes";
  else return "no";
}


// =====================================================================
// =====================================================================


bool OpenFLUIDBuddy::parseOptions(std::string OptsStr)
{
  // example inputdir=/usr/lib,outputdir=./toto,withfoo=1

  std::vector<std::string> OptsItems;
  std::vector<std::string> OptKeyValue;

  OptsItems = SplitString(OptsStr,",");

  for (unsigned int i = 0; i<OptsItems.size();i++)
  {
    OptKeyValue = SplitString(OptsItems[i],"=");

    if (OptKeyValue.size() == 2 )
    {
      m_Options[OptKeyValue[0]] = OptKeyValue[1];
    }
    else
    {
      m_Options.clear();
      return false;
    }
  }

  return true;
}


// =====================================================================
// =====================================================================


void OpenFLUIDBuddy::printlnOptions()
{

  std::map<std::string,std::string>::iterator it;

  for (it = m_Options.begin();it != m_Options.end();++it)
  {
    std::cout << it->first << " : " << it->second << std::endl;
  }

}


// =====================================================================
// =====================================================================


void OpenFLUIDBuddy::displayHelp()
{
  std::map<std::string,std::string>::iterator it;

  std::cout << "  Required options:" << std::endl;
  if (m_RequiredOptionsHelp.size() ==  0)
    std::cout << "    (none)" << std::endl;
  else
  {
    for (it = m_RequiredOptionsHelp.begin();it != m_RequiredOptionsHelp.end();++it)
    {
      std::cout << "    " << it->first << " : " << it->second << std::endl;
    }
  }

  std::cout << "  Other options:" << std::endl;
  if (m_OtherOptionsHelp.size() ==  0)
    std::cout << "    (none)" << std::endl;
  else
  {
    for (it = m_OtherOptionsHelp.begin();it != m_OtherOptionsHelp.end();++it)
    {
      std::cout << "    " << it->first << " : " << it->second << std::endl;
    }
  }

}

