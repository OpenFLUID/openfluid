/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "Engine.h"


Engine::Engine(mhydasdk::core::CoreRepository* CoreData, mhydasdk::base::RuntimeEnvironment* RunEnv)
{

  mp_CoreData = CoreData;
  mp_RunEnv = RunEnv;

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
          mp_IOMan->loadRainEvent(mp_CoreData->getRainEvent()) &&
          mp_IOMan->loadRainDistribution(mp_CoreData));
}


// =====================================================================
// =====================================================================

bool Engine::checkConsistency()
{

  // builds topology by linking objects
  if (!mp_CoreData->getSpatialData()->buildObjectLinkedTopologyFromIDs())
  {
    mhydasdk::base::LastError::Message = wxT("Topology rebuild error.");
    return false;
  }

  if (mp_HydroModule == NULL /*|| !mp_HydroModule->checkConsistency()*/)
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

  mp_HydroModule->initialize();

  // run
  do
  {

    std::cout.width(8);
    std::cout << "t+" << mp_SimStatus->getCurrentStep();
    std::cout.width(20);
    std::cout << mp_SimStatus->getCurrentTime().asString().mb_str(wxConvUTF8) << std::endl;

    mp_HydroModule->runStep(mp_SimStatus);

  } while (mp_SimStatus->switchToNextStep());



  // finalization of functions
  mp_HydroModule->finalize();

  return true;
}

// =====================================================================
// =====================================================================



bool Engine::saveResults()
{
  return true;
}

// =====================================================================
// =====================================================================


