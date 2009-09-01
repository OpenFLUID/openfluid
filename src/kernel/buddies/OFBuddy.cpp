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


bool OpenFLUIDBuddy::parseOptions(std::string OptsStr)
{
  // example inputdir=/usr/lib;outputdir=./toto;withfoo=1

  std::vector<std::string> OptsItems;
  std::vector<std::string> OptKeyValue;

  OptsItems = SplitString(OptsStr,";");

  for (unsigned int i = 0; i<OptsItems.size();i++)
  {
    OptKeyValue = SplitString(OptsItems[i],"=");

    if (OptKeyValue.size() == 2 )
    {
      m_Options[OptKeyValue[0]] == OptKeyValue[1];
    }
    else
    {
      m_Options.clear();
      return false;
    }
  }

  return true;
}




