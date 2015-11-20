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

#include "WaresHubImportWorker.hpp"

#include <QApplication>
#include <QFileInfo>

#include <git2/cred_helpers.h>

#include <openfluid/waresdev/WareSrcManager.hpp>


namespace openfluid { namespace waresdev {


int CertCheckNb;


// =====================================================================
// =====================================================================


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
    SSLConfig.setCertificateVerifyMode(QSslSocket::VerifyNone);

  bool Ok = mp_WareshubClient->connect(m_WaresHubUrl, SSLConfig);

  for (const auto& ByType : mp_WareshubClient->getAllAvailableWares())
    m_AvailableWaresDetailsByIDByType[ByType.first] = mp_WareshubClient->getAvailableWaresWithDetails(ByType.first,
                                                                                                      m_Username);

  if (!Ok)
    emit finished(false, tr("Fetching information failed"));
  else
    emit finished(true, tr("Fetching information done"));

  if (qApp && qApp->thread() != thread())
    moveToThread(qApp->thread());

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


int WaresHubImportWorker::certificateCheckCb(git_cert* cert, int valid, const char* host, void* payload)
{
  (void) (cert);
  (void) (valid);

  if (CertCheckNb > 10)
    return GIT_EUSER;

  CertCheckNb++;

  return 1;
}


// =====================================================================
// =====================================================================


bool WaresHubImportWorker::clone()
{
  if (!isConnected())
    return false;

  QString ErrStr;

  openfluid::waresdev::WareSrcManager* Mgr = openfluid::waresdev::WareSrcManager::instance();

  git_libgit2_init();

  git_clone_options opts = GIT_CLONE_OPTIONS_INIT;
  opts.fetch_opts = GIT_FETCH_OPTIONS_INIT;
  opts.checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;

  // keep the std::string "transitional" values, otherwise the pointers returned by c_str() are lost
  std::string username = m_Username.toStdString();
  std::string password = m_Password.toStdString();

  git_cred_userpass_payload user_pass = { username.c_str(), password.c_str() };

  opts.fetch_opts.callbacks.payload = &user_pass;
  opts.fetch_opts.callbacks.credentials = git_cred_userpass;

  if (m_SslNoVerify)
    opts.fetch_opts.callbacks.certificate_check = certificateCheckCb;

  opts.checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;

  for (const auto& Pair : m_SelectedWaresUrlByType)
  {
    QString WareTypePath = Mgr->getWareTypePath(Pair.first);

    for (const auto& GitUrl : Pair.second)
    {
      CertCheckNb = 0;

      git_repository* repo = 0;

      QString DestPath = QString("%1/%2").arg(WareTypePath).arg(QFileInfo(GitUrl).fileName());

      emit info(tr("Cloning from \"%1\" to \"%2\"").arg(GitUrl).arg(DestPath));

      int error = git_clone(&repo, GitUrl.toStdString().c_str(), DestPath.toStdString().c_str(), &opts);

      if (error < 0)
      {
        const git_error* e = giterr_last();

        switch (error)
        {
          case GIT_ECERTIFICATE:
            ErrStr = tr("Server certificate is invalid (you can try to uncheck ssl verification and re-connect)");
            break;
          case GIT_EUSER:
            ErrStr = tr("Authentication error");
            break;
          default:
            ErrStr = QString(e->message);
            break;
        }

        ErrStr.prepend(tr("(code %1/%2): ").arg(error).arg(e->klass));

        git_repository_free(repo);

        break;
      }

      git_repository_free(repo);
    }
  }

  git_libgit2_shutdown();


  bool Ok = ErrStr.isEmpty();

  if (Ok)
    emit finished(true, tr("Cloning done"));
  else
    emit finished(false, tr("Cloning failed %1").arg(ErrStr));

  if (qApp && qApp->thread() != thread())
    moveToThread(qApp->thread());

  return Ok;
}


// =====================================================================
// =====================================================================


} } // namespaces
