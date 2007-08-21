/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <wx/datetime.h>

#include "RuntimeEnv.h"
#include "setup.h"

#include <wx/stdpaths.h>

#include <iostream>

RuntimeEnvironment::RuntimeEnvironment(wxString AppDir)
{

  m_AppDir = AppDir;
  m_OutputDir = wxStandardPaths::Get().GetUserDataDir() + wxFILE_SEP_PATH + MHYDAS_DEFAULT_OUTDIR;
  m_InputDir = wxStandardPaths::Get().GetUserDataDir() + wxFILE_SEP_PATH + MHYDAS_DEFAULT_INDIR;

  m_ClearOutputDir = false; 
  m_QuietRun = false;  
  m_WriteResults = true;  
  m_WriteSimReport = true;  
  
  // plugins search order: user directory then system directory

  m_PlugsDirs.Add(wxStandardPaths::Get().GetUserDataDir() + wxFILE_SEP_PATH + MHYDAS_PLUGINS_SUBDIR);  
  
  #ifdef __LINUX__
  m_PlugsDirs.Add(MHYDAS_PLUGINS_STDSYSDIR);
  #endif

  #ifdef __WXMSW__
  m_PlugsDirs.Add(wxStandardPaths::Get().GetPluginsDir() + wxFILE_SEP_PATH + MHYDAS_PLUGINS_SUBDIR);
  #endif  
    
  #ifdef __LINUX__
  m_PlugsDirs.Add(m_AppDir + wxFILE_SEP_PATH + MHYDAS_PLUGINS_SUBDIR);
  #endif
  


  //std::cerr << _C(wxStandardPaths::Get().GetInstallPrefix()) << std::endl;
  //std::cerr << _C(m_PlugsDirs[0]) << std::endl; 
  //std::cerr << _C(m_PlugsDirs[1]) << std::endl;  
  //std::cerr << _C(m_PlugsDirs[2]) << std::endl;  
  

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
  m_OutputDir = wxStandardPaths::Get().GetUserDataDir() + wxFILE_SEP_PATH + wxT("MHYDAS.") + Now.Format(wxT("%Y%m%d-%H%M%S")) + wxT(".OUT");
}


// =====================================================================
// =====================================================================


wxString RuntimeEnvironment::getPluginFullPath(wxString Filename)
{
  wxString PlugFullPath = wxT("");  
  wxString TmpPath;
  
  int i = 0;
  
  while ((PlugFullPath.Length() == 0) && (i<m_PlugsDirs.Count()))
  {
    
    TmpPath = m_PlugsDirs[i] + wxFILE_SEP_PATH + Filename;
    
    if (wxFileExists(TmpPath)) PlugFullPath = TmpPath;

    i++;  
  }
  
  
  
  return PlugFullPath;
}


