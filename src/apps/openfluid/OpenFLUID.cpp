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
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <QElapsedTimer>

#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/base/ApplicationException.hpp>
#include <openfluid/tools/DataHelpers.hpp>
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


void OpenFLUIDApp::printSimulatorsHandledDataItemReport(openfluid::ware::SignatureHandledDataItem HandledItem,
                                                        std::string Suffix, std::string Type)
{
  std::string TypeStr = ("");

  std::cout << Suffix;

  std::string UnitStr = ("");
  std::string DistribStr = ("");


  if (HandledItem.DataUnit != ("")) UnitStr = (" (")+HandledItem.DataUnit+(")");
  if (HandledItem.UnitClass != ("")) DistribStr = "{"+HandledItem.UnitClass+"} ";


  if (Type == ("pvar")) TypeStr = ("produced variable");
  if (Type == ("uvar")) TypeStr = ("updated variable");

  if (Type == ("rvar")) TypeStr = ("required variable");
  if (Type == ("svar")) TypeStr = ("used variable (only if available)");

  if (Type == ("fpar")) TypeStr = ("simulator parameter");

  if (Type == ("pinput")) TypeStr = ("produced attribute");
  if (Type == ("rinput")) TypeStr = ("required attribute");
  if (Type == ("sinput")) TypeStr = ("used attribute");

  std::cout << DistribStr << HandledItem.DataName << UnitStr << " : " << TypeStr << ".";
  if (HandledItem.Description.length()!=0) std::cout << " " << HandledItem.Description;
  std::cout << std::endl;


}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printSimulatorsHandledUnitsGraphReport(openfluid::ware::SignatureHandledUnitsGraph HandledUnitsGraph,
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

  for (i=0;i<HandledData.SimulatorParams.size();i++)
    printSimulatorsHandledDataItemReport(HandledData.SimulatorParams[i],Suffix,("fpar"));

  for (i=0;i<HandledData.ProducedVars.size();i++)
    printSimulatorsHandledDataItemReport(HandledData.ProducedVars[i],Suffix,("pvar"));

  for (i=0;i<HandledData.RequiredVars.size();i++)
    printSimulatorsHandledDataItemReport(HandledData.RequiredVars[i],Suffix,("rvar"));

  for (i=0;i<HandledData.UpdatedVars.size();i++)
    printSimulatorsHandledDataItemReport(HandledData.UpdatedVars[i],Suffix,("uvar"));

  for (i=0;i<HandledData.UsedVars.size();i++)
    printSimulatorsHandledDataItemReport(HandledData.UsedVars[i],Suffix,("svar"));

  for (i=0;i<HandledData.ProducedAttribute.size();i++)
    printSimulatorsHandledDataItemReport(HandledData.ProducedAttribute[i],Suffix,("pinput"));

  for (i=0;i<HandledData.RequiredAttribute.size();i++)
    printSimulatorsHandledDataItemReport(HandledData.RequiredAttribute[i],Suffix,("rinput"));

  for (i=0;i<HandledData.UsedAttribute.size();i++)
    printSimulatorsHandledDataItemReport(HandledData.UsedAttribute[i],Suffix,("sinput"));

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

  if (openfluid::base::RuntimeEnvironment::instance()->isWriteResults() ||
      openfluid::base::RuntimeEnvironment::instance()->isWriteSimReport())
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

  if ((openfluid::base::RuntimeEnvironment::instance()->isWriteResults() ||
      openfluid::base::RuntimeEnvironment::instance()->isWriteSimReport()) &&
      (openfluid::base::RuntimeEnvironment::instance()->isClearOutputDir()))
    std::cout << "Output dir cleared before data saving" << std::endl;

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

  if (openfluid::base::RuntimeEnvironment::instance()->isWriteResults() ||
      openfluid::base::RuntimeEnvironment::instance()->isWriteSimReport()) std::cout << std::endl;

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

  // TODO adapt colon or semicolon separated path to system win32 or unix
  boost::program_options::options_description OptionsDesc("openfluid allowed options");
  OptionsDesc.add_options()
      ("auto-output-dir,a","generate automatic results output directory")
      ("buddy,b",boost::program_options::value< std::string >(),"run specified OpenFLUID buddy")
      ("buddyhelp",boost::program_options::value< std::string >(),"display help message for specified OpenFLUID buddy")
      ("buddyopts",boost::program_options::value< std::string >(),"set options for specified OpenFLUID buddy")
      ("clean-output-dir,c","clean results output directory by removing existing files")
      ("observers-list,e","list available observers (do not run the simulation)")
      ("simulators-list,f","list available simulators (do not run the simulation)")
      ("help,h", "display help message")
      ("input-dir,i",boost::program_options::value< std::string >(),"set dataset input directory")
      ("enable-simulation-profiling,k","enable time profiling for simulators")
      ("observers-report,l","print a report of available observers, with details (do not run the simulation)")
      ("observers-paths,n",boost::program_options::value< std::string >(),
                           "add extra observers search paths (colon separated)")
      ("output-dir,o",boost::program_options::value< std::string >(),"set results output directory")
      ("simulators-paths,p",boost::program_options::value< std::string >(),
                            "add extra simulators search paths (colon separated)")
      ("quiet,q","quiet display during simulation run")
      ("simulators-report,r","print a report of available simulators, with details (do not run the simulation)")
      ("no-simreport,s","do not generate simulation report")
      ("show-paths","print the used paths (do not run the simulation)")
      ("max-threads,t",boost::program_options::value< unsigned int >(),
                       std::string("change maximum number of threads for threaded spatial loops "
                        "(default is "+DefaultMaxThreadsStr+")").c_str())
      ("matching-simulators-report,u",boost::program_options::value< std::string >(),
                                      "print a report of simulators matching "
                                      "the given wildcard-based pattern (do not run the simulation)")
      ("verbose,v","verbose display during simulation")
      ("project,w",boost::program_options::value< std::string >(),"set project directory")
      ("version","get version (do not run the simulation)")
      ("no-result,z","do not write results files")
  ;

  boost::program_options::variables_map OptionsVars;

  boost::program_options::store(
      boost::program_options::parse_command_line(
          ArgC, ArgV, OptionsDesc,
          boost::program_options::command_line_style::default_style ^
          boost::program_options::command_line_style::allow_guessing),
          OptionsVars);

  boost::program_options::notify(OptionsVars);


  m_RunType = Simulation;

  if (OptionsVars.count("help"))
  {
    std::cout << OptionsDesc << std::endl;
    m_RunType = None;
    return;
  }


  if (OptionsVars.count("buddyhelp"))
  {
    openfluid::buddies::OpenFLUIDBuddy* Buddy = NULL;

    openfluid::buddies::BuddiesListener* BuddyObs = new DefaultBuddiesListener();

    if (OptionsVars["buddyhelp"].as<std::string>() == "newsim" )
      Buddy = new openfluid::buddies::NewSimulatorBuddy(BuddyObs);

#ifndef __APPLE__
    // Disabled for compilation errors due to boost.spirit usage under MacOSX
    // TODO Should be re-enabled later
    if (OptionsVars["buddyhelp"].as<std::string>() == "sim2doc" )
      Buddy = new openfluid::buddies::Sim2DocBuddy(BuddyObs);
#endif
    if (OptionsVars["buddyhelp"].as<std::string>() == "newdata" )
      Buddy = new openfluid::buddies::NewDataBuddy(BuddyObs);

    if (OptionsVars["buddyhelp"].as<std::string>() == "examples" )
      Buddy = new openfluid::buddies::ExamplesBuddy(BuddyObs);

    if (Buddy != NULL)
    {
      std::cout << "Options for buddy " + OptionsVars["buddyhelp"].as<std::string>() + ":" << std::endl;
      Buddy->invokeHelp();
      delete Buddy;
    }
    else throw openfluid::base::ApplicationException("openfluid",
                                                     "Buddy " + OptionsVars["buddyhelp"].as<std::string>() +
                                                     " does not exists");
    m_RunType = None;
    return;
  }


  if (OptionsVars.count("buddy"))
  {

    m_BuddyToRun.first = OptionsVars["buddy"].as<std::string>();

    if (OptionsVars.count("buddyopts"))
    {
      m_BuddyToRun.second = OptionsVars["buddyopts"].as<std::string>();
    }
    m_RunType = Buddy;
    return;
  }


  if (OptionsVars.count("version"))
  {
    std::cout << openfluid::config::FULL_VERSION << std::endl;
    m_RunType = InfoRequest;
    return;
  }

  if (OptionsVars.count("simulators-paths"))
  {
    openfluid::base::RuntimeEnvironment::instance()
    ->addExtraSimulatorsPluginsPaths(OptionsVars["simulators-paths"].as<std::string>());
  }

  if (OptionsVars.count("observers-paths"))
  {
    openfluid::base::RuntimeEnvironment::instance()
    ->addExtraObserversPluginsPaths(OptionsVars["observers-paths"].as<std::string>());
  }


  if (OptionsVars.count("observers-list"))
  {
    m_RunType = InfoRequest;
    printOpenFLUIDInfos();
    printMonitoring();
    return;
  }

  if (OptionsVars.count("simulators-list"))
  {
    m_RunType = InfoRequest;
    printOpenFLUIDInfos();
    printSimulatorsList();
    return;
  }

  if (OptionsVars.count("simulators-report"))
  {
    m_RunType = InfoRequest;
    printSimulatorsReport("");
    return;
  }

  if (OptionsVars.count("observers-report"))
  {
    m_RunType = InfoRequest;
    printObserversReport("");
    return;
  }

  if (OptionsVars.count("matching-simulators-report"))
  {
    m_RunType = InfoRequest;
    printSimulatorsReport(OptionsVars["matching-simulators-report"].as<std::string>());
    return;
  }

  if (OptionsVars.count("project"))
  {
    if (openfluid::base::ProjectManager::instance()->open(OptionsVars["project"].as<std::string>()))
    {
      openfluid::base::RuntimeEnvironment::instance()->linkToProject();
      openfluid::base::ProjectManager::instance()->updateOutputDir();
    }
    else
      throw openfluid::base::ApplicationException("openfluid",
                                                  OptionsVars["project"].as<std::string>() +
                                                  " is not a correct project path");
  }


  if (OptionsVars.count("input-dir"))
  {
    openfluid::base::RuntimeEnvironment::instance()->setInputDir(OptionsVars["input-dir"].as<std::string>());
  }

  if (OptionsVars.count("output-dir"))
  {
    openfluid::base::RuntimeEnvironment::instance()->setOutputDir(OptionsVars["output-dir"].as<std::string>());
  }

  if (OptionsVars.count("auto-output-dir"))
  {
    openfluid::base::RuntimeEnvironment::instance()->setDateTimeOutputDir();
  }

  if (OptionsVars.count("show-paths"))
  {
    m_RunType = InfoRequest;
    printPaths();
    return;
  }

  if (OptionsVars.count("max-threads"))
  {
    openfluid::base::RuntimeEnvironment::instance()
    ->setSimulatorsMaxNumThreads(OptionsVars["max-threads"].as<unsigned int>());
  }

  if (OptionsVars.count("clean-output-dir"))
  {
    openfluid::base::RuntimeEnvironment::instance()->setClearOutputDir(true);
  }

  if (OptionsVars.count("quiet"))
  {
    openfluid::base::RuntimeEnvironment::instance()->extraProperties().setValue("display.quiet",true);
    openfluid::base::RuntimeEnvironment::instance()->extraProperties().setValue("display.verbose",false);
  }

  if (OptionsVars.count("verbose"))
  {
    openfluid::base::RuntimeEnvironment::instance()->extraProperties().setValue("display.verbose",true);
    openfluid::base::RuntimeEnvironment::instance()->extraProperties().setValue("display.quiet",false);
  }

  if (OptionsVars.count("no-simreport"))
  {
    openfluid::base::RuntimeEnvironment::instance()->setWriteSimReport(false);
  }

  if (OptionsVars.count("no-result"))
  {
    openfluid::base::RuntimeEnvironment::instance()->setWriteResults(false);
  }

  if (OptionsVars.count("enable-simulation-profiling"))
  {
    openfluid::base::RuntimeEnvironment::instance()->setSimulationProfilingEnabled(true);
  }


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
  else throw openfluid::base::ApplicationException("openfluid","Buddy " + m_BuddyToRun.first + " does not exists");

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






