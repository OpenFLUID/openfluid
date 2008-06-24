/**
  \file Tools.h
  \brief Common super class dor mhydasdk-tools classes
  
  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <wx/string.h>

namespace mhydasdk { namespace tools {

class Tools
{
  private:
    wxString m_ErrorMessage;
    
  public:
    Tools();
    virtual ~Tools();
    
    void setErrorMessage(const wxString Msg) { m_ErrorMessage = Msg; };
    
    wxString getErrorMessage() const { return m_ErrorMessage; };
    
};

} } // namespace

#endif /*__TOOLS_H___*/
