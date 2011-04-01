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
  \file RunDialogMachineListener.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __RUNDIALOGMACHINELISTENER_HPP__
#define __RUNDIALOGMACHINELISTENER_HPP__



#include <openfluid/machine/MachineListener.hpp>
#include <gtkmm.h>
#include <glibmm/i18n.h>


// =====================================================================
// =====================================================================


class RunDialogMachineListener : public openfluid::machine::MachineListener
{
  private:
    unsigned int m_CurrentStep;
    std::string m_CurrentStepStr;
    unsigned int m_CurrentFunction;

    unsigned int m_TotalSteps;
    std::string m_TotalStepsStr;
    unsigned int m_TotalFunctions;

    Gtk::ProgressBar* mp_PreSimProgressBar;
    Gtk::ProgressBar* mp_InitProgressBar;
    Gtk::ProgressBar* mp_RunProgressBar;
    Gtk::ProgressBar* mp_FinalProgressBar;
    Gtk::TextView* mp_DetailsTextView;
    Glib::RefPtr<Gtk::TextBuffer> m_RefDetailsTextBuffer;

    std::string getStatusStr(const openfluid::base::Listener::Status& Status)
    {
      switch (Status)
      {
        case openfluid::machine::MachineListener::OK :
          return _("[OK]"); break;
        case openfluid::machine::MachineListener::WARNING :
          return _("[Warning]");break;
        default : return _("[Error]"); break;

      }
    }


  public:

    RunDialogMachineListener() {};

    ~RunDialogMachineListener() {};

    void setWidgets(Gtk::ProgressBar* PreSimProgressBar, Gtk::ProgressBar* InitProgressBar,
                    Gtk::ProgressBar* RunProgressBar, Gtk::ProgressBar* FinalProgressBar,
                    Gtk::TextView* DetailsTextView)
    {
      mp_PreSimProgressBar = PreSimProgressBar;
      mp_InitProgressBar = InitProgressBar;
      mp_RunProgressBar = RunProgressBar;
      mp_FinalProgressBar = FinalProgressBar;
      mp_DetailsTextView = DetailsTextView;
      m_RefDetailsTextBuffer = mp_DetailsTextView->get_buffer();
    };


    void setFunctionsCount(const unsigned int& TotalFunctions)
    {
      m_TotalFunctions= TotalFunctions;
    }


    void setTotalStepsCount(const unsigned int& TotalSteps)
    {
      m_CurrentStep = 0;
      m_TotalSteps = TotalSteps;
      openfluid::tools::ConvertValue(m_TotalSteps,&m_TotalStepsStr);
    };


    void onInitParams()
    {
      mp_PreSimProgressBar->set_text("In progress");
    };

    virtual void onFunctionInitParams(const std::string& FunctionID)
    {
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), "<initParams> " + FunctionID);
    };

    void onFunctionInitParamsDone(const openfluid::base::Listener::Status& Status,
                                          const std::string& /*FunctionID*/)
    {
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), "  " + getStatusStr(Status) + "\n");
      mp_DetailsTextView->scroll_to_mark(m_RefDetailsTextBuffer->get_insert(), 0);
    };


    virtual void onInitParamsDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      mp_PreSimProgressBar->set_fraction(0.33);
    };



    void onFunctionPrepareData(const std::string& FunctionID)
    {
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), "<prepareData> " + FunctionID);

    };

    void onFunctionPrepareDataDone(const openfluid::base::Listener::Status& Status,
                                          const std::string& /*FunctionID*/)
    {
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), "  " + getStatusStr(Status) +"\n");
      mp_DetailsTextView->scroll_to_mark(m_RefDetailsTextBuffer->get_insert(), 0);
    };


    virtual void onPrepareDataDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      mp_PreSimProgressBar->set_fraction(0.66);
    };


    virtual void onFunctionCheckConsistency(const std::string& FunctionID)
    {
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), "<checkConsistency> " + FunctionID);
    };

    void onFunctionCheckConsistencyDone(const openfluid::base::Listener::Status& Status,
                                          const std::string& /*FunctionID*/)
    {
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), "  " + getStatusStr(Status) +"\n");
      mp_DetailsTextView->scroll_to_mark(m_RefDetailsTextBuffer->get_insert(), 0);
    };


    void onCheckConsistencyDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      mp_PreSimProgressBar->set_fraction(1.0);
      mp_PreSimProgressBar->set_text("Completed");
    };


    void onInitializeRun(const std::string& /*FunctionID*/)
    {
      m_CurrentFunction = 0;
      mp_InitProgressBar->set_text("In progress");
    };

    void onFunctionInitializeRun(const std::string& FunctionID)
    {
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), "<initializeRun> " + FunctionID);
    }

    void onFunctionInitializeRunDone(const openfluid::base::Listener::Status& Status,
                                          const std::string& /*FunctionID*/)
    {
      m_CurrentFunction++;
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), "  " + getStatusStr(Status) +"\n");
      mp_InitProgressBar->set_fraction(m_CurrentFunction/m_TotalFunctions);
      mp_DetailsTextView->scroll_to_mark(m_RefDetailsTextBuffer->get_insert(), 0);
    };


    void onInitializeRunDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      mp_InitProgressBar->set_fraction(1.0);
      mp_InitProgressBar->set_text("Completed");
      while( Gtk::Main::events_pending()) Gtk::Main::iteration();

    };


    void onBeforeRunSteps()
    {
      m_CurrentStep = 0;
      mp_RunProgressBar->set_text("0/"+m_TotalStepsStr);
      while( Gtk::Main::events_pending()) Gtk::Main::iteration();
    };

    void onRunStep(const openfluid::base::SimulationStatus* /*SimStatus*/)
    {
      openfluid::tools::ConvertValue(m_CurrentStep,&m_CurrentStepStr);
    };

    void onFunctionRunStep(const std::string& FunctionID)
    {
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), "<runStep("+m_CurrentStepStr+")> " + FunctionID);
    };


    void onFunctionRunStepDone(const openfluid::base::Listener::Status& Status,
                               const std::string& /*FunctionID*/)
    {
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), "  " + getStatusStr(Status) +"\n");
      mp_DetailsTextView->scroll_to_mark(m_RefDetailsTextBuffer->get_insert(), 0);
    };


    void onRunStepDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      m_CurrentStep++;

      mp_RunProgressBar->set_fraction(double(m_CurrentStep)/double(m_TotalSteps));
      mp_RunProgressBar->set_text("Step "+m_CurrentStepStr+"/"+m_TotalStepsStr);
    };


    void onAfterRunSteps()
    {
      mp_RunProgressBar->set_fraction(1.0);
    };

    void onFinalizeRun(const std::string& /*FunctionID*/)
    {
      m_CurrentFunction = 0;
      mp_FinalProgressBar->set_text("In progress");
    };

    void onFunctionFinalizeRun(const std::string& FunctionID)
    {
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), "<finalizeRun> " + FunctionID);
    }


    virtual void onFunctionFinalizeRunDone(const openfluid::base::Listener::Status& Status,
                                          const std::string& /*FunctionID*/)
    {
      m_CurrentFunction++;
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), "  " + getStatusStr(Status) +"\n");
      mp_FinalProgressBar->set_fraction(m_CurrentFunction/m_TotalFunctions);
      mp_DetailsTextView->scroll_to_mark(m_RefDetailsTextBuffer->get_insert(), 0);
    };

    virtual void onFinalizeRunDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      mp_FinalProgressBar->set_fraction(1.0);
      mp_FinalProgressBar->set_text("Completed");
    };

};




#endif /* __RUNDIALOGMACHINELISTENER_HPP__ */
