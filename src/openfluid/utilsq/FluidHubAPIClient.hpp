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
  @file FluidHubAPIClient.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THONI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_UTILSQ_FLUIDHUBAPICLIENT_HPP__
#define __OPENFLUID_UTILSQ_FLUIDHUBAPICLIENT_HPP__


#include <set>

#include <QString>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/utilsq/RESTClient.hpp>


namespace openfluid { namespace utils {


class OPENFLUID_API FluidHubAPIClient
{
  public:

    class WareDetailedDescription
    {
      public:

        std::string ShortDescription;

        std::string GitUrl;

        std::vector<std::string> GitBranches;

        std::map<std::string,unsigned int> IssuesCounters;

        std::set<std::string> ROUsers;

        std::set<std::string> RWUsers;
    };

    typedef std::map<openfluid::ware::WareType,std::set<openfluid::ware::WareID_t>> WaresListByType_t;

    typedef std::map<openfluid::ware::WareID_t,WareDetailedDescription> WaresDetailsByID_t;


  private:

    RESTClient m_RESTClient;

    RESTClient::SSLConfiguration m_SSLConfig;

    QString m_HubName;

    QString m_HubStatus;

    QString m_HubAPIVersion;

    bool m_IsV0ofAPI = true;

    QString m_WareCapabilityName = "wareshub";

    std::set<QString> m_HubCapabilities;

    void reset();

    bool isCapable(const QString& Capacity) const;

    static QString wareTypeToString(openfluid::ware::WareType Type);


  public:

    FluidHubAPIClient() = default;

    ~FluidHubAPIClient()
    { }

    /**
      Connects to the FluidHub given by the URL
      @param[in] URL the URL of the FluidHub to connect to
      @param[in] SSLConfig the SSL configuration for the connection (if needed)
      @return true if the connection is OK, false otherwise
    */
    bool connect(const QString& URL, const RESTClient::SSLConfiguration& SSLConfig = RESTClient::SSLConfiguration());

    /**
      Disconnects from the connected FluidHub
    */
    void disconnect();

    /**
      Unsets the authentication informations
    */
    void logout();

    /**
      Returns true if the client is currently connected
      @return true if connected
    */
    bool isConnected() const
    {
      return !(m_RESTClient.getBaseURL().isEmpty());
    }

    /**
      Returns true if the serveur uses the deprecated API (old wareshub)
      @return true if connected
    */
    bool isV0ofAPI() const
    {
      return m_IsV0ofAPI;
    }

    /**
      Returns the URL of the current FluidHub
      @return the FluidHub URL
    */
    QString getHubURL() const
    {
      return m_RESTClient.getBaseURL();
    }

    /**
      Returns the API version of the current FluidHub
      @return the FluidHub API version
    */
    QString getHubAPIVersion() const
    {
      return m_HubAPIVersion;
    }

    /**
      Returns the status of the current FluidHub
      @return the FluidHub status
    */
    QString getHubStatus() const
    {
      return m_HubStatus;
    }

    /**
      Returns the name of the current FluidHub
      @return the FluidHub name
    */
    QString getHubName() const
    {
      return m_HubName;
    }

    /**
      Returns the capabilities list of the current FluidHub ("news","wareshub", ...)
      @return the FluidHub capabilities
    */
    std::set<QString> getHubCapabilities() const
    {
      return m_HubCapabilities;
    }

    /**
      Returns the corresponding user unixname if connected and not deprecated API
      @return the connected user unixname
    */
    std::string getUserUnixname(const std::string& Email, const std::string& Password);

    /**
      Returns the list of all available wares in the current FluidHub
      @return the list of wares categorized by ware type
    */
    WaresListByType_t getAllAvailableWares() const;

    /**
      Returns the detailed list of all available wares in the current FluidHub
      @param[in] Type the type of wares to list
      @param[in] Username Optional username used in returned git URL
      @return the detailed list of wares of the give type
    */
    WaresDetailsByID_t getAvailableWaresWithDetails(openfluid::ware::WareType Type, 
                                                    const QString& Username = "") const;

    /**
      Returns the news as an RSS string content
      @param[in] Lang Optional lang for news content
      @return the RSS content
    */
    QString getNews(const QString& Lang = "") const;


};


} }  // namespaces


#endif /* __OPENFLUID_UTILSQ_FLUIDHUBAPICLIENT_HPP__ */
