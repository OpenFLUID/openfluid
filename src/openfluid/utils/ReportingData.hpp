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
  @file ReportingData.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UTILS_REPORTINGDATA_HPP__
#define __OPENFLUID_UTILS_REPORTINGDATA_HPP__


#include <list>
#include <map>


namespace openfluid { namespace utils {


/**
  A template structure to store reporting data.
  A report is madde of categories. Each category is a list of report items.
  Each report item is made of a message and a status.
  @tparam C the data type for categories
  @tparam M the data type for messages
*/
template<typename C,typename M>
class ReportingData
{
  public:

    enum class ReportingStatus
    { 
      UNKNOWN = 0, DISABLED, OK, WARNING, ERROR_STATUS 
    };


    struct ReportingItem
    {
      M Message;

      ReportingStatus Status;
    };


    struct ReportingList
    {
      /**
        Items contained in the list
      */
      std::list<ReportingItem> Items;

      /**
        Computes the overall status of the list
      */
      ReportingStatus getStatus() const
      {
        auto S = ReportingStatus::OK;
        for (const auto& I : Items)
        {
          if (I.Status > S)
          {
            S = I.Status; 
          }
        }
        return S;
      }
    };


    // =====================================================================
    // ===================================================================== 


    /**
      Categories composing the report
    */
    std::map<C,ReportingList> Categories;

    /**
      Computes the overall status of the report
    */
    ReportingStatus getStatus() const
    {
      auto S = ReportingStatus::OK;
      for (const auto& Cat : Categories)
      {
        auto CatS = Cat.second.getStatus();
        if (CatS > S)
        {
          S = CatS; 
        }
      }
      return S;
    }
};


} }  //namespaces


#endif /* __OPENFLUID_UTILS_REPORTINGDATA_HPP__ */
