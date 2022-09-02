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
  @file RESTClient.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THONI <armel.thoni@inrae.fr>
*/


#include <QObject>
#include <QBuffer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkReply>
#include <QEventLoop>

#include <openfluid/utilsq/RESTClient.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace utils {


class RequestExecutionImpl : public QObject
{
    Q_OBJECT

    private:

      QNetworkAccessManager m_Manager;

      QBuffer m_DataBuffer;

      RESTClient::Reply m_Reply;


    public slots:

      void requestFinished(QNetworkReply *NetReply)
      {
        QByteArray TmpContent;

        if (!NetReply->error())
        {
          TmpContent = NetReply->readAll();
        }

        m_Reply = RESTClient::Reply(NetReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
                                    NetReply->error(), NetReply->errorString(),
                                    QString(TmpContent));

        emit completed();

        NetReply->deleteLater();
      }


    signals:

        /**
          Signal emitted when request is completed
        */
        void completed();


    public:

      RequestExecutionImpl() : QObject()
      {
        // Connect signal emitted by manager when request is completed to the method which build reply
        connect(&m_Manager,SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
      }

      ~RequestExecutionImpl()
      { }

      void run(const QString& URL, const RESTClient::SSLConfiguration& SSLConfig,
               const QString& Method, const QString& Body, const t_HeadersList RawHeaders)
      {
        m_Reply.clear();
        m_DataBuffer.setData("");

        m_DataBuffer.setData(Body.toUtf8());

        QNetworkRequest Request(URL);

        QSslConfiguration RequestSSLConfig = Request.sslConfiguration();
        RequestSSLConfig.setPeerVerifyMode(SSLConfig.getCertificateVerifyMode());
        Request.setSslConfiguration(RequestSSLConfig);
        for (auto Header : RawHeaders)
        {
          Request.setRawHeader(Header.first, Header.second);
        }

#if OPENFLUID_REST_URL_REDIRECT
        Request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
#endif

        m_Manager.sendCustomRequest(Request,Method.toUtf8(),&m_DataBuffer);
      }


      RESTClient::Reply getReply() const
      {
        return m_Reply;
      }
};


// =====================================================================
// =====================================================================


RESTClient::RESTClient()
{

}


// =====================================================================
// =====================================================================


void RESTClient::removeRawHeader(const QByteArray Key)
{
  t_HeadersList NewHeaders;
  for (auto Header : m_RawHeaders)
  {
    if (Header.first != Key)
    {
      NewHeaders.push_back(Header);
    }
  }
  m_RawHeaders = NewHeaders;
}


// =====================================================================
// =====================================================================


bool RESTClient::hasRawHeader(const QByteArray Key)
{
  for (auto Header : m_RawHeaders)
  {
    if (Header.first == Key)
    {
      return true;
    }
  }
  return false;
}


// =====================================================================
// =====================================================================


void RESTClient::resetRawHeaders()
{
  m_RawHeaders.clear();
}


// =====================================================================
// =====================================================================


void RESTClient::addRawHeader(const QByteArray Key, const QByteArray Value)
{
  m_RawHeaders.push_back(std::pair<QByteArray,QByteArray>(Key, Value));
}


// =====================================================================
// =====================================================================


RESTClient::Reply RESTClient::performRequest(const QString& Path, const QString& Method, const QString& Data) const
{
  RequestExecutionImpl RequestMan;

  QString FullURL = m_BaseURL + Path;

  RequestMan.run(FullURL, m_SSLConfiguration, Method, Data, m_RawHeaders);

  QEventLoop Loop;
  QObject::connect(&RequestMan, SIGNAL(completed()), &Loop, SLOT(quit()));
  Loop.exec();

  return RequestMan.getReply();
}


// =====================================================================
// =====================================================================


void RESTClient::setBaseURL(const QString& URL)
{
  m_BaseURL = URL;

  while (m_BaseURL.endsWith('/' ))
  {
    m_BaseURL.chop(1);
  }
}


// =====================================================================
// =====================================================================


RESTClient::Reply RESTClient::getResource(const QString& Path) const
{
  return performRequest(Path,"GET");
}


// =====================================================================
// =====================================================================


RESTClient::Reply RESTClient::postResource(const QString& Path, const QString& Data) const
{
  return performRequest(Path, "POST", Data);
}


// =====================================================================
// =====================================================================


RESTClient::Reply RESTClient::putResource(const QString& /*Path*/, const QString& /*Data*/) const
{
  throw openfluid::base::FrameworkException(
      openfluid::base::FrameworkException::computeContext(OPENFLUID_CODE_LOCATION),"not implemented");

  return RESTClient::Reply();
}


// =====================================================================
// =====================================================================


RESTClient::Reply RESTClient::patchResource(const QString& /*Path*/, const QString& /*Data*/) const
{
  throw openfluid::base::FrameworkException(
      openfluid::base::FrameworkException::computeContext(OPENFLUID_CODE_LOCATION),"not implemented");

  return RESTClient::Reply();
}


// =====================================================================
// =====================================================================


RESTClient::Reply RESTClient::deleteResource(const QString& /*Path*/, const QString& /*Data*/) const
{
  throw openfluid::base::FrameworkException(
      openfluid::base::FrameworkException::computeContext(OPENFLUID_CODE_LOCATION),"not implemented");

  return RESTClient::Reply();
}


// =====================================================================
// =====================================================================


} }  // namespaces


#include "RESTClient.moc"

