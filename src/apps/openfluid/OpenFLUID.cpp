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
  @file
  @brief implements OpenFLUID application

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/FunctionPluginsManager.hpp>
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
  std::cout << "                          software environment               " << std::endl;
  std::cout << "            for Modelling Fluxes in Landscapes             " << std::endl;
  std::cout << std::endl;
  std::cout << "                 LISAH, Montpellier, France                " << std::endl;
  std::cout << "===========================================================" << std::endl;
  std::cout << std::endl;
  std::cout.flush();
}



// =====================================================================
// =====================================================================


void OpenFLUIDApp::printFunctionsList()
{

  std::vector<openfluid::machine::ModelItemSignatureInstance*> PlugContainers =
    openfluid::machine::FunctionPluginsManager::getInstance()->getAvailableWaresSignatures();

  std::cout << "Available simulation functions:" << std::endl;

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
    openfluid::machine::ObserverPluginsManager::getInstance()->getAvailableWaresSignatures();

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

  std::cout << "   - Name: " << openfluid::tools::ReplaceEmptyString(Signature->Name,("(unknown)")) << std::endl;
  std::cout << "   - File: " << Filename << std::endl;
  std::cout << "   - Description: " << openfluid::tools::ReplaceEmptyString(Signature->Description,("(none)")) << std::endl;
  std::cout << "   - Version: " << openfluid::tools::ReplaceEmptyString(Signature->Version,("(unknown)")) << std::endl;
  std::cout << "   - SDK version used at build time: " << Signature->ABIVersion <<  std::endl;
  std::cout << "   - Development status: " << StatusStr <<  std::endl;
  std::cout << "   - Author(s): " << openfluid::tools::ReplaceEmptyString(Signature->Author,("(unknown)")) << std::endl;
  std::cout << "   - Author(s) email(s) : " << openfluid::tools::ReplaceEmptyString(Signature->AuthorEmail,("(unknown)")) << std::endl;
}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printFunctionsHandledDataItemReport(openfluid::ware::SignatureHandledDataItem HandledItem, std::string Suffix, std::string Type)
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

  if (Type == ("fpar")) TypeStr = ("function parameter");

  if (Type == ("pinput")) TypeStr = ("produced input data");
  if (Type == ("rinput")) TypeStr = ("required input data");
  if (Type == ("sinput")) TypeStr = ("used input data");

  std::cout << DistribStr << HandledItem.DataName << UnitStr << " : " << TypeStr << ".";
  if (HandledItem.Description.length()!=0) std::cout << " " << HandledItem.Description;
  std::cout << std::endl;


}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printFunctionsHandledUnitsGraphReport(openfluid::ware::SignatureHandledUnitsGraph HandledUnitsGraph, std::string Suffix)
{
  unsigned int i;
  if (!HandledUnitsGraph.UpdatedUnitsGraph.empty())
    std::cout << Suffix << "Global units graph updates: " << HandledUnitsGraph.UpdatedUnitsGraph << std::endl;

  for (i=0;i<HandledUnitsGraph.UpdatedUnitsClass.size();i++)
  {
    std::cout << Suffix << "Units graph update on class " << HandledUnitsGraph.UpdatedUnitsClass[i].UnitsClass << ": " << HandledUnitsGraph.UpdatedUnitsClass[i].Description << std::endl;
  }
}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printFunctionsTimeSchedulingReport(openfluid::ware::SignatureTimeScheduling TScheduling)
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


void OpenFLUIDApp::printFunctionsHandledDataReport(openfluid::ware::SignatureHandledData HandledData, std::string Suffix)
{

  unsigned int i;

  for (i=0;i<HandledData.FunctionParams.size();i++) printFunctionsHandledDataItemReport(HandledData.FunctionParams[i],Suffix,("fpar"));
  for (i=0;i<HandledData.ProducedVars.size();i++) printFunctionsHandledDataItemReport(HandledData.ProducedVars[i],Suffix,("pvar"));
  for (i=0;i<HandledData.RequiredVars.size();i++) printFunctionsHandledDataItemReport(HandledData.RequiredVars[i],Suffix,("rvar"));
  for (i=0;i<HandledData.UpdatedVars.size();i++) printFunctionsHandledDataItemReport(HandledData.UpdatedVars[i],Suffix,("uvar"));
  for (i=0;i<HandledData.UsedVars.size();i++) printFunctionsHandledDataItemReport(HandledData.UsedVars[i],Suffix,("svar"));
  for (i=0;i<HandledData.ProducedInputdata.size();i++) printFunctionsHandledDataItemReport(HandledData.ProducedInputdata[i],Suffix,("pinput"));
  for (i=0;i<HandledData.RequiredInputdata.size();i++) printFunctionsHandledDataItemReport(HandledData.RequiredInputdata[i],Suffix,("rinput"));
  for (i=0;i<HandledData.UsedInputdata.size();i++) printFunctionsHandledDataItemReport(HandledData.UsedInputdata[i],Suffix,("sinput"));

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

  for (i=0;i<HandledData.RequiredExtraFiles.size();i++) std::cout << Suffix << "Required extra file : " << HandledData.RequiredExtraFiles[i] << std::endl;
  for (i=0;i<HandledData.UsedExtraFiles.size();i++) std::cout << Suffix << "Used extra file : " << HandledData.UsedExtraFiles[i] << std::endl;

}



// =====================================================================
// =====================================================================


void OpenFLUIDApp::printFunctionsReport(const std::string Pattern)
{

  std::vector<openfluid::machine::ModelItemSignatureInstance*> PlugContainers =
      openfluid::machine::FunctionPluginsManager::getInstance()->getAvailableWaresSignatures(Pattern);
  std::string StatusStr;


  if (PlugContainers.size() > 0)
  {
    for (unsigned int i=0;i<PlugContainers.size();i++)
    {

      // Status string
      StatusStr = "experimental";
      if (PlugContainers[i]->Signature->Status == openfluid::ware::BETA) StatusStr = "beta";
      if (PlugContainers[i]->Signature->Status == openfluid::ware::STABLE) StatusStr = "stable";


      std::cout << "* " << PlugContainers[i]->Signature->ID << std::endl;
      std::cout << "   - Name: " << openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Name,("(unknown)")) << std::endl;
      std::cout << "   - File: " << PlugContainers[i]->Filename << std::endl;
      std::cout << "   - Domain: " << openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Domain,("(unknown)")) << std::endl;
      std::cout << "   - Process: " << openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Process,("(unknown)")) << std::endl;
      std::cout << "   - Method: " << openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Method,("(unknown)")) << std::endl;
      std::cout << "   - Description: " << openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Description,("(none)")) << std::endl;
      std::cout << "   - Version: " << openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Version,("(unknown)")) << std::endl;
      std::cout << "   - SDK version used at build time: " << PlugContainers[i]->Signature->ABIVersion <<  std::endl;
      std::cout << "   - Development status: " << StatusStr <<  std::endl;
      std::cout << "   - Author(s): " << openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->Author,("(unknown)")) << std::endl;
      std::cout << "   - Author(s) email(s) : " << openfluid::tools::ReplaceEmptyString(PlugContainers[i]->Signature->AuthorEmail,("(unknown)")) << std::endl;
      std::cout << "   - Time scheduling : ";
      printFunctionsTimeSchedulingReport(PlugContainers[i]->Signature->TimeScheduling);
      std::cout << "   - Handled data" << std::endl;
      printFunctionsHandledDataReport(PlugContainers[i]->Signature->HandledData,("     . "));
      std::cout << "   - Handled units graph" << std::endl;
      printFunctionsHandledUnitsGraphReport(PlugContainers[i]->Signature->HandledUnitsGraph,("     . "));


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
      openfluid::machine::ObserverPluginsManager::getInstance()->getAvailableWaresSignatures(Pattern);
  std::string StatusStr;


  if (PlugContainers.size() > 0)
  {
    for (unsigned int i=0;i<PlugContainers.size();i++)
    {
      std::cout << "* " << PlugContainers[i]->Signature->ID << std::endl;
      printWareInfosReport((openfluid::ware::WareSignature*)(PlugContainers[i]->Signature),PlugContainers[i]->Filename);

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
  std::vector<std::string> FunctionsPaths = openfluid::base::RuntimeEnvironment::getInstance()->getFunctionsPluginsPaths();
  std::vector<std::string> ObserversPaths = openfluid::base::RuntimeEnvironment::getInstance()->getObserversPluginsPaths();
  unsigned int i;

  std::cout << "Input dir: " << openfluid::base::RuntimeEnvironment::getInstance()->getInputDir() << std::endl;
  if (openfluid::base::RuntimeEnvironment::getInstance()->isWriteResults() || openfluid::base::RuntimeEnvironment::getInstance()->isWriteSimReport()) std::cout << "Output dir: " << openfluid::base::RuntimeEnvironment::getInstance()->getOutputDir() << std::endl;

  std::cout << "Functions search path(s):" << std::endl;
  for (i=0;i<FunctionsPaths.size();i++) std::cout << " #" << (i+1) << " " << FunctionsPaths[i] << std::endl;

  std::cout << "Observers search path(s):" << std::endl;
  for (i=0;i<ObserversPaths.size();i++) std::cout << " #" << (i+1) << " " << ObserversPaths[i] << std::endl;

  if (ShowTemp) std::cout << "Temp dir: " << openfluid::base::RuntimeEnvironment::getInstance()->getTempDir() << std::endl;
}

// =====================================================================
// =====================================================================


void OpenFLUIDApp::printEnvInfos()
{
  bool IsVerbose = false;
  openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().getValue("display.verbose",IsVerbose);
  bool IsQuiet = false;
  openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().getValue("display.quiet",IsQuiet);


  printPaths(false);
  if ((openfluid::base::RuntimeEnvironment::getInstance()->isWriteResults() || openfluid::base::RuntimeEnvironment::getInstance()->isWriteSimReport()) && (openfluid::base::RuntimeEnvironment::getInstance()->isClearOutputDir())) std::cout << "Output dir cleared before data saving" << std::endl;
  if (IsQuiet) std::cout << "Quiet mode enabled" << std::endl;
  if (IsVerbose) std::cout << "Verbose mode enabled" << std::endl;
  std::cout << std::endl;
}


// =====================================================================
// =====================================================================

void OpenFLUIDApp::runSimulation()
{

  bool IsVerbose = false;
  openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().getValue("display.verbose",IsVerbose);
  bool IsQuiet = false;
  openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().getValue("display.quiet",IsQuiet);

  m_FullStartTime = boost::posix_time::microsec_clock::local_time();

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
  FXDesc.loadFromDirectory(openfluid::base::RuntimeEnvironment::getInstance()->getInputDir());


  std::cout << "* Building spatial domain... "; std::cout.flush();
  openfluid::machine::Factory::buildSimulationBlobFromDescriptors(FXDesc,m_SimBlob);
  std::cout << "[OK]" << std::endl; std::cout.flush();


  std::cout << "* Building model... "; std::cout.flush();
  openfluid::machine::Factory::buildModelInstanceFromDescriptor(FXDesc.getModelDescriptor(),
                                                                Model);
  std::cout << "[OK]" << std::endl; std::cout.flush();

  std::cout << "* Building monitoring... "; std::cout.flush();
  openfluid::machine::Factory::buildMonitoringInstanceFromDescriptor(FXDesc.getMonitoringDescriptor(),
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
  std::cout << "Simulation ID: " << openfluid::base::RuntimeEnvironment::getInstance()->getSimulationID() << std::endl;
  std::cout << std::endl;

  unsigned int UnitsCount = 0;
  for (UnitsIt = m_SimBlob.getCoreRepository().getUnitsByClass()->begin(); UnitsIt != m_SimBlob.getCoreRepository().getUnitsByClass()->end();++UnitsIt )
  {
    UnitsCount = UnitsCount + (*UnitsIt).second.getList()->size();
  }

  std::cout << "Spatial domain, " << UnitsCount << " units :"<< std::endl;
  for (UnitsIt = m_SimBlob.getCoreRepository().getUnitsByClass()->begin(); UnitsIt != m_SimBlob.getCoreRepository().getUnitsByClass()->end();++UnitsIt )
  {
    std::cout << "  - " << (*UnitsIt).first << ", " << (*UnitsIt).second.getList()->size() << " units" << std::endl;
  }
  std::cout << std::endl;

  std::cout << "Simulation from " << mp_Engine->getSimulationStatus()->getBeginDate().getAsISOString()
            << " to " << mp_Engine->getSimulationStatus()->getEndDate().getAsISOString() << std::endl
            << "Default DeltaT is " << mp_Engine->getSimulationStatus()->getDefaultDeltaT() << " seconds" << std::endl;

  std::cout << std::endl;

  if (openfluid::base::RuntimeEnvironment::getInstance()->isUserValuesBufferSize())
    std::cout << "Buffers for variables set to " << openfluid::base::RuntimeEnvironment::getInstance()->getValuesBufferSize() << " time steps" <<  std::endl;
  else std::cout << "Buffers for variables set to full simulation" << std::endl;
  std::cout << "Buffers for output files set to " << openfluid::base::RuntimeEnvironment::getInstance()->getFilesBufferSize() << " bytes" << std::endl;

  std::cout << std::endl;

  std::cout.flush();

  std::cout << std::endl << "**** Running simulation ****" << std::endl;
  std::cout.flush();
  m_EffectiveStartTime = boost::posix_time::microsec_clock::local_time();
  mp_Engine->run();
  m_EffectiveEndTime = boost::posix_time::microsec_clock::local_time();
  std::cout << "**** Simulation completed ****" << std::endl << std::endl;std::cout << std::endl;
  std::cout.flush();
  openfluid::base::RuntimeEnvironment::getInstance()->setEffectiveSimulationDuration(m_EffectiveEndTime-m_EffectiveStartTime);

  m_FullEndTime = boost::posix_time::microsec_clock::local_time();

  if (openfluid::base::RuntimeEnvironment::getInstance()->isWriteResults() || openfluid::base::RuntimeEnvironment::getInstance()->isWriteSimReport()) std::cout << std::endl;

  boost::posix_time::time_duration FullSimDuration = m_FullEndTime - m_FullStartTime;

  printlnExecMessagesStats();

  std::cout << std::endl;

  std::cout << "Simulation run time: " << boost::posix_time::to_simple_string(openfluid::base::RuntimeEnvironment::getInstance()->getEffectiveSimulationDuration()) << std::endl;
  std::cout << "     Total run time: " << boost::posix_time::to_simple_string(FullSimDuration) << std::endl;
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
  openfluid::tools::ConvertValue(openfluid::config::SIMULATORS_MAXNUMTHREADS,&DefaultMaxThreadsStr);

  // TODO adapt colon or semicolon separated path to system win32 or unix
  boost::program_options::options_description OptionsDesc("openfluid allowed options");
  OptionsDesc.add_options()
      ("auto-output-dir,a","generate automatic results output directory")
      ("buddy,b",boost::program_options::value< std::string >(),"run specified OpenFLUID buddy")
      ("buddyhelp",boost::program_options::value< std::string >(),"display help message for specified OpenFLUID buddy")
      ("buddyopts",boost::program_options::value< std::string >(),"set options for specified OpenFLUID buddy")
      ("clean-output-dir,c","clean results output directory by removing existing files")
      ("observers-list,e","list available observers (do not run the simulation)")
      ("functions-list,f","list available functions (do not run the simulation)")
      ("help,h", "display help message")
      ("input-dir,i",boost::program_options::value< std::string >(),"set dataset input directory")
      ("enable-simulation-profiling,k","enable time profiling for functions")
      ("observers-report,l","print a report of available observers, with details (do not run the simulation)")
      ("observers-paths,n",boost::program_options::value< std::string >(),"add extra observers search paths (colon separated)")
      ("output-dir,o",boost::program_options::value< std::string >(),"set results output directory")
      ("functions-paths,p",boost::program_options::value< std::string >(),"add extra functions search paths (colon separated)")
      ("quiet,q","quiet display during simulation run")
      ("functions-report,r","print a report of available functions, with details (do not run the simulation)")
      ("no-simreport,s","do not generate simulation report")
      ("show-paths","print the used paths (do not run the simulation)")
      ("max-threads,t",boost::program_options::value< unsigned int >(),std::string("change maximum number of threads for threaded spatial loops (default is "+DefaultMaxThreadsStr+")").c_str())
      ("matching-functions-report,u",boost::program_options::value< std::string >(),"print a report of functions matching the given wildcard-based pattern (do not run the simulation)")
      ("verbose,v","verbose display during simulation")
      ("project,w",boost::program_options::value< std::string >(),"set project directory")
      ("version","get version (do not run the simulation)")
      ("no-result,z","do not write results files")
  ;

  boost::program_options::variables_map OptionsVars;
  boost::program_options::store(boost::program_options::parse_command_line(ArgC, ArgV, OptionsDesc,
		                                                                   boost::program_options::command_line_style::default_style ^ boost::program_options::command_line_style::allow_guessing),
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
    if (OptionsVars["buddyhelp"].as<std::string>() == "newfunc" ) Buddy = new openfluid::buddies::NewFunctionBuddy(BuddyObs);
    if (OptionsVars["buddyhelp"].as<std::string>() == "func2doc" ) Buddy = new openfluid::buddies::Func2DocBuddy(BuddyObs);
    if (OptionsVars["buddyhelp"].as<std::string>() == "convert" ) Buddy = new openfluid::buddies::ConvertBuddy(BuddyObs);
    if (OptionsVars["buddyhelp"].as<std::string>() == "newdata" ) Buddy = new openfluid::buddies::NewDataBuddy(BuddyObs);
    if (OptionsVars["buddyhelp"].as<std::string>() == "examples" ) Buddy = new openfluid::buddies::ExamplesBuddy(BuddyObs);


    if (Buddy != NULL)
    {
      std::cout << "Options for buddy " + OptionsVars["buddyhelp"].as<std::string>() + ":" << std::endl;
      Buddy->invokeHelp();
      delete Buddy;
    }
    else throw openfluid::base::OFException("openfluid","Buddy " + OptionsVars["buddyhelp"].as<std::string>() + " does not exists");
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

  if (OptionsVars.count("functions-paths"))
  {
    openfluid::base::RuntimeEnvironment::getInstance()->addExtraFunctionsPluginsPaths(OptionsVars["functions-paths"].as<std::string>());
  }

  if (OptionsVars.count("observers-paths"))
  {
    openfluid::base::RuntimeEnvironment::getInstance()->addExtraObserversPluginsPaths(OptionsVars["observers-paths"].as<std::string>());
  }


  if (OptionsVars.count("observers-list"))
  {
    m_RunType = InfoRequest;
    printOpenFLUIDInfos();
    printMonitoring();
    return;
  }

  if (OptionsVars.count("functions-list"))
  {
    m_RunType = InfoRequest;
    printOpenFLUIDInfos();
    printFunctionsList();
    return;
  }

  if (OptionsVars.count("functions-report"))
  {
    m_RunType = InfoRequest;
    printFunctionsReport("");
    return;
  }

  if (OptionsVars.count("observers-report"))
  {
    m_RunType = InfoRequest;
    printObserversReport("");
    return;
  }

  if (OptionsVars.count("matching-functions-report"))
  {
    m_RunType = InfoRequest;
    printFunctionsReport(OptionsVars["matching-functions-report"].as<std::string>());
    return;
  }

  if (OptionsVars.count("project"))
  {
    if (openfluid::base::ProjectManager::getInstance()->open(OptionsVars["project"].as<std::string>()))
    {
      openfluid::base::RuntimeEnvironment::getInstance()->linkToProject();
      openfluid::base::ProjectManager::getInstance()->updateOutputDir();
    }
    else
      throw openfluid::base::OFException("openfluid",OptionsVars["project"].as<std::string>() + " is not a correct project path");
  }


  if (OptionsVars.count("input-dir"))
  {
    openfluid::base::RuntimeEnvironment::getInstance()->setInputDir(OptionsVars["input-dir"].as<std::string>());
  }

  if (OptionsVars.count("output-dir"))
  {
    openfluid::base::RuntimeEnvironment::getInstance()->setOutputDir(OptionsVars["output-dir"].as<std::string>());
  }

  if (OptionsVars.count("auto-output-dir"))
  {
    openfluid::base::RuntimeEnvironment::getInstance()->setDateTimeOutputDir();
  }

  if (OptionsVars.count("show-paths"))
  {
    m_RunType = InfoRequest;
    printPaths();
    return;
  }

  if (OptionsVars.count("max-threads"))
  {
    openfluid::base::RuntimeEnvironment::getInstance()->setFunctionsMaxNumThreads(OptionsVars["max-threads"].as<unsigned int>());
  }

  if (OptionsVars.count("clean-output-dir"))
  {
    openfluid::base::RuntimeEnvironment::getInstance()->setClearOutputDir(true);
  }

  if (OptionsVars.count("quiet"))
  {
    openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().setValue("display.quiet",true);
    openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().setValue("display.verbose",false);
  }

  if (OptionsVars.count("verbose"))
  {
    openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().setValue("display.verbose",true);
    openfluid::base::RuntimeEnvironment::getInstance()->getExtraProperties().setValue("display.quiet",false);
  }

  if (OptionsVars.count("no-simreport"))
  {
    openfluid::base::RuntimeEnvironment::getInstance()->setWriteSimReport(false);
  }

  if (OptionsVars.count("no-result"))
  {
    openfluid::base::RuntimeEnvironment::getInstance()->setWriteResults(false);
  }

  if (OptionsVars.count("enable-simulation-profiling"))
  {
    openfluid::base::RuntimeEnvironment::getInstance()->setSimulationProfilingEnabled(true);
  }


}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::runBuddy()
{
  openfluid::buddies::OpenFLUIDBuddy* Buddy = NULL;
  openfluid::buddies::BuddiesListener* BuddyObs = new DefaultBuddiesListener();
  if (m_BuddyToRun.first == "newfunc" ) Buddy = new openfluid::buddies::NewFunctionBuddy(BuddyObs);
  if (m_BuddyToRun.first == "func2doc" ) Buddy = new openfluid::buddies::Func2DocBuddy(BuddyObs);
  if (m_BuddyToRun.first == "convert" ) Buddy = new openfluid::buddies::ConvertBuddy(BuddyObs);
  if (m_BuddyToRun.first == "newdata" ) Buddy = new openfluid::buddies::NewDataBuddy(BuddyObs);
  if (m_BuddyToRun.first == "examples" ) Buddy = new openfluid::buddies::ExamplesBuddy(BuddyObs);

  if (Buddy != NULL)
  {
    Buddy->parseOptions(m_BuddyToRun.second);
    Buddy->run();
    delete Buddy;
  }
  else throw openfluid::base::OFException("openfluid","Buddy " + m_BuddyToRun.first + " does not exists");

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



// =====================================================================
// =====================================================================





