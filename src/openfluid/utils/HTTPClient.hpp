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
  @file HTTPClient.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_UTILS_HTTPCLIENT_HPP__
#define __OPENFLUID_UTILS_HTTPCLIENT_HPP__


#include <string>
#include <map>
#include <list>

#include <openfluid/utils/CppLangHelpers.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace utils {


class OPENFLUID_API HTTPClient
{
  public:

    struct OPENFLUID_API KeyValueList
    {
      std::list<std::pair<std::string,std::string>> Data;

      bool exists(const std::string& Key) const;

      void add(const std::string& Key, const std::string& Value);

      void remove(const std::string& Key);
    };

    /**
      Request data structure for HTTPClient
    */
    struct OPENFLUID_API Request
    {
      /**
        @brief Path of the request, relative to the base URL of the client
      */
      std::string Path;

      /**
        @brief Parameters list to pass in URL
      */
      KeyValueList Parameters;

      /**
        @brief Request headers
      */
      KeyValueList Headers;

      /**
        @brief Request body
      */
      std::string Body;
    };

    /**
      Response data structure for HTTPClient
    */
    struct OPENFLUID_API Response
    {
      /**
        @brief Status code of the performed request
      */
      int StatusCode = -1;

      /**
        @brief Network error during request (0 = no error)
      */
      unsigned int NetworkError = 0;

      /**
        @brief Other error during request
      */
      bool OtherError = false;

      /**
        @brief Error string if an error occurred
      */
      std::string ErrorStr;

      /**
        @brief Response content
      */
      std::string Content;

      /**
        Returns true if the request was successfully performed
      */
      bool isOK();
    };


  private:

    std::string m_BaseURL;

    KeyValueList m_DefaultHeaders;

    int m_AllowedRedirections = -1; // redirections not allowed by default

    bool m_VerifyCertificates = true;


    Response performRequest(const std::string& ReqMethod, const HTTPClient::Request& Req, 
                            const std::string& ContentOutputPath = std::string()) const;


  public:

    CPPCLASS_DEFAULT_FIVE(HTTPClient);

    /**
      Constructs an HTTP client with the given base URL
      @param[in] BaseURL the base URL used by the client that will prefix requests paths 
    */
    HTTPClient(const std::string& BaseURL);

    /**
      Returns the base URL of the client
      @return the base URL used by the client that will prefix requests paths
    */
    std::string getBaseURL() const
    {
      return m_BaseURL;
    }

    /**
      Sets the base URL of the client
      @param[in] BaseURL the base URL used by the client that will prefix requests paths
    */
    void setBaseURL(const std::string& BaseURL)
    {
      m_BaseURL = BaseURL;
    }

    /**
     The default headers added to the headers of any request
     @return a reference to the default headers
    */
    KeyValueList& defaultHeaders()
    {
      return m_DefaultHeaders;
    }

    /**
      Sets maximum allowed redirections
      @param[in] MaxRedir maximum allowed redirections
    */
    void setAllowedRedirections(int MaxRedir);

    /**
      Disallows redirections (sets allowed redirections to -1)
    */
    void disallowRedirections()
    {
      setAllowedRedirections(-1);
    }

    /**
      Enables or disables SSL certicates verification
      @param[in] Verify enable or disable verification
    */
    void setCertificateVerify(bool Verify);

    /**
      Performs a GET request
      @param[in] Req the request data
      @return the response to the request
    */
    Response getResource(const Request& Req) const;

    /**
      Performs a POST request
      @param[in] Req the request data
      @return the response to the request
    */
    Response postResource(const Request& Req) const;

    /**
      Performs a PUT request
      @param[in] Req the request data
      @return the response to the request
      @warning not implemented
    */
    Response putResource(const Request& Req) const;

    /**
      Performs a PATCH request
      @param[in] Req the request data
      @return the response to the request
      @warning not implemented
    */
    Response patchResource(const Request& Req) const;

    /**
      Performs a DELETE request
      @param[in] Req the request data
      @return the response to the request
      @warning not implemented
    */
    Response deleteResource(const Request& Req) const;

    /**
      Downloads URL contents to string
      @param[in] URL The URL for the download
      @param[out] Content The string containing the downloaded contents
    */
    static bool downloadToString(const std::string& URL, std::string& Content);

    /**
      Downloads URL contents to file
      @param[in] URL The URL for the download
      @param[out] FilePath The path of the file to store the downloaded contents
    */
    static bool downloadToFile(const std::string& URL, const std::string& FilePath);
};


} } // namespaces


#endif /* __OPENFLUID_UTILS_HTTPCLIENT_HPP__ */
