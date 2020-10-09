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
  @file MarketPackage.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_MARKET_MARKETPACKAGE_HPP__
#define __OPENFLUID_MARKET_MARKETPACKAGE_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/market/MarketInfos.hpp>


namespace openfluid { namespace market {


class OPENFLUID_API MarketPackage
{
  protected:

    static const std::string BUILDS_SUBDIR;

    static const std::string DLOADS_SUBDIR;

    static const std::string LOG_FILENAME;


    /*
     * Temporary dir for openfluid-market
    */
    static std::string m_TempDir;

    /**
     * Temporary dir for building packages
    */
    static std::string m_TempBuildsDir;

    /**
     * Temporary dir for downloaded packages
    */
    static std::string m_TempDownloadsDir;

    /**
     * Market-bag dir for simulators
    */
    static std::string m_MarketBagSimulatorDir;

    /**
     * Market-bag dir for observers
    */
    static std::string m_MarketBagObserverDir;

    /**
     * Market-bag dir for builder extensions
    */
    static std::string m_MarketBagBuilderextDir;

    /**
     * Market-bag dir for datasets
    */
    static std::string m_MarketBagDatasetDir;

    /**
     * Market-bag subdir for binary packages of current type
    */
    static std::string m_MarketBagBinSubDir;

    /**
     * Market-bag subdir for source packages of current type
    */
    static std::string m_MarketBagSrcSubDir;

    /**
     * Common build options for simulators
    */
    static std::string m_SimulatorBuildConfigOptions;

    /**
     * Common build options for observers
    */
    static std::string m_ObserverBuildConfigOptions;

    /**
     * Common build options for builder extensions
    */
    static std::string m_BuilderextBuildConfigOptions;

    static std::string m_LogFile;

    static bool m_IsLogEnabled;

    static bool m_Initialized;


    openfluid::ware::WareID_t m_ID;

    std::string m_PackageURL;

    std::string m_PackageFilename;

    std::string m_PackageDest;

    bool m_Downloaded;

    static void resetLogFile();

    static void appendToLogFile(const std::string& PackageName,
        const PackageInfo::PackageType& Type,
        const std::string& Action,
        const std::string& Str);

    static void appendToLogFile(const std::string& Str);


  public:

    MarketPackage(const openfluid::ware::WareID_t& ID, const std::string& PackageURL);

    virtual ~MarketPackage();

    static void initialize(bool EnableLog);

    /**
     Set directory paths attributes with paths passed as parameter
    */
    static void setWorksDirs(const std::string& TempDir, const std::string& MarketBagSimulatorDir,
                             const std::string& MarketBagObserverDir, const std::string& MarketBagBuilderextDir,
                             const std::string& MarketBagDatasetDir, const std::string& MarketBagBinSubDir,
                             const std::string& MarketBagSrcSubDir);

    static std::string getMarketBagSimulatorDir() 
    { 
      return m_MarketBagSimulatorDir; 
    }

    static std::string getMarketBagObserverDir() 
    { 
      return m_MarketBagObserverDir;
    }

    static std::string getMarketBagBuilderextDir() 
    { 
      return m_MarketBagBuilderextDir;
    }

    static std::string getMarketBagDatasetDir() 
    { 
      return m_MarketBagDatasetDir;
    }

    static std::string getMarketBagBinSubDir()
    { 
      return m_MarketBagBinSubDir;
    }

    static std::string getMarketBagSrcSubDir() 
    { 
      return m_MarketBagSrcSubDir;
    }

    static std::string getTempDir() 
    { 
      return m_TempDir;
    }

    static std::string getTempBuildsDir()
    { 
      return m_TempBuildsDir;
    }

    static std::string getTempDownloadsDir()
    { 
      return m_TempDownloadsDir;
    }

    static std::string getLogFile()
    { 
      return m_LogFile;
    }

    /**
     @return build options of package type passed as parameter
     @param Type of options
    */
    static std::string getCommonBuildOptions(const PackageInfo::PackageType& Type);

    /**
     Sets build options of package type passed as parameter with BuildOptions
     @param Type of options
     @param BuildOptions options to store
    */
    static void setCommonBuildOptions(const PackageInfo::PackageType& Type, const std::string& BuildOptions);

    /**
     Adds BuildOptions to options of package type
     @return Common build options of package type with BuildOptions
     @param Type of package
     @param BuildOptions to add
    */
    static std::string composeFullBuildOptions(const PackageInfo::PackageType& Type, const std::string& BuildOptions);

    openfluid::ware::WareID_t getID() const { return m_ID; };

    virtual MetaPackageInfo::SelectionType getFormat() const = 0;


    /**
     @return the market-bag path directory for the package type class
    */
    virtual std::string getInstallPath() const = 0;

    /**
     @return type of current package
    */
    virtual PackageInfo::PackageType getPackageType() const = 0;

    virtual void process() = 0;

    void download();


};


} } // namespaces


#endif /* __OPENFLUID_MARKET_MARKETPACKAGE_HPP__ */
