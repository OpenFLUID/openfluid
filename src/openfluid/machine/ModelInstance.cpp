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

namespace openfluid { namespace machine {


#define DECLARE_FUNCTION_PARSER \
    std::list<ModelItemInstance*>::const_iterator _M_FuncIter; \

/**
  Macro for parsing the functions list and calling the given method of each function of the list
  @param[in] calledmethod the method to call on each function
  @param[out] statevar the globalized return of the method calls
 */
#define PARSE_FUNCTION_LIST(calledmethod,listenermethod,statevar) \
    _M_FuncIter = m_ModelItems.begin(); \
    while (_M_FuncIter != m_ModelItems.end() && statevar) \
    { \
      ModelItemInstance* _M_CurrentFunction = *_M_FuncIter; \
      if (_M_CurrentFunction != NULL) \
      { \
        mp_Listener->onFunction##listenermethod(_M_CurrentFunction->Signature->ID); \
        statevar = (statevar && _M_CurrentFunction->Function->calledmethod); \
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


/**
  Macro for parsing the functions list and calling the given two methods of each function of the list
  @param[in] calledmethod1 the first method to call on each function
  @param[in] calledmethod2 the first method to call on each function
  @param[out] statevar the globalized return of the method calls
 */

#define PARSE_FUNCTION_LIST_TWO(calledmethod1,calledmethod2,listenermethod,statevar) \
    _M_FuncIter = m_ModelItems.begin(); \
    while (_M_FuncIter != m_ModelItems.end() && statevar) \
    { \
      ModelItemInstance* _M_CurrentFunction = *_M_FuncIter; \
      if (_M_CurrentFunction != NULL) \
      { \
        mp_Listener->onFunction##listenermethod(_M_CurrentFunction->Signature->ID); \
        statevar = (statevar && (_M_CurrentFunction->Function->calledmethod1 && _M_CurrentFunction->Function->calledmethod2)); \
        if (!statevar) \
        { \
          mp_Listener->onFunction##listenermethod##Done(openfluid::machine::MachineListener::ERROR,_M_CurrentFunction->Signature->ID); \
          throw openfluid::base::OFException("OpenFLUID framework","Bad return value while calling function method"); \
        }  \
        else \
        { \
          if (openfluid::base::ExecutionMessages::getInstance()->isWarningFlag()) mp_Listener->onFunction##listenermethod##Done(openfluid::machine::MachineListener::WARNING,_M_CurrentFunction->Signature->ID); \
          else mp_Listener->onFunction##listenermethod##Done(openfluid::machine::MachineListener::OK,_M_CurrentFunction->Signature->ID); \
          openfluid::base::ExecutionMessages::getInstance()->resetWarningFlag(); \
        } \
      } \
      _M_FuncIter++; \
    } \


// =====================================================================
// =====================================================================



ModelInstance::ModelInstance(openfluid::machine::SimulationBlob& SimulationBlob,
                             openfluid::machine::MachineListener* Listener)
             : m_SimulationBlob(SimulationBlob)
{
  mp_Listener = Listener;
  if (mp_Listener == NULL) mp_Listener = new openfluid::machine::MachineListener();
}


// =====================================================================
// =====================================================================


ModelInstance::~ModelInstance()
{

}


// =====================================================================
// =====================================================================


void ModelInstance::appendItem(ModelItemInstance* ItemInstance)
{
  m_ModelItems.push_back(ItemInstance);
}


// =====================================================================
// =====================================================================


void ModelInstance::insertItem(ModelItemInstance* ItemInstance, unsigned int Position)
{
  if (Position < m_ModelItems.size())
  {
    std::list<ModelItemInstance*>::iterator it = m_ModelItems.begin();
    for (unsigned int i = 0; i< Position; i++) ++it;
    m_ModelItems.insert(it,ItemInstance);
  }
  else
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::insertItem","Bad index of item to insert");

}


// =====================================================================
// =====================================================================


void ModelInstance::deleteItem(unsigned int Position)
{
  if (Position < m_ModelItems.size())
  {
    std::list<ModelItemInstance*>::iterator it = m_ModelItems.begin();
    for (unsigned int i = 0; i< Position; i++) ++it;
    m_ModelItems.erase(it);
  }
  else
    throw openfluid::base::OFException("OpenFLUID framework","ModelInstance::deleteItem","Bad index of item to delete");
}


// =====================================================================
// =====================================================================


void ModelInstance::clear()
{
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


void ModelInstance::initialize() const
{
  std::list<ModelItemInstance*>::const_iterator FuncIter;

  FuncIter = m_ModelItems.begin();
  while (FuncIter != m_ModelItems.end())
  {
    (*FuncIter)->Function->initializeFunction();
    FuncIter++;
  }
}



// =====================================================================
// =====================================================================

bool ModelInstance::call_initParams() const
{
  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(initParams(_M_CurrentFunction->Params),InitParams, IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::call_prepareData() const
{
  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(prepareData(),PrepareData,IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::call_checkConsistency() const
{
  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(checkConsistency(),CheckConsistency,IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::call_initializeRun(const openfluid::base::SimulationInfo* SimInfo) const
{
  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(initializeRun(SimInfo),InitializeRun, IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::call_runStep(const openfluid::base::SimulationStatus* SimStatus) const
{
  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(runStep(SimStatus),RunStep,  IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::call_finalizeRun(const openfluid::base::SimulationInfo* SimInfo) const
{
  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(finalizeRun(SimInfo),FinalizeRun,IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================



} } //namespaces

