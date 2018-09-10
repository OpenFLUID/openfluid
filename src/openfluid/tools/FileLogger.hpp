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
  @file FileLogger.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_TOOLS_FILELOGGER_HPP__
#define __OPENFLUID_TOOLS_FILELOGGER_HPP__


#include <fstream>

#include <openfluid/dllexport.hpp>


class QMutex;


namespace openfluid { namespace tools {


class OPENFLUID_API FileLogger
{
  private:

    QMutex* mp_LogMutex;

    std::ofstream m_LogFile;

    unsigned int m_InfosCount;

    unsigned int m_WarningsCount;

    bool m_IsError;


  public:

    enum LogType {LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_DEBUG };

    FileLogger();

    ~FileLogger();


    static std::string logTypeToString(LogType LType);

    void init(const std::string& FilePath);

    void close();

    void add(LogType LType, const std::string& Context, const std::string& Msg);

    unsigned int getInfosCount() const
    { return m_InfosCount; }

    bool isError() const
    { return m_IsError; }

    unsigned int getWarningsCount() const
    { return m_WarningsCount; }

};


} }  // namespaces


#endif /* __OPENFLUID_TOOLS_FILELOGGER_HPP__ */
