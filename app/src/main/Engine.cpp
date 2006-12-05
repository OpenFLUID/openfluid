/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "Engine.h"

#include <iostream>
#include <iomanip>



Engine::Engine(mhydasdk::core::CoreRepository* CoreData, mhydasdk::base::RuntimeEnvironment* RunEnv,
               PluginManager* PlugMan)
{

  mp_CoreData = CoreData;
  mp_RunEnv = RunEnv;
  mp_PlugMan = PlugMan;


  mp_Module = NULL;
  m_Functions.Clear();

  mp_IOMan = new IOManager(mp_RunEnv);

}

// =====================================================================
// =====================================================================

Engine::~Engine()
{

}


bool Engine::processConfig()
{
  /** \internal

    uses the PluginManager to
      - load and parametrize functions
      - build the processing list of each module
  */


  FunctionConfigsList::Node *FuncNode = m_Config.ModuleConfig.GetFirst();
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


bool Engine::plugFunctions()
{

  mp_Module = new Module(mp_CoreData,m_Functions);

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

  return (mp_IOMan->loadModelConfig(&m_Config) && processConfig() && plugFunctions());
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

  // prepares data for module
  if (mp_Module == NULL || !mp_Module->prepareData())
  {
    mhydasdk::base::LastError::Message = wxT("Module data preparation error.");
    return false;
  }


  // chacks conssitency for module
  if (mp_Module == NULL || !mp_Module->checkConsistency())
  {
    mhydasdk::base::LastError::Message = wxT("Module consistency error.");
    return false;
  }



  return true;
}


// =====================================================================
// =====================================================================


bool Engine::run()
{


  // inits

  mp_SimStatus = new mhydasdk::base::SimulationStatus(mp_CoreData->getRainEvent()->getEventStartingTime(),
                                                      mp_CoreData->getRainEvent()->getEventEndingTime(),
                                                      m_Config.DeltaT);

  // initialization of functions
  if (!mp_Module->initializeRun((mhydasdk::base::SimulationStatus*)mp_SimStatus))
  {
    mhydasdk::base::LastError::Message = wxT("Module initialization error.");
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

    if (mp_Module->runStep(mp_SimStatus))
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
  mp_Module->finalizeRun((mhydasdk::base::SimulationStatus*)mp_SimStatus);

  return true;
}


// =====================================================================
// =====================================================================


bool Engine::saveResults()
{
  return mp_IOMan->saveResults(mp_CoreData);
}

// =====================================================================
// =====================================================================


