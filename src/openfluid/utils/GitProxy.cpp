/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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
 @file GitProxy.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 @author Armel THÖNI <armel.thoni@inrae.fr>
 @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <openfluid/utils/GitProxy.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/utils/Process.hpp>
#include <openfluid/base/InternalLogger.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace utils {


GitProxy::GitProxy()
{
  findGitProgram();
  m_LocalGitProgram = m_ExecutablePath;  // Hotfix to avoid issues under windows with static var def by child class
}


// =====================================================================
// =====================================================================


GitProxy::~GitProxy()
{

}


// =====================================================================
// =====================================================================


std::string GitProxy::getExecutablePath()
{
  return m_ExecutablePath;
}


// =====================================================================
// =====================================================================


void GitProxy::findGitProgram()
{
  if (m_ExecutablePath.empty())
  {
    m_ExecutablePath = 
      ExternalProgram::getRegisteredProgram(ExternalProgram::RegisteredPrograms::Git).getFullProgramPath();

    if (!m_ExecutablePath.empty())
    {
      Process P(m_ExecutablePath,{"--version"});
      P.run();
      const auto OutLines = P.stdOutLines();

      if (!OutLines.empty() && openfluid::tools::startsWith(OutLines[0],"git version "))
      {
        m_Version = OutLines[0];
        m_Version.erase(0,12);
      }
    }
  }
}


// =====================================================================
// =====================================================================


bool GitProxy::isAvailable()
{
  findGitProgram();

  return (!m_ExecutablePath.empty() && !m_Version.empty());
}


// =====================================================================
// =====================================================================


bool GitProxy::isPathGitRepo(const std::string& Path)
{
  openfluid::utils::Process::Command Cmd{
    .Program = m_ExecutablePath,
    .Args = {"rev-parse", "--is-inside-work-tree"},
    .WorkDir = Path
  };
  openfluid::utils::Process Process(Cmd);
  Process.run();
  return Process.getExitCode() == 0;
}


// =====================================================================
// =====================================================================


int callRemoteProcess(openfluid::utils::Process::Command Cmd, std::string Context)
{
  openfluid::utils::Process P(Cmd);
  P.run();
  if (P.getExitCode() == 0)
  {
    openfluid::base::log::debug("Git", Context+" OK");
  }
  else
  {
    openfluid::base::log::debug("Git", Context+" out: "+openfluid::tools::join(P.stdOutLines(), "\n"));
    openfluid::base::log::error("Git", Context+" err: "+openfluid::tools::join(P.stdErrLines(), "\n"));
  }
  return P.getExitCode();
}


// =====================================================================
// =====================================================================


int GitProxy::clone(const std::string& Path, const std::string& URL, const std::string& LocalName)
{
  openfluid::utils::Process::Command Cmd{
    .Program = m_ExecutablePath,
    .Args = {"clone", "--recurse-submodules", URL, LocalName},
    .WorkDir = Path
  };
  return callRemoteProcess(Cmd, "Git clone "+URL+" at "+Path+" as "+LocalName);
}


// =====================================================================
// =====================================================================


const std::string GitProxy::getCurrentPosition(const std::string& Path, bool AsBranch)
{
  if (AsBranch && !canGetBranch())
  {
    std::string ErrorMsg = "Error with git branch command: Git version not supported (" + m_Version + ")";
    openfluid::base::log::error("Git", ErrorMsg);
    throw GitOperationException(ErrorMsg);
  }
  std::vector<std::string> Args = {"branch", "--show-current"};
  if (!AsBranch)
  {
    Args = {"rev-parse", "HEAD"}; // returns commit hash
  }
  openfluid::utils::Process::Command Cmd{
    .Program = m_ExecutablePath,
    .Args = Args,
    .WorkDir = Path
  };
  openfluid::utils::Process Process(Cmd);
  Process.run();
  const auto& OutLines = Process.stdOutLines();
  if (Process.getExitCode() == 0 && OutLines.size() > 0)
  {
    return OutLines[0];
  }
  else
  {
    std::string ErrorMsg = "Error with git branch command in path : " + Path;
    openfluid::base::log::error("Git", ErrorMsg);
    throw GitOperationException(ErrorMsg);
  }
}


// =====================================================================
// =====================================================================


int GitProxy::setRemote(const std::string RepoPath, const std::string RemoteUrl)
{
  if(isPathGitRepo(RepoPath))
  {
    // already versioned, use 'set-url'
    openfluid::utils::Process::Command Cmd{
      .Program = m_ExecutablePath,
      .Args = {"remote", "set-url", "origin", RemoteUrl},
      .WorkDir = RepoPath
    };
    return callRemoteProcess(Cmd, "Set remote for "+RepoPath);
  }
  else
  {
    // not versioned, use 'git init+git remote add origin'
    openfluid::utils::Process::Command CmdInit{
      .Program = m_ExecutablePath,
      .Args = {"init"},
      .WorkDir = RepoPath
    };
    int InitReturn = callRemoteProcess(CmdInit, "(Add remote context) Init for "+RepoPath);
    if (InitReturn != 0)
    {
      return InitReturn;
    }
    openfluid::utils::Process::Command Cmd{
      .Program = m_ExecutablePath,
      .Args = {"remote", "add", "origin", RemoteUrl},
      .WorkDir = RepoPath
    };
    return callRemoteProcess(Cmd, "Add remote for "+RepoPath);;
  }

}


} } // namespaces
