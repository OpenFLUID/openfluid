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
  @file SimulationDrivenWare.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_WARE_SIMULATIONDRIVENWARE_HPP__
#define __OPENFLUID_WARE_SIMULATIONDRIVENWARE_HPP__


#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/base/SimulationLogger.hpp>
#include <openfluid/ware/WareException.hpp>
#include <openfluid/base/SimulationStatus.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/deprecation.hpp>


#define REQUIRE_SIMULATION_STAGE(stage,sender,msg) \
  if (OPENFLUID_GetCurrentStage() != (stage)) \
    throw openfluid::base::FrameworkException(sender,msg);

#define REQUIRE_SIMULATION_STAGE_GE(stage,sender,msg) \
  if (OPENFLUID_GetCurrentStage() < (stage)) \
    throw openfluid::base::FrameworkException(sender,msg);

#define REQUIRE_SIMULATION_STAGE_LE(stage,sender,msg) \
  if (OPENFLUID_GetCurrentStage() > (stage)) \
    throw openfluid::base::FrameworkException(sender,msg);


// =====================================================================
// =====================================================================


#define _STREAMTOSTRING(_stream) ((static_cast<std::ostringstream&>(std::ostringstream().flush() << _stream)).str())


// Log macros for warnings

#define OPENFLUID_LogWarning(_stream) \
  appendToLog(openfluid::tools::FileLogger::LOG_WARNING,_STREAMTOSTRING(_stream));

#define OPENFLUID_DisplayWarning(_stream) \
  displayToConsole(openfluid::tools::FileLogger::LOG_WARNING,_STREAMTOSTRING(_stream));

#define OPENFLUID_LogAndDisplayWarning(_stream) \
  OPENFLUID_LogWarning(_stream) \
  OPENFLUID_DisplayWarning(_stream)


// Log macros for infos

#define OPENFLUID_LogInfo(_stream) \
  appendToLog(openfluid::tools::FileLogger::LOG_INFO,_STREAMTOSTRING(_stream));

#define OPENFLUID_DisplayInfo(_stream) \
  displayToConsole(openfluid::tools::FileLogger::LOG_INFO,_STREAMTOSTRING(_stream));

#define OPENFLUID_LogAndDisplayInfo(_stream) \
  OPENFLUID_LogInfo(_stream) \
  OPENFLUID_DisplayInfo(_stream)


// Log macros for debug

#ifndef NDEBUG

#define OPENFLUID_LogDebug(_stream) \
  appendToLog(openfluid::tools::FileLogger::LOG_DEBUG,_STREAMTOSTRING(_stream));

#define OPENFLUID_DisplayDebug(_stream) \
  displayToConsole(openfluid::tools::FileLogger::LOG_DEBUG,_STREAMTOSTRING(_stream));

#define OPENFLUID_LogAndDisplayDebug(_stream) \
  OPENFLUID_LogDebug(_stream) \
  OPENFLUID_DisplayDebug(_stream)

#else

#define OPENFLUID_LogDebug(_stream)

#define OPENFLUID_DisplayDebug(_stream)

#define OPENFLUID_LogAndDisplayDebug(_stream)

#endif


// =====================================================================
// =====================================================================


namespace openfluid { namespace ware {


class OPENFLUID_API SimulationDrivenWare : public PluggableWare
{
  private:

    const openfluid::base::SimulationStatus* mp_SimStatus;

    openfluid::base::SimulationLogger* mp_SimLogger;

    openfluid::core::TimeIndex_t m_PreviousTimeIndex;


    openfluid::base::ExceptionContext computeWareContext() const;


  protected:


    virtual bool isLinked() const
    { return (PluggableWare::isLinked() &&  mp_SimLogger != NULL && mp_SimStatus != NULL); };

    void appendToLog(openfluid::tools::FileLogger::LogType LType, const std::string& Msg) const;

    void displayToConsole(openfluid::tools::FileLogger::LogType LType, const std::string& Msg) const;


    /**
      Returns the real beginning date of the simulated period
      @return the date
    */
    openfluid::core::DateTime OPENFLUID_GetBeginDate() const;

    /**
      Returns the real ending date of the simulated period
      @return the date
    */
    openfluid::core::DateTime OPENFLUID_GetEndDate() const;

    /**
      Returns the current real date corresponding to the current time index
      @return the date
    */
    openfluid::core::DateTime OPENFLUID_GetCurrentDate() const;

    /**
      Returns the simulation duration in seconds
      @return the duration in seconds
    */
    openfluid::core::Duration_t OPENFLUID_GetSimulationDuration() const;

    /**
      Returns the default DeltaT used by the scheduler
      @return the deltaT in seconds
    */
    openfluid::core::Duration_t OPENFLUID_GetDefaultDeltaT() const;

    /**
      Returns the current time index of the simulation, in seconds since the simulation started.
      When the simulation starts, the time index is equal to zero.
      @return the current time index in seconds
    */
    openfluid::core::TimeIndex_t OPENFLUID_GetCurrentTimeIndex() const;

    /**
      Returns the time index of the simulation when the plugged ware was previously run
      @return the time index in seconds
    */
    openfluid::core::TimeIndex_t OPENFLUID_GetPreviousRunTimeIndex() const;

    /**
      Returns the current stage of the simulation
      @return the stage
    */
    openfluid::base::SimulationStatus::SimulationStage OPENFLUID_GetCurrentStage() const;

    /**
      Returns the scheduling constraint applied to the simulation (may be NONE)
      @return the constraint type
    */
    openfluid::base::SimulationStatus::SchedulingConstraint OPENFLUID_GetSchedulingConstraint() const;


    /**
      Raises a time-marked warning message to the kernel. This does not stops the simulation
      @param[in] Msg the content of the message
    */
    virtual void OPENFLUID_RaiseWarning(const std::string& Msg);

    /**
       Raises a time-marked warning message to the kernel. This does not stops the simulation
       @param[in] Source the source of the message
       @param[in] Msg the content of the message
       @deprecated
     */
    virtual void OPENFLUID_RaiseWarning(const std::string& Source, const std::string& Msg) OPENFLUID_DEPRECATED;

    /**
      Raises an error message to the kernel. This stops the simulation the next time the kernel has the control
      @param[in] Msg the content of the message
    */
    virtual void OPENFLUID_RaiseError(const std::string& Msg);

    /**
      Raises an error message to the kernel. This stops the simulation the next time the kernel has the control
      @param[in] Source the source of the message
      @param[in] Msg the content of the message
      @deprecated
    */
    virtual void OPENFLUID_RaiseError(const std::string& Source, const std::string& Msg) OPENFLUID_DEPRECATED;

    SimulationDrivenWare(WareType WType) : PluggableWare(WType),
        mp_SimStatus(NULL), mp_SimLogger(NULL), m_PreviousTimeIndex(0) { };


  public:

    virtual ~SimulationDrivenWare()
    { };

    void linkToSimulation(const openfluid::base::SimulationStatus* SimStatus);

    void linkToSimulationLogger(openfluid::base::SimulationLogger* SimLogger)
    { mp_SimLogger = SimLogger; };

    void initializeWare(const WareID_t& ID);

    void finalizeWare();

    void setPreviousTimeIndex(const openfluid::core::TimeIndex_t& TimeIndex)
    { m_PreviousTimeIndex = TimeIndex; };

};



} } // openfluid::ware



#endif /* __OPENFLUID_WARE_SIMULATIONDRIVENWARE_HPP__ */
