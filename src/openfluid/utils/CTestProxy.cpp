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
  @file CTestProxy.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/utils/CTestProxy.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/utils/InternalLogger.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace utils {


void CTestProxy::findCTestProgram()
{
  if (m_ExecutablePath.empty())
  {
    m_ExecutablePath = 
      ExternalProgram::getRegisteredProgram(ExternalProgram::RegisteredPrograms::CTest).getFullProgramPath();

    if (!m_ExecutablePath.empty())
    {
      Process P(m_ExecutablePath,{"--version"});
      P.run();
      const auto OutLines = P.stdOutLines();

      if (!OutLines.empty() && openfluid::tools::startsWith(OutLines[0],"ctest version "))
      {
        m_Version = OutLines[0];
        m_Version.erase(0,14);
      }
      else
      {
        openfluid::utils::log::warning("CTestProxy", "Issue with ctest detection"); 
      }
    }
    else
    {
      //TODO transmit behaviour if path empty
      openfluid::utils::log::warning("CTestProxy", "CTest path is empty"); 
    }
  }
}


// =====================================================================
// =====================================================================


bool CTestProxy::isAvailable()
{
  findCTestProgram();

  return (!m_ExecutablePath.empty() && !m_Version.empty());
}


// =====================================================================
// =====================================================================


Process::Command CTestProxy::getTestCommand(const std::string& BuildDir,
                                             const unsigned int Jobs,
                                             const std::vector<std::string>& CTestOptions, 
                                             const std::vector<std::string>& OtherOptions)
{
  Process::Command Cmd;

  if (!isAvailable())
  {
    return Cmd;
  }

  Cmd.Program = m_ExecutablePath;

  Cmd.WorkDir = BuildDir;

  Cmd.Args << "--output-on-failure";

  for (const auto& Opt : CTestOptions)
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


} }  // namespaces
