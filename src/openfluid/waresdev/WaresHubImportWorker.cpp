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
 @file WaresHubImportWorker.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <QCoreApplication>
#include <QFileInfo>

#include <openfluid/waresdev/WaresHubImportWorker.hpp>
#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/utils/GitProxy.hpp>


namespace openfluid { namespace waresdev {


WaresHubImportWorker::WaresHubImportWorker(const QString& WareshubUrl, const QString& Username,
  const QString& Password, bool SslNoVerify) :
    mp_WareshubClient(new openfluid::utils::FluidHubAPIClient()), m_WaresHubUrl(WareshubUrl), m_Username(Username),
        m_Password(Password), m_SslNoVerify(SslNoVerify)
{

}


// =====================================================================
// =====================================================================


WaresHubImportWorker::~WaresHubImportWorker()
{
  delete mp_WareshubClient;
}


// =====================================================================
// =====================================================================


bool WaresHubImportWorker::isConnected() const
{
  return mp_WareshubClient->isConnected();
}


// =====================================================================
// =====================================================================


openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t WaresHubImportWorker::getAvailableWaresWithDetails(
  openfluid::ware::WareType Type) const
  {
  return m_AvailableWaresDetailsByIDByType.value(Type, openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t());
}


// =====================================================================
// =====================================================================


void WaresHubImportWorker::setSelectedWaresUrl(
  const std::map<openfluid::ware::WareType, QStringList>& SelectedWaresUrlByType)
{
  m_SelectedWaresUrlByType = SelectedWaresUrlByType;
}


// =====================================================================
// =====================================================================


bool WaresHubImportWorker::connect()
{
  m_AvailableWaresDetailsByIDByType.clear();

  openfluid::utils::RESTClient::SSLConfiguration SSLConfig;
  if (m_SslNoVerify)
  {
    SSLConfig.setCertificateVerifyMode(QSslSocket::VerifyNone);
  }

  bool Ok = mp_WareshubClient->connect(m_WaresHubUrl, SSLConfig);

  for (const auto& ByType : mp_WareshubClient->getAllAvailableWares())
  {
    m_AvailableWaresDetailsByIDByType[ByType.first] = mp_WareshubClient->getAvailableWaresWithDetails(ByType.first,
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


void WaresHubImportWorker::disconnect()
{
  mp_WareshubClient->disconnect();
}


// =====================================================================
// =====================================================================


bool WaresHubImportWorker::clone()
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
