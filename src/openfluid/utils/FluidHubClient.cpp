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
  @file FluidHubClient.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <openfluid/utils/FluidHubClient.hpp>
#include <openfluid/utils/FileDownloader.hpp>
#include <rapidjson/document.h>


namespace openfluid { namespace utils {


void JSONArrayToStringSet(const rapidjson::Value& Obj, std::set<std::string>& Set)
{
  Set.clear();

  if (Obj.IsArray() && !Obj.Empty())
  {
    for (unsigned int i=0;i<Obj.Capacity();i++)
    {
      if (Obj[i].IsString())
        Set.insert(std::string(Obj[i].GetString()));
    }
  }
}


// =====================================================================
// =====================================================================


void JSONObjectToDetailedWares(const rapidjson::Value& Obj,
                               std::map<std::string,openfluid::utils::FluidHubClient::WareDetailedDescription>& WMap)
{
  WMap.clear();

  if (Obj.IsObject())
  {
    for (rapidjson::Value::ConstMemberIterator it = Obj.MemberBegin();it != Obj.MemberEnd(); ++it)
    {
      if (it->value.IsObject())
      {
        std::string WareID = it->name.GetString();

        WMap[WareID] = openfluid::utils::FluidHubClient::WareDetailedDescription();

        rapidjson::Value::ConstMemberIterator itMember = it->value.FindMember("git-url");
        if (itMember != it->value.MemberEnd() && itMember->value.IsString())
        {
          WMap[WareID].GitUrl = std::string(itMember->value.GetString());
        }

        itMember = it->value.FindMember("shortdesc");
        if (itMember != it->value.MemberEnd() && itMember->value.IsString())
        {
          WMap[WareID].ShortDescription = std::string(itMember->value.GetString());
        }

        itMember = it->value.FindMember("users-ro");
        if (itMember != it->value.MemberEnd() && itMember->value.IsArray())
        {
          JSONArrayToStringSet(itMember->value,WMap[WareID].ROUsers);
        }

        itMember = it->value.FindMember("users-rw");
        if (itMember != it->value.MemberEnd() && itMember->value.IsArray())
        {
          JSONArrayToStringSet(itMember->value,WMap[WareID].RWUsers);
        }
      }
    }
  }
}


// =====================================================================
// =====================================================================


FluidHubClient::FluidHubClient()
{

}


// =====================================================================
// =====================================================================


FluidHubClient::~FluidHubClient()
{

}


// =====================================================================
// =====================================================================


void FluidHubClient::reset()
{
  m_HubURL.clear();
  m_HubAPIVersion.clear();
  m_HubCapabilities.clear();
  m_HubName.clear();
  m_HubStatus.clear();
}


// =====================================================================
// =====================================================================


bool FluidHubClient::isCapable(const std::string& Capacity) const
{
  return (m_HubCapabilities.find(Capacity) != m_HubCapabilities.end());
}


// =====================================================================
// =====================================================================


bool FluidHubClient::connect(const std::string& URL)
{
  std::string Content;

  disconnect();

  if (FileDownloader::downloadToString(URL,Content))
  {
    rapidjson::Document JSONDoc;

    if (JSONDoc.Parse<0>(Content.c_str()).HasParseError())
      return false;

    if (JSONDoc.IsObject())
    {
      for (rapidjson::Value::ConstMemberIterator it = JSONDoc.MemberBegin();it != JSONDoc.MemberEnd(); ++it)
      {
        std::string Key = std::string(it->name.GetString());

        if (Key == "nature" && it->value.IsString() && std::string(it->value.GetString()) == "OpenFLUID FluidHub")
          m_HubURL = URL; // URL is really a FluidHub
        else if (Key == "api-version" && it->value.IsString())
          m_HubAPIVersion = it->value.GetString();
        else if (Key == "name" && it->value.IsString())
          m_HubName = it->value.GetString();
        else if (Key == "status" && it->value.IsString())
          m_HubStatus = it->value.GetString();
        else if (Key == "capabilities" && it->value.IsArray())
          JSONArrayToStringSet(it->value,m_HubCapabilities);
      }
    }
  }

  return isConnected();
}


// =====================================================================
// =====================================================================


void FluidHubClient::disconnect()
{
  reset();
}


// =====================================================================
// =====================================================================


FluidHubClient::WaresDescByType_t FluidHubClient::getAvailableWares() const
{
  WaresDescByType_t WaresDesc;

  WaresDesc[SIMULATOR] = std::set<openfluid::ware::WareID_t>();
  WaresDesc[OBSERVER] = std::set<openfluid::ware::WareID_t>();
  WaresDesc[BUILDEREXT] = std::set<openfluid::ware::WareID_t>();

  if (isConnected() && isCapable("wareshub"))
  {
    std::string URL = m_HubURL;

    if (URL.back() != '/')
      URL += '/';
    URL += "wares";

    std::string Content;

    if (FileDownloader::downloadToString(URL,Content))
    {
      rapidjson::Document JSONDoc;

      if (JSONDoc.Parse<0>(Content.c_str()).HasParseError())
        return WaresDesc;

      if (JSONDoc.IsObject())
      {
        for (rapidjson::Value::ConstMemberIterator it = JSONDoc.MemberBegin();it != JSONDoc.MemberEnd(); ++it)
        {
          std::string Key = std::string(it->name.GetString());

          if (Key == "simulators")
            JSONArrayToStringSet(it->value,WaresDesc[SIMULATOR]);
          else if (Key == "observers")
            JSONArrayToStringSet(it->value,WaresDesc[OBSERVER]);
          else if (Key == "builderexts")
            JSONArrayToStringSet(it->value,WaresDesc[BUILDEREXT]);
        }
      }
    }
  }

  return WaresDesc;
}


// =====================================================================
// =====================================================================


FluidHubClient::WaresDetailedDescByType_t
  FluidHubClient::getAvailableWaresWithDetails(const std::string& Username) const
{
  WaresDetailedDescByType_t WaresDesc;

  WaresDesc[SIMULATOR] = std::map<openfluid::ware::WareID_t,WareDetailedDescription>();
  WaresDesc[OBSERVER] = std::map<openfluid::ware::WareID_t,WareDetailedDescription>();
  WaresDesc[BUILDEREXT] = std::map<openfluid::ware::WareID_t,WareDetailedDescription>();

  if (isConnected() && isCapable("wareshub"))
  {
    std::string URL = m_HubURL;

    if (URL.back() != '/')
      URL += '/';
    URL += "wares?detailed=yes";

    if (!Username.empty())
      URL += "&username="+Username;

    std::string Content;

    if (FileDownloader::downloadToString(URL,Content))
    {
      rapidjson::Document JSONDoc;

      if (JSONDoc.Parse<0>(Content.c_str()).HasParseError())
        return WaresDesc;

      if (JSONDoc.IsObject())
      {
        for (rapidjson::Value::ConstMemberIterator it = JSONDoc.MemberBegin();it != JSONDoc.MemberEnd(); ++it)
        {
          std::string Key = std::string(it->name.GetString());

          if (Key == "simulators")
            JSONObjectToDetailedWares(it->value,WaresDesc[SIMULATOR]);
          else if (Key == "observers")
            JSONObjectToDetailedWares(it->value,WaresDesc[OBSERVER]);
          else if (Key == "builderexts")
            JSONObjectToDetailedWares(it->value,WaresDesc[BUILDEREXT]);
        }
      }
    }
  }

  return WaresDesc;
}


// =====================================================================
// =====================================================================


std::string FluidHubClient::getNews(const std::string& Lang) const
{
  std::string Content;

  if (isConnected() && isCapable("news"))
  {
    std::string URL = m_HubURL;

    if (URL.back() != '/')
      URL += '/';
    URL += "news";

    if (!Lang.empty())
      URL += "?lang="+Lang;

    FileDownloader::downloadToString(URL,Content);
  }

  return Content;
}


} }  // namespaces

