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
    m_IsConnected(false)
{

  std::string m_TempDir = openfluid::base::RuntimeEnvironment::getInstance()->getTempDir()+"/market";
  MarketPackage::setWorksDirs(boost::filesystem::path(m_TempDir).string(),
                              openfluid::base::RuntimeEnvironment::getInstance()->getMarketBagVersionDir());

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


void MarketClient::splitPackageFilename(const std::string& Filename, std::string& ID, std::string& Version, std::string& Arch)
{
  std::string TmpStr = Filename;
  std::vector< std::string > TmpTokens;

  TmpStr.erase(TmpStr.size()-5,5);
  openfluid::tools::TokenizeString(TmpStr,TmpTokens,"_");

  unsigned int TmpTokensNbr = TmpTokens.size();

  Arch = TmpTokens[TmpTokensNbr-1];
  Version = TmpTokens[TmpTokensNbr-2];

  ID.clear();

  for (unsigned int i=0;i<TmpTokensNbr-2;i++)
  {
    ID = ID + TmpTokens[i];
    if (i != TmpTokensNbr-3) ID = ID+"_";
  }

}


// =====================================================================
// =====================================================================


void MarketClient::initMarketBag()
{

  boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagDir()));

  if (!boost::filesystem::is_directory(boost::filesystem::path(MarketPackage::getMarketBagDir())))
    throw openfluid::base::OFException("OpenFLUID framework","MarketClient::initMarketBag()","Unable to initialize market-bag directory");
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

  // std::cout << boost::filesystem::path(LockFilename).string() << std::endl;

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

  m_MarketInfo.Name = KFile.get_string("openfluid-market-site","name");
  m_MarketInfo.Description = KFile.get_string("openfluid-market-site","description");
  m_MarketInfo.Contact = KFile.get_string("openfluid-market-site","contact");
}


// =====================================================================
// =====================================================================


void MarketClient::parseCatalogsData(const std::string& BinCatalogData, const std::string& SrcCatalogData)
{

  std::string TmpVersion, TmpArch, TmpID;
  std::vector<std::string> PackagesNames;
  Glib::KeyFile KFile;

  m_MetaPackagesCatalog.clear();


  // bin packages
  try
  {
    if (!BinCatalogData.empty() && KFile.load_from_data(BinCatalogData))
    {
      PackagesNames = KFile.get_groups();

      for (unsigned int i=0; i< PackagesNames.size(); i++)
      {
        splitPackageFilename(PackagesNames[i],TmpID,TmpVersion,TmpArch);

        if (TmpVersion == openfluid::base::RuntimeEnvironment::getInstance()->getFullVersion())
        {
          m_MetaPackagesCatalog[TmpID].ID = TmpID;

          m_MetaPackagesCatalog[TmpID].ID = TmpID;
          m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::BIN] = PackageInfo();

          m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::BIN].URL = m_URL + "/"+openfluid::base::RuntimeEnvironment::getInstance()->getArch()+"/"+PackagesNames[i];
          m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::BIN].Name = KFile.get_string(PackagesNames[i],"name");
          m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::BIN].Description = KFile.get_string(PackagesNames[i],"description");
          m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::BIN].Authors = KFile.get_string(PackagesNames[i],"authors");
          m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::BIN].License = KFile.get_string(PackagesNames[i],"license");
        }

      }

    }
  }
  catch (...) {};


  // source packages

  PackagesNames.clear();

  try
  {
    if (!SrcCatalogData.empty() && KFile.load_from_data(SrcCatalogData))
    {
      PackagesNames = KFile.get_groups();

      for (unsigned int i=0; i< PackagesNames.size(); i++)
      {
        splitPackageFilename(PackagesNames[i],TmpID,TmpVersion,TmpArch);

        if (TmpVersion == openfluid::base::RuntimeEnvironment::getInstance()->getFullVersion())
        {
          m_MetaPackagesCatalog[TmpID].ID = TmpID;
          m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC] = PackageInfo();

          m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC].URL = m_URL + "/src/"+PackagesNames[i];
          m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC].Name = KFile.get_string(PackagesNames[i],"name");
          m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC].Description = KFile.get_string(PackagesNames[i],"description");
          m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC].Authors = KFile.get_string(PackagesNames[i],"authors");
          m_MetaPackagesCatalog[TmpID].AvailablePackages[MetaPackageInfo::SRC].License = KFile.get_string(PackagesNames[i],"license");
        }
      }
    }
  }
  catch (...) {};

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
  std::string BinCatalogData;
  std::string SrcCatalogData;

  m_URL = URL;
  m_MarketInfo.clear();
  m_MetaPackagesCatalog.clear();

  initMarketBag();
  initMarketTemp();

  MarketPackage::initialize();

  std::string MarketFileURL = m_URL+"/Market";
  std::string BinCatalogFileURL = m_URL + "/"+openfluid::base::RuntimeEnvironment::getInstance()->getArch()+"/Catalog";
  std::string SrcCatalogFileURL = m_URL + "/src/Catalog";

  if (!m_IsConnected && openfluid::tools::CURLDownloader::downloadToString(MarketFileURL, MarketData) == openfluid::tools::CURLDownloader::NO_ERROR)
  {
    lockMarketTemp();
    parseMarketSiteData(MarketData);
    m_IsConnected = true;
  }

  if (m_IsConnected)
  {

    if (openfluid::tools::CURLDownloader::downloadToString(BinCatalogFileURL, BinCatalogData)  != openfluid::tools::CURLDownloader::NO_ERROR)
      BinCatalogData.clear();

    if (openfluid::tools::CURLDownloader::downloadToString(SrcCatalogFileURL, SrcCatalogData)  != openfluid::tools::CURLDownloader::NO_ERROR)
      SrcCatalogData.clear();

    parseCatalogsData(BinCatalogData,SrcCatalogData);

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


bool MarketClient::setSelectionFlag(const openfluid::base::FuncID_t& ID, const MetaPackageInfo::SelectionType& Flag)
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


MetaPackageInfo::SelectionType MarketClient::getSelectionFlag(const openfluid::base::FuncID_t& ID) const
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


void MarketClient::installSelection(const bool /*IgnoreMissing = true*/)
{

  MetaPackagesCatalog_t::iterator PCit;
  std::vector<MarketPackage*> PacksToInstall;
  std::vector<MarketPackage*>::iterator PIit;


  // creating the list of packages to install
  for (PCit = m_MetaPackagesCatalog.begin(); PCit != m_MetaPackagesCatalog.end();++PCit)
  {
    if (PCit->second.Selected == MetaPackageInfo::BIN)
    {
      // std::cout << "BIN to install : " << PCit->second.AvailablePackages[MetaPackageInfo::BIN].URL << std::endl;
      PacksToInstall.push_back(new MarketBinPackage(PCit->second.ID,PCit->second.AvailablePackages[MetaPackageInfo::BIN].URL));
    }
    if (PCit->second.Selected == MetaPackageInfo::SRC)
    {
      // std::cout << "SRC to install : " << PCit->second.AvailablePackages[MetaPackageInfo::SRC].URL << std::endl;
      PacksToInstall.push_back(new MarketSrcPackage(PCit->second.ID,PCit->second.AvailablePackages[MetaPackageInfo::SRC].URL));
    }
  }


  // download and install of packages
  for (PIit = PacksToInstall.begin(); PIit != PacksToInstall.end();++PIit)
  {
    (*PIit)->download();
  }


  for (PIit = PacksToInstall.begin(); PIit != PacksToInstall.end();++PIit)
  {
    (*PIit)->process();
  }

  for (PIit = PacksToInstall.begin(); PIit != PacksToInstall.end();++PIit)
  {
    delete (*PIit);
  }
}


} } // namespaces
