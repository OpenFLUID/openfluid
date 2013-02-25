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
 @file
 @brief implements ...

 @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/base/RuntimeEnv.hpp>

#include <glibmm/miscutils.h>
#include <boost/filesystem/operations.hpp>

#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/tools/SwissTools.hpp>
#include <openfluid/config.hpp>

#if defined __unix__ || defined __APPLE__
#include <unistd.h>
#endif

namespace openfluid {
namespace base {

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

  m_FunctionsMaxNumThreads = openfluid::config::FUNCTIONS_MAXNUMTHREADS;

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

#if defined __unix__ || defined __APPLE__
  char ChHostName[512];

  if (gethostname(ChHostName, 512) == 0)
  {
    m_HostName = ChHostName;
  }
#endif

#if WIN32
  char* ChHostName = NULL;

  ChHostName= std::getenv("COMPUTERNAME");
  if (ChHostName != NULL) m_HostName = ChHostName;
#endif

  m_HomeDir = boost::filesystem::path(Glib::get_home_dir()).string();
  m_TempDir = boost::filesystem::path(Glib::get_tmp_dir() + "/openfluid-tmp").string();
  m_UserID = Glib::get_user_name();

#if WIN32
  m_UserDataDir = boost::filesystem::path(m_HomeDir+"/"+openfluid::config::RELATIVEDIR).string();
#endif

#if defined __unix__ || defined __APPLE__
  m_UserDataDir = boost::filesystem::path(m_HomeDir + "/."
      + openfluid::config::RELATIVEDIR).string();
#endif

  m_OutputDir = boost::filesystem::path(m_UserDataDir + "/"
      + openfluid::config::DEFAULT_OUTDIR).string();
  m_InputDir = boost::filesystem::path(m_UserDataDir + "/"
      + openfluid::config::DEFAULT_INDIR).string();
  m_MarketBagDir = boost::filesystem::path(m_UserDataDir + "/"
      + openfluid::config::MARKETBAG_SUBDIR).string();

  m_MarketBagVersionDir = boost::filesystem::path(m_MarketBagDir + "/"
      + m_Version).string();
  m_MarketBagFuncVersionDir = boost::filesystem::path(m_MarketBagVersionDir + "/"
      + "functions").string();
  m_MarketBagObsVersionDir = boost::filesystem::path(m_MarketBagVersionDir + "/"
      + "observers").string();
  m_MarketBagBinSubDir = boost::filesystem::path(m_Arch).string();
  m_MarketBagSrcSubDir = boost::filesystem::path("src").string();

  m_DefaultConfigFilePath = boost::filesystem::path(m_UserDataDir + "/"
      + openfluid::config::DEFAULT_CONFIGFILE).string();

  // ====== Default values ======

  m_ClearOutputDir = false;
  m_WriteResults = true;
  m_WriteSimReport = true;

  m_FilesBufferSize = openfluid::config::DEFAULT_OUTFILES_BUFFER_KB * 1024;
  m_ValuesBufferSize = 0;
  m_IsUserValuesBufferSize = false;

  m_TimeStep = 0;

  // ====== Function environnement ======

  mp_WareEnv = new openfluid::base::EnvironmentProperties();

  mp_WareEnv->setValue("dir.input", m_InputDir);
  mp_WareEnv->setValue("dir.output", m_OutputDir);
  mp_WareEnv->setValue("dir.temp", m_TempDir);

  mp_WareEnv->setValue("mode.cleanoutput", m_ClearOutputDir);
  mp_WareEnv->setValue("mode.saveresults", m_WriteResults);
  mp_WareEnv->setValue("mode.writereport", m_WriteSimReport);

  // ====== Function plugins search order ======
  //  1) command line paths,
  //  2) environment var OPENFLUID_FUNCS_PATH
  //  3) user directory,
  //  4) market-bag directory
  //  5) install directory

  // env var
  char *FUNCSPATHEnvVar;
  FUNCSPATHEnvVar = std::getenv("OPENFLUID_FUNCS_PATH");

  if (FUNCSPATHEnvVar != NULL)
  {
    addExtraFunctionsPluginsPaths(std::string(FUNCSPATHEnvVar));
  }

  // user dir
  m_DefaultFunctionsPlugsDirs.push_back(boost::filesystem::path(m_UserDataDir + "/"
      + openfluid::config::FUNCTIONS_PLUGINS_SUBDIR).string());

  // market-bag dir (for current version)
  m_DefaultFunctionsPlugsDirs.push_back(m_MarketBagFuncVersionDir + m_MarketBagBinSubDir);

  // install directory
  std::string FunctionsPluginsInstallPath = boost::filesystem::path(m_InstallPrefix
      + "/" + openfluid::config::FUNCTIONS_PLUGINS_STDDIR).string();
  m_DefaultFunctionsPlugsDirs.push_back(FunctionsPluginsInstallPath);



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
      + openfluid::config::OBSERVERS_PLUGINS_SUBDIR).string());

  // TODO market-bag dir (for current version)
   m_DefaultObserversPlugsDirs.push_back(m_MarketBagObsVersionDir + m_MarketBagBinSubDir);

  // install directory
  std::string ObserversPluginsInstallPath = boost::filesystem::path(m_InstallPrefix
      + "/" + openfluid::config::OBSERVERS_PLUGINS_STDDIR).string();
  m_DefaultObserversPlugsDirs.push_back(ObserversPluginsInstallPath);



  // set ignition date time
  resetIgnitionDateTime();

  // build simulation ID
  resetSimulationID();

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


void RuntimeEnvironment::addExtraFunctionsPluginsPaths(
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
    m_ExtraFunctionsPlugsDirs.insert(m_ExtraFunctionsPlugsDirs.begin(), 1,
        openfluid::tools::RemoveTrailingSlashes(ExtraPaths[i]));
}


// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getFunctionPluginFullPath(std::string Filename)
{

  std::vector<std::string> PluginsPaths = getFunctionsPluginsPaths();
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


std::string RuntimeEnvironment::getLocaleDir() const
{
  return boost::filesystem::path(m_InstallPrefix + "/" + openfluid::config::NLS_SHARE_LOCALE_PATH).string();
}


// =====================================================================
// =====================================================================


void RuntimeEnvironment::resetSimulationID()
{
  std::string BaseStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  m_SimulationID = boost::gregorian::to_iso_string(m_IgnitionDateTime.date())
      + "-";

  srand(time(NULL));

  for (int i = 0; i < 6; i++)
  {
    m_SimulationID = m_SimulationID + BaseStr[rand() % 26];
  }
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
  setInputDir(ProjectManager::getInstance()->getInputDir());
  setOutputDir(ProjectManager::getInstance()->getOutputDir());
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

