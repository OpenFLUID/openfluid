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

  mp_IOMan = new IOManager(mp_RunEnv);

}

// =====================================================================
// =====================================================================

Engine::~Engine()
{

}


bool Engine::processConfig()
{

  return true;
}


// =====================================================================
// =====================================================================


bool Engine::plugFunctions()
{

  return true;
}


// =====================================================================
// =====================================================================

bool Engine::buildModel()
{
  return (mp_IOMan->loadModelConfig(&m_Config) && processConfig() && plugFunctions());
}


// =====================================================================
// =====================================================================

bool Engine::loadData()
{
  return (mp_IOMan->loadHydroObjects(mp_CoreData->getSpatialData()) &&
          mp_IOMan->loadRainEvent(mp_CoreData->getRainEvent()));
}


// =====================================================================
// =====================================================================

bool Engine::checkConsistency()
{
  if (!mp_CoreData->getSpatialData()->buildObjectLinkedTopologyFromIDs())
  {
    mhydasdk::base::LastError::Message = wxT("Topology rebuild error.");
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


  // run
  do
  {
    /*
    std::cout.width(8);
    std::cout << "t" << mp_SimStatus->getCurrentStep();
    std::cout.width(20);
    std::cout << mp_SimStatus->getCurrentTime().asString().mb_str(wxConvUTF8) << std::endl;
    */

  } while (mp_SimStatus->switchToNextStep());



  // finalization of functions


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


