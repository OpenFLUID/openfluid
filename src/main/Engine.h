/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __ENGINE_H__
#define __ENGINE_H__


#include "openfluid-core.h"
#include "openfluid-base.h"


#include "IOMan.h"
#include "PluginManager.h"
#include "RuntimeEnv.h"


#include <wx/list.h>
WX_DECLARE_LIST(PluginContainer*, PluginsList);

// =====================================================================
// =====================================================================



/**
  Computation engine class
*/
class Engine
{
  private:

     PluginsList m_Functions;

     openfluid::core::CoreRepository* mp_CoreData;

     RuntimeEnvironment* mp_RunEnv;
     openfluid::base::ExecutionMessages* mp_ExecMsgs;

     openfluid::base::SimulationStatus* mp_SimStatus;

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
     ModelConfig m_ModelConfig;

     /**

     */
     RunConfig m_RunConfig;
     
     
     /**
       Processes the config file, check the list of plugins to load,
       loads them, registers params to pass and builds the processing list.
     */
     bool processConfig();

     bool checkSimulationVarsProduction(int ExpectedVarsCount, wxString* Message);

     bool checkModelConsistency();

     bool checkDataConsistency();
          
     bool checkExtraFilesConsistency();
          
     
  public:
    /**
      Constructor
    */
    Engine(openfluid::core::CoreRepository* CoreData, openfluid::base::ExecutionMessages* ExecMsgs, RuntimeEnvironment* RunEnv,
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

    bool saveReports(ExtraSimInfos ExSI);

    ModelConfig getModelConfig() const { return m_ModelConfig; };

    RunConfig getRunConfig() const { return m_RunConfig; };

    openfluid::base::SimulationInfo *getSimulationInfo() { return (openfluid::base::SimulationInfo*)mp_SimStatus; };

};



#endif






