/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/

/**
  \file SimulationDrivenWare.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __SIMULATIONDRIVENWARE_HPP__
#define __SIMULATIONDRIVENWARE_HPP__


#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/base/SimulationLogger.hpp>
#include <openfluid/base/StdoutFileOStream.hpp>
#include <openfluid/ware/WareException.hpp>
#include <openfluid/base/SimulationStatus.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/dllexport.hpp>


#define REQUIRE_SIMULATION_STAGE(stage,sender,msg) \
  if (OPENFLUID_GetCurrentStage() != (stage)) \
    throw openfluid::base::FrameworkException(sender,msg);

#define REQUIRE_SIMULATION_STAGE_GE(stage,sender,msg) \
  if (OPENFLUID_GetCurrentStage() < (stage)) \
    throw openfluid::base::FrameworkException(sender,msg);

#define REQUIRE_SIMULATION_STAGE_LE(stage,sender,msg) \
  if (OPENFLUID_GetCurrentStage() > (stage)) \
    throw openfluid::base::FrameworkException(sender,msg);


namespace openfluid { namespace ware {


class DLLEXPORT SimulationDrivenWare : public PluggableWare
{
  private:

    const openfluid::base::SimulationStatus* mp_SimStatus;

    openfluid::core::TimeIndex_t m_PreviousTimeIndex;


  protected:


    /**
      Pointer to the execution messages repository
     */
    openfluid::base::SimulationLogger* mp_SimLogger;


    virtual bool isLinked() const { return (PluggableWare::isLinked() &&  mp_SimLogger != NULL && mp_SimStatus != NULL); };

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
      Raises a time-marked warning message to the kernel. This do not stops the simulation
      @param[in] Msg the content of the message
    */
    virtual void OPENFLUID_RaiseWarning(const std::string& Msg);

    /**
      Raises a time-marked warning message to the kernel. This do not stops the simulation
      @param[in] Source the source of the message
      @param[in] Msg the content of the message
    */
    virtual void OPENFLUID_RaiseWarning(const std::string& Source, const std::string& Msg);

    /**
      Raises an error message to the kernel. This stops the simulation the next time the kernel has the control
      @param[in] Msg the content of the message
    */
    virtual void OPENFLUID_RaiseError(const std::string& Msg);

    /**
      Raises an error message to the kernel. This stops the simulation the next time the kernel has the control
      @param[in] Source the source of the message
      @param[in] Msg the content of the message
    */
    virtual void OPENFLUID_RaiseError(const std::string& Source, const std::string& Msg);

    openfluid::base::StdoutAndFileOutputStream OPENFLUID_Logger;

    SimulationDrivenWare(WareType WType) : PluggableWare(WType),
        mp_SimStatus(NULL), m_PreviousTimeIndex(0), mp_SimLogger(NULL) { };


  public:

    virtual ~SimulationDrivenWare()
    { };

    void linkToSimulation(const openfluid::base::SimulationStatus* SimStatus);

    void linkToSimulationLogger(openfluid::base::SimulationLogger* SimLogger)
    {
      mp_SimLogger = SimLogger;
    };

    void initializeWare(const WareID_t& ID);

    void finalizeWare();

    void setPreviousTimeIndex(const openfluid::core::TimeIndex_t& TimeIndex) { m_PreviousTimeIndex = TimeIndex; };

};



} } // openfluid::ware



#endif /* __SIMULATIONDRIVENWARE_HPP__ */
