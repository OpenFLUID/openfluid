/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  @file
  @brief Implements ...

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/machine/ModelInstance.hpp>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/machine/MachineListener.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/FunctionPluginsManager.hpp>
#include <openfluid/machine/FixedGenerator.hpp>
#include <openfluid/machine/RandomGenerator.hpp>
#include <openfluid/machine/InterpGenerator.hpp>
#include <openfluid/machine/InjectGenerator.hpp>



namespace openfluid { namespace machine {


#define DECLARE_FUNCTION_PARSER \
    std::list<ModelItemInstance*>::const_iterator _M_FuncIter; \

/**
  Macro for parsing the functions list and calling the given method of each function of the list
  @param[in] calledmethod the method to call on each function
  @param[out] statevar the globalized return of the method calls
 */
#define PARSE_FUNCTION_LIST(calledmethod,listenermethod,timeprofilepart) \
    _M_FuncIter = m_ModelItems.begin(); \
    while (_M_FuncIter != m_ModelItems.end()) \
    { \
      ModelItemInstance* _M_CurrentFunction = *_M_FuncIter; \
      if (_M_CurrentFunction != NULL) \
      { \
        mp_Listener->onFunction##listenermethod(_M_CurrentFunction->Signature->ID); \
        boost::posix_time::ptime _M_TimeProfileStart = boost::posix_time::microsec_clock::universal_time(); \
        _M_CurrentFunction->Body->calledmethod; \
        if (mp_SimProfiler != NULL) mp_SimProfiler->addDuration(_M_CurrentFunction->Signature->ID,timeprofilepart,boost::posix_time::time_period(_M_TimeProfileStart,boost::posix_time::microsec_clock::universal_time()).length()); \
        if (mp_SimLogger->isWarningFlag())  mp_Listener->onFunction##listenermethod##Done(openfluid::machine::MachineListener::WARNING,_M_CurrentFunction->Signature->ID); \
        else  mp_Listener->onFunction##listenermethod##Done(openfluid::machine::MachineListener::OK,_M_CurrentFunction->Signature->ID); \
        mp_SimLogger->resetWarningFlag(); \
      } \
      _M_FuncIter++; \
    } \


// =====================================================================
// =====================================================================


ModelInstance::ModelInstance(openfluid::machine::SimulationBlob& SimulationBlob,
                             openfluid::machine::MachineListener* Listener)
             : mp_Listener(Listener), mp_SimLogger(NULL), mp_SimProfiler(NULL),
               m_SimulationBlob(SimulationBlob), m_Initialized(false)
{
  if (mp_Listener == NULL) mp_Listener = new openfluid::machine::MachineListener();
}


// =====================================================================
// =====================================================================


ModelInstance::~ModelInstance()
{
  if (m_Initialized)
    finalize();

  if (m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::~ModelInstance()","Destroyed initialized ModelInstance");
}


// =====================================================================
// =====================================================================


void ModelInstance::appendItemToTimePoint(openfluid::core::TimeIndex_t TimeIndex,
                                          openfluid::machine::ModelItemInstance* Item)
{

  // no back in time nor iteration
  if (TimeIndex <= m_SimulationBlob.getSimulationStatus().getCurrentTimeIndex())
    throw openfluid::base::OFException("OpenFLUID framework","SimulationScheduler::appendItemToTimePoint","Cannot append simulation item before or on current time point");

  // ignore time points after simulation end
  if (TimeIndex > m_SimulationBlob.getSimulationStatus().getSimulationDuration())
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


openfluid::ware::WareParams_t ModelInstance::mergeParamsWithGlobalParams(const openfluid::ware::WareParams_t& Params) const
{
  openfluid::ware::WareParams_t MergedParams = m_GlobalParams;

  for(openfluid::ware::WareParams_t::const_iterator it = Params.begin();it != Params.end();++it)
  {
    if(!it->second.get().empty())
      MergedParams[it->first] = it->second;
    else if(!m_GlobalParams.count(it->first))
      MergedParams[it->first] = it->second;
  }

  return MergedParams;
}


// =====================================================================
// =====================================================================


void ModelInstance::setGlobalParameter(const openfluid::ware::WareParamKey_t& Key, const openfluid::ware::WareParamKey_t& Value)
{
  m_GlobalParams[Key] = Value;
}


// =====================================================================
// =====================================================================


void ModelInstance::appendItem(ModelItemInstance* ItemInstance)
{
  if (m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::appendItem()","Trying to append model item after model initialization");

  m_ModelItems.push_back(ItemInstance);
}


// =====================================================================
// =====================================================================


void ModelInstance::insertItem(ModelItemInstance* ItemInstance, unsigned int Position)
{
  if (m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::insertItem()","Trying to insert model item after model initialization");


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
      throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::insertItem()","Bad index of item to insert");
  }

}


// =====================================================================
// =====================================================================


void ModelInstance::deleteItem(unsigned int Position)
{
  if (m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::deleteItem()","Trying to delete model item after model initialization");

  if (Position < m_ModelItems.size())
  {
    std::list<ModelItemInstance*>::iterator it = m_ModelItems.begin();
    for (unsigned int i = 0; i< Position; i++) ++it;
    m_ModelItems.erase(it);
  }
  else
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::deleteItem()","Bad index of item to delete");
}


// =====================================================================
// =====================================================================


void ModelInstance::clear()
{
  if (m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::clear()","Trying to clear model after model initialization");

  std::list<ModelItemInstance*>::iterator it;

  for (it=m_ModelItems.begin();it!=m_ModelItems.end();++it)
  {
    if ((*it)->Body != NULL) delete (*it)->Body;
    if ((*it)->Signature != NULL) delete (*it)->Signature;
  }

  m_ModelItems.clear();
}



// =====================================================================
// =====================================================================


void ModelInstance::initialize(openfluid::base::SimulationLogger* SimLogger)
{
  mp_SimLogger = SimLogger;

  openfluid::machine::SimulationProfiler::WareIDSequence_t FuncSequence;

  openfluid::machine::FunctionPluginsManager* FPlugsMgr = openfluid::machine::FunctionPluginsManager::getInstance();

  std::list<ModelItemInstance*>::const_iterator FuncIter;
  ModelItemInstance* CurrentFunction;

  FuncIter = m_ModelItems.begin();
  while (FuncIter != m_ModelItems.end())
  {
    CurrentFunction = (*FuncIter);

    if(CurrentFunction->ItemType == openfluid::fluidx::ModelItemDescriptor::PluggedFunction)
      FPlugsMgr->completeSignatureWithWareBody(CurrentFunction);

    if(CurrentFunction->ItemType == openfluid::fluidx::ModelItemDescriptor::Generator && CurrentFunction->GeneratorInfo != NULL)
    {
      if (CurrentFunction->GeneratorInfo->GeneratorMethod == openfluid::fluidx::GeneratorDescriptor::Fixed)
        CurrentFunction->Body = new FixedGenerator();

      if (CurrentFunction->GeneratorInfo->GeneratorMethod == openfluid::fluidx::GeneratorDescriptor::Random)
        CurrentFunction->Body = new RandomGenerator();

      if (CurrentFunction->GeneratorInfo->GeneratorMethod == openfluid::fluidx::GeneratorDescriptor::Inject)
        CurrentFunction->Body = new InjectGenerator();

      if (CurrentFunction->GeneratorInfo->GeneratorMethod == openfluid::fluidx::GeneratorDescriptor::Interp)
        CurrentFunction->Body = new InterpGenerator();

      ((openfluid::machine::Generator*)(CurrentFunction->Body))->setInfos(CurrentFunction->GeneratorInfo->VariableName,
                                                                              CurrentFunction->GeneratorInfo->UnitClass,
                                                                              CurrentFunction->GeneratorInfo->GeneratorMethod,
                                                                              CurrentFunction->GeneratorInfo->VariableSize);
    }

    CurrentFunction->Body->linkToSimulationLogger(mp_SimLogger);
    CurrentFunction->Body->linkToSimulation(&(m_SimulationBlob.getSimulationStatus()));
    CurrentFunction->Body->linkToRunEnvironment(openfluid::base::RuntimeEnvironment::getInstance()->getWareEnvironment());
    CurrentFunction->Body->linkToCoreRepository(&(m_SimulationBlob.getCoreRepository()));
    CurrentFunction->Body->linkToDatastore(&(m_SimulationBlob.getDatastore()));
    CurrentFunction->Body->initializeWare(CurrentFunction->Signature->ID,
                                    openfluid::base::RuntimeEnvironment::getInstance()->getFunctionsMaxNumThreads());
    FuncSequence.push_back(CurrentFunction->Signature->ID);

    FuncIter++;
  }

  if (openfluid::base::RuntimeEnvironment::getInstance()->isSimulationProfilingEnabled())
    mp_SimProfiler = new SimulationProfiler(&(m_SimulationBlob.getSimulationStatus()), FuncSequence);

  m_Initialized = true;
}



// =====================================================================
// =====================================================================


void ModelInstance::finalize()
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::finalize()","Trying to finalize an uninitialized model");

  std::list<ModelItemInstance*>::const_iterator FuncIter;


  // call of finalizeFunction method on each function
  FuncIter = m_ModelItems.begin();
  while (FuncIter != m_ModelItems.end())
  {
    (*FuncIter)->Body->finalizeWare();
    FuncIter++;
  }


  // destroy of each function
  FuncIter = m_ModelItems.begin();
  while (FuncIter != m_ModelItems.end())
  {
    delete (*FuncIter)->Body;
    FuncIter++;
  }

  if (mp_SimProfiler != NULL) delete mp_SimProfiler;
  mp_SimProfiler = NULL;

  m_Initialized = false;
}


// =====================================================================
// =====================================================================


void ModelInstance::call_initParams() const
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::call_initParams()","Model not initialized");


  DECLARE_FUNCTION_PARSER;
  PARSE_FUNCTION_LIST(initParams(mergeParamsWithGlobalParams(_M_CurrentFunction->Params)),InitParams,openfluid::base::SimulationStatus::INITPARAMS);
}


// =====================================================================
// =====================================================================


void ModelInstance::call_prepareData() const
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::call_prepareData()","Model not initialized");


  DECLARE_FUNCTION_PARSER;
  PARSE_FUNCTION_LIST(prepareData(),PrepareData,openfluid::base::SimulationStatus::PREPAREDATA);
}


// =====================================================================
// =====================================================================


void ModelInstance::call_checkConsistency() const
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::call_checkConsistency()","Model not initialized");


  DECLARE_FUNCTION_PARSER;
  PARSE_FUNCTION_LIST(checkConsistency(),CheckConsistency,openfluid::base::SimulationStatus::CHECKCONSISTENCY);
}


// =====================================================================
// =====================================================================


void ModelInstance::call_initializeRun()
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::call_initializeRun()","Model not initialized");

  std::list<ModelItemInstance*>::const_iterator FuncIter;


  FuncIter = m_ModelItems.begin();
  while (FuncIter != m_ModelItems.end())
  {
    ModelItemInstance* CurrentFunction = *FuncIter;
    if (CurrentFunction != NULL)
    {
      mp_Listener->onFunctionInitializeRun(CurrentFunction->Signature->ID);

      boost::posix_time::ptime TimeProfileStart = boost::posix_time::microsec_clock::universal_time();
      openfluid::base::SchedulingRequest SchedReq = CurrentFunction->Body->initializeRun();
      if (mp_SimProfiler != NULL) mp_SimProfiler->addDuration(CurrentFunction->Signature->ID,
                                                              openfluid::base::SimulationStatus::INITIALIZERUN,
                                                              boost::posix_time::time_period(TimeProfileStart,
                                                                                             boost::posix_time::microsec_clock::universal_time()).length());

      if (mp_SimLogger->isWarningFlag())  mp_Listener->onFunctionInitializeRunDone(openfluid::machine::MachineListener::WARNING,CurrentFunction->Signature->ID);
      else  mp_Listener->onFunctionInitializeRunDone(openfluid::machine::MachineListener::OK,CurrentFunction->Signature->ID);

      mp_SimLogger->resetWarningFlag();

      if (SchedReq.RequestType == openfluid::base::SchedulingRequest::ATTHEEND) // AtTheEnd();
      {
        appendItemToTimePoint(m_SimulationBlob.getSimulationStatus().getSimulationDuration(),
                              CurrentFunction);
      }
      else if (SchedReq.RequestType == openfluid::base::SchedulingRequest::DURATION) // != Never()
      {
        appendItemToTimePoint(m_SimulationBlob.getSimulationStatus().getCurrentTimeIndex()+SchedReq.Duration,
                              CurrentFunction);
      }

    }
    else
      throw openfluid::base::OFException("OpenFLUID framework","SimulationScheduler::call_initializeRun","NULL model item instance!");

    FuncIter++;
  }
}


// =====================================================================
// =====================================================================


void ModelInstance::processNextTimePoint()
{

  if (hasTimePointToProcess())
    m_SimulationBlob.getSimulationStatus().setCurrentTimeIndex(m_TimePointList.front().getTimeIndex());
  else
    return;

  m_TimePointList.front().sortByOriginalPosition();

  mp_Listener->onRunStep(&m_SimulationBlob.getSimulationStatus());

  while (m_TimePointList.front().hasItemsToProcess())
  {
    openfluid::machine::ModelItemInstance* NextItem = m_TimePointList.front().getNextItem();

    mp_Listener->onFunctionRunStep(NextItem->Signature->ID);
    boost::posix_time::ptime TimeProfileStart = boost::posix_time::microsec_clock::universal_time();

    openfluid::base::SchedulingRequest SchedReq = m_TimePointList.front().processNextItem();

    if (mp_SimProfiler != NULL) mp_SimProfiler->addDuration(NextItem->Signature->ID,
                                                            openfluid::base::SimulationStatus::RUNSTEP,
                                                            boost::posix_time::time_period(TimeProfileStart,boost::posix_time::microsec_clock::universal_time()).length());
    if (mp_SimLogger->isWarningFlag())
      mp_Listener->onFunctionRunStepDone(openfluid::machine::MachineListener::WARNING,NextItem->Signature->ID);
    else  mp_Listener->onFunctionRunStepDone(openfluid::machine::MachineListener::OK,NextItem->Signature->ID);
    mp_SimLogger->resetWarningFlag();


    if (SchedReq.RequestType == openfluid::base::SchedulingRequest::ATTHEEND) // AtTheEnd();
    {
      appendItemToTimePoint(m_SimulationBlob.getSimulationStatus().getSimulationDuration(),
                                           NextItem);
    }
    else if (SchedReq.RequestType == openfluid::base::SchedulingRequest::DURATION) // != Never()
    {
      appendItemToTimePoint(m_SimulationBlob.getSimulationStatus().getCurrentTimeIndex()+SchedReq.Duration,
                                     NextItem);
    }
  }

  if (mp_SimLogger->isWarningFlag()) mp_Listener->onRunStepDone(openfluid::machine::MachineListener::WARNING);
      mp_Listener->onRunStepDone(openfluid::machine::MachineListener::OK);

  m_TimePointList.pop_front();
}


// =====================================================================
// =====================================================================


void ModelInstance::call_finalizeRun() const
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::call_finalizeRun()","Model not initialized");


  DECLARE_FUNCTION_PARSER;
  PARSE_FUNCTION_LIST(finalizeRun(),FinalizeRun,openfluid::base::SimulationStatus::FINALIZERUN);
}


// =====================================================================
// =====================================================================



} } //namespaces

