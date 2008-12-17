/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <wx/datetime.h>

#include "RuntimeEnv.h"
#include "config.h"

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

  mp_FuncEnv = new openfluid::base::FunctionEnvironment();

  mp_FuncEnv->setValue(wxT("dir.input"),m_InputDir);
  mp_FuncEnv->setValue(wxT("dir.output"),m_OutputDir);
  mp_FuncEnv->setValue(wxT("dir.trace"),m_TraceDir);

  mp_FuncEnv->setValue(wxT("mode.cleanoutput"),m_ClearOutputDir);
  mp_FuncEnv->setValue(wxT("mode.quiet"),m_QuietRun);
  mp_FuncEnv->setValue(wxT("mode.verbose"),m_VerboseRun);
  mp_FuncEnv->setValue(wxT("mode.saveresults"),m_WriteResults);
  mp_FuncEnv->setValue(wxT("mode.writereport"),m_WriteSimReport);
  mp_FuncEnv->setValue(wxT("mode.trace"),m_EnableTrace);
  mp_FuncEnv->setValue(wxT("mode.checkvarnames"),m_CheckVarNames);



  // when development mode (__DEVEL__),
  // use the plugins built on the same development cycle (located in mainappdir/functions)
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


