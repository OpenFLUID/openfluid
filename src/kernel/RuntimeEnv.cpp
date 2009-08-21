/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


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

RuntimeEnvironment::RuntimeEnvironment(std::string AppDir)
{

  m_AppDir = AppDir;
  m_UserDataDir = _S(wxStandardPaths::Get().GetUserDataDir()) + _S(wxFILE_SEP_PATH) + "engine";

  m_OutputDir = m_UserDataDir + _S(wxFILE_SEP_PATH) + OPENFLUID_DEFAULT_OUTDIR;
  m_InputDir = m_UserDataDir + _S(wxFILE_SEP_PATH) + OPENFLUID_DEFAULT_INDIR;
  m_TraceDir = m_UserDataDir + _S(wxFILE_SEP_PATH) + OPENFLUID_DEFAULT_TRACEDIR;


  m_ClearOutputDir = false;
  m_QuietRun = false;
  m_VerboseRun = false;
  m_WriteResults = true;
  m_WriteSimReport = true;
  m_EnableTrace = false;
  m_CheckVarNames = true;

  m_ProgressiveOutput = false;

  mp_FuncEnv = new openfluid::base::FunctionEnvironment();

  mp_FuncEnv->setValue("dir.input",m_InputDir);
  mp_FuncEnv->setValue("dir.output",m_OutputDir);
  mp_FuncEnv->setValue("dir.trace",m_TraceDir);

  mp_FuncEnv->setValue("mode.cleanoutput",m_ClearOutputDir);
  mp_FuncEnv->setValue("mode.quiet",m_QuietRun);
  mp_FuncEnv->setValue("mode.verbose",m_VerboseRun);
  mp_FuncEnv->setValue("mode.saveresults",m_WriteResults);
  mp_FuncEnv->setValue("mode.writereport",m_WriteSimReport);
  mp_FuncEnv->setValue("mode.trace",m_EnableTrace);
  mp_FuncEnv->setValue("mode.checkvarnames",m_CheckVarNames);



  // when development mode (__DEVEL__),
  // use the plugins built on the same development cycle (located in mainappdir/functions)
  #ifdef __LINUX__
  #ifdef __DEVEL__
    m_PlugsDirs.Add(m_AppDir + wxFILE_SEP_PATH + OPENFLUID_PLUGINS_SUBDIR);
  #endif
  #endif

  // plugins search order: user directory then system directory
  m_PlugsDirs.push_back(m_UserDataDir + _S(wxFILE_SEP_PATH) + OPENFLUID_PLUGINS_SUBDIR);

  #ifdef __LINUX__
  #ifndef __DEVEL__
  m_PlugsDirs.push_back(OPENFLUID_PLUGINS_STDSYSDIR);
  #endif
  #endif

  #ifdef __WXMSW__
  m_PlugsDirs.push_back(_S(wxStandardPaths::Get().GetPluginsDir()) + _S(wxFILE_SEP_PATH) + OPENFLUID_PLUGINS_SUBDIR);
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
  m_OutputDir = m_UserDataDir + _S(wxFILE_SEP_PATH) + "OPENFLUID." + _S(Now.Format(wxT("%Y%m%d-%H%M%S"))) + ".OUT";
}


// =====================================================================
// =====================================================================

void RuntimeEnvironment::addExtraPluginsPaths(std::string ColonSeparatedPaths)
{
  std::vector<std::string> ExtraPaths;

  ExtraPaths = SplitString(ColonSeparatedPaths,":");

  for (int i = ExtraPaths.size()-1 ; i>=0 ; i--) m_PlugsDirs.insert(m_PlugsDirs.begin(),1,RemoveTrailingSlashes(ExtraPaths[i]));
}


// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getPluginFullPath(std::string Filename)
{
  std::string PlugFullPath = "";
  std::string TmpPath;

  unsigned int i = 0;

  while ((PlugFullPath.length() == 0) && (i<m_PlugsDirs.size()))
  {

    TmpPath = m_PlugsDirs[i] + _S(wxFILE_SEP_PATH) + Filename;

    if (wxFileExists(_U(TmpPath.c_str()))) PlugFullPath = TmpPath;

    i++;
  }



  return PlugFullPath;
}


// =====================================================================
// =====================================================================


