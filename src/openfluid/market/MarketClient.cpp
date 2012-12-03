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


#include <openfluid/market/MarketClient.hpp>
#include <openfluid/market/MarketBinPackage.hpp>
#include <openfluid/market/MarketSrcPackage.hpp>
#include <openfluid/tools/CURLDownloader.hpp>
#include <openfluid/config.hpp>

#include <fstream>
#include <deque>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <glibmm/keyfile.h>


namespace openfluid { namespace market {



const std::string MarketClient::LOCK_FILE = ".lock";


// =====================================================================
// =====================================================================


MarketClient::MarketClient() :
    m_IsConnected(false), m_IsLogEnabled(false)
{

  std::string m_TempDir = openfluid::base::RuntimeEnvironment::getInstance()->getTempDir()+"/market";
  MarketPackage::setWorksDirs(boost::filesystem::path(m_TempDir).string(),
                              openfluid::base::RuntimeEnvironment::getInstance()->getMarketBagBinVersionDir(),
                              openfluid::base::RuntimeEnvironment::getInstance()->getMarketBagSrcVersionDir());

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

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagBinDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getMarketBagBinDir())))
    throw openfluid::base::OFException("OpenFLUID framework","MarketClient::initMarketBag()","Unable to initialize market-bag binary directory");

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagSrcDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getMarketBagSrcDir())))
    throw openfluid::base::OFException("OpenFLUID framework","MarketClient::initMarketBag()","Unable to initialize market-bag source directory");

}


// =====================================================================
// =====================================================================


void MarketClient::initMarketTemp()
{

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getTempBuildsDir()));
  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getTempDownloadsDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getTempBuildsDir())) ||
      !boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getTempDownloadsDir())))
    throw openfluid::base::OFException("OpenFLUID framework","MarketClient::initMarketTemp()","Unable to initialize market temp directory");
}


// =====================================================================
// =====================================================================


void MarketClient::lockMarketTemp()
{
  std::string LockFilename = MarketPackage::getTempDir()+"/"+LOCK_FILE;

  if (boost::filesystem::exists(boost::filesystem::path(LockFilename)))
    throw openfluid::base::OFException("OpenFLUID framework","MarketClient::lockMarketTemp()","Unable to lock market temp directory (already locked)");

  std::ofstream(boost::filesystem::path(LockFilename).string().c_str()).close();

  if (!boost::filesystem::exists(boost::filesystem::path(LockFilename)))
    throw openfluid::base::OFException("OpenFLUID framework","MarketClient::lockMarketTemp()","Unable to lock market temp directory (cannot create lock)");
}


// =====================================================================
// =====================================================================


void MarketClient::unlockMarketTemp()
{
  std::string LockFilename = MarketPackage::getTempDir()+"/"+LOCK_FILE;

  boost::filesystem::remove(boost::filesystem::path(LockFilename));

  if (boost::filesystem::exists(boost::filesystem::path(LockFilename)))
    throw openfluid::base::OFException("OpenFLUID framework","MarketClient::unlockMarketTemp()","Unable to unlock market temp directory (cannot remove lock)");
}


// =====================================================================
// =====================================================================


void MarketClient::parseMarketSiteData(const std::string& SiteData)
{

  Glib::KeyFile KFile;

  KFile.load_from_data(SiteData);

  m_MarketInfo.Name = KFile.get_string("openfluid-marketplace","name");
  m_MarketInfo.Description = KFile.get_string("openfluid-marketplace","description");
  m_MarketInfo.Contact = KFile.get_string("openfluid-marketplace","contact");
}


// =====================================================================
// =====================================================================


void MarketClient::parseCatalogData(const std::string& CatalogData)
{

  std::string TmpVersion, TmpArch, TmpID;
  std::vector<std::string> PackagesIDs;
  Glib::KeyFile KFile;

  m_MetaPackagesCatalog.clear();

  std::string BinaryArchKey= "arch." + openfluid::base::RuntimeEnvironment::getInstance()->getArch();


  try
  {
    if (!CatalogData.empty() && KFile.load_from_data(CatalogData))
    {
      PackagesIDs = KFile.get_groups();


      for (unsigned int i=0; i< PackagesIDs.size(); i++)
      {
         std::string TmpID = PackagesIDs[i];


         if (KFile.has_key(TmpID,BinaryArchKey+".file") || KFile.has_key(TmpID,"arch.src.file"))
         {

           m_MetaPackagesCatalog[TmpID].ID = TmpID;

           // name
           if (KFile.has_key(TmpID,"name"))
             m_MetaPackagesCatalog[TmpID].Name = KFile.get_string(TmpID,"name");

           // description
           if (KFile.has_key(TmpID,"description"))
             m_MetaPackagesCatalog[TmpID].Description = KFile.get_string(TmpID,"description");

           // version
           if (KFile.has_key(TmpID,"version"))
             m_MetaPackagesCatalog[TmpID].Version = KFile.get_string(TmpID,"version");

           // authors
           if (KFile.has_key(TmpID,"authors"))
             m_MetaPackagesCatalog[TmpID].Authors = KFile.get_string(TmpID,"authors");

           // binary ?
           if (KFile.has_key(TmpID,BinaryArchKey+".file"))
           {
             m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::BIN] = PackageInfo();
             m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::BIN].URL = m_URL + "/"+openfluid::base::RuntimeEnvironment::getInstance()->getArch()+"/"+KFile.get_string(TmpID,BinaryArchKey+".file");

             // license
             if (KFile.has_key(TmpID,BinaryArchKey+".license"))
              m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::BIN].License = KFile.get_string(TmpID,BinaryArchKey+".license");

             // dependencies
             if (KFile.has_key(TmpID,BinaryArchKey+".depends"))
              m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::BIN].Dependencies = KFile.get_string(TmpID,BinaryArchKey+".depends");


           }

           // source ?
           if (KFile.has_key(TmpID,"arch.src.file"))
           {
             m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC] = PackageInfo();
             m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC].URL = m_URL + "/src/"+KFile.get_string(TmpID,"arch.src.file");

             // license
             if (KFile.has_key(TmpID,"arch.src.license"))
               m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC].License = KFile.get_string(TmpID,"arch.src.license");

             // dependencies
             if (KFile.has_key(TmpID,"arch.src.depends"))
               m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC].Dependencies = KFile.get_string(TmpID,"arch.src.depends");

             // build options
             if (KFile.has_key(TmpID,"arch.src.buildoptions"))
               m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC].BuildOptions = KFile.get_string(TmpID,"arch.src.buildoptions");


           }
         }
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

  MetaPackagesCatalog_t::iterator PCit;
  std::map<MetaPackageInfo::SelectionType,PackageInfo>::iterator PIit;
  MarketLicensesTexts_t::iterator Licit;

  for (PCit = m_MetaPackagesCatalog.begin(); PCit != m_MetaPackagesCatalog.end();++PCit)
  {
    for (PIit = PCit->second.AvailablePackages.begin();PIit != PCit->second.AvailablePackages.end();++PIit)
    {
      if (PIit->second.License != "") m_LicensesTexts[PIit->second.License] = "";
    }
  }

  for (Licit = m_LicensesTexts.begin(); Licit != m_LicensesTexts.end();++Licit)
  {
    if (openfluid::tools::CURLDownloader::downloadToString(m_URL+"/licenses/"+Licit->first+".txt",Licit->second) != openfluid::tools::CURLDownloader::NO_ERROR)
      Licit->second = "(license text not available)";
  }
}


// =====================================================================
// =====================================================================


void MarketClient::connect(const std::string URL)
{
  std::string MarketData;
  std::string CatalogData;

  m_URL = URL;
  m_MarketInfo.clear();
  m_MetaPackagesCatalog.clear();

  initMarketBag();
  initMarketTemp();

  MarketPackage::initialize(m_IsLogEnabled);

  std::string MarketFileURL = m_URL+"/"+openfluid::config::MARKETPLACE_SITEFILE;
  std::string CatalogFileURL = m_URL + "/"+openfluid::config::MARKETPLACE_CATALOGFILE+"_"+openfluid::base::RuntimeEnvironment::getInstance()->getVersion();


  if (!m_IsConnected && openfluid::tools::CURLDownloader::downloadToString(MarketFileURL, MarketData) == openfluid::tools::CURLDownloader::NO_ERROR)
  {
    lockMarketTemp();
    parseMarketSiteData(MarketData);
    m_IsConnected = true;
  }

  if (m_IsConnected)
  {

    if (openfluid::tools::CURLDownloader::downloadToString(CatalogFileURL, CatalogData)  != openfluid::tools::CURLDownloader::NO_ERROR)
      CatalogData.clear();

    parseCatalogData(CatalogData);

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
  m_MetaPackagesCatalog.clear();
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


const MetaPackagesCatalog_t& MarketClient::getMetaPackagesCatalog()
{
  return m_MetaPackagesCatalog;
}


// =====================================================================
// =====================================================================


const MarketLicensesTexts_t& MarketClient::getLicensesTexts()
{
  return m_LicensesTexts;
}


// =====================================================================
// =====================================================================


bool MarketClient::setSelectionFlag(const openfluid::ware::WareID_t& ID, const MetaPackageInfo::SelectionType& Flag)
{
  MetaPackagesCatalog_t::iterator PCit = m_MetaPackagesCatalog.find(ID);

  if (PCit != m_MetaPackagesCatalog.end())
  {
    if (Flag == MetaPackageInfo::BIN
        && PCit->second.AvailablePackages.find(MetaPackageInfo::BIN) ==  PCit->second.AvailablePackages.end())
      return false;

    if (Flag == MetaPackageInfo::SRC
        && PCit->second.AvailablePackages.find(MetaPackageInfo::SRC) ==  PCit->second.AvailablePackages.end())
      return false;

    PCit->second.Selected = Flag;
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


void MarketClient::setSRCBuildOptions(const openfluid::ware::WareID_t& ID, const std::string& BuildOpts)
{
  MetaPackagesCatalog_t::iterator PCit = m_MetaPackagesCatalog.find(ID);

  if (PCit != m_MetaPackagesCatalog.end())
  {
    if (PCit->second.AvailablePackages.find(MetaPackageInfo::SRC) !=  PCit->second.AvailablePackages.end())
    {
      PCit->second.AvailablePackages[MetaPackageInfo::SRC].BuildOptions = BuildOpts;
    }
  }
}

// =====================================================================
// =====================================================================


MetaPackageInfo::SelectionType MarketClient::getSelectionFlag(const openfluid::ware::WareID_t& ID) const
{
  MetaPackagesCatalog_t::const_iterator PCit = m_MetaPackagesCatalog.find(ID);

  if (PCit != m_MetaPackagesCatalog.end())
  {
    return PCit->second.Selected;
  }

  return MetaPackageInfo::NONE;
}


// =====================================================================
// =====================================================================


void MarketClient::preparePackagesInstallation()
{
  MetaPackagesCatalog_t::iterator PCit;

  // clearing list of packages to install
  while (!m_PacksToInstall.empty())
  {
    delete m_PacksToInstall.front();
    m_PacksToInstall.pop_front();
  }

  // creating the list of packages to install
  for (PCit = m_MetaPackagesCatalog.begin(); PCit != m_MetaPackagesCatalog.end();++PCit)
  {
    if (PCit->second.Selected == MetaPackageInfo::BIN)
    {
      m_PacksToInstall.push_back(new MarketBinPackage(PCit->second.ID,PCit->second.AvailablePackages[MetaPackageInfo::BIN].URL));
    }
    if (PCit->second.Selected == MetaPackageInfo::SRC)
    {
      m_PacksToInstall.push_back(new MarketSrcPackage(PCit->second.ID,PCit->second.AvailablePackages[MetaPackageInfo::SRC].URL));

      std::string BuildOptsStr = PCit->second.AvailablePackages[MetaPackageInfo::SRC].BuildOptions;


      if (!BuildOptsStr.empty())
        ((MarketSrcPackage*)m_PacksToInstall.back())->setBuildConfigOptions(BuildOptsStr);
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

} } // namespaces
