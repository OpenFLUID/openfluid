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
  @file ExceptionContext.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_BASE_EXCEPTIONCONTEXT_HPP__
#define __OPENFLUID_BASE_EXCEPTIONCONTEXT_HPP__


#include <string>
#include <map>
#include <sstream>
#include <cstdint>


namespace openfluid { namespace base {


class ExceptionContext : public std::map<std::string, std::string>
{
  public:


    ExceptionContext& addCodeLocation(const std::string& CodeLocStr)
    {
      insert(std::pair<std::string,std::string>("codeloc",CodeLocStr));

      return (*this);
    }


    // =====================================================================
    // =====================================================================


    ExceptionContext& addInfos(const std::map<std::string, std::string>& Infos)
    {
      for (auto& it : Infos)
      {
        insert(std::pair<std::string,std::string>(it.first,it.second));
      }

      return (*this);
    }


    // =====================================================================
    // =====================================================================


    ExceptionContext& addTimeIndex(const std::uint64_t& TimeIndex)
    {
      std::ostringstream iss;
      iss << TimeIndex;
      insert(std::pair<std::string,std::string>("timeindex",iss.str()));

      return (*this);
    }


    // =====================================================================
    // =====================================================================


    ExceptionContext& addSpatialUnit(const std::string& SpatialUnitStr)
    {
      insert(std::pair<std::string,std::string>("spatialunit",SpatialUnitStr));

      return (*this);
    }


    // =====================================================================
    // =====================================================================


    ExceptionContext& addSpatialConnection(const std::string& SpatialUnitStr1,const std::string& SpatialUnitStr2)
    {
      insert(std::pair<std::string,std::string>("spatialconnection",SpatialUnitStr1+"->"+SpatialUnitStr2));

      return (*this);
    }


    // =====================================================================
    // =====================================================================


    ExceptionContext& addStage(const std::string& StageStr)
    {
      if (!StageStr.empty())
      {
        insert(std::pair<std::string,std::string>("stage",StageStr));
      }

      return (*this);
    }


    // =====================================================================
    // =====================================================================


    std::string toString() const
    {
      std::string Str;

      auto itb = begin();
      auto ite = end();

      for (auto it = itb; it!= ite; ++it)
      {
        if (it!=itb)
        {
          Str += ",";
        }

        Str += (*it).first + "=" + (*it).second;
      }

      return Str;
    }

};


} } // namespace


#endif  /* __OPENFLUID_BASE_EXCEPTIONCONTEXT_HPP__ */
