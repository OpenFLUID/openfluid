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
  @file SignatureCommon.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
 */


#ifndef __OPENFLUID_WARESDEVTESTS_SIGNATURECOMMON_HPP__
#define __OPENFLUID_WARESDEVTESTS_SIGNATURECOMMON_HPP__


void compareIssues(const openfluid::ware::WareIssue S1, openfluid::ware::WareIssue S2)
{
  BOOST_CHECK_EQUAL(S1.Title,S2.Title);
  BOOST_CHECK_EQUAL(S1.Description,S2.Description);
  BOOST_CHECK_EQUAL_COLLECTIONS(S1.Tags.begin(),S1.Tags.end(),S2.Tags.begin(),S2.Tags.end());
  BOOST_CHECK_EQUAL(S1.Creator,S2.Creator);
  BOOST_CHECK(S1.CreatedAt == S2.CreatedAt);
  BOOST_CHECK_EQUAL(S1.IsOpen,S2.IsOpen);
}


// =====================================================================
// =====================================================================


void compareIssuesJSON(const openfluid::thirdparty::json& Issue1, const openfluid::thirdparty::json& Issue2) 
{
  openfluid::thirdparty::json TagsArr1 = Issue1.value("tags",openfluid::thirdparty::json::array());
  openfluid::thirdparty::json TagsArr2 = Issue2.value("tags",openfluid::thirdparty::json::array());
  BOOST_REQUIRE_EQUAL(TagsArr1.size(), TagsArr2.size());

  BOOST_CHECK_EQUAL(Issue1.value("id", 0), Issue2.value("id", 0));
  BOOST_CHECK_EQUAL(Issue1.value("title", ""), Issue2.value("title", ""));
  BOOST_CHECK_EQUAL(Issue1.value("description", ""), Issue2.value("description", ""));
  for (unsigned int i=0; i < TagsArr1.size(); i++)
  {
    BOOST_CHECK_EQUAL(TagsArr1[i], TagsArr2[i]);
  }
  BOOST_CHECK_EQUAL(Issue1.value("creator", ""), Issue2.value("creator", ""));
  BOOST_CHECK_EQUAL(Issue1.value("created_at", ""), Issue2.value("created_at", ""));
  BOOST_CHECK_EQUAL(Issue1.value("updated_at", ""), Issue2.value("updated_at", ""));
  BOOST_CHECK_EQUAL(Issue1.value("state", ""), Issue2.value("state", ""));
  
}


// =====================================================================
// =====================================================================


void checkArraysJSON(const openfluid::thirdparty::json& Json1, const openfluid::thirdparty::json& Json2)
{
  BOOST_CHECK(Json1.is_array());
  BOOST_CHECK(Json2.is_array());
  BOOST_REQUIRE_EQUAL(Json1.size(), Json2.size());
}


// =====================================================================
// =====================================================================


template<typename SignatureType>
void compareSignaturesBase(const SignatureType& Sign1,const SignatureType& Sign2)
{
  BOOST_CHECK_EQUAL(Sign1.ID,Sign2.ID);
  BOOST_CHECK_EQUAL(Sign1.Name,Sign2.Name);
  BOOST_CHECK_EQUAL(Sign1.Description,Sign2.Description);
  BOOST_CHECK_EQUAL(Sign1.Status,Sign2.Status);
  BOOST_CHECK_EQUAL(Sign1.Version,Sign2.Version);
  
  BOOST_REQUIRE_EQUAL(Sign1.Authors.size(),Sign2.Authors.size());
  for (unsigned int i=0; i < Sign1.Authors.size(); i++)
  {
    BOOST_CHECK_EQUAL(Sign1.Authors[i].first,Sign2.Authors[i].first);
    BOOST_CHECK_EQUAL(Sign1.Authors[i].second,Sign2.Authors[i].second);
  }
  
  BOOST_REQUIRE_EQUAL(Sign1.Contacts.size(),Sign2.Contacts.size());
  for (unsigned int i=0; i < Sign1.Contacts.size(); i++)
  {
    BOOST_CHECK_EQUAL(Sign1.Contacts[i].first,Sign2.Contacts[i].first);
    BOOST_CHECK_EQUAL(Sign1.Contacts[i].second,Sign2.Contacts[i].second);
  }
  
  BOOST_CHECK_EQUAL(Sign1.License,Sign2.License);

  BOOST_CHECK_EQUAL(Sign1.Tags.size(),Sign2.Tags.size());
  BOOST_CHECK_EQUAL_COLLECTIONS(Sign1.Tags.begin(),Sign1.Tags.end(),Sign2.Tags.begin(),Sign2.Tags.end());

  BOOST_CHECK_EQUAL(Sign1.Links.size(),Sign2.Links.size());

  BOOST_REQUIRE_EQUAL(Sign1.Issues().size(),Sign2.Issues().size()); 
  for (const auto& S1 : Sign1.Issues())
  {
    BOOST_REQUIRE_NO_THROW(Sign2.Issues.get(S1.first));
    auto S2 = Sign2.Issues().at(S1.first);

    compareIssues(S1.second, S2);
  }
}


// =====================================================================
// =====================================================================


void compareJSONBase(const openfluid::thirdparty::json& Json1, const openfluid::thirdparty::json& Json2) 
{
  BOOST_CHECK_EQUAL(Json1.value("id", ""), Json2.value("id", ""));
  BOOST_CHECK_EQUAL(Json1.value("name", ""), Json2.value("name", ""));
  BOOST_CHECK_EQUAL(Json1.value("description", ""), Json2.value("description", ""));
  BOOST_CHECK_EQUAL(Json1.value("status", ""), Json2.value("status", ""));
  BOOST_CHECK_EQUAL(Json1.value("version", ""), Json2.value("version", ""));

  openfluid::thirdparty::json AuthorsArr1 = Json1.value("authors",openfluid::thirdparty::json::array());
  openfluid::thirdparty::json AuthorsArr2 = Json2.value("authors",openfluid::thirdparty::json::array());
  checkArraysJSON(AuthorsArr1, AuthorsArr2);
  for (unsigned int i=0; i < AuthorsArr1.size(); i++)
  {
    BOOST_CHECK_EQUAL(AuthorsArr1[i].value("name", ""), AuthorsArr1[i].value("name", ""));
    BOOST_CHECK_EQUAL(AuthorsArr1[i].value("email", ""), AuthorsArr1[i].value("email", ""));
  }
  
  openfluid::thirdparty::json ContactsArr1 = Json1.value("contacts",openfluid::thirdparty::json::array());
  openfluid::thirdparty::json ContactsArr2 = Json2.value("contacts",openfluid::thirdparty::json::array());
  checkArraysJSON(ContactsArr1, ContactsArr2);
  for (unsigned int i=0; i < ContactsArr1.size(); i++)
  {
    BOOST_CHECK_EQUAL(ContactsArr1[i].value("name", ""), ContactsArr2[i].value("name", ""));
    BOOST_CHECK_EQUAL(ContactsArr1[i].value("email", ""), ContactsArr2[i].value("email", ""));
  }

  BOOST_CHECK_EQUAL(Json1.value("license", ""), Json2.value("license", ""));

  openfluid::thirdparty::json TagsArr1 = Json1.value("tags",openfluid::thirdparty::json::array());
  openfluid::thirdparty::json TagsArr2 = Json2.value("tags",openfluid::thirdparty::json::array());
  checkArraysJSON(TagsArr1, TagsArr2);
  for (unsigned int i=0; i < TagsArr1.size(); i++)
  {
    BOOST_CHECK_EQUAL(TagsArr1[i], TagsArr2[i]);
  }

  openfluid::thirdparty::json LinksArr1 = Json1.value("links",openfluid::thirdparty::json::array());
  openfluid::thirdparty::json LinksArr2 = Json2.value("links",openfluid::thirdparty::json::array());
  checkArraysJSON(LinksArr1, LinksArr2);
  for (unsigned int i=0; i < LinksArr1.size(); i++)
  {
    BOOST_CHECK_EQUAL(LinksArr1[i].value("label", ""), LinksArr2[i].value("label", ""));
    BOOST_CHECK_EQUAL(LinksArr1[i].value("url", ""), LinksArr2[i].value("url", ""));
  }

  openfluid::thirdparty::json IssuesArr1 = Json1.value("issues", openfluid::thirdparty::json::array());
  openfluid::thirdparty::json IssuesArr2 = Json2.value("issues", openfluid::thirdparty::json::array());
  checkArraysJSON(IssuesArr1, IssuesArr2);
  for (unsigned int i=0; i < IssuesArr1.size(); i++)
  {
    compareIssuesJSON(IssuesArr1[i], IssuesArr2[i]);
  }
}


// =====================================================================
// =====================================================================


void sortJson(openfluid::thirdparty::json& Json) 
{
  if(Json.is_object())
  {
    std::map<std::string, openfluid::thirdparty::json> orderedMapJSON;
    for (auto& [key, value] : Json.items()) 
    {
      orderedMapJSON[key] = value;
    }
    for (auto& [key, value] : orderedMapJSON) 
    {
      sortJson(value);
    }
    Json = openfluid::thirdparty::json(orderedMapJSON);
  }
}


// =====================================================================
// =====================================================================


bool compareJSONRecursive(openfluid::thirdparty::json& Json1,
                          openfluid::thirdparty::json& Json2,
                          bool WithCheck = true)
{
  bool IsEqual = true;

  sortJson(Json1);
  sortJson(Json2);

  unsigned int Json1Size = Json1.size();
  unsigned int Json2Size = Json2.size();
  if(WithCheck) 
  {
    BOOST_CHECK_EQUAL(Json1Size, Json2Size);
  }
  IsEqual = IsEqual && Json1Size == Json2Size;

  if(Json1.is_object() || Json2.is_object())
  {
    auto It2 = Json2.cbegin();
    for (auto It1 = Json1.cbegin(); It1 != Json1.cend(); ++It1, ++It2)
    {
      if(WithCheck) 
      {
        BOOST_CHECK_EQUAL(It1.key(), It2.key());
      }
      IsEqual = IsEqual && It1.key() == It2.key();
      auto It1Value = It1.value();
      auto It2Value = It2.value();
      if(It1Value.is_primitive() || It2Value.is_primitive()) 
      {
        if(WithCheck) 
        {
          BOOST_CHECK_EQUAL(It1Value, It2Value);
        }
        IsEqual = IsEqual && It1Value == It2Value;
      }
      else 
      {
        IsEqual = IsEqual && compareJSONRecursive(It1Value, It2Value, WithCheck);
      }
    }
  }
  else if(Json1.is_array() || Json2.is_array())
  {
    auto It2 = Json2.cbegin();
    for (auto It1 = Json1.cbegin(); It1 != Json1.cend(); ++It1, ++It2) 
    {
      auto Child1 = *It1;
      auto Child2 = *It2;
      if(Child1.is_primitive() || Child2.is_primitive()) 
      {
        if(WithCheck) 
        {       
          BOOST_CHECK_EQUAL(Child1, Child2);
        }
        IsEqual = IsEqual && Child1 == Child2;
      }
      else 
      {
        IsEqual = IsEqual && compareJSONRecursive(Child1, Child2, WithCheck);
      }
    }
  }
  else 
  {
    if(WithCheck) 
    {       
      BOOST_CHECK_EQUAL(Json1, Json2);
    }
    IsEqual = IsEqual && Json1 == Json2;
  }

  return IsEqual;
}

#endif /* __OPENFLUID_WARESDEVTESTS_SIGNATURECOMMON_HPP__ */
