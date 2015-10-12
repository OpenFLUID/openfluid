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
  @file FluidHubClient.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_UTILS_FLUIDHUBCLIENT_HPP__
#define __OPENFLUID_UTILS_FLUIDHUBCLIENT_HPP__


#include <set>
#include <string>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>


namespace openfluid { namespace utils {


class OPENFLUID_API FluidHubClient
{
  public:

    // TODO unify with other waretype definitions (PluggableWare, WareSrcManager, ...)
    enum WareType { UNDEFINED, OBSERVER, SIMULATOR, BUILDEREXT, OTHER };


    class WareDetailedDescription
    {
      public:

        std::string ShortDescription;

        std::string GitUrl;

        std::set<std::string> ROUsers;

        std::set<std::string> RWUsers;
    };


    typedef std::map<WareType,std::set<openfluid::ware::WareID_t>> WaresDescByType_t;

    typedef std::map<WareType,std::map<openfluid::ware::WareID_t,WareDetailedDescription>> WaresDetailedDescByType_t;


  private:

    std::string m_HubURL;

    std::string m_HubName;

    std::string m_HubStatus;

    std::string m_HubAPIVersion;

    std::set<std::string> m_HubCapabilities;

    void reset();

    bool isCapable(const std::string& Capacity) const;


  public:

    FluidHubClient();

    ~FluidHubClient();

    /**
      Connects to the FluidHub given by the URL
      @param[in] URL the URL of the FluidHub to connect to
      @return true if the connection is OK, false otherwise
    */
    bool connect(const std::string& URL);

    /**
      Disconnects from the connected FluidHub
    */
    void disconnect();

    /**
      Returns true if the client is currently connected
      @return true if connected
    */
    bool isConnected() const
    { return !(m_HubURL.empty()); }

    /**
      Returns the URL of the current FluidHub
      @return the FluidHub URL
    */
    std::string getHubURL() const
    { return m_HubURL; }

    /**
      Returns the API version of the current FluidHub
      @return the FluidHub API version
    */
    std::string getHubAPIVersion() const
    { return m_HubAPIVersion; }

    /**
      Returns the status of the current FluidHub
      @return the FluidHub status
    */
    std::string getHubStatus() const
    { return m_HubStatus; }

    /**
      Returns the name of the current FluidHub
      @return the FluidHub name
    */
    std::string getHubName() const
    { return m_HubName; }

    /**
      Returns the capabilities list of the current FluidHub ("news","wareshub", ...)
      @return the FluidHub capabilities
    */
    std::set<std::string> getHubCapabilities() const
    { return m_HubCapabilities; }

    /**
      Returns the list of all available wares in the current FluidHub
      @return the list of wares categorized by ware type
    */
    WaresDescByType_t getAvailableWares() const;

    /**
      Returns the detailed list of all available wares in the current FluidHub
      @param[in] Username Optional username used in returned git URL
      @return the detailed list of wares categorized by ware type
    */
    WaresDetailedDescByType_t getAvailableWaresWithDetails(const std::string& Username = "") const;

    /**
      Returns the news as an RSS string content
      @param[in] Lang Optional lang for news content
      @return the RSS content
    */
    std::string getNews(const std::string& Lang = "") const;


};


} }  // namespaces


#endif /* __OPENFLUID_UTILS_FLUIDHUBCLIENT_HPP__ */
