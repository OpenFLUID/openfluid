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


#endif /* __OPENFLUID_WARESDEVTESTS_SIGNATURECOMMON_HPP__ */
