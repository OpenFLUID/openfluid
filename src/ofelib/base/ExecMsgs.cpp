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


void ExecutionMessages::addWarning(std::string Sender, openfluid::core::TimeStep_t TimeStep, std::string WarningMsg)
{
  m_WarningFlag = true;

  std::ostringstream OutStr;
  OutStr << TimeStep;

  m_WarningMsgs.push_back(Sender + ";" + OutStr.str() + ";" + WarningMsg);

  m_RealWarningsCount++;
}



// =====================================================================
// =====================================================================


} } // namespace openfluid::base



