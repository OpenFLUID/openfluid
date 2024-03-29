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
  @file WorkspaceDevDashboardTypes.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_WORKSPACEDEVDASHBOARDTYPES_HPP__
#define __OPENFLUID_UIWARESDEV_WORKSPACEDEVDASHBOARDTYPES_HPP__


#include <list>
#include <map>
#include <vector>

#include <QString>

#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/ui/waresdev/GitUIProxy.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class WorkspaceDevDashboardTypes
{
  public:

    class WareInfos
    {
      public:

        QString ID = "";

        QString Path = "";
    };

    class WareGitInfos : public WareInfos
    {
      public:

        bool IsTracked = false;

        openfluid::ware::WareType Type = openfluid::ware::WareType::UNDEFINED;

        QString BranchName = "";

        bool IsDirty = false;

        int DirtyCounter = 0;

        std::map<GitUIProxy::FileStatus,int> IndexCounters =
        {
         {GitUIProxy::FileStatus::UNTRACKED,0},
         {GitUIProxy::FileStatus::TRACKED,0},
         {GitUIProxy::FileStatus::IGNORED,0},
         {GitUIProxy::FileStatus::CONFLICT,0},
         {GitUIProxy::FileStatus::ADDED,0},
         {GitUIProxy::FileStatus::DELETED,0},
         {GitUIProxy::FileStatus::MODIFIED,0}
        };

    };

    class WareProcessInfos : public WareInfos
    { };

    typedef std::list<WareProcessInfos> WaresSelection;

    typedef std::map<openfluid::ware::WareType,WaresSelection> WaresSelectionByType;

    typedef std::vector<std::vector<std::pair<QString,QString>>> ActionsByRows;

};


} } }  // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WORKSPACEDEVDASHBOARDTYPES_HPP__ */
