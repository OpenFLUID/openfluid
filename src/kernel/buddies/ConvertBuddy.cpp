/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ConvertBuddy.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "ConvertBuddy.h"
#include "openfluid-core.h"


ConvertBuddy::ConvertBuddy() : OpenFLUIDBuddy()
{
  m_RequiredOptionsHelp["conv"] = "Conversion mode";
  m_RequiredOptionsHelp["inputdir"] = "Input directory for dataset to convert";
  m_RequiredOptionsHelp["outputdir"] = "Output directory for converted dataset";
}


// =====================================================================
// =====================================================================


ConvertBuddy::~ConvertBuddy()
{

}


// =====================================================================
// =====================================================================


bool ConvertBuddy::run()
{
  throw openfluid::base::OFException("under construction");

  return true;
}
