/**
  \file main.cpp
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <iostream>

#include <wx/log.h>


#include "AppTools.h"
#include "main.h"
#include "setup.h"

using namespace mhydasdk::base;
using namespace mhydasdk::core;


wxString LastError::Message;

// =====================================================================
// =====================================================================


IMPLEMENT_APP_CONSOLE(MHYDASApp);

// =====================================================================
// =====================================================================



bool MHYDASApp::buildModel()
{
  bool ExecStatus;

  std::cout << "* Building model... ";

  ExecStatus = mp_Engine->buildModel();

  printlnExecStatus(ExecStatus);

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool MHYDASApp::loadData()
{
  bool ExecStatus;

  std::cout << "* Loading input data... ";
  std::cout.flush();

  ExecStatus = mp_Engine->loadData();

  printlnExecStatus(ExecStatus);

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool MHYDASApp::checkConsistency()
{

  bool ExecStatus;

  std::cout << "* Checking consistency... ";

  ExecStatus = mp_Engine->checkConsistency();

  printlnExecStatus(ExecStatus);

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool MHYDASApp::runSimulation()
{

  bool ExecStatus;

  printDataInfos();

  std::cout << "**** Starting simulation ****" << endl;

  m_EffectiveStartTime = wxDateTime::Now();

  ExecStatus = mp_Engine->run();

  m_EffectiveEndTime = wxDateTime::Now();

  if (ExecStatus) std::cout << "**** Simulation done ****" << endl << endl;
  else  std::cout << "**** Simulation aborted ****" << endl << endl;

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool MHYDASApp::saveResults()
{

  bool ExecStatus;

  std::cout << "* Saving results... ";

  ExecStatus = mp_Engine->saveResults();

  printlnExecStatus(ExecStatus);

  return ExecStatus;
}

// =====================================================================
// =====================================================================


void MHYDASApp::printlnExecStatus(bool Status)
{
  if (Status) std::cout << "[OK]" << std::endl;
  else std::cout << "[Error]" << std::endl;
}


// =====================================================================
// =====================================================================


void MHYDASApp::printMHYDASInfos()
{
  std::cout << std::endl;
  std::cout << "==========================================================" << std::endl;
  std::cout << "                       M H Y D A S                        " << std::endl;
  std::cout << "      Distributed hydrological model for agrosystems      " << std::endl;
  std::cout << std::endl;
  #ifdef SVN_REV
  std::cout << "                      revision " << SVN_REVISION.mb_str(wxConvUTF8) << std::endl;
  std::cout << std::endl;
  #endif
  std::cout << "               LISAH-INRA, Montpellier, France            " << std::endl;
  std::cout << "==========================================================" << std::endl;
  std::cout << std::endl;
}



// =====================================================================
// =====================================================================

void MHYDASApp::printDataInfos()
{
  std::cout << std::endl;
  std::cout << "Spatial objects: " << std::endl
            << "   - " << mp_CoreData->getSpatialData()->getSUsCollection().size() << " Surface Units" << std::endl
            << "   - " << mp_CoreData->getSpatialData()->getRSsCollection().size() << " Reach Segments" << std::endl
            << "   - " << mp_CoreData->getSpatialData()->getGUsCollection().size() << " Groundwater Units" << std::endl;
  std::cout << "Rain source(s): " << mp_CoreData->getRainEvent()->getRainSourceCollection().size() << std::endl;
  std::cout << "Simulation period: " << std::endl
            << "   from " << mp_CoreData->getRainEvent()->getEventStartingTime().asString().mb_str(wxConvUTF8) << std::endl
            << "   to " << mp_CoreData->getRainEvent()->getEventEndingTime().asString().mb_str(wxConvUTF8) << std::endl
            << "   time step: " << mp_Engine->getConfig().DeltaT << "s" << std::endl;
  std::cout << std::endl;

}


// =====================================================================
// =====================================================================

void MHYDASApp::printPluginsList()
{
  ArrayOfPluginsSignatures Signatures = mp_PlugMan->getAvailableFunctionsList();

  std::cout << "Available pluggable functions:" << std::endl;


  if (Signatures.GetCount() > 0)
  {
    for (int i=0;i<Signatures.GetCount();i++) std::cout << "  - " << Signatures[i]->Name.mb_str(wxConvUTF8) << std::endl;
  }
  else
  {
    std::cout << "  (none)" << std::endl;
  }

}

// =====================================================================
// =====================================================================

bool MHYDASApp::stopAppReturn()
{
  std::cout << std::endl << "Oooops! " << LastError::Message.mb_str(wxConvUTF8) << std::endl;
  std::cout << "Aborting MHYDAS application." << std::endl;

  return false;
}

// =====================================================================
// =====================================================================



bool MHYDASApp::OnInit()
{
  wxString TmpStr;

  m_OKToRun = true;

  mp_RunEnv = new RuntimeEnvironment(wxPathOnly(GetExecutablePath()));

  mp_PlugMan = new PluginManager(mp_RunEnv);

  LastError::Message = wxT("");

  wxLog::SetTimestamp(NULL);

  printMHYDASInfos();

  // parsing command line

  wxCmdLineParser Parser(CmdLineDesc,argc,argv);

  if (Parser.Parse() != 0)
  {
     std::cout << std::endl;
     return false;
  }

  if (Parser.Found(wxT("v"))) wxLog::SetVerbose(true);

  if (Parser.Found(wxT("p")))
  {
    printPluginsList();

    m_OKToRun = false;
  }
  else
  {
    if (Parser.Found(wxT("i"),&TmpStr)) mp_RunEnv->setInputDir(TmpStr);
    if (Parser.Found(wxT("o"),&TmpStr)) mp_RunEnv->setOutputDir(TmpStr);
    if (Parser.Found(wxT("g"))) mp_RunEnv->setDateTimeOutputDir();

    wxLogVerbose(wxT("Input dir: ")+mp_RunEnv->getInputDir());
    wxLogVerbose(wxT("Output dir: ")+mp_RunEnv->getOutputDir());
  }

  return true;
}


// =====================================================================
// =====================================================================

int MHYDASApp::OnRun()
{

  if (m_OKToRun)
  {
    mp_CoreData = new CoreRepository();
    mp_Engine = new Engine(mp_CoreData,mp_RunEnv);


    mp_CoreData->getRainEvent()->enableFirstSerieConstraint(true);

    // chargement, vérification et montage du modèle
    if (!buildModel()) return stopAppReturn();

    // chargement et vérification du jeu de données d'entrée
    if (!loadData()) return stopAppReturn();

    // vérification de la cohérence de l'ensemble
    if (!checkConsistency()) return stopAppReturn();

    // simulation
    if (!runSimulation()) return stopAppReturn();

    // sauvegarde des résultats
    if (!saveResults()) return stopAppReturn();
  }

  std::cout << std::endl;

  wxTimeSpan EffSimTime = m_EffectiveEndTime.Subtract(m_EffectiveEndTime);

  std::cout << "Effective run time: " << EffSimTime.Format(wxT("%Hh %Mm %Ss %lms")).mb_str(wxConvUTF8) << std::endl;

  return true;

}



// =====================================================================
// =====================================================================

int MHYDASApp::OnExit()
{
  std::cout << std::endl;
}


// =====================================================================
// =====================================================================

