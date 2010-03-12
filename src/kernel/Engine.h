/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __ENGINE_H__
#define __ENGINE_H__


#include "openfluid-core.h"
#include "openfluid-base.h"


#include "IOMan.h"
#include "PluginManager.h"
#include "RuntimeEnv.h"

#include "model/ModelInstance.h"



// =====================================================================
// =====================================================================


/**
  Computation engine class
*/
class Engine
{
  private:

     openfluid::core::CoreRepository* mp_CoreData;

     RuntimeEnvironment* mp_RunEnv;

     openfluid::base::ExecutionMessages* mp_ExecMsgs;

     openfluid::base::SimulationStatus* mp_SimStatus;

     /**
       Plugin manager
     */
     PluginManager* mp_PlugMan;

     /**
       Memory monitor
     */
     openfluid::core::MemoryMonitor* mp_MemMon;

     /**
       IOManager
     */
     IOManager* mp_IOMan;


     ModelDescriptor m_ModelDesc;

     RunDescriptor m_RunDesc;

     DomainDescriptor m_DomainDesc;


     const ModelInstance* mp_ModelInstance;


     bool checkSimulationVarsProduction(int ExpectedVarsCount, std::string* Message);

     bool checkModelConsistency();

     bool checkDataConsistency();

     bool checkExtraFilesConsistency();

     bool checkExistingVariable(openfluid::core::VariableName_t VarName,
                                openfluid::core::UnitClass_t ClassName);

     bool createVariable(openfluid::core::VariableName_t VarName,
                         openfluid::core::UnitClass_t ClassName, bool UpdateMode);

     bool checkExistingInputData(openfluid::core::InputDataName_t DataName,
                                 openfluid::core::UnitClass_t ClassName);

  public:
    /**
      Constructor
    */
    Engine();

    /**
      Destructor
    */
    ~Engine();

    bool buildModel();

    bool buildSpatialDomain();

    bool loadData();

    bool processRunConfiguration();

    bool initParams();

    bool prepareDataAndCheckConsistency();

    bool run();

    bool saveReports(std::string ErrorMsg);

    bool saveMessages();

    openfluid::base::SimulationInfo *getSimulationInfo() { return (openfluid::base::SimulationInfo*)mp_SimStatus; };

};



#endif






