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
  @file CMakeProxy.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/utils/CMakeProxy.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/utils/InternalLogger.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace utils {


void CMakeProxy::findCMakeProgram()
{
  if (m_ExecutablePath.empty())
  {
    m_ExecutablePath = 
      ExternalProgram::getRegisteredProgram(ExternalProgram::RegisteredPrograms::CMake).getFullProgramPath();

    if (!m_ExecutablePath.empty())
    {
      Process P(m_ExecutablePath,{"--version"});
      P.run();
      const auto OutLines = P.stdOutLines();

      if (!OutLines.empty() && openfluid::tools::startsWith(OutLines[0],"cmake version "))
      {
        m_Version = OutLines[0];
        m_Version.erase(0,14);
      }
    }
    else
    {
      //TODO transmit behaviour if path empty
      openfluid::utils::log::warning("CMakeProxy", "CMake path is empty"); 
    }
  }
}


// =====================================================================
// =====================================================================


bool CMakeProxy::isAvailable()
{
  findCMakeProgram();

  return (!m_ExecutablePath.empty() && !m_Version.empty());
}


// =====================================================================
// =====================================================================


std::string CMakeProxy::getBuildDir(const std::string& BuildType)
{
  return "_build-"+openfluid::tools::toLowerCase(BuildType)+"-"
                  +openfluid::config::VERSION_MAJOR+"."+openfluid::config::VERSION_MINOR;
}


// =====================================================================
// =====================================================================


Process::Command CMakeProxy::getConfigureCommand(const std::string& BuildDir, const std::string& SrcDir,
                                                 const std::map<std::string,std::string>& Variables,
                                                 const std::string& Generator, const std::vector<std::string>& Options)
{
  Process::Command Cmd;

  if (!isAvailable())
  {
    return Cmd;
  }

  Cmd.Program = m_ExecutablePath;

  Cmd.WorkDir = BuildDir;
  Cmd.Args << SrcDir; // cmake configure command with the sources directory

  if (!Generator.empty())
  {
    Cmd.Args << "-G" << Generator;
  }

  for (const auto& Var : Variables)
  {
    Cmd.Args << std::string("-D"+Var.first+"="+Var.second);
  }

  for (const auto& Opt : Options)
  {
    Cmd.Args << Opt;
  }

  return Cmd;
}


// =====================================================================
// =====================================================================


Process::Command CMakeProxy::getBuildCommand(const std::string& BuildDir,
                                             const std::string& Target,
                                             const unsigned int Jobs,
                                             const std::vector<std::string>& CMakeOptions, 
                                             const std::vector<std::string>& OtherOptions)
{
  Process::Command Cmd;

  if (!isAvailable())
  {
    return Cmd;
  }

  Cmd.Program = m_ExecutablePath;

  Cmd.WorkDir = BuildDir;
  Cmd.Args << "--build" << "."; // build command in the current directory 

  if (!Target.empty())
  {
    Cmd.Args << "--target" << Target;
  }

  for (const auto& Opt : CMakeOptions)
  {
    Cmd.Args << Opt;
  }

  // Add Jobs option
  if (Jobs || !OtherOptions.empty())
  {
    Cmd.Args << "--";

    if (Jobs)
    {
      Cmd.Args << "-j" << std::to_string(Jobs);
    }

    for (const auto& Opt : OtherOptions)
    {
      Cmd.Args << Opt;
    }
  }

  return Cmd;
}


// =====================================================================
// =====================================================================


Process::Command CMakeProxy::getTarCompressCommand(const std::string& WorkDir,
                                                   const std::string& TarFilePath, 
                                                   const std::vector<std::string>& RelativePathsToCompress,
                                                   const std::string& Options)
{
  Process::Command Cmd;

  if (!isAvailable())
  {
    return Cmd;
  }

  Cmd.Program = m_ExecutablePath;

  // #TODO Use command workdir instead
  Cmd.Args << "-E" << "chdir" << WorkDir; // cd to work directory
  Cmd.Args << m_ExecutablePath << "-E" << "tar"; // tar command
  Cmd.Args << "cf"+Options;
  Cmd.Args << TarFilePath;
  Cmd.Args << RelativePathsToCompress;

  return Cmd;
}


// =====================================================================
// =====================================================================


Process::Command CMakeProxy::getTarUncompressCommand(const std::string& WorkDir, const std::string& TarFilePath,
                                                     const std::string& Options)
{
  Process::Command Cmd;

  if (!isAvailable())
  {
    return Cmd;
  }

  Cmd.Program = m_ExecutablePath;

  // #TODO Use command workdir instead
  Cmd.Args << "-E" << "chdir" << WorkDir; // cd to work directory
  Cmd.Args << m_ExecutablePath << "-E" << "tar"; // tar command
  Cmd.Args << "xf"+Options;
  Cmd.Args << TarFilePath;

  return Cmd;
}


} }  // namespaces
