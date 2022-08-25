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
 */


#include <openfluid/ware/WareIssues.hpp>


namespace openfluid { namespace ware {


void WareIssues::add(const WareIssue& Issue)
{
  unsigned int ID = 0;

  for (const auto& Iss : m_Issues)
  {
    if (Iss.first > ID)
    {
      ID = Iss.first;
    }
  }

  ID++;

  m_Issues[ID] = Issue;
}


// =====================================================================
// =====================================================================


void WareIssues::insert(const WareIssue& Issue, unsigned int ID)
{
  m_Issues[ID] = Issue;
}


// =====================================================================
// =====================================================================


void WareIssues::remove(int /*ID*/)
{
  throw openfluid::base::FrameworkException("not implemented"); // TOIMPL
}


// =====================================================================
// =====================================================================


const WareIssue& WareIssues::get(int /*ID*/) const
{
  throw openfluid::base::FrameworkException("not implemented"); // TOIMPL
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
