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
  \file FileDownloader.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#ifndef __FILEDOWNLOADER_HPP__
#define __FILEDOWNLOADER_HPP__

#include <string>
#include <openfluid/dllexport.hpp>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>


// =====================================================================
// =====================================================================

namespace openfluid { namespace tools {

/**
  Management of data downloads over network
*/
class DLLEXPORT FileDownloader : public QObject
{
  Q_OBJECT

  private:

    QNetworkAccessManager m_Manager;
    QByteArray m_FileContent;
    bool m_ContentDownloaded;

    FileDownloader();

    ~FileDownloader();

    /**
      @action Send request to get content of URL file
    */
    void downloadContent(const std::string& URL);

    /**
      @return content stored in attribute
    */
    QByteArray getContent() const;

    /**
      @return true if file content is downloaded and stored
    */
    bool contentIsDownloaded() const;

    /**
      Writes content stored in a file
      @param FilePath Path where the file will be written
    */
    void writeToFile(const std::string& FilePath) const;

  // List of created Qt signals
  signals:

    /**
      Signal emitted when store of downloaded file content is finished
    */
    void processFinished();

  // List of created Qt slots
  public slots:

    /**
      Stores file content downloaded in attribute
      @param[in] Reply Reply of sent request
    */
    void downloadFinished(QNetworkReply *Reply);


  public:

    /**
     Downloads URL contents to string
     @param[in] URL The URL for the download
     @param[out] Contents The string containing the downloaded contents
    */
    static bool downloadToString(const std::string& URL, std::string& Contents);

    /**
     Downloads URL contents to file
     @param[in] URL The URL for the download
     @param[out] FilePath The path of the file to store the downloaded contents
    */
    static bool downloadToFile(const std::string& URL, const std::string& FilePath);

};


} } // namespaces



#endif /* __FILEDOWNLOADER_HPP__ */

