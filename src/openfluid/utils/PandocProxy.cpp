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
  @file PandocProxy.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#include <openfluid/utils/PandocProxy.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace utils {


void PandocProxy::findPandocProgram()
{
  if (m_ExecutablePath.empty())
  {
    m_ExecutablePath = 
      ExternalProgram::getRegisteredProgram(ExternalProgram::RegisteredPrograms::Pandoc).getFullProgramPath();

    if (!m_ExecutablePath.empty())
    {
      Process P(m_ExecutablePath,{"--version"});
      P.run();
      const auto OutLines = P.stdOutLines();

      if (!OutLines.empty() && openfluid::tools::startsWith(OutLines[0],"pandoc "))
      {
        m_Version = OutLines[0];
        m_Version.erase(0,7);
      }
    }
  }
}


// =====================================================================
// =====================================================================


bool PandocProxy::isAvailable()
{
  findPandocProgram();

  return (!m_ExecutablePath.empty() && !m_Version.empty());
}


// =====================================================================
// =====================================================================


Process::Command PandocProxy::getCommand(const std::string& InputFormat, const std::string& InputFilePath,
                                         const std::string& OutputFormat, const std::string& OutputFilePath,
                                         bool WithCiteproc)
{
  Process::Command Cmd;

  if (!isAvailable())
  {
    return Cmd;
  }

  Cmd.Program = m_ExecutablePath;

  if (!InputFormat.empty())
  {
    Cmd.Args << "--from" << InputFormat;
  }

  if (!OutputFormat.empty())
  {
    Cmd.Args << "--to" << OutputFormat;
  }

  if (WithCiteproc)
  {
    bool IsFilter = (openfluid::tools::compareVersions(m_Version,"2.11") > 0);

    if (IsFilter)
    {
      Cmd.Args << "--filter=pandoc-citeproc";
    }
    else
    {
      Cmd.Args << "--citeproc";
    }
  }

  Cmd.Args << InputFilePath << "-o" << OutputFilePath;
  

  return Cmd;
}


} }  // namespaces
