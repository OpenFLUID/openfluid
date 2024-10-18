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
  @file InternalLogger.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/utils/InternalLogger.hpp>
#include "openfluid/tools/FilesystemPath.hpp"
#include "openfluid/tools/FileLogger.hpp"


namespace openfluid { namespace utils {


OPENFLUID_SINGLETON_INITIALIZATION(LoggingSystem)


LoggingSystem::LoggingSystem() : m_Activated(false), m_AsFile(true)
{
}


// =====================================================================
// =====================================================================


LoggingSystem::~LoggingSystem()
{
  close();
}


// =====================================================================
// =====================================================================


void LoggingSystem::setup(bool DefaultAsFallback, std::string LogPath)
{
  // FALLBACK LOG PATH: 
  //    1) direct param 
  //    2) env var OPENFLUID_LOG_PATH if present 
  //    3) default path if UseDefault true
  if (LogPath == "")
  {
    char* LogPathChar = std::getenv("OPENFLUID_LOG_PATH");
    std::string CurrentWorkspace = openfluid::base::PreferencesManager::instance()->getCurrentWorkspacePath();
    if (LogPathChar != NULL)
    {
      LogPath = openfluid::tools::FilesystemPath({std::string(LogPathChar), 
                                                  openfluid::config::INTERNAL_LOG_FILE}).toGeneric();
    }
    else if(CurrentWorkspace != "")
    {
      LogPath = openfluid::tools::FilesystemPath({CurrentWorkspace, 
                                                  openfluid::config::INTERNAL_LOG_FILE}).toGeneric();
    }
    else if (DefaultAsFallback)
    {
      LogPath = openfluid::tools::FilesystemPath({
        openfluid::base::Environment::getUserDataFullPath(openfluid::config::WORKSPACE_PATH),
        openfluid::config::INTERNAL_LOG_FILE}).toGeneric();
    }
  }
  m_Activated = false;
  if (LogPath == "" && m_LogPath == "")
  {
    return;
  }
  m_Activated = true;
  if (LogPath != m_LogPath)
  {
    if (LogPath != "")
    {
      m_LogPath = LogPath;
    }
    
    // Creates path if necessary
    openfluid::tools::FilesystemPath(openfluid::tools::FilesystemPath(m_LogPath).dirname()).makeDirectory();

    std::cout << "Initializing internal OpenFLUID logs at: "+m_LogPath << std::endl;
    // FIXME should not be displayed in some cases 
    // (interference risk with version detection for now, eg regex L142 in OpenFLUIDConfig.cmake.in)
    m_Logger.init(m_LogPath, false);
  }
}


// =====================================================================
// =====================================================================


void LoggingSystem::add(openfluid::tools::FileLogger::LogType LType, const std::string& Context, const std::string& Msg)
{
  if (m_Activated)
  {
    if (m_AsFile)
    {
      m_Logger.add(LType, Context,  openfluid::core::DateTime::now().getAsISOString() + "\t" + Msg);
      m_Logger.flush();  // TODO optimize flushing strategy
    }
    else
    {
      m_Logs.push_back(Msg);
    }
  }
  else
  {
    std::cout << "Logging system not activated" << std::endl;
  }
}


// =====================================================================
// =====================================================================


void LoggingSystem::close()
{
  m_Logger.close();
}


// =====================================================================
// =====================================================================


namespace log {


void setup(bool DefaultAsFallback, std::string LogPath)
{
  LoggingSystem::instance()->setup(DefaultAsFallback, LogPath);
}


// =====================================================================
// =====================================================================


void debug(const std::string& Context, const std::string& Msg)
{
  LoggingSystem::instance()->add(openfluid::tools::FileLogger::LogType::DEBUG_MSG, Context, Msg);
}


// =====================================================================
// =====================================================================


void info(const std::string& Context, const std::string& Msg)
{
  LoggingSystem::instance()->add(openfluid::tools::FileLogger::LogType::INFO_MSG, Context, Msg);
}


// =====================================================================
// =====================================================================


void warning(const std::string& Context, const std::string& Msg)
{
  LoggingSystem::instance()->add(openfluid::tools::FileLogger::LogType::WARNING_MSG, Context, Msg);
}


// =====================================================================
// =====================================================================


void error(const std::string& Context, const std::string& Msg)
{
  LoggingSystem::instance()->add(openfluid::tools::FileLogger::LogType::ERROR_MSG, Context, Msg);
}


}


} }  // namespaces

