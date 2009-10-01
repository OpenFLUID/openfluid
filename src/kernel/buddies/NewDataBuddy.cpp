/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file NewDataBuddy.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "NewDataBuddy.h"
#include "openfluid-core.h"


NewDataBuddy::NewDataBuddy() : OpenFLUIDBuddy()
{
  m_RequiredOptionsHelp["outputdir"] = "Output directory for generated dataset";
}


// =====================================================================
// =====================================================================


NewDataBuddy::~NewDataBuddy()
{

}


// =====================================================================
// =====================================================================


bool NewDataBuddy::run()
{
  throw openfluid::base::OFException("under construction");

  return true;
}
