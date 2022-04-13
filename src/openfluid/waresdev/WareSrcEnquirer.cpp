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
 @file WareSrcEnquirer.cpp
   @brief Implements ...

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
 */


#include <openfluid/base/Environment.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/waresdev/WareSrcEnquirer.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace waresdev {


std::string WareSrcEnquirer::findWarePath(openfluid::ware::WareType WareType, const std::string& WareDirName)
{
  std::string Path;

  // find in current workspace  
  std::string InWorkspacePath = openfluid::base::WorkspaceManager::instance()->getWarePath(WareType,WareDirName);
  if (openfluid::tools::Path(InWorkspacePath).isDirectory())
  {
    Path = InWorkspacePath;
  }
  // find in user examples
  else
  {
    std::string InExamplesPath = 
      openfluid::base::WorkspaceManager::getWarePath(openfluid::base::Environment::getUserExamplesDir(),
                                                     WareType,WareDirName);
    if (openfluid::tools::Path(InExamplesPath).isDirectory())
    {
      Path = InExamplesPath;
    }
  }

  return Path;
}


// =====================================================================
// =====================================================================


WareSrcEnquirer::WarePathInfo WareSrcEnquirer::getWareInfoFromPath(const std::string& Path)
{
  auto fillInfo = [](const std::string& BasePath, WarePathInfo& Info)
  {
    openfluid::tools::Path AbsolutePathObject(Info.AbsolutePath);

    for (const auto& WType : { openfluid::ware::WareType::SIMULATOR,
                               openfluid::ware::WareType::OBSERVER,
                               openfluid::ware::WareType::BUILDEREXT})
    {
      const auto WareTypePath = openfluid::base::WorkspaceManager::getWaresPath(BasePath,WType);

      // current path is the type directory
      if (WareTypePath == Info.AbsolutePath)
      {
        return;
      }
      // current path is a ware directory
      else if (AbsolutePathObject.dirname() ==  WareTypePath && AbsolutePathObject.isDirectory())
      {
        Info.AbsoluteWarePath = Info.AbsolutePath;
        Info.WareType = WType;
        Info.WareDirName = openfluid::tools::Path(Info.AbsolutePath).filename();
        Info.IsWareDirectory = true;
        
        return;
      }
      // current path is inside a ware directory
      // TODO to be refactored for a better way to perform this
      else if (openfluid::tools::Path(WareTypePath).contains(
                 openfluid::tools::Path(AbsolutePathObject.dirname()).dirname()))
      {
        const auto RelativePathToType = AbsolutePathObject.relativeTo(WareTypePath);
        const auto RelativePathToTypeParts = openfluid::tools::Path(RelativePathToType).split();

        if (!RelativePathToTypeParts.empty())
        {
          Info.WareType = WType;
          Info.WareDirName = RelativePathToTypeParts[0];  // first part is the ware directory name
          Info.AbsoluteWarePath = openfluid::tools::Filesystem::joinPath({WareTypePath,Info.WareDirName});
          Info.RelativePathToWare = AbsolutePathObject.relativeTo(
                                      openfluid::tools::Filesystem::joinPath({WareTypePath,Info.WareDirName})
                                    );
          Info.IsWareFile = AbsolutePathObject.isFile();

          if (Info.IsWareFile)
          {
            Info.FileName = AbsolutePathObject.filename();
          }
        }
        return;
      }
    }
  };


  WarePathInfo Info;

  auto CleanWorkspacePath = 
    openfluid::tools::FilesystemPath(
      openfluid::base::WorkspaceManager::instance()->getWorkspacePath()
    ).toGenericClean();
  Info.AbsolutePath = 
    openfluid::tools::FilesystemPath(openfluid::tools::Filesystem::absolutePath(Path)).toGenericClean();

  openfluid::tools::Path AbsolutePathObject(Info.AbsolutePath);

  if (AbsolutePathObject.exists())
  {
    // path is not the path of the current workspace
    if (Info.AbsolutePath != CleanWorkspacePath)
    {
      // path is in the current workspace
      if (openfluid::tools::Path(CleanWorkspacePath).contains(Info.AbsolutePath))
      {
        Info.IsInCurrentWorkspace = true;

        fillInfo(CleanWorkspacePath,Info);
      }
      // path is in examples simulators directory
      else if (openfluid::tools::Path(openfluid::base::Environment::getUserExampleSimulatorsDir())
                 .contains(Info.AbsolutePath))
      {
        Info.IsInExamples = true;
        
        fillInfo(openfluid::base::Environment::getUserExamplesDir(),Info);
      }
    }
  }

  return Info;
}


} }  // namespaces
