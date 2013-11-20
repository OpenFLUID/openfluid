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


/**
  \file NewslineDownloadWorker.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/tools/FileDownloader.hpp>

#include "NewslineDownloadWorker.hpp"
#include "builderconfig.hpp"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>

//TODO to remove
#include <iostream>


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


void NewslineDownloadWorker::run()
{
  QString LastUpdFile =
      QString::fromStdString(openfluid::base::RuntimeEnvironment::getInstance()->getUserDataPath("network/hubs/openfluid-project.org/lastupdate.info"));

  QString RSSDefaultFile =
      QString::fromStdString(openfluid::base::RuntimeEnvironment::getInstance()->getUserDataPath("network/hubs/openfluid-project.org/en.rss"));

  QString RSSFile =
      QString::fromStdString(openfluid::base::RuntimeEnvironment::getInstance()->getUserDataPath("network/hubs/openfluid-project.org/%1.rss"))
      .arg(m_ShortLocale);


  // TODO use conf variables for URL

  bool DownloadSucceeded = false;

  if (openfluid::tools::FileDownloader::downloadToFile(BUILDER_NEWSLINE_SOURCEURL.toStdString()+"?request=rss&lang="+m_ShortLocale.toStdString(),
                                                       RSSFile.toStdString()))
  {
    DownloadSucceeded = true;
  }
  else if(openfluid::tools::FileDownloader::downloadToFile(BUILDER_NEWSLINE_SOURCEURL.toStdString()+"?request=rss",
                                                       RSSDefaultFile.toStdString()))
  {
    DownloadSucceeded = true;
  }

  if (DownloadSucceeded)
  {
    QFile LastUpdF(LastUpdFile);
    if (LastUpdF.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream Out(&LastUpdF);
      std::cout << "la date: " << QDateTime::currentDateTime().toString("yyyy-MM-dd'T'HH:mm:ss").toStdString() << std::endl;
      Out << QDateTime::currentDateTime().toString("yyyy-MM-dd'T'HH:mm:ss");
//      Out.flush();
//      LastUpdF.close();
    }
  }

  emit finished();
}


// =====================================================================
// =====================================================================


bool NewslineDownloadWorker::isTimeForDownload(const QString& ShortLocale)
{
  QDir CacheDir = QString::fromStdString(openfluid::base::RuntimeEnvironment::getInstance()->getUserDataPath("network/hubs/openfluid-project.org"));
  if (!CacheDir.exists("lastupdate.info"))
   return true;
  else
  {
    // check date
    QString LastUpdFile =
        QString::fromStdString(openfluid::base::RuntimeEnvironment::getInstance()->getUserDataPath("network/hubs/openfluid-project.org/lastupdate.info"));
    QByteArray Line;

    QFile LastUpdF(LastUpdFile);
    if (!LastUpdF.open(QIODevice::ReadOnly | QIODevice::Text))
      return true;

    while (!LastUpdF.atEnd())
    {
      Line = LastUpdF.readLine();
    }

    LastUpdF.close();

    QDateTime DT = QDateTime::fromString(QString(Line),"yyyy-MM-dd'T'HH:mm:ss");

    if (DT.addSecs(7200) < QDateTime::currentDateTime())
      return true;

    // check locale rss file exist
    if (!CacheDir.exists(ShortLocale+".rss"))
       return true;

  }

  return false;
}

