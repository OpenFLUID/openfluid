/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file ExamplesBuddy.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/buddies/ExamplesBuddy.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/SwissTools.hpp>
#include <openfluid/config.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>


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

  boost::filesystem::path InstallPath(ProjectsInstallPath);

  if (!boost::filesystem::is_directory(InstallPath) || Force)
  {
    boost::filesystem::create_directories(InstallPath);

    openfluid::tools::CopyDirectoryRecursively(boost::filesystem::path(ProjectsSourcePath+"/"+ProjectDir).string(),
                                               InstallPath.string());
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
  setOptionIfNotSet("sourcedir",boost::filesystem::path(openfluid::base::RuntimeEnvironment::getInstance()->getProvidedExamplesDir()+"/"+openfluid::config::PROJECTS_SUBDIR).string());
  setOptionIfNotSet("installdir",boost::filesystem::path(openfluid::base::RuntimeEnvironment::getInstance()->getUserExamplesDir()+"/"+openfluid::config::PROJECTS_SUBDIR).string());
  setOptionIfNotSet("force","0");

  if (m_Options["selection"] == "*")
    installAllExamplesProjects(m_Options["sourcedir"],m_Options["installdir"],m_Options["force"]=="1");
  else
    installExampleProject(m_Options["sourcedir"],m_Options["installdir"],m_Options["selection"],m_Options["force"]=="1");

  return true;
}

} } // namespaces

