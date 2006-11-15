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


  mp_HydroModule = NULL;
  m_HydroFunctions.Clear();

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


  FunctionConfigsList::Node *FuncNode = m_Config.HydroModuleConfig.GetFirst();
  FunctionConfig *FConf;

  m_HydroFunctions.clear();


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

    FuncToAdd = mp_PlugMan->getFunctionFromPlugin(FConf->File,mhydasdk::base::MOD_HYDROLOGY,mp_CoreData);

    if (FuncToAdd != NULL)
    {
       if (FuncToAdd->initParams(FConf->Params))
       {
         m_HydroFunctions.Append((mhydasdk::base::Function**)FuncToAdd);
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

  mp_HydroModule = new Module(mp_CoreData,m_HydroFunctions);

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

  // prepares data for each module
  if (mp_HydroModule == NULL || !mp_HydroModule->prepareData())
  {
    mhydasdk::base::LastError::Message = wxT("Hydrology module data preparation error.");
    return false;
  }


  // chacks intensity for each module
  if (mp_HydroModule == NULL || !mp_HydroModule->checkConsistency())
  {
    mhydasdk::base::LastError::Message = wxT("Hydrology module consistency error.");
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
  if (!mp_HydroModule->initializeRun())
  {
    mhydasdk::base::LastError::Message = wxT("Hydro module initialization error.");
    return false;
  }

  // run
  #define display 0
  #if display
  std::cout << std::endl;
  std::cout << std::setw(10) << "Time step";
  std::cout << std::setw(18) << "Real time";
  std::cout << std::setw(17) << "Status";
  std::cout << std::endl;
  std::cout << std::endl;
  cout.flush();
  #endif

  do
  {
    #if display
    std::cout << std::setw(8) << mp_SimStatus->getCurrentStep();
    std::cout << std::setw(25) << _C(mp_SimStatus->getCurrentTime().asString());
    #endif

    if (mp_HydroModule->runStep(mp_SimStatus))
    {
      #if display
      std::cout << std::setw(11) << "[OK]";

      std::cout << std::setw(11) << mp_CoreData->getSpatialData()->getSUByID(1)->getRainSource()->getTimeSerie()->getItemsCollection()->at(mp_SimStatus->getCurrentStep())->getValue();
      std::cout << std::setw(11) << mp_CoreData->getSpatialData()->getSUByID(1)->getSimulatedVars()->find(wxT("infiltration"))->second->at(mp_SimStatus->getCurrentStep());
      std::cout << std::setw(11) << mp_CoreData->getSpatialData()->getSUByID(1)->getSimulatedVars()->find(wxT("runoff"))->second->at(mp_SimStatus->getCurrentStep());
      std::cout << std::setw(11) << mp_CoreData->getSpatialData()->getSUByID(1)->getSimulatedVars()->find(wxT("qoutput"))->second->at(mp_SimStatus->getCurrentStep());
      std::cout << std::endl;
      #endif
    }
    else
    {
      std::cout << std::setw(9) << "[Error]";
    }

    #if display
    cout.flush();
    #endif

  } while (mp_SimStatus->switchToNextStep());
  //std::cout << std::setw(9) << "[OK]";

  std::cout << std::endl;

  // finalization of functions
  mp_HydroModule->finalizeRun();

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


