/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file
  \brief implements of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "ExecMsgs.h"

namespace openfluid { namespace base {


ExecutionMessages* ExecutionMessages::mp_Singleton = NULL;


// =====================================================================
// =====================================================================


ExecutionMessages::ExecutionMessages()
{
  m_WarningFlag = false;
  m_ErrorFlag = false;
  m_ErrorMsg = "";
  m_WarningMsgs.clear();
  m_RealWarningsCount = 0;

}

// =====================================================================
// =====================================================================

ExecutionMessages::~ExecutionMessages()
{

}


// =====================================================================
// =====================================================================


ExecutionMessages* ExecutionMessages::getInstance()
{
  if (mp_Singleton == NULL) mp_Singleton = new ExecutionMessages();
  return mp_Singleton;
}


// =====================================================================
// =====================================================================


void ExecutionMessages::addWarning(Message Msg)
{
  m_WarningFlag = true;

  m_WarningMsgs.push_back(Msg);

  m_RealWarningsCount++;
}


} } // namespace openfluid::base



