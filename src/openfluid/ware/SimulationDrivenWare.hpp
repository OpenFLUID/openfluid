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


/**
  @internal
*/
#define REQUIRE_SIMULATION_STAGE(stage,msg) \
  if (OPENFLUID_GetCurrentStage() != (stage)) \
  { \
    openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION); \
    throw openfluid::base::FrameworkException(Context,msg); \
  }

/**
  @internal
*/
#define REQUIRE_SIMULATION_STAGE_GE(stage,msg) \
  if (OPENFLUID_GetCurrentStage() < (stage)) \
  { \
    openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION); \
    throw openfluid::base::FrameworkException(Context,msg); \
  }

/**
  @internal
*/
#define REQUIRE_SIMULATION_STAGE_LE(stage,msg) \
  if (OPENFLUID_GetCurrentStage() > (stage)) \
  { \
    openfluid::base::ExceptionContext Context = computeFrameworkContext(OPENFLUID_CODE_LOCATION); \
    throw openfluid::base::FrameworkException(Context,msg); \
  }


// =====================================================================
// =====================================================================


/**
  @internal
*/
#define _STREAMTOSTRING(_stream) ((static_cast<std::ostringstream&>(std::ostringstream().flush() << _stream)).str())


// Log macros for warnings

/**
  Adds a warning message to simulation log file

  Exemple:
  @code{.cpp}
    OPENFLUID_LogWarning("This is a logged warning message for " << "TestUnits#" << TU->getID());
  @endcode
*/
#define OPENFLUID_LogWarning(_stream) \
  appendToLog(openfluid::tools::FileLogger::LOG_WARNING,_STREAMTOSTRING(_stream))

/**
  Displays a warning message to stdout (on screen by default)

  Exemple:
  @code{.cpp}
    OPENFLUID_DisplayWarning("This is a displayed warning message for " << "TestUnits#" << TU->getID());
  @endcode
*/
#define OPENFLUID_DisplayWarning(_stream) \
  displayToConsole(openfluid::tools::FileLogger::LOG_WARNING,_STREAMTOSTRING(_stream))

/**
  Adds a warning message to simulation log file and displays it to stdout (on screen by default)

  Exemple:
  @code{.cpp}
    OPENFLUID_LogAndDisplayWarning("This is a logged and displayed warning message for " <<
                                   "TestUnits#" << TU->getID());
  @endcode
*/
#define OPENFLUID_LogAndDisplayWarning(_stream) \
  OPENFLUID_LogWarning(_stream); \
  OPENFLUID_DisplayWarning(_stream)


// Log macros for infos

/**
  Adds an information message to simulation log file

  Exemple:
  @code{.cpp}
    OPENFLUID_LogInfo("This is a logged information message for " << "TestUnits#" << TU->getID());
  @endcode
*/
#define OPENFLUID_LogInfo(_stream) \
  appendToLog(openfluid::tools::FileLogger::LOG_INFO,_STREAMTOSTRING(_stream))

/**
  Displays an information message to stdout (on screen by default)

  Exemple:
  @code{.cpp}
    OPENFLUID_DisplayInfo("This is a displayed information message for " << "TestUnits#" << TU->getID());
  @endcode
*/
#define OPENFLUID_DisplayInfo(_stream) \
  displayToConsole(openfluid::tools::FileLogger::LOG_INFO,_STREAMTOSTRING(_stream))

/**
  Adds an information message to simulation log file and displays it to stdout (on screen by default)

  Exemple:
  @code{.cpp}
    OPENFLUID_LogAndDisplayInfo("This is a logged and displayed information message for " <<
                                "TestUnits#" << TU->getID());
  @endcode
*/
#define OPENFLUID_LogAndDisplayInfo(_stream) \
  OPENFLUID_LogInfo(_stream); \
  OPENFLUID_DisplayInfo(_stream)


// Log macros for debug

/**
  @def OPENFLUID_LogDebug
  Adds a debug message to simulation log file

  Exemple:
  @code{.cpp}
    OPENFLUID_LogDebug("This is a logged debug message for " << "TestUnits#" << TU->getID());
  @endcode
*/


/**
  @def OPENFLUID_DisplayDebug
  Displays a debug message to stdout (on screen by default)

  Exemple:
  @code{.cpp}
    OPENFLUID_DisplayDebug("This is a displayed debug message for " << "TestUnits#" << TU->getID());
  @endcode
*/

/**
  @def OPENFLUID_LogAndDisplayDebug
  Adds a debug message to simulation log file and displays it to stdout (on screen by default)

  Exemple:
  @code{.cpp}
    OPENFLUID_LogAndDisplayDebug("This is a logged and displayed debug message for " << "TestUnits#" << TU->getID());
  @endcode
*/

#ifndef NDEBUG

#define OPENFLUID_LogDebug(_stream) \
  appendToLog(openfluid::tools::FileLogger::LOG_DEBUG,_STREAMTOSTRING(_stream))

#define OPENFLUID_DisplayDebug(_stream) \
  displayToConsole(openfluid::tools::FileLogger::LOG_DEBUG,_STREAMTOSTRING(_stream))

#define OPENFLUID_LogAndDisplayDebug(_stream) \
  OPENFLUID_LogDebug(_stream); \
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


  protected:


    virtual bool isLinked() const
    { return (PluggableWare::isLinked() &&  mp_SimLogger != nullptr && mp_SimStatus != nullptr); };

    void appendToLog(openfluid::tools::FileLogger::LogType LType, const std::string& Msg) const;

    void displayToConsole(openfluid::tools::FileLogger::LogType LType, const std::string& Msg) const;


    openfluid::base::ExceptionContext computeWareContext(const std::string& CodeLoc = "") const;

    openfluid::base::ExceptionContext computeFrameworkContext(const std::string& CodeLoc = "") const;


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

    std::string OPENFLUID_GetCurrentStageAsString() const;

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
       @deprecated Since version 2.1.0.
       Use openfluid::ware::SimulationDrivenWare::OPENFLUID_RaiseWarning(const std::string&)
       or #OPENFLUID_LogWarning instead
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
      @deprecated Since version 2.1.0.
      Use openfluid::ware::SimulationDrivenWare::OPENFLUID_RaiseError(const std::string&) instead
    */
    virtual void OPENFLUID_RaiseError(const std::string& Source, const std::string& Msg) OPENFLUID_DEPRECATED;

    SimulationDrivenWare(WareType WType) : PluggableWare(WType),
        mp_SimStatus(nullptr), mp_SimLogger(nullptr), m_PreviousTimeIndex(0) { };


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
