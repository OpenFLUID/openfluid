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
 @file GitProxy.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/utils/GitProxy.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
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


} } // namespaces
