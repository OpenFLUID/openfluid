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

  @author JC.Fabre <fabrejc@supagro.inra.fr>
*/




#ifndef __MARKETCLIENT_HPP__
#define __MARKETCLIENT_HPP__

#include <string>
#include <queue>

#include <openfluid/dllexport.hpp>
#include <openfluid/tools/CURLDownloader.hpp>
#include <openfluid/market/MarketInfos.hpp>
#include <openfluid/market/MarketSrcPackage.hpp>
#include <openfluid/market/MarketBinPackage.hpp>

namespace openfluid { namespace market {


typedef std::map<std::string,std::string> MarketLicensesTexts_t;
// List of catalog urls
typedef std::map<PackageInfo::PackageType,std::string> CatalogsFileURL_t;
// List of catalog data
typedef std::map<PackageInfo::PackageType,std::string> CatalogsData_t;
// List of MetaPackagesCatalogs
typedef std::map<PackageInfo::PackageType,MetaPackagesCatalog_t> TypesMetaPackagesCatalogs_t;

// =====================================================================
// =====================================================================


class DLLEXPORT MarketClient
{
  public:

    static const std::string LOCK_FILE;

  private:

    MarketInfo m_MarketInfo;
    TypesMetaPackagesCatalogs_t m_TypesMetaPackagesCatalogs;
    std::string m_TempDir;

    std::string m_URL;

    MarketLicensesTexts_t m_LicensesTexts;

    bool m_IsConnected;

    bool m_IsLogEnabled;

    std::list<MarketPackage*> m_PacksToInstall;

    void initMarketBag();

    void initMarketTemp();

    void lockMarketTemp();

    void unlockMarketTemp();

    void parseMarketSiteData(const std::string& SiteData);

    /**
     Store all type catalog data in MetaPackageCatalog
     @param CatalogType Type of catalog read
     @param CatalogData string with data read
    */
    void parseCatalogData(const PackageInfo::PackageType& CatalogType, const std::string& CatalogData);

    void downloadAssociatedLicenses();

    /**
     @return string of SelectionType passed as parameter
     @param SelectionType to convert
    */
    //std::string selectionTypeToString(const MetaPackageInfo::SelectionType& Selec) const;

  public:

    MarketClient();

    ~MarketClient();

    /**
     @return string name of type passed as parameter
     @param Type Type of package
     @param Maj First letter in maj
     @param Plural Return plural name
    */
    static std::string getTypeName(const PackageInfo::PackageType& Type, const bool Maj, const bool Plural);

    /**
     @return true if MetaPackagesCatalogs contain packages
    */
    bool catalogsContainPackages() const;

    void connect(const std::string& URL);

    void disconnect();

    void getMarketInfo(MarketInfo& Info);

    const MarketLicensesTexts_t& getLicensesTexts();

    const TypesMetaPackagesCatalogs_t& getTypesMetaPackagesCatalogs();

    /**
     Finds the ID package in m_TypesMetaPackagesCatalogs
     @return iterator on ID package
     @param ID Id of package to find
    */
    MetaPackagesCatalog_t::iterator findInTypesMetaPackagesCatalogs(const openfluid::ware::WareID_t& ID);

    /**
     Set selected format of ID package with Flag
     @return true Flag has been set as selected flag
     @param ID Id of package to modify
     @param Flag Flag to be set
    */
    bool setSelectionFlag(const openfluid::ware::WareID_t& ID, const MetaPackageInfo::SelectionType& Flag);

    /**
     Set build options of ID package for source format with BuildOpts
     @param ID Id of package to modify
     @param BuildOpts Build options to be set
    */
    void setSRCBuildOptions(const openfluid::ware::WareID_t& ID, const std::string& BuildOpts);

    /**
     @return selected format of ID package
     @param ID Id of package to be got
    */
    MetaPackageInfo::SelectionType getSelectionFlag(const openfluid::ware::WareID_t& ID) const;

    void installSelection(const bool IgnoreMissing = true);

    void preparePackagesInstallation();

    unsigned int getCountOfPackagesToInstall() const { return m_PacksToInstall.size(); };

    const MarketPackage* getNextPackageToBeInstalled() const { return m_PacksToInstall.front(); };

    bool hasSelectedPackagesToInstall();

    void installNextSelectedPackage();

    const std::list<MarketPackage*>& getSelectionToInstall() const { return m_PacksToInstall; };

    bool isConnected() const { return m_IsConnected; };

    void enableLog(bool Enabled) { m_IsLogEnabled = Enabled; };

    /**
     Display packages list of market-place in console
    */
    //void displayPackages() const;

};


} } // namespaces


#endif /* __MARKETCLIENT_HPP__ */

