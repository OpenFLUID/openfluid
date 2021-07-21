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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
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

    static std::string m_SettingsFile;

    static std::vector<std::string> m_DefaultSimulatorsDirs;

    static std::vector<std::string> m_ExtraSimulatorsDirs;

    static std::vector<std::string> m_DefaultObserversDirs;

    static std::vector<std::string> m_ExtraObserversDirs;

    static std::vector<std::string> m_DefaultBuilderextsDirs;

    static std::vector<std::string> m_ExtraBuilderextsDirs;

    static std::string m_ProvidedExamplesDir;

    static std::string m_UserExamplesDir;
    
    static std::string m_UserExampleSimulatorsDir;

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
    {

    }

    static std::string getWareFullPath(const std::vector<std::string>& Dirs, const std::string& Filename);


  public:

    /**
      Initializes OpenFLUID environment
    */
    static void init();

    /**
      Returns the current host name
      @return the host name
    */
    static std::string getHostName()
    {
      return m_HostName;
    }

    /**
      Returns the current system architecture. Possible values are defined in the global.hpp file
      with the OPENFLUID_OS_STRLABEL definition
      @return the system architecture
    */
    static std::string getSystemArch()
    {
      return m_SystemArch;
    }

    /**
      Returns the ideal thread count for the current running system
      @return the number of threads
    */
    static int getIdealThreadCount()
    {
      return m_IdealThreadCount;
    }

    /**
      Returns the ideal jobs count for the current running system
      @return the number of jobs
    */
    static int getIdealJobsCount()
    {
      return int(m_IdealThreadCount/2);
    }

    /**
      Returns the current user name
      @return the user name
    */
    static std::string getUserName()
    {
      return m_UserName;
    }

    /**
      Returns the current OpenFLUID version using the MAJOR.MINOR.PATCH format
      @return the version number
    */
    static std::string getVersion()
    {
      return m_Version;
    }

    /**
      Returns the current OpenFLUID version using the MAJOR.MINOR.PATCH~STATUS format
      @return the fully qualified version number
    */
    static std::string getVersionFull()
    {
      return m_VersionFull;
    }

    /**
      Returns the current OpenFLUID version using the MAJOR.MINOR format
      @return the simplified version number
    */
    static std::string getVersionMajorMinor()
    {
      return m_VersionMajorMinor;
    }

    /**
      Returns the current OpenFLUID temporary directory. It is automatically defined but can be forced using
      the OPENFLUID_TEMP_PATH environment variable.
      @return the full path to the directory
    */
    static std::string getTempDir()
    {
      return m_TempDir;
    }

    /**
      Returns the OpenFLUID install prefix directory. It is defined at build time but can be forced using
      the OPENFLUID_INSTALL_PREFIX environment variable.
      @return the full path to the directory
    */
    static std::string getInstallPrefix()
    {
      return m_InstallPrefix;
    }

    /**
      Returns the current user home directory
      @return the full path to the directory
    */
    static std::string getUserHomeDir()
    {
      return m_UserHomeDir;
    }

    /**
      Returns the OpenFLUID data directory for the current user
      @return the full path to the directory
    */
    static std::string getUserDataDir()
    {
      return m_UserDataDir;
    }

    /**
      Returns the full path of a file or directory relative to the OpenFLUID data directory for the current user
      @param[in] Path the path relative to the OpenFLUID user data directory
      @return the full path to the file or directory
    */
    static std::string getUserDataFullPath(const std::string& Path);

    /**
      Returns the full path of the examples directory for the current user
      @return the full path to the directory
    */
    static std::string getUserExamplesDir()
    {
      return m_UserExamplesDir;
    }
    
    /**
      Returns the full path of the example simulators directory
      @return the full path to the directory
    */
    static std::string getUserExampleSimulatorsDir()
    {
      return m_UserExampleSimulatorsDir;
    }

    /**
      Returns the full path of the directory where examples provided by OpenFLUID or models installations are stored
      @return the full path to the directory
    */
    static std::string getProvidedExamplesDir()
    {
      return m_ProvidedExamplesDir;
    }

    /**
      Automatically prepares the user data directory
    */
    static void prepareUserDataDirectory();

    /**
      Returns the configuration file path for the current user
      @return the full path to the file
    */
    static std::string getSettingsFile()
    {
      return m_SettingsFile;
    }

    /**
      Returns the OpenFLUID translations directory
      @return the full path to the directory
    */
    static std::string getTranslationsDir()
    {
      return m_TranslationsDir;
    }

    /**
      Returns the OpenFLUID common resources directory
      @return the full path to the directory
    */
    static std::string getCommonResourcesDir()
    {
      return m_CommonResourcesDir;
    }

    /**
      Returns the full path of a file or directory relative to the OpenFLUID common resources directory
      @param[in] Path the path relative to the OpenFLUID common resources directory
      @return the full path to the file or directory
    */
    static std::string getCommonResourcesFullPath(const std::string& Path);

    /**
      Returns the resources directory for a given OpenFLUID software application
      @param[in] AppName the name of the OpenFLUID application
      @return the full path to the directory
    */
    static std::string getAppResourcesDir(const std::string& AppName);

    /**
      Returns the full path of a file or directory relative to the resources directory
      of a given OpenFLUID software application
      @param[in] AppName the name of the OpenFLUID application
      @param[in] Path the path relative to the OpenFLUID application resources directory
      @return the full path to the file or directory
    */
    static std::string getAppResourcesFullPath(const std::string& AppName, const std::string& Path);

    /**
      Returns the list of directories full paths where OpenFLUID searches for simulators.
      Extra directories added at run time are at the beginning of the list, followed by standard search paths.
      @return the list of directories full paths
    */
    static std::vector<std::string> getSimulatorsDirs();

    /**
      Returns the list of default directories full paths where OpenFLUID searches for simulators.
      @return the list of directories full paths
    */
    static std::vector<std::string> getDefaultSimulatorsDirs()
    {
      return m_DefaultSimulatorsDirs;
    }

    /**
      Returns the list of extra added directories full paths where OpenFLUID searches for simulators.
      @return the list of directories full paths
    */
    static std::vector<std::string> getExtraSimulatorsDirs()
    {
      return m_ExtraSimulatorsDirs;
    }

    /**
      Adds paths to the list of directories where OpenFLUID searches for simulators.
      The paths are given as a single string of paths separated by ":" (on Unices systems) or ";" (on Windows systems)
      @param[in] Paths the list of full paths as a single string
    */
    static void addExtraSimulatorsDirs(const std::string& Paths);

    /**
      Resets the list of extra directories where OpenFLUID searches for simulators to empty
    */
    static void resetExtraSimulatorsDirs();

    /**
      Returns the full path of a searched simulator file
      @return the full path of the searched file, empty if not found
    */
    static std::string getSimulatorFullPath(const std::string& Filename);

    /**
      Returns the list of directories full paths where OpenFLUID searches for observers.
      Extra directories added at run time are at the beginning of the list, followed by standard search paths.
      @return the list of directories full paths
    */
    static std::vector<std::string> getObserversDirs();

    /**
      Returns the list of default directories full paths where OpenFLUID searches for observers.
      @return the list of directories full paths
    */
    static std::vector<std::string> getDefaultObserversDirs()
    {
      return m_DefaultObserversDirs;
    }

    /**
      Returns the list of extra added directories full paths where OpenFLUID searches for observers.
      @return the list of directories full paths
    */
    static std::vector<std::string> getExtraObserversDirs()
    {
      return m_ExtraObserversDirs;
    }

    /**
      Adds paths to the list of directories where OpenFLUID searches for observers.
      The paths are given as a single string of paths separated by ":" (on Unices systems) or ";" (on Windows systems)
      @param[in] Paths the list of full paths as a single string
    */
    static void addExtraObserversDirs(const std::string& Paths);

    /**
      Resets the list of extra directories where OpenFLUID searches for observers to empty
    */
    static void resetExtraObserversDirs();

    /**
      Returns the full path of a searched observer file
      @return the full path of the searched file, empty if not found
    */
    static std::string getObserverFullPath(const std::string& Filename);

    /**
      Returns the list of directories full paths where OpenFLUID searches for builder-extensions.
      Extra directories added at run time are at the beginning of the list, followed by standard search paths.
      @return the list of directories full paths
    */
    static std::vector<std::string> getBuilderextsDirs();

    /**
      Returns the list of default directories full paths where OpenFLUID searches for builder-extensions.
      @return the list of directories full paths
    */
    static std::vector<std::string> getDefaultBuilderextsDirs()
    {
      return m_DefaultBuilderextsDirs;
    }

    /**
      Returns the list of extra added directories full paths where OpenFLUID searches for builder-extensions.
      @return the list of directories full paths
    */
    static std::vector<std::string> getExtraBuilderextsDirs()
    {
      return m_ExtraBuilderextsDirs;
    }

    /**
      Adds paths to the list of directories where OpenFLUID searches for builder-extensions.
      The paths are given as a single string of paths separated by ":" (on Unices systems) or ";" (on Windows systems)
      @param[in] Paths the list of full paths as a single string
    */
    static void addExtraBuilderextsDirs(const std::string& Paths);

    /**
      Resets the list of extra directories where OpenFLUID searches for builder-extensions to empty
    */
    static void resetExtraBuilderextsDirs();

    /**
      Returns the full path of a searched builder-extension file
      @return the full path of the searched file, empty if not found
    */
    static std::string getBuilderextFullPath(const std::string& Filename);

    static std::string getMarketBagBinSubDir()
    {
      return m_MarketBagBinSubDir;
    }

    static std::string getMarketBagBuilderextsDir()
    {
      return m_MarketBagBuilderextsDir;
    }

    static std::string getMarketBagDataDir()
    {
      return m_MarketBagDataDir;
    }

    static std::string getMarketBagDir()
    {
      return m_MarketBagDir;
    }

    static std::string getMarketBagObserversDir()
    {
      return m_MarketBagObserversDir;
    }

    static std::string getMarketBagSimulatorsDir()
    {
      return m_MarketBagSimulatorsDir;
    }

    static std::string getMarketBagSrcSubDir()
    {
      return m_MarketBagSrcSubDir;
    }

    static std::string getMarketBagVersionDir()
    {
      return m_MarketBagVersionDir;
    }
};


} }  // namespaces


#endif /* __OPENFLUID_BASE_ENVIRONMENT_HPP__ */
