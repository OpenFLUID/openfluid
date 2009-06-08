/**
  \file
  \brief implements of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "ExecMsgs.h"

namespace openfluid { namespace base {



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

  //Parts = SplitString(Message,";");
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



