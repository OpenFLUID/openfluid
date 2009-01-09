/**
  \file ExecMsgs.h
  \brief Header of class managing execution messages

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __EXECMSGS_H__
#define __EXECMSGS_H__

#include <wx/string.h>
#include <wx/arrstr.h>

namespace openfluid { namespace base {

/**
  Class ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/
class ExecutionMessages
{

  private:
    bool m_WarningFlag;
    bool m_ErrorFlag;

    wxArrayString m_WarningMsgs;
    wxString m_ErrorMsg;


  public:

    /**
      Constructor
    */
    ExecutionMessages();

    /**
      Destructor
    */
    ~ExecutionMessages();

    void addWarning(wxString Sender, int TimeStep, wxString WarningMsg);

    void addWarning(wxString Sender, wxString WarningMsg) { addWarning(Sender,-1,WarningMsg); };

    void resetWarningFlag() { m_WarningFlag = false; };

    bool isWarningFlag() const { return m_WarningFlag; };

    wxArrayString getWarningMsgs() const { return m_WarningMsgs; };

};

} } // namespace openfluid::base


#endif /*__EXECMSGS_H__*/

