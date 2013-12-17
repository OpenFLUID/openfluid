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
  \file FileDownloader.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <openfluid/tools/FileDownloader.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <QEventLoop>
#include <QFile>

// =====================================================================
// =====================================================================


namespace openfluid { namespace tools {


FileDownloader::FileDownloader() : QObject(), m_FileContent(""), m_ContentDownloaded(false)
{
  // Connect signal emitted by mangager when download is completed to the method which store content
  connect(&m_Manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
}


// =====================================================================
// =====================================================================


FileDownloader::~FileDownloader()
{

}


// =====================================================================
// =====================================================================


void FileDownloader::downloadContent(const std::string& URL)
{
  QNetworkRequest Request(QString(URL.c_str()));
  m_Manager.get(Request);
}


// =====================================================================
// =====================================================================


QByteArray FileDownloader::getContent() const
{
  return m_FileContent;
}


// =====================================================================
// =====================================================================


bool FileDownloader::contentIsDownloaded() const
{
  return m_ContentDownloaded;
}


// =====================================================================
// =====================================================================


void FileDownloader::writeToFile(const std::string& FilePath) const
{
  QFile File(QString(FilePath.c_str()));
  if (!File.open(QIODevice::WriteOnly))
    throw openfluid::base::FrameworkException("FileDownloader::writeToFile",
         "Could not open "+FilePath+" : "+qPrintable(File.errorString()));

  File.write(m_FileContent);
  File.close();
}


// =====================================================================
// =====================================================================


void FileDownloader::downloadFinished(QNetworkReply *Reply)
{
  if (!Reply->error())
  {
    m_FileContent = Reply->readAll();
    m_ContentDownloaded = true;
  }

  emit processFinished();
  Reply->deleteLater();
}


// =====================================================================
// =====================================================================


bool FileDownloader::downloadToString(const std::string& URL, std::string& Contents)
{
  Contents.clear();

  FileDownloader Downloader;
  Downloader.downloadContent(URL);

  QEventLoop Loop;
  QObject::connect(&Downloader, SIGNAL(processFinished()), &Loop, SLOT(quit()));
  Loop.exec();

  if (Downloader.contentIsDownloaded())
  {
    // Get content downloaded
    QByteArray Bytes = Downloader.getContent();
    Contents = QString(Bytes).toStdString();
  }

  return Downloader.contentIsDownloaded();
}


// =====================================================================
// =====================================================================


bool FileDownloader::downloadToFile(const std::string& URL, const std::string& FilePath)
{
  FileDownloader Downloader;
  Downloader.downloadContent(URL);

  QEventLoop Loop;
  QObject::connect(&Downloader, SIGNAL(processFinished()), &Loop, SLOT(quit()));
  Loop.exec();

  if (Downloader.contentIsDownloaded())
    Downloader.writeToFile(FilePath);

  return Downloader.contentIsDownloaded();
}


// =====================================================================
// =====================================================================


} } // namespaces


