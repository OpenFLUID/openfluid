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
  @file HTTPClient.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <fstream>
#include <algorithm>

#include <curl/curl.h>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/utils/HTTPClient.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace utils {


bool HTTPClient::KeyValueList::exists(const std::string& Key) const
{
  const auto Predicate = [&Key](const std::pair<std::string,std::string>& KV) { return KV.first == Key; };
  auto FoundIt = std::find_if(Data.begin(),Data.end(),Predicate);

  return FoundIt != Data.end();
}


void HTTPClient::KeyValueList::add(const std::string& Key, const std::string& Value)
{
  Data.push_back({Key,Value});
}


void HTTPClient::KeyValueList::remove(const std::string& Key)
{
  const auto Predicate = [&Key](const std::pair<std::string,std::string>& KV) { return KV.first == Key; };
  auto EraseIt = std::remove_if(Data.begin(),Data.end(),Predicate);
  Data.erase(EraseIt,Data.end());
}


// =====================================================================
// =====================================================================


bool HTTPClient::Response::isOK()
{
  return (!OtherError && (NetworkError == 0) && (StatusCode == 200));
}


// =====================================================================
// =====================================================================


static size_t WriteToString(char* Data, size_t Size, size_t NMemb, std::string* Str)
{
  if (Str == nullptr)
  {
    return 0;
  }
 
  const auto Volume = Size*NMemb;
  Str->append(Data,Volume);
 
  return Volume;
}


static size_t WriteToFile(char* Data, size_t Size, size_t NMemb, void* FStreamPtr)
{
  if (FStreamPtr == nullptr)
  {
    return 0;
  }

  const auto Volume = Size*NMemb;
  static_cast<std::ofstream*>(FStreamPtr)->write(Data,Volume);
 
  return Volume;
}


std::string ComposeURL(const std::string& Base,const std::string& Path)
{
  if (Path.empty())  // no composition if Path is empty
  {
    return Base;
  }

  auto BaseEndingSlash = (Base.back() == '/');
  auto PathStartingSlash = (Path.front() == '/');

  std::string ComposedURL;

  if (BaseEndingSlash ^ PathStartingSlash)  // ending or starting slash but not both
  {
    ComposedURL = Base + Path;
  }
  else if (BaseEndingSlash && PathStartingSlash)  // ending and starting slashes
  {
    ComposedURL = Base.substr(0,Base.size()-1) + Path;
  }
  else  // neither ending and starting slash
  {
    ComposedURL = Base + "/" + Path;
  }

  return ComposedURL;
}


HTTPClient::Response HTTPClient::performRequest(const std::string& ReqMethod, const HTTPClient::Request& Req, 
                                                const std::string& ContentOutputPath) const
{
  HTTPClient::Response Rep;
  auto URL = ComposeURL(m_BaseURL,Req.Path);


  // add parameters to URL
  if (!Req.Parameters.Data.empty())
  {
    URL += "?";
    bool First = true;

    for (const auto& P : Req.Parameters.Data)
    {
      if (!First)
      {
        URL += "&";
      }
      else
      {
        First = false;
      }

      URL += P.first+"="+P.second;
    }
  }


  // Curl initialization
  curl_global_init(CURL_GLOBAL_DEFAULT);
  auto Curl = curl_easy_init();


  if (Curl)
  {
    std::string AgentStr = "OpenFLUID/"+openfluid::config::VERSION_FULL;
    curl_easy_setopt(Curl,CURLOPT_USERAGENT,AgentStr.c_str());
    curl_easy_setopt(Curl,CURLOPT_URL,URL.c_str());

    curl_easy_setopt(Curl,CURLOPT_CUSTOMREQUEST,ReqMethod.c_str());


    // set default client headers
    auto Headers = m_DefaultHeaders;

    // append request headers
    for (const auto& H : Req.Headers.Data)
    {
      Headers.Data.push_back(H);
    }
    
    // override libcurl default Content-Type header
    if (!Req.Body.empty() && (Headers.Data.empty() || Headers.exists("Content-Type")))
    {
      Headers.add("Content-Type","text/plain");
    }

    struct curl_slist* HeadersList = nullptr;
    if (!Headers.Data.empty())
    {
      for (const auto& H : Headers.Data)
      {
        auto HeadStr = H.first + ": " + H.second;
        HeadersList = curl_slist_append(HeadersList, HeadStr.c_str());
      }
      curl_easy_setopt(Curl,CURLOPT_HTTPHEADER,HeadersList);
    }


    // payload
    if (!Req.Body.empty())
    {
      curl_easy_setopt(Curl,CURLOPT_POSTFIELDSIZE,long(Req.Body.size()));
      curl_easy_setopt(Curl,CURLOPT_POSTFIELDS,Req.Body.c_str());
    }


    // certificates verification
    curl_easy_setopt(Curl,CURLOPT_SSL_VERIFYPEER,long(m_VerifyCertificates));


    // redirections
    curl_easy_setopt(Curl,CURLOPT_FOLLOWLOCATION,long(m_AllowedRedirections!=0));
    curl_easy_setopt(Curl,CURLOPT_MAXREDIRS,long(m_AllowedRedirections));


    // set write handler to reply or to an output file
    std::ofstream FStream;
    if (ContentOutputPath.empty())
    {
      curl_easy_setopt(Curl,CURLOPT_WRITEFUNCTION,WriteToString);
      curl_easy_setopt(Curl,CURLOPT_WRITEDATA,&Rep.Content);
    }
    else
    {
      FStream.open(ContentOutputPath);

      if (FStream.is_open())
      {
        curl_easy_setopt(Curl,CURLOPT_WRITEFUNCTION,WriteToFile);
        curl_easy_setopt(Curl,CURLOPT_WRITEDATA,&FStream);
      }
      else
      {
        Rep.OtherError = true;
        Rep.ErrorStr = "output file cannot be created/open"; 
      }
    }


    if (!Rep.OtherError)
    {
      // perform request
      auto Result = curl_easy_perform(Curl);
      curl_easy_getinfo(Curl,CURLINFO_RESPONSE_CODE,&Rep.StatusCode);

      FStream.close();

      if (Result != CURLE_OK)
      {
        Rep.NetworkError = Result;
        Rep.ErrorStr = std::string(curl_easy_strerror(Result));
      }
    }

    curl_slist_free_all(HeadersList);
    curl_easy_cleanup(Curl);
  }

  curl_global_cleanup();

  return Rep;
}


// =====================================================================
// =====================================================================


HTTPClient::HTTPClient(const std::string& BaseURL):
  m_BaseURL(BaseURL)
{

}


// =====================================================================
// =====================================================================


void HTTPClient::setAllowedRedirections(int MaxRedir)
{
  m_AllowedRedirections = MaxRedir;
}


// =====================================================================
// =====================================================================


void HTTPClient::setCertificateVerify(bool Verify)
{
  m_VerifyCertificates = Verify;
}


// =====================================================================
// =====================================================================


HTTPClient::Response HTTPClient::getResource(const HTTPClient::Request& Req) const
{
  return performRequest("GET",Req);
}


// =====================================================================
// =====================================================================


HTTPClient::Response HTTPClient::postResource(const HTTPClient::Request& Req) const
{
  return performRequest("POST",Req);
}


// =====================================================================
// =====================================================================


HTTPClient::Response HTTPClient::putResource(const HTTPClient::Request& /*Req*/) const
{
  throw openfluid::base::FrameworkException(
      openfluid::base::FrameworkException::computeContext(OPENFLUID_CODE_LOCATION),"not implemented");

  return HTTPClient::Response();
}


// =====================================================================
// =====================================================================


HTTPClient::Response HTTPClient::patchResource(const HTTPClient::Request& /*Req*/) const
{
  throw openfluid::base::FrameworkException(
      openfluid::base::FrameworkException::computeContext(OPENFLUID_CODE_LOCATION),"not implemented");

  return HTTPClient::Response();
}


// =====================================================================
// =====================================================================


HTTPClient::Response HTTPClient::deleteResource(const HTTPClient::Request& /*Req*/) const
{
  throw openfluid::base::FrameworkException(
      openfluid::base::FrameworkException::computeContext(OPENFLUID_CODE_LOCATION),"not implemented");

  return HTTPClient::Response();
}


// =====================================================================
// =====================================================================


bool HTTPClient::downloadToString(const std::string& URL, std::string& Content)
{
  auto Rep = HTTPClient(URL).performRequest("GET",{});
  Content = Rep.Content;
  return Rep.isOK();
}


// =====================================================================
// =====================================================================


bool HTTPClient::downloadToFile(const std::string& URL, const std::string& FilePath)
{
  return HTTPClient(URL).performRequest("GET",{},FilePath).isOK();
}


} } // namespaces
