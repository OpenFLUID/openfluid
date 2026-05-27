/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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
  @file WareSetManager.hpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_WARESDEV_WARESETMANAGER_HPP__
#define __OPENFLUID_WARESDEV_WARESETMANAGER_HPP__


#include <map>

#include <openfluid/dllexport.hpp>
#include <openfluid/thirdparty/JSON.hpp>


namespace openfluid { namespace waresdev {

const std::string OK_STRING = "[OK]";
const std::string KO_STRING = "[KO]";
const std::string MULTIBUILD_STR = "<Common>";

class OPENFLUID_API WareSetManager
{
  private:
    
    std::string m_WareSourceType;
    std::string m_WaresetSourceType;
    openfluid::thirdparty::json m_JSONWareset;
    std::string m_ID;
    std::string m_WaresOrigin;
    
    std::map<std::string, std::map<std::string, std::string>> m_WareStatus;
    unsigned int m_Problems;


  public:

    // constructor also deduce expected wares from set and complementary information from eventual datasetinfo.json
    WareSetManager(const std::string& WareSourceType, const std::string& WaresetSourceType, 
                   const std::string& SetOption, const std::string& WaresOrigin, std::string& ID);

    void setStatus(const std::string& Line, const std::string& Step, const std::string& Status)
    {
      m_WareStatus[Line][Step] = Status;
    }
    
    // writes a table in cout with status for each ware and each step
    void displayStatus();

    // runs the fetch, checkout, configure, build, install for all wares of the set and may run the simulation
    int scaffoldWareset(const std::string& ParentPathStr, 
                        const std::string& WareSourceURL, 
                        bool IsStrict, bool NoBuild, unsigned int JobsNbr, bool BuildTogether);

    // generates a lock file with precise hash version of git repo for each ware
    void freeze(const std::string& ParentPathStr);
};


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESETMANAGER_HPP__ */
