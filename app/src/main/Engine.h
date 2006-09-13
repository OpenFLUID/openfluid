/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __ENGINE_H__
#define __ENGINE_H__


#include "sdk-core.h"
#include "sdk-base.h"

#include "Module.h"
#include "IOMan.h"



/**

*/
class Engine
{
  private:
     Module *mp_HydroModule;
     FunctionsList m_HydroFunctions;

     mhydasdk::core::CoreRepository* mp_CoreData;

     mhydasdk::base::RuntimeEnvironment* mp_RunEnv;

     mhydasdk::base::SimulationStatus* mp_SimStatus;

     IOManager* mp_IOMan;

     EngineConfig m_Config;

     /**
       Processes the config file, check the list of plugins to load,
       loads them, registers params to pass and builds the processing list of each module.
     */
     bool processConfig();

     /**
       plugs the processing list into each module.
     */
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

    Module *getHydroModule() { return mp_HydroModule; };

    EngineConfig getConfig() const { return m_Config; };


};



#endif






