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
 @file GitClient.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include <QObject>
#include <git2/cred_helpers.h>
#include "GitClient.hpp"

namespace openfluid { namespace utils {


int CertCheckNb;


// =====================================================================
// =====================================================================


int GitClient::certificateCheckCb(git_cert* cert, int valid, const char* /*host*/, void* /*payload*/)
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


QString GitClient::clone(const QString& FromUrl, const QString& ToPath, const QString& Username,
  const QString& Password, bool SslNoVerify)
{
  QString ErrStr;

  // an empty url leads to a sigabrt in git_clone (an assert fails)
  if (FromUrl.isEmpty())
  {
    ErrStr = QObject::tr("Source url is empty.");
    return ErrStr;
  }

  git_libgit2_init();

  git_clone_options opts = GIT_CLONE_OPTIONS_INIT;

// keep the std::string "transitional" values, otherwise the pointers returned by c_str() are lost
  std::string username = Username.toStdString();
  std::string password = Password.toStdString();

  git_cred_userpass_payload user_pass = { username.c_str(), password.c_str() };

  opts.fetch_opts.callbacks.payload = &user_pass;
  opts.fetch_opts.callbacks.credentials = git_cred_userpass;

  if (SslNoVerify)
    opts.fetch_opts.callbacks.certificate_check = certificateCheckCb;

  opts.checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;

  CertCheckNb = 0;

  git_repository* repo = 0;

  int error = git_clone(&repo, FromUrl.toStdString().c_str(), ToPath.toStdString().c_str(), &opts);

  if (error < 0)
  {
    const git_error* e = giterr_last();

    switch (error)
    {
      case GIT_ECERTIFICATE:
        ErrStr = QObject::tr("Server certificate is invalid (you can try to uncheck ssl verification and re-connect)");
        break;
      case GIT_EUSER:
        ErrStr = QObject::tr("Authentication error");
        break;
      default:
        ErrStr = QString(e->message);
        break;
    }

    ErrStr.prepend(QObject::tr("(code %1/%2): ").arg(error).arg(e->klass));
  }

  git_repository_free(repo);


  git_libgit2_shutdown();

  return ErrStr;
}


// =====================================================================
// =====================================================================


} } // namespaces
