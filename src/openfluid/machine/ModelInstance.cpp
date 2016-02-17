/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/



/**
  @file ModelInstance.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/machine/ModelInstance.hpp>

#include <openfluid/machine/MachineListener.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/machine/FixedGenerator.hpp>
#include <openfluid/machine/RandomGenerator.hpp>
#include <openfluid/machine/InterpGenerator.hpp>
#include <openfluid/machine/InjectGenerator.hpp>
#include <openfluid/tools/DataHelpers.hpp>


namespace openfluid { namespace machine {


#define DECLARE_SIMULATOR_PARSER \
    std::list<ModelItemInstance*>::const_iterator _M_SimIter; \

/**
  Macro for parsing the simulators list and calling the given method of each simulator of the list
  @param[in] calledmethod the method to call on each simulator
  @param[out] statevar the globalized return of the method calls
 */
#define PARSE_SIMULATOR_LIST(calledmethod,listenermethod,timeprofilepart) \
    _M_SimIter = m_ModelItems.begin(); \
    while (_M_SimIter != m_ModelItems.end()) \
    { \
      ModelItemInstance* _M_CurrentSimulator = *_M_SimIter; \
      if (_M_CurrentSimulator != nullptr) \
      { \
        mp_Listener->onSimulator##listenermethod(_M_CurrentSimulator->Signature->ID); \
        std::chrono::high_resolution_clock::time_point _M_TimeProfileStart = \
          std::chrono::high_resolution_clock::now(); \
        _M_CurrentSimulator->Body->calledmethod; \
        if (mp_SimProfiler != nullptr)\
        { \
          mp_SimProfiler->addDuration(_M_CurrentSimulator->Signature->ID,\
                                      timeprofilepart, \
                                      std::chrono::duration_cast<SimulationProfiler::TimeResolution_t>(\
                                        std::chrono::high_resolution_clock::now() - _M_TimeProfileStart)); \
        } \
        if (mp_SimLogger->isCurrentWarningFlag()) \
          mp_Listener->onSimulator##listenermethod##Done(openfluid::machine::MachineListener::LISTEN_WARNING,\
                                                         _M_CurrentSimulator->Signature->ID); \
        else \
          mp_Listener->onSimulator##listenermethod##Done(openfluid::machine::MachineListener::LISTEN_OK,\
                                                         _M_CurrentSimulator->Signature->ID); \
        mp_SimLogger->resetCurrentWarningFlag(); \
      } \
      ++_M_SimIter; \
    } \


// =====================================================================
// =====================================================================


ModelInstance::ModelInstance(openfluid::machine::SimulationBlob& SimulationBlob,
                             openfluid::machine::MachineListener* Listener)
             : mp_Listener(Listener), mp_SimLogger(nullptr), mp_SimProfiler(nullptr),
               m_SimulationBlob(SimulationBlob), m_Initialized(false)
{
  if (!mp_Listener)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Listener can not be NULL");
}


// =====================================================================
// =====================================================================


ModelInstance::~ModelInstance()
{
  if (m_Initialized)
    finalize();
}


// =====================================================================
// =====================================================================


void ModelInstance::appendItemToTimePoint(openfluid::core::TimeIndex_t TimeIndex,
                                          openfluid::machine::ModelItemInstance* Item)
{

  // no back in time nor iteration
  if (TimeIndex <= m_SimulationBlob.simulationStatus().getCurrentTimeIndex())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Cannot append simulation item before or on current time point");

  // ignore time points after simulation end
  if (TimeIndex > m_SimulationBlob.simulationStatus().getSimulationDuration())
    return;

  // add time point if no existing time point
  if (m_TimePointList.empty())
  {
    m_TimePointList.push_back(ExecutionTimePoint(TimeIndex));
    m_TimePointList.back().appendItem(Item);
    return;
  }

  // add time point if after existing time points
  if (TimeIndex > m_TimePointList.back().getTimeIndex())
  {
    m_TimePointList.push_back(ExecutionTimePoint(TimeIndex));
    m_TimePointList.back().appendItem(Item);
  }
  else // search for the right time point insertion
  {
    bool TmpInserted = false;
    std::list<ExecutionTimePoint>::iterator TPit = m_TimePointList.begin();

    while (TPit != m_TimePointList.end() && !TmpInserted)
    {
      if ((*TPit).getTimeIndex() == TimeIndex)
      {
        (*TPit).appendItem(Item);
        TmpInserted = true;
      }
      else if ((*TPit).getTimeIndex() > TimeIndex)
      {
        ExecutionTimePoint TmpPoint(TimeIndex);
        TmpPoint.appendItem(Item);
        m_TimePointList.insert(TPit,TmpPoint);
        TmpInserted = true;
      }
      else
      {
        ++TPit;
      }
    }

  }
}


// =====================================================================
// =====================================================================


openfluid::ware::WareParams_t
  ModelInstance::mergeParamsWithGlobalParams(const openfluid::ware::WareParams_t& Params) const
{
  openfluid::ware::WareParams_t MergedParams = m_GlobalParams;

  for(openfluid::ware::WareParams_t::const_iterator it = Params.begin();it != Params.end();++it)
  {
    if(!it->second.data().empty())
      MergedParams[it->first] = it->second;
    else if(!m_GlobalParams.count(it->first))
      MergedParams[it->first] = it->second;
  }

  return MergedParams;
}


// =====================================================================
// =====================================================================


void ModelInstance::setGlobalParameter(const openfluid::ware::WareParamKey_t& Key,
                                       const openfluid::ware::WareParamValue_t& Value)
{
  m_GlobalParams[Key] = Value;
}


// =====================================================================
// =====================================================================


void ModelInstance::appendItem(ModelItemInstance* ItemInstance)
{
  if (m_Initialized)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Trying to append model item after model initialization");

  m_ModelItems.push_back(ItemInstance);
}


// =====================================================================
// =====================================================================


void ModelInstance::insertItem(ModelItemInstance* ItemInstance, unsigned int Position)
{
  if (m_Initialized)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Trying to insert model item after model initialization");


  if (Position == 0)
    m_ModelItems.insert(m_ModelItems.begin(),ItemInstance);
  else
  {
    if (Position < m_ModelItems.size())
    {
      std::list<ModelItemInstance*>::iterator it = m_ModelItems.begin();
      for (unsigned int i = 0; i< Position; i++) ++it;
      m_ModelItems.insert(it,ItemInstance);
    }
    else
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "Bad index of item to insert");
  }

}


// =====================================================================
// =====================================================================


void ModelInstance::deleteItem(unsigned int Position)
{
  if (m_Initialized)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Trying to delete model item after model initialization");

  if (Position < m_ModelItems.size())
  {
    std::list<ModelItemInstance*>::iterator it = m_ModelItems.begin();
    for (unsigned int i = 0; i< Position; i++) ++it;
    m_ModelItems.erase(it);
  }
  else
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Bad index of item to delete");
}


// =====================================================================
// =====================================================================


void ModelInstance::clear()
{
  if (m_Initialized)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Trying to clear model after model initialization");

  std::list<ModelItemInstance*>::iterator it;

  for (it=m_ModelItems.begin();it!=m_ModelItems.end();++it)
  {
    (*it)->Body.reset();
    // TODO reset Signature?
  }

  m_ModelItems.clear();
}



// =====================================================================
// =====================================================================


void ModelInstance::initialize(openfluid::base::SimulationLogger* SimLogger)
{
  mp_SimLogger = SimLogger;

  openfluid::machine::SimulationProfiler::WareIDSequence_t SimSequence;

  openfluid::machine::SimulatorPluginsManager* FPlugsMgr = openfluid::machine::SimulatorPluginsManager::instance();

  std::list<ModelItemInstance*>::const_iterator SimIter;
  ModelItemInstance* CurrentSimulator;

  SimIter = m_ModelItems.begin();
  while (SimIter != m_ModelItems.end())
  {
    CurrentSimulator = (*SimIter);

    if(CurrentSimulator->ItemType == openfluid::ware::WareType::SIMULATOR)
      FPlugsMgr->completeSignatureWithWareBody(CurrentSimulator);

    if (CurrentSimulator->ItemType == openfluid::ware::WareType::GENERATOR &&
        CurrentSimulator->GeneratorInfo != nullptr)
    {
      if (CurrentSimulator->GeneratorInfo->GeneratorMethod == openfluid::fluidx::GeneratorDescriptor::Fixed)
        CurrentSimulator->Body.reset(new FixedGenerator());

      if (CurrentSimulator->GeneratorInfo->GeneratorMethod == openfluid::fluidx::GeneratorDescriptor::Random)
        CurrentSimulator->Body.reset(new RandomGenerator());

      if (CurrentSimulator->GeneratorInfo->GeneratorMethod == openfluid::fluidx::GeneratorDescriptor::Inject)
        CurrentSimulator->Body.reset(new InjectGenerator());

      if (CurrentSimulator->GeneratorInfo->GeneratorMethod == openfluid::fluidx::GeneratorDescriptor::Interp)
        CurrentSimulator->Body.reset(new InterpGenerator());

      ((openfluid::machine::Generator*)
          (CurrentSimulator->Body.get()))->setInfos(CurrentSimulator->GeneratorInfo->VariableName,
                                              CurrentSimulator->GeneratorInfo->UnitsClass,
                                              CurrentSimulator->GeneratorInfo->GeneratorMethod,
                                              CurrentSimulator->GeneratorInfo->VariableSize);
    }

    CurrentSimulator->Body->linkToSimulationLogger(mp_SimLogger);
    CurrentSimulator->Body->linkToSimulation(&(m_SimulationBlob.simulationStatus()));
    CurrentSimulator->Body->linkToRunEnvironment(&openfluid::base::RunContextManager::instance()
                                                   ->getWaresEnvironment());
    CurrentSimulator->Body->linkToSpatialGraph(&(m_SimulationBlob.spatialGraph()));
    CurrentSimulator->Body->linkToDatastore(&(m_SimulationBlob.datastore()));
    CurrentSimulator->Body->initializeWare(CurrentSimulator->Signature->ID,
                                           openfluid::base::RunContextManager::instance()->getWaresMaxNumThreads());
    SimSequence.push_back(CurrentSimulator->Signature->ID);

    ++SimIter;
  }

  if (openfluid::base::RunContextManager::instance()->isProfiling())
    mp_SimProfiler = new SimulationProfiler(&(m_SimulationBlob.simulationStatus()), SimSequence);

  m_Initialized = true;
}



// =====================================================================
// =====================================================================


void ModelInstance::finalize()
{
  if (!m_Initialized)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Trying to finalize an uninitialized model");

  std::list<ModelItemInstance*>::const_iterator SimIter;


  // call of finalizeWare method on each simulator
  SimIter = m_ModelItems.begin();
  while (SimIter != m_ModelItems.end())
  {
    (*SimIter)->Body->finalizeWare();
    ++SimIter;
  }


  // destroy of each simulator
  SimIter = m_ModelItems.begin();
  while (SimIter != m_ModelItems.end())
  {
    (*SimIter)->Body.reset();
    // TODO reset Signature?
    ++SimIter;
  }

  if (mp_SimProfiler != nullptr)
    delete mp_SimProfiler;
  mp_SimProfiler = nullptr;

  m_Initialized = false;
}


// =====================================================================
// =====================================================================


void ModelInstance::call_initParams() const
{
  if (!m_Initialized)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Model not initialized");


  DECLARE_SIMULATOR_PARSER;
  PARSE_SIMULATOR_LIST(initParams(mergeParamsWithGlobalParams(_M_CurrentSimulator->Params)),
                       InitParams,openfluid::base::SimulationStatus::INITPARAMS);
}


// =====================================================================
// =====================================================================


void ModelInstance::call_prepareData() const
{
  if (!m_Initialized)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Model not initialized");


  DECLARE_SIMULATOR_PARSER;
  PARSE_SIMULATOR_LIST(prepareData(),PrepareData,openfluid::base::SimulationStatus::PREPAREDATA);
}


// =====================================================================
// =====================================================================


void ModelInstance::call_checkConsistency() const
{
  if (!m_Initialized)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Model not initialized");


  DECLARE_SIMULATOR_PARSER;
  PARSE_SIMULATOR_LIST(checkConsistency(),CheckConsistency,openfluid::base::SimulationStatus::CHECKCONSISTENCY);
}


// =====================================================================
// =====================================================================


void ModelInstance::checkDeltaTMode(openfluid::base::SchedulingRequest& SReq, const openfluid::ware::WareID_t& ID)
{
  // check if "checked" DeltaT mode is respected
  if (m_SimulationBlob.simulationStatus().getSchedulingConstraint() ==
        openfluid::base::SimulationStatus::SCHED_DTCHECKED)
  {
     if (!(SReq.RequestType == openfluid::base::SchedulingRequest::DURATION &&
           SReq.Duration ==  m_SimulationBlob.simulationStatus().getDefaultDeltaT()))
     {
       std::string TIStr;
       openfluid::tools::convertValue(m_SimulationBlob.simulationStatus().getCurrentTimeIndex(),&TIStr);
       throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                          "DeltaT checked mode not respected by simulator " + ID +
                                          " at time index " + TIStr);
     }
  }

  // check if "forced" DeltaT mode is respected
  if (m_SimulationBlob.simulationStatus().getSchedulingConstraint() ==
        openfluid::base::SimulationStatus::SCHED_DTFORCED)
    SReq =  openfluid::base::SchedulingRequest(m_SimulationBlob.simulationStatus().getDefaultDeltaT());

}


// =====================================================================
// =====================================================================


void ModelInstance::call_initializeRun()
{
  if (!m_Initialized)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Model not initialized");

  std::list<ModelItemInstance*>::const_iterator SimIter;


  SimIter = m_ModelItems.begin();
  while (SimIter != m_ModelItems.end())
  {
    ModelItemInstance* CurrentSimulator = *SimIter;
    if (CurrentSimulator != nullptr)
    {
      mp_Listener->onSimulatorInitializeRun(CurrentSimulator->Signature->ID);

      std::chrono::high_resolution_clock::time_point TimeProfileStart = std::chrono::high_resolution_clock::now();

      openfluid::base::SchedulingRequest SchedReq = CurrentSimulator->Body->initializeRun();

      if (mp_SimProfiler != nullptr)
        mp_SimProfiler->addDuration(CurrentSimulator->Signature->ID,
                                    openfluid::base::SimulationStatus::INITIALIZERUN,
                                    std::chrono::duration_cast<SimulationProfiler::TimeResolution_t>(
                                        std::chrono::high_resolution_clock::now()-TimeProfileStart)
                                    );

      if (mp_SimLogger->isCurrentWarningFlag())
        mp_Listener->onSimulatorInitializeRunDone(openfluid::machine::MachineListener::LISTEN_WARNING,
                                                  CurrentSimulator->Signature->ID);
      else
        mp_Listener->onSimulatorInitializeRunDone(openfluid::machine::MachineListener::LISTEN_OK,
                                                  CurrentSimulator->Signature->ID);
      mp_SimLogger->resetCurrentWarningFlag();

      checkDeltaTMode(SchedReq,CurrentSimulator->Signature->ID);

      // TODO optimize by testing DURATION before ATTHEEND
      if (SchedReq.RequestType == openfluid::base::SchedulingRequest::ATTHEEND) // AtTheEnd();
      {
        appendItemToTimePoint(m_SimulationBlob.simulationStatus().getSimulationDuration(),
                              CurrentSimulator);
      }
      else if (SchedReq.RequestType == openfluid::base::SchedulingRequest::DURATION) // != Never()
      {
        appendItemToTimePoint(m_SimulationBlob.simulationStatus().getCurrentTimeIndex()+SchedReq.Duration,
                              CurrentSimulator);
      }

    }
    else
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"NULL model item instance");

    ++SimIter;
  }
}


// =====================================================================
// =====================================================================


void ModelInstance::processNextTimePoint()
{

  if (hasTimePointToProcess())
    m_SimulationBlob.simulationStatus().setCurrentTimeIndex(m_TimePointList.front().getTimeIndex());
  else
    return;

  bool AtLeastOneWarningFlag = false;

  m_TimePointList.front().sortByOriginalPosition();

  mp_Listener->onRunStep(&m_SimulationBlob.simulationStatus());

  while (m_TimePointList.front().hasItemsToProcess())
  {

    openfluid::machine::ModelItemInstance* NextItem = m_TimePointList.front().nextItem();

    mp_Listener->onSimulatorRunStep(NextItem->Signature->ID);
    std::chrono::high_resolution_clock::time_point TimeProfileStart = std::chrono::high_resolution_clock::now();

    openfluid::base::SchedulingRequest SchedReq = m_TimePointList.front().processNextItem();

    if (mp_SimProfiler != nullptr)
      mp_SimProfiler->addDuration(NextItem->Signature->ID,
                                  openfluid::base::SimulationStatus::RUNSTEP,
                                  std::chrono::duration_cast<SimulationProfiler::TimeResolution_t>(
                                      std::chrono::high_resolution_clock::now()-TimeProfileStart)
                                 );

    if (mp_SimLogger->isCurrentWarningFlag())
    {
      AtLeastOneWarningFlag = true;
      mp_Listener->onSimulatorRunStepDone(openfluid::machine::MachineListener::LISTEN_WARNING,NextItem->Signature->ID);
    }
    else
      mp_Listener->onSimulatorRunStepDone(openfluid::machine::MachineListener::LISTEN_OK,NextItem->Signature->ID);

    mp_SimLogger->resetCurrentWarningFlag();

    checkDeltaTMode(SchedReq,NextItem->Signature->ID);

    if (SchedReq.RequestType == openfluid::base::SchedulingRequest::ATTHEEND) // AtTheEnd();
    {
      appendItemToTimePoint(m_SimulationBlob.simulationStatus().getSimulationDuration(),
                                           NextItem);
    }
    else if (SchedReq.RequestType == openfluid::base::SchedulingRequest::DURATION) // != Never()
    {
      appendItemToTimePoint(m_SimulationBlob.simulationStatus().getCurrentTimeIndex()+SchedReq.Duration,
                                     NextItem);
    }
  }

  if (AtLeastOneWarningFlag)
    mp_Listener->onRunStepDone(openfluid::machine::MachineListener::LISTEN_WARNING);
  else
    mp_Listener->onRunStepDone(openfluid::machine::MachineListener::LISTEN_OK);

  m_TimePointList.pop_front();
}


// =====================================================================
// =====================================================================


void ModelInstance::call_finalizeRun() const
{
  if (!m_Initialized)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Model not initialized");


  DECLARE_SIMULATOR_PARSER;
  PARSE_SIMULATOR_LIST(finalizeRun(),FinalizeRun,openfluid::base::SimulationStatus::FINALIZERUN);
}


// =====================================================================
// =====================================================================



} } //namespaces

