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
  @file Environment.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_BASE_ENVIRONMENT_HPP__
#define __OPENFLUID_BASE_ENVIRONMENT_HPP__


#include <string>
#include <vector>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace base {


class OPENFLUID_API Environment
{
  private:

    Environment(Environment const&) = delete;

    void operator=(Environment const&) = delete;

    static bool m_Initialized;


  protected:

    static std::string m_SystemArch;

    static std::string m_HostName;

    static std::string m_UserName;

    static std::string m_Version;

    static std::string m_VersionFull;

    static std::string m_VersionMajorMinor;

    static std::string m_InstallPrefix;

    static std::string m_TempDir;

    static std::string m_UserHomeDir;

    static std::string m_UserDataDir;

    static std::string m_ConfigFile;

    static std::vector<std::string> m_DefaultSimulatorsDirs;

    static std::vector<std::string> m_ExtraSimulatorsDirs;

    static std::vector<std::string> m_DefaultObserversDirs;

    static std::vector<std::string> m_ExtraObserversDirs;

    static std::vector<std::string> m_DefaultBuilderextsDirs;

    static std::vector<std::string> m_ExtraBuilderextsDirs;

    static std::string m_ProvidedExamplesDir;

    static std::string m_UserExamplesDir;

    static std::string m_MarketBagDir;

    static std::string m_MarketBagVersionDir;

    static std::string m_MarketBagSimulatorsDir;

    static std::string m_MarketBagObserversDir;

    static std::string m_MarketBagBuilderextsDir;

    static std::string m_MarketBagDataDir;

    static std::string m_MarketBagBinSubDir;

    static std::string m_MarketBagSrcSubDir;

    static std::string m_TranslationsDir;

    static std::string m_CommonResourcesDir;

    static std::string m_AppsResourcesDir;

    static int m_IdealThreadCount;

    Environment()
    { }

    static std::string getWareFullPath(const std::vector<std::string>& Dirs, const std::string& Filename);


  public:

    static void init();

    static std::string getHostName()
    { return m_HostName; }

    static std::string getSystemArch()
    { return m_SystemArch; }

    static int getIdealThreadCount()
    { return m_IdealThreadCount; }

    static std::string getUserName()
    { return m_UserName; }

    static std::string getVersion()
    { return m_Version; }

    static std::string getVersionFull()
    { return m_VersionFull; }

    static std::string getVersionMajorMinor()
    { return m_VersionMajorMinor; }

    static std::string getTempDir()
    { return m_TempDir; }

    static std::string getInstallPrefix()
    { return m_InstallPrefix; }

    static std::string getUserHomeDir()
    { return m_UserHomeDir; }

    static std::string getConfigFile()
    { return m_ConfigFile; }

    static std::string getUserDataDir()
    { return m_UserDataDir; }

    static std::string getUserDataFullPath(const std::string& Path)
    { return m_UserDataDir + "/" + Path; }

    static std::string getUserExamplesDir()
    { return m_UserExamplesDir; }

    static void prepareUserDataDirectory();

    static std::string getTranslationsDir()
    { return m_TranslationsDir; }

    static std::string getCommonResourcesDir()
    { return m_CommonResourcesDir; }

    static std::string getCommonResourcesFullPath(const std::string& Path)
    { return m_CommonResourcesDir + "/" + Path; }

    static std::string getAppResourcesDir(const std::string& AppName)
    { return m_AppsResourcesDir + "/" + AppName; }

    static std::string getAppResourcesFullPath(const std::string& AppName, const std::string& Path)
    { return getAppResourcesDir(AppName) + "/" + Path; }

    static std::vector<std::string> getSimulatorsDirs();

    static std::vector<std::string> getDefaultSimulatorsDirs()
    { return m_DefaultSimulatorsDirs; }

    static std::vector<std::string> getExtraSimulatorsDirs()
    { return m_ExtraSimulatorsDirs; }

    static void addExtraSimulatorsDirs(const std::string& Paths);

    static void resetExtraSimulatorsDirs();

    static std::string getSimulatorFullPath(const std::string& Filename);

    static std::vector<std::string> getObserversDirs();

    static std::vector<std::string> getDefaultObserversDirs()
    { return m_DefaultObserversDirs; }

    static std::vector<std::string> getExtraObserversDirs()
    { return m_ExtraObserversDirs; }

    static void addExtraObserversDirs(const std::string& Paths);

    static void resetExtraObserversDirs();

    static std::string getObserverFullPath(const std::string& Filename);

    static std::vector<std::string> getBuilderextsDirs();

    static std::vector<std::string> getDefaultBuilderextsDirs()
    { return m_DefaultBuilderextsDirs; }

    static std::vector<std::string> getExtraBuilderextsDirs()
    { return m_ExtraBuilderextsDirs; }

    static void addExtraBuilderextsDirs(const std::string& Paths);

    static void resetExtraBuilderextsDirs();

    static std::string getBuilderextFullPath(const std::string& Filename);

    static std::string getMarketBagBinSubDir()
    { return m_MarketBagBinSubDir; }

    static std::string getMarketBagBuilderextsDir()
    { return m_MarketBagBuilderextsDir; }

    static std::string getMarketBagDataDir()
    { return m_MarketBagDataDir; }

    static std::string getMarketBagDir()
    { return m_MarketBagDir; }

    static std::string getMarketBagObserversDir()
    { return m_MarketBagObserversDir; }

    static std::string getMarketBagSimulatorsDir()
    { return m_MarketBagSimulatorsDir; }

    static std::string getMarketBagSrcSubDir()
    { return m_MarketBagSrcSubDir; }

    static std::string getMarketBagVersionDir()
    { return m_MarketBagVersionDir; }

    static std::string getProvidedExamplesDir()
    { return m_ProvidedExamplesDir; }
};


} }  // namespaces


#endif /* __OPENFLUID_BASE_ENVIRONMENT_HPP__ */
