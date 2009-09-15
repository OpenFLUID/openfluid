/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file NewFuncBuddy.cpp
  \brief Implements the "new function" buddy

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "NewFuncBuddy.h"
#include "openfluid-core.h"


NewFunctionBuddy::NewFunctionBuddy() : OpenFLUIDBuddy()
{
  m_RequiredOptionsHelp["funcid"] = "ID of the created function";
  m_RequiredOptionsHelp["outputdir"] = "path for generated files";

  m_OtherOptionsHelp["authoremail"] = "email(s) of the author(s) of the function";
  m_OtherOptionsHelp["authorname"] = "name(s) of the author(s) of the function";
  m_OtherOptionsHelp["srcformat"] = "format of the generated files (default is singleCPP, value can be CPPwithH)";
}


// =====================================================================
// =====================================================================


NewFunctionBuddy::~NewFunctionBuddy()
{

}


// =====================================================================
// =====================================================================


bool NewFunctionBuddy::run()
{
  throw openfluid::base::OFException("under construction");
  return true;
}
