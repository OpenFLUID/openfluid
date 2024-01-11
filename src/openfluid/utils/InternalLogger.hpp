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
  @file InternalLogger.hpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_UTILS_INTERNALLOGGER_HPP__
#define __OPENFLUID_UTILS_INTERNALLOGGER_HPP__


#include <iostream>

#include <string>
#include <ctime>    

#include <openfluid/base/Environment.hpp>
#include <openfluid/config.hpp>
#include <openfluid/tools/FileLogger.hpp>

#include <openfluid/utils/SingletonMacros.hpp>


namespace openfluid { namespace utils {


class OPENFLUID_API LoggingSystem
{
  OPENFLUID_SINGLETON_DEFINITION(LoggingSystem)

  private:

    bool m_Activated;

    bool m_AsFile;

    openfluid::tools::FileLogger m_Logger;

    std::vector<std::string> m_Logs; // only used when output as file set to false

    std::string m_LogPath;
  
    LoggingSystem();

    ~LoggingSystem();


  public:

    void setup(bool DefaultAsFallback=true, std::string LogPath="");

    void close();

    void add(openfluid::tools::FileLogger::LogType LType, const std::string& Context, const std::string& Msg);

    bool isActivated()
    {
      return m_Activated;
    }

    std::string getLogPath()
    {
      return m_LogPath;
    }

    void setOutput(bool AsFile=true)
    {
      m_AsFile = AsFile;
    }

    std::string getLastLog()
    {
      if (m_Logs.size() > 0)
      {
        return m_Logs.back();
      }
      return "";
    }

};


namespace log {

void OPENFLUID_API setup(bool DefaultAsFallback=true, std::string LogPath="");

void OPENFLUID_API debug(const std::string& Context, const std::string& Msg);

void OPENFLUID_API info(const std::string& Context, const std::string& Msg);

void OPENFLUID_API warning(const std::string& Context, const std::string& Msg);

void OPENFLUID_API error(const std::string& Context, const std::string& Msg);
}

} }  // namespaces


#endif /* __OPENFLUID_UTILS_INTERNALLOGGER_HPP__ */
