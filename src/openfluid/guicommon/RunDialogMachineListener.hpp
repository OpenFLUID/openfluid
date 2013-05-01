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

#include <glibmm/i18n.h>

#include <gtkmm/textview.h>
#include <gtkmm/main.h>

#include <openfluid/dllexport.hpp>
#include <openfluid/guicommon/RunStatusWidget.hpp>


namespace openfluid { namespace guicommon {

// =====================================================================
// =====================================================================


class DLLEXPORT RunDialogMachineListener : public openfluid::machine::MachineListener
{
  private:
    unsigned int m_CurrentIndex;
    std::string m_CurrentIndexStr;
    unsigned int m_CurrentSimulator;

    unsigned int m_TotalTime;
    unsigned int m_TotalSimulators;

    RunStatusWidget* mp_RunStatusWidget;

    Gtk::TextView* mp_DetailsTextView;
    Glib::RefPtr<Gtk::TextBuffer> m_RefDetailsTextBuffer;

    unsigned int m_CurrentPreSim;
    unsigned int m_CurrentInit;
    unsigned int m_CurrentFinal;
    unsigned int m_TotalTotal;


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


    inline void refreshWidgets()
    {
      while(Gtk::Main::events_pending()) Gtk::Main::iteration();
    }


    inline void appendToTextBuffer(const std::string& Str)
    {
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), Str);
      mp_DetailsTextView->scroll_to_mark(m_RefDetailsTextBuffer->get_insert(),0);
      refreshWidgets();
    }


    inline double computeCurrentFraction()
    {
      return (double(m_CurrentPreSim + m_CurrentInit + (m_CurrentIndex+1) + m_CurrentFinal)/double(m_TotalTotal));
    }


    inline void updateProgressBar()
    {
      mp_RunStatusWidget->setProgressFraction(computeCurrentFraction());
      refreshWidgets();
    }




  public:

    RunDialogMachineListener() {};

    ~RunDialogMachineListener() {};

    void setWidgets(RunStatusWidget* RStatusWidget,
                    Gtk::TextView* DetailsTextView)
    {
      mp_RunStatusWidget = RStatusWidget;
      mp_DetailsTextView = DetailsTextView;
      m_RefDetailsTextBuffer = mp_DetailsTextView->get_buffer();
    };


    void setInfos(const unsigned int& TotalSimulators, const unsigned int& TotalTime)
    {
      m_TotalSimulators = TotalSimulators;

      m_TotalTime = TotalTime;

      m_TotalTotal = 3 + m_TotalSimulators + m_TotalTime + m_TotalSimulators;
      m_CurrentPreSim = 0;
      m_CurrentInit = 0;
      m_CurrentIndex = 0;
      m_CurrentFinal = 0;
    }


    void onInitParams()
    {
      mp_RunStatusWidget->setPresimRunning();
    };

    virtual void onSimulatorInitParams(const std::string& SimulatorID)
    {
      appendToTextBuffer("(initParams) " + SimulatorID);
    };

    void onSimulatorInitParamsDone(const openfluid::base::Listener::Status& Status,
                                  const std::string& /*SimulatorID*/)
    {
      appendToTextBuffer("  " + getStatusStr(Status) + "\n");
    };


    virtual void onInitParamsDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      m_CurrentPreSim = 1;
      updateProgressBar();
    };



    void onSimulatorPrepareData(const std::string& SimulatorID)
    {
      appendToTextBuffer("(prepareData) " + SimulatorID);

    };

    void onSimulatorPrepareDataDone(const openfluid::base::Listener::Status& Status,
                                          const std::string& /*SimulatorID*/)
    {
      appendToTextBuffer("  " + getStatusStr(Status) +"\n");
    };


    virtual void onPrepareDataDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      m_CurrentPreSim = 2;
      updateProgressBar();

    };


    virtual void onSimulatorCheckConsistency(const std::string& SimulatorID)
    {
      appendToTextBuffer("(checkConsistency) " + SimulatorID);
    };

    void onSimulatorCheckConsistencyDone(const openfluid::base::Listener::Status& Status,
                                          const std::string& /*SimulatorID*/)
    {
      appendToTextBuffer("  " + getStatusStr(Status) +"\n");
    };


    void onCheckConsistencyDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      m_CurrentPreSim = 3;
      updateProgressBar();
      mp_RunStatusWidget->setPresimDone();
    };


    void onInitializeRun()
    {
      m_CurrentInit = 0;
      mp_RunStatusWidget->setInitRunning();
      refreshWidgets();
    };

    void onSimulatorInitializeRun(const std::string& SimulatorID)
    {
      appendToTextBuffer("(initializeRun) " + SimulatorID);
    }

    void onSimulatorInitializeRunDone(const openfluid::base::Listener::Status& Status,
                                          const std::string& /*SimulatorID*/)
    {
      m_CurrentInit++;
      appendToTextBuffer("  " + getStatusStr(Status) +"\n");
      updateProgressBar();
    };


    void onInitializeRunDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      mp_RunStatusWidget->setInitDone();
      refreshWidgets();
    };


    void onBeforeRunSteps()
    {
      m_CurrentIndex = 0;
      openfluid::tools::ConvertValue(m_CurrentIndex,&m_CurrentIndexStr);
      mp_RunStatusWidget->setRunstepRunning();
      updateProgressBar();
    };

    void onRunStep(const openfluid::base::SimulationStatus* SimStatus)
    {
      m_CurrentIndex = SimStatus->getCurrentTimeIndex();
      openfluid::tools::ConvertValue(m_CurrentIndex,&m_CurrentIndexStr);
      updateProgressBar();
    };

    void onSimulatorRunStep(const std::string& SimulatorID)
    {
      appendToTextBuffer("(runStep "+m_CurrentIndexStr+") " + SimulatorID);
    };


    void onSimulatorRunStepDone(const openfluid::base::Listener::Status& Status,
                               const std::string& /*SimulatorID*/)
    {
      appendToTextBuffer("  " + getStatusStr(Status) +"\n");
    };


    void onRunStepDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      updateProgressBar();
    };


    void onAfterRunSteps()
    {
      refreshWidgets();
    };

    void onFinalizeRun()
    {
      m_CurrentFinal = 0;
      mp_RunStatusWidget->setRunstepDone();
      mp_RunStatusWidget->setFinalRunning();
      updateProgressBar();
    };

    void onSimulatorFinalizeRun(const std::string& SimulatorID)
    {
      appendToTextBuffer("(finalizeRun) " + SimulatorID);
    }


    virtual void onSimulatorFinalizeRunDone(const openfluid::base::Listener::Status& Status,
                                          const std::string& /*SimulatorID*/)
    {
      m_CurrentFinal++;
      appendToTextBuffer("  " + getStatusStr(Status) +"\n");
      updateProgressBar();
    };

    virtual void onFinalizeRunDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      mp_RunStatusWidget->setProgressFraction(1.0);
      mp_RunStatusWidget->setFinalDone();
      updateProgressBar();
    };

};

} } //namespaces



#endif /* __RUNDIALOGMACHINELISTENER_HPP__ */
