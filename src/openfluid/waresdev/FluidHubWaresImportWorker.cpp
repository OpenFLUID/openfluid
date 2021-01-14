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
  @file FluidHubWaresImportWorker.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Armel THONI <armel.thoni@inrae.fr>
 */


#include <QCoreApplication>
#include <QFileInfo>

#include <openfluid/waresdev/FluidHubWaresImportWorker.hpp>
#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/utils/GitProxy.hpp>


namespace openfluid { namespace waresdev {


FluidHubWaresImportWorker::FluidHubWaresImportWorker(const QString& WareshubUrl, bool SslNoVerify) :
    mp_HubClient(new openfluid::utils::FluidHubAPIClient()), m_HubUrl(WareshubUrl), m_SslNoVerify(SslNoVerify)
{

}


// =====================================================================
// =====================================================================


FluidHubWaresImportWorker::~FluidHubWaresImportWorker()
{
  delete mp_HubClient;
}


// =====================================================================
// =====================================================================


bool FluidHubWaresImportWorker::isConnected() const
{
  return mp_HubClient->isConnected();
}


// =====================================================================
// =====================================================================


bool FluidHubWaresImportWorker::isLoggedIn() const
{
  return !(m_Username.isEmpty() || m_Username.isNull());
}


// =====================================================================
// =====================================================================


bool FluidHubWaresImportWorker::isV0ofAPI() const
{
  return mp_HubClient->isV0ofAPI();
}


// =====================================================================
// =====================================================================


QString FluidHubWaresImportWorker::getUsername() const
{
  return m_Username;
}


// =====================================================================
// =====================================================================


openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t FluidHubWaresImportWorker::getAvailableWaresWithDetails(
  openfluid::ware::WareType Type) const
  {
  return m_AvailableWaresDetailsByIDByType.value(Type, openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t());
}


// =====================================================================
// =====================================================================


void FluidHubWaresImportWorker::setSelectedWaresUrl(
  const std::map<openfluid::ware::WareType, QStringList>& SelectedWaresUrlByType)
{
  m_SelectedWaresUrlByType = SelectedWaresUrlByType;
}


// =====================================================================
// =====================================================================


bool FluidHubWaresImportWorker::connect()
{
  m_AvailableWaresDetailsByIDByType.clear();

  openfluid::utils::RESTClient::SSLConfiguration SSLConfig;
  if (m_SslNoVerify)
  {
    SSLConfig.setCertificateVerifyMode(QSslSocket::VerifyNone);
  }

  bool Ok = mp_HubClient->connect(m_HubUrl, SSLConfig);

  for (const auto& ByType : mp_HubClient->getAllAvailableWares())
  {
    m_AvailableWaresDetailsByIDByType[ByType.first] = mp_HubClient->getAvailableWaresWithDetails(ByType.first, 
                                                                                                      m_Username);  
  }

  if (!Ok)
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

  return Ok;
}


// =====================================================================
// =====================================================================


void FluidHubWaresImportWorker::disconnect()
{
  mp_HubClient->disconnect();
}


// =====================================================================
// =====================================================================


bool FluidHubWaresImportWorker::login(const QString& Username, const QString& Password)
{
  m_Password = Password;
  
  if (isV0ofAPI())
  {
    m_Username = Username;
  }
  else
  {
    //fetch unixname from email via request /account/, field unixname
    m_Username = QString::fromStdString(mp_HubClient->getUserUnixname(Username.toStdString(), 
                                                                            m_Password.toStdString()));
  }

  return !m_Username.isEmpty(); // returns true when username found
}


// =====================================================================
// =====================================================================


void FluidHubWaresImportWorker::logout()
{
  m_Username = "";
  m_Password = "";
  mp_HubClient->logout();
}


// =====================================================================
// =====================================================================


bool FluidHubWaresImportWorker::clone()
{
  if (!isConnected())
  {
    return false;
  }

  openfluid::waresdev::WareSrcManager* Mgr = openfluid::waresdev::WareSrcManager::instance();

  double ProgressRatio = 100;
  int SelectedWarePathsNb = 0;
  for (const auto& Pair : m_SelectedWaresUrlByType)
  {
    SelectedWarePathsNb += Pair.second.size();
  }
  if (SelectedWarePathsNb)
  {
    ProgressRatio /= SelectedWarePathsNb;
  }

  int Progress = 0;
  bool Ok = true;

  for (const auto& Pair : m_SelectedWaresUrlByType)
  {
    QString WareTypePath = Mgr->getWareTypePath(Pair.first);

    for (const auto& GitUrl : Pair.second)
    {
      QString DestPath = QString("%1/%2").arg(WareTypePath).arg(QFileInfo(GitUrl).fileName());
      
      openfluid::utils::GitProxy Git;
      QObject::connect(&Git, SIGNAL(info(const QString&)), this, SIGNAL(info(const QString&)));
      QObject::connect(&Git, SIGNAL(error(const QString&)), this, SIGNAL(error(const QString&)));

      if (!Git.clone(GitUrl, DestPath, m_Username, m_Password, m_SslNoVerify))
      {
        Ok = false;
        break;
      }

      Progress += ProgressRatio;
      emit progressed(Progress);
    }
  }

  if (Ok)
  {
    emit finished(true,tr("Import completed"));
  }
  else
  {
    emit finished(false, tr("Import failed"));
  }

  if (qApp && qApp->thread() != thread())
  {
    moveToThread(qApp->thread());
  }

  return Ok;
}


} } // namespaces
