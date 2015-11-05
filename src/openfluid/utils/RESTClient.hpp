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
  @file RESTClient.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_UTILS_RESTCLIENT_HPP__
#define __OPENFLUID_UTILS_RESTCLIENT_HPP__


#include <QSslSocket>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace utils {


class OPENFLUID_API RESTClient
{

  public:

    class Reply
    {
      private:

        int m_StatusCode = -1;

        unsigned int m_NetworkErrorCode = 0;

        QString m_NetworkErrorString;

        QString m_Content;


      public:

        Reply()
        { }

        Reply(int StatusCode, unsigned int NetworkErrorCode, const QString& NetworkErrorString, const QString& Content):
          m_StatusCode(StatusCode),
          m_NetworkErrorCode(NetworkErrorCode),
          m_NetworkErrorString(NetworkErrorString),
          m_Content(Content)
        { }

        int getStatusCode() const
        { return m_StatusCode; }

        int getNetworkErrorCode() const
        { return m_NetworkErrorCode; }

        QString getNetworkErrorString() const
        { return m_NetworkErrorString; }

        QString getContent() const
        { return m_Content; }

        bool isOK() const
        { return m_NetworkErrorCode == 0 && m_StatusCode >= 200 && m_StatusCode < 300; }

        void clear()
        {
          m_StatusCode = -1;
          m_Content.clear();
        }
    };

    class SSLConfiguration
    {
      private:

        QSslSocket::PeerVerifyMode m_CertificateVerifyMode;

      public:

        SSLConfiguration() : m_CertificateVerifyMode(QSslSocket::AutoVerifyPeer)
        { }

        QSslSocket::PeerVerifyMode getCertificateVerifyMode() const
        { return m_CertificateVerifyMode; }

        void setCertificateVerifyMode(QSslSocket::PeerVerifyMode Mode)
        { m_CertificateVerifyMode = Mode; }
    };


  private:

    QString m_BaseURL;

    SSLConfiguration m_SSLConfiguration;

    Reply performRequest(const QString& Path, const QString& Method, const QString& Data = "") const;


  public:

    RESTClient();

    ~RESTClient()
    { }

    void setBaseURL(const QString& URL);

    QString getBaseURL() const
    { return m_BaseURL; }

    void setSSLConfiguration(const SSLConfiguration& Config)
    { m_SSLConfiguration = Config; }

    SSLConfiguration getSSLConfiguration() const
    { return m_SSLConfiguration; }

    Reply getResource(const QString& Path) const;

    Reply postResource(const QString& Path, const QString& Data) const;

    Reply putResource(const QString& Path, const QString& Data) const;

    Reply patchResource(const QString& Path, const QString& Data) const;

    Reply deleteResource(const QString& Path, const QString& Data) const;

};


} }  // namespaces


#endif /* __OPENFLUID_UTILS_RESTCLIENT_HPP__ */
