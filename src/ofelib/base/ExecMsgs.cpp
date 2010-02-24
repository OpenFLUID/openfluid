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


void ExecutionMessages::addWarning(std::string Sender, bool IsTimeStep, openfluid::core::TimeStep_t TimeStep, std::string WarningMsg)
{
  m_WarningFlag = true;

  std::ostringstream OutStr;
  OutStr << IsTimeStep << ";" << TimeStep;

  m_WarningMsgs.push_back(Sender + ";" + OutStr.str() + ";" + WarningMsg);

  m_RealWarningsCount++;
}



// =====================================================================
// =====================================================================


std::string ExecutionMessages::FormatMessage(std::string Message)
{
  std::string Formatted = "";

  std::vector<std::string> Parts;

  openfluid::tools::TokenizeString(Message,Parts,";");

  Formatted = Parts[3] + " (message sent by " + Parts[0];

  if (Parts[1] != "0")
  {
    Formatted = Formatted + ", at step " + Parts[2];
  }

  Formatted = Formatted +")";

  return Formatted;

}



} } // namespace openfluid::base



