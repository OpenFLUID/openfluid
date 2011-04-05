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

#include "RunStatusWidget.hpp"


// =====================================================================
// =====================================================================


class RunDialogMachineListener : public openfluid::machine::MachineListener
{
  private:
    unsigned int m_CurrentStep;
    std::string m_CurrentStepStr;
    unsigned int m_CurrentFunction;

    unsigned int m_TotalSteps;
    unsigned int m_TotalFunctions;

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


    void appendToTextBuffer(const std::string& Str)
    {
      m_RefDetailsTextBuffer->insert(m_RefDetailsTextBuffer->end(), Str);
//      mp_DetailsTextView->scroll_to_mark(m_RefDetailsTextBuffer->get_insert(),0);
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


    void setInfos(const unsigned int& TotalFunctions, const unsigned int& TotalSteps)
    {
      m_TotalFunctions = TotalFunctions;

      m_TotalSteps = TotalSteps;

      m_TotalTotal = 3 + m_TotalFunctions + m_TotalSteps + m_TotalFunctions;
      m_CurrentPreSim = 0;
      m_CurrentInit = 0;
      m_CurrentStep = 0;
      m_CurrentFinal = 0;
    }

    double computeCurrentFraction()
    {
      return (double(m_CurrentPreSim + m_CurrentInit + (m_CurrentStep+1) + m_CurrentFinal)/double(m_TotalTotal));
    }


    void onInitParams()
    {
      mp_RunStatusWidget->setPresimRunning();
    };

    virtual void onFunctionInitParams(const std::string& FunctionID)
    {
      appendToTextBuffer("<initParams> " + FunctionID);
    };

    void onFunctionInitParamsDone(const openfluid::base::Listener::Status& Status,
                                  const std::string& /*FunctionID*/)
    {
      appendToTextBuffer("  " + getStatusStr(Status) + "\n");
    };


    virtual void onInitParamsDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      m_CurrentPreSim = 1;
      mp_RunStatusWidget->setProgressFraction(computeCurrentFraction());
    };



    void onFunctionPrepareData(const std::string& FunctionID)
    {
      appendToTextBuffer("<prepareData> " + FunctionID);

    };

    void onFunctionPrepareDataDone(const openfluid::base::Listener::Status& Status,
                                          const std::string& /*FunctionID*/)
    {
      appendToTextBuffer("  " + getStatusStr(Status) +"\n");
    };


    virtual void onPrepareDataDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      m_CurrentPreSim = 2;
      mp_RunStatusWidget->setProgressFraction(computeCurrentFraction());

    };


    virtual void onFunctionCheckConsistency(const std::string& FunctionID)
    {
      appendToTextBuffer("<checkConsistency> " + FunctionID);
    };

    void onFunctionCheckConsistencyDone(const openfluid::base::Listener::Status& Status,
                                          const std::string& /*FunctionID*/)
    {
      appendToTextBuffer("  " + getStatusStr(Status) +"\n");
    };


    void onCheckConsistencyDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      m_CurrentPreSim = 3;
      mp_RunStatusWidget->setProgressFraction(computeCurrentFraction());
      mp_RunStatusWidget->setPresimDone();
    };


    void onInitializeRun()
    {
      m_CurrentInit = 0;
      mp_RunStatusWidget->setInitRunning();
    };

    void onFunctionInitializeRun(const std::string& FunctionID)
    {
      appendToTextBuffer("<initializeRun> " + FunctionID);
    }

    void onFunctionInitializeRunDone(const openfluid::base::Listener::Status& Status,
                                          const std::string& /*FunctionID*/)
    {
      m_CurrentInit++;
      appendToTextBuffer("  " + getStatusStr(Status) +"\n");
      mp_RunStatusWidget->setProgressFraction(computeCurrentFraction());
    };


    void onInitializeRunDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      mp_RunStatusWidget->setInitDone();
    };


    void onBeforeRunSteps()
    {
      m_CurrentStep = 0;
      openfluid::tools::ConvertValue(m_CurrentStep,&m_CurrentStepStr);
      mp_RunStatusWidget->setProgressFraction(computeCurrentFraction());
      mp_RunStatusWidget->setRunstepRunning();
    };

    void onRunStep(const openfluid::base::SimulationStatus* SimStatus)
    {
      m_CurrentStep = SimStatus->getCurrentStep();
      openfluid::tools::ConvertValue(m_CurrentStep,&m_CurrentStepStr);
    };

    void onFunctionRunStep(const std::string& FunctionID)
    {
      appendToTextBuffer("<runStep("+m_CurrentStepStr+")> " + FunctionID);
    };


    void onFunctionRunStepDone(const openfluid::base::Listener::Status& Status,
                               const std::string& /*FunctionID*/)
    {
      appendToTextBuffer("  " + getStatusStr(Status) +"\n");
    };


    void onRunStepDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      mp_RunStatusWidget->setProgressFraction(computeCurrentFraction());
      mp_RunStatusWidget->updateCurrentStep(m_CurrentStepStr);
      mp_RunStatusWidget->setRunstepRunning();
    };


    void onAfterRunSteps()
    {

    };

    void onFinalizeRun()
    {
      m_CurrentFinal = 0;
      mp_RunStatusWidget->setRunstepDone();
      mp_RunStatusWidget->setFinalRunning();
    };

    void onFunctionFinalizeRun(const std::string& FunctionID)
    {
      appendToTextBuffer("<finalizeRun> " + FunctionID);
    }


    virtual void onFunctionFinalizeRunDone(const openfluid::base::Listener::Status& Status,
                                          const std::string& /*FunctionID*/)
    {
      m_CurrentFinal++;
      appendToTextBuffer("  " + getStatusStr(Status) +"\n");
      mp_RunStatusWidget->setProgressFraction(computeCurrentFraction());
    };

    virtual void onFinalizeRunDone(const openfluid::base::Listener::Status& /*Status*/)
    {
      mp_RunStatusWidget->setProgressFraction(1.0);
      mp_RunStatusWidget->setFinalDone();
    };

};




#endif /* __RUNDIALOGMACHINELISTENER_HPP__ */
