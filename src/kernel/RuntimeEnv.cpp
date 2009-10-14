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

#include "RuntimeEnv.h"
#include "config.h"

#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>


RuntimeEnvironment::RuntimeEnvironment()
{

  std::string HomeDir = "";
  m_TempDir = "";

  char* TempDir;

#if defined __unix__ || defined __APPLE__
  HomeDir = std::getenv("HOME");
  TempDir = std::getenv("TMPDIR");
  if (TempDir == NULL ) TempDir = std::getenv("TMP");
  if (TempDir == NULL ) TempDir = std::getenv("TEMP");
  if (TempDir == NULL ) m_TempDir = "/tmp";
  else m_TempDir = TempDir;
#endif

#if WIN32
  HomeDir = std::getenv("USERPROFILE");
  m_TempDir = std::getenv("TEMP");
#endif

  if (HomeDir == "") HomeDir = m_TempDir;

  HomeDir = boost::filesystem::path(HomeDir).string();
  m_TempDir = boost::filesystem::path(m_TempDir).string();

  m_UserDataDir = boost::filesystem::path(HomeDir+"/.openfluid/engine").string();

  m_OutputDir = boost::filesystem::path(m_UserDataDir + "/" + OPENFLUID_DEFAULT_OUTDIR).string();
  m_InputDir =  boost::filesystem::path(m_UserDataDir + "/" + OPENFLUID_DEFAULT_INDIR).string();


  m_ClearOutputDir = false;
  m_QuietRun = false;
  m_VerboseRun = false;
  m_WriteResults = true;
  m_WriteSimReport = true;
  m_CheckVarNames = true;

  m_ProgressiveOutput = false;

  mp_FuncEnv = new openfluid::base::FunctionEnvironment();

  mp_FuncEnv->setValue("dir.input",m_InputDir);
  mp_FuncEnv->setValue("dir.output",m_OutputDir);
  mp_FuncEnv->setValue("dir.temp",m_TempDir);

  mp_FuncEnv->setValue("mode.cleanoutput",m_ClearOutputDir);
  mp_FuncEnv->setValue("mode.quiet",m_QuietRun);
  mp_FuncEnv->setValue("mode.verbose",m_VerboseRun);
  mp_FuncEnv->setValue("mode.saveresults",m_WriteResults);
  mp_FuncEnv->setValue("mode.writereport",m_WriteSimReport);
  mp_FuncEnv->setValue("mode.checkvarnames",m_CheckVarNames);



  // plugins search order:
  //   command line paths, then user directory, then system directory

  m_PlugsDirs.push_back(boost::filesystem::path(m_UserDataDir + "/" + OPENFLUID_PLUGINS_SUBDIR).string());

#if defined __unix__ || defined __APPLE__
  m_PlugsDirs.push_back(OPENFLUID_PLUGINS_STDSYSDIR);
#endif

  // set ignition date time
  m_IgnitionDateTime = boost::posix_time::microsec_clock::local_time();


  // build simulation ID
  std::string BaseStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  m_SimulationID = boost::gregorian::to_iso_string(m_IgnitionDateTime.date()) + "-";

  srand(time(NULL));

  for (int i=0;i<6;i++)
  {
    m_SimulationID = m_SimulationID + BaseStr[rand() % 26];
  }


  m_EffectiveSimulationDuration = boost::posix_time::time_duration();


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
  m_OutputDir = boost::filesystem::path(m_UserDataDir + "/" + "OPENFLUID." + boost::posix_time::to_iso_string(m_IgnitionDateTime) + ".OUT").string();
}


// =====================================================================
// =====================================================================

void RuntimeEnvironment::addExtraPluginsPaths(std::string SemicolonSeparatedPaths)
{
  std::vector<std::string> ExtraPaths;

#if  defined __unix__ || defined __APPLE__
  ExtraPaths = SplitString(SemicolonSeparatedPaths,":");
#endif

#if WIN32
  ExtraPaths = SplitString(SemicolonSeparatedPaths,";");
#endif


  for (int i = ExtraPaths.size()-1 ; i>=0 ; i--) m_PlugsDirs.insert(m_PlugsDirs.begin(),1,RemoveTrailingSlashes(ExtraPaths[i]));
}


// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getPluginFullPath(std::string Filename)
{
  std::string PlugFullPath = "";
  boost::filesystem::path TmpPath;

  unsigned int i = 0;

  while ((PlugFullPath.length() == 0) && (i<m_PlugsDirs.size()))
  {

    TmpPath = boost::filesystem::path(m_PlugsDirs[i] + "/" + Filename);

    if (boost::filesystem::exists(TmpPath)) PlugFullPath = TmpPath.string();

    i++;
  }

  return PlugFullPath;
}


// =====================================================================
// =====================================================================

void RuntimeEnvironment::setQuietRun(bool Quiet)
{
  m_QuietRun = Quiet;
  mp_FuncEnv->setValue("mode.quiet",m_QuietRun);

  if (Quiet)
  {
    m_VerboseRun = !Quiet;
    mp_FuncEnv->setValue("mode.verbose",m_VerboseRun);
  }
};


// =====================================================================
// =====================================================================

void RuntimeEnvironment::setVerboseRun(bool Verbose)
{
  m_VerboseRun = Verbose;
  mp_FuncEnv->setValue("mode.verbose",m_VerboseRun);

  if (Verbose)
  {
    m_QuietRun = !Verbose;
    mp_FuncEnv->setValue("mode.quiet",m_QuietRun);
  }
};

