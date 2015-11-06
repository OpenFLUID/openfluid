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
 @file WaresHubImportWorker.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_WARESDEV_WARESHUBIMPORTWORKER_HPP__
#define __OPENFLUID_WARESDEV_WARESHUBIMPORTWORKER_HPP__

#include <QObject>
#include <QMap>
#include <QStringList>

#include <git2.h>

#include <openfluid/dllexport.hpp>
#include <openfluid/utils/FluidHubAPIClient.hpp>


namespace openfluid { namespace waresdev {


class OPENFLUID_API WaresHubImportWorker: public QObject
{
  Q_OBJECT

  public:

    typedef QMap<openfluid::ware::WareType, openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t>
    WaresDetailsByIDByType_t;

  private:

    openfluid::utils::FluidHubAPIClient* mp_WareshubClient;

    QString m_WaresHubUrl;

    QString m_Username;

    QString m_Password;

    bool m_SslVerify;

    WaresDetailsByIDByType_t m_AvailableWaresDetailsByIDByType;

    std::map<openfluid::ware::WareType, QStringList> m_SelectedWaresUrlByType;

    static int certificateCheckCb(git_cert* cert, int valid, const char* host, void* payload);

  public:

    WaresHubImportWorker(const QString& WareshubUrl, const QString& Username = "", const QString& Password = "",
      bool SslVerify = true);

    ~WaresHubImportWorker();

    bool isConnected() const;

    openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t getAvailableWaresWithDetails(
      openfluid::ware::WareType Type) const;

    void setSelectedWaresUrl(const std::map<openfluid::ware::WareType, QStringList>& SelectedWaresUrlByType);

  public slots :

    bool connect();

    bool clone();

  signals:

    void info(const QString& Message);

    void error(const QString& Message);

    void finished(bool Ok);

    void progressed(int Value);

};


} } // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESHUBIMPORTWORKER_HPP__ */
