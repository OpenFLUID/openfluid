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


namespace openfluid { namespace machine {


#define DECLARE_FUNCTION_PARSER \
    std::list<ModelItemInstance*>::const_iterator _M_FuncIter; \

/**
  Macro for parsing the functions list and calling the given method of each function of the list
  @param[in] calledmethod the method to call on each function
  @param[out] statevar the globalized return of the method calls
 */
#define PARSE_FUNCTION_LIST(calledmethod,listenermethod,timeprofilepart,statevar) \
    _M_FuncIter = m_ModelItems.begin(); \
    while (_M_FuncIter != m_ModelItems.end() && statevar) \
    { \
      ModelItemInstance* _M_CurrentFunction = *_M_FuncIter; \
      if (_M_CurrentFunction != NULL) \
      { \
        mp_Listener->onFunction##listenermethod(_M_CurrentFunction->Signature->ID); \
        boost::posix_time::ptime _M_TimeProfileStart = boost::posix_time::microsec_clock::universal_time(); \
        statevar = (statevar && _M_CurrentFunction->Function->calledmethod); \
        openfluid::base::SimulationProfiler::getInstance()->addDuration(_M_CurrentFunction->Signature->ID,timeprofilepart,boost::posix_time::time_period(_M_TimeProfileStart,boost::posix_time::microsec_clock::universal_time()).length()); \
        if (!statevar) \
        { \
          mp_Listener->onFunction##listenermethod##Done(openfluid::machine::MachineListener::ERROR,_M_CurrentFunction->Signature->ID); \
          throw openfluid::base::OFException("OpenFLUID framework","Bad return value while calling function method"); \
        }  \
        else \
        { \
          if (m_SimulationBlob.getExecutionMessages().isWarningFlag())  mp_Listener->onFunction##listenermethod##Done(openfluid::machine::MachineListener::WARNING,_M_CurrentFunction->Signature->ID); \
          else  mp_Listener->onFunction##listenermethod##Done(openfluid::machine::MachineListener::OK,_M_CurrentFunction->Signature->ID); \
          m_SimulationBlob.getExecutionMessages().resetWarningFlag(); \
        } \
      } \
      _M_FuncIter++; \
    } \


// =====================================================================
// =====================================================================


ModelInstance::ModelInstance(openfluid::machine::SimulationBlob& SimulationBlob,
                             openfluid::machine::MachineListener* Listener)
             : mp_Listener(Listener), m_SimulationBlob(SimulationBlob), m_Initialized(false)
{
  if (mp_Listener == NULL) mp_Listener = new openfluid::machine::MachineListener();
}


// =====================================================================
// =====================================================================


ModelInstance::~ModelInstance()
{

}


// =====================================================================
// =====================================================================


openfluid::core::FuncParamsMap_t ModelInstance::mergeParamsWithGlobalParams(const openfluid::core::FuncParamsMap_t& Params) const
{
  openfluid::core::FuncParamsMap_t MergedParams = m_GlobalParams;
  openfluid::core::FuncParamsMap_t::const_iterator itParams;

  for(itParams = Params.begin();itParams != Params.end();++itParams)
    MergedParams[(*itParams).first] = (*itParams).second;

  return MergedParams;
}


// =====================================================================
// =====================================================================


void ModelInstance::setGlobalParameter(const openfluid::core::FuncParamKey_t& Key, const openfluid::core::FuncParamKey_t& Value)
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
    if ((*it)->Function != NULL) delete (*it)->Function;
    if ((*it)->Signature != NULL) delete (*it)->Signature;
  }

  m_ModelItems.clear();
}



// =====================================================================
// =====================================================================


void ModelInstance::initialize()
{
  std::list<ModelItemInstance*>::const_iterator FuncIter;

  FuncIter = m_ModelItems.begin();
  while (FuncIter != m_ModelItems.end())
  {
    (*FuncIter)->Function->initializeFunction(&(m_SimulationBlob.getCoreRepository()),&(m_SimulationBlob.getExecutionMessages()),
        openfluid::base::RuntimeEnvironment::getInstance()->getFunctionEnvironment(),
        openfluid::base::RuntimeEnvironment::getInstance()->getFunctionsMaxNumThreads(),
        (*FuncIter)->Signature->ID);
    FuncIter++;
  }
  m_Initialized = true;
}



// =====================================================================
// =====================================================================


void ModelInstance::finalize()
{
  std::list<ModelItemInstance*>::const_iterator FuncIter;

  FuncIter = m_ModelItems.begin();
  while (FuncIter != m_ModelItems.end())
  {
    (*FuncIter)->Function->finalizeFunction();
    FuncIter++;
  }
  m_Initialized = false;
}


// =====================================================================
// =====================================================================


bool ModelInstance::call_initParams() const
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::call_initParams()","Model not initialized");


  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(initParams(mergeParamsWithGlobalParams(_M_CurrentFunction->Params)),InitParams,openfluid::base::SimulationProfiler::INITPARAMS,IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::call_prepareData() const
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::call_prepareData()","Model not initialized");


  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(prepareData(),PrepareData,openfluid::base::SimulationProfiler::PREPAREDATA,IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::call_checkConsistency() const
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::call_checkConsistency()","Model not initialized");


  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(checkConsistency(),CheckConsistency,openfluid::base::SimulationProfiler::CHECKCONSISTENCY,IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::call_initializeRun(const openfluid::base::SimulationInfo* SimInfo) const
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::call_initializeRun()","Model not initialized");


  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(initializeRun(SimInfo),InitializeRun,openfluid::base::SimulationProfiler::INITIALIZERUN,IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::call_runStep(const openfluid::base::SimulationStatus* SimStatus) const
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::call_runStep()","Model not initialized");


  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(runStep(SimStatus),RunStep,openfluid::base::SimulationProfiler::RUNSTEP,IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::call_finalizeRun(const openfluid::base::SimulationInfo* SimInfo) const
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::call_finalizeRun()","Model not initialized");


  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(finalizeRun(SimInfo),FinalizeRun,openfluid::base::SimulationProfiler::FINALIZERUN,IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================



} } //namespaces

