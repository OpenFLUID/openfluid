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
  @file WorkspaceDevDashboardWorker.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QDir>

#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevDashboardWorker.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WorkspaceDevDashboardWorker::WorkspaceDevDashboardWorker()
{

}


// =====================================================================
// =====================================================================


WorkspaceDevDashboardWorker::~WorkspaceDevDashboardWorker()
{

}


// =====================================================================
// =====================================================================


void WorkspaceDevDashboardWorker::run()
{

  auto Manager = openfluid::base::WorkspaceManager::instance();


  std::vector<std::pair<openfluid::ware::WareType,QString>> RootPathsByType =
  {
    {
      openfluid::ware::WareType::SIMULATOR,
      QString::fromStdString(Manager->getWaresPath(openfluid::ware::WareType::SIMULATOR))
    },
    {
      openfluid::ware::WareType::OBSERVER,
      QString::fromStdString(Manager->getWaresPath(openfluid::ware::WareType::OBSERVER))
    },
    {
      openfluid::ware::WareType::BUILDEREXT,
      QString::fromStdString(Manager->getWaresPath(openfluid::ware::WareType::BUILDEREXT))
    }
  };


  openfluid::utils::GitProxy Git;

  for (auto& RootPath : RootPathsByType)
  {
    QDir CurrentDir(RootPath.second);

    for (QFileInfo FileInfo : CurrentDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs))
    {
      WorkspaceDevDashboardTypes::WareGitInfos Infos;

      Infos.Type = RootPath.first;
      Infos.ID = FileInfo.fileName();
      Infos.Path = CurrentDir.absoluteFilePath(FileInfo.fileName());
      openfluid::utils::GitProxy::TreeStatusInfo TreeStatus = Git.status(Infos.Path);

      if (TreeStatus.m_IsGitTracked)
      {
        Infos.IsTracked = true;
        Infos.BranchName = TreeStatus.m_BranchName;

        for (auto& Status : TreeStatus.m_FileStatusByTreePath.values())
        {
          Infos.IndexCounters[Status.m_IndexStatus]++;// = Infos.Counters[Status.m_IndexStatus]+1;
          Infos.DirtyCounter += Status.m_IsDirty;
        }
      }

      emit wareParsed(Infos);
    }
  }


  emit finished();
}


} } }  // namespaces
