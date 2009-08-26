/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file main.cpp
  \brief implements main

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <iostream>

#include <wx/filename.h>


#include "AppTools.h"
#include "main.h"
#include "config.h"



// =====================================================================
// =====================================================================


IMPLEMENT_APP_CONSOLE(OpenFLUIDApp)

// =====================================================================
// =====================================================================


bool OpenFLUIDApp::buildModel()
{
  bool ExecStatus = true;

  std::cout << "* Building model... ";
  std::cout.flush();

  mp_Engine->buildModel();

  printlnExecStatus();

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool OpenFLUIDApp::loadData()
{
  bool ExecStatus;

  std::cout << "* Loading data... ";
  std::cout.flush();

  ExecStatus = mp_Engine->loadData();

  printlnExecStatus();

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool OpenFLUIDApp::checkConsistency()
{

  bool ExecStatus;

  std::cout << "* Preparing data and checking consistency... ";
  std::cout.flush();


  ExecStatus = mp_Engine->prepareDataAndCheckConsistency();


  if (!mp_RunEnv->isVerboseRun()) printlnExecStatus();
  else std::cout << std::endl;

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool OpenFLUIDApp::runSimulation()
{

  bool ExecStatus;

  printDataInfos();

  std::cout << std::endl << "**** Running simulation ****" << std::endl;
  std::cout.flush();

  m_EffectiveStartTime = boost::posix_time::microsec_clock::local_time();

  ExecStatus = mp_Engine->run();

  m_EffectiveEndTime = boost::posix_time::microsec_clock::local_time();

  std::cout << "**** Simulation completed ****" << std::endl << std::endl;
//  else  std::cout << "**** Simulation aborted ****" << std::endl;
  std::cout << std::endl;
  std::cout.flush();

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool OpenFLUIDApp::saveSimulationReports(std::string ErrorMsg)
{

  bool ExecStatus;

  std::cout << "* Saving simulation report... ";
  std::cout.flush();

  ExecStatus = mp_Engine->saveReports(ErrorMsg);

  std::cout << "[Done]" << std::endl;

  std::cout.flush();

  return ExecStatus;

  return true;
}


// =====================================================================
// =====================================================================


void OpenFLUIDApp::printlnExecStatus()
{
  if (mp_ExecMsgs->isWarningFlag()) std::cout << "[Warning]" << std::endl;
  else std::cout << "[OK]" << std::endl;

  std::cout.flush();
}

// =====================================================================
// =====================================================================


void OpenFLUIDApp::printlnExecMessagesStats()
{
  std::cout << mp_ExecMsgs->getWarningsCount() << " warning(s)" << std::endl;
}

// =====================================================================
// =====================================================================


void OpenFLUIDApp::printOpenFLUIDInfos()
{

  int Width = 60;
  std::string VersionInfo = "OpenFLUID-engine v";
  std::string Whites = "";

//  VersionInfo = VersionInfo + MAJOR_VERSION + wxT(".") + MINOR_VERSION + RELEASE_STATUS;
  VersionInfo = VersionInfo + FULL_VERSION;

/*
  if (BUILD_VERSION != wxT(""))
  {

    VersionInfo = VersionInfo + wxT(" (") + BUILD_VERSION + wxT(" build");

    #ifdef SVN_REV
    VersionInfo = VersionInfo + wxT(", rev ") + SVN_REVISION;
    #endif
    VersionInfo = VersionInfo + wxT(")");
  }
*/
  // centering the version number, using white spaces
  for (unsigned int i=0;i<((Width-VersionInfo.length())/2);i++) Whites = Whites + " ";

  VersionInfo = Whites + VersionInfo;


  std::cout << std::endl;
  std::cout << "===========================================================" << std::endl;
/*  std::cout << "                     OpenFLUID-engine                      " << std::endl;
  std::cout << std::endl;*/
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

void OpenFLUIDApp::printDataInfos()
{
  openfluid::core::UnitsListByClassMap_t::const_iterator UnitsIt;

  std::cout << std::endl;
  std::cout << "Simulation ID: " << mp_RunEnv->getSimulationID() << std::endl;
  std::cout << std::endl;


  std::cout << "Spatial domain: " << std::endl;
  for (UnitsIt = mp_CoreData->getUnits()->begin(); UnitsIt != mp_CoreData->getUnits()->end();++UnitsIt )
  {
    std::cout << "  - " << (*UnitsIt).first << ", " << (*UnitsIt).second.getList()->size() << " units" << std::endl;
  }

  std::cout << std::endl;

  std::cout << "Simulation from " << mp_Engine->getSimulationInfo()->getStartTime().getAsISOString()
            << " to " << mp_Engine->getSimulationInfo()->getEndTime().getAsISOString() << std::endl
            << "         -> " <<  (mp_Engine->getSimulationInfo()->getStepsCount()) << " time steps of " << mp_Engine->getSimulationInfo()->getTimeStep() << " seconds" << std::endl;

  std::cout << std::endl;

  if (mp_RunEnv->isProgressiveOutput()) std::cout << "Progressive output enabled (Packet=" << mp_RunEnv->getProgressiveOutputPacket() << ", Keep=" << mp_RunEnv->getProgressiveOutputKeep() << ")" <<  std::endl;
  else std::cout << "Progressive output disabled" << std::endl;

  std::cout << std::endl;

  std::cout.flush();

}


// =====================================================================
// =====================================================================

void OpenFLUIDApp::printPluginsList()
{

  ArrayOfPluginsContainers PlugContainers = mp_PlugMan->getAvailableFunctions();

  std::cout << "Available simulation functions:" << std::endl;

  bool OneAtLeast = false;

  for (unsigned int i=0;i<PlugContainers.size();i++)
  {
    if (PlugContainers[i]->SDKCompatible && PlugContainers[i]->Signature!=NULL)
    {
      wxFileName TmpFilename = wxFileName(_U(PlugContainers[i]->Filename.c_str()));
      std::string TmpFilenameStr = _S(TmpFilename.GetName());
      std::cout << "  - " << ReplaceEmptyString(TmpFilenameStr,"(unknown simulation function)") << std::endl;
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

  ArrayOfPluginsContainers PlugContainers = mp_PlugMan->getAvailableFunctions(Pattern);
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

  mp_Engine->saveMessages();

  if (mp_RunEnv->isWriteSimReport())
  {
    saveSimulationReports(Msg);
  }

  std::cout << std::endl << Msg << std::endl;

//  std::cout << "ERROR: " << FormatExecutionMessage(wxString(Msg.c_str(), wxConvUTF8)) << std::endl;


  /*
  for (int i=0; i<mp_ExecMsgs->getWarningMsgs().Count();i++)
  {
    std::cout << "WARNING: " << FormatExecutionMessage(mp_ExecMsgs->getWarningMsgs().Item(i)) << std::endl;
  }
  */

  std::cout << std::endl;
  std::cout.flush();

  return 127;
}

// =====================================================================
// =====================================================================



bool OpenFLUIDApp::OnInit()
{
  wxString TmpStr;

  // =========== initialization ===========

  m_OKToRun = true;

  SetAppName(_U(OPENFLUID_APPNAME.c_str()));

  mp_RunEnv = new RuntimeEnvironment();

  mp_ExecMsgs = new openfluid::base::ExecutionMessages();

  mp_PlugMan = new PluginManager(mp_ExecMsgs,mp_RunEnv);



  // =========== command line parsing ===========

  wxCmdLineParser Parser(CmdLineDesc,argc,argv);


  if (Parser.Parse() != 0)
  {
     std::cout << std::endl;
     return false;
  }


  if (Parser.Found(wxT("p"),&TmpStr)) mp_RunEnv->addExtraPluginsPaths(_S(TmpStr));


  if (Parser.Found(wxT("f")) || Parser.Found(wxT("k")) || Parser.Found(wxT("r")) || Parser.Found(wxT("u")) ||Parser.Found(wxT("version")) || Parser.Found(wxT("x")))
  {
    if (Parser.Found(wxT("f")))
    {
      printOpenFLUIDInfos();
      printPluginsList();
    }

    if (Parser.Found(wxT("r"))) printPluginsReport(false,"");
    if (Parser.Found(wxT("x"))) printPluginsReport(true,"");
    if (Parser.Found(wxT("u"),&TmpStr)) printPluginsReport(false,_S(TmpStr));

    if (Parser.Found(wxT("version"))) std::cout << FULL_VERSION << std::endl;
    if (Parser.Found(wxT("k"))) std::cout << FULL_VERSION << std::endl;

    m_OKToRun = false;
  }
  else
  {
    // update of run environment information
	if (Parser.Found(wxT("i"),&TmpStr)) mp_RunEnv->setInputDir(RemoveTrailingSlashes(_S(TmpStr)));
    if (Parser.Found(wxT("o"),&TmpStr)) mp_RunEnv->setOutputDir(RemoveTrailingSlashes(_S(TmpStr)));
    if (Parser.Found(wxT("a"))) mp_RunEnv->setDateTimeOutputDir();
    if (Parser.Found(wxT("c"))) mp_RunEnv->setClearOutputDir(true);
    if (Parser.Found(wxT("q"))) mp_RunEnv->setQuietRun(true);
    if (Parser.Found(wxT("s"))) mp_RunEnv->setWriteSimReport(false);
    if (Parser.Found(wxT("v"))) mp_RunEnv->setVerboseRun(true);
    if (Parser.Found(wxT("z"))) mp_RunEnv->setWriteResults(false);
    if (Parser.Found(wxT("no-varname-check"))) mp_RunEnv->setCheckVarNames(false);

	printOpenFLUIDInfos();
	printEnvInfos();

  }

  return true;
}

// =====================================================================
// =====================================================================

void OpenFLUIDApp::printEnvInfos()
{
  std::vector<std::string> FunctionsPaths = mp_RunEnv->getPluginsPaths();
  unsigned int i;

  std::cout << "Input dir: " << mp_RunEnv->getInputDir() << std::endl;
  if (mp_RunEnv->isWriteResults() || mp_RunEnv->isWriteSimReport()) std::cout << "Output dir: " << mp_RunEnv->getOutputDir() << std::endl;
  std::cout << "Functions search path(s):" << std::endl;
  for (i=0;i<FunctionsPaths.size();i++) std::cout << " #" << (i+1) << " " << FunctionsPaths[i] << std::endl;
  if ((mp_RunEnv->isWriteResults() || mp_RunEnv->isWriteSimReport()) && (mp_RunEnv->isClearOutputDir())) std::cout << "Output dir cleared before data saving" << std::endl;
  if (mp_RunEnv->isQuietRun()) std::cout << "Quiet mode enabled" << std::endl;
  if (mp_RunEnv->isVerboseRun()) std::cout << "Verbose mode enabled" << std::endl;
  if (!mp_RunEnv->isCheckVarNames()) std::cout << "Variable names checking disabled" << std::endl;
  std::cout << std::endl;
}


// =====================================================================
// =====================================================================

int OpenFLUIDApp::OnRun()
{
  int ReturnValue = 0;

  if (m_OKToRun)
  {

    try
    {
      m_FullStartTime = boost::posix_time::microsec_clock::local_time();

      mp_CoreData = openfluid::core::CoreRepository::getInstance();
      mp_CoreData->setMemoryMonitor(openfluid::core::MemoryMonitor::getInstance());

      mp_Engine = new Engine(mp_CoreData,mp_ExecMsgs,mp_RunEnv,mp_PlugMan);

      // model load and check
      buildModel();
//      if (mp_ExecMsgs->isErrorFlag()) throw openfluid::base::OFException(mp_ExecMsgs->getErrorMsg());
      mp_ExecMsgs->resetWarningFlag();

      // input data load and check
      loadData();
//      if (mp_ExecMsgs->isErrorFlag())  throw openfluid::base::OFException(mp_ExecMsgs->getErrorMsg());
      mp_ExecMsgs->resetWarningFlag();

      // global consistency check
      checkConsistency();
//      if (mp_ExecMsgs->isErrorFlag()) throw openfluid::base::OFException(mp_ExecMsgs->getErrorMsg());
      mp_ExecMsgs->resetWarningFlag();


      // simulation
      runSimulation();
//      if (mp_ExecMsgs->isErrorFlag()) throw openfluid::base::OFException(mp_ExecMsgs->getErrorMsg());
      mp_ExecMsgs->resetWarningFlag();


      mp_RunEnv->setEffectiveSimulationDuration(m_EffectiveEndTime-m_EffectiveStartTime);


      if (mp_RunEnv->isWriteSimReport())
      {
        saveSimulationReports("");
        mp_ExecMsgs->resetWarningFlag();
      }


      m_FullEndTime = boost::posix_time::microsec_clock::local_time();

      if (mp_RunEnv->isWriteResults() || mp_RunEnv->isWriteSimReport()) std::cout << std::endl;

      boost::posix_time::time_duration FullSimDuration = m_FullEndTime - m_FullStartTime;

      printlnExecMessagesStats();

      std::cout << std::endl;

      std::cout << "Simulation run time: " << boost::posix_time::to_simple_string(mp_RunEnv->getEffectiveSimulationDuration()) << std::endl;
      std::cout << "     Total run time: " << boost::posix_time::to_simple_string(FullSimDuration) << std::endl;
      std::cout << std::endl;


    }

    catch (openfluid::base::OFException& E)
    {
      ReturnValue = stopAppReturn("ERROR: " + std::string(E.what()));
    }
    catch (std::bad_alloc& E)
    {
      ReturnValue = stopAppReturn("MEMORY ALLOCATION ERROR: " + std::string(E.what()) + ". Possibly not enough memory available");
    }
    catch (std::exception& E)
    {
      ReturnValue = stopAppReturn("SYSTEM ERROR: " + std::string(E.what()));
    }
    catch (...)
    {
      ReturnValue = stopAppReturn("UNKNOWN ERROR");
    }

  }
  return ReturnValue;

}




// =====================================================================
// =====================================================================





