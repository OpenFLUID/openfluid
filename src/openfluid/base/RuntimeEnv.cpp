/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/

/**
 @file
 @brief implements ...

 @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <QDir>

#include <openfluid/base/RuntimeEnv.hpp>

#include <boost/filesystem/operations.hpp>

#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/tools/SwissTools.hpp>
#include <openfluid/config.hpp>

#if defined __unix__ || defined __APPLE__
#include <unistd.h>
#endif


namespace openfluid { namespace base {


RuntimeEnvironment* RuntimeEnvironment::mp_Singleton = NULL;

RuntimeEnvironment::RuntimeEnvironment() :
    m_Profiling(false), m_IsLinkedToProject(false)
{
  std::string UserID = "";

  m_Version = openfluid::config::MAJOR_VERSION + "."
      + openfluid::config::MINOR_VERSION + "."
      + openfluid::config::PATCH_VERSION;
  m_FullVersion = openfluid::config::FULL_VERSION;
  m_MajorMinorVersion = openfluid::config::MAJOR_VERSION + "."
      + openfluid::config::MINOR_VERSION;

  m_TempDir = "";
  m_HostName = "(unknown)";
  m_UserID = "(unknown)";
  m_Arch = "unknown";

  m_SimulatorsMaxNumThreads = openfluid::config::SIMULATORS_MAXNUMTHREADS;

  // ====== System architecture ======

#if linux
#if __i386__
  m_Arch = "linux-i386";
#endif
#ifdef __x86_64__
  m_Arch = "linux-x86-64";
#endif
#endif

#if WIN32
  m_Arch = "win32";
#endif

#if WIN64
  m_Arch = "win64";
#endif

#if __APPLE__
#if __LP64__
  m_Arch = "osx64";
#else
  m_Arch = "osx32";
#endif
#endif

  m_InstallPrefix = openfluid::config::INSTALL_PREFIX;

  char *INSTALLEnvVar;
  INSTALLEnvVar = std::getenv("OPENFLUID_INSTALL_PREFIX");

  if (INSTALLEnvVar != NULL)
  {
    m_InstallPrefix
        = boost::filesystem::path(std::string(INSTALLEnvVar)).string();
  }

  // ====== Default directories ======
  // UNIX:
  //  User directory for Openfluid : home dir + .openfluid subdir
  // WIN32:
  //  User directory for Openfluid : home dir + openfluid subdir

  m_HomeDir = QDir::homePath().toStdString();
  m_TempDir = QDir(QDir::tempPath()+"/openfluid-tmp").absolutePath().toStdString();

#if defined __unix__ || defined __APPLE__
  char ChHostName[512];

  if (gethostname(ChHostName, 512) == 0)
  {
    m_HostName = ChHostName;
  }

  char* ChUserName = NULL;
  ChUserName= std::getenv("USER");
  if (ChUserName != NULL) m_UserID= ChUserName;

  m_UserDataDir = boost::filesystem::path(m_HomeDir + "/."
        + openfluid::config::RELATIVEDIR).string();
#endif

#if WIN32
  char* ChHostName = NULL;
  ChHostName= std::getenv("COMPUTERNAME");
  if (ChHostName != NULL) m_HostName = ChHostName;

  char* ChUserName = NULL;
  ChUserName= std::getenv("USERNAME");
  if (ChUserName != NULL) m_UserID= ChUserName;

  m_UserDataDir = boost::filesystem::path(m_HomeDir+"/"+openfluid::config::RELATIVEDIR).string();
#endif


  char *USERDATAEnvVar;
  USERDATAEnvVar = std::getenv("OPENFLUID_USERDATA_PATH");

  if (USERDATAEnvVar != NULL)
  {
    m_UserDataDir
        = boost::filesystem::path(std::string(USERDATAEnvVar)).string();
  }


  char *TEMPEnvVar;
  TEMPEnvVar = std::getenv("OPENFLUID_TEMP_PATH");

  if (TEMPEnvVar != NULL)
  {
    m_TempDir
        = boost::filesystem::path(std::string(TEMPEnvVar)).string();
  }


  m_OutputDir = boost::filesystem::path(m_UserDataDir + "/"
      + openfluid::config::DEFAULT_OUTDIR).string();
  m_InputDir = boost::filesystem::path(m_UserDataDir + "/"
      + openfluid::config::DEFAULT_INDIR).string();

  // ====== Market ======

  m_MarketBagDir = boost::filesystem::path(m_UserDataDir + "/"
      + openfluid::config::MARKETBAG_SUBDIR).string();
  m_MarketBagVersionDir = boost::filesystem::path(m_MarketBagDir + "/" + m_Version).string();

  m_MarketBagSimVersionDir = boost::filesystem::path(m_MarketBagVersionDir + "/" + "simulators").string();
  m_MarketBagObsVersionDir = boost::filesystem::path(m_MarketBagVersionDir + "/" + "observers").string();
  m_MarketBagBuildVersionDir = boost::filesystem::path(m_MarketBagVersionDir + "/" + "builderexts").string();
  m_MarketBagDataVersionDir = boost::filesystem::path(m_MarketBagVersionDir + "/" + "datasets").string();
  m_MarketBagBinSubDir = m_Arch;
  m_MarketBagSrcSubDir = "src";


  // ====== Config file ======

  m_DefaultConfigFilePath = boost::filesystem::path(m_UserDataDir + "/"
      + openfluid::config::DEFAULT_CONFIGFILE).string();


  // ====== Examples ======

  m_UserExamplesDir = boost::filesystem::path(m_UserDataDir + "/"
      + openfluid::config::EXAMPLES_SUBDIR).string();

  m_ProvidedExamplesDir = boost::filesystem::path(m_InstallPrefix + "/"
        + openfluid::config::EXAMPLES_STDDIR).string();


  // ====== Default values ======

  m_ClearOutputDir = false;
  m_WriteResults = true;
  m_WriteSimReport = true;

  m_ValuesBufferSize = 0;
  m_IsUserValuesBufferSize = false;

  m_TimeStep = 0;

  // ====== Simulator environnement ======

  mp_WareEnv = new openfluid::base::EnvironmentProperties();

  mp_WareEnv->setValue("dir.input", m_InputDir);
  mp_WareEnv->setValue("dir.output", m_OutputDir);
  mp_WareEnv->setValue("dir.temp", m_TempDir);

  mp_WareEnv->setValue("mode.cleanoutput", m_ClearOutputDir);
  mp_WareEnv->setValue("mode.saveresults", m_WriteResults);
  mp_WareEnv->setValue("mode.writereport", m_WriteSimReport);

  // ====== Simulator plugins search order ======
  //  1) command line paths,
  //  2) environment var OPENFLUID_SIMS_PATH
  //  3) user directory,
  //  4) market-bag directory
  //  5) install directory

  // env var
  char *SIMSPATHEnvVar;
  SIMSPATHEnvVar = std::getenv("OPENFLUID_SIMS_PATH");

  if (SIMSPATHEnvVar != NULL)
  {
    addExtraSimulatorsPluginsPaths(std::string(SIMSPATHEnvVar));
  }

  // user dir
  m_DefaultSimulatorsPlugsDirs.push_back(boost::filesystem::path(m_UserDataDir + "/"
      + openfluid::config::SIMULATORS_PLUGINS_USRDIR).string());

  // market-bag dir (for current version)
  m_DefaultSimulatorsPlugsDirs.push_back(boost::filesystem::path(m_MarketBagSimVersionDir
      + "/" + m_MarketBagBinSubDir).string());

  // install directory
  std::string SimulatorsPluginsInstallPath = boost::filesystem::path(m_InstallPrefix
      + "/" + openfluid::config::SIMULATORS_PLUGINS_STDDIR).string();
  m_DefaultSimulatorsPlugsDirs.push_back(SimulatorsPluginsInstallPath);



  // ====== Observer plugins search order ======
  //  1) command line paths,
  //  2) environment var OPENFLUID_OBSS_PATH
  //  3) user directory,
  //  4) market-bag directory
  //  5) install directory

  // env var
  char *OBSSPATHEnvVar;
  OBSSPATHEnvVar = std::getenv("OPENFLUID_OBSS_PATH");

  if (OBSSPATHEnvVar != NULL)
  {
    addExtraObserversPluginsPaths(std::string(OBSSPATHEnvVar));
  }

  // user dir
  m_DefaultObserversPlugsDirs.push_back(boost::filesystem::path(m_UserDataDir + "/"
      + openfluid::config::OBSERVERS_PLUGINS_USRDIR).string());

  // market-bag dir (for current version)
   m_DefaultObserversPlugsDirs.push_back(boost::filesystem::path(m_MarketBagObsVersionDir
       + "/" + m_MarketBagBinSubDir).string());

  // install directory
  std::string ObserversPluginsInstallPath = boost::filesystem::path(m_InstallPrefix
      + "/" + openfluid::config::OBSERVERS_PLUGINS_STDDIR).string();
  m_DefaultObserversPlugsDirs.push_back(ObserversPluginsInstallPath);



  // set ignition date time
  resetIgnitionDateTime();

  m_EffectiveSimulationDuration = boost::posix_time::time_duration();

}

// =====================================================================
// =====================================================================


RuntimeEnvironment::~RuntimeEnvironment()
{

}

// =====================================================================
// =====================================================================


RuntimeEnvironment* RuntimeEnvironment::instance()
{
  if (mp_Singleton == NULL)
    mp_Singleton = new RuntimeEnvironment();
  return mp_Singleton;
}

// =====================================================================
// =====================================================================


void RuntimeEnvironment::setDateTimeOutputDir()
{
  m_OutputDir = boost::filesystem::path(m_UserDataDir + "/" + "OPENFLUID."
      + boost::posix_time::to_iso_string(m_IgnitionDateTime) + ".OUT").string();
}


// =====================================================================
// =====================================================================


void RuntimeEnvironment::addExtraSimulatorsPluginsPaths(
    std::string SemicolonSeparatedPaths)
{
  std::vector<std::string> ExtraPaths;

#if  defined __unix__ || defined __APPLE__
  ExtraPaths = openfluid::tools::SplitString(SemicolonSeparatedPaths, ":");
#endif

#if WIN32
  ExtraPaths = openfluid::tools::SplitString(SemicolonSeparatedPaths,";");
#endif

  for (int i = ExtraPaths.size() - 1; i >= 0; i--)
    m_ExtraSimulatorsPlugsDirs.insert(m_ExtraSimulatorsPlugsDirs.begin(), 1,
        openfluid::tools::RemoveTrailingSlashes(ExtraPaths[i]));
}


// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getSimulatorPluginFullPath(std::string Filename)
{

  std::vector<std::string> PluginsPaths = getSimulatorsPluginsPaths();
  std::string PlugFullPath = "";
  boost::filesystem::path TmpPath;

  unsigned int i = 0;

  while ((PlugFullPath.length() == 0) && (i < PluginsPaths.size()))
  {

    TmpPath = boost::filesystem::path(PluginsPaths[i] + "/" + Filename);

    if (boost::filesystem::exists(TmpPath))
      PlugFullPath = TmpPath.string();

    i++;
  }

  return PlugFullPath;
}

// =====================================================================
// =====================================================================


void RuntimeEnvironment::addExtraObserversPluginsPaths(
    std::string SemicolonSeparatedPaths)
{
  std::vector<std::string> ExtraPaths;

#if  defined __unix__ || defined __APPLE__
  ExtraPaths = openfluid::tools::SplitString(SemicolonSeparatedPaths, ":");
#endif

#if WIN32
  ExtraPaths = openfluid::tools::SplitString(SemicolonSeparatedPaths,";");
#endif

  for (int i = ExtraPaths.size() - 1; i >= 0; i--)
    m_ExtraObserversPlugsDirs.insert(m_ExtraObserversPlugsDirs.begin(), 1,
        openfluid::tools::RemoveTrailingSlashes(ExtraPaths[i]));
}

// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getObserverPluginFullPath(std::string Filename)
{

  std::vector<std::string> PluginsPaths = getObserversPluginsPaths();
  std::string PlugFullPath = "";
  boost::filesystem::path TmpPath;

  unsigned int i = 0;

  while ((PlugFullPath.length() == 0) && (i < PluginsPaths.size()))
  {

    TmpPath = boost::filesystem::path(PluginsPaths[i] + "/" + Filename);

    if (boost::filesystem::exists(TmpPath))
      PlugFullPath = TmpPath.string();

    i++;
  }

  return PlugFullPath;
}


// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getCommonResourcesDir() const
{
  return boost::filesystem::path(m_InstallPrefix + "/"
      + openfluid::config::SHARE_COMMON_INSTALL_PATH).string();
}

// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getCommonResourceFilePath(
    std::string RelativeFilePath) const
{
  return boost::filesystem::path(m_InstallPrefix + "/"
      + openfluid::config::SHARE_COMMON_INSTALL_PATH + "/" + RelativeFilePath).string();
}

// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getAppResourcesDir(std::string AppName) const
{
  return boost::filesystem::path(m_InstallPrefix + "/"
      + openfluid::config::SHARE_APPS_INSTALL_PATH + "/" + AppName).string();
}

// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getAppResourceFilePath(std::string AppName,
    std::string RelativeFilePath) const
{
  return boost::filesystem::path(m_InstallPrefix + "/"
      + openfluid::config::SHARE_APPS_INSTALL_PATH + "/" + AppName + "/"
      + RelativeFilePath).string();
}


// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getTranslationsDir() const
{
  return boost::filesystem::path(m_InstallPrefix + "/" + openfluid::config::SHARE_TRANSLATIONS_PATH).string();
}


// =====================================================================
// =====================================================================


void RuntimeEnvironment::resetIgnitionDateTime()
{
  m_IgnitionDateTime = boost::posix_time::microsec_clock::local_time();
}


// =====================================================================
// =====================================================================


void RuntimeEnvironment::setSimulationTimeInformation(
    openfluid::core::DateTime StartTime, openfluid::core::DateTime EndTime,
    int TimeStep)
{
  m_StartTime = StartTime;
  m_EndTime = EndTime;
  m_TimeStep = TimeStep;
}

// =====================================================================
// =====================================================================


void RuntimeEnvironment::linkToProject()
{
  m_IsLinkedToProject = true;
  setInputDir(ProjectManager::instance()->getInputDir());
  setOutputDir(ProjectManager::instance()->getOutputDir());
}


// =====================================================================
// =====================================================================


void RuntimeEnvironment::detachFromProject()
{
  m_IsLinkedToProject = false;
  setInputDir("");
  setOutputDir("");
}


} } //namespaces

