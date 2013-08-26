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
  \file RunSimulationWorker.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __RUNSIMULATIONWORKER_HPP__
#define __RUNSIMULATIONWORKER_HPP__

#include <QApplication>
#include <QtDeclarative>

#include <openfluid/machine/MachineListener.hpp>


#include <openfluid/dllexport.hpp>


namespace openfluid {

namespace fluidx {
  class FluidXDescriptor;
}


namespace guicommon {


class DLLEXPORT RunSimulationListener : public QObject, public openfluid::machine::MachineListener
{
  Q_OBJECT;

  public:

    enum Stage { RUNW_BEFORE, RUNW_PRESIM, RUNW_INIT, RUNW_RUN, RUNW_FINAL, RUNW_AFTER };

  private:
    unsigned int m_CurrentIndex;
    std::string m_CurrentIndexStr;
    unsigned int m_CurrentSimulator;

    unsigned int m_TotalTime;
    unsigned int m_TotalSimulators;

    unsigned int m_CurrentPreSim;
    unsigned int m_CurrentInit;
    unsigned int m_CurrentFinal;
    unsigned int m_TotalTotal;

    bool m_Completed;

    openfluid::core::Duration_t m_SimDuration;


  signals:

    void stageChanged(openfluid::guicommon::RunSimulationListener::Stage S);

    void progressValueChanged(int Index);

    void progressMaxChanged(int Index);


  public:

    RunSimulationListener();

    ~RunSimulationListener();


    void setInfos(const unsigned int& TotalSimulators, const unsigned int& TotalTime);


    void onInitParams();


    void onInitializeRun();


    void onBeforeRunSteps();

    void onRunStep(const openfluid::base::SimulationStatus* SimStatus);


    void onFinalizeRun();

    void onFinalizeRunDone(const openfluid::base::Listener::Status& /*Status*/);

};


// =====================================================================
// =====================================================================


class RunSimulationWorker : public QObject
{

  Q_OBJECT;

  private:

    openfluid::fluidx::FluidXDescriptor* mp_FXDesc;

    RunSimulationListener* mp_Listener;

  signals:

    void periodChanged(QString Begin, QString End, int Duration);

    void finished();

    void error(QString Error);

  public slots:

    void run();

  public:


    RunSimulationWorker(openfluid::fluidx::FluidXDescriptor* FXDesc, RunSimulationListener* Listener);

    ~RunSimulationWorker();


};

} } //namespaces


Q_DECLARE_METATYPE(openfluid::guicommon::RunSimulationListener::Stage);

#endif /* __RUNSIMULATIONWORKER_HPP__ */
