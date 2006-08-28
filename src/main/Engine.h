/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __ENGINE_H__
#define __ENGINE_H__


#include "sdk-core.h"
#include "sdk-base.h"

#include "HydroModule.h"
#include "IOMan.h"



/**

*/
class Engine
{
  private:
     HydroModule *mp_HydroModule;

     mhydasdk::core::CoreRepository* mp_CoreData;

     mhydasdk::base::RuntimeEnvironment* mp_RunEnv;

     mhydasdk::base::SimulationStatus* mp_SimStatus;

     IOManager* mp_IOMan;

     EngineConfig m_Config;

     bool processConfig();

     bool plugFunctions();

  public:
    /**
      Constructor
    */
    Engine(mhydasdk::core::CoreRepository* CoreData, mhydasdk::base::RuntimeEnvironment* RunEnv);

    /**
      Destructor
    */
    ~Engine();

    bool buildModel();

    bool checkConsistency();

    bool loadData();

    bool run();

    bool saveResults();

    HydroModule *getHydroModule() { return mp_HydroModule; };

    EngineConfig getConfig() const { return m_Config; };


};



#endif






