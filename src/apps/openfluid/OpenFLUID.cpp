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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/

#include <iostream>
#include <string>


#include <QElapsedTimer>

#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/base/ApplicationException.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/utils/CommandLineParser.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/machine/ObserverPluginsManager.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/Factory.hpp>
#include <openfluid/buddies.hpp>

#include "OpenFLUID.hpp"
#include "DefaultIOListener.hpp"
#include "DefaultMachineListener.hpp"
#include "VerboseMachineListener.hpp"
#include "DefaultBuddiesListener.hpp"



// =====================================================================
// =====================================================================

std::string msecsToString(qint64 MSecs)
{
  int LeftMSecs = (int) (MSecs % 1000);
  int Seconds = (int) (MSecs / 1000) % 60;
  int Minutes = (int) ((MSecs / (1000*60)) % 60);
  int Hours   = (int) ((MSecs / (1000*60*60)) % 24);
  int Days   = (int) (MSecs / (1000*60*60*24));

  return openfluid::tools::convertValue(Days)+"d "+
         openfluid::tools::convertValue(Hours)+"h "+
         openfluid::tools::convertValue(Minutes)+"m "+
         openfluid::tools::convertValue(Seconds)+"."+
         openfluid::tools::convertValue(LeftMSecs)+"s";
}


// =====================================================================
// =====================================================================

OpenFLUIDApp::OpenFLUIDApp()
{
  m_RunType = None;
  mp_Engine = NULL;
  m_BuddyToRun.first = "";
  m_BuddyToRun.second = "";
}


// =====================================================================
// =====================================================================


OpenFLUIDApp::~OpenFLUIDApp()
{

}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printlnExecMessagesStats()
{
  if (mp_Engine != NULL) std::cout << mp_Engine->getWarningsCount() << " warning(s)" << std::endl;
}

// =====================================================================
// =====================================================================


void OpenFLUIDApp::printOpenFLUIDInfos()
{

  int Width = 60;
  std::string VersionInfo = "OpenFLUID v";
  std::string Whites = "";

  VersionInfo = VersionInfo + openfluid::config::FULL_VERSION;

  // centering the version number, using white spaces
  for (unsigned int i=0;i<((Width-VersionInfo.length())/2);i++) Whites = Whites + " ";

  VersionInfo = Whites + VersionInfo;


  std::cout << std::endl;
  std::cout << "===========================================================" << std::endl;
  std::cout << VersionInfo << std::endl;
  std::cout << std::endl;
  std::cout << "                          software environment             " << std::endl;
  std::cout << "            for Modelling Fluxes in Landscapes             " << std::endl;
  std::cout << std::endl;
  std::cout << "                 www.openfluid-project.org                 " << std::endl;
  std::cout << "===========================================================" << std::endl;
  std::cout << std::endl;
  std::cout.flush();
}



// =====================================================================
// =====================================================================


void OpenFLUIDApp::printSimulatorsList()
{

  std::vector<openfluid::machine::ModelItemSignatureInstance*> PlugContainers =
    openfluid::machine::SimulatorPluginsManager::instance()->getAvailableWaresSignatures();

  std::cout << "Available simulators:" << std::endl;

  bool OneAtLeast = false;

  for (unsigned int i=0;i<PlugContainers.size();i++)
  {
    if (PlugContainers[i]->Verified && PlugContainers[i]->Signature!=NULL)
    {
      std::cout << "  - " << PlugContainers[i]->Signature->ID << std::endl;
      OneAtLeast = true;
    }
  }

  if (!OneAtLeast)
  {
    std::cout << "  (none)" << std::endl;
  }
  std::cout << std::endl;
  std::cout.flush();

}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printMonitoring()
{

  std::vector<openfluid::machine::ObserverSignatureInstance*> PlugContainers =
    openfluid::machine::ObserverPluginsManager::instance()->getAvailableWaresSignatures();

  std::cout << "Available observers:" << std::endl;

  bool OneAtLeast = false;

  for (unsigned int i=0;i<PlugContainers.size();i++)
  {
    if (PlugContainers[i]->Verified && PlugContainers[i]->Signature!=NULL)
    {
      std::cout << "  - " << PlugContainers[i]->Signature->ID << std::endl;
      OneAtLeast = true;
    }
  }

  if (!OneAtLeast)
  {
    std::cout << "  (none)" << std::endl;
  }
  std::cout << std::endl;
  std::cout.flush();

}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printWareInfosReport(const openfluid::ware::WareSignature* Signature, const std::string& Filename)
{
  std::string StatusStr = "experimental";
  if (Signature->Status == openfluid::ware::BETA) StatusStr = "beta";
  if (Signature->Status == openfluid::ware::STABLE) StatusStr = "stable";

  std::cout << "   - Name: " << openfluid::tools::replaceEmptyString(Signature->Name,("(unknown)"))
            << std::endl;
  std::cout << "   - File: " << Filename
            << std::endl;
  std::cout << "   - Description: " << openfluid::tools::replaceEmptyString(Signature->Description,("(none)"))
            << std::endl;
  std::cout << "   - Version: " << openfluid::tools::replaceEmptyString(Signature->Version,("(unknown)"))
            << std::endl;
  std::cout << "   - SDK version used at build time: " << Signature->ABIVersion
            <<  std::endl;
  std::cout << "   - Development status: " << StatusStr
            <<  std::endl;
  std::cout << "   - Author(s): " << Signature->getAuthorsAsString()
            << std::endl;
}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printSimulatorsDataItemReport(openfluid::ware::SignatureDataItem HandledItem,
                                                 std::string Suffix, std::string Type)
{
  std::string TypeStr = ("");

  std::cout << Suffix;

  std::string UnitStr = ("");

  if (HandledItem.DataUnit != ("")) UnitStr = (" (")+HandledItem.DataUnit+(")");

  if (Type == ("fpar")) TypeStr = ("simulator parameter");


  std::cout << HandledItem.DataName << UnitStr << " : " << TypeStr << ".";
  if (HandledItem.Description.length()!=0) std::cout << " " << HandledItem.Description;
  std::cout << std::endl;
}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printSimulatorsSpatialDataItemReport(openfluid::ware::SignatureSpatialDataItem HandledItem,
                                                        std::string Suffix, std::string Type)
{
  std::string TypeStr = ("");

  std::cout << Suffix;

  std::string UnitStr = ("");
  std::string DistribStr = ("");


  if (HandledItem.DataUnit != ("")) UnitStr = (" (")+HandledItem.DataUnit+(")");
  if (HandledItem.UnitsClass != ("")) DistribStr = " {"+HandledItem.UnitsClass+"}";


  if (Type == ("pvar")) TypeStr = ("produced variable");
  if (Type == ("uvar")) TypeStr = ("updated variable");

  if (Type == ("rvar")) TypeStr = ("required variable");
  if (Type == ("svar")) TypeStr = ("used variable (only if available)");

  if (Type == ("fpar")) TypeStr = ("simulator parameter");

  if (Type == ("pinput")) TypeStr = ("produced attribute");
  if (Type == ("rinput")) TypeStr = ("required attribute");
  if (Type == ("sinput")) TypeStr = ("used attribute");

  std::cout << HandledItem.DataName << DistribStr << " : " << TypeStr << ".";
  if (HandledItem.Description.length()!=0) std::cout << " " << HandledItem.Description << UnitStr;
  std::cout << std::endl;
}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printSimulatorsHandledUnitsGraphReport(openfluid::ware::SignatureUnitsGraph HandledUnitsGraph,
                                                          std::string Suffix)
{
  unsigned int i;
  if (!HandledUnitsGraph.UpdatedUnitsGraph.empty())
    std::cout << Suffix << "Global units graph updates: " << HandledUnitsGraph.UpdatedUnitsGraph << std::endl;

  for (i=0;i<HandledUnitsGraph.UpdatedUnitsClass.size();i++)
  {
    std::cout << Suffix << "Units graph update on class "
              << HandledUnitsGraph.UpdatedUnitsClass[i].UnitsClass << ": "
              << HandledUnitsGraph.UpdatedUnitsClass[i].Description << std::endl;
  }
}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printSimulatorsTimeSchedulingReport(openfluid::ware::SignatureTimeScheduling TScheduling)
{
  if (TScheduling.Type == openfluid::ware::SignatureTimeScheduling::DEFAULT)
  {
    std::cout << "fixed to default deltaT" << std::endl;
  }
  else if (TScheduling.Type == openfluid::ware::SignatureTimeScheduling::FIXED)
  {
    std::cout << "fixed to " <<  TScheduling.Min << " seconds" << std::endl;
  }
  else if (TScheduling.Type == openfluid::ware::SignatureTimeScheduling::RANGE)
  {
    std::cout << "range between " <<  TScheduling.Min << " and " << TScheduling.Max << " seconds" << std::endl;
  }
  else
  {
    std::cout << "undefined" << std::endl;
  }
}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printSimulatorsHandledDataReport(openfluid::ware::SignatureHandledData HandledData,
                                                    std::string Suffix)
{

  unsigned int i;

  for (i=0;i<HandledData.UsedParams.size();i++)
    printSimulatorsDataItemReport(HandledData.UsedParams[i],Suffix,("fpar"));

  for (i=0;i<HandledData.ProducedVars.size();i++)
    printSimulatorsSpatialDataItemReport(HandledData.ProducedVars[i],Suffix,("pvar"));

  for (i=0;i<HandledData.RequiredVars.size();i++)
    printSimulatorsSpatialDataItemReport(HandledData.RequiredVars[i],Suffix,("rvar"));

  for (i=0;i<HandledData.UpdatedVars.size();i++)
    printSimulatorsSpatialDataItemReport(HandledData.UpdatedVars[i],Suffix,("uvar"));

  for (i=0;i<HandledData.UsedVars.size();i++)
    printSimulatorsSpatialDataItemReport(HandledData.UsedVars[i],Suffix,("svar"));

  for (i=0;i<HandledData.ProducedAttribute.size();i++)
    printSimulatorsSpatialDataItemReport(HandledData.ProducedAttribute[i],Suffix,("pinput"));

  for (i=0;i<HandledData.RequiredAttribute.size();i++)
    printSimulatorsSpatialDataItemReport(HandledData.RequiredAttribute[i],Suffix,("rinput"));

  for (i=0;i<HandledData.UsedAttribute.size();i++)
    printSimulatorsSpatialDataItemReport(HandledData.UsedAttribute[i],Suffix,("sinput"));

  if (HandledData.UsedEventsOnUnits.size() > 0)
  {
    std::cout << Suffix << "Events used on: ";
    for (i=0;i<HandledData.UsedEventsOnUnits.size();i++)
    {
      std::cout << HandledData.UsedEventsOnUnits[i];
      if (i == HandledData.UsedEventsOnUnits.size()-1 ) std::cout << std::endl;
      else std::cout << ", ";
    }
  }

  for (i=0;i<HandledData.RequiredExtraFiles.size();i++)
    std::cout << Suffix << "Required extra file : " << HandledData.RequiredExtraFiles[i] << std::endl;

  for (i=0;i<HandledData.UsedExtraFiles.size();i++)
    std::cout << Suffix << "Used extra file : " << HandledData.UsedExtraFiles[i] << std::endl;

}



// =====================================================================
// =====================================================================


void OpenFLUIDApp::printSimulatorsReport(const std::string Pattern)
{

  std::vector<openfluid::machine::ModelItemSignatureInstance*> PlugContainers =
      openfluid::machine::SimulatorPluginsManager::instance()->getAvailableWaresSignatures(Pattern);
  std::string StatusStr;


  if (PlugContainers.size() > 0)
  {
    for (unsigned int i=0;i<PlugContainers.size();i++)
    {

      // Status string
      StatusStr = "experimental";
      if (PlugContainers[i]->Signature->Status == openfluid::ware::BETA) StatusStr = "beta";
      if (PlugContainers[i]->Signature->Status == openfluid::ware::STABLE) StatusStr = "stable";


      std::cout << "* " << PlugContainers[i]->Signature->ID
                << std::endl;

      std::cout << "   - Name: "
                << openfluid::tools::replaceEmptyString(PlugContainers[i]->Signature->Name,("(unknown)"))
                << std::endl;

      std::cout << "   - File: " << PlugContainers[i]->FileFullPath << std::endl;

      std::cout << "   - Domain: "
                << openfluid::tools::replaceEmptyString(PlugContainers[i]->Signature->Domain,("(unknown)"))
                << std::endl;

      std::cout << "   - Process: "
                << openfluid::tools::replaceEmptyString(PlugContainers[i]->Signature->Process,("(unknown)"))
                << std::endl;

      std::cout << "   - Method: "
                << openfluid::tools::replaceEmptyString(PlugContainers[i]->Signature->Method,("(unknown)"))
                << std::endl;

      std::cout << "   - Description: "
                << openfluid::tools::replaceEmptyString(PlugContainers[i]->Signature->Description,("(none)"))
                << std::endl;

      std::cout << "   - Version: "
                << openfluid::tools::replaceEmptyString(PlugContainers[i]->Signature->Version,("(unknown)"))
                << std::endl;

      std::cout << "   - SDK version used at build time: "
                << PlugContainers[i]->Signature->ABIVersion
                <<  std::endl;

      std::cout << "   - Development status: "
                << StatusStr
                <<  std::endl;

      std::cout << "   - Author(s): "
                << PlugContainers[i]->Signature->getAuthorsAsString()
                << std::endl;

      std::cout << "   - Time scheduling : ";
      printSimulatorsTimeSchedulingReport(PlugContainers[i]->Signature->TimeScheduling);

      std::cout << "   - Handled data" << std::endl;
      printSimulatorsHandledDataReport(PlugContainers[i]->Signature->HandledData,("     . "));

      std::cout << "   - Handled units graph" << std::endl;
      printSimulatorsHandledUnitsGraphReport(PlugContainers[i]->Signature->HandledUnitsGraph,("     . "));


      if (i != PlugContainers.size()-1)
        std::cout << "================================================================================" << std::endl;
    }
  }

  std::cout.flush();

}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printObserversReport(const std::string Pattern)
{
  std::vector<openfluid::machine::ObserverSignatureInstance*> PlugContainers =
      openfluid::machine::ObserverPluginsManager::instance()->getAvailableWaresSignatures(Pattern);
  std::string StatusStr;


  if (PlugContainers.size() > 0)
  {
    for (unsigned int i=0;i<PlugContainers.size();i++)
    {
      std::cout << "* " << PlugContainers[i]->Signature->ID << std::endl;
      printWareInfosReport((openfluid::ware::WareSignature*)(PlugContainers[i]->Signature),
                           PlugContainers[i]->FileFullPath);

      if (i != PlugContainers.size()-1)
        std::cout << "================================================================================" << std::endl;
    }
  }

  std::cout.flush();

}

// =====================================================================
// =====================================================================


int OpenFLUIDApp::stopAppReturn(std::string Msg)
{
  std::cout << std::endl;

  printlnExecMessagesStats();

  std::cout << std::endl << Msg << std::endl;

  std::cout << std::endl;
  std::cout.flush();

  if (mp_Engine != NULL) delete mp_Engine;

  return 127;

}

// =====================================================================
// =====================================================================


void OpenFLUIDApp::printPaths(bool ShowTemp)
{
  std::vector<std::string> SimulatorsPaths =
      openfluid::base::RuntimeEnvironment::instance()->getSimulatorsPluginsPaths();

  std::vector<std::string> ObserversPaths =
      openfluid::base::RuntimeEnvironment::instance()->getObserversPluginsPaths();

  unsigned int i;


  std::cout << "Input dir: " << openfluid::base::RuntimeEnvironment::instance()->getInputDir() << std::endl;

  std::cout << "Output dir: " << openfluid::base::RuntimeEnvironment::instance()->getOutputDir() << std::endl;

  std::cout << "Simulators search path(s):" << std::endl;
  for (i=0;i<SimulatorsPaths.size();i++)
    std::cout << " #" << (i+1) << " " << SimulatorsPaths[i] << std::endl;

  std::cout << "Observers search path(s):" << std::endl;
  for (i=0;i<ObserversPaths.size();i++)
    std::cout << " #" << (i+1) << " " << ObserversPaths[i] << std::endl;

  if (ShowTemp)
    std::cout << "Temp dir: " << openfluid::base::RuntimeEnvironment::instance()->getTempDir() << std::endl;
}

// =====================================================================
// =====================================================================


void OpenFLUIDApp::printEnvInfos()
{
  bool IsVerbose = false;
  openfluid::base::RuntimeEnvironment::instance()->extraProperties().getValue("display.verbose",IsVerbose);
  bool IsQuiet = false;
  openfluid::base::RuntimeEnvironment::instance()->extraProperties().getValue("display.quiet",IsQuiet);


  printPaths(false);

  if (openfluid::base::RuntimeEnvironment::instance()->isClearOutputDir())
    std::cout << "Output directory cleared before simulation" << std::endl;

  if (IsQuiet)
    std::cout << "Quiet mode enabled" << std::endl;

  if (IsVerbose)
    std::cout << "Verbose mode enabled" << std::endl;

  std::cout << std::endl;
}


// =====================================================================
// =====================================================================

void OpenFLUIDApp::runSimulation()
{
  QElapsedTimer FullTimer;
  QElapsedTimer EffectiveRunTimer;


  FullTimer.start();

  bool IsVerbose = false;
  openfluid::base::RuntimeEnvironment::instance()->extraProperties().getValue("display.verbose",IsVerbose);
  bool IsQuiet = false;
  openfluid::base::RuntimeEnvironment::instance()->extraProperties().getValue("display.quiet",IsQuiet);


  openfluid::machine::MachineListener* MListener;
  openfluid::base::IOListener* IOListener = new DefaultIOListener();


  if (IsQuiet)
  {
    MListener = new openfluid::machine::MachineListener();
  }
  else
  {
    if (IsVerbose)
    {
      MListener = new VerboseMachineListener();
    }
    else MListener = new DefaultMachineListener();
  }

  openfluid::machine::ModelInstance Model(m_SimBlob,MListener);
  openfluid::machine::MonitoringInstance Monitoring(m_SimBlob);

  printOpenFLUIDInfos();
  printEnvInfos();




  std::cout << "* Loading data... " << std::endl; std::cout.flush();
  openfluid::fluidx::FluidXDescriptor FXDesc(IOListener);
  FXDesc.loadFromDirectory(openfluid::base::RuntimeEnvironment::instance()->getInputDir());


  std::cout << "* Building spatial domain... "; std::cout.flush();
  openfluid::machine::Factory::buildSimulationBlobFromDescriptors(FXDesc,m_SimBlob);
  std::cout << "[OK]" << std::endl; std::cout.flush();


  std::cout << "* Building model... "; std::cout.flush();
  openfluid::machine::Factory::buildModelInstanceFromDescriptor(FXDesc.modelDescriptor(),
                                                                Model);
  std::cout << "[OK]" << std::endl; std::cout.flush();

  std::cout << "* Building monitoring... "; std::cout.flush();
  openfluid::machine::Factory::buildMonitoringInstanceFromDescriptor(FXDesc.monitoringDescriptor(),
                                                                Monitoring);
  std::cout << "[OK]" << std::endl; std::cout.flush();

  mp_Engine = new openfluid::machine::Engine(m_SimBlob, Model, Monitoring, MListener);

  mp_Engine->initialize();

  std::cout << "* Initializing parameters... "; std::cout.flush();
  mp_Engine->initParams();

  std::cout << "* Preparing data... "; std::cout.flush();
  mp_Engine->prepareData();


  std::cout << "* Checking consistency... "; std::cout.flush();
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

  if (openfluid::base::RuntimeEnvironment::instance()->isUserValuesBufferSize())
    std::cout << " (using dataset run configuration)";
  else
    std::cout << " (automatically computed)";
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

  std::cout << "Simulation run time: " << msecsToString(EffectiveTime) << std::endl;
  std::cout << "     Total run time: " << msecsToString(FullTimer.elapsed()) << std::endl;
  std::cout << std::endl;

  mp_Engine->finalize();

  delete mp_Engine;
  mp_Engine = NULL;

}

// =====================================================================
// =====================================================================


void OpenFLUIDApp::processOptions(int ArgC, char **ArgV)
{

  std::string DefaultMaxThreadsStr;
  openfluid::tools::convertValue(openfluid::config::SIMULATORS_MAXNUMTHREADS,&DefaultMaxThreadsStr);


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

  std::string PathSepText = "colon";

#if defined OPENFLUID_OS_WINDOWS
  PathSepText = "semicolon";
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
    RunDatasetCmd.addOption(Opt);
  for (auto& Opt : SearchOptions)
    RunDatasetCmd.addOption(Opt);
  Parser.addCommand(RunDatasetCmd);


  // buddies
  openfluid::utils::CommandLineCommand BuddyCmd("buddy","Execute a buddy. "
                                                        "Available buddies are newsim, newdata, sim2doc, examples");
  BuddyCmd.addOption(openfluid::utils::CommandLineOption("options","o","set buddy options",true));
  BuddyCmd.addOption(openfluid::utils::CommandLineOption("buddy-help","","display specific buddy help"));
  Parser.addCommand(BuddyCmd);


  // reporting
  openfluid::utils::CommandLineCommand ReportCmd("report","Display informations about available wares");
  ReportCmd.addOption(openfluid::utils::CommandLineOption("list","l","display simple list instead of report"));
  for (auto& Opt : SearchOptions)
    ReportCmd.addOption(Opt);
  Parser.addCommand(ReportCmd);



  // show paths
  openfluid::utils::CommandLineCommand ShowPathsCmd("show-paths","Show search paths for wares");
  for (auto& Opt : SearchOptions)
    ShowPathsCmd.addOption(Opt);
  Parser.addCommand(ShowPathsCmd);


  if (!Parser.parse(ArgC,ArgV))
    throw openfluid::base::ApplicationException(
        openfluid::base::ApplicationException::computeContext("openfluid","command line parsing"),
            Parser.getParsingMessage());



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
      std::cout << openfluid::config::FULL_VERSION << std::endl;
      m_RunType = InfoRequest;
      return;
    }
  }
  else if (ActiveCommandStr == "run")
  {
    if (Parser.command(ActiveCommandStr).isOptionActive("simulators-paths"))
    {
      openfluid::base::RuntimeEnvironment::instance()->addExtraSimulatorsPluginsPaths(
          Parser.command(ActiveCommandStr).getOptionValue("simulators-paths"));
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("observers-paths"))
    {
      openfluid::base::RuntimeEnvironment::instance()->addExtraObserversPluginsPaths(
          Parser.command(ActiveCommandStr).getOptionValue("observers-paths"));
    }

    if (Parser.extraArgs().size() >= 1)
    {
      if (openfluid::base::ProjectManager::instance()->open(Parser.extraArgs().at(0)))
      {
        openfluid::base::RuntimeEnvironment::instance()->linkToProject();
        openfluid::base::ProjectManager::instance()->updateOutputDir();
      }
      else
      {
        openfluid::base::RuntimeEnvironment::instance()->setInputDir(Parser.extraArgs().at(0));

        if (Parser.extraArgs().size() >= 2)
          openfluid::base::RuntimeEnvironment::instance()->setOutputDir(Parser.extraArgs().at(1));

        if (Parser.command(ActiveCommandStr).isOptionActive("automatic-output-dir"))
        {
          openfluid::base::RuntimeEnvironment::instance()->setDateTimeOutputDir();
        }
      }
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("max-threads"))
    {
      unsigned int MaxThreads = 0;

      std::cout << Parser.command(ActiveCommandStr).getOptionValue("max-threads") << std::endl;

      if (openfluid::tools::convertString(Parser.command(ActiveCommandStr).getOptionValue("max-threads"),&MaxThreads))
        openfluid::base::RuntimeEnvironment::instance()->setSimulatorsMaxNumThreads(MaxThreads);
      else
        throw openfluid::base::ApplicationException(
            openfluid::base::ApplicationException::computeContext("openfluid","command line parsing"),
                "wrong value for threads number");
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("clean-output-dir"))
    {
      openfluid::base::RuntimeEnvironment::instance()->setClearOutputDir(true);
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("quiet"))
    {
      openfluid::base::RuntimeEnvironment::instance()->extraProperties().setValue("display.quiet",true);
      openfluid::base::RuntimeEnvironment::instance()->extraProperties().setValue("display.verbose",false);
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("verbose"))
    {
      openfluid::base::RuntimeEnvironment::instance()->extraProperties().setValue("display.verbose",true);
      openfluid::base::RuntimeEnvironment::instance()->extraProperties().setValue("display.quiet",false);
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("profiling"))
    {
      openfluid::base::RuntimeEnvironment::instance()->setSimulationProfilingEnabled(true);
    }

    m_RunType = Simulation;
    return;
  }
  else if (ActiveCommandStr == "report")
  {
    std::string Waretype;

    if (Parser.extraArgs().empty() ||
        (Parser.extraArgs().at(0) != "simulators" && Parser.extraArgs().at(0) != "observers"))
      throw openfluid::base::ApplicationException(
          openfluid::base::ApplicationException::computeContext("openfluid","command line parsing"),
              "Type of wares is missing for reporting");
    else
      Waretype = Parser.extraArgs().at(0);

    std::string MatchStr;
    if (Parser.extraArgs().size() > 1)
      MatchStr = Parser.extraArgs().at(1);

    if (Waretype == "simulators")
    {
      if (Parser.command(ActiveCommandStr).isOptionActive("simulators-paths"))
      {
        openfluid::base::RuntimeEnvironment::instance()->addExtraSimulatorsPluginsPaths(
            Parser.command(ActiveCommandStr).getOptionValue("simulators-paths"));
      }

      if (Parser.command(ActiveCommandStr).isOptionActive("list"))
      {
        m_RunType = InfoRequest;
        printSimulatorsList();
        return;
      }
      else
      {
        m_RunType = InfoRequest;
        printSimulatorsReport(MatchStr);
        return;
      }

    }
    else if (Waretype == "observers")
    {
      if (Parser.command(ActiveCommandStr).isOptionActive("observers-paths"))
      {
        openfluid::base::RuntimeEnvironment::instance()->addExtraObserversPluginsPaths(
            Parser.command(ActiveCommandStr).getOptionValue("observers-paths"));
      }

      if (Parser.command(ActiveCommandStr).isOptionActive("list"))
      {
        m_RunType = InfoRequest;
        printMonitoring();
        return;
      }
      else
      {
        m_RunType = InfoRequest;
        printObserversReport(MatchStr);
        return;
      }
    }
  }
  else if (ActiveCommandStr == "buddy")
  {
    if (Parser.extraArgs().empty())
      throw openfluid::base::ApplicationException(
               openfluid::base::ApplicationException::computeContext("openfluid","command line parsing"),
                   "Buddy name is missing");

    openfluid::buddies::OpenFLUIDBuddy* BuddyBody = nullptr;
    openfluid::buddies::BuddiesListener* BuddyListener = new DefaultBuddiesListener();

    std::string BuddyName = Parser.extraArgs().at(0);

    if (BuddyName == "newsim")
      BuddyBody = new openfluid::buddies::NewSimulatorBuddy(BuddyListener);
    else if (BuddyName == "newdata")
      BuddyBody = new openfluid::buddies::NewDataBuddy(BuddyListener);
    else if (BuddyName == "examples")
      BuddyBody = new openfluid::buddies::ExamplesBuddy(BuddyListener);
#if !defined(OPENFLUID_OS_MAC)
    else if (BuddyName == "sim2doc")
      BuddyBody = new openfluid::buddies::Sim2DocBuddy(BuddyListener);
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
      openfluid::base::RuntimeEnvironment::instance()->addExtraSimulatorsPluginsPaths(
          Parser.command(ActiveCommandStr).getOptionValue("simulators-paths"));
    }

    if (Parser.command(ActiveCommandStr).isOptionActive("observers-paths"))
    {
      openfluid::base::RuntimeEnvironment::instance()->addExtraObserversPluginsPaths(
          Parser.command(ActiveCommandStr).getOptionValue("observers-paths"));
    }

    m_RunType = InfoRequest;
    printPaths();
    return;
  }
  else
    throw openfluid::base::ApplicationException(
                   openfluid::base::ApplicationException::computeContext("openfluid","command line parsing"),
                       "unknown command \"" + ActiveCommandStr + "\"");


}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::runBuddy()
{
  openfluid::buddies::OpenFLUIDBuddy* Buddy = NULL;
  openfluid::buddies::BuddiesListener* BuddyObs = new DefaultBuddiesListener();
  if (m_BuddyToRun.first == "newsim" ) Buddy = new openfluid::buddies::NewSimulatorBuddy(BuddyObs);
#ifndef __APPLE__
    // Disabled for compilation errors due to boost.spirit usage under MacOSX
    // TODO Should be re-enabled later
  if (m_BuddyToRun.first == "sim2doc" ) Buddy = new openfluid::buddies::Sim2DocBuddy(BuddyObs);
#endif
  if (m_BuddyToRun.first == "newdata" ) Buddy = new openfluid::buddies::NewDataBuddy(BuddyObs);
  if (m_BuddyToRun.first == "examples" ) Buddy = new openfluid::buddies::ExamplesBuddy(BuddyObs);

  if (Buddy != NULL)
  {
    Buddy->parseOptions(m_BuddyToRun.second);
    Buddy->run();
    delete Buddy;
  }
  else throw openfluid::base::ApplicationException(openfluid::base::ApplicationException::computeContext("openfluid"),
                                                   "Buddy " + m_BuddyToRun.first + " does not exists");

}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::run()
{

  if (m_RunType == Simulation)
  {
    runSimulation();
  }

  if (m_RunType == Buddy)
  {
    runBuddy();
  }
}






