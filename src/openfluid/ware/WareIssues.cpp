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
  @file WareIssues.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <openfluid/ware/WareIssues.hpp>


namespace openfluid { namespace ware {


// =====================================================================
// =====================================================================


void WareIssues::add(const WareIssue& Issue)
{
  if (m_Issues.find(Issue.ID) == m_Issues.end()) 
  {
    m_Issues[Issue.ID] = Issue;
  }
  else
  {
    throw openfluid::base::FrameworkException("Issue ID already in list");
  }
}


// =====================================================================
// =====================================================================


void WareIssues::insert(const WareIssue& Issue, unsigned int ID)
{
  m_Issues[ID] = Issue;
}


// =====================================================================
// =====================================================================


void WareIssues::remove(unsigned int ID)
{
  if (m_Issues.find(ID) == m_Issues.end()) 
  {
    throw openfluid::base::FrameworkException("ID not in Ware Issues");
  }
  else
  {
    m_Issues.erase(ID);
  }
}


// =====================================================================
// =====================================================================


const WareIssue& WareIssues::get(unsigned int ID) const
{
  const auto& IssueIt = m_Issues.find(ID);
  if (IssueIt == m_Issues.end())
  {
    throw openfluid::base::FrameworkException("ID not in Ware Issues");
  }
  else
  {
    return IssueIt->second;
  }
}


// =====================================================================
// =====================================================================


const std::map<unsigned int,WareIssue>& WareIssues::getAll() const
{
  return m_Issues;
}


// =====================================================================
// =====================================================================


const std::map<unsigned int,WareIssue>& WareIssues::operator()() const
{
  return m_Issues;
}


} } // namespaces
