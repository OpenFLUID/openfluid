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
  @file WareBuildOptions.hpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_WARESDEV_WAREBUILDOPTIONS_HPP__
#define __OPENFLUID_WARESDEV_WAREBUILDOPTIONS_HPP__


#include <openfluid/base/Environment.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/waresdev/WareSrcContainer.hpp>
#include <openfluid/base/PreferencesManager.hpp>


namespace openfluid { namespace waresdev {


const std::string BUILD_ONLY_MODE_STRING = "BUILDONLY";
const std::string BUILD_INSTALL_MODE_STRING = "BUILDINSTALL";
const std::string DEBUG_CONFIG_MODE_STRING = "DEBUG";
const std::string RELEASE_CONFIG_MODE_STRING = "RELEASE";


struct WareBuildOptions
{
  bool IsReleaseMode = true;

  bool IsWithInstall = true;

  bool IsMultipleJobs = true;

  int JobsNumber = openfluid::base::Environment::getIdealJobsCount();

  WareBuildOptions()
  {
    auto* WMgr = openfluid::base::WorkspaceManager::instance();
    WMgr->openWorkspace(openfluid::base::PreferencesManager::instance()->getCurrentWorkspacePath());

    std::string ConfigMode = WMgr->getWaresConfigureMode();
    std::string BuildMode = WMgr->getWaresBuildMode();
    

    IsReleaseMode = ConfigMode == RELEASE_CONFIG_MODE_STRING;
    IsWithInstall = BuildMode == BUILD_INSTALL_MODE_STRING;
    IsMultipleJobs = WMgr->isWaresParallelJobsEnabled();
    JobsNumber = WMgr->getWaresParallelJobsCount();
  }

  openfluid::waresdev::WareSrcContainer::ConfigMode getConfigMode()
  {
    return IsReleaseMode ? openfluid::waresdev::WareSrcContainer::ConfigMode::CONFIG_RELEASE :
                             openfluid::waresdev::WareSrcContainer::ConfigMode::CONFIG_DEBUG;
  }


  openfluid::waresdev::WareSrcContainer::BuildMode getBuildMode()
  {
    return IsWithInstall ? openfluid::waresdev::WareSrcContainer::BuildMode::BUILD_WITHINSTALL :
                             openfluid::waresdev::WareSrcContainer::BuildMode::BUILD_NOINSTALL;
  }


  void writeOptionsInWorkspace()
  {
    auto* WMgr = openfluid::base::WorkspaceManager::instance();
    WMgr->openWorkspace(openfluid::base::PreferencesManager::instance()->getCurrentWorkspacePath());

    std::string ConfigMode = (IsReleaseMode ? RELEASE_CONFIG_MODE_STRING : DEBUG_CONFIG_MODE_STRING);
    std::string BuildMode = (IsWithInstall ? BUILD_INSTALL_MODE_STRING : BUILD_ONLY_MODE_STRING);
    WMgr->setWaresConfigureMode(ConfigMode);
    WMgr->setWaresBuildMode(BuildMode);
    WMgr->setWaresParallelJobsEnabled(IsMultipleJobs);
    WMgr->setWaresParallelJobsCount(JobsNumber);
  }
};

} }  // namespaces


Q_DECLARE_METATYPE(openfluid::waresdev::WareBuildOptions);

#endif /* __OPENFLUID_WARESDEV_WAREBUILDOPTIONS_HPP__ */
