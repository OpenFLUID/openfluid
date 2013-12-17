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
  @file
  @brief implements of ...

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include <openfluid/base/SimulationLogger.hpp>

namespace openfluid { namespace base {


// =====================================================================
// =====================================================================


SimulationLogger::SimulationLogger(const std::string& LogFilePath):
  m_WarningFlag(false), m_RealWarningsCount(0)
{
  m_LogFile.open(LogFilePath.c_str(),std::ios::out);
}


// =====================================================================
// =====================================================================


SimulationLogger::~SimulationLogger()
{
  m_LogFile.close();
}


// =====================================================================
// =====================================================================


void SimulationLogger::addLog(const std::string& Prefix, const std::string& Sender,
                                  const std::string& Source, const openfluid::core::TimeIndex_t& TimeIndex,
                                  const std::string& Msg)
{
  m_LogFile << Prefix << ' ' << Msg << " (sent by " << Sender << ", from " << Source << ", at time index " << TimeIndex << ")\n";
}


// =====================================================================
// =====================================================================


void SimulationLogger::addLog(const std::string& Prefix, const std::string& Sender,
                                  const openfluid::core::TimeIndex_t& TimeIndex, const std::string& Msg)
{
  m_LogFile << Prefix << ' ' << Msg << " (sent by " << Sender << ", at time index " << TimeIndex << ")\n";
}


// =====================================================================
// =====================================================================


void SimulationLogger::addLog(const std::string& Prefix, const std::string& Sender,
                                  const std::string& Source, const std::string& Msg)
{
  m_LogFile << Prefix << ' ' << Msg << " (sent by " << Sender << ", from " << Source << ")\n";
}


// =====================================================================
// =====================================================================


void SimulationLogger::addLog(const std::string& Prefix, const std::string& Sender,
                                  const std::string& Msg)
{
  m_LogFile << Prefix << ' ' << Msg << " (sent by " << Sender << ")\n";
}


// =====================================================================
// =====================================================================


void SimulationLogger::addLog(const std::string& Prefix,
                               const std::string& Msg)
{
  m_LogFile << Prefix << ' ' << Msg << "\n";
}



} } // namespace openfluid::base



