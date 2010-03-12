/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file IOMan.cpp
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "IOMan.h"
#include "AppTools.h"
#include "config.h"
#include <sstream>
#include <fstream>

#include <boost/filesystem.hpp>


#include "io/MessagesWriter.h"
#include "io/SimReportWriter.h"
#include "io/OutputsWriter.h"

IOManager* IOManager::mp_Singleton = NULL;


// =====================================================================
// =====================================================================


IOManager::IOManager()
{
  mp_MemMon = openfluid::core::MemoryMonitor::getInstance();
  mp_Repository = openfluid::core::CoreRepository::getInstance();
  mp_ExecMsgs = openfluid::base::ExecutionMessages::getInstance();
  mp_RunEnv = RuntimeEnvironment::getInstance();

  mp_OutputsWriter = NULL;

};

// =====================================================================
// =====================================================================

IOManager* IOManager::getInstance()
{
  if (mp_Singleton == NULL) mp_Singleton = new IOManager();

  return mp_Singleton;
}

// =====================================================================
// =====================================================================


bool IOManager::prepareOutputDir()
{

  boost::filesystem::path OutputDirPath(mp_RunEnv->getOutputDir());


  if (!boost::filesystem::exists(OutputDirPath))
  {
    boost::filesystem::create_directory(OutputDirPath);
    if (!boost::filesystem::exists(OutputDirPath))
      throw openfluid::base::OFException("kernel","IOManager::prepareOutputDir","Error creating output directory");

  }
  else
  {
    if (mp_RunEnv->isClearOutputDir() && !m_ClearedOuputDir)
    {
      EmptyDirectoryRecursively(mp_RunEnv->getOutputDir().c_str());
      m_ClearedOuputDir = true;
    }
  }


  // create empty message file

  std::ofstream OutMsgFile;

  boost::filesystem::path OutMsgFilePath = boost::filesystem::path(mp_RunEnv->getOutputFullPath(CONFIG_OUTMSGSFILE));
  OutMsgFile.open(OutMsgFilePath.string().c_str(),std::ios::out);
  OutMsgFile.close();

  return true;
}

// =====================================================================
// =====================================================================



bool IOManager::prepareOutputs()
{

  mp_OutputsWriter->prepareDirectory();

  return true;
}


// =====================================================================
// =====================================================================


bool IOManager::saveOutputs(openfluid::core::TimeStep_t CurrentStep, openfluid::base::SimulationInfo *SimInfo, bool WithoutKeep)
{

  mp_OutputsWriter->saveToDirectory(CurrentStep,SimInfo,WithoutKeep);

  return true;
}


// =====================================================================
// =====================================================================


bool IOManager::saveMessages()
{
  MessagesWriter::saveToFile(mp_RunEnv->getOutputFullPath(CONFIG_OUTMSGSFILE));

  return true;
}


// =====================================================================
// =====================================================================


bool IOManager::saveSimulationInfos(openfluid::base::SimulationInfo *SimInfo, std::string ErrorMsg)
{
  SimulationReportWriter::saveToFile(mp_RunEnv->getOutputFullPath(CONFIG_SIMINFOFILE),SimInfo);

  return true;
}

// =====================================================================
// =====================================================================

void IOManager::loadInputs(ModelDescriptor& ModelDesc, DomainDescriptor& DomainDesc,
                           RunDescriptor& RunDesc)
{
  m_FluidXData.loadFromDirectory(mp_RunEnv->getInputDir());

  ModelDesc = m_FluidXData.getModelDescriptor();
  DomainDesc = m_FluidXData.getDomainDescriptor();
  RunDesc = m_FluidXData.getRunDescriptor();

  mp_OutputsWriter = new OutputsWriter(mp_RunEnv->getOutputDir(),m_FluidXData.getOutputDescriptor());
}
