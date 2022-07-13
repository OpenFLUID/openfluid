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
  @file ExamplesManager.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
 */


#ifndef __OPENFLUID_BASE_EXAMPLESMANAGER_HPP__
#define __OPENFLUID_BASE_EXAMPLESMANAGER_HPP__


#include <string>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace base {


class OPENFLUID_API ExamplesManager
{

  private:

    static std::string buildRessourcesPath(const std::string& Path);

    static std::string buildInstallPath(const std::string& Path);

    static bool installDirectory(const std::string& FromPath, const std::string& ToPath,
                                 const std::string& DirName, const bool Force = false);

  public:

    ExamplesManager() = delete;

    ~ExamplesManager() = delete;

    /**
      Installs a single example project, using its directory name
      @param[in] ProjectDir Name of the project directory to install
      @param[in] ResourcesPath Path to the examples ressources, 
                               uses standard ressources path if empty (default is empty)
      @param[in] InstallPath Path where to install the examples ressources, 
                             uses user openfluid path if empty (default is empty)
      @param[in] Force If true, the installation is forced even if it already exists (default is false)
    */
    static bool installProject(const std::string& ProjectDir,
                               const std::string& ResourcesPath = "", const std::string& InstallPath = "",
                               const bool Force = false);

    /**
      Installs a single example simulator, using its directory name
      @param[in] SimulatorDir Name of the simulator directory to install
      @param[in] ResourcesPath Path to the examples ressources, 
                               uses standard ressources path if empty (default is empty)
      @param[in] InstallPath Path where to install the examples ressources, 
                             uses user openfluid path if empty (default is empty)
      @param[in] Force If true, the installation is forced even if it already exists (default is false)
    */
    static bool installSimulator(const std::string& SimulatorDir, 
                                 const std::string& ResourcesPath = "", const std::string& InstallPath = "",
                                 const bool Force = false);

    /**
      Installs all example projects
      @param[in] ResourcesPath Path to the examples ressources, 
                               uses standard ressources path if empty (default is empty)
      @param[in] InstallPath Path where to install the examples ressources, 
                             uses user openfluid path if empty (default is empty)
      @param[in] Force If true, the installation is forced even if it already exists (default is false)
    */
    static bool installAllProjects(const std::string& ResourcesPath = "", const std::string& InstallPath = "", 
                                   const bool Force = false);

    /**
      Installs all example simulators
      @param[in] ResourcesPath Path to the examples ressources, 
                               uses standard ressources path if empty (default is empty)
      @param[in] InstallPath Path where to install the examples ressources, 
                             uses user openfluid path if empty (default is empty)
      @param[in] Force If true, the installation is forced even if it already exists (default is false)
    */
    static bool installAllSimulators(const std::string& ResourcesPath = "", const std::string& InstallPath = "",
                                     const bool Force = false);

    /**
      Installs all example projects and simulators
      @param[in] ResourcesPath Path to the examples ressources, 
                               uses standard ressources path if empty (default is empty)
      @param[in] InstallPath Path where to install the examples ressources, 
                             uses user openfluid path if empty (default is empty)
      @param[in] Force If true, the installation is forced even if it already exists (default is false)
    */
    static bool installAll(const std::string& ResourcesPath = "", const std::string& InstallPath = "",
                           const bool Force = false);
};


} } // namespaces


#endif /* __OPENFLUID_BASE_EXAMPLESMANAGER_HPP__ */
