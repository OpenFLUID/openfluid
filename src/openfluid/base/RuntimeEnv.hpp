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
  @file RuntimeEnv.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/

#ifndef __OPENFLUID_BASE_RUNTIMEENV_HPP__
#define __OPENFLUID_BASE_RUNTIMEENV_HPP__

#include <string>
#include <vector>

#include <openfluid/dllexport.hpp>
#include <openfluid/base/EnvProperties.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/ware/TypeDefs.hpp>



namespace openfluid { namespace base {


// =====================================================================
// =====================================================================


/**

*/
class OPENFLUID_API RuntimeEnvironment
{
  private:

    static RuntimeEnvironment* mp_Singleton;

    const std::string m_Version;
    const std::string m_FullVersion;
    const std::string m_MajorMinorVersion;

    std::string m_OutputDir;
    std::string m_InputDir;
    std::string m_UserDataDir;
    std::string m_TempDir;
    std::string m_HomeDir;
    std::string m_MarketBagDir;
    std::string m_MarketBagVersionDir;

    std::string m_MarketBagSimVersionDir;
    std::string m_MarketBagObsVersionDir;
    std::string m_MarketBagBuildVersionDir;
    std::string m_MarketBagDataVersionDir;
    std::string m_MarketBagBinSubDir;
    std::string m_MarketBagSrcSubDir;

    std::string m_InstallPrefix;

    std::vector<std::string> m_DefaultSimulatorsPlugsDirs;
    std::vector<std::string> m_ExtraSimulatorsPlugsDirs;

    std::vector<std::string> m_DefaultObserversPlugsDirs;
    std::vector<std::string> m_ExtraObserversPlugsDirs;


    std::string m_ProvidedExamplesDir;
    std::string m_UserExamplesDir;

    std::string m_UserID;
    std::string m_HostName;
    const std::string m_Arch;

    unsigned int m_SimulatorsMaxNumThreads;

    std::string m_DefaultConfigFilePath;

    bool m_ClearOutputDir;

    bool m_WriteResults;

    bool m_WriteSimReport;

    bool m_Profiling;

    unsigned int m_ValuesBufferSize;

    bool m_IsUserValuesBufferSize;

    openfluid::base::EnvironmentProperties* mp_WareEnv;

    openfluid::base::EnvironmentProperties m_ExtraProperties;

    bool m_IsLinkedToProject;

    /**
      Default constructor
    */
    RuntimeEnvironment();


  public:

    static RuntimeEnvironment* instance();

    /**
      Destructor
    */
    ~RuntimeEnvironment();

    /**
      Returns the version of OpenFLUID
      @return the version as a std::string
    */
    std::string getVersion() const
    { return m_Version; };

    /**
      Returns the full version of OpenFLUID, including status (alpha, beta, rc, ...)
      @return the full version as a std::string
    */
    std::string getFullVersion() const
    { return m_FullVersion; };

    /**
      Returns the version of OpenFLUID, composed of major and minor numbers
      @return the major.minor version as a std::string
    */
    std::string getMajorMinorVersion() const
    { return m_MajorMinorVersion; };



    /**
      Sets the input directory, overriding the default input dir
      @param[in] InputDir The input directory
    */
    void setInputDir(const std::string& InputDir)
    { m_InputDir = InputDir; mp_WareEnv->setValue("dir.input",m_InputDir); };

    /**
      Returns the input directory
      @return the input directory
    */
    inline std::string getInputDir() const
    { return m_InputDir; };

    /**
      Sets the output directory, overriding the default output dir
      @param[in] OutputDir The output directory
    */
    void setOutputDir(const std::string& OutputDir)
      { m_OutputDir = OutputDir; mp_WareEnv->setValue("dir.output",m_OutputDir); };

    /**
      Returns the output directory
      @return the output directory
    */
    inline std::string getOutputDir() const
    { return m_OutputDir; };

    /**
      Sets the output directory as a directory located in the user data directory,
      with a name based on the current date-time
    */
    void setDateTimeOutputDir();

    /**
      Returns the market bag directory (i.e. $HOME/.openfluid/market-bag)
      @return the market bag directory
    */
    inline std::string getMarketBagDir() const
    { return m_MarketBagDir; };

    /**
      @return the market bag directory for the current OpenFLUID version
      (i.e. $HOME/.openfluid/market-bag/2.0.0)
     */
    inline std::string getMarketBagVersionDir() const
    { return m_MarketBagVersionDir; };

    /**
      @return the market bag directory for simulators of the current version
      (i.e. $HOME/.openfluid/market-bag/2.0.0/simulators)
     */
    inline std::string getMarketBagSimVersionDir() const
    { return m_MarketBagSimVersionDir; };

    /**
      @return the market bag directory for observers of the current version
      (i.e. $HOME/.openfluid/market-bag/2.0.0/observers)
     */
    inline std::string getMarketBagObsVersionDir() const
    { return m_MarketBagObsVersionDir; };

    /**
      @return the market bag directory for builderexts of the current version
      (i.e. $HOME/.openfluid/market-bag/2.0.0/builderexts)
     */
    inline std::string getMarketBagBuildVersionDir() const
    { return m_MarketBagBuildVersionDir; };

    /**
      @return the market bag directory for datasets of the current version
      (i.e. $HOME/.openfluid/market-bag/2.0.0/datasets)
     */
    inline std::string getMarketBagDataVersionDir() const
    { return m_MarketBagDataVersionDir; };

    /**
      @return the binairies market bag subdirectory
     */
    inline std::string getMarketBagBinSubDir() const
    { return m_MarketBagBinSubDir; };

    /**
      @return the sources market bag subdirectory
     */
    inline std::string getMarketBagSrcSubDir() const
    { return m_MarketBagSrcSubDir; };

    /**
      Returns the default config file path (i.e. $HOME/.openfluid/openfluid.conf)
      @return the default config file path
    */
    inline std::string getDefaultConfigFile() const
      { return m_DefaultConfigFilePath; };

    /**
      Returns the path for a given config file (i.e. $HOME/.openfluid/Filename)
      @param[in] Filename the given config file name
      @return the path
    */
    std::string getConfigFilePath(std::string Filename) const
    { return m_UserDataDir + "/" + Filename; };


    /**
      Returns the absolute path of the home directory
      @return the absolute path
    */
    std::string getUserHomeDir() const
      { return m_HomeDir; };

    /**
      Returns the absolute path for a given relative path in the user OpenFLUID directory
      (i.e. $HOME/.openfluid/RelativePath)
      @param[in] RelativePath the given relative path
      @return the absolute path
    */
    std::string getUserDataPath(std::string RelativePath) const
    { return m_UserDataDir + "/" + RelativePath; };

    /**
      Checks the user data directory and create missing subdirectories and files
    */
    void prepareUserDataDirectory() const;

    /**
      Returns the path of the temporary directory
      @return the path of the temporary directory
    */
    inline std::string getTempDir() const
      { return m_TempDir; };

    /**
      Returns the path for a given input file (i.e. InputDir/Filename)
      @param[in] Filename The given input file name
      @return the path for a given input file
    */
    std::string getInputFullPath(std::string Filename) const
      { return m_InputDir + "/" + Filename; };

    /**
      Returns the path for a given output file (i.e. OutputDir/Filename)
      @param[in] Filename The given output file name
      @return the path for a given output file
    */
    inline std::string getOutputFullPath(std::string Filename) const
    { return m_OutputDir + "/" + Filename; };

    /**
      Returns the path for a given simulator plugin file, taking into account the simulator plugins path search order
      @param[in] Filename The given simulator plugin file name
      @return the first path found for a given simulator plugin file
    */
    std::string getSimulatorPluginFullPath(const std::string& Filename) const;

    /**
      Adds search paths for plugins, separated by semicolon characters (i.e. /path/to/plugs:another/path/to/plugs).
      These paths are added at the top of the search paths list.
      @param[in] SemicolonSeparatedPaths a collection of paths separated by semicolons, as a std::string
    */
    void addExtraSimulatorsPluginsPaths(std::string SemicolonSeparatedPaths);

    inline void resetExtraSimulatorsPluginsPaths()
    { m_ExtraSimulatorsPlugsDirs.clear(); };

    inline std::vector<std::string> getDefaultSimulatorsPluginsPaths() const
    { return m_DefaultSimulatorsPlugsDirs;  };

    inline std::vector<std::string> getExtraSimulatorsPluginsPaths() const
    { return m_ExtraSimulatorsPlugsDirs;  };

    /**
      Returns the ordered list of paths used to search for simulator plugins
      @return the ordered list of paths
    */
    inline std::vector<std::string> getSimulatorsPluginsPaths() const
    {
      std::vector<std::string> ComposedPaths(m_ExtraSimulatorsPlugsDirs);
      ComposedPaths.insert(ComposedPaths.end(),
                           m_DefaultSimulatorsPlugsDirs.begin(),
                           m_DefaultSimulatorsPlugsDirs.end());
      return ComposedPaths;
    };

    /**
      Returns the path for a given observer plugin file, taking into account the observer plugins path search order
      @param[in] Filename The given observer plugin file name
      @return the first path found for a given observer plugin file
    */
    std::string getObserverPluginFullPath(const std::string& Filename) const;

    /**
      Adds search paths for plugins, separated by semicolon characters (i.e. /path/to/plugs:another/path/to/plugs).
      These paths are added at the top of the search paths list.
      @param[in] SemicolonSeparatedPaths a collection of paths separated by semicolons, as a std::string
    */
    void addExtraObserversPluginsPaths(const std::string& SemicolonSeparatedPaths);

    inline void resetExtraObserversPluginsPaths()
    { m_ExtraObserversPlugsDirs.clear(); };

    inline std::vector<std::string> getDefaultObserversPluginsPaths() const
    { return m_DefaultObserversPlugsDirs;  };

    inline std::vector<std::string> getExtraObserversPluginsPaths() const
    { return m_ExtraObserversPlugsDirs;  };

    /**
      Returns the ordered list of paths used to search for observer plugins
      @return the ordered list of paths
    */
    inline std::vector<std::string> getObserversPluginsPaths() const
    {
      std::vector<std::string> ComposedPaths(m_ExtraObserversPlugsDirs);
      ComposedPaths.insert(ComposedPaths.end(),
                           m_DefaultObserversPlugsDirs.begin(),
                           m_DefaultObserversPlugsDirs.end());
      return ComposedPaths;
    };



    /**
      Returns the install prefix path.
      The install prefix is given at compile time, but can be overriden by the
      OPENFLUID_INSTALL_PREFIX environment variable (recommended on windows systems).
      @return the install prefix path
    */
    inline std::string getInstallPrefix() const
      { return m_InstallPrefix; };

    /**
      Returns the path for common resources, taking into account the install prefix path
      @return the path for common resources
    */
    std::string getCommonResourcesDir() const;

    /**
      Returns the path for a given file, relative to the common resources path
      @param[in] RelativeFilePath The given file
      @return the path for a given file in the common resources
    */
    std::string getCommonResourceFilePath(std::string RelativeFilePath) const;

    /**
      Returns the path for resources of a given application, taking into account the install prefix path
      @param[in] AppName The given application name
      @return the path for resources of a given application
    */
    std::string getAppResourcesDir(std::string AppName) const;

    /**
      Returns the path for a given file, relative a given application resources path
      @param[in] AppName The given application name
      @param[in] RelativeFilePath The given file
      @return the path for a given file in the given application resources
    */
    std::string getAppResourceFilePath(std::string AppName, std::string RelativeFilePath) const;

    /**
      Returns the translations directory
      @return the path for translationslocale directory
    */
    std::string getTranslationsDir() const;

    /**
      Returns the path for provided examples, taking into account the install prefix path
      @return the path for provided examples
     */
    std::string getProvidedExamplesDir() const
    { return m_ProvidedExamplesDir; };


    /**
      Returns the path for examples in user's directory
      @return the path for user's examples
     */
    std::string getUserExamplesDir() const
    { return m_UserExamplesDir; };


    /**
      Returns the extra properties list
      @return the extra properties list
    */
    inline openfluid::base::EnvironmentProperties& extraProperties()
      { return m_ExtraProperties; };

    /**
      Returns the clear output directory flag
    */
    inline bool isClearOutputDir() const
    { return m_ClearOutputDir; };

    /**
      Sets the clear output directory flag
      @param[in] ClearDir The value of the flag
    */
    inline void setClearOutputDir(bool ClearDir)
    { m_ClearOutputDir = ClearDir; mp_WareEnv->setValue("mode.clearoutputdir",m_ClearOutputDir); };


    openfluid::base::EnvironmentProperties* wareEnvironment() const
    { return mp_WareEnv; };


    /**
      Returns the local host name (if could be determined)
      @return the local host name
    */
    std::string getHostName() const {return m_HostName; };

    /**
      Returns the local user ID (if could be determined)
      @return the local user ID
    */
    std::string getUserID() const {return m_UserID; };

    /**
      Returns the local system architecture (if could be determined)
      @return the local architecture
    */
    std::string getArch() const {return m_Arch; };

    unsigned int getSimulatorsMaxNumThreads() const
    { return m_SimulatorsMaxNumThreads; };

    void setSimulatorsMaxNumThreads(const unsigned int& MaxNumThreads)
    { if (MaxNumThreads > 0) m_SimulatorsMaxNumThreads = MaxNumThreads; };

    // TODO Values buffer size should not be managed here
    void setValuesBufferSize(const unsigned int StepsNbr)
    { m_ValuesBufferSize = StepsNbr; m_IsUserValuesBufferSize = true; };

    inline unsigned int getValuesBufferSize() const
    { return m_ValuesBufferSize; };

    bool isUserValuesBufferSize() const
    { return m_IsUserValuesBufferSize; };

    inline void unsetUserValuesBufferSize()
      { m_IsUserValuesBufferSize = false; }

    void linkToProject();

    void detachFromProject();

    bool isLinkedToProject() const
      { return m_IsLinkedToProject; };

    bool isSimulationProfilingEnabled() const
    { return m_Profiling; };

    void setSimulationProfilingEnabled(bool Profiling)
    { m_Profiling = Profiling; };

    void processWareParams(openfluid::ware::WareParams_t& Params) const;
};


} } //namespaces

#endif



