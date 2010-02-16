/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OpenFLUID.cpp
  \brief implements OpenFLUID application

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "AppTools.h"
#include "OpenFLUID.h"
#include "openfluid-base.h"

#include "buddies/NewFuncBuddy.h"
#include "buddies/Func2DocBuddy.h"
#include "buddies/NewDataBuddy.h"
#include "buddies/ConvertBuddy.h"


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


void OpenFLUIDApp::printlnExecStatus()
{
  if (m_ExecMsgs.isWarningFlag()) std::cout << "[Warning]" << std::endl;
  else std::cout << "[OK]" << std::endl;

  std::cout.flush();
}

// =====================================================================
// =====================================================================


void OpenFLUIDApp::printlnExecMessagesStats()
{
  std::cout << m_ExecMsgs.getWarningsCount() << " warning(s)" << std::endl;
}

// =====================================================================
// =====================================================================


void OpenFLUIDApp::printOpenFLUIDInfos()
{

  int Width = 60;
  std::string VersionInfo = "OpenFLUID-engine v";
  std::string Whites = "";

  VersionInfo = VersionInfo + CONFIG_FULL_VERSION;

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


void OpenFLUIDApp::printPluginsList()
{

  ArrayOfModelItemInstance PlugContainers = PluginManager(&m_RunEnv).getAvailableFunctions();

  std::cout << "Available simulation functions:" << std::endl;

  bool OneAtLeast = false;

  for (unsigned int i=0;i<PlugContainers.size();i++)
  {
    if (PlugContainers[i]->SDKCompatible && PlugContainers[i]->Signature!=NULL)
    {
      boost::filesystem::path TmpPath(PlugContainers[i]->Filename);
      TmpPath = boost::filesystem::change_extension(TmpPath,"");
      std::cout << "  - " << ReplaceEmptyString(TmpPath.leaf(),"(unknown simulation function)") << std::endl;
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



void OpenFLUIDApp::printPluginsHandledDataItemReport(openfluid::base::SignatureHandledDataItem HandledItem, std::string Suffix, std::string Type, bool IsXMLFormat)
{
  std::string TypeStr = ("");

  std::cout << Suffix;

  if (IsXMLFormat)
  {
    std::cout << "<vardata";
    std::cout << " type=\"" << Type << "\"";
    std::cout << " unitclass=\"" << HandledItem.UnitClass << "\"";
    std::cout << " name=\"" << HandledItem.DataName << "\"";
    std::cout << " description=\"" << HandledItem.Description << "\"";
    std::cout << " unit=\"" << HandledItem.DataUnit << "\"";
    std::cout << "/>" << std::endl;

  }
  else
  {
    std::string UnitStr = ("");
    std::string DistribStr = ("");


    if (HandledItem.DataUnit != ("")) UnitStr = (" (")+HandledItem.DataUnit+(")");
    if (HandledItem.UnitClass != ("")) DistribStr = "{"+HandledItem.UnitClass+"} ";


    if (Type == ("pvar")) TypeStr = ("produced variable");
    if (Type == ("uvar")) TypeStr = ("updated variable");

    if (Type == ("rvar")) TypeStr = ("required variable");
    if (Type == ("rprevvar")) TypeStr = ("required variable produced at previous step");
    if (Type == ("svar")) TypeStr = ("used variable (only if available)");
    if (Type == ("sprevvar")) TypeStr = ("used variable produced at previous step (only if available)");

    if (Type == ("fpar")) TypeStr = ("function parameter");

    if (Type == ("rinput")) TypeStr = ("required input data");
    if (Type == ("sinput")) TypeStr = ("used input data");

    std::cout << DistribStr << HandledItem.DataName << UnitStr << " : " << TypeStr << ".";
    if (HandledItem.Description.length()!=0) std::cout << " " << HandledItem.Description;
    std::cout << std::endl;

  }


}



// =====================================================================
// =====================================================================


void OpenFLUIDApp::printPluginsHandledDataReport(openfluid::base::SignatureHandledData HandledData, std::string Suffix, bool IsXMLFormat)
{

  unsigned int i;

  for (i=0;i<HandledData.FunctionParams.size();i++) printPluginsHandledDataItemReport(HandledData.FunctionParams[i],Suffix,("fpar"),IsXMLFormat);
  for (i=0;i<HandledData.ProducedVars.size();i++) printPluginsHandledDataItemReport(HandledData.ProducedVars[i],Suffix,("pvar"),IsXMLFormat);
  for (i=0;i<HandledData.RequiredVars.size();i++) printPluginsHandledDataItemReport(HandledData.RequiredVars[i],Suffix,("rvar"),IsXMLFormat);
  for (i=0;i<HandledData.UpdatedVars.size();i++) printPluginsHandledDataItemReport(HandledData.UpdatedVars[i],Suffix,("uvar"),IsXMLFormat);
  for (i=0;i<HandledData.UsedVars.size();i++) printPluginsHandledDataItemReport(HandledData.UsedVars[i],Suffix,("svar"),IsXMLFormat);
  for (i=0;i<HandledData.RequiredPrevVars.size();i++) printPluginsHandledDataItemReport(HandledData.RequiredPrevVars[i],Suffix,("rprevvar"),IsXMLFormat);
  for (i=0;i<HandledData.UsedPrevVars.size();i++) printPluginsHandledDataItemReport(HandledData.UsedPrevVars[i],Suffix,("sprevvar"),IsXMLFormat);
  for (i=0;i<HandledData.RequiredInput.size();i++) printPluginsHandledDataItemReport(HandledData.RequiredInput[i],Suffix,("rinput"),IsXMLFormat);
  for (i=0;i<HandledData.UsedInput.size();i++) printPluginsHandledDataItemReport(HandledData.UsedInput[i],Suffix,("sinput"),IsXMLFormat);

  if (IsXMLFormat)
  {
    for (i=0;i<HandledData.UsedEventsOnUnits.size();i++)
      std::cout << Suffix << "<usedevents unitclass=\"" << HandledData.UsedEventsOnUnits[i] << "\" />" << std::endl;

    for (i=0;i<HandledData.RequiredExtraFiles.size();i++) std::cout << Suffix << "<extrafile type=\"required\" name=\"" << HandledData.RequiredExtraFiles[i] << "\" />" << std::endl;
    for (i=0;i<HandledData.UsedExtraFiles.size();i++) std::cout << Suffix << "<extrafile type=\"used\" name=\"" << HandledData.UsedExtraFiles[i] << "\" />" << std::endl;

  }
  else
  {
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

}


// =====================================================================
// =====================================================================

void OpenFLUIDApp::printPluginsReport(bool IsXMLFormat, const std::string Pattern)
{

  ArrayOfModelItemInstance PlugContainers = PluginManager(&m_RunEnv).getAvailableFunctions(Pattern);
  std::string StatusStr;


  // XML header + tags
  if (IsXMLFormat)
  {
    std::cout << "<?xml version=\"1.0\" standalone=\"yes\"?>" << std::endl;
    std::cout << "<openfluid>" << std::endl;
    std::cout << "  <funcsreport>" << std::endl;
  }



  if (PlugContainers.size() > 0)
  {
    for (unsigned int i=0;i<PlugContainers.size();i++)
    {

      // Status string
      StatusStr = "experimental";
      if (PlugContainers[i]->Signature->Status == openfluid::base::BETA) StatusStr = "beta";
      if (PlugContainers[i]->Signature->Status == openfluid::base::STABLE) StatusStr = "stable";

      if (IsXMLFormat)

      {
        std::cout << "    <funcdef fileID=\"" << PlugContainers[i]->Signature->ID
                  << "\" name=\"" << PlugContainers[i]->Signature->Name << "\">" << std::endl;
        std::cout << "      <file>" << PlugContainers[i]->Filename << "</file>" << std::endl;
        std::cout << "      <domain>" << PlugContainers[i]->Signature->Domain << "</domain>" << std::endl;
        std::cout << "      <process>" << PlugContainers[i]->Signature->Process << "</process>" << std::endl;
        std::cout << "      <method>" << PlugContainers[i]->Signature->Method << "</method>" << std::endl;
        std::cout << "      <description>" << PlugContainers[i]->Signature->Description << "</description>" << std::endl;
        std::cout << "      <version number=\"" << PlugContainers[i]->Signature->Version << "\" sdk=\""<< PlugContainers[i]->Signature->SDKVersion << "\" devstatus=\"" << StatusStr << "\"/>" << std::endl;
        std::cout << "      <author name=\"" << PlugContainers[i]->Signature->Author
                  << "\" email=\"" << PlugContainers[i]->Signature->AuthorEmail << "\"/>" << std::endl;

        std::cout << "      <handleddata>" << std::endl;
        printPluginsHandledDataReport(PlugContainers[i]->Signature->HandledData,("        "),IsXMLFormat);
        std::cout << "      </handleddata>" << std::endl;

      }
      else
      {
        std::cout << "* " << PlugContainers[i]->Signature->ID << std::endl;
        std::cout << "   - Name: " << ReplaceEmptyString(PlugContainers[i]->Signature->Name,("(unknown)")) << std::endl;
        std::cout << "   - File: " << PlugContainers[i]->Filename << std::endl;
        std::cout << "   - Domain: " << ReplaceEmptyString(PlugContainers[i]->Signature->Domain,("(unknown)")) << std::endl;
        std::cout << "   - Process: " << ReplaceEmptyString(PlugContainers[i]->Signature->Process,("(unknown)")) << std::endl;
        std::cout << "   - Method: " << ReplaceEmptyString(PlugContainers[i]->Signature->Method,("(unknown)")) << std::endl;
        std::cout << "   - Description: " << ReplaceEmptyString(PlugContainers[i]->Signature->Description,("(none)")) << std::endl;
        std::cout << "   - Version: " << ReplaceEmptyString(PlugContainers[i]->Signature->Method,("(unknown)")) << std::endl;
        std::cout << "   - SDK version used at build time: " << PlugContainers[i]->Signature->SDKVersion <<  std::endl;
        std::cout << "   - Development status: " << StatusStr <<  std::endl;
        std::cout << "   - Author(s): " << ReplaceEmptyString(PlugContainers[i]->Signature->Author,("(unknown)")) << std::endl;
        std::cout << "   - Author(s) email(s) : " << ReplaceEmptyString(PlugContainers[i]->Signature->AuthorEmail,("(unknown)")) << std::endl;
        std::cout << "   - Handled data" << std::endl;
        printPluginsHandledDataReport(PlugContainers[i]->Signature->HandledData,("     . "),IsXMLFormat);

      }

      if (IsXMLFormat)
      {
        std::cout << "    </funcdef> " << std::endl;
      }
      else
      {
        if (i != PlugContainers.size()-1)
          std::cout << "================================================================================" << std::endl;
      }
    }

  }
  else
  {

  }

  // XML end
  if (IsXMLFormat)
  {
    std::cout << "  </funcsreport>" << std::endl;
    std::cout << "</openfluid>" << std::endl;
  }

  std::cout.flush();


}

// =====================================================================
// =====================================================================

int OpenFLUIDApp::stopAppReturn(std::string Msg)
{
  std::cout << std::endl;

  printlnExecMessagesStats();

  if (mp_Engine != NULL) mp_Engine->saveMessages();

  if (mp_Engine != NULL && m_RunEnv.isWriteSimReport())
  {
    std::cout << "* Saving simulation report... "; std::cout.flush();
    mp_Engine->saveReports("");
    std::cout << "[Done]" << std::endl; std::cout.flush();
    m_ExecMsgs.resetWarningFlag();
  }

  std::cout << std::endl << Msg << std::endl;

  std::cout << std::endl;
  std::cout.flush();

  return 127;

}

// =====================================================================
// =====================================================================


void OpenFLUIDApp::printPaths(bool ShowTemp)
{
  std::vector<std::string> FunctionsPaths = m_RunEnv.getPluginsPaths();
  unsigned int i;

  std::cout << "Input dir: " << m_RunEnv.getInputDir() << std::endl;
  if (m_RunEnv.isWriteResults() || m_RunEnv.isWriteSimReport()) std::cout << "Output dir: " << m_RunEnv.getOutputDir() << std::endl;
  std::cout << "Functions search path(s):" << std::endl;
  for (i=0;i<FunctionsPaths.size();i++) std::cout << " #" << (i+1) << " " << FunctionsPaths[i] << std::endl;
  if (ShowTemp) std::cout << "Temp dir: " << m_RunEnv.getTempDir() << std::endl;
}

// =====================================================================
// =====================================================================


void OpenFLUIDApp::printEnvInfos()
{
  printPaths(false);
  if ((m_RunEnv.isWriteResults() || m_RunEnv.isWriteSimReport()) && (m_RunEnv.isClearOutputDir())) std::cout << "Output dir cleared before data saving" << std::endl;
  if (m_RunEnv.isQuietRun()) std::cout << "Quiet mode enabled" << std::endl;
  if (m_RunEnv.isVerboseRun()) std::cout << "Verbose mode enabled" << std::endl;
  if (!m_RunEnv.isCheckVarNames()) std::cout << "Variable names checking disabled" << std::endl;
  std::cout << std::endl;
}


// =====================================================================
// =====================================================================

void OpenFLUIDApp::runSimulation()
{

  m_FullStartTime = boost::posix_time::microsec_clock::local_time();

  PluginManager PlugMan(&m_RunEnv);

  openfluid::core::CoreRepository* pCoreData = openfluid::core::CoreRepository::getInstance();
  pCoreData->setMemoryMonitor(openfluid::core::MemoryMonitor::getInstance());

  mp_Engine = new Engine(pCoreData,&m_ExecMsgs,&m_RunEnv,&PlugMan);


  printOpenFLUIDInfos();
  printEnvInfos();


  std::cout << "* Building model... "; std::cout.flush();
  mp_Engine->buildModel();
  printlnExecStatus();
  m_ExecMsgs.resetWarningFlag();


  std::cout << "* Loading data... "; std::cout.flush();
  mp_Engine->loadData();
  printlnExecStatus();
  m_ExecMsgs.resetWarningFlag();


  std::cout << "* Preparing data and checking consistency... "; std::cout.flush();
  mp_Engine->prepareDataAndCheckConsistency();
  if (!m_RunEnv.isVerboseRun()) printlnExecStatus();
  else std::cout << std::endl;
  m_ExecMsgs.resetWarningFlag();


  openfluid::core::UnitsListByClassMap_t::const_iterator UnitsIt;

  std::cout << std::endl;
  std::cout << "Simulation ID: " << m_RunEnv.getSimulationID() << std::endl;
  std::cout << std::endl;

  std::cout << "Spatial domain: " << std::endl;
  for (UnitsIt = pCoreData->getUnits()->begin(); UnitsIt != pCoreData->getUnits()->end();++UnitsIt )
  {
    std::cout << "  - " << (*UnitsIt).first << ", " << (*UnitsIt).second.getList()->size() << " units" << std::endl;
  }
  std::cout << std::endl;

  std::cout << "Simulation from " << mp_Engine->getSimulationInfo()->getStartTime().getAsISOString()
            << " to " << mp_Engine->getSimulationInfo()->getEndTime().getAsISOString() << std::endl
            << "         -> " <<  (mp_Engine->getSimulationInfo()->getStepsCount()) << " time steps of " << mp_Engine->getSimulationInfo()->getTimeStep() << " seconds" << std::endl;

  std::cout << std::endl;

  if (m_RunEnv.isProgressiveOutput()) std::cout << "Progressive output enabled (Packet=" << m_RunEnv.getProgressiveOutputPacket() << ", Keep=" << m_RunEnv.getProgressiveOutputKeep() << ")" <<  std::endl;
  else std::cout << "Progressive output disabled" << std::endl;

  std::cout << std::endl;

  std::cout.flush();

  std::cout << std::endl << "**** Running simulation ****" << std::endl;
  std::cout.flush();
  m_EffectiveStartTime = boost::posix_time::microsec_clock::local_time();
  mp_Engine->run();
  m_EffectiveEndTime = boost::posix_time::microsec_clock::local_time();
  std::cout << "**** Simulation completed ****" << std::endl << std::endl;std::cout << std::endl;
  std::cout.flush();
  m_ExecMsgs.resetWarningFlag();
  m_RunEnv.setEffectiveSimulationDuration(m_EffectiveEndTime-m_EffectiveStartTime);


  if (m_RunEnv.isWriteSimReport())
  {
    std::cout << "* Saving simulation report... "; std::cout.flush();
    mp_Engine->saveReports("");
    std::cout << "[Done]" << std::endl; std::cout.flush();
    m_ExecMsgs.resetWarningFlag();
  }


  m_FullEndTime = boost::posix_time::microsec_clock::local_time();

  if (m_RunEnv.isWriteResults() || m_RunEnv.isWriteSimReport()) std::cout << std::endl;

  boost::posix_time::time_duration FullSimDuration = m_FullEndTime - m_FullStartTime;

  printlnExecMessagesStats();

  std::cout << std::endl;

  std::cout << "Simulation run time: " << boost::posix_time::to_simple_string(m_RunEnv.getEffectiveSimulationDuration()) << std::endl;
  std::cout << "     Total run time: " << boost::posix_time::to_simple_string(FullSimDuration) << std::endl;
  std::cout << std::endl;


  delete mp_Engine;
  mp_Engine = NULL;

}

// =====================================================================
// =====================================================================


void OpenFLUIDApp::processOptions(int ArgC, char **ArgV)
{


  boost::program_options::options_description OptionsDesc("openfluid-engine allowed options");
  OptionsDesc.add_options()
      ("auto-output-dir,a","generate automatic results output directory")
      ("buddy,b",boost::program_options::value< std::string >(),"run specified OpenFLUID buddy")
      ("buddyhelp",boost::program_options::value< std::string >(),"display help message for specified OpenFLUID buddy")
      ("buddyopts",boost::program_options::value< std::string >(),"set options for specified OpenFLUID buddy")
      ("clean-output-dir,c","clean results output directory by removing existing files")
      ("functions-list,f","list available functions (do not run the simulation)")
      ("help,h", "display help message")
      ("input-dir,i",boost::program_options::value< std::string >(),"set dataset input directory")
      ("no-varname-check","do not check variable name against nomenclature")
      ("output-dir,o",boost::program_options::value< std::string >(),"set results output directory")
      ("functions-paths,p",boost::program_options::value< std::string >(),"add extra functions research paths (colon separated)")
      ("quiet,q","quiet display during simulation run")
      ("functions-report,r","print a report of available functions, with details (do not run the simulation)")
      ("no-simreport,s","do not generate simulation report")
      ("show-paths","print the used paths (do not run the simulation)")
      ("matching-functions-report,u",boost::program_options::value< std::string >(),"print a report of functions matching the given wildcard-based pattern (do not run the simulation)")
      ("verbose,v","verbose display during simulation")
      ("version","get version (do not run the simulation)")
      ("xml-functions-report,x","print a report of available functions in xml format, with details (do not run the simulation)")
      ("no-result,z","do not write results files")
  ;

  boost::program_options::variables_map OptionsVars;
  boost::program_options::store(boost::program_options::parse_command_line(ArgC, ArgV, OptionsDesc), OptionsVars);
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

    OpenFLUIDBuddy* Buddy = NULL;
    if (OptionsVars["buddyhelp"].as<std::string>() == "newfunc" ) Buddy = new NewFunctionBuddy();
    if (OptionsVars["buddyhelp"].as<std::string>() == "func2doc" ) Buddy = new Func2DocBuddy();
    if (OptionsVars["buddyhelp"].as<std::string>() == "convert" ) Buddy = new ConvertBuddy();
    if (OptionsVars["buddyhelp"].as<std::string>() == "newdata" ) Buddy = new NewDataBuddy();


    if (Buddy != NULL)
    {
      std::cout << "Options for buddy " + OptionsVars["buddyhelp"].as<std::string>() + ":" << std::endl;
      Buddy->displayHelp();
      delete Buddy;
    }
    else throw openfluid::base::OFException("Buddy " + OptionsVars["buddyhelp"].as<std::string>() + " does not exists");
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
    std::cout << CONFIG_FULL_VERSION << std::endl;
    m_RunType = InfoRequest;
    return;
  }

  if (OptionsVars.count("functions-paths"))
  {
    m_RunEnv.addExtraPluginsPaths(OptionsVars["functions-paths"].as<std::string>());
  }


  if (OptionsVars.count("functions-list"))
  {
    m_RunType = InfoRequest;
    printOpenFLUIDInfos();
    printPluginsList();
    return;
  }

  if (OptionsVars.count("functions-report"))
  {
    m_RunType = InfoRequest;
    printPluginsReport(false,"");
    return;
  }

  if (OptionsVars.count("matching-functions-report"))
  {
    m_RunType = InfoRequest;
    printPluginsReport(false,OptionsVars["matching-functions-report"].as<std::string>());
    return;
  }

  if (OptionsVars.count("xml-functions-report"))
  {
    m_RunType = InfoRequest;
    printPluginsReport(true,"");
    return;
  }

  if (OptionsVars.count("input-dir"))
  {
    m_RunEnv.setInputDir(OptionsVars["input-dir"].as<std::string>());
  }

  if (OptionsVars.count("output-dir"))
  {
    m_RunEnv.setOutputDir(OptionsVars["output-dir"].as<std::string>());
  }

  if (OptionsVars.count("auto-output-dir"))
  {
    m_RunEnv.setDateTimeOutputDir();
  }

  if (OptionsVars.count("show-paths"))
  {
    m_RunType = InfoRequest;
    printPaths();
    return;
  }

  if (OptionsVars.count("clean-output-dir"))
  {
    m_RunEnv.setClearOutputDir(true);
  }

  if (OptionsVars.count("quiet"))
  {
    m_RunEnv.setQuietRun(true);
  }

  if (OptionsVars.count("verbose"))
  {
    m_RunEnv.setVerboseRun(true);
  }

  if (OptionsVars.count("no-simreport"))
  {
    m_RunEnv.setWriteSimReport(false);
  }

  if (OptionsVars.count("no-result"))
  {
    m_RunEnv.setWriteResults(false);
  }

  if (OptionsVars.count("no-varname-check"))
  {
    m_RunEnv.setCheckVarNames(false);
  }

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
    OpenFLUIDBuddy* Buddy = NULL;
    if (m_BuddyToRun.first == "newfunc" ) Buddy = new NewFunctionBuddy();
    if (m_BuddyToRun.first == "func2doc" ) Buddy = new Func2DocBuddy();
    if (m_BuddyToRun.first == "convert" ) Buddy = new ConvertBuddy();
    if (m_BuddyToRun.first == "newdata" ) Buddy = new NewDataBuddy();

    if (Buddy != NULL)
    {
      Buddy->parseOptions(m_BuddyToRun.second);
      Buddy->run();
      delete Buddy;
    }
    else throw openfluid::base::OFException("Buddy " + m_BuddyToRun.first + " does not exists");

  }
}



// =====================================================================
// =====================================================================





