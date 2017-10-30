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
*/

#include <QMutex>
#include <QMutexLocker>

#include <openfluid/tools/FileLogger.hpp>


namespace openfluid { namespace tools {



FileLogger::FileLogger() :
    m_InfosCount(0), m_WarningsCount(0), m_IsError(false)
{
  mp_LogMutex = new QMutex;
}


// =====================================================================
// =====================================================================


FileLogger::~FileLogger()
{
  close();
  delete mp_LogMutex;
}


// =====================================================================
// =====================================================================


std::string FileLogger::logTypeToString(LogType LType)
{
  std::string LTypeStr = "";

  if (LType == LOG_INFO)
    LTypeStr = "Info";
  else  if (LType == LOG_WARNING)
    LTypeStr = "Warning";
  else if (LType == LOG_DEBUG)
    LTypeStr = "Debug";
  else if (LType == LOG_ERROR)
    LTypeStr = "Error";

  return LTypeStr;
}


// =====================================================================
// =====================================================================


void FileLogger::init(const std::string& FilePath)
{
  m_LogFile.open(FilePath.c_str(),std::ios::out);
}


// =====================================================================
// =====================================================================


void FileLogger::close()
{
  if (m_LogFile.is_open())
    m_LogFile.close();
}


// =====================================================================
// =====================================================================


void FileLogger::add(LogType LType, const std::string& Sender, const std::string& Msg)
{
  QMutexLocker Locker(mp_LogMutex);

  std::string LTypeStr = logTypeToString(LType);

  if (LType == LOG_INFO)
    m_InfosCount++;
  else  if (LType == LOG_WARNING)
    m_WarningsCount++;
  else if (LType == LOG_ERROR)
    m_IsError = true;

  m_LogFile << "[" << LTypeStr << "]" << "[" << Sender << "] " << Msg << "\n";
}


} }
