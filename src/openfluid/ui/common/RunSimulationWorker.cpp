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
  @file RunSimulationWorker.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <openfluid/ui/common/RunSimulationWorker.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/base/OtherException.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/Factory.hpp>
#include <openfluid/machine/Generator.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>


// =====================================================================
// =====================================================================


#define HANDLE_USER_PAUSE_ABORT \
  { \
    while (m_PausedByUser) \
    { \
      if (!m_ConfirmedPauseByUser) \
      { \
        m_ConfirmedPauseByUser = true; \
        emit pauseConfirmed(); \
      } \
      if (m_AbortedByUser) \
      { \
        throw UserAbortException(); \
      } \
      openfluid::tools::millisleep(200); \
    } \
  }


// =====================================================================
// =====================================================================


namespace openfluid { namespace ui { namespace common {


class UserAbortException : public openfluid::base::Exception
{
  protected:

    void buildFullMessage()
    {
      m_FullMessage = m_Message;
    }


  public:

    UserAbortException() :
      Exception("Aborted by user")
    {
      buildFullMessage();
    }
};


// =====================================================================
// =====================================================================


RunSimulationListener::RunSimulationListener()
  : m_CurrentIndex(0),m_CurrentSimulator(0),
    m_TotalTime(0),m_TotalSimulators(0),
    m_CurrentPreSim(0),m_CurrentInit(0),m_CurrentFinal(0),
    m_TotalTotal(0),
    m_Completed(false), m_SimDuration(0),
    m_PausedByUser(false), m_ConfirmedPauseByUser(false),
    m_AbortedByUser(false)
{

}


// =====================================================================
// =====================================================================


RunSimulationListener::~RunSimulationListener()
{

}


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


bool RunSimulationListener::isPausedByUser()
{
  return m_PausedByUser;
}


// =====================================================================
// =====================================================================


void RunSimulationListener::requestAbort()
{
  m_AbortedByUser = true;
}


// =====================================================================
// =====================================================================


void RunSimulationListener::requestSuspendResume()
{
  m_PausedByUser = !m_PausedByUser;
  m_ConfirmedPauseByUser = !m_PausedByUser;
}


// =====================================================================
// =====================================================================


void RunSimulationListener::onInitParams()
{
  HANDLE_USER_PAUSE_ABORT;

  emit stageChanged(Stage::BEFORE);
};


// =====================================================================
// =====================================================================


void RunSimulationListener::onInitializeRun()
{
  HANDLE_USER_PAUSE_ABORT;

  emit stageChanged(Stage::INIT);
};


// =====================================================================
// =====================================================================


void RunSimulationListener::onBeforeRunSteps()
{
  HANDLE_USER_PAUSE_ABORT;

  emit stageChanged(Stage::RUN);
  emit progressValueChanged(0);
};


// =====================================================================
// =====================================================================


void RunSimulationListener::onRunStep(const openfluid::base::SimulationStatus* SimStatus)
{
  HANDLE_USER_PAUSE_ABORT;

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
  HANDLE_USER_PAUSE_ABORT;
};


// =====================================================================
// =====================================================================


void RunSimulationListener::onFinalizeRunDone(const openfluid::base::Listener::Status& /*Status*/)
{
  HANDLE_USER_PAUSE_ABORT;

  emit progressValueChanged(m_SimDuration);
  emit stageChanged(Stage::AFTER);
};


// =====================================================================
// =====================================================================


void RunSimulationListener::onSimulatorInitParams(const std::string& /*SimulatorID*/)
{
  HANDLE_USER_PAUSE_ABORT;
}


// =====================================================================
// =====================================================================


void RunSimulationListener::onSimulatorPrepareData(const std::string& /*SimulatorID*/)
{
  HANDLE_USER_PAUSE_ABORT;
}


// =====================================================================
// =====================================================================


void RunSimulationListener::onSimulatorCheckConsistency(const std::string& /*SimulatorID*/)
{
  HANDLE_USER_PAUSE_ABORT;
}


// =====================================================================
// =====================================================================


void RunSimulationListener::onSimulatorInitializeRun(const std::string& /*SimulatorID*/)
{
  HANDLE_USER_PAUSE_ABORT;
}


// =====================================================================
// =====================================================================


void RunSimulationListener::onSimulatorRunStep(const std::string& /*SimulatorID*/)
{
  HANDLE_USER_PAUSE_ABORT;
}


// =====================================================================
// =====================================================================


void RunSimulationListener::onSimulatorFinalizeRun(const std::string& /*SimulatorID*/)
{
  HANDLE_USER_PAUSE_ABORT;
}


// =====================================================================
// =====================================================================


RunSimulationWorker::RunSimulationWorker(const openfluid::fluidx::FluidXDescriptor* FXDesc,
                                         RunSimulationListener* Listener)
  : mp_FXDesc(FXDesc), mp_Listener(Listener)
{
  if (!Listener)
  {
    emit error(QString("Listener can not be NULL"),openfluid::base::ExceptionContext());
  }
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
    openfluid::base::RunContextManager::instance()->updateProjectOutputDir();

    openfluid::machine::SimulationBlob SimBlob;

    openfluid::machine::Factory::buildSimulationBlobFromDescriptors(*mp_FXDesc, SimBlob);

    emit periodChanged(QString(SimBlob.simulationStatus().getBeginDate().getAsString("%Y-%m-%d %H:%M:%S").c_str()),
                       QString(SimBlob.simulationStatus().getEndDate().getAsString("%Y-%m-%d %H:%M:%S").c_str()),
                       SimBlob.simulationStatus().getSimulationDuration());

    openfluid::machine::ModelInstance ModelInstance(SimBlob, mp_Listener);

    openfluid::machine::MonitoringInstance MonitInstance(SimBlob);

    openfluid::machine::Engine Engine(SimBlob, ModelInstance, MonitInstance,
                                      mp_Listener);

    openfluid::machine::Factory::buildModelInstanceFromDescriptor(mp_FXDesc->model(),
                                                                  ModelInstance);

    openfluid::machine::Factory::buildMonitoringInstanceFromDescriptor(mp_FXDesc->monitoring(),
                                                                       MonitInstance);


    // simulation begins -->

    Engine.initialize();

    emit warningsChanged(Engine.getWarningsCount());

    Engine.initParams();

    emit warningsChanged(Engine.getWarningsCount());

    Engine.prepareData();

    emit warningsChanged(Engine.getWarningsCount());

    Engine.checkConsistency();

    emit warningsChanged(Engine.getWarningsCount());

    Engine.run();

    emit warningsChanged(Engine.getWarningsCount());

    Engine.finalize();

    emit warningsChanged(Engine.getWarningsCount());

    //  <-- simulation ends
  }
  catch (UserAbortException& E)
  {
    emit userAbort();
  }
  catch (openfluid::base::Exception& E)
  {
    emit error(QString(E.what()),E.getContext());
  }
  catch (std::exception& E)
  {
    std::cout << E.what() << std::endl;
    emit error(QString(E.what()),openfluid::base::ExceptionContext());
  }
  catch (...)
  {
    std::cout << "Undetermined error" << std::endl;
    emit error(QString("Undetermined error"),openfluid::base::ExceptionContext());
  }

  emit finished();
}


} } } // namespaces
