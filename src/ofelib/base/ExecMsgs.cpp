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
  m_ErrorMsg = wxT("");
  m_WarningMsgs.Clear();
}

// =====================================================================
// =====================================================================

ExecutionMessages::~ExecutionMessages()
{

}

// =====================================================================
// =====================================================================


void ExecutionMessages::addWarning(wxString Sender, int TimeStep, wxString WarningMsg)
{
  m_WarningFlag = true;
  m_WarningMsgs.Add(Sender + wxT(";") + wxString::Format(wxT("%d"),TimeStep) + wxT(";") + WarningMsg);
}


// =====================================================================
// =====================================================================



} } // namespace openfluid::base



