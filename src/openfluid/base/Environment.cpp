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
  @file Environment.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <thread>

#include <QDir>

#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/config.hpp>


// OpenFLUID:stylecheck:!incs


#if defined(OPENFLUID_OS_UNIX)
#include <unistd.h>
#endif


namespace openfluid { namespace base {


bool Environment::m_Initialized = false;
std::string Environment::m_SystemArch;
std::string Environment::m_HostName;
std::string Environment::m_UserName;
std::string Environment::m_Version;
std::string Environment::m_VersionFull;
std::string Environment::m_VersionMajorMinor;
std::string Environment::m_InstallPrefix;
std::string Environment::m_TempDir;
std::string Environment::m_UserHomeDir;
std::string Environment::m_UserDataDir;
std::string Environment::m_ConfigFile;
std::vector<std::string> Environment::m_DefaultSimulatorsDirs;
std::vector<std::string> Environment::m_ExtraSimulatorsDirs;
std::vector<std::string> Environment::m_DefaultObserversDirs;
std::vector<std::string> Environment::m_ExtraObserversDirs;
std::vector<std::string> Environment::m_DefaultBuilderextsDirs;
std::vector<std::string> Environment::m_ExtraBuilderextsDirs;
std::string Environment::m_ProvidedExamplesDir;
std::string Environment::m_UserExamplesDir;
std::string Environment::m_UserExampleSimulatorsDir;
std::string Environment::m_MarketBagDir;
std::string Environment::m_MarketBagVersionDir;
std::string Environment::m_MarketBagSimulatorsDir;
std::string Environment::m_MarketBagObserversDir;
std::string Environment::m_MarketBagBuilderextsDir;
std::string Environment::m_MarketBagDataDir;
std::string Environment::m_MarketBagBinSubDir;
std::string Environment::m_MarketBagSrcSubDir;
std::string Environment::m_TranslationsDir;
std::string Environment::m_CommonResourcesDir;
std::string Environment::m_AppsResourcesDir;
int Environment::m_IdealThreadCount = -1;


// =====================================================================
// =====================================================================


void Environment::init()
{
  if (m_Initialized)
  {
    return;
  }


  // ====== System informations ======

  m_SystemArch = OPENFLUID_OS_STRLABEL;

#if defined(OPENFLUID_OS_UNIX)

  char ChHostName[512];

  if (gethostname(ChHostName, 512) == 0)
  {
    m_HostName = ChHostName;
  }

  char* ChUserName = nullptr;
  ChUserName= std::getenv("USER");
  if (ChUserName != nullptr)
  {
    m_UserName = std::string(ChUserName);
  }

#elif defined(OPENFLUID_OS_WINDOWS)

  char* ChHostName = nullptr;
  ChHostName= std::getenv("COMPUTERNAME");
  if (ChHostName != nullptr)
  {
    m_HostName = ChHostName;
  }

  char* ChUserName = nullptr;
  ChUserName= std::getenv("USERNAME");
  if (ChUserName != nullptr)
  {
    m_UserName = ChUserName;
  }

#endif


  // ====== OpenFLUID version ======

  m_Version = openfluid::config::VERSION_MAJOR+"." +
              openfluid::config::VERSION_MINOR+"." +
              openfluid::config::VERSION_PATCH;
  m_VersionFull = openfluid::config::VERSION_FULL;
  m_VersionMajorMinor = openfluid::config::VERSION_MAJOR+"." +
                        openfluid::config::VERSION_MINOR;


  // ====== System directories ======

  m_InstallPrefix = openfluid::config::INSTALL_PREFIX;

  char *INSTALLEnvVar;
  INSTALLEnvVar = std::getenv("OPENFLUID_INSTALL_PREFIX");

  if (INSTALLEnvVar != nullptr)
  {
    m_InstallPrefix = std::string(INSTALLEnvVar);
  }

  m_TempDir = QDir(QDir::tempPath()+"/openfluid-tmp").absolutePath().toStdString();

  char *TEMPEnvVar = std::getenv("OPENFLUID_TEMP_PATH");

  if (TEMPEnvVar != nullptr)
  {
    m_TempDir = std::string(TEMPEnvVar);
  }


  // ====== User directories ======

  m_UserHomeDir = QDir::homePath().toStdString();

  // UNIX: user directory for OpenFLUID : <m_HomeDir>/.openfluid
  // WINDOWS:user directory for OpenFLUID : <m_HomeDir>/openfluid
#if defined(OPENFLUID_OS_UNIX)
  m_UserDataDir = m_UserHomeDir + "/." + openfluid::config::RELATIVE_PATH;
#elif defined(OPENFLUID_OS_WINDOWS)
  m_UserDataDir = m_UserHomeDir+"/"+openfluid::config::RELATIVE_PATH;
#endif


  char *USERDATAEnvVar = std::getenv("OPENFLUID_USERDATA_PATH");

  if (USERDATAEnvVar != nullptr)
  {
    m_UserDataDir = std::string(USERDATAEnvVar);
  }


  // ====== Config file ======

  m_ConfigFile = m_UserDataDir + "/" + openfluid::config::DEFAULT_CONFIGFILE;


  // ====== Examples directories ======

  m_UserExamplesDir = m_UserDataDir + "/" + openfluid::config::EXAMPLES_PATH;

  m_UserExampleSimulatorsDir = m_UserExamplesDir + "/" + openfluid::config::WARESDEV_PATH + "/" + 
                                 openfluid::config::SIMULATORS_PATH;
  m_ProvidedExamplesDir = m_InstallPrefix + "/" + openfluid::config::EXAMPLES_STD_PATH;


  // ====== Market directories ======

  m_MarketBagDir = m_UserDataDir + "/" + openfluid::config::MARKETBAG_PATH;
  m_MarketBagVersionDir = m_MarketBagDir + "/" + m_Version;

  m_MarketBagSimulatorsDir = m_MarketBagVersionDir + "/" + openfluid::config::SIMULATORS_PATH;
  m_MarketBagObserversDir = m_MarketBagVersionDir + "/" + openfluid::config::OBSERVERS_PATH;
  m_MarketBagBuilderextsDir = m_MarketBagVersionDir + "/" + openfluid::config::BUILDEREXTS_PATH;
  m_MarketBagDataDir = m_MarketBagVersionDir + "/" + "datasets";
  m_MarketBagBinSubDir = m_SystemArch;
  m_MarketBagSrcSubDir = "src";


  // ====== Simulators directories ======
  // search order:
  //  1) command line paths
  //  2) environment var OPENFLUID_SIMS_PATH
  //  3) user directory
  //  4) market-bag directory
  //  5) install directory

  // env var
  char *SIMSPATHEnvVar;
  SIMSPATHEnvVar = std::getenv("OPENFLUID_SIMS_PATH");

  if (SIMSPATHEnvVar != nullptr)
  {
    addExtraSimulatorsDirs(std::string(SIMSPATHEnvVar));
  }

  // user dir
  m_DefaultSimulatorsDirs.push_back(m_UserDataDir + "/" + openfluid::config::SIMULATORS_WARESBIN_USR_PATH);

#if OPENFLUID_MARKET_ENABLED
  // market-bag dir (for current version)
  m_DefaultSimulatorsDirs.push_back(m_MarketBagSimulatorsDir + "/" + m_MarketBagBinSubDir);
#endif

  // install directory
  std::string SimulatorsInstallPath =  m_InstallPrefix + "/" + openfluid::config::SIMULATORS_WARESBIN_STD_PATH;
  m_DefaultSimulatorsDirs.push_back(SimulatorsInstallPath);


  // ====== Observers directories ======
  // search order:
  //  1) command line paths
  //  2) environment var OPENFLUID_OBSS_PATH
  //  3) user directory
  //  4) market-bag directory
  //  5) install directory

  // env var
  char *OBSSPATHEnvVar;
  OBSSPATHEnvVar = std::getenv("OPENFLUID_OBSS_PATH");

  if (OBSSPATHEnvVar != nullptr)
  {
    addExtraObserversDirs(std::string(OBSSPATHEnvVar));
  }

  // user dir
  m_DefaultObserversDirs.push_back(m_UserDataDir + "/" + openfluid::config::OBSERVERS_WARESBIN_USR_PATH);

#if OPENFLUID_MARKET_ENABLED
  // market-bag dir (for current version)
   m_DefaultObserversDirs.push_back(m_MarketBagObserversDir + "/" + m_MarketBagBinSubDir);
#endif

  // install directory
  std::string ObserversInstallPath = m_InstallPrefix + "/" + openfluid::config::OBSERVERS_WARESBIN_STD_PATH;
  m_DefaultObserversDirs.push_back(ObserversInstallPath);


  // ====== Builderexts directories ======
  // search order:
  //  1) command line paths
  //  2) environment var OPENFLUID_BEXTS_PATH
  //  3) user directory
  //  4) market-bag directory
  //  5) install directory

  // env var
  char *BEXTSPATHEnvVar;
  BEXTSPATHEnvVar = std::getenv("OPENFLUID_BEXTS_PATH");

  if (BEXTSPATHEnvVar != nullptr)
  {
    addExtraBuilderextsDirs(std::string(BEXTSPATHEnvVar));
  }

  // user dir
  m_DefaultBuilderextsDirs.push_back(m_UserDataDir + "/" + openfluid::config::BUILDEREXTS_WARESBIN_USR_PATH);

#if OPENFLUID_MARKET_ENABLED
  // market-bag dir (for current version)
   m_DefaultBuilderextsDirs.push_back(m_MarketBagBuilderextsDir + "/" + m_MarketBagBinSubDir);
#endif

  // install directory
  std::string BuilderextsInstallPath = m_InstallPrefix + "/" + openfluid::config::BUILDEREXTS_WARESBIN_STD_PATH;
  m_DefaultBuilderextsDirs.push_back(BuilderextsInstallPath);


  // ====== Resources and translations directories ======

  m_CommonResourcesDir = m_InstallPrefix + "/" + openfluid::config::SHARE_COMMON_INSTALL_PATH;
  m_AppsResourcesDir = m_InstallPrefix + "/" + openfluid::config::SHARE_APPS_INSTALL_PATH;

  m_TranslationsDir = m_InstallPrefix + "/" + openfluid::config::SHARE_TRANSLATIONS_INSTALL_PATH;


  // ====== Threads ======

  m_IdealThreadCount = std::thread::hardware_concurrency();

  // if ideal thread count cannot be computed, set it to 1 thread
  if (!m_IdealThreadCount)
  {
    m_IdealThreadCount = 1;
  }


  m_Initialized = true;
}


// =====================================================================
// =====================================================================


std::string Environment::getWareFullPath(const std::vector<std::string>& Dirs, const std::string& Filename)
{
  for (auto CurrentDir : Dirs)
  {
    std::string TmpPath = CurrentDir + "/" + Filename;

    if (openfluid::tools::Filesystem::isFile(TmpPath))
    {
      return TmpPath;
    }
  }

  return "";
}


// =====================================================================
// =====================================================================


void Environment::prepareUserDataDirectory()
{
  openfluid::tools::Filesystem::makeDirectory(m_UserDataDir);

  openfluid::tools::Filesystem::makeDirectory(getUserDataFullPath(openfluid::config::SIMULATORS_WARESBIN_USR_PATH));
  openfluid::tools::Filesystem::makeDirectory(getUserDataFullPath(openfluid::config::OBSERVERS_WARESBIN_USR_PATH));
  openfluid::tools::Filesystem::makeDirectory(getUserDataFullPath(openfluid::config::BUILDEREXTS_WARESBIN_USR_PATH));

  openfluid::tools::Filesystem::makeDirectory(getUserDataFullPath(openfluid::config::WORKSPACE_PATH));
  openfluid::tools::Filesystem::makeDirectory(getUserDataFullPath(openfluid::config::WORKSPACE_PATH+"/"+
                                                                  openfluid::config::PROJECTS_PATH));
  openfluid::tools::Filesystem::makeDirectory(getUserDataFullPath(openfluid::config::WORKSPACE_PATH+"/"+
                                                                  openfluid::config::WARESDEV_PATH));
  openfluid::tools::Filesystem::makeDirectory(getUserDataFullPath(openfluid::config::WORKSPACE_PATH+"/"+
                                                                  openfluid::config::WARESDEV_PATH+"/"+
                                                                  openfluid::config::SIMULATORS_PATH));
  openfluid::tools::Filesystem::makeDirectory(getUserDataFullPath(openfluid::config::WORKSPACE_PATH+"/"+
                                                                  openfluid::config::WARESDEV_PATH+"/"+
                                                                  openfluid::config::OBSERVERS_PATH));
  openfluid::tools::Filesystem::makeDirectory(getUserDataFullPath(openfluid::config::WORKSPACE_PATH+"/"+
                                                                  openfluid::config::WARESDEV_PATH+"/"+
                                                                  openfluid::config::BUILDEREXTS_PATH));
}


// =====================================================================
// =====================================================================


std::vector<std::string> Environment::getSimulatorsDirs()
{
  std::vector<std::string> ComposedDirs(m_ExtraSimulatorsDirs);
  ComposedDirs.insert(ComposedDirs.end(),
                       m_DefaultSimulatorsDirs.begin(),
                       m_DefaultSimulatorsDirs.end());
  return ComposedDirs;
}


// =====================================================================
// =====================================================================


void Environment::addExtraSimulatorsDirs(const std::string& Dirs)
{
  std::vector<std::string> ExtraDirs;

  ExtraDirs = openfluid::tools::splitString(Dirs, openfluid::tools::Filesystem::pathsListSeparator());

  for (int i = ExtraDirs.size() - 1; i >= 0; i--)
  {
    m_ExtraSimulatorsDirs.insert(m_ExtraSimulatorsDirs.begin(), 1,
                                 openfluid::tools::removeTrailingSlashes(ExtraDirs[i]));
  }
}


// =====================================================================
// =====================================================================


void Environment::resetExtraSimulatorsDirs()
{
  m_ExtraSimulatorsDirs.clear();
}


// =====================================================================
// =====================================================================


std::string Environment::getSimulatorFullPath(const std::string& Filename)
{
  return getWareFullPath(getSimulatorsDirs(),Filename);
}


// =====================================================================
// =====================================================================


std::vector<std::string> Environment::getObserversDirs()
{
  std::vector<std::string> ComposedDirs(m_ExtraObserversDirs);
  ComposedDirs.insert(ComposedDirs.end(),m_DefaultObserversDirs.begin(),m_DefaultObserversDirs.end());
  return ComposedDirs;
}


// =====================================================================
// =====================================================================


void Environment::addExtraObserversDirs(const std::string& Dirs)
{
  std::vector<std::string> ExtraDirs;

  ExtraDirs = openfluid::tools::splitString(Dirs,openfluid::tools::Filesystem::pathsListSeparator());

  for (int i = ExtraDirs.size() - 1; i >= 0; i--)
  {
    m_ExtraObserversDirs.insert(m_ExtraObserversDirs.begin(), 1,
                                openfluid::tools::removeTrailingSlashes(ExtraDirs[i]));
  }
}


// =====================================================================
// =====================================================================


void Environment::resetExtraObserversDirs()
{
  m_ExtraObserversDirs.clear();
}


// =====================================================================
// =====================================================================


std::string Environment::getObserverFullPath(const std::string& Filename)
{
  return getWareFullPath(getObserversDirs(),Filename);
}


// =====================================================================
// =====================================================================


std::vector<std::string> Environment::getBuilderextsDirs()
{
  std::vector<std::string> ComposedDirs(m_ExtraBuilderextsDirs);
  ComposedDirs.insert(ComposedDirs.end(),
                       m_DefaultBuilderextsDirs.begin(),
                       m_DefaultBuilderextsDirs.end());
  return ComposedDirs;
}


// =====================================================================
// =====================================================================


void Environment::addExtraBuilderextsDirs(const std::string& Dirs)
{
  std::vector<std::string> ExtraDirs;

  ExtraDirs = openfluid::tools::splitString(Dirs,openfluid::tools::Filesystem::pathsListSeparator());

  for (int i = ExtraDirs.size() - 1; i >= 0; i--)
  {
    m_ExtraBuilderextsDirs.insert(m_ExtraBuilderextsDirs.begin(), 1,
                                  openfluid::tools::removeTrailingSlashes(ExtraDirs[i]));
  }
}


// =====================================================================
// =====================================================================


void Environment::resetExtraBuilderextsDirs()
{
  m_ExtraBuilderextsDirs.clear();
}


// =====================================================================
// =====================================================================


std::string Environment::getBuilderextFullPath(const std::string& Filename)
{
  return getWareFullPath(getBuilderextsDirs(),Filename);
}


} }  // namespaces
