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
 @file MarketClient.cpp

 @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 @author Manuel Chataigner <manuel.chataigner@supagro.inra.fr>
*/

#include <openfluid/market/MarketClient.hpp>
#include <openfluid/market/MarketBinSimulatorPackage.hpp>
#include <openfluid/market/MarketSrcSimulatorPackage.hpp>
#include <openfluid/market/MarketBinObserverPackage.hpp>
#include <openfluid/market/MarketSrcObserverPackage.hpp>
#include <openfluid/market/MarketSrcBuilderextPackage.hpp>
#include <openfluid/market/MarketBinBuilderextPackage.hpp>
#include <openfluid/market/MarketDatasetPackage.hpp>
#include <openfluid/config.hpp>

#include <QSettings>
#include <QStringList>

#include <fstream>
#include <deque>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/utils/FileDownloader.hpp>


namespace openfluid { namespace market {



const std::string MarketClient::LOCK_FILE = ".lock";


// =====================================================================
// =====================================================================


MarketClient::MarketClient() :
    m_IsConnected(false), m_IsLogEnabled(false)
{

  std::string m_TempDir = openfluid::base::RuntimeEnvironment::instance()->getTempDir()+"/market";
  MarketPackage::setWorksDirs(boost::filesystem::path(m_TempDir).string(),
                              openfluid::base::RuntimeEnvironment::instance()->getMarketBagSimVersionDir(),
                              openfluid::base::RuntimeEnvironment::instance()->getMarketBagObsVersionDir(),
                              openfluid::base::RuntimeEnvironment::instance()->getMarketBagBuildVersionDir(),
                              openfluid::base::RuntimeEnvironment::instance()->getMarketBagDataVersionDir(),
                              openfluid::base::RuntimeEnvironment::instance()->getMarketBagBinSubDir(),
                              openfluid::base::RuntimeEnvironment::instance()->getMarketBagSrcSubDir());

  boost::filesystem::create_directories(boost::filesystem::path(m_TempDir));

  m_URL.clear();
  m_MarketInfo.clear();
}


// =====================================================================
// =====================================================================


MarketClient::~MarketClient()
{
  unlockMarketTemp();
}


// =====================================================================
// =====================================================================


void MarketClient::initMarketBag()
{
  // ===== Creation of simulator directories =====

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagSimulatorDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getMarketBagSimulatorDir())))
    throw openfluid::base::FrameworkException("MarketClient::initMarketBag()",
                                              "Unable to initialize market-bag simulators directory");

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagSimulatorDir()
    + "/" + MarketPackage::getMarketBagBinSubDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getMarketBagSimulatorDir()
    + "/" + MarketPackage::getMarketBagBinSubDir())))
    throw openfluid::base::FrameworkException("MarketClient::initMarketBag()",
                                              "Unable to initialize market-bag simulators binary subdirectory");

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagSimulatorDir()
    + "/" + MarketPackage::getMarketBagSrcSubDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getMarketBagSimulatorDir()
    + "/" + MarketPackage::getMarketBagSrcSubDir())))
    throw openfluid::base::FrameworkException("MarketClient::initMarketBag()",
                                              "Unable to initialize market-bag simulators source subdirectory");


  // ===== Creation of observer directories =====

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagObserverDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getMarketBagObserverDir())))
    throw openfluid::base::FrameworkException("MarketClient::initMarketBag()",
                                              "Unable to initialize market-bag observers directory");

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagObserverDir()
    + "/" + MarketPackage::getMarketBagBinSubDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getMarketBagObserverDir()
    + "/" + MarketPackage::getMarketBagBinSubDir())))
    throw openfluid::base::FrameworkException("MarketClient::initMarketBag()",
                                              "Unable to initialize market-bag observers binary subdirectory");

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagObserverDir()
    + "/" + MarketPackage::getMarketBagSrcSubDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getMarketBagObserverDir()
    + "/" + MarketPackage::getMarketBagSrcSubDir())))
    throw openfluid::base::FrameworkException("MarketClient::initMarketBag()",
                                              "Unable to initialize market-bag observers source subdirectory");


  // ===== Creation of builder extension directories =====

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagBuilderextDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getMarketBagBuilderextDir())))
    throw openfluid::base::FrameworkException("MarketClient::initMarketBag()",
                                              "Unable to initialize market-bag builderexts directory");

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagBuilderextDir()
    + "/" + MarketPackage::getMarketBagBinSubDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getMarketBagBuilderextDir()
    + "/" + MarketPackage::getMarketBagBinSubDir())))
    throw openfluid::base::FrameworkException("MarketClient::initMarketBag()",
                                              "Unable to initialize market-bag builderexts binary subdirectory");

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagBuilderextDir()
    + "/" + MarketPackage::getMarketBagSrcSubDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getMarketBagBuilderextDir()
    + "/" + MarketPackage::getMarketBagSrcSubDir())))
    throw openfluid::base::FrameworkException("MarketClient::initMarketBag()",
                                              "Unable to initialize market-bag builderexts source subdirectory");


  // ===== Creation of dataset directory =====

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagDatasetDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getMarketBagDatasetDir())))
    throw openfluid::base::FrameworkException("MarketClient::initMarketBag()",
                                              "Unable to initialize market-bag datasets directory");
}


// =====================================================================
// =====================================================================


void MarketClient::initMarketTemp()
{

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getTempBuildsDir()));
  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getTempDownloadsDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getTempBuildsDir())) ||
      !boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getTempDownloadsDir())))
    throw openfluid::base::FrameworkException("MarketClient::initMarketTemp()",
                                              "Unable to initialize market temp directory");
}


// =====================================================================
// =====================================================================


void MarketClient::lockMarketTemp()
{
  std::string LockFilename = MarketPackage::getTempDir()+"/"+LOCK_FILE;

  if (boost::filesystem::exists(boost::filesystem::path(LockFilename)))
    throw openfluid::base::FrameworkException("MarketClient::lockMarketTemp()",
                                              "Unable to lock market temp directory (already locked)");

  std::ofstream(boost::filesystem::path(LockFilename).string().c_str()).close();

  if (!boost::filesystem::exists(boost::filesystem::path(LockFilename)))
    throw openfluid::base::FrameworkException("MarketClient::lockMarketTemp()",
                                              "Unable to lock market temp directory (cannot create lock)");
}


// =====================================================================
// =====================================================================


void MarketClient::unlockMarketTemp()
{
  std::string LockFilename = MarketPackage::getTempDir()+"/"+LOCK_FILE;

  boost::filesystem::remove(boost::filesystem::path(LockFilename));

  if (boost::filesystem::exists(boost::filesystem::path(LockFilename)))
    throw openfluid::base::FrameworkException("MarketClient::unlockMarketTemp()",
                                              "Unable to unlock market temp directory (cannot remove lock)");
}


// =====================================================================
// =====================================================================


void MarketClient::parseMarketSiteData(const std::string& SiteFile)
{

  QSettings MarketSiteFile(QString::fromStdString(SiteFile),QSettings::IniFormat);

  MarketSiteFile.beginGroup("openfluid-marketplace");
  m_MarketInfo.Name = openfluid::tools::fromIniCompatible(MarketSiteFile.value("name"));
  m_MarketInfo.Description = openfluid::tools::fromIniCompatible(MarketSiteFile.value("description"));
  m_MarketInfo.Contact = openfluid::tools::fromIniCompatible(MarketSiteFile.value("contact"));
  MarketSiteFile.endGroup();

}


// =====================================================================
// =====================================================================


std::string MarketClient::getTypeName(const PackageInfo::PackageType& Type, const bool Maj, const bool Plural)
{
  std::string TypesNames[] = { "simulator", "observer", "builderext", "dataset"};
  std::string Name = TypesNames[Type];

  if (Maj)
    Name[0] = toupper(Name[0]);

  if (Plural)
    Name += "s";

  return Name;
}


// =====================================================================
// =====================================================================


bool MarketClient::catalogsContainPackages() const
{
  openfluid::market::TypesMetaPackagesCatalogs_t::const_iterator TPCit = m_TypesMetaPackagesCatalogs.begin();

  while (TPCit != m_TypesMetaPackagesCatalogs.end() && TPCit->second.empty())
    ++TPCit;
  return (TPCit != m_TypesMetaPackagesCatalogs.end());
}


// =====================================================================
// =====================================================================


void MarketClient::parseCatalogData(const PackageInfo::PackageType& CatalogType, const std::string& CatalogFile)
{
  std::string TmpVersion, TmpArch, TmpID;
  QStringList PackagesIDs;

  // Catalog of type passed as parameter
  MetaPackagesCatalog_t& MetaPackagesCatalog = m_TypesMetaPackagesCatalogs[CatalogType];
  MetaPackagesCatalog.clear();

  std::string BinaryArchKey= "arch." + openfluid::base::RuntimeEnvironment::instance()->getArch();

  QSettings CatalogData(QString::fromStdString(CatalogFile),QSettings::IniFormat);

  try
  {
    PackagesIDs = CatalogData.childGroups();

    if (!PackagesIDs.isEmpty())
    {
      for (int i=0; i< PackagesIDs.size(); i++)
      {
        std::string TmpID = PackagesIDs[i].toStdString();

         CatalogData.beginGroup(PackagesIDs[i]);
         // searching for key for TmpID group
         if (CatalogData.contains(QString::fromStdString(BinaryArchKey+".file")) ||
             CatalogData.contains("arch.src.file") ||
             CatalogData.contains("file"))
         {

           MetaPackagesCatalog[TmpID].ID = TmpID;

           // name
           if (CatalogData.contains("name"))
             MetaPackagesCatalog[TmpID].Name = openfluid::tools::fromIniCompatible(CatalogData.value("name"));

           // description
           if (CatalogData.contains("description"))
             MetaPackagesCatalog[TmpID].Description =
                 openfluid::tools::fromIniCompatible(CatalogData.value("description"));

           // version
           if (CatalogData.contains("version"))
             MetaPackagesCatalog[TmpID].Version = openfluid::tools::fromIniCompatible(CatalogData.value("version"));

           // authors
           if (CatalogData.contains("authors"))
             MetaPackagesCatalog[TmpID].Authors = openfluid::tools::fromIniCompatible(CatalogData.value("authors"));


           // dataset ?
           if (CatalogType == PackageInfo::DATA)
           {
             if (!CatalogData.contains("file"))
             {
               MetaPackagesCatalog.erase(TmpID);
             }
             else
             {
               MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::FLUIDX] = PackageInfo();
               MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::FLUIDX].URL =
                   m_URL + "/"+
                   getTypeName(CatalogType,false,true)+"/"+
                   openfluid::tools::fromIniCompatible(CatalogData.value("file"));

               // license
               if (CatalogData.contains("license"))
               {
                 MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::FLUIDX].License =
                     openfluid::tools::fromIniCompatible(CatalogData.value("license"));
               }

               // dependencies
               if (CatalogData.contains("dependencies.sim"))
               {
                 MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::FLUIDX].Dependencies[PackageInfo::SIM] =
                     openfluid::tools::toStdStringList(CatalogData.value("dependencies.sim").toStringList());
               }

               if (CatalogData.contains("dependencies.obs"))
               {
                 MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::FLUIDX].Dependencies[PackageInfo::OBS] =
                     openfluid::tools::toStdStringList(CatalogData.value("dependencies.obs").toStringList());
               }
             }
           }
           else
           {
             if (!CatalogData.contains(QString::fromStdString(BinaryArchKey)+".file") &&
                 !CatalogData.contains("arch.src.file"))
             {
               MetaPackagesCatalog.erase(TmpID);
             }
             else
             {
               // binary ?
               if (CatalogData.contains(QString::fromStdString(BinaryArchKey)+".file"))
               {
                 MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::BIN] = PackageInfo();
                 MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::BIN].URL =
                     m_URL + "/"+
                     getTypeName(CatalogType,false,true)+"/"+
                     openfluid::base::RuntimeEnvironment::instance()->getArch()+"/"+
                     openfluid::tools::fromIniCompatible(CatalogData.value(QString::fromStdString(BinaryArchKey)+
                                                                           ".file"));

                 // license
                 if (CatalogData.contains(QString::fromStdString(BinaryArchKey)+".license"))
                 {
                   MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::BIN].License =
                       openfluid::tools::fromIniCompatible(CatalogData.value(QString::fromStdString(BinaryArchKey)+
                                                                             ".license"));
                 }
               }

               // source ?
               if (CatalogData.contains("arch.src.file"))
               {
                 MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC] = PackageInfo();
                 MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC].URL =
                     m_URL + "/"+
                     getTypeName(CatalogType,false,true)+
                     "/src/"+
                     openfluid::tools::fromIniCompatible(CatalogData.value("arch.src.file"));

                 // license
                 if (CatalogData.contains("arch.src.license"))
                   MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC].License =
                       openfluid::tools::fromIniCompatible(CatalogData.value("arch.src.license"));

                 // build options
                 if (CatalogData.contains("arch.src.buildoptions"))
                   MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC].BuildOptions =
                       openfluid::tools::fromIniCompatible(CatalogData.value("arch.src.buildoptions"));
               }
             }
           }
         }

        CatalogData.endGroup();
      }
    }
  }
  catch (...)
  {
    // TODO check this!
  };
}


// =====================================================================
// =====================================================================


void MarketClient::downloadAssociatedLicenses()
{
  TypesMetaPackagesCatalogs_t::iterator TPCit;
  MetaPackagesCatalog_t::iterator PCit;
  std::map<MetaPackageInfo::SelectionType,PackageInfo>::iterator PIit;
  MarketLicensesTexts_t::iterator Licit;

  for (TPCit = m_TypesMetaPackagesCatalogs.begin(); TPCit != m_TypesMetaPackagesCatalogs.end(); ++TPCit)
  {
    for (PCit = TPCit->second.begin(); PCit != TPCit->second.end(); ++PCit)
    {
      // download licenses of all formats for each package
      for (PIit = PCit->second.AvailablePackages.begin(); PIit != PCit->second.AvailablePackages.end(); ++PIit)
      {
        if (PIit->second.License != "") m_LicensesTexts[PIit->second.License] = "";
      }
    }
  }

  for (Licit = m_LicensesTexts.begin(); Licit != m_LicensesTexts.end(); ++Licit)
  {
    if (!openfluid::utils::FileDownloader::downloadToString(m_URL+"/licenses/"+Licit->first+".txt",Licit->second))
      Licit->second = "(license text not available)";
  }
}


// =====================================================================
// =====================================================================


void MarketClient::connect(const std::string& URL)
{

  // List of catalog urls
  CatalogsFileURL_t CatalogsFileURL;
  // List of catalog data
  CatalogsData_t CatalogsData;


  m_URL = URL;
  m_MarketInfo.clear();
  m_TypesMetaPackagesCatalogs.clear();

  initMarketBag();
  initMarketTemp();

  MarketPackage::initialize(m_IsLogEnabled);

  std::string MarketFileURL = m_URL+"/"+openfluid::config::MARKETPLACE_SITEFILE;

  // Creating type keys
  CatalogsFileURL[PackageInfo::SIM] = "";
  CatalogsFileURL[PackageInfo::OBS] = "";
  CatalogsFileURL[PackageInfo::BUILD] = "";
  CatalogsFileURL[PackageInfo::DATA] = "";

  // Downloading OpenFLUID-Marketplace file
  std::string MarketFileLocal = openfluid::base::RuntimeEnvironment::instance()->getTempDir()+"/marketplace.tmp";

  if (!m_IsConnected && openfluid::utils::FileDownloader::downloadToFile(MarketFileURL, MarketFileLocal))
  {
    lockMarketTemp();
    parseMarketSiteData(MarketFileLocal);
    m_IsConnected = true;
  }

  if (m_IsConnected)
  {
    CatalogsFileURL_t::iterator CUit;

    // for each catalog
    for (CUit = CatalogsFileURL.begin(); CUit != CatalogsFileURL.end(); ++CUit)
    {
      // building url
      CUit->second = m_URL+"/"+openfluid::config::MARKETPLACE_CATALOGFILE+getTypeName(CUit->first,true,true)+"_"+
                     openfluid::base::RuntimeEnvironment::instance()->getVersion();

      std::string CatalogFileLocal =
          openfluid::base::RuntimeEnvironment::instance()->getTempDir()+"/"+
          QString("catalog_%1.tmp").arg(CUit->first).toStdString();

      if (openfluid::utils::FileDownloader::downloadToFile(CUit->second, CatalogFileLocal))
        parseCatalogData(CUit->first, CatalogFileLocal);

    }

    downloadAssociatedLicenses();
  }
}


// =====================================================================
// =====================================================================


void MarketClient::disconnect()
{
  unlockMarketTemp();

  m_URL.clear();
  m_MarketInfo.clear();
  m_TypesMetaPackagesCatalogs.clear();
  m_LicensesTexts.clear();
  m_IsConnected = false;
}


// =====================================================================
// =====================================================================


void MarketClient::getMarketInfo(MarketInfo& Info)
{
  Info = m_MarketInfo;
}


// =====================================================================
// =====================================================================


const TypesMetaPackagesCatalogs_t& MarketClient::typesMetaPackagesCatalogs()
{
  return m_TypesMetaPackagesCatalogs;
}


// =====================================================================
// =====================================================================


const MarketLicensesTexts_t& MarketClient::licensesTexts()
{
  return m_LicensesTexts;
}


// =====================================================================
// =====================================================================


MetaPackagesCatalog_t::iterator MarketClient::findInTypesMetaPackagesCatalogs(const openfluid::ware::WareID_t& ID)
{
  TypesMetaPackagesCatalogs_t::iterator TPCit = m_TypesMetaPackagesCatalogs.begin();
  MetaPackagesCatalog_t::iterator PCit;

  // searching for ID package in each MetaPackages catalog
  while (TPCit != m_TypesMetaPackagesCatalogs.end() && (PCit = TPCit->second.find(ID)) == TPCit->second.end())
    TPCit++;

  return PCit;
}


// =====================================================================
// =====================================================================


bool MarketClient::setSelectionFlag(const openfluid::ware::WareID_t& ID, const MetaPackageInfo::SelectionType& Flag)
{
  MetaPackagesCatalog_t::iterator PCit = findInTypesMetaPackagesCatalogs(ID);

  // if package found
  if (PCit != m_TypesMetaPackagesCatalogs.rbegin()->second.end())
  {
    if (Flag == MetaPackageInfo::BIN
        && PCit->second.AvailablePackages.find(MetaPackageInfo::BIN) ==  PCit->second.AvailablePackages.end())
      return false;

    if (Flag == MetaPackageInfo::SRC
        && PCit->second.AvailablePackages.find(MetaPackageInfo::SRC) ==  PCit->second.AvailablePackages.end())
      return false;

    if (Flag == MetaPackageInfo::FLUIDX
        && PCit->second.AvailablePackages.find(MetaPackageInfo::FLUIDX) == PCit->second.AvailablePackages.end())
      return false;

    // Setting Flag as selected flag
    PCit->second.Selected = Flag;
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


void MarketClient::setSRCBuildOptions(const openfluid::ware::WareID_t& ID, const std::string& BuildOpts)
{
  MetaPackagesCatalog_t::iterator PCit = findInTypesMetaPackagesCatalogs(ID);

  // if package found
  if (PCit != m_TypesMetaPackagesCatalogs.rbegin()->second.end())
  {
    // if src format is available
    if (PCit->second.AvailablePackages.find(MetaPackageInfo::SRC) !=  PCit->second.AvailablePackages.end())
    {
      // Setting build options
      PCit->second.AvailablePackages[MetaPackageInfo::SRC].BuildOptions = BuildOpts;
    }
  }
}

// =====================================================================
// =====================================================================


MetaPackageInfo::SelectionType MarketClient::getSelectionFlag(const openfluid::ware::WareID_t& ID) const
{
  TypesMetaPackagesCatalogs_t::const_iterator TPCit = m_TypesMetaPackagesCatalogs.begin();
  MetaPackagesCatalog_t::const_iterator PCit;

  // searching for ID package in each MetaPackages catalog
  while (TPCit != m_TypesMetaPackagesCatalogs.end() && (PCit = TPCit->second.find(ID)) == TPCit->second.end())
    TPCit++;

  // if ID package found
  if (TPCit != m_TypesMetaPackagesCatalogs.end())
  {
    return PCit->second.Selected;
  }

  return MetaPackageInfo::NONE;
}


// =====================================================================
// =====================================================================


void MarketClient::preparePackagesInstallation()
{
  TypesMetaPackagesCatalogs_t::iterator TPCit;
  MetaPackagesCatalog_t::iterator PCit;

  // clearing list of packages to install
  while (!m_PacksToInstall.empty())
  {
    delete m_PacksToInstall.front();
    m_PacksToInstall.pop_front();
  }

  // creating list of packages to install
  for (TPCit = m_TypesMetaPackagesCatalogs.begin(); TPCit != m_TypesMetaPackagesCatalogs.end(); ++TPCit)
  {
    for (PCit = TPCit->second.begin(); PCit != TPCit->second.end(); ++PCit)
    {
      // Binary
      if (PCit->second.Selected == MetaPackageInfo::BIN)
      {
        if (TPCit->first == PackageInfo::SIM)
        {
          m_PacksToInstall.push_back(
              new MarketBinSimulatorPackage(PCit->second.ID,PCit->second.AvailablePackages[MetaPackageInfo::BIN].URL));
        }
        else if (TPCit->first == PackageInfo::OBS)
        {
          m_PacksToInstall.push_back(
              new MarketBinObserverPackage(PCit->second.ID,PCit->second.AvailablePackages[MetaPackageInfo::BIN].URL));
        }
        else if (TPCit->first == PackageInfo::BUILD)
        {
          m_PacksToInstall.push_back(
              new MarketBinBuilderextPackage(PCit->second.ID,PCit->second.AvailablePackages[MetaPackageInfo::BIN].URL));
        }
      }


      // Source
      if (PCit->second.Selected == MetaPackageInfo::SRC)
      {
        if (TPCit->first == PackageInfo::SIM)
        {
          m_PacksToInstall.push_back(
              new MarketSrcSimulatorPackage(PCit->second.ID,PCit->second.AvailablePackages[MetaPackageInfo::SRC].URL));
        }
        else if (TPCit->first == PackageInfo::OBS)
        {
          m_PacksToInstall.push_back(
              new MarketSrcObserverPackage(PCit->second.ID,PCit->second.AvailablePackages[MetaPackageInfo::SRC].URL));
        }
        else if (TPCit->first == PackageInfo::BUILD)
        {
          m_PacksToInstall.push_back(
              new MarketSrcBuilderextPackage(PCit->second.ID,PCit->second.AvailablePackages[MetaPackageInfo::SRC].URL));
        }

        std::string BuildOptsStr = PCit->second.AvailablePackages[MetaPackageInfo::SRC].BuildOptions;

        if (!BuildOptsStr.empty())
          ((MarketSrcPackage*)m_PacksToInstall.back())->setBuildConfigOptions(BuildOptsStr);
      }


      // Dataset
      if (PCit->second.Selected == MetaPackageInfo::FLUIDX)
      {
        m_PacksToInstall.push_back(
            new MarketDatasetPackage(PCit->second.ID,PCit->second.AvailablePackages[MetaPackageInfo::FLUIDX].URL));
      }
    }
  }
}


// =====================================================================
// =====================================================================


void MarketClient::installSelection(const bool /*IgnoreMissing = true*/)
{

  preparePackagesInstallation();


  while (!m_PacksToInstall.empty())
  {
    MarketPackage* MP = m_PacksToInstall.front();
    m_PacksToInstall.pop_front();

    MP->download();
    MP->process();

    delete MP;
  }
}


// =====================================================================
// =====================================================================


bool MarketClient::hasSelectedPackagesToInstall()
{
  return (!m_PacksToInstall.empty());
}


// =====================================================================
// =====================================================================


void MarketClient::installNextSelectedPackage()
{
  if (!m_PacksToInstall.empty())
  {
    MarketPackage* MP = m_PacksToInstall.front();
    m_PacksToInstall.pop_front();

    MP->download();
    MP->process();

    delete MP;
  }
}


// =====================================================================
// =====================================================================

/*
std::string MarketClient::selectionTypeToString(const MetaPackageInfo::SelectionType& Selec) const
{
  const bool WithArch = true;

  if (Selec == MetaPackageInfo::BIN)
  {
    if(WithArch)
      return openfluid::base::RuntimeEnvironment::instance()->getArch();
    else
      return "BIN";
  }
  else if (Selec == MetaPackageInfo::SRC)
  {
    return "SRC";
  }
  else if (Selec == MetaPackageInfo::FLUIDX)
  {
    return "FLUIDX";
  }

  return "";
}


// =====================================================================
// =====================================================================


void MarketClient::displayPackages() const
{
  TypesMetaPackagesCatalogs_t::const_iterator TPCit;
  MetaPackagesCatalog_t::const_iterator PCit;
  std::map<MetaPackageInfo::SelectionType,PackageInfo>::const_iterator APit;

  std::cout << "\n#####################\n";

  for (TPCit = m_TypesMetaPackagesCatalogs.begin(); TPCit != m_TypesMetaPackagesCatalogs.end(); ++TPCit)
  {
    std::cout << "List of " << getTypeName(TPCit->first,false,true) << std::endl;

    for (PCit = TPCit->second.begin(); PCit != TPCit->second.end(); ++PCit)
    {
      std::cout << "Package " << PCit->second.ID << std::endl;

      for (APit = PCit->second.AvailablePackages.begin(); APit != PCit->second.AvailablePackages.end(); ++APit)
      {
        std::cout << "\t" << selectionTypeToString(APit->first) << std::endl;

        // dependencies
        if (APit->first == MetaPackageInfo::FLUIDX)
        {
          PackageInfo::Dependencies_t::const_iterator DMit;
          std::list<openfluid::ware::WareID_t>::const_iterator DLit;
          std::cout << "\tDependencies : " << std::endl;

          for (DMit = APit->second.Dependencies.begin(); DMit != APit->second.Dependencies.end(); ++DMit)
          {
            std::cout << "\t\t" << getTypeName(DMit->first,true,false) << std::endl;
            for (DLit = DMit->second.begin(); DLit != DMit->second.end(); ++DLit)
            {
              std::cout << "\t\t\t" << *DLit << std::endl;
            }
          }

        }
      }

      std::cout << std::endl;
    }

    std::cout << "\n#####################\n";
  }
}
*/

} } // namespaces
