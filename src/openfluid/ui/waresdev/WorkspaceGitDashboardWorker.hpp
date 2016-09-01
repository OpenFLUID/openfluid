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
  @file WorkspaceGitDashboardWorker.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_WORKSPACEGITDASHBOARDWORKER_HPP__
#define __OPENFLUID_UIWARESDEV_WORKSPACEGITDASHBOARDWORKER_HPP__


#include <QObject>
#include <QMetaType>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/utils/GitProxy.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API WorkspaceGitDashboardWorker : public QObject
{

  Q_OBJECT;


  public:

    struct WareStatusInfo
    {
      openfluid::ware::WareType Type = openfluid::ware::WareType::UNDEFINED;

      QString ID = "";

      QString BranchName = "";

      bool IsDirty = false;

      int DirtyCounter = 0;

      std::map<openfluid::utils::GitProxy::FileStatus,int> IndexCounters =
      {
        {openfluid::utils::GitProxy::FileStatus::UNTRACKED,0},
        {openfluid::utils::GitProxy::FileStatus::TRACKED,0},
        {openfluid::utils::GitProxy::FileStatus::IGNORED,0},
        {openfluid::utils::GitProxy::FileStatus::CONFLICT,0},
        {openfluid::utils::GitProxy::FileStatus::ADDED,0},
        {openfluid::utils::GitProxy::FileStatus::DELETED,0},
        {openfluid::utils::GitProxy::FileStatus::MODIFIED,0}
      };

    };


  signals:

    void wareParsed(openfluid::ui::waresdev::WorkspaceGitDashboardWorker::WareStatusInfo);

    void finished();


  public slots:

    void run();


  public:

    WorkspaceGitDashboardWorker();

    ~WorkspaceGitDashboardWorker();

};


} } }  // namespaces


Q_DECLARE_METATYPE(openfluid::ui::waresdev::WorkspaceGitDashboardWorker::WareStatusInfo);


#endif /* __OPENFLUID_UIWARESDEV_WORKSPACEGITDASHBOARDWORKER_HPP__ */
