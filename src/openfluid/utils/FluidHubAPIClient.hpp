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


#ifndef __OPENFLUID_UTILS_FLUIDHUBAPICLIENT_HPP__
#define __OPENFLUID_UTILS_FLUIDHUBAPICLIENT_HPP__


#include <string>
#include <map>
#include <set>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/utils/HTTPClient.hpp>
#include <openfluid/utils/CppLangHelpers.hpp>


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

    HTTPClient m_RESTClient;

    std::string m_HubName;

    std::string m_HubStatus;

    std::string m_HubAPIVersion;

    bool m_IsV0ofAPI = true;

    std::string m_WareCapabilityName = "wareshub";

    std::set<std::string> m_HubCapabilities;

    void reset();

    bool isCapable(const std::string& Capacity) const;

    static std::string wareTypeToString(openfluid::ware::WareType Type);

    WaresDetailsByID_t getAvailableElementsWithDetails(std::string& Path, const std::string& Username) const;


  public:

    CPPCLASS_DEFAULT_FIVE(FluidHubAPIClient)

    /**
      Connects to the FluidHub given by the URL
      @param[in] URL the URL of the FluidHub to connect to
      @param[in] VerifyCertificate enables/disables SSL certicate verification (default is true)
      @param[in] AllowedRedirections maximum redirections allowed, -1 for no redirection (default is 2)
      @return true if the connection is OK, false otherwise
    */
    bool connect(const std::string& URL, bool VerifyCertificate = true, bool AllowedRedirections = 2);

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
      return !(m_RESTClient.getBaseURL().empty());
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
    std::string getHubURL() const
    {
      return m_RESTClient.getBaseURL();
    }

    /**
      Returns the API version of the current FluidHub
      @return the FluidHub API version
    */
    std::string getHubAPIVersion() const
    {
      return m_HubAPIVersion;
    }

    /**
      Returns the status of the current FluidHub
      @return the FluidHub status
    */
    std::string getHubStatus() const
    {
      return m_HubStatus;
    }

    /**
      Returns the name of the current FluidHub
      @return the FluidHub name
    */
    std::string getHubName() const
    {
      return m_HubName;
    }

    /**
      Returns the capabilities list of the current FluidHub ("news","wareshub", ...)
      @return the FluidHub capabilities
    */
    std::set<std::string> getHubCapabilities() const
    {
      return m_HubCapabilities;
    }

    /**
      Returns the corresponding user unixname if connected and not deprecated API
      @return the connected user unixname
    */
    std::string getUserUnixname(const std::string& Email, const std::string& Password);

    /**
      Returns true if login credentials are valid
      @return True if login credentials are valid, false otherwise
    */
    bool areCredentialsValid(const std::string& Unixname, const std::string& Password);

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
                                                    const std::string& Username = "") const;

    /**
      Returns the detailed list of all available fragments in the current FluidHub
      @param[in] Username Optional username used in returned git URL
      @return the detailed list of fragments
    */
    WaresDetailsByID_t getAvailableFragmentsWithDetails(const std::string& Username = "") const;

    /**
      Returns the news as an RSS string content
      @param[in] Lang Optional lang for news content
      @return the RSS content
    */
    std::string getNews(const std::string& Lang = "") const;


};


} }  // namespaces


#endif /* __OPENFLUID_UTILS_FLUIDHUBAPICLIENT_HPP__ */
