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
  \file RunSimulationWorker.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/guicommon/RunSimulationWorker.hpp>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/Factory.hpp>
#include <openfluid/machine/Generator.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>


namespace openfluid { namespace guicommon {


RunSimulationListener::RunSimulationListener()
  : m_Completed(false), m_SimDuration(0)
{

};


// =====================================================================
// =====================================================================


RunSimulationListener::~RunSimulationListener()
{

};


// =====================================================================
// =====================================================================


void RunSimulationListener::setInfos(const unsigned int& TotalSimulators, const unsigned int& TotalTime)
{
  m_TotalSimulators = TotalSimulators;

  m_TotalTime = TotalTime;

  m_TotalTotal = 3 + m_TotalSimulators + m_TotalTime + m_TotalSimulators;
  m_CurrentPreSim = 0;
  m_CurrentInit = 0;
  m_CurrentIndex = 0;
  m_CurrentFinal = 0;
  m_Completed = false;
}


// =====================================================================
// =====================================================================


void RunSimulationListener::onInitParams()
{
/*  mp_RunDlg->setStage(RunSimulationDialog::RUNDLG_BEFORE);
  mp_RunDlg->repaint();
  QApplication::processEvents();*/
  emit stageChanged(RUNW_BEFORE);
};


// =====================================================================
// =====================================================================


void RunSimulationListener::onInitializeRun()
{
/*  mp_RunDlg->setStage(RunSimulationDialog::RUNDLG_INIT);
  mp_RunDlg->setProgress(25);
  mp_RunDlg->repaint();
  QApplication::processEvents();*/
  emit stageChanged(RUNW_INIT);
};


// =====================================================================
// =====================================================================


void RunSimulationListener::onBeforeRunSteps()
{
/*  mp_RunDlg->setStage(RunSimulationDialog::RUNDLG_RUN);
  mp_RunDlg->setProgress(50);
  mp_RunDlg->repaint();
  QCoreApplication::sendPostedEvents();
  QApplication::processEvents();*/
/*      m_CurrentIndex = 0;
  openfluid::tools::ConvertValue(m_CurrentIndex,&m_CurrentIndexStr);
  mp_RunStatusWidget->setRunstepRunning();
  updateProgressBar();*/
  emit stageChanged(RUNW_RUN);
  emit progressValueChanged(0);
};


// =====================================================================
// =====================================================================


void RunSimulationListener::onRunStep(const openfluid::base::SimulationStatus* SimStatus)
{
/*      m_CurrentIndex = SimStatus->getCurrentTimeIndex();
  openfluid::tools::ConvertValue(m_CurrentIndex,&m_CurrentIndexStr);
  updateProgressBar();*/
  if (m_SimDuration == 0)
  {
    m_SimDuration = SimStatus->getSimulationDuration();
    emit progressMaxChanged(m_SimDuration);
  }
  emit progressValueChanged(SimStatus->getCurrentTimeIndex());
};


// =====================================================================
// =====================================================================


void RunSimulationListener::onFinalizeRun()
{
/*  mp_RunDlg->setStage(RunSimulationDialog::RUNDLG_FINAL);
  QApplication::processEvents();*/
//  emit progressValueChanged(m_SimDuration);
};


// =====================================================================
// =====================================================================


void RunSimulationListener::onFinalizeRunDone(const openfluid::base::Listener::Status& /*Status*/)
{
  emit progressValueChanged(m_SimDuration);
  emit stageChanged(RUNW_AFTER);
};


// =====================================================================
// =====================================================================


RunSimulationWorker::RunSimulationWorker(openfluid::fluidx::FluidXDescriptor* FXDesc,
                                         RunSimulationListener* Listener)
  : mp_FXDesc(FXDesc), mp_Listener(Listener)
{

}


// =====================================================================
// =====================================================================


RunSimulationWorker::~RunSimulationWorker()
{

}


// =====================================================================
// =====================================================================


void RunSimulationWorker::run()
{

  try
  {
    openfluid::base::ProjectManager::getInstance()->updateOutputDir();

    openfluid::base::RuntimeEnvironment::getInstance()->linkToProject();

    openfluid::machine::SimulationBlob SimBlob;

    openfluid::machine::Factory::buildSimulationBlobFromDescriptors(*mp_FXDesc,
                                                                    SimBlob);

    emit periodChanged(QString(SimBlob.getSimulationStatus().getBeginDate().getAsString("%Y-%m-%d %H:%M:%S").c_str()),
                       QString(SimBlob.getSimulationStatus().getEndDate().getAsString("%Y-%m-%d %H:%M:%S").c_str()),
                       SimBlob.getSimulationStatus().getSimulationDuration());

    openfluid::machine::ModelInstance ModelInstance(SimBlob, mp_Listener);

    openfluid::machine::MonitoringInstance MonitInstance(SimBlob);

    openfluid::machine::Engine Engine(SimBlob, ModelInstance, MonitInstance,
                                      mp_Listener);

    openfluid::machine::Factory::buildModelInstanceFromDescriptor(
        mp_FXDesc->getModelDescriptor(), ModelInstance);

    openfluid::machine::Factory::buildMonitoringInstanceFromDescriptor(
        mp_FXDesc->getMonitoringDescriptor(), MonitInstance);

    openfluid::machine::Factory::fillRunEnvironmentFromDescriptor(
        mp_FXDesc->getRunDescriptor());

    SimBlob.getCoreRepository().sortUnitsByProcessOrder();

    Engine.initialize();

    Engine.initParams();
    Engine.prepareData();
    Engine.checkConsistency();
    Engine.run();
    Engine.finalize();
  }
  catch (openfluid::base::Exception& E)
  {
    emit error(QString(E.what()));
  }
  catch (std::exception& E)
  {
    emit error(QString(E.what()));
  }
  catch (...)
  {
    emit error(QString("Undetermined error"));
  }

  emit finished();
}


} }  // namespaces
