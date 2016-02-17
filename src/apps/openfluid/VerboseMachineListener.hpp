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
  @file VerboseMachineListener.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_CMDLINEAPP_VERBOSEMACHINELISTENER_HPP__
#define __OPENFLUID_CMDLINEAPP_VERBOSEMACHINELISTENER_HPP__


#include "DefaultMachineListener.hpp"
#include <iostream>

// =====================================================================
// =====================================================================


class VerboseMachineListener : public DefaultMachineListener
{
  private:

    void displaySimulatorStatus(const openfluid::base::Listener::Status& Status)
    {
      switch (Status)
      {
        case openfluid::machine::MachineListener::LISTEN_ERROR :
          openfluid::tools::Console::setErrorColor();
          std::cout << std::setw(12) << "[Error]";
          openfluid::tools::Console::resetAttributes();
          break;
        case openfluid::machine::MachineListener::LISTEN_OK :
          openfluid::tools::Console::setOKColor();
          std::cout << std::setw(12) << "[OK]";
          openfluid::tools::Console::resetAttributes();
          break;
        case openfluid::machine::MachineListener::LISTEN_WARNING :
          openfluid::tools::Console::setWarningColor();
          std::cout << std::setw(12) << "[Warning]";
          openfluid::tools::Console::resetAttributes();
          break;
      }

      std::cout << std::endl;
      std::cout.flush();

    };

  public:

    VerboseMachineListener() {};

    ~VerboseMachineListener() {};


    virtual void onSimulatorInitParams(const std::string& SimulatorID)
    {
      std::cout << std::endl << std::setw(50) << SimulatorID;
      std::cout.flush();
    };

    virtual void onSimulatorInitParamsDone(const openfluid::base::Listener::Status& Status,
                                            const std::string& /*SimulatorID*/)
    {
      displaySimulatorStatus(Status);
    };

    virtual void onInitParamsDone(const openfluid::base::Listener::Status& /*Status*/) {};

    virtual void onSimulatorPrepareData(const std::string& SimulatorID)
    {
      std::cout << std::endl << std::setw(50) << SimulatorID;
      std::cout.flush();
    };


    virtual void onSimulatorPrepareDataDone(const openfluid::base::Listener::Status& Status,
                                            const std::string& /*SimulatorID*/)
    {
      displaySimulatorStatus(Status);
    };

    virtual void onPrepareDataDone(const openfluid::base::Listener::Status& /*Status*/) {};

    virtual void onSimulatorCheckConsistency(const std::string& SimulatorID)
    {
      std::cout << std::endl << std::setw(50) << SimulatorID;
      std::cout.flush();
    };


    virtual void onSimulatorCheckConsistencyDone(const openfluid::base::Listener::Status& Status,
                                                 const std::string& /*SimulatorID*/)
    {
      displaySimulatorStatus(Status);
    };

    virtual void onCheckConsistencyDone(const openfluid::base::Listener::Status& /*Status*/) {};

    virtual void onSimulatorInitializeRun(const std::string& SimulatorID)
    {
      std::cout << std::endl << std::setw(50) << SimulatorID;
      std::cout.flush();
    };


    virtual void onSimulatorInitializeRunDone(const openfluid::base::Listener::Status& Status,
                                              const std::string& /*SimulatorID*/)
    {
      displaySimulatorStatus(Status);
    };

    virtual void onInitializeRunDone(const openfluid::base::Listener::Status& /*Status*/) {};

    virtual void onSimulatorRunStep(const std::string& SimulatorID)
    {
      std::cout << std::endl << std::setw(50) << SimulatorID;
      std::cout.flush();
    };


    virtual void onSimulatorRunStepDone(const openfluid::base::Listener::Status& Status,
                                        const std::string& /*SimulatorID*/)
    {
      displaySimulatorStatus(Status);
    };

    virtual void onRunStepDone(const openfluid::base::Listener::Status& /*Status*/) {};

    virtual void onSimulatorFinalizeRun(const std::string& SimulatorID)
    {
      std::cout << std::endl << std::setw(50) << SimulatorID;
      std::cout.flush();
    };


    virtual void onSimulatorFinalizeRunDone(const openfluid::base::Listener::Status& Status,
                                            const std::string& /*SimulatorID*/)
    {
      displaySimulatorStatus(Status);
    };

    virtual void onFinalizeRunDone(const openfluid::base::Listener::Status& /*Status*/) {};


};


#endif /* __OPENFLUID_CMDLINEAPP_VERBOSEMACHINELISTENER_HPP__ */
