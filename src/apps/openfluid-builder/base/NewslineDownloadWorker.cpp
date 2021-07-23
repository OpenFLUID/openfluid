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
  @file NewslineDownloadWorker.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>

#include <openfluid/utils/FluidHubAPIClient.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/config.hpp>

#include "NewslineDownloadWorker.hpp"
#include "builderconfig.hpp"


NewslineDownloadWorker::NewslineDownloadWorker(const QString& ShortLocale):
  m_ShortLocale(ShortLocale)
{

}


// =====================================================================
// =====================================================================


NewslineDownloadWorker::~NewslineDownloadWorker()
{

}


// =====================================================================
// =====================================================================


bool NewslineDownloadWorker::donwloadRSSToFile(const QString& RSSFilename, const QString& ShortLocale) const
{
  openfluid::utils::RESTClient::SSLConfiguration SSLConfig;
  SSLConfig.setCertificateVerifyMode(QSslSocket::VerifyNone);
  openfluid::utils::FluidHubAPIClient FHClient;

  if (FHClient.connect(QString::fromStdString(openfluid::config::URL_OFFICIAL_API),SSLConfig))
  {
    QString Content = FHClient.getNews(ShortLocale);

    if (Content.isEmpty())
    {
      Content = FHClient.getNews();
    }

    FHClient.disconnect();

    if (!Content.isEmpty())
    {
      QFile RSSFile(RSSFilename);

      if (!RSSFile.open(QIODevice::WriteOnly | QIODevice::Text))
      {
        return false;
      }

      QTextStream OutFile(&RSSFile);

      OutFile << QString::fromUtf8(Content.toStdString().c_str());
    }

    return !Content.isEmpty();
  }

  return false;
}


// =====================================================================
// =====================================================================


void NewslineDownloadWorker::run()
{
  openfluid::tools::Filesystem::makeDirectory(
    openfluid::base::Environment::getUserDataFullPath(BUILDER_NEWSLINE_CACHERELDIR));

  QString LastUpdFile = QString::fromStdString(openfluid::base::Environment::getUserDataFullPath("%1/lastupdate.info"))
                        .arg(QString::fromStdString(BUILDER_NEWSLINE_CACHERELDIR));

  QString RSSDefaultFile = QString::fromStdString(openfluid::base::Environment::getUserDataFullPath("%1/en.rss"))
                           .arg(QString::fromStdString(BUILDER_NEWSLINE_CACHERELDIR));

  QString RSSFile = QString::fromStdString(openfluid::base::Environment::getUserDataFullPath("%1/%2.rss"))
                    .arg(QString::fromStdString(BUILDER_NEWSLINE_CACHERELDIR),m_ShortLocale);


  if (donwloadRSSToFile(RSSFile,m_ShortLocale))
  {
    QFile LastUpdF(LastUpdFile);
    if (LastUpdF.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream Out(&LastUpdF);
      Out << QDateTime::currentDateTime().toString("yyyy-MM-dd'T'HH:mm:ss");
    }
  }

  emit finished();
}


// =====================================================================
// =====================================================================


bool NewslineDownloadWorker::isTimeForDownload(const QString& ShortLocale)
{
  QDir CacheDir =
      QString::fromStdString(openfluid::base::Environment::getUserDataFullPath(BUILDER_NEWSLINE_CACHERELDIR));

  if (!CacheDir.exists("lastupdate.info"))
  {
    return true;
  }
  else
  {
    // check date
    QString LastUpdFile =
        QString::fromStdString(openfluid::base::Environment::getUserDataFullPath("%1/lastupdate.info"))
        .arg(QString::fromStdString(BUILDER_NEWSLINE_CACHERELDIR));
    QByteArray Line;

    QFile LastUpdF(LastUpdFile);
    if (!LastUpdF.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      return true;
    }

    while (!LastUpdF.atEnd())
    {
      Line = LastUpdF.readLine();
    }

    LastUpdF.close();

    QDateTime DT = QDateTime::fromString(QString(Line),"yyyy-MM-dd'T'HH:mm:ss");

    if (DT.addSecs(7200) < QDateTime::currentDateTime())
    {
      return true;
    }

    // check locale rss file exist
    if (!CacheDir.exists(ShortLocale+".rss"))
    {
      return true;
    }

  }

  return false;
}

