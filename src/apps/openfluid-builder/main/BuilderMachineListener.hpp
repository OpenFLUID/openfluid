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


#ifndef __BUILDERMACHINELISTENER_HPP__
#define __BUILDERMACHINELISTENER_HPP__


#include <iostream>

#include <openfluid/machine/MachineListener.hpp>


// =====================================================================
// =====================================================================


class BuilderMachineListener : public openfluid::machine::MachineListener
{
  private:

  Gtk::TextView * mp_TextView;
  Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer;
  std::ostringstream ss;

    void display()
    {
      if(mp_TextView && m_refTextBuffer)
      {
        m_refTextBuffer->insert(m_refTextBuffer->end() ,ss.str());

        while( Gtk::Main::events_pending() )
          Gtk::Main::iteration();

        mp_TextView->scroll_to_mark(m_refTextBuffer->get_insert(),0);
      }
      else
        std::cout << ss.str();

      ss.str("");
    };

    void displayStatus(const openfluid::base::Listener::Status& Status)
    {
      switch (Status)
      {
        case openfluid::machine::MachineListener::ERROR :
          ss << " [Error]";
          break;
        case openfluid::machine::MachineListener::OK :
          ss << " [OK]";
          break;
        case openfluid::machine::MachineListener::WARNING :
          ss << " [Warning]";
          break;
      }

      ss << std::endl;

      display();

    };

    void displayStatusWithLeftMargin(const openfluid::base::Listener::Status& Status)
    {
      switch (Status)
      {
        case openfluid::machine::MachineListener::ERROR :
          ss << std::setw(12) << "[Error]";
          break;
        case openfluid::machine::MachineListener::OK :
          ss << std::setw(12) << "[OK]";
          break;
        case openfluid::machine::MachineListener::WARNING :
          ss << std::setw(12) << "[Warning]";
          break;
      }

      ss << std::endl;

      display();
    };



  public:

    BuilderMachineListener() { mp_TextView = 0; };

    ~BuilderMachineListener() {};

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
      ss << std::endl;
      ss << std::setw(16) << "Initialize...";

      display();
    };

    virtual void onInitializeRunDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);
    };


    virtual void onBeforeRunSteps()
    {
      ss << std::endl;
      ss << std::setw(10) << "Time step";
      ss << std::setw(18) << "Real time";
      ss << std::setw(17) << "Status";
      ss << std::endl;
      ss << std::setw(10) << "---------";
      ss << std::setw(18) << "---------";
      ss << std::setw(17) << "------";
      ss << std::endl;

      display();
    };

    virtual void onRunStep(const openfluid::base::SimulationStatus* SimStatus)
    {
      ss << std::setw(8) << SimStatus->getCurrentStep();
      ss << std::setw(25) << SimStatus->getCurrentTime().getAsISOString();

      display();
    };

    virtual void onRunStepDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatusWithLeftMargin(Status);
    };


    virtual void onAfterRunSteps()
    {
      ss << std::endl;

      display();
    };

    virtual void onFinalizeRun()
    {
      ss << std::setw(16) << "Finalize...";

      display();
    };

    virtual void onFinalizeRunDone(const openfluid::base::Listener::Status& Status)
    {
      displayStatus(Status);

      ss << std::endl;

      display();
    };

    void setTextView(Gtk::TextView * TextView)
    {
      mp_TextView = TextView;
      m_refTextBuffer = TextView->get_buffer();
    };

    void displayText(std::ostringstream & ss_input)
    {
      ss.str(ss_input.str());
      display();
    }


};


#endif /* __BUILDERMACHINELISTENER_HPP__ */
