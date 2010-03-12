/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file IOMan.h
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __IOMAN_H__
#define __IOMAN_H__

#include "openfluid-base.h"
#include "openfluid-core.h"
#include "openfluid-tools.h"
#include "RuntimeEnv.h"
#include "io/FluidXReader.h"
#include "io/OutputsWriter.h"

// =====================================================================
// =====================================================================

/**
  Structure storing the config of a function, read from the xml model file
*/
struct FunctionConfig
{
  std::string FileID;  // Plug-in fileID (filename without ext)
  openfluid::core::FuncParamsMap_t Params;  // Function parameters set
};


struct ModelConfig
{
  int DeltaT;

  std::list<FunctionConfig> FuncConfigs;

};


struct RunConfig
{
  int DeltaT;

  std::string SimulationID;

  openfluid::core::DateTime BeginDate;

  openfluid::core::DateTime EndDate;

};

// =====================================================================
// =====================================================================


class IOManager
{

  private:

    static IOManager* mp_Singleton;

    openfluid::core::MemoryMonitor* mp_MemMon;
    openfluid::core::CoreRepository* mp_Repository;
    openfluid::base::ExecutionMessages* mp_ExecMsgs;
    RuntimeEnvironment* mp_RunEnv;

    bool m_ClearedOuputDir;

    OutputsWriter* mp_OutputsWriter;

    FluidXReader m_FluidXData;

    IOManager();


  public:

    static IOManager* getInstance();

    void loadInputs(ModelDescriptor& ModelDesc, DomainDescriptor& DomainDesc,
                    RunDescriptor& RunDesc);


    bool prepareOutputs();

    bool saveOutputs(openfluid::core::TimeStep_t CurrentStep, openfluid::base::SimulationInfo *SimInfo, bool WithoutKeep);

    bool saveMessages();

    bool saveSimulationInfos(openfluid::base::SimulationInfo *SimInfo, std::string ErrorMsg);

    bool prepareOutputDir();



};


#endif /* __IOMAN_H__ */
