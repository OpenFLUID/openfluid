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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_BASE_SIMULATIONLOGGER_HPP__
#define __OPENFLUID_BASE_SIMULATIONLOGGER_HPP__

#include <string>
#include <fstream>

#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/core/DateTime.hpp>

#include <openfluid/tools/SwissTools.hpp>


namespace openfluid { namespace base {

/**
  Class for managing messages during execution

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/
class OPENFLUID_API SimulationLogger
{

  private:

    std::ofstream m_LogFile;

    bool m_WarningFlag;

    unsigned int m_RealWarningsCount;

    void addLog(const std::string& Prefix,
                    const std::string& Sender,
                    const std::string& Source,
                    const openfluid::core::TimeIndex_t& TimeIndex,
                    const std::string& Msg);

    void addLog(const std::string& Prefix,
                    const std::string& Sender,
                    const openfluid::core::TimeIndex_t& TimeIndex,
                    const std::string& Msg);

    void addLog(const std::string& Prefix,
                    const std::string& Sender,
                    const std::string& Source,
                    const std::string& Msg);

    void addLog(const std::string& Prefix,
                    const std::string& Sender,
                    const std::string& Msg);

    void addLog(const std::string& Prefix,
                    const std::string& Msg);


  public:

    SimulationLogger(const std::string& LogFilePath);


    ~SimulationLogger();


    void addWarning(const std::string& Sender, const std::string& Source, const openfluid::core::TimeIndex_t& TimeIndex, const std::string& Msg)
    { addLog("[Warning]",Sender,Source, TimeIndex, Msg); m_WarningFlag = true; m_RealWarningsCount++; };


    void addWarning(const std::string& Sender, const openfluid::core::TimeIndex_t& TimeIndex, const std::string& Msg)
    { addLog("[Warning]",Sender,TimeIndex, Msg); m_WarningFlag = true; m_RealWarningsCount++; };


    void addWarning(const std::string& Sender, const std::string& Source, const std::string& Msg)
    { addLog("[Warning]",Sender,Source,Msg); m_WarningFlag = true; m_RealWarningsCount++; };


    void addWarning(const std::string& Sender, const std::string& Msg)
    { addLog("[Warning]",Sender,Msg); m_WarningFlag = true; m_RealWarningsCount++; };


    void addMessage(const std::string& Sender, const std::string& Source, const openfluid::core::TimeIndex_t& TimeIndex, const std::string& Msg)
    { addLog("[Message]",Sender,Source, TimeIndex, Msg); };


    void addMessage(const std::string& Sender, const openfluid::core::TimeIndex_t& TimeIndex, const std::string& Msg)
    { addLog("[Message]",Sender, TimeIndex, Msg); };


    void addMessage(const std::string& Sender, const std::string& Source, const std::string& Msg)
    { addLog("[Message]",Sender,Source, Msg); };


    void addMessage(const std::string& Sender, const std::string& Msg)
    { addLog("[Message]",Sender,Msg); };


    void addInfo(const std::string& Sender, const std::string& Msg)
    { addLog("[Info]",Sender,Msg); };


    void addInfo(const std::string& Msg)
    { addLog("[Info]",Msg); };


    inline void resetWarningFlag() { m_WarningFlag = false; };


    inline bool isWarningFlag() const { return m_WarningFlag; };


    inline unsigned int getWarningsCount() const { return m_RealWarningsCount; };

};


} } // namespaces


#endif /*__OPENFLUID_BASE_SIMULATIONLOGGER_HPP__*/

