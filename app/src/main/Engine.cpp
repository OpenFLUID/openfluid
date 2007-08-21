/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "Engine.h"
#include "AppTools.h"

#include <iostream>
#include <iomanip>


#include <wx/listimpl.cpp>
WX_DEFINE_LIST(FunctionsList);


// =====================================================================
// =====================================================================

#define DECLARE_FUNCTION_PARSER \
    FunctionsList::Node *_M_FuncNode = NULL;


#define PARSE_FUNCTION_LIST(calledmethod,statevar) \
    _M_FuncNode = m_Functions.GetFirst(); \
    while (_M_FuncNode && statevar) \
    { \
      mhydasdk::base::PluggableFunction* CurrentFunction = (mhydasdk::base::PluggableFunction*)_M_FuncNode->GetData(); \
      if (CurrentFunction != NULL) statevar = statevar && CurrentFunction->calledmethod; \
      _M_FuncNode = _M_FuncNode->GetNext(); \
    }

#define PARSE_FUNCTION_LIST_TWO(calledmethod1,calledmethod2,statevar) \
    _M_FuncNode = m_Functions.GetFirst(); \
    while (_M_FuncNode && statevar) \
    { \
      mhydasdk::base::PluggableFunction* CurrentFunction = (mhydasdk::base::PluggableFunction*)_M_FuncNode->GetData(); \
      if (CurrentFunction != NULL) statevar = (statevar && (CurrentFunction->calledmethod1 && CurrentFunction->calledmethod2)); \
      _M_FuncNode = _M_FuncNode->GetNext(); \
    }

#define PARSE_FUNCTION_LIST_FOUR(calledmethod1,calledmethod2,calledmethod3,calledmethod4,statevar) \
    _M_FuncNode = m_Functions.GetFirst(); \
    while (_M_FuncNode && statevar) \
    { \
      mhydasdk::base::PluggableFunction* CurrentFunction = (mhydasdk::base::PluggableFunction*)_M_FuncNode->GetData(); \
      if (CurrentFunction != NULL) statevar = (statevar && ((CurrentFunction->calledmethod1 && CurrentFunction->calledmethod2) && (CurrentFunction->calledmethod3 && CurrentFunction->calledmethod4))); \
      _M_FuncNode = _M_FuncNode->GetNext(); \
    }




// =====================================================================
// =====================================================================



Engine::Engine(mhydasdk::core::CoreRepository* CoreData, mhydasdk::base::ExecutionMessages* ExecMsgs,
               RuntimeEnvironment* RunEnv, PluginManager* PlugMan)
{

  mp_CoreData = CoreData;
  mp_ExecMsgs = ExecMsgs;
  mp_RunEnv = RunEnv;
  mp_PlugMan = PlugMan;


  m_Functions.Clear();

  mp_IOMan = new IOManager(mp_ExecMsgs,mp_RunEnv);

  m_Config.SimulationID = wxT("");
  
  mp_SimStatus = NULL;

}

// =====================================================================
// =====================================================================

Engine::~Engine()
{

}

// =====================================================================
// =====================================================================


bool Engine::processConfig()
{
  /** \internal

    uses the PluginManager to
      - load and parametrize functions
  */


  FunctionConfigsList::Node *FuncNode = m_Config.FuncConfigs.GetFirst();
  FunctionConfig *FConf;
  mhydasdk::base::PluggableFunction* FuncToAdd;
  
  m_Functions.clear();



  while (FuncNode)
  {
    FConf = (FunctionConfig*)(FuncNode->GetData());


    FuncToAdd = mp_PlugMan->getFunctionFromPlugin(FConf->File,mhydasdk::base::SIMULATION,mp_CoreData);

    if (FuncToAdd != NULL)
    {
       if (FuncToAdd->initParams(FConf->Params))
       {
         m_Functions.Append((mhydasdk::base::PluggableFunction**)FuncToAdd);
       }
       else
       {
         mp_ExecMsgs->setError(wxT("Engine"),wxT("Initializing params function error."));
         return false;
       }

    }
    else
    {
      mp_ExecMsgs->setError(wxT("Engine"),wxT("Loading function from plugin ") + FConf->File + wxT(" error."));
      return false;
    }

    FuncNode = FuncNode->GetNext();
  }


  return true;
}



// =====================================================================
// =====================================================================

bool Engine::buildModel()
{
  /** \internal

    builds the model from the config file, calling:
    - loadModelConfig() from mp_IOMan
    - processConfig()
    - plugFunctions

  */

  return (mp_IOMan->loadModelConfig(&m_Config) && processConfig());
}


// =====================================================================
// =====================================================================

bool Engine::loadData()
{
  return (mp_IOMan->loadHydroObjects(mp_CoreData->getSpatialData()) &&
          mp_IOMan->loadHydroObjectsProperties(mp_CoreData->getSpatialData()) &&
          mp_IOMan->loadHydroObjectsInitialConditions(mp_CoreData->getSpatialData()) &&
          mp_IOMan->loadRainEvent(mp_CoreData->getRainEvent()) &&
          mp_IOMan->loadRainDistribution(mp_CoreData) &&
          mp_IOMan->loadOutputConfig());
}


// =====================================================================
// =====================================================================

bool Engine::prepareDataAndCheckConsistency()
{

  bool IsOK = true;
  DECLARE_FUNCTION_PARSER;

  // builds topology by linking objects
  if (!mp_CoreData->getSpatialData()->buildObjectLinkedTopologyFromIDs())
  {
    mp_ExecMsgs->setError(wxT("Engine"),wxT("Topology rebuild error"));
    return false;
  }

  // builds process orders lists
  if (!mp_CoreData->getSpatialData()->buildProcessOrders())
  {
    mp_ExecMsgs->setError(wxT("Engine"),wxT("Process orders build error"));
    return false;
  }

  // process RainEVent
  if (!mp_CoreData->getRainEvent()->ProcessRainSources(m_Config.DeltaT))
  {
    mp_ExecMsgs->setError(wxT("Engine"),wxT("Rain sources process error"));
    return false;
  }
  



  // check simulation functions count
  
  if (m_Functions.GetCount() == 0) 
  {
    mp_ExecMsgs->setError(wxT("Engine"),wxT("No simulation function"));
    return false;
  }



  // checks consistency
    
//  PARSE_FUNCTION_LIST_TWO(prepareData(),checkConsistency(),IsOK);
  PARSE_FUNCTION_LIST_FOUR(prepareData(),prepareFunctionData(),
                           checkConsistency(),checkFunctionConsistency(),IsOK);
  if (!IsOK)
  {       
    return false;    
  }




  // inits the simulation infos and status

  mp_SimStatus = new mhydasdk::base::SimulationStatus(mp_CoreData->getRainEvent()->getEventStartingTime(),
                                                      mp_CoreData->getRainEvent()->getEventEndingTime(),
                                                      m_Config.DeltaT);


  // preparation des donnees de simulation
  mp_CoreData->getSpatialData()->reserveSimulationVars(mp_SimStatus->getStepsCount());


  return true;
}


// =====================================================================
// =====================================================================


bool Engine::run()
{
  bool IsOK = true;
  DECLARE_FUNCTION_PARSER;
                                                      
                                                      
  // initialization of functions
/*  if (!mp_Module->initializeRun((mhydasdk::base::SimulationStatus*)mp_SimStatus))
  {
    return false;
  }
*/

  PARSE_FUNCTION_LIST(initializeRun((mhydasdk::base::SimulationStatus*)mp_SimStatus),IsOK);
  if (mp_ExecMsgs->isErrorFlag())
  {
    return false;
  }

  // run

  if (!mp_RunEnv->isQuietRun())
  {
    std::cout << std::endl;
    std::cout << std::setw(10) << "Time step";
    std::cout << std::setw(18) << "Real time";
    std::cout << std::setw(17) << "Status";
    std::cout << std::endl;
    std::cout << std::endl;
    cout.flush();
  }

  do
  {
    if (!mp_RunEnv->isQuietRun())
    {
      std::cout << std::setw(8) << mp_SimStatus->getCurrentStep();
      std::cout << std::setw(25) << _C(mp_SimStatus->getCurrentTime().asString());
      std::cout.flush();
    }  

    mp_ExecMsgs->resetWarningFlag();
   
    PARSE_FUNCTION_LIST(runStep(mp_SimStatus),IsOK);
        

    if (mp_ExecMsgs->isErrorFlag())
    {
      std::cout << std::setw(12) << "[Error]";
      std::cout << std::endl << std::endl;
      return false;
      
    }
    else
    {
      if (!mp_RunEnv->isQuietRun())
      {

        if (mp_ExecMsgs->isWarningFlag()) std::cout << std::setw(12) << "[Warning]";
        else std::cout << std::setw(12) << "[OK]";
      }  
    }

    if (!mp_RunEnv->isQuietRun())
    {
      std::cout << std::endl;
      cout.flush();
    }  

  } while (mp_SimStatus->switchToNextStep());


  std::cout << std::endl;

  // finalization of functions
  PARSE_FUNCTION_LIST(finalizeRun((mhydasdk::base::SimulationStatus*)mp_SimStatus),IsOK)  

  return IsOK;
}


// =====================================================================
// =====================================================================


bool Engine::saveResults(ExtraSimInfos ExSI)
{
  mp_ExecMsgs->resetWarningFlag();
  return (mp_IOMan->saveResults(mp_CoreData,ExSI));
}

// =====================================================================
// =====================================================================

bool Engine::saveReports(ExtraSimInfos ExSI)
{
  mp_ExecMsgs->resetWarningFlag();
  return (mp_IOMan->saveSimulationInfos(mp_CoreData,ExSI,(mhydasdk::base::SimulationInfo*)mp_SimStatus));
}



// =====================================================================
// =====================================================================


