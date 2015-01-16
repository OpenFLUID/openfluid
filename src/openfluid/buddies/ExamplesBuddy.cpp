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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/buddies/ExamplesBuddy.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/config.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <openfluid/tools/FileHelpers.hpp>


namespace openfluid { namespace buddies {


ExamplesBuddy::ExamplesBuddy(openfluid::buddies::BuddiesListener* Listener):
    OpenFLUIDBuddy(Listener)
{
  m_OtherOptionsHelp["selection"] = "* to install all examples (default), the name of the example to install instead";
  m_OtherOptionsHelp["sourcedir"] = "source directory of examples to install (default is standard source directory)";
  m_OtherOptionsHelp["installdir"] = "destination directory of examples to install (default is standard install directory)";
  m_OtherOptionsHelp["force"] = "set to 1 to force installation, even if example is already installed (default is 0)";
}


// =====================================================================
// =====================================================================


ExamplesBuddy::~ExamplesBuddy()
{

}


// =====================================================================
// =====================================================================


bool ExamplesBuddy::installExampleProject(const std::string& ProjectsSourcePath, const std::string& ProjectsInstallPath,
                                          const std::string& ProjectDir, const bool Force)
{
  mp_Listener->onSubstageCompleted("Installing example project \"" + ProjectDir + "\" ... ");

  boost::filesystem::path AllProjectsInstallPath(ProjectsInstallPath);
  boost::filesystem::path ThisProjectInstallPath(ProjectsInstallPath+"/"+ProjectDir);

  if (!boost::filesystem::is_directory(ThisProjectInstallPath) || Force)
  {
    boost::filesystem::create_directories(ThisProjectInstallPath);

    openfluid::tools::copyDirectoryRecursively(boost::filesystem::path(ProjectsSourcePath+"/"+ProjectDir).string(),
                                               AllProjectsInstallPath.string());
    mp_Listener->onSubstageCompleted("Done");
    return true;
  }
  else
    mp_Listener->onSubstageCompleted("Skipped");

  return false;
}


// =====================================================================
// =====================================================================


bool ExamplesBuddy::installAllExamplesProjects(const std::string& ProjectsSourcePath, const std::string& ProjectsInstallPath, const bool Force)
{
  boost::filesystem::path PathToExplore(ProjectsSourcePath);


  if (boost::filesystem::is_directory(PathToExplore))
  {
    boost::filesystem::directory_iterator it;

    for (it = boost::filesystem::directory_iterator(PathToExplore);it != boost::filesystem::directory_iterator(); ++it)
    {
      if (boost::filesystem::is_directory(it->status()))
        installExampleProject(ProjectsSourcePath,ProjectsInstallPath,it->path().stem().string(),Force);
    }
  }
  else
    throw openfluid::base::FrameworkException("ExamplesBuddy::installAllExamplesProjects()","Projects source path is not a directory");

  return true;
}


// =====================================================================
// =====================================================================


bool ExamplesBuddy::run()
{
  setOptionIfNotSet("selection","*");
  setOptionIfNotSet("sourcedir",boost::filesystem::path(openfluid::base::RuntimeEnvironment::instance()->getProvidedExamplesDir()+"/"+openfluid::config::PROJECTS_SUBDIR).string());
  setOptionIfNotSet("installdir",boost::filesystem::path(openfluid::base::RuntimeEnvironment::instance()->getUserExamplesDir()+"/"+openfluid::config::PROJECTS_SUBDIR).string());
  setOptionIfNotSet("force","0");

  if (m_Options["selection"] == "*")
    installAllExamplesProjects(m_Options["sourcedir"],m_Options["installdir"],m_Options["force"]=="1");
  else
    installExampleProject(m_Options["sourcedir"],m_Options["installdir"],m_Options["selection"],m_Options["force"]=="1");

  return true;
}

} } // namespaces

