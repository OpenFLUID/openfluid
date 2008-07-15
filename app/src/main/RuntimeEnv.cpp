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
  m_UserDataDir = wxStandardPaths::Get().GetUserDataDir() + wxFILE_SEP_PATH + wxT("engine");

  m_OutputDir = m_UserDataDir + wxFILE_SEP_PATH + OPENFLUID_DEFAULT_OUTDIR;
  m_InputDir = m_UserDataDir + wxFILE_SEP_PATH + OPENFLUID_DEFAULT_INDIR;
  m_TraceDir = m_UserDataDir + wxFILE_SEP_PATH + OPENFLUID_DEFAULT_TRACEDIR;


  m_ClearOutputDir = false;
  m_QuietRun = false;
  m_VerboseRun = false;
  m_WriteResults = true;
  m_WriteSimReport = true;
  m_EnableTrace = false;
  m_CheckVarNames = true;


  // when development mode (__DEVEL__),
  // use the plugins built on the same development cycle (located in mainappdir/plugs)
  #ifdef __LINUX__
  #ifdef __DEVEL__
    m_PlugsDirs.Add(m_AppDir + wxFILE_SEP_PATH + OPENFLUID_PLUGINS_SUBDIR);
  #endif
  #endif

  // plugins search order: user directory then system directory
  m_PlugsDirs.Add(m_UserDataDir + wxFILE_SEP_PATH + OPENFLUID_PLUGINS_SUBDIR);

  #ifdef __LINUX__
  #ifndef __DEVEL__
  m_PlugsDirs.Add(OPENFLUID_PLUGINS_STDSYSDIR);
  #endif
  #endif

  #ifdef __WXMSW__
  m_PlugsDirs.Add(wxStandardPaths::Get().GetPluginsDir() + wxFILE_SEP_PATH + OPENFLUID_PLUGINS_SUBDIR);
  #endif





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
  m_OutputDir = m_UserDataDir + wxFILE_SEP_PATH + wxT("OPENFLUID.") + Now.Format(wxT("%Y%m%d-%H%M%S")) + wxT(".OUT");
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


// =====================================================================
// =====================================================================

openfluid::base::FunctionEnvironment RuntimeEnvironment::createFunctionEnvironment()
{
  openfluid::base::FunctionEnvironment FuncEnv;

  FuncEnv.InputDir = m_InputDir;
  FuncEnv.OutputDir = m_OutputDir;

  return FuncEnv;

}


