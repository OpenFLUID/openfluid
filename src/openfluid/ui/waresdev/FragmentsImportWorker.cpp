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
  @file FragmentsImportWorker.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <iostream>

#include <openfluid/config.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/ui/waresdev/FragmentsImportWorker.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/utilsq/GitProxy.hpp>
#include <openfluid/tools/FilesystemPath.hpp>


namespace openfluid { namespace ui { namespace waresdev {


FragmentsImportWorker::FragmentsImportWorker(bool SslNoVerify) : GitImportWorker(SslNoVerify)
{

}


// =====================================================================
// =====================================================================


FragmentsImportWorker::~FragmentsImportWorker()
{

}


// =====================================================================
// =====================================================================


void FragmentsImportWorker::setSubmoduleParameter(bool FragmentsAsSubmodule)
{
  m_AsSubmodule = FragmentsAsSubmodule;
}


// =====================================================================
// =====================================================================


bool FragmentsImportWorker::importElement(const QString& GitUrl, const QString& RootPath)
{
  if (m_Username.isEmpty() || m_Password.isEmpty() || RootPath.isEmpty())
  {
    emit error(tr("Missing mandatory information."));
    return false;
  }
  if (GitUrl.isEmpty())
  {
    emit error(tr("Fragment URL is empty"));
    return false;
  }
  // Checking if git URL is valid
  QString ProcessedGitUrl = GitUrl;
  if (ProcessedGitUrl[ProcessedGitUrl.length()-1] == "/")
  {
    ProcessedGitUrl.chop(1);
  }

  QFileInfo GitRepoInfo = QFileInfo(ProcessedGitUrl);
  QString RepoName = GitRepoInfo.fileName();
  if (RepoName.endsWith(".git"))  // WARNING: would cut the name of a ware of the form "a.b.git"
  {
    RepoName = GitRepoInfo.completeBaseName();
  }

  if (RepoName.isEmpty())
  {
    emit error(tr("Unable to detect the ware name from the given URL: ")+GitUrl);
    return false;
  }

  openfluid::tools::FilesystemPath SrcFragmentsSubPath({openfluid::config::WARESDEV_SRC_DIR, 
                                                        openfluid::config::WARESDEV_FRAGMENTS_DIR});
  openfluid::tools::FilesystemPath FragmentsPath({RootPath.toStdString(), SrcFragmentsSubPath.toNative()});

  bool FragmentDirCreated = false;
  if (!FragmentsPath.isDirectory())
  {
    FragmentDirCreated = FragmentsPath.makeDirectory();
  }

  // FETCHING REPO
  openfluid::tools::FilesystemPath DestSubPath({SrcFragmentsSubPath.toGeneric(), RepoName.toStdString()});
    
  openfluid::utils::GitProxy Git;
  QObject::connect(&Git, SIGNAL(info(const QString&)), this, SIGNAL(info(const QString&)));
  QObject::connect(&Git, SIGNAL(error(const QString&)), this, SIGNAL(error(const QString&)));

  bool Success;
  if (m_AsSubmodule)
  {
    Success = Git.addSubmodule(GitUrl, QString::fromStdString(DestSubPath.toNative()), 
                                                              RootPath, 
                                                              m_Username, 
                                                              m_Password, 
                                                              m_SslNoVerify);
    if (!Success)
    {
      openfluid::tools::FilesystemPath TargetDirectory = openfluid::tools::FilesystemPath(
        {RootPath.toStdString(), DestSubPath.toGeneric()});

      // removing also the reference in .git folder
      openfluid::tools::FilesystemPath GitSubmoduleTargetDirectory = openfluid::tools::FilesystemPath(
        {RootPath.toStdString(), ".git", "modules", DestSubPath.toGeneric()});
      if (GitSubmoduleTargetDirectory.isDirectory())
      {
        GitSubmoduleTargetDirectory.removeDirectory();
        emit info(tr("Fragment directory removed from .git/modules since import not successful"));
      }
      else
      {
        if (FragmentDirCreated)
        {
          TargetDirectory = openfluid::tools::FilesystemPath({RootPath.toStdString(), SrcFragmentsSubPath.toGeneric()});
        }
        if (TargetDirectory.removeDirectory())
        {
          emit info(tr("Fragment directory removed since import not successful"));
        }
        else
        {
          emit error(tr("Unable to remove the fragments directory after unsuccessful submodule addition."));
        }
      }
    }
  }
  else
  {
    Success = Git.clone(GitUrl, QString::fromStdString(DestSubPath.toNative()), 
                                                               m_Username, 
                                                               m_Password, 
                                                               m_SslNoVerify, 
                                                               RootPath, 
                                                               true);
    if (Success)
    {
      openfluid::tools::FilesystemPath FragmentGitPath = 
        openfluid::tools::FilesystemPath({RootPath.toStdString(), DestSubPath.toGeneric(), ".git"});
      if (FragmentGitPath.exists())
      {
        // removing .git/ folder to avoid potential git conflict with parent folder
        if (!FragmentGitPath.removeDirectory())
        {
          emit error(tr("Unable to remove the git repository of the fragment."));
        }
      }
    }
  }
  return Success;
}


} } }  // namespaces
