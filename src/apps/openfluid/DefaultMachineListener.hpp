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
  @file DefaultMachineListener.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_CMDLINEAPP_DEFAULTMACHINELISTENER_HPP__
#define __OPENFLUID_CMDLINEAPP_DEFAULTMACHINELISTENER_HPP__


#include <openfluid/machine/MachineListener.hpp>
#include <iostream>

// =====================================================================
// =====================================================================


class DefaultMachineListener : public openfluid::machine::MachineListener
{
  private:

    void displayStatus(const openfluid::base::Listener::Status& Status)
    {
      switch (Status)
      {
        case openfluid::machine::MachineListener::LISTEN_ERROR :
          std::cout << " [Error]";
          break;
        case openfluid::machine::MachineListener::LISTEN_OK :
          std::cout << " [OK]";
          break;
        case openfluid::machine::MachineListener::LISTEN_WARNING :
          std::cout << " [Warning]";
          break;
      }

      std::cout << std::endl;
      std::cout.flush();

    };

    void displayStatusWithLeftMargin(const openfluid::base::Listener::Status& Status)
    {
      switch (Status)
      {
        case openfluid::machine::MachineListener::LISTEN_ERROR :
          std::cout << std::setw(12) << "[Error]";
          break;
        case openfluid::machine::MachineListener::LISTEN_OK :
          std::cout << std::setw(12) << "[OK]";
          break;
        case openfluid::machine::MachineListener::LISTEN_WARNING :
          std::cout << std::setw(12) << "[Warning]";
          break;
      }

      std::cout << std::endl;
      std::cout.flush();

    };

  public:

    DefaultMachineListener() {};

    ~DefaultMachineListener() {};

    virtual void onModelBuild() {};

    virtual void onModelBuildDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
    };

    virtual void onLandscapeBuild() {};

    virtual void onLandscapeBuildDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
    };

    virtual void onRunConfiguration() {};

    virtual void onRunConfigurationDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
    };


    virtual void onInitParams() {};

    virtual void onInitParamsDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
    };


    virtual void onPrepareData() {};

    virtual void onPrepareDataDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
    };


    virtual void onCheckConsistency() {};

    virtual void onCheckConsistencyDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
    };


    virtual void onInitializeRun()
    {
      std::cout << std::endl;
      std::cout << std::setw(16) << "Initialize...";
      std::cout.flush();
    };

    virtual void onInitializeRunDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
    };


    virtual void onBeforeRunSteps()
    {
      std::cout << std::endl;
      std::cout << std::setw(10) << "Progress";
      std::cout << std::setw(18) << "Real time";
      std::cout << std::setw(17) << "Status";
      std::cout << std::endl;
      std::cout << std::setw(10) << "---------";
      std::cout << std::setw(18) << "---------";
      std::cout << std::setw(17) << "------";
      std::cout << std::endl;
      std::cout.flush();
    };

    virtual void onRunStep(const openfluid::base::SimulationStatus* SimStatus)
    {
      double Percent = (double(SimStatus->getCurrentTimeIndex())/double(SimStatus->getSimulationDuration()))*100;
      std::cout << std::fixed << std::setprecision(2) << std::setw(8) << Percent << "%";
      std::cout << std::setw(25) << SimStatus->getCurrentDate().getAsISOString();
      std::cout.flush();
    };

    virtual void onRunStepDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatusWithLeftMargin(Status);
    };


    virtual void onAfterRunSteps()
    {
      std::cout << std::endl;
    };

    virtual void onFinalizeRun()
    {
      std::cout << std::setw(16) << "Finalize...";
      std::cout.flush();

    };

    virtual void onFinalizeRunDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
      std::cout << std::endl;

    };


};


#endif /* __OPENFLUID_CMDLINEAPP_DEFAULTMACHINELISTENER_HPP__ */
