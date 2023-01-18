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
  @file WareSrcEnquirer.hpp
 
  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/


#ifndef __OPENFLUID_WARESDEV_WARESRCENQUIRER_HPP__
#define __OPENFLUID_WARESDEV_WARESRCENQUIRER_HPP__


#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace waresdev {


class OPENFLUID_API WareSrcEnquirer
{

  public:

    struct WarePathInfo
    {
      std::string AbsolutePath;

      std::string AbsoluteWarePath;

      std::string FileName;

      std::string RelativePathToWare;

      bool IsInCurrentWorkspace = false;
      
      bool IsInExamples = false;

      openfluid::ware::WareType WareType = openfluid::ware::WareType::UNDEFINED;

      std::string WareDirName;

      bool IsWareDirectory = false;

      bool IsWareFile = false;

      bool isValid()
      {
        return (IsWareDirectory || IsWareFile) && 
               (IsInCurrentWorkspace || IsInExamples) &&
               (WareType != openfluid::ware::WareType::UNDEFINED);
      }
    };


  public:

    /**
      Try to detect full ware sources path using its type and its supposed directory name.
      It searches first in the current workspace then in the user examples
      @param[in] WareType the type of the ware to find
      @param[in] WareDirName the expected directory name for the ware
      @return the full path of the found ware sources, empty if not found
    */
    static std::string findWarePath(openfluid::ware::WareType WareType, const std::string& WareDirName);

    /**
      Returns information about the wares sources located at the given path.
      @param[in] Path the path to the ware to inspect
      @return the information about the ware
    */
    static WarePathInfo getWareInfoFromPath(const std::string& Path);

};


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESRCENQUIRER_HPP__ */
