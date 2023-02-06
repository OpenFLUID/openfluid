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
  @file RunTasks.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#include <memory>

#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/Factory.hpp>
#include <openfluid/fluidx/FluidXIO.hpp>
#include <openfluid/tools/Timer.hpp>

#include "DefaultIOListener.hpp"
#include "DefaultMachineListener.hpp"
#include "VerboseMachineListener.hpp"
#include "RunTasks.hpp"


// =====================================================================
// =====================================================================


void printlnExecMessagesStats(const openfluid::machine::Engine* Engine)
{
  if (Engine != nullptr)
  {
    if (Engine->getWarningsCount())
    {
      openfluid::tools::Console::setWarningColor();
    }

    std::cout << Engine->getWarningsCount() << " warning(s)" << std::endl;

    if (Engine->getWarningsCount())
    {
      openfluid::tools::Console::resetAttributes();
    }
  }
}


// =====================================================================
// =====================================================================


void printOpenFLUIDInfos()
{
  const unsigned int HeaderWidth = 60;


  auto displayHeaderSeparator = []()
  {
    std::cout << std::string(HeaderWidth,'=') << std::endl;
  };

  auto displayRightAlign = [](const std::string& Msg)
  {
    std::cout << std::right << std::setw(HeaderWidth-(HeaderWidth/8)) << Msg << std::endl;
  };

  auto displayCenterAlign = [](const std::string& Msg)
  {
    std::cout << std::right << std::setw((HeaderWidth/2)+(Msg.length()/2)) << Msg << std::endl;
  };


  std::string VersionInfo = "OpenFLUID v" + openfluid::config::VERSION_FULL;


  std::cout << std::endl;
  displayHeaderSeparator();
  displayCenterAlign(VersionInfo);
  std::cout << std::endl;
  displayRightAlign("software environment");
  displayRightAlign("for Spatial Modelling in Landscapes");
  std::cout << std::endl;
  displayCenterAlign(openfluid::config::NETWORK_DOMAIN_WWW);
  displayHeaderSeparator();
  std::cout << std::endl;
  std::cout.flush();
}


// =====================================================================
// =====================================================================


void printPaths(bool ShowTemp)
{
  std::vector<std::string> SimulatorsPaths = openfluid::base::Environment::getSimulatorsDirs();

  std::vector<std::string> ObserversPaths = openfluid::base::Environment::getObserversDirs();

  unsigned int i;


  std::cout << "Input dir: " << openfluid::base::RunContextManager::instance()->getInputDir() << std::endl;

  std::cout << "Output dir: " << openfluid::base::RunContextManager::instance()->getOutputDir() << std::endl;

  std::cout << "Simulators search path(s):" << std::endl;
  for (i=0;i<SimulatorsPaths.size();i++)
  {
    std::cout << " #" << (i+1) << " " << SimulatorsPaths[i] << std::endl;
  }

  std::cout << "Observers search path(s):" << std::endl;
  for (i=0;i<ObserversPaths.size();i++)
  {
    std::cout << " #" << (i+1) << " " << ObserversPaths[i] << std::endl;
  }

  if (ShowTemp)
  {
    std::cout << "Temp dir: " << openfluid::base::Environment::getTempDir() << std::endl;
  }
}


// =====================================================================
// =====================================================================


void printEnvInfos()
{
  bool IsVerbose = openfluid::base::RunContextManager::instance()->extraProperties().getBoolean("display.verbose");
  bool IsQuiet = openfluid::base::RunContextManager::instance()->extraProperties().getBoolean("display.quiet");

  printPaths(false);

  if (openfluid::base::RunContextManager::instance()->isClearOutputDir())
  {
    std::cout << "Output directory cleared before simulation" << std::endl;
  }

  if (IsQuiet)
  {
    std::cout << "Quiet mode enabled" << std::endl;
  }

  if (IsVerbose)
  {
    std::cout << "Verbose mode enabled" << std::endl;
  }

  std::cout << std::endl;
}


// =====================================================================
// =====================================================================


int RunTasks::process() const
{
  openfluid::base::RunContextManager::instance()->extraProperties().setBoolean("display.verbose",false);
  openfluid::base::RunContextManager::instance()->extraProperties().setBoolean("display.quiet",false);

  if (m_Cmd.isOptionActive("simulators-paths"))
  {
    openfluid::base::Environment::addExtraSimulatorsDirs(m_Cmd.getOptionValue("simulators-paths"));
  }

  if (m_Cmd.isOptionActive("observers-paths"))
  {
    openfluid::base::Environment::addExtraObserversDirs(m_Cmd.getOptionValue("observers-paths"));
  }

  if (m_ExtraArgs.size() >= 1)
  {
    if (openfluid::base::RunContextManager::instance()->openProject(m_ExtraArgs.at(0)))
    {
      openfluid::base::RunContextManager::instance()->updateProjectOutputDir();
    }
    else
    {
      openfluid::base::RunContextManager::instance()->setInputDir(m_ExtraArgs.at(0));

      if (m_ExtraArgs.size() >= 2)
      {
        openfluid::base::RunContextManager::instance()->setOutputDir(m_ExtraArgs.at(1));
      }

      if (m_Cmd.isOptionActive("automatic-output-dir"))
      {
        openfluid::base::RunContextManager::instance()->setDateTimeOutputDir();
      }
    }
  }

  if (m_Cmd.isOptionActive("max-threads"))
  {
    unsigned int MaxThreads = 0;

    if (openfluid::tools::toNumeric(m_Cmd.getOptionValue("max-threads"),MaxThreads))
    {
      openfluid::base::RunContextManager::instance()->setWaresMaxNumThreads(MaxThreads);
    }
    else
    {
      return error("wrong value for threads number");
    }
  }

  if (m_Cmd.isOptionActive("clean-output-dir"))
  {
    openfluid::base::RunContextManager::instance()->setClearOutputDir(true);
  }

  if (m_Cmd.isOptionActive("quiet"))
  {
    openfluid::base::RunContextManager::instance()->extraProperties().setBoolean("display.quiet",true);
    openfluid::base::RunContextManager::instance()->extraProperties().setBoolean("display.verbose",false);
  }

  if (m_Cmd.isOptionActive("verbose"))
  {
    openfluid::base::RunContextManager::instance()->extraProperties().setBoolean("display.verbose",true);
    openfluid::base::RunContextManager::instance()->extraProperties().setBoolean("display.quiet",false);
  }

  if (m_Cmd.isOptionActive("profiling"))
  {
    openfluid::base::RunContextManager::instance()->setProfiling(true);
  }


  // -----


  openfluid::tools::Console::saveAttributes();

  try
  {
    

    openfluid::machine::SimulationBlob SimBlob;
    std::unique_ptr<openfluid::machine::Engine> Engine;

    openfluid::tools::Timer FullTimer;
    openfluid::tools::Timer EffectiveRunTimer;


    FullTimer.start();

    bool IsVerbose = openfluid::base::RunContextManager::instance()->extraProperties().getBoolean("display.verbose");
    bool IsQuiet = openfluid::base::RunContextManager::instance()->extraProperties().getBoolean("display.quiet");


    std::unique_ptr<openfluid::machine::MachineListener> MListener;
    std::unique_ptr<openfluid::base::IOListener> IOListener = std::make_unique<DefaultIOListener>();


    if (IsQuiet)
    {
      MListener = std::make_unique<openfluid::machine::MachineListener>();
    }
    else
    {
      if (IsVerbose)
      {
        MListener = std::make_unique<VerboseMachineListener>();
      }
      else
      {
        MListener = std::make_unique<DefaultMachineListener>();
      }
    }

    openfluid::machine::ModelInstance Model(SimBlob,MListener.get());
    openfluid::machine::MonitoringInstance Monitoring(SimBlob);

    printOpenFLUIDInfos();
    printEnvInfos();


    std::cout << "* Loading data... " << std::endl;
    std::cout.flush();
    openfluid::fluidx::FluidXIO FXIO(IOListener.get());
    auto FXDesc = FXIO.loadFromDirectory(openfluid::base::RunContextManager::instance()->getInputDir());


    std::cout << "* Building spatial domain... ";
    std::cout.flush();
    openfluid::machine::Factory::buildSimulationBlobFromDescriptors(FXDesc,SimBlob);
    openfluid::tools::Console::setOKColor();
    std::cout << "[OK]";
    openfluid::tools::Console::resetAttributes();
    std::cout << std::endl;


    std::cout << "* Building model...";
    std::cout.flush();
    openfluid::machine::Factory::buildModelInstanceFromDescriptor(FXDesc.model(),Model);
    openfluid::tools::Console::setOKColor();
    std::cout << " [OK]";
    openfluid::tools::Console::resetAttributes();
    std::cout << std::endl;

    std::cout << "* Building monitoring...";
    std::cout.flush();
    openfluid::machine::Factory::buildMonitoringInstanceFromDescriptor(FXDesc.monitoring(),Monitoring);
    openfluid::tools::Console::setOKColor();
    std::cout << " [OK]";
    openfluid::tools::Console::resetAttributes();
    std::cout << std::endl;


    Engine = std::make_unique<openfluid::machine::Engine>(SimBlob, Model, Monitoring, MListener.get());

    Engine->initialize();

    Engine->initParams();
    Engine->prepareData();
    Engine->checkConsistency();

    openfluid::core::UnitsListByClassMap_t::const_iterator UnitsIt;

    std::cout << std::endl;

    unsigned int UnitsCount = 0;
    for (UnitsIt = SimBlob.spatialGraph().allSpatialUnitsByClass()->begin();
        UnitsIt != SimBlob.spatialGraph().allSpatialUnitsByClass()->end();
        ++UnitsIt )
    {
      UnitsCount = UnitsCount + (*UnitsIt).second.list()->size();
    }

    std::cout << "Spatial domain, " << UnitsCount << " units :"<< std::endl;
    for (UnitsIt = SimBlob.spatialGraph().allSpatialUnitsByClass()->begin();
        UnitsIt != SimBlob.spatialGraph().allSpatialUnitsByClass()->end();
        ++UnitsIt )
    {
      std::cout << "  - " << (*UnitsIt).first << ", " << (*UnitsIt).second.list()->size() << " units" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Simulation from " << Engine->simulationStatus()->getBeginDate().getAsISOString()
              << " to " << Engine->simulationStatus()->getEndDate().getAsISOString() << std::endl;

    if (Engine->simulationStatus()->getSchedulingConstraint() ==
          openfluid::base::SimulationStatus::SCHED_DTCHECKED)
    {
      std::cout << "Checked";
    }
    else if (Engine->simulationStatus()->getSchedulingConstraint() ==
          openfluid::base::SimulationStatus::SCHED_DTFORCED)
    {
      std::cout << "Forced";
    }
    else
    {
      std::cout << "Default";
    }


    std::cout << " DeltaT is " << Engine->simulationStatus()->getDefaultDeltaT() << " seconds" << std::endl;

    std::cout << std::endl;

    std::cout << "Size of buffers for variables is set to "
              << openfluid::core::ValuesBufferProperties::getBufferSize();

    if (openfluid::base::RunContextManager::instance()->isValuesBufferUserSize())
    {
      std::cout << " (using run configuration)";
    }
    else
    {
      std::cout << " (automatically computed)";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout.flush();

    std::cout << std::endl << "**** Running simulation ****" << std::endl;
    std::cout.flush();

    EffectiveRunTimer.start();
    Engine->run();
    EffectiveRunTimer.stop();

    std::cout << "**** Simulation completed ****" << std::endl << std::endl;
    std::cout << std::endl;
    std::cout.flush();

    printlnExecMessagesStats(Engine.get());

    FullTimer.stop();

    std::cout << std::endl;
    std::cout << "Simulation run time: " << EffectiveRunTimer.elapsedAsPrettyString() << std::endl;
    std::cout << "     Total run time: "
              << FullTimer.elapsedAsPrettyString() << std::endl;
    std::cout << std::endl;

    Engine->finalize();

    Engine.reset();
  }
  catch (openfluid::base::Exception& E)
  {
    return error(E.getMessage() + " [" + E.getContext().toString() + "]","OpenFLUID ERROR");
  }
  catch (std::bad_alloc& E)
  {
    return error(std::string(E.what()) +". Possibly not enough memory available","MEMORY ALLOCATION ERROR");
  }
  catch (std::exception& E)
  {
    return error(std::string(E.what()),"SYSTEM ERROR");
  }
  catch (...)
  {
    return error("no information","UNKNOWN ERROR");
  }

  return 0;
}
