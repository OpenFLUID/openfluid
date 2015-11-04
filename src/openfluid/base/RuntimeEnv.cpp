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
  @file RuntimeEnv.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <QDir>
#include <QDateTime>

#include <openfluid/base/RuntimeEnv.hpp>

#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>

#include <openfluid/config.hpp>

#if defined(OPENFLUID_OS_UNIX)
#include <unistd.h>
#endif


namespace openfluid { namespace base {


OPENFLUID_SINGLETON_INITIALIZATION(RuntimeEnvironment)


RuntimeEnvironment::RuntimeEnvironment() :
  m_Version(openfluid::config::VERSION_MAJOR+"."+openfluid::config::VERSION_MINOR+"."+openfluid::config::VERSION_PATCH),
  m_FullVersion(openfluid::config::VERSION_FULL),
  m_MajorMinorVersion(openfluid::config::VERSION_MAJOR+"."+openfluid::config::VERSION_MINOR),
  m_InstallPrefix(openfluid::config::INSTALL_PREFIX),
  m_Arch(OPENFLUID_OS_STRLABEL),
  m_SimulatorsMaxNumThreads(openfluid::config::SIMULATORS_MAXNUMTHREADS),
  m_Profiling(false), m_IsLinkedToProject(false)
{

  char *INSTALLEnvVar;
  INSTALLEnvVar = std::getenv("OPENFLUID_INSTALL_PREFIX");

  if (INSTALLEnvVar != nullptr)
  {
    m_InstallPrefix = std::string(INSTALLEnvVar);
  }

  // ====== Default directories ======
  // UNIX:
  //  User directory for OpenFLUID : home dir + .openfluid subdir
  // WINDOWS:
  //  User directory for OpenFLUID : home dir + openfluid subdir

  m_HomeDir = QDir::homePath().toStdString();
  m_TempDir = QDir(QDir::tempPath()+"/openfluid-tmp").absolutePath().toStdString();

#if defined(OPENFLUID_OS_UNIX)

  char ChHostName[512];

  if (gethostname(ChHostName, 512) == 0)
  {
    m_HostName = ChHostName;
  }

  char* ChUserName = nullptr;
  ChUserName= std::getenv("USER");
  if (ChUserName != nullptr) m_UserID= ChUserName;

  m_UserDataDir = m_HomeDir + "/." + openfluid::config::RELATIVE_PATH;

#elif defined(OPENFLUID_OS_WINDOWS)

  char* ChHostName = nullptr;
  ChHostName= std::getenv("COMPUTERNAME");
  if (ChHostName != nullptr) m_HostName = ChHostName;

  char* ChUserName = nullptr;
  ChUserName= std::getenv("USERNAME");
  if (ChUserName != nullptr) m_UserID= ChUserName;

  m_UserDataDir = m_HomeDir+"/"+openfluid::config::RELATIVE_PATH;

#endif


  char *USERDATAEnvVar;
  USERDATAEnvVar = std::getenv("OPENFLUID_USERDATA_PATH");

  if (USERDATAEnvVar != nullptr)
  {
    m_UserDataDir = std::string(USERDATAEnvVar);
  }


  char *TEMPEnvVar;
  TEMPEnvVar = std::getenv("OPENFLUID_TEMP_PATH");

  if (TEMPEnvVar != nullptr)
  {
    m_TempDir = std::string(TEMPEnvVar);
  }


  m_OutputDir = m_UserDataDir + "/" + openfluid::config::DEFAULT_OUTPUT_PATH;
  m_InputDir = m_UserDataDir + "/" + openfluid::config::DEFAULT_INPUT_PATH;

  // ====== Market ======

  m_MarketBagDir = m_UserDataDir + "/" + openfluid::config::MARKETBAG_PATH;
  m_MarketBagVersionDir = m_MarketBagDir + "/" + m_Version;

  m_MarketBagSimVersionDir = m_MarketBagVersionDir + "/" + "simulators";
  m_MarketBagObsVersionDir = m_MarketBagVersionDir + "/" + "observers";
  m_MarketBagBuildVersionDir = m_MarketBagVersionDir + "/" + "builderexts";
  m_MarketBagDataVersionDir = m_MarketBagVersionDir + "/" + "datasets";
  m_MarketBagBinSubDir = m_Arch;
  m_MarketBagSrcSubDir = "src";


  // ====== Config file ======

  m_DefaultConfigFilePath = m_UserDataDir + "/" + openfluid::config::DEFAULT_CONFIGFILE;


  // ====== Examples ======

  m_UserExamplesDir = m_UserDataDir + "/" + openfluid::config::EXAMPLES_PATH;

  m_ProvidedExamplesDir = m_InstallPrefix + "/" + openfluid::config::EXAMPLES_STD_PATH;


  // ====== Default values ======

  m_ClearOutputDir = false;
  m_WriteResults = true;
  m_WriteSimReport = true;

  m_ValuesBufferSize = 0;
  m_IsUserValuesBufferSize = false;


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

  if (SIMSPATHEnvVar != nullptr)
  {
    addExtraSimulatorsPluginsPaths(std::string(SIMSPATHEnvVar));
  }

  // user dir
  m_DefaultSimulatorsPlugsDirs.push_back(m_UserDataDir + "/" + openfluid::config::SIMULATORS_WARESBIN_USR_PATH);

  // market-bag dir (for current version)
  m_DefaultSimulatorsPlugsDirs.push_back(m_MarketBagSimVersionDir + "/" + m_MarketBagBinSubDir);

  // install directory
  std::string SimulatorsPluginsInstallPath =  m_InstallPrefix + "/" + openfluid::config::SIMULATORS_WARESBIN_STD_PATH;
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

  if (OBSSPATHEnvVar != nullptr)
  {
    addExtraObserversPluginsPaths(std::string(OBSSPATHEnvVar));
  }

  // user dir
  m_DefaultObserversPlugsDirs.push_back(m_UserDataDir + "/" + openfluid::config::OBSERVERS_WARESBIN_USR_PATH);

  // market-bag dir (for current version)
   m_DefaultObserversPlugsDirs.push_back(m_MarketBagObsVersionDir + "/" + m_MarketBagBinSubDir);

  // install directory
  std::string ObserversPluginsInstallPath = m_InstallPrefix + "/" + openfluid::config::OBSERVERS_WARESBIN_STD_PATH;
  m_DefaultObserversPlugsDirs.push_back(ObserversPluginsInstallPath);

}


// =====================================================================
// =====================================================================


RuntimeEnvironment::~RuntimeEnvironment()
{

}


// =====================================================================
// =====================================================================


void RuntimeEnvironment::prepareUserDataDirectory() const
{
  openfluid::tools::Filesystem::makeDirectory(m_UserDataDir);

  openfluid::tools::Filesystem::makeDirectory(getUserDataPath(openfluid::config::SIMULATORS_WARESBIN_USR_PATH));
  openfluid::tools::Filesystem::makeDirectory(getUserDataPath(openfluid::config::OBSERVERS_WARESBIN_USR_PATH));

  openfluid::tools::Filesystem::makeDirectory(getUserDataPath(openfluid::config::WORKSPACE_PATH));
  openfluid::tools::Filesystem::makeDirectory(getUserDataPath(openfluid::config::WORKSPACE_PATH+"/"+
                                                              openfluid::config::PROJECTS_PATH));
  openfluid::tools::Filesystem::makeDirectory(getUserDataPath(openfluid::config::WORKSPACE_PATH+"/"+
                                                              openfluid::config::WARESDEV_PATH));
  openfluid::tools::Filesystem::makeDirectory(getUserDataPath(openfluid::config::WORKSPACE_PATH+"/"+
                                                              openfluid::config::WARESDEV_PATH+"/"+
                                                              openfluid::config::SIMULATORS_PATH));
  openfluid::tools::Filesystem::makeDirectory(getUserDataPath(openfluid::config::WORKSPACE_PATH+"/"+
                                                              openfluid::config::WARESDEV_PATH+"/"+
                                                              openfluid::config::OBSERVERS_PATH));
}


// =====================================================================
// =====================================================================


void RuntimeEnvironment::setDateTimeOutputDir()
{
  m_OutputDir = m_UserDataDir + "/" + "OPENFLUID." +
                QDateTime::currentDateTime().toString("yyyyMMdd'T'hhmmss").toStdString() + ".OUT";
}


// =====================================================================
// =====================================================================


void RuntimeEnvironment::addExtraSimulatorsPluginsPaths(
    std::string SemicolonSeparatedPaths)
{
  std::vector<std::string> ExtraPaths;

#if defined(OPENFLUID_OS_UNIX)
  ExtraPaths = openfluid::tools::splitString(SemicolonSeparatedPaths, ":");
#elif defined(OPENFLUID_OS_WINDOWS)
  ExtraPaths = openfluid::tools::splitString(SemicolonSeparatedPaths,";");
#endif

  for (int i = ExtraPaths.size() - 1; i >= 0; i--)
    m_ExtraSimulatorsPlugsDirs.insert(m_ExtraSimulatorsPlugsDirs.begin(), 1,
        openfluid::tools::removeTrailingSlashes(ExtraPaths[i]));
}


// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getSimulatorPluginFullPath(const std::string& Filename) const
{

  std::vector<std::string> PluginsPaths = getSimulatorsPluginsPaths();
  std::string PlugFullPath = "";

  unsigned int i = 0;

  while ((PlugFullPath.length() == 0) && (i < PluginsPaths.size()))
  {
    std::string TmpPath = PluginsPaths[i] + "/" + Filename;

    if (openfluid::tools::Filesystem::isFile(TmpPath))
      PlugFullPath = TmpPath;

    i++;
  }

  return PlugFullPath;
}


// =====================================================================
// =====================================================================


void RuntimeEnvironment::addExtraObserversPluginsPaths(const std::string& SemicolonSeparatedPaths)
{
  std::vector<std::string> ExtraPaths;

#if defined(OPENFLUID_OS_UNIX)
  ExtraPaths = openfluid::tools::splitString(SemicolonSeparatedPaths, ":");
#elif defined(OPENFLUID_OS_WINDOWS)
  ExtraPaths = openfluid::tools::splitString(SemicolonSeparatedPaths,";");
#endif

  for (int i = ExtraPaths.size() - 1; i >= 0; i--)
    m_ExtraObserversPlugsDirs.insert(m_ExtraObserversPlugsDirs.begin(), 1,
        openfluid::tools::removeTrailingSlashes(ExtraPaths[i]));
}


// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getObserverPluginFullPath(const std::string& Filename) const
{

  std::vector<std::string> PluginsPaths = getObserversPluginsPaths();
  std::string PlugFullPath = "";

  unsigned int i = 0;

  while ((PlugFullPath.length() == 0) && (i < PluginsPaths.size()))
  {

    std::string TmpPath = PluginsPaths[i] + "/" + Filename;

    if (openfluid::tools::Filesystem::isFile(TmpPath))
      PlugFullPath = TmpPath;

    i++;
  }

  return PlugFullPath;
}


// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getCommonResourcesDir() const
{
  return m_InstallPrefix + "/" + openfluid::config::SHARE_COMMON_INSTALL_PATH;
}


// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getCommonResourceFilePath(
    std::string RelativeFilePath) const
{
  return m_InstallPrefix + "/" + openfluid::config::SHARE_COMMON_INSTALL_PATH + "/" + RelativeFilePath;
}


// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getAppResourcesDir(std::string AppName) const
{
  return m_InstallPrefix + "/" + openfluid::config::SHARE_APPS_INSTALL_PATH + "/" + AppName;
}

// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getAppResourceFilePath(std::string AppName,
    std::string RelativeFilePath) const
{
  return m_InstallPrefix + "/" + openfluid::config::SHARE_APPS_INSTALL_PATH + "/" + AppName + "/" + RelativeFilePath;
}


// =====================================================================
// =====================================================================


std::string RuntimeEnvironment::getTranslationsDir() const
{
  return m_InstallPrefix + "/" + openfluid::config::SHARE_TRANSLATIONS_INSTALL_PATH;
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


// =====================================================================
// =====================================================================


void RuntimeEnvironment::processWareParams(openfluid::ware::WareParams_t& Params) const
{
  std::map<std::string,std::string> Replacements =
    {{"${dir.input}",m_InputDir},
     {"${dir.output}",m_OutputDir},
     {"${dir.temp}",m_TempDir}};

  for (auto& Param : Params)
    for (const auto& R : Replacements)
      Param.second.replaceAll(R.first,R.second);
}


} } //namespaces

