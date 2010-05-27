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
  \file ModelInstance.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/base/RuntimeEnv.hpp>

namespace openfluid { namespace machine {


#define DECLARE_FUNCTION_PARSER \
    std::list<ModelItemInstance*>::const_iterator _M_FuncIter;

/**
  Macro for parsing the functions list and calling the given method of each function of the list
  \param[in] calledmethod the method to call on each function
  \param[out] statevar the globalized return of the method calls
 */
#define PARSE_FUNCTION_LIST(calledmethod,statevar) \
    _M_FuncIter = m_ModelItems.begin(); \
    while (_M_FuncIter != m_ModelItems.end() && statevar) \
    { \
      ModelItemInstance* _M_CurrentFunction = *_M_FuncIter; \
      if (_M_CurrentFunction != NULL) \
      { \
        if (openfluid::base::RuntimeEnvironment::getInstance()->isVerboseRun()) \
        { \
          std::cout << std::endl << std::setw(50) << _M_CurrentFunction->Signature->ID; \
          std::cout.flush(); \
        } \
        statevar = (statevar && _M_CurrentFunction->Function->calledmethod); \
        if (openfluid::base::RuntimeEnvironment::getInstance()->isVerboseRun()) \
        { \
          if (!statevar) \
          { \
            std::cout << "  " << "[Error]";\
            throw openfluid::base::OFException("kernel","Bad return value while calling function method"); \
          }  \
          else \
          { \
            if (openfluid::base::ExecutionMessages::getInstance()->isWarningFlag()) std::cout << "  " << "[Warning]"; \
            else std::cout << "  " << "[OK]"; \
            openfluid::base::ExecutionMessages::getInstance()->resetWarningFlag(); \
          } \
          std::cout.flush(); \
        } \
      } \
      _M_FuncIter++; \
    } \
    if (openfluid::base::RuntimeEnvironment::getInstance()->isVerboseRun()) std::cout << std::endl; std::cout.flush();


/**
  Macro for parsing the functions list and calling the given two methods of each function of the list
  \param[in] calledmethod1 the first method to call on each function
  \param[in] calledmethod2 the first method to call on each function
  \param[out] statevar the globalized return of the method calls
 */

#define PARSE_FUNCTION_LIST_TWO(calledmethod1,calledmethod2,statevar) \
    _M_FuncIter = m_ModelItems.begin(); \
    while (_M_FuncIter != m_ModelItems.end() && statevar) \
    { \
      ModelItemInstance* _M_CurrentFunction = *_M_FuncIter; \
      if (_M_CurrentFunction != NULL) \
      { \
        if (openfluid::base::RuntimeEnvironment::getInstance()->isVerboseRun()) \
        { \
          std::cout << std::endl << std::setw(50) << _M_CurrentFunction->Signature->ID; \
          std::cout.flush(); \
        } \
        statevar = (statevar && (_M_CurrentFunction->Function->calledmethod1 && _M_CurrentFunction->Function->calledmethod2)); \
        if (openfluid::base::RuntimeEnvironment::getInstance()->isVerboseRun()) \
        { \
          if (!statevar) \
          { \
            std::cout << "  " << "[Error]";\
            throw openfluid::base::OFException("kernel","Bad return value while calling function method"); \
          }  \
          else \
          { \
            if (openfluid::base::ExecutionMessages::getInstance()->isWarningFlag()) std::cout << "  " << "[Warning]"; \
            else std::cout << "  " << "[OK]"; \
            openfluid::base::ExecutionMessages::getInstance()->resetWarningFlag(); \
          } \
          std::cout.flush(); \
        } \
      } \
      _M_FuncIter++; \
    } \
    if (openfluid::base::RuntimeEnvironment::getInstance()->isVerboseRun()) std::cout << std::endl; std::cout.flush();



// =====================================================================
// =====================================================================



ModelInstance::ModelInstance()
{

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


void ModelInstance::clear()
{
  m_ModelItems.clear();
}



// =====================================================================
// =====================================================================


void ModelInstance::deleteItemsAndClear()
{

  std::list<ModelItemInstance*>::iterator it;

  for (it=m_ModelItems.begin();it!=m_ModelItems.end();++it)
  {
    if ((*it)->Function != NULL) delete (*it)->Function;
    if ((*it)->Signature != NULL) delete (*it)->Signature;
  }

  clear();
}


// =====================================================================
// =====================================================================


bool ModelInstance::initParams() const
{
  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(initParams(_M_CurrentFunction->Params), IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::prepareDataAndCheckConsistency() const
{
  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST_TWO(prepareData(),checkConsistency(),IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::initializeRun(const openfluid::base::SimulationInfo* SimInfo) const
{
  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(initializeRun(SimInfo), IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::runStep(const openfluid::base::SimulationStatus* SimStatus) const
{
  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(runStep(SimStatus), IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================


bool ModelInstance::finalizeRun(const openfluid::base::SimulationInfo* SimInfo) const
{
  DECLARE_FUNCTION_PARSER;
  bool IsOK = true;

  PARSE_FUNCTION_LIST(finalizeRun(SimInfo), IsOK);

  return IsOK;
}


// =====================================================================
// =====================================================================



} } //namespaces

