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
  @file ExamplesBuddy.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/buddies/ExamplesBuddy.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/config.hpp>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>


namespace openfluid { namespace buddies {


ExamplesBuddy::ExamplesBuddy(openfluid::buddies::BuddiesListener* Listener):
    OpenFLUIDBuddy(Listener)
{
  m_OtherOptionsHelp["selection"] = "* to install all examples (default), single to give custom install instead "
                                    "(cf simulator or project parameter)";
  m_OtherOptionsHelp["project"] = "give example project name to install this specific project";
  m_OtherOptionsHelp["simulator"] = "give simulator name to install this specific simulator";
  m_OtherOptionsHelp["sourcedir"] = "source directory of examples to install (default is standard source directory)";
  m_OtherOptionsHelp["installdir"] = "destination directory of examples to install "
                                     "(default is standard install directory)";
  m_OtherOptionsHelp["force"] = "set to 1 to force installation, even if example is already installed (default is 0)";
}


// =====================================================================
// =====================================================================


ExamplesBuddy::~ExamplesBuddy()
{

}


// =====================================================================
// =====================================================================


bool ExamplesBuddy::installExampleFolder(const std::string& SourcePath,
                                          const std::string& InstallPath,
                                          const std::string& FolderName, const bool Force)
{
  std::string ThisFolderInstallPath = InstallPath+"/"+FolderName;

  if (!openfluid::tools::Filesystem::isDirectory(ThisFolderInstallPath) || Force)
  {
    openfluid::tools::Filesystem::makeDirectory(ThisFolderInstallPath);

    openfluid::tools::Filesystem::copyDirectory(SourcePath+"/"+FolderName,
                                                InstallPath);
    mp_Listener->onSubstageCompleted("Done");

    return true;
  }
  else
  {
    mp_Listener->onSubstageCompleted("Skipped");
  }

  return false;
}

    
// =====================================================================
// =====================================================================


bool ExamplesBuddy::installExampleProject(const std::string& ProjectsSourcePath,
                                          const std::string& ProjectsInstallPath,
                                          const std::string& ProjectDir, const bool Force)
{
  mp_Listener->onSubstageCompleted("Installing example project \"" + ProjectDir + "\" ... ");

  return installExampleFolder(ProjectsSourcePath, ProjectsInstallPath, ProjectDir, Force);
}


// =====================================================================
// =====================================================================


bool ExamplesBuddy::installExampleSimulator(const std::string& SimulatorsSourcePath,
                                          const std::string& SimulatorsInstallPath,
                                          const std::string& SimulatorDir, const bool Force)
{
  // check if simulator exists
  if (openfluid::tools::Filesystem::isDirectory(SimulatorsSourcePath+"/"+SimulatorDir))
  {
    mp_Listener->onSubstageCompleted("Installing example simulator \"" + SimulatorDir + "\" ... ");
    return installExampleFolder(SimulatorsSourcePath, SimulatorsInstallPath, SimulatorDir, Force);
  }
  else
  {
    mp_Listener->onSubstageCompleted("Can't install example simulator: not found");
    return false;
  }
}


// =====================================================================
// =====================================================================


bool ExamplesBuddy::installAllExamplesFolders(const std::string& ExamplesSourcePath,
                                               const std::string& ExamplesInstallPath, const bool Force)
{
  // PROJECTS
  std::string ProjectSuffix = "/"+openfluid::config::PROJECTS_PATH;
  std::string ProjectsSourcePath = ExamplesSourcePath + ProjectSuffix;
  if (openfluid::tools::Filesystem::isDirectory(ProjectsSourcePath))
  {
    std::vector<std::string> FoundProjects = openfluid::tools::findDirectories(ProjectsSourcePath);

    for (unsigned int i=0;i<FoundProjects.size();i++)
    {
      installExampleProject(ProjectsSourcePath,ExamplesInstallPath+ProjectSuffix,FoundProjects[i],Force);
    }
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Projects source path is not a directory");
  }
  
  //SIMULATORS
  std::string SimulatorSuffix = "/"+openfluid::config::WARESDEV_PATH+"/"+openfluid::config::SIMULATORS_PATH;
  std::string SimulatorsSourcePath = ExamplesSourcePath + SimulatorSuffix;
  
  if (openfluid::tools::Filesystem::isDirectory(SimulatorsSourcePath))
  {
    std::vector<std::string> FoundSimulators = openfluid::tools::findDirectories(SimulatorsSourcePath);

    for (unsigned int i=0;i<FoundSimulators.size();i++)
    {
      installExampleSimulator(SimulatorsSourcePath,ExamplesInstallPath+SimulatorSuffix,FoundSimulators[i],Force);
    }
  }

  return true;
}


// =====================================================================
// =====================================================================


bool ExamplesBuddy::run()
{
  setOptionIfNotSet("selection","*");
  setOptionIfNotSet("sourcedir",openfluid::base::Environment::getProvidedExamplesDir());
  setOptionIfNotSet("installdir",openfluid::base::Environment::getUserExamplesDir());
  setOptionIfNotSet("force","0");

  if (m_Options["selection"] == "*")
  {
    installAllExamplesFolders(m_Options["sourcedir"],m_Options["installdir"],m_Options["force"]=="1");
  }
  else if (m_Options["selection"] == "single")
  {
    if (m_Options["project"] != "")
    {
      std::string ProjectSuffix = "/"+openfluid::config::PROJECTS_PATH;
      installExampleProject(m_Options["sourcedir"]+ProjectSuffix,m_Options["installdir"]+ProjectSuffix,
                            m_Options["project"],m_Options["force"]=="1");
    }
    if (m_Options["simulator"] != "")
    {
      std::string SimulatorSuffix = "/"+openfluid::config::WARESDEV_PATH+"/"+openfluid::config::SIMULATORS_PATH;
      installExampleSimulator(m_Options["sourcedir"]+SimulatorSuffix,m_Options["installdir"]+SimulatorSuffix,
                            m_Options["simulator"],m_Options["force"]=="1");
    }
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Selection mode not recognized, must be '*' or 'single'");
  }

  return true;
}


} } // namespaces
