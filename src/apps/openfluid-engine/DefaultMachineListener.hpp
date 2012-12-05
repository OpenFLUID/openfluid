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
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __DEFAULTMACHINELISTENER_HPP__
#define __DEFAULTMACHINELISTENER_HPP__


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
        case openfluid::machine::MachineListener::ERROR :
          std::cout << " [Error]";
          break;
        case openfluid::machine::MachineListener::OK :
          std::cout << " [OK]";
          break;
        case openfluid::machine::MachineListener::WARNING :
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
        case openfluid::machine::MachineListener::ERROR :
          std::cout << std::setw(12) << "[Error]";
          break;
        case openfluid::machine::MachineListener::OK :
          std::cout << std::setw(12) << "[OK]";
          break;
        case openfluid::machine::MachineListener::WARNING :
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
      std::cout << std::setw(10) << "Time step";
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
      std::cout << std::setw(8) << (SimStatus->getCurrentTimeIndex() / SimStatus->getDefaultDeltaT());
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


#endif /* __DEFAULTMACHINELISTENER_HPP__ */
