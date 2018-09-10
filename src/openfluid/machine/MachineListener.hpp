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
  @file MachineListener.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_MACHINE_MACHINELISTENER_HPP__
#define __OPENFLUID_MACHINE_MACHINELISTENER_HPP__


#include <string>

#include <openfluid/dllexport.hpp>
#include <openfluid/base/Listener.hpp>
#include <openfluid/base/SimulationStatus.hpp>


namespace openfluid { namespace machine {


class OPENFLUID_API MachineListener : public openfluid::base::Listener
{
  private:


  public:

    MachineListener() {};

    virtual ~MachineListener() {};

    virtual void onInitParams() {};

    virtual void onInitParamsDone(const openfluid::base::Listener::Status& /*Status*/) {};

    virtual void onSimulatorInitParams(const std::string& /*SimulatorID*/) {};

    virtual void onSimulatorInitParamsDone(const openfluid::base::Listener::Status& /*Status*/,
                                          const std::string& /*SimulatorID*/) {};

    virtual void onPrepareData() {};

    virtual void onPrepareDataDone(const openfluid::base::Listener::Status& /*Status*/) {};

    virtual void onSimulatorPrepareData(const std::string& /*SimulatorID*/) {};

    virtual void onSimulatorPrepareDataDone(const openfluid::base::Listener::Status& /*Status*/,
                                          const std::string& /*SimulatorID*/) {};

    virtual void onCheckConsistency() {};

    virtual void onCheckConsistencyDone(const openfluid::base::Listener::Status& /*Status*/) {};

    virtual void onSimulatorCheckConsistency(const std::string& /*SimulatorID*/) {};

    virtual void onSimulatorCheckConsistencyDone(const openfluid::base::Listener::Status& /*Status*/,
                                          const std::string& /*SimulatorID*/) {};

    virtual void onInitializeRun() {};

    virtual void onInitializeRunDone(const openfluid::base::Listener::Status& /*Status*/) {};

    virtual void onSimulatorInitializeRun(const std::string& /*SimulatorID*/) {};

    virtual void onSimulatorInitializeRunDone(const openfluid::base::Listener::Status& /*Status*/,
                                          const std::string& /*SimulatorID*/) {};

    virtual void onBeforeRunSteps() {};

    virtual void onRunStep(const openfluid::base::SimulationStatus* /*SimStatus*/) {};

    virtual void onRunStepDone(const openfluid::base::Listener::Status& /*Status*/) {};

    virtual void onSimulatorRunStep(const std::string& /*SimulatorID*/) {};

    virtual void onSimulatorRunStepDone(const openfluid::base::Listener::Status& /*Status*/,
                                       const std::string& /*SimulatorID*/) {};

    virtual void onAfterRunSteps() {};

    virtual void onFinalizeRun() {};

    virtual void onFinalizeRunDone(const openfluid::base::Listener::Status& /*Status*/) {};

    virtual void onSimulatorFinalizeRun(const std::string& /*SimulatorID*/) {};

    virtual void onSimulatorFinalizeRunDone(const openfluid::base::Listener::Status& /*Status*/,
                                          const std::string& /*SimulatorID*/) {};

};


} } //namespaces


#endif /* __OPENFLUID_MACHINE_MACHINELISTENER_HPP__ */
