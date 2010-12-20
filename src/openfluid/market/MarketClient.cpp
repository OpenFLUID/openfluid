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

#include <fstream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>


namespace openfluid { namespace market {


const std::string MarketClient::BUILDS_SUBDIR = "market/builds";
const std::string MarketClient::DLOADS_SUBDIR = "market/downloads";
const std::string MarketClient::LOCK_FILE = "market/.lock";


// =====================================================================
// =====================================================================


MarketClient::MarketClient()
{

  std::string OFTempDir = openfluid::base::RuntimeEnvironment::getInstance()->getTempDir();
  MarketPackage::setWorksDirs(boost::filesystem::path(OFTempDir+"/"+BUILDS_SUBDIR).string(),
                              boost::filesystem::path(OFTempDir+"/"+DLOADS_SUBDIR).string(),
                              openfluid::base::RuntimeEnvironment::getInstance()->getMarketBagVersionDir());
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
  if (!boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getMarketBagDir())))
    throw openfluid::base::OFException("OpenFLUID framework","MarketClient::initMarketBag()","Unable to initialize market-bag directory");
}


// =====================================================================
// =====================================================================


void MarketClient::initMarketTemp()
{

  if (!boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getTempBuildsDir())) ||
      !boost::filesystem::create_directories(boost::filesystem::path(MarketPackage::getTempDownloadsDir())))
    throw openfluid::base::OFException("OpenFLUID framework","MarketClient::initMarketTemp()","Unable to initialize market temp directory");
}


// =====================================================================
// =====================================================================


void MarketClient::lockMarketTemp()
{
  std::string LockFilename = openfluid::base::RuntimeEnvironment::getInstance()->getTempDir()+"/"+LOCK_FILE;

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
  std::string LockFilename = openfluid::base::RuntimeEnvironment::getInstance()->getTempDir()+"/"+LOCK_FILE;

  if (!boost::filesystem::remove(boost::filesystem::path(LockFilename)))
    throw openfluid::base::OFException("OpenFLUID framework","MarketClient::unlockMarketTemp()","Unable to unlock market temp directory (cannot remove lock)");
}


// =====================================================================
// =====================================================================


openfluid::tools::CURLDownloader::ErrorCode MarketClient::connect(const std::string URL)
{
  m_MarketInfo.URL = URL;

  initMarketBag();
  initMarketTemp();

  MarketPackage::initialize();

  lockMarketTemp();

  throw openfluid::base::OFException("OpenFLUID framework","MarketClient::connect()","under construction");

}

// =====================================================================
// =====================================================================


void MarketClient::disconnect()
{
  unlockMarketTemp();

  throw openfluid::base::OFException("OpenFLUID framework","MarketClient::disconnect()","under construction");
}


// =====================================================================
// =====================================================================


void MarketClient::getMarketInfo(MarketInfo& /*Info*/)
{
  throw openfluid::base::OFException("OpenFLUID framework","MarketClient::getMarketInfo()","under construction");
}


// =====================================================================
// =====================================================================


void MarketClient::getPackagesInfos(PackagesInfosList_t& /*Infos*/)
{
  throw openfluid::base::OFException("OpenFLUID framework","MarketClient::getPackagesInfos()","under construction");
}



} } // namespaces
