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
  @file FileLogger.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/FileLogger.hpp>


namespace openfluid { namespace tools {


FileLogger::FileLogger() :
    m_LogMutex(std::mutex()), m_InfosCount(0), m_WarningsCount(0), m_IsError(false)
{

}


// =====================================================================
// =====================================================================


FileLogger::~FileLogger()
{
  close();
}


// =====================================================================
// =====================================================================


std::string FileLogger::logTypeToString(LogType LType)
{
  std::string LTypeStr = "";

  if (LType == LogType::INFO_MSG)
  {
    LTypeStr = "Info";
  }
  else  if (LType == LogType::WARNING_MSG)
  {
    LTypeStr = "Warning";
  }
  else if (LType == LogType::DEBUG_MSG)
  {
    LTypeStr = "Debug";
  }
  else if (LType == LogType::ERROR_MSG)
  {
    LTypeStr = "Error";
  }

  return LTypeStr;
}


// =====================================================================
// =====================================================================


void FileLogger::init(const std::string& FilePath, bool Overwrite)
{
  if (Overwrite)
  {
    m_LogFile.open(FilePath.c_str(),std::ios::out);
  }
  else
  {
    m_LogFile.open(FilePath.c_str(),std::ios::app);
  }
  if (!m_LogFile.is_open())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                             "Log file opening failed: "+FilePath);
  }
}


// =====================================================================
// =====================================================================


void FileLogger::close()
{
  if (m_LogFile.is_open())
  {
    m_LogFile.close();
  }
}


// =====================================================================
// =====================================================================


void FileLogger::flush()
{
  if (m_LogFile.is_open())
  {
    m_LogFile.flush();
  }
}


// =====================================================================
// =====================================================================


void FileLogger::add(LogType LType, const std::string& Sender, const std::string& Msg)
{
  std::scoped_lock<std::mutex> Lock(m_LogMutex);

  std::string LTypeStr = logTypeToString(LType);

  if (LType == LogType::INFO_MSG)
  {
    m_InfosCount++;
  }
  else  if (LType == LogType::WARNING_MSG)
  {
    m_WarningsCount++;
  }
  else if (LType == LogType::ERROR_MSG)
  {
    m_IsError = true;
  }

  m_LogFile << "[" << LTypeStr << "]" << "[" << Sender << "] " << Msg << "\n";
}


} }
