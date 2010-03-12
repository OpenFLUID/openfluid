/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ModelInstance.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "ModelInstance.h"



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
        if (RuntimeEnvironment::getInstance()->isVerboseRun()) \
        { \
          std::cout << std::endl << std::setw(50) << _M_CurrentFunction->Signature->ID; \
          std::cout.flush(); \
        } \
        statevar = (statevar && _M_CurrentFunction->Function->calledmethod); \
        if (RuntimeEnvironment::getInstance()->isVerboseRun()) \
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
    if (RuntimeEnvironment::getInstance()->isVerboseRun()) std::cout << std::endl; std::cout.flush();


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
        if (RuntimeEnvironment::getInstance()->isVerboseRun()) \
        { \
          std::cout << std::endl << std::setw(50) << _M_CurrentFunction->Signature->ID; \
          std::cout.flush(); \
        } \
        statevar = (statevar && (_M_CurrentFunction->Function->calledmethod1 && _M_CurrentFunction->Function->calledmethod2)); \
        if (RuntimeEnvironment::getInstance()->isVerboseRun()) \
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
    if (RuntimeEnvironment::getInstance()->isVerboseRun()) std::cout << std::endl; std::cout.flush();



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


