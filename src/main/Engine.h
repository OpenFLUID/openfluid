/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __ENGINE_H__
#define __ENGINE_H__


#include "mhydasdk-core.h"
#include "mhydasdk-base.h"


#include "IOMan.h"
#include "PluginManager.h"
#include "RuntimeEnv.h"


#include <wx/list.h>
WX_DECLARE_LIST(mhydasdk::base::PluggableFunction*, FunctionsList);

// =====================================================================
// =====================================================================



/**
  Computation engine class
*/
class Engine
{
  private:

     FunctionsList m_Functions;

     mhydasdk::core::CoreRepository* mp_CoreData;

     RuntimeEnvironment* mp_RunEnv;
     mhydasdk::base::ExecutionMessages* mp_ExecMsgs;

     mhydasdk::base::SimulationStatus* mp_SimStatus;

     /**
       Plugin manager 
     */
     
     /**
       IOManager
     */     
     PluginManager* mp_PlugMan;

     IOManager* mp_IOMan;

     /**

     */
     EngineConfig m_Config;

     /**
       Processes the config file, check the list of plugins to load,
       loads them, registers params to pass and builds the processing list.
     */
     bool processConfig();

  public:
    /**
      Constructor
    */
    Engine(mhydasdk::core::CoreRepository* CoreData, mhydasdk::base::ExecutionMessages* ExecMsgs, RuntimeEnvironment* RunEnv,
           PluginManager* PlugMan);

    /**
      Destructor
    */
    ~Engine();

    bool buildModel();

    bool loadData();

    bool prepareDataAndCheckConsistency();

    bool run();

    bool saveResults(ExtraSimInfos ExSI);

    EngineConfig getConfig() const { return m_Config; };

    mhydasdk::base::SimulationInfo *getSimulationInfo() { return (mhydasdk::base::SimulationInfo*)mp_SimStatus; };

};



#endif






