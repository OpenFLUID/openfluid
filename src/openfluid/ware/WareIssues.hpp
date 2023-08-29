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
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */

#ifndef __OPENFLUID_WARE_WAREISSUES_HPP__
#define __OPENFLUID_WARE_WAREISSUES_HPP__


#include <string>
#include <map>

#include <openfluid/dllexport.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/thirdparty/JSON.hpp>
#include <openfluid/tools/StringHelpers.hpp>


namespace openfluid { namespace ware {


enum class IssueUrgency { LOW, MEDIUM, HIGH };


class OPENFLUID_API WareIssue
{
  public:

    unsigned int ID;

    std::string Title;
    
    std::string Description;

    std::vector<std::string> Tags;

    std::string Creator;

    openfluid::core::DateTime CreatedAt;

    openfluid::core::DateTime UpdatedAt;

    bool IsOpen = true;


    openfluid::thirdparty::json toJSON(unsigned int NewID=0) const
    {
      if (NewID == 0)
      {
        NewID = ID;
      }
      auto IObj = openfluid::thirdparty::json::object();
      IObj["id"] = NewID;
      IObj["title"] = Title;
      IObj["description"] = Description;
      IObj["tags"] = Tags;
      IObj["creator"] = Creator;
      IObj["created_at"] = CreatedAt.getAsISOString();
      IObj["updated_at"] = UpdatedAt.getAsISOString();
      IObj["state"] = IsOpen ? "open" : "closed";
      return IObj;
    }

    static WareIssue fromJSON(const openfluid::thirdparty::json& IssueJson)
    {
      WareIssue NewI;
      if (!IssueJson.contains("id"))
      {
        throw openfluid::base::FrameworkException("Info not containing id");
      }
      if (IssueJson["id"].is_number_integer())
      {
        NewI.ID = IssueJson["id"].get<unsigned int>();
      }
      else
      {
        throw openfluid::base::FrameworkException("Bad id format for ware issue");
      }
      NewI.Title = IssueJson.value("title","");
      NewI.Description = IssueJson.value("description","");
      if (IssueJson.contains("tags"))
      {
        NewI.Tags = IssueJson.value("tags",std::vector<std::string>());
      }
      
      NewI.Creator = IssueJson.value("creator","");
      NewI.CreatedAt = openfluid::core::DateTime::fromISOString(IssueJson.value("created_at",""));
      NewI.UpdatedAt = openfluid::core::DateTime::fromISOString(IssueJson.value("updated_at",""));
      NewI.IsOpen = (IssueJson.value("state","") != "closed");
      return NewI;
    }
    //NOTE: issue comparison is implemented in waresdev/tests, move here as class method when needed
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
    void remove(unsigned int ID);

    /**
      Returns the issue with the given ID
      @param[in] ID The ID of the issue
    */
    const WareIssue& get(unsigned int ID) const;

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
