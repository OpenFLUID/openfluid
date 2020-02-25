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
  @file OpenFLUID.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <iostream>
#include <string>

#include <QElapsedTimer>

#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/base/ApplicationException.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/tools/Console.hpp>
#include <openfluid/utils/CommandLineParser.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/machine/ObserverPluginsManager.hpp>
#include <openfluid/machine/WarePluginsSearchResultsSerializer.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/Factory.hpp>
#include <openfluid/buddies/OpenFLUIDBuddy.hpp>
#include <openfluid/buddies/NewSimBuddy.hpp>
#include <openfluid/buddies/NewDataBuddy.hpp>
#include <openfluid/buddies/ExamplesBuddy.hpp>

#if OPENFLUID_SIM2DOC_ENABLED
#include <openfluid/buddies/Sim2DocBuddy.hpp>
#endif

#include "OpenFLUID.hpp"
#include "DefaultIOListener.hpp"
#include "DefaultMachineListener.hpp"
#include "VerboseMachineListener.hpp"
#include "DefaultBuddiesListener.hpp"


// =====================================================================
// =====================================================================


OpenFLUIDApp::OpenFLUIDApp() :
  mp_RunEnv(nullptr)
{
  m_RunType = None;
  mp_Engine = nullptr;
  m_BuddyToRun.first = "";
  m_BuddyToRun.second = "";

  openfluid::base::RunContextManager::instance()->extraProperties().setBoolean("display.verbose",false);
  openfluid::base::RunContextManager::instance()->extraProperties().setBoolean("display.quiet",false);
}


// =====================================================================
// =====================================================================


OpenFLUIDApp::~OpenFLUIDApp()
{
  openfluid::machine::SimulatorPluginsManager::kill();
  openfluid::machine::ObserverPluginsManager::kill();
  openfluid::base::RunContextManager::kill();
}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printlnExecMessagesStats()
{
  if (mp_Engine != nullptr)
  {
    if (mp_Engine->getWarningsCount())
    {
      openfluid::tools::Console::setWarningColor();
    }

    std::cout << mp_Engine->getWarningsCount() << " warning(s)" << std::endl;

    if (mp_Engine->getWarningsCount())
    {
      openfluid::tools::Console::resetAttributes();
    }
  }
}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printOpenFLUIDInfos()
{
  const unsigned int HeaderWidth = 60;


  auto displayHeaderSeparator = [&HeaderWidth]()
  {
    std::cout << std::string(HeaderWidth,'=') << std::endl;
  };

  auto displayRightAlign = [&HeaderWidth](const std::string& Msg)
  {
    std::cout << std::right << std::setw(HeaderWidth-(HeaderWidth/8)) << Msg << std::endl;
  };

  auto displayCenterAlign = [&HeaderWidth](const std::string& Msg)
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


int OpenFLUIDApp::stopAppReturn(const std::string& ErrorType, const std::string& Msg)
{
  std::cout << std::endl;

  printlnExecMessagesStats();

  std::cout << std::endl;
  openfluid::tools::Console::setErrorColor();
  std::cout << ErrorType << ": ";
  openfluid::tools::Console::resetAttributes();
  std::cout << Msg;
  std::cout << std::endl;

  std::cout << std::endl;
  std::cout.flush();

  if (mp_Engine)
  {
    mp_Engine.reset();
  }

  return 127;

}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printPaths(bool ShowTemp)
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


void OpenFLUIDApp::printEnvInfos()
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


void OpenFLUIDApp::runSimulation()
{
  QElapsedTimer FullTimer;
  QElapsedTimer EffectiveRunTimer;


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

  openfluid::machine::ModelInstance Model(m_SimBlob,MListener.get());
  openfluid::machine::MonitoringInstance Monitoring(m_SimBlob);

  printOpenFLUIDInfos();
  printEnvInfos();


  std::cout << "* Loading data... " << std::endl;
  std::cout.flush();
  openfluid::fluidx::FluidXDescriptor FXDesc(IOListener.get());
  FXDesc.loadFromDirectory(openfluid::base::RunContextManager::instance()->getInputDir());


  std::cout << "* Building spatial domain... ";
  std::cout.flush();
  openfluid::machine::Factory::buildSimulationBlobFromDescriptors(FXDesc,m_SimBlob);
  openfluid::tools::Console::setOKColor();
  std::cout << "[OK]";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;


  std::cout << "* Building model...";
  std::cout.flush();
  openfluid::machine::Factory::buildModelInstanceFromDescriptor(FXDesc.model(),
                                                                Model);
  openfluid::tools::Console::setOKColor();
  std::cout << " [OK]";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "* Building monitoring...";
  std::cout.flush();
  openfluid::machine::Factory::buildMonitoringInstanceFromDescriptor(FXDesc.monitoring(),
                                                                     Monitoring);
  openfluid::tools::Console::setOKColor();
  std::cout << " [OK]";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  mp_Engine = std::make_unique<openfluid::machine::Engine>(m_SimBlob, Model, Monitoring, MListener.get());

  mp_Engine->initialize();


  mp_Engine->initParams();

  mp_Engine->prepareData();

  mp_Engine->checkConsistency();

  openfluid::core::UnitsListByClassMap_t::const_iterator UnitsIt;

  std::cout << std::endl;

  unsigned int UnitsCount = 0;
  for (UnitsIt = m_SimBlob.spatialGraph().allSpatialUnitsByClass()->begin();
      UnitsIt != m_SimBlob.spatialGraph().allSpatialUnitsByClass()->end();
      ++UnitsIt )
  {
    UnitsCount = UnitsCount + (*UnitsIt).second.list()->size();
  }

  std::cout << "Spatial domain, " << UnitsCount << " units :"<< std::endl;
  for (UnitsIt = m_SimBlob.spatialGraph().allSpatialUnitsByClass()->begin();
      UnitsIt != m_SimBlob.spatialGraph().allSpatialUnitsByClass()->end();
      ++UnitsIt )
  {
    std::cout << "  - " << (*UnitsIt).first << ", " << (*UnitsIt).second.list()->size() << " units" << std::endl;
  }
  std::cout << std::endl;

  std::cout << "Simulation from " << mp_Engine->simulationStatus()->getBeginDate().getAsISOString()
            << " to " << mp_Engine->simulationStatus()->getEndDate().getAsISOString() << std::endl;

  if (mp_Engine->simulationStatus()->getSchedulingConstraint() ==
        openfluid::base::SimulationStatus::SCHED_DTCHECKED)
  {
    std::cout << "Checked";
  }
  else if (mp_Engine->simulationStatus()->getSchedulingConstraint() ==
        openfluid::base::SimulationStatus::SCHED_DTFORCED)
  {
    std::cout << "Forced";
  }
  else
  {
    std::cout << "Default";
  }


  std::cout << " DeltaT is " << mp_Engine->simulationStatus()->getDefaultDeltaT() << " seconds" << std::endl;

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
  mp_Engine->run();
  qint64 EffectiveTime = EffectiveRunTimer.elapsed();

  std::cout << "**** Simulation completed ****" << std::endl << std::endl;
  std::cout << std::endl;
  std::cout.flush();

  printlnExecMessagesStats();

  std::cout << std::endl;

  std::cout << "Simulation run time: " << openfluid::tools::getDurationAsPrettyString(EffectiveTime) << std::endl;
  std::cout << "     Total run time: "
            << openfluid::tools::getDurationAsPrettyString(FullTimer.elapsed()) << std::endl;
  std::cout << std::endl;

  mp_Engine->finalize();

  mp_Engine.reset();
}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::processOptions(int ArgC, char **ArgV)
{

  std::string DefaultMaxThreadsStr =
      openfluid::tools::convertValue(openfluid::base::RunContextManager::instance()->getWaresMaxNumThreads());


  // ---- definition of possible options

  std::vector<openfluid::utils::CommandLineOption> RunOptions =
  {
    openfluid::utils::CommandLineOption("quiet","q","quiet display during simulation"),
    openfluid::utils::CommandLineOption("verbose","v","verbose display during simulation"),
    openfluid::utils::CommandLineOption("profiling","k","enable simulation profiling"),
    openfluid::utils::CommandLineOption("clean-output-dir","c","clean output directory before simulation"),
    openfluid::utils::CommandLineOption("auto-output-dir","a","create automatic output directory"),
    openfluid::utils::CommandLineOption("max-threads","t",
                                        "set maximum number of threads for threaded spatial loops"
                                        " (default is "+DefaultMaxThreadsStr+")",true)
  };


#if defined OPENFLUID_OS_WINDOWS
  std::string PathSepText = "semicolon";
#else
  std::string PathSepText = "colon";
#endif

  std::vector<openfluid::utils::CommandLineOption> SearchOptions =
  {
    openfluid::utils::CommandLineOption("observers-paths","n",
                                        "add extra observers search paths ("+PathSepText+" separated)",true),
    openfluid::utils::CommandLineOption("simulators-paths","p",
                                        "add extra simulators search paths ("+PathSepText+" separated)",true),
  };

  openfluid::utils::CommandLineParser Parser("openfluid","");

  Parser.addOption(openfluid::utils::CommandLineOption("version","","display version"));


  // run dataset
  openfluid::utils::CommandLineCommand RunDatasetCmd("run","Run a simulation from a project or an input dataset");
  for (auto& Opt : RunOptions)
  {
    RunDatasetCmd.addOption(Opt);
  }
  for (auto& Opt : SearchOptions)
  {
    RunDatasetCmd.addOption(Opt);
  }
  Parser.addCommand(RunDatasetCmd);


  // buddies
  openfluid::utils::CommandLineCommand BuddyCmd("buddy","Execute a buddy. "
                                                        "Available buddies are newsim, newdata, sim2doc, examples");
  BuddyCmd.addOption(openfluid::utils::CommandLineOption("options","o","set buddy options",true));
  BuddyCmd.addOption(openfluid::utils::CommandLineOption("buddy-help","","display specific buddy help"));
  Parser.addCommand(BuddyCmd);


  // reporting
  openfluid::utils::CommandLineCommand ReportCmd("report","Display report about available wares");
  ReportCmd.addOption(openfluid::utils::CommandLineOption("format","",
                                                          "output format, argument can be text (default) or json",
                                                          true));
  ReportCmd.addOption(openfluid::utils::CommandLineOption("list","l","display as simple list of wares IDs"));
  ReportCmd.addOption(openfluid::utils::CommandLineOption("with-errors","e","report errors if any"));
  for (auto& Opt : SearchOptions)
  {
    ReportCmd.addOption(Opt);
  }
  Parser.addCommand(ReportCmd);


  // show paths
  openfluid::utils::CommandLineCommand ShowPathsCmd("show-paths","Show search paths for wares");
  for (auto& Opt : SearchOptions)
  {
    ShowPathsCmd.addOption(Opt);
  }
  Parser.addCommand(ShowPathsCmd);


  // ---- parsing of effective options

  if (!Parser.parse(ArgC,ArgV))
  {
    throw openfluid::base::ApplicationException(
        openfluid::base::ApplicationException::computeContext("openfluid","command line parsing"),
            Parser.getParsingMessage());
  }

  if (Parser.isHelpAsked())
  {
    m_RunType = None;
    Parser.printHelp(std::cout);
    return;
  }

  std::string ActiveCommandStr = Parser.getActiveCommand();

  if (ActiveCommandStr.empty())
  {
    if (Parser.command(ActiveCommandStr).isOptionActive("version"))
    {
      std::cout << openfluid::config::VERSION_FULL << std::endl;
      m_RunType = InfoRequest;
      return;
    }
  }
  else if (ActiveCommandStr == "run")
  {
    if (Parser.command(ActiveCommandStr).isOptionActive("simulators-paths"))
    {
      openfluid::base::Environment::addExtraSimulatorsDirs(
          Parser.command(ActiveCommandStr).getOptionValue("simulators-paths"));
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("observers-paths"))
    {
      openfluid::base::Environment::addExtraObserversDirs(
          Parser.command(ActiveCommandStr).getOptionValue("observers-paths"));
    }

    if (Parser.extraArgs().size() >= 1)
    {
      if (openfluid::base::RunContextManager::instance()->openProject(Parser.extraArgs().at(0)))
      {
        openfluid::base::RunContextManager::instance()->updateProjectOutputDir();
      }
      else
      {
        openfluid::base::RunContextManager::instance()->setInputDir(Parser.extraArgs().at(0));

        if (Parser.extraArgs().size() >= 2)
        {
          openfluid::base::RunContextManager::instance()->setOutputDir(Parser.extraArgs().at(1));
        }

        if (Parser.command(ActiveCommandStr).isOptionActive("automatic-output-dir"))
        {
          openfluid::base::RunContextManager::instance()->setDateTimeOutputDir();
        }
      }
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("max-threads"))
    {
      unsigned int MaxThreads = 0;

      if (openfluid::tools::convertString(Parser.command(ActiveCommandStr).getOptionValue("max-threads"),&MaxThreads))
      {
        openfluid::base::RunContextManager::instance()->setWaresMaxNumThreads(MaxThreads);
      }
      else
      {
        throw openfluid::base::ApplicationException(
            openfluid::base::ApplicationException::computeContext("openfluid","command line parsing"),
                "wrong value for threads number");
      }
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("clean-output-dir"))
    {
      openfluid::base::RunContextManager::instance()->setClearOutputDir(true);
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("quiet"))
    {
      openfluid::base::RunContextManager::instance()->extraProperties().setBoolean("display.quiet",true);
      openfluid::base::RunContextManager::instance()->extraProperties().setBoolean("display.verbose",false);
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("verbose"))
    {
      openfluid::base::RunContextManager::instance()->extraProperties().setBoolean("display.verbose",true);
      openfluid::base::RunContextManager::instance()->extraProperties().setBoolean("display.quiet",false);
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("profiling"))
    {
      openfluid::base::RunContextManager::instance()->setProfiling(true);
    }

    m_RunType = Simulation;
    return;
  }
  else if (ActiveCommandStr == "report")
  {
    std::string Waretype;

    if (Parser.extraArgs().empty() ||
        (Parser.extraArgs().at(0) != "simulators" && Parser.extraArgs().at(0) != "observers"))
    {
      throw openfluid::base::ApplicationException(
          openfluid::base::ApplicationException::computeContext("openfluid","command line parsing"),
              "Type of wares is missing for reporting");
    }
    else
    {
      Waretype = Parser.extraArgs().at(0);
    }

    std::string MatchStr;
    if (Parser.extraArgs().size() > 1)
    {
      MatchStr = Parser.extraArgs().at(1);
    }

    bool Detailed = !Parser.command(ActiveCommandStr).isOptionActive("list");
    bool WithErrors = Parser.command(ActiveCommandStr).isOptionActive("with-errors");
    std::string Format = "text";

    if (Parser.command(ActiveCommandStr).isOptionActive("format"))
    {
      Format = Parser.command(ActiveCommandStr).getOptionValue("format");
    }

    if (Waretype == "simulators")
    {
      if (Parser.command(ActiveCommandStr).isOptionActive("simulators-paths"))
      {
        openfluid::base::Environment::addExtraSimulatorsDirs(
            Parser.command(ActiveCommandStr).getOptionValue("simulators-paths"));
      }

      const openfluid::machine::SimulatorPluginsManager::PluginsSearchResults SearchResults =
        openfluid::machine::SimulatorPluginsManager::instance()->getAvailableWaresSignatures();

      const openfluid::machine::WarePluginsSearchResultsSerializer<openfluid::machine::ModelItemSignatureInstance> 
        SearchSerializer(SearchResults);

      SearchSerializer.writeToStream(std::cout,Format,Detailed,WithErrors);
      std::cout.flush();
    }
    else if (Waretype == "observers")
    {
      if (Parser.command(ActiveCommandStr).isOptionActive("observers-paths"))
      {
        openfluid::base::Environment::addExtraObserversDirs(
            Parser.command(ActiveCommandStr).getOptionValue("observers-paths"));
      }

      const openfluid::machine::ObserverPluginsManager::PluginsSearchResults SearchResults =
        openfluid::machine::ObserverPluginsManager::instance()->getAvailableWaresSignatures();      

      const openfluid::machine::WarePluginsSearchResultsSerializer<openfluid::machine::ObserverSignatureInstance> 
        SearchSerializer(SearchResults);

      SearchSerializer.writeToStream(std::cout,Format,Detailed,WithErrors);
      std::cout.flush();
    }
  }
  else if (ActiveCommandStr == "buddy")
  {
    if (Parser.extraArgs().empty())
    {
      throw openfluid::base::ApplicationException(
               openfluid::base::ApplicationException::computeContext("openfluid","command line parsing"),
                   "Buddy name is missing");
    }

    openfluid::buddies::OpenFLUIDBuddy* BuddyBody = nullptr;
    openfluid::buddies::BuddiesListener* BuddyListener = new DefaultBuddiesListener();

    std::string BuddyName = Parser.extraArgs().at(0);

    if (BuddyName == "newsim")
    {
      BuddyBody = new openfluid::buddies::NewSimulatorBuddy(BuddyListener);
    }
    else if (BuddyName == "newdata")
    {
      BuddyBody = new openfluid::buddies::NewDataBuddy(BuddyListener);
    }
    else if (BuddyName == "examples")
    {
      BuddyBody = new openfluid::buddies::ExamplesBuddy(BuddyListener);
    }
#if OPENFLUID_SIM2DOC_ENABLED
    else if (BuddyName == "sim2doc")
    {
      BuddyBody = new openfluid::buddies::Sim2DocBuddy(BuddyListener);
    }
#endif
    else
    {
      delete BuddyListener;
      throw openfluid::base::ApplicationException(openfluid::base::ApplicationException::computeContext("openfluid"),
                                                           "Buddy " + BuddyName +" does not exists");
    }


    if (Parser.command(ActiveCommandStr).isOptionActive("buddy-help") && BuddyBody != nullptr)
    {
      std::cout << "Options for buddy " + BuddyName + ":" << std::endl;
      BuddyBody->invokeHelp();
      delete BuddyBody;
    }
    else
    {
      m_BuddyToRun.first = BuddyName;

      if (Parser.command(ActiveCommandStr).isOptionActive("options"))
      {
        m_BuddyToRun.second = Parser.command(ActiveCommandStr).getOptionValue("options");
      }
      m_RunType = Buddy;
      return;
    }

    m_RunType = None;
    return;
  }
  else if (ActiveCommandStr == "show-paths")
  {
    if (Parser.command(ActiveCommandStr).isOptionActive("simulators-paths"))
    {
      openfluid::base::Environment::addExtraSimulatorsDirs(
          Parser.command(ActiveCommandStr).getOptionValue("simulators-paths"));
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("observers-paths"))
    {
      openfluid::base::Environment::addExtraObserversDirs(
          Parser.command(ActiveCommandStr).getOptionValue("observers-paths"));
    }

    m_RunType = InfoRequest;
    printPaths();
    return;
  }
  else
  {
    throw openfluid::base::ApplicationException(
                   openfluid::base::ApplicationException::computeContext("openfluid","command line parsing"),
                       "unknown command \"" + ActiveCommandStr + "\"");
  }

}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::runBuddy()
{
  std::unique_ptr<openfluid::buddies::OpenFLUIDBuddy> Buddy;
  std::unique_ptr<openfluid::buddies::BuddiesListener> BuddyObs = std::make_unique<DefaultBuddiesListener>();

  if (m_BuddyToRun.first == "newsim" )
  {
    Buddy = std::make_unique<openfluid::buddies::NewSimulatorBuddy>(BuddyObs.get());
  }
#if OPENFLUID_SIM2DOC_ENABLED
    // Disabled for compilation errors due to boost.spirit usage under MacOSX
    // TODO Should be re-enabled later
  else if (m_BuddyToRun.first == "sim2doc" )
  {
    Buddy = std::make_unique<openfluid::buddies::Sim2DocBuddy>(BuddyObs.get());
  }
#endif
  else if (m_BuddyToRun.first == "newdata" )
  {
    Buddy = std::make_unique<openfluid::buddies::NewDataBuddy>(BuddyObs.get());
  }
  else if (m_BuddyToRun.first == "examples" )
  {
    Buddy = std::make_unique<openfluid::buddies::ExamplesBuddy>(BuddyObs.get());
  }

  if (Buddy)
  {
    Buddy->parseOptions(m_BuddyToRun.second);
    Buddy->run();
  }
  else
  {
    throw openfluid::base::ApplicationException(openfluid::base::ApplicationException::computeContext("openfluid"),
                                                "Buddy " + m_BuddyToRun.first + " does not exists");
  }
}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::run()
{
  openfluid::tools::Console::saveAttributes();

  if (m_RunType == Simulation)
  {
    runSimulation();
  }
  else if (m_RunType == Buddy)
  {
    runBuddy();
  }
}
