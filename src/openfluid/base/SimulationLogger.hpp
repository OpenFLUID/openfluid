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
  @file SimulationLogger.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_BASE_SIMULATIONLOGGER_HPP__
#define __OPENFLUID_BASE_SIMULATIONLOGGER_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/tools/FileLogger.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/base/SimulationStatus.hpp>
#include <openfluid/tools/MiscHelpers.hpp>


namespace openfluid { namespace base {


class OPENFLUID_API SimulationLogger : public openfluid::tools::FileLogger
{

  private:

    bool m_CurrentWarningFlag;


  public:

    SimulationLogger(const std::string& LogFilePath);


    ~SimulationLogger();


    void add(LogType LType, const std::string& ContextStr, const std::string& Msg);

    void addInfo(const std::string& Context, const std::string& Msg)
    {
      add(LOG_INFO,Context,Msg);
    }

    void addDebug(const std::string& Context, const std::string& Msg)
    {
      add(LOG_DEBUG,Context,Msg);
    }

    void addWarning(const std::string& Context, const std::string& Msg)
    {
      add(LOG_WARNING,Context,Msg);
    }

    void addError(const std::string& Context, const std::string& Msg)
    { add(LOG_ERROR,Context,Msg); }


    inline void resetCurrentWarningFlag()
    {
      m_CurrentWarningFlag = false;
    }

    inline bool isCurrentWarningFlag() const
    {
      return m_CurrentWarningFlag;
    }

};


} } // namespaces


#endif /*__OPENFLUID_BASE_SIMULATIONLOGGER_HPP__*/

