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
      mhydasdk::base::Function* CurrentFunction = (mhydasdk::base::Function*)_M_FuncNode->GetData(); \
      if (CurrentFunction != NULL) statevar = statevar && CurrentFunction->calledmethod; \
      _M_FuncNode = _M_FuncNode->GetNext(); \
    }

#define PARSE_FUNCTION_LIST_TWO(calledmethod1,calledmethod2,statevar) \
    _M_FuncNode = m_Functions.GetFirst(); \
    while (_M_FuncNode && statevar) \
    { \
      mhydasdk::base::Function* CurrentFunction = (mhydasdk::base::Function*)_M_FuncNode->GetData(); \
      if (CurrentFunction != NULL) statevar = statevar && CurrentFunction->calledmethod1 && CurrentFunction->calledmethod2; \
      _M_FuncNode = _M_FuncNode->GetNext(); \
    }



// =====================================================================
// =====================================================================



Engine::Engine(mhydasdk::core::CoreRepository* CoreData, RuntimeEnvironment* RunEnv,
               PluginManager* PlugMan)
{

  mp_CoreData = CoreData;
  mp_RunEnv = RunEnv;
  mp_PlugMan = PlugMan;


  m_Functions.Clear();

  mp_IOMan = new IOManager(mp_RunEnv);

  m_Config.SimulationID = wxT("");

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

  m_Functions.clear();


  // start display test
/*
  while (FuncNode)
  {
    FConf = (FunctionConfig*)(FuncNode->GetData());
    std::cerr << "Function " << FConf->Name.mb_str(wxConvUTF8) << ", " << FConf->File.mb_str(wxConvUTF8) << std::endl;


    mhydasdk::core::ParamsMap::iterator it;
    for( it = FConf->Params.begin(); it != FConf->Params.end(); ++it )
    {
      std::cerr << "  - " << it->first.mb_str(wxConvUTF8) << " = " << it->second << std::endl;
    }

    FuncNode = FuncNode->GetNext();
  }
*/
  // end display test

  mhydasdk::base::Function* FuncToAdd;

  while (FuncNode)
  {
    FConf = (FunctionConfig*)(FuncNode->GetData());


    FuncToAdd = mp_PlugMan->getFunctionFromPlugin(FConf->File,mhydasdk::base::SIMULATION,mp_CoreData);

    if (FuncToAdd != NULL)
    {
       if (FuncToAdd->initParams(FConf->Params))
       {
         m_Functions.Append((mhydasdk::base::Function**)FuncToAdd);
       }
       else
       {
         mhydasdk::base::LastError::Message = wxT("Error initializing params function.");
         return false;
       }

    }
    else
    {
      mhydasdk::base::LastError::Message = wxT("Error loading function from plugin.");
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
    mhydasdk::base::LastError::Message = wxT("Topology rebuild error.");
    return false;
  }

  // builds process orders lists
  if (!mp_CoreData->getSpatialData()->buildProcessOrders())
  {
    mhydasdk::base::LastError::Message = wxT("Process orders build error.");
    return false;
  }

  // process RainEVent
  if (!mp_CoreData->getRainEvent()->ProcessRainSources(m_Config.DeltaT))
  {
    mhydasdk::base::LastError::Message = wxT("Rain sources process error.");
    return false;
  }
  

  /*
  // integrates rain on time step (converts from intensity to water height)
  mhydasdk::core::RainSourceMap RainSrcColl = mp_CoreData->getRainEvent()->getRainSourceCollection();
  mhydasdk::core::RainSourceMap::iterator RainSrcit;
  mhydasdk::core::ChronDataSource *Source;
  for(RainSrcit = RainSrcColl.begin(); RainSrcit != RainSrcColl.end(); ++RainSrcit )
  {
    Source = RainSrcit->second;
    Source->getTimeSerie()->multiplyValuesByFactor(m_Config.DeltaT);
  }
  */


  // check simulation functions count
  
  if (m_Functions.GetCount() == 0) 
  {
    mhydasdk::base::LastError::Message = wxT("No simulation function.");
    return false;
  }


/*
  // prepare data
 
  PARSE_FUNCTION_LIST(prepareData(),IsOK);
  if (!IsOK)
  {
    mhydasdk::base::LastError::Message = wxT("Data preparation error.");
    return false;    
  }
  
  


  // checks consistency
  
  PARSE_FUNCTION_LIST(checkConsistency(),IsOK);
  if (!IsOK)
  {
    mhydasdk::base::LastError::Message = wxT("Consistency error.");
    return false;    
  }
*/  

  // checks consistency
  
  PARSE_FUNCTION_LIST_TWO(prepareData(),checkConsistency(),IsOK);
  if (!IsOK)
  {
    mhydasdk::base::LastError::Message = wxT("Consistency error.");
    return false;    
  }




  // inits the simulation infos and status

  mp_SimStatus = new mhydasdk::base::SimulationStatus(mp_CoreData->getRainEvent()->getEventStartingTime(),
                                                      mp_CoreData->getRainEvent()->getEventEndingTime(),
                                                      m_Config.DeltaT);


  // pr�paration des donn�es de simulation
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
    mhydasdk::base::LastError::Message = wxT("Module initialization error.");
    return false;
  }
*/

  PARSE_FUNCTION_LIST(initializeRun((mhydasdk::base::SimulationStatus*)mp_SimStatus),IsOK);
  if (!IsOK)
  {
    mhydasdk::base::LastError::Message = wxT("Function initialization error.");
    return false;
  }

  // run

  std::cout << std::endl;
  std::cout << std::setw(10) << "Time step";
  std::cout << std::setw(18) << "Real time";
  std::cout << std::setw(17) << "Status";
  std::cout << std::endl;
  std::cout << std::endl;
  cout.flush();


  do
  {

    std::cout << std::setw(8) << mp_SimStatus->getCurrentStep();
    std::cout << std::setw(25) << _C(mp_SimStatus->getCurrentTime().asString());
    std::cout.flush();

    PARSE_FUNCTION_LIST(runStep(mp_SimStatus),IsOK);
    if (IsOK)
    {
      
      std::cout << std::setw(11) << "[OK]";
      
    }
    else
    {
      std::cout << std::setw(9) << "[Error]";
    }

    std::cout << std::endl;
    cout.flush();

  } while (mp_SimStatus->switchToNextStep());


  std::cout << std::endl;

  // finalization of functions
  //mp_Module->finalizeRun((mhydasdk::base::SimulationStatus*)mp_SimStatus);
  PARSE_FUNCTION_LIST(finalizeRun((mhydasdk::base::SimulationStatus*)mp_SimStatus),IsOK)  

  return IsOK;
}


// =====================================================================
// =====================================================================


bool Engine::saveResults(ExtraSimInfos ExSI)
{
 
  return (mp_IOMan->saveResults(mp_CoreData,ExSI) && mp_IOMan->saveSimulationInfos(mp_CoreData,ExSI,(mhydasdk::base::SimulationInfo*)mp_SimStatus));
}

// =====================================================================
// =====================================================================


