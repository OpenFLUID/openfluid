/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <wx/datetime.h>

#include "RuntimeEnv.h"
#include "setup.h"


namespace mhydasdk { namespace base {


RuntimeEnvironment::RuntimeEnvironment(wxString AppDir)
{

  m_AppDir = AppDir;
  m_OutputDir = m_AppDir + wxFILE_SEP_PATH + MHYDAS_DEFAULT_OUTDIR;
  m_InputDir = m_AppDir + wxFILE_SEP_PATH + MHYDAS_DEFAULT_INDIR;

}

// =====================================================================
// =====================================================================

RuntimeEnvironment::~RuntimeEnvironment()
{

}



// =====================================================================
// =====================================================================


void RuntimeEnvironment::setDateTimeOutputDir()
{
  wxDateTime Now = wxDateTime::Now();
  m_OutputDir = getAppDir() + wxFILE_SEP_PATH + wxT("MHYDAS.") + Now.Format(wxT("%Y%m%d-%H%M%S")) + wxT(".OUT");
}


// =====================================================================
// =====================================================================



} } // namespace mhydasdk::base
