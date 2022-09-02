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
  @file HubManager.cpp

  @author Armel THÖNI <armel.thoni@inra.fr>
*/


#include <string>

#include <QString>
#include <QApplication>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/ui/waresdev/HubManager.hpp>


namespace openfluid { namespace ui { namespace waresdev {

HubManager::HubManager(std::string WareshubUrl) : mp_HubClient(new openfluid::utils::FluidHubAPIClient()), 
  m_HubUrl(WareshubUrl), m_Username(""), m_Password("")
{

}


// =====================================================================
// =====================================================================


HubManager::~HubManager()
{
  delete mp_HubClient;
}


// =====================================================================
// =====================================================================


bool HubManager::isConnected() const
{
  return mp_HubClient->isConnected();
}


// =====================================================================
// =====================================================================


bool HubManager::isLoggedIn() const
{
  return !(m_Username.empty());
}


// =====================================================================
// =====================================================================


bool HubManager::isV0ofAPI() const
{
  return mp_HubClient->isV0ofAPI();
}


// =====================================================================
// =====================================================================


openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t HubManager::getAvailableWaresWithDetails(
  openfluid::ware::WareType Type) const
{
  WaresDetailsByIDByType_t::const_iterator Pos = m_AvailableWaresDetailsByIDByType.find(Type);
  if (Pos != m_AvailableWaresDetailsByIDByType.end())
  {
    return Pos->second;
  }
  else
  {
    return openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t();
  }
}


// =====================================================================
// =====================================================================


openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t HubManager::getAvailableFragmentsWithDetails() const
{
  return m_AvailableFragmentsDetails;
}


// =====================================================================
// =====================================================================


void HubManager::disconnect()
{
  mp_HubClient->disconnect();
}


// =====================================================================
// =====================================================================


bool HubManager::login(const std::string& UserID, const std::string& Password)
{
  m_Password = Password;
  
  if (isV0ofAPI())
  {
    m_Username = UserID;
  }
  else
  {
    if (UserID.find('@') != std::string::npos)
    {
      //fetch unixname from email via request /account/, field unixname
      m_Username = mp_HubClient->getUserUnixname(UserID, m_Password);
    }
    else
    {
      //special case where LDAP username is given
      m_Username = UserID;
    }
  }

  return !m_Username.empty(); // returns true when username found
}


// =====================================================================
// =====================================================================


void HubManager::logout()
{
  m_Username = "";
  m_Password = "";
  mp_HubClient->logout();
}


// =====================================================================
// =====================================================================


bool HubManager::connect()
{
  m_AvailableWaresDetailsByIDByType.clear();
  m_AvailableFragmentsDetails.clear();

  openfluid::utils::RESTClient::SSLConfiguration SSLConfig;
  if (openfluid::base::PreferencesManager::instance()->isWaresdevGitSslNoVerify())
  {
    SSLConfig.setCertificateVerifyMode(QSslSocket::VerifyNone);
  }

  bool OK = mp_HubClient->connect(QString::fromStdString(m_HubUrl), SSLConfig); 
  // TODO convert hub strings from QString to std

  for (const auto& ByType : mp_HubClient->getAllAvailableWares())
  {
    m_AvailableWaresDetailsByIDByType[ByType.first] = mp_HubClient->getAvailableWaresWithDetails(ByType.first, 
                                                                              QString::fromStdString(m_Username));
  }
  m_AvailableFragmentsDetails = mp_HubClient->getAvailableFragmentsWithDetails(QString::fromStdString(m_Username));

  if (!OK)
  {
    emit finished(false, tr("Fetching information failed"));
  }
  else
  {
    emit finished(true, tr("Fetching information completed"));
  }

  if (qApp && qApp->thread() != thread())
  {
    moveToThread(qApp->thread());
  }
  return OK;
}


// =====================================================================
// =====================================================================


std::string HubManager::getUsername() const
{
  return m_Username;
}


// =====================================================================
// =====================================================================


std::string HubManager::getPassword() const
{
  return m_Password;
}


} } } //namespaces
