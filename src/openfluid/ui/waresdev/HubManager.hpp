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
  @file HubManager.hpp

  @author Armel THÖNI <armel.thoni@inra.fr>
 */

#ifndef __OPENFLUID_UIWARESDEV_HUBMANAGER_HPP__
#define __OPENFLUID_UIWARESDEV_HUBMANAGER_HPP__


#include <QObject>

#include <openfluid/utilsq/FluidHubAPIClient.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API HubManager: public QObject
{
  Q_OBJECT

  private:

    typedef std::map<openfluid::ware::WareType, openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t>
      WaresDetailsByIDByType_t;
      
    openfluid::utils::FluidHubAPIClient* mp_HubClient;

    std::string m_HubUrl;

    std::string m_Username = "";
    std::string m_Password = "";

    WaresDetailsByIDByType_t m_AvailableWaresDetailsByIDByType;

    openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t m_AvailableFragmentsDetails;


  signals:

    void info(const QString& Message);

    void error(const QString& Message);

    void finished(bool Ok, const QString& Message);

    void progressed(int Value);


  public slots :

    bool connect();

    void disconnect();

    bool login(const std::string& UserID, const std::string& Password);

    void logout();
  
  
  public:

    bool isConnected() const;

    bool isLoggedIn() const;

    bool isV0ofAPI() const; // V0 is for Wareshub API, V1 is for Fluidhub API

    std::string getUsername() const;

    std::string getPassword() const;

    openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t getAvailableWaresWithDetails(
      openfluid::ware::WareType Type) const;

    openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t getAvailableFragmentsWithDetails() const;

    HubManager(std::string WareshubUrl);

    ~HubManager();


};

} } } //namespaces

#endif /* __OPENFLUID_UIWARESDEV_HUBMANAGER_HPP__ */
