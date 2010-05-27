/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/config.hpp>

#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>


#if defined __unix__ || defined __APPLE__
  #include <unistd.h>
#endif


namespace openfluid { namespace base {


RuntimeEnvironment* RuntimeEnvironment::mp_Singleton = NULL;


RuntimeEnvironment::RuntimeEnvironment()
{
  char* ChTempDir = NULL;
  char* ChHomeDir = NULL;
  char* ChUser = NULL;

  std::string HomeDir = "";
  std::string UserID = "";

  m_TempDir = "";
  m_HostName = "(unknown)";
  m_UserID = "(unknown)";

  // default directories
  // UNIX:
  //  Temp directory : using TMPDIR, TMP or TEMP env. var.
  //  User directory for Openfluid : using HOME env. var. + .openfluid/engine suffix
  // WIN32:
  //  Temp directory : using TEMP env. var.
  //  User directory for Openfluid : using APPDATA env. var. + openfluid/engine suffix

#if defined __unix__ || defined __APPLE__

  char ChHostName[512];

  ChTempDir = std::getenv("TMPDIR");
  if (ChTempDir == NULL ) ChTempDir = std::getenv("TMP");
  if (ChTempDir == NULL ) ChTempDir = std::getenv("TEMP");
  if (ChTempDir == NULL ) m_TempDir = "/tmp";
  else m_TempDir = ChTempDir;

  ChHomeDir = std::getenv("HOME");
  if (ChHomeDir == NULL) HomeDir = m_TempDir;
  else HomeDir = ChHomeDir;

  ChUser = std::getenv("USER");
  if (ChUser != NULL) m_UserID = ChUser;

  if (gethostname(ChHostName,512) == 0 )
  {
    m_HostName = ChHostName;
  }
#endif

#if WIN32

  char* ChHostName = NULL;

  ChTempDir = std::getenv("TEMP");
  if (ChTempDir != NULL ) m_TempDir = ChTempDir;

  ChHomeDir = std::getenv("APPDATA");
  if (ChHomeDir == NULL) HomeDir = m_TempDir;
  else HomeDir = ChHomeDir;

  ChUser = std::getenv("USERNAME");
  if (ChUser != NULL) m_UserID = ChUser;

  ChHostName= std::getenv("COMPUTERNAME");
  if (ChHostName != NULL) m_HostName = ChHostName;
#endif


  HomeDir = boost::filesystem::path(HomeDir).string();
  m_TempDir = boost::filesystem::path(m_TempDir).string();

#if WIN32
  m_UserDataDir = boost::filesystem::path(HomeDir+"/"+openfluid::config::RELATIVEDIR).string();
#endif

#if defined __unix__ || defined __APPLE__
  m_UserDataDir = boost::filesystem::path(HomeDir+"/."+openfluid::config::RELATIVEDIR).string();
#endif

  m_OutputDir = boost::filesystem::path(m_UserDataDir + "/" + openfluid::config::DEFAULT_OUTDIR).string();
  m_InputDir =  boost::filesystem::path(m_UserDataDir + "/" + openfluid::config::DEFAULT_INDIR).string();


  m_ClearOutputDir = false;
  m_QuietRun = false;
  m_VerboseRun = false;
  m_WriteResults = true;
  m_WriteSimReport = true;

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


  // plugins search order:
  //  1) command line paths,
  //  2) environment var OPENFLUID_FUNCS_PATH
  //  3) user directory,
  //  4) install directory

  // env var
  char *PATHEnvVar;
  PATHEnvVar = std::getenv("OPENFLUID_FUNCS_PATH");

  if (PATHEnvVar != NULL)
  {
    addExtraPluginsPaths(std::string(PATHEnvVar));
  }

  // user dir
  m_PlugsDirs.push_back(boost::filesystem::path(m_UserDataDir + "/" + openfluid::config::PLUGINS_SUBDIR).string());


  // install path
  std::string PluginsInstallPath = boost::filesystem::path(openfluid::config::INSTALL_PREFIX + "/" + openfluid::config::PLUGINS_STDDIR).string();
  char *INSTALLEnvVar;

  INSTALLEnvVar = std::getenv("OPENFLUID_INSTALL_PREFIX");

  if (INSTALLEnvVar != NULL)
  {
    PluginsInstallPath = boost::filesystem::path(std::string(INSTALLEnvVar) + "/" + openfluid::config::PLUGINS_STDDIR).string();
  }

  m_PlugsDirs.push_back(PluginsInstallPath);

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


RuntimeEnvironment* RuntimeEnvironment::getInstance()
{
  if (mp_Singleton == NULL) mp_Singleton = new RuntimeEnvironment();
  return mp_Singleton;
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
  ExtraPaths = openfluid::tools::SplitString(SemicolonSeparatedPaths,":");
#endif

#if WIN32
  ExtraPaths = SplitString(SemicolonSeparatedPaths,";");
#endif


  for (int i = ExtraPaths.size()-1 ; i>=0 ; i--) m_PlugsDirs.insert(m_PlugsDirs.begin(),1,openfluid::tools::RemoveTrailingSlashes(ExtraPaths[i]));
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


} } //namespaces

