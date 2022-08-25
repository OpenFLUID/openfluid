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
  @file WareIssues.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
 */

#ifndef __OPENFLUID_WARE_WAREISSUES_HPP__
#define __OPENFLUID_WARE_WAREISSUES_HPP__


#include <string>
#include <map>

#include <openfluid/dllexport.hpp>
#include <openfluid/core/DateTime.hpp>


namespace openfluid { namespace ware {


enum class IssueUrgency { LOW, MEDIUM, HIGH };


class OPENFLUID_API WareIssue
{
  public:

    std::string Title;
    
    std::string Description;

    std::vector<std::string> Tags;

    std::string Creator;

    openfluid::core::DateTime CreatedAt;

    openfluid::core::DateTime UpdatedAt;

    bool IsOpen = true;
};


// =====================================================================
// =====================================================================


class OPENFLUID_API WareIssues
{
  private:

    std::map<unsigned int,WareIssue> m_Issues;


  public:
    
    /**
      Adds an issue and automatically assigns an ID
      @param[in] Issue The issue to add
    */
    void add(const WareIssue& Issue);

    /**
      Inserts an issue with the given ID
      @param[in] Issue The issue to insert
      @param[in] ID The ID of the issue to insert
    */
    void insert(const WareIssue& Issue, unsigned int ID);

    /**
      Removes the issue with the given ID (does nothing if the ID does not exist)
      @param[in] ID The ID of the issue to remove
    */
    void remove(int ID);

    /**
      Returns the issue with the given ID
      @param[in] ID The ID of the issue
    */
    const WareIssue& get(int ID) const;

    /**
      Returns all the issues as a map indexed by issues ID
      @return All issues
    */
    const std::map<unsigned int,WareIssue>& getAll() const;

    /**
      Returns all the issues as a map indexed by issues ID
      @return All issues
    */
    const std::map<unsigned int,WareIssue>& operator()() const;

    void clear()
    {
      m_Issues.clear();
    }
};


} } // namespaces


#endif /* __OPENFLUID_WARE_WAREISSUES_HPP__ */
