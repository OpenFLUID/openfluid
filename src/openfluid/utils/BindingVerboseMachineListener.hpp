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
  @file BindingVerboseMachineListener.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UTILS_BINDINGVERBOSEMACHINELISTENER_HPP__
#define __OPENFLUID_UTILS_BINDINGVERBOSEMACHINELISTENER_HPP__


#include <openfluid/machine/MachineListener.hpp>
#include <openfluid/utils/BindingAbstractOutErr.hpp>


namespace openfluid { namespace utils {


class BindingVerboseMachineListener : public openfluid::machine::MachineListener
{
  private:

    const openfluid::utils::BindingAbstractOutErr* mp_OutErr;

    void displayStatusWithLeftMargin(const openfluid::base::Listener::Status& Status)
    {
      switch (Status)
      {
        case openfluid::base::Listener::LISTEN_ERROR :
          mp_OutErr->printfOut("%14s\n","[Error]");
          break;
        case openfluid::base::Listener::LISTEN_OK :
          mp_OutErr->printfOut("%14s\n","[OK]");
          break;
        case openfluid::base::Listener::LISTEN_WARNING :
          mp_OutErr->printfOut("%14s\n","[Warning]");
          break;
      }
    };

  public:

    BindingVerboseMachineListener(const openfluid::utils::BindingAbstractOutErr* OutErr):
      openfluid::machine::MachineListener(), mp_OutErr(OutErr)
    { }

    ~BindingVerboseMachineListener()
    { }


    void displayStatus(const openfluid::base::Listener::Status& Status)
    {
      switch (Status)
      {
        case openfluid::base::Listener::LISTEN_ERROR :
          mp_OutErr->printfOut(" [Error]\n");
          break;
        case openfluid::base::Listener::LISTEN_OK :
          mp_OutErr->printfOut(" [OK]\n");
          break;
        case openfluid::base::Listener::LISTEN_WARNING :
          mp_OutErr->printfOut(" [Warning]\n");
          break;
      }
    }


    void onInitParams()
    {
      mp_OutErr->printfOut("%s","Initializing parameters...");
    }

    void onInitParamsDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
    }


    void onPrepareData()
    {
      mp_OutErr->printfOut("%s","Preparing data...");
    }

    void onPrepareDataDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
    }


    void onCheckConsistency()
    {
      mp_OutErr->printfOut("%s","Checking consistency...");
    }

    void onCheckConsistencyDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
    }


    void onInitializeRun()
    {
      mp_OutErr->printfOut("%s","\nInitializing simulation...");
    }

    void onInitializeRunDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
    }


    void onBeforeRunSteps()
    {
      mp_OutErr->printfOut("   Progress            Real time              Status\n");
      mp_OutErr->printfOut("   --------            ---------              ------\n");
    }

    void onRunStep(const openfluid::base::SimulationStatus* SimStatus)
    {
      double Percent = (double(SimStatus->getCurrentTimeIndex())/double(SimStatus->getSimulationDuration()))*100;
      mp_OutErr->printfOut("  %7.2f%%%27s",Percent,SimStatus->getCurrentDate().getAsISOString().c_str());
    }

    void onRunStepDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatusWithLeftMargin(Status);
    }


    void onAfterRunSteps()
    { }

    void onFinalizeRun()
    {
      mp_OutErr->printfOut("%s","Finalizing simulation...");
    }

    void onFinalizeRunDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
    }

};


} } // namespaces


#endif /* __OPENFLUID_UTILS_BINDINGVERBOSEMACHINELISTENER_HPP__ */
