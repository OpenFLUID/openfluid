/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <iostream>

#include "Function.h"


// =====================================================================
// =====================================================================


#define CHECK_VAR(name,objects,objshashtype,vars,status) \
    objshashtype::iterator it;\
    it = objects->begin(); \
    while (status && (it != objects->end()))\
    {\
      status = (!(it->second->vars->find(name) == it->second->vars->end())); \
      ++it; \
    }\


#define CREATE_VAR(name,objects,objshashtype,vars,varshashtype,status) \
    objshashtype::iterator it;\
    it = objects->begin(); \
    while (status && (it != objects->end()))\
    {\
      status = (it->second->vars->find(name) == it->second->vars->end()); \
      ++it; \
    }\
    if (status) \
    {\
      for(it = objects->begin(); it != objects->end(); ++it ) \
      { \
        it->second->vars->insert(varshashtype::value_type(name,new std::vector<double>)); \
      } \
    }

#define UPDATE_VAR(name,objects,objshashtype,vars,varshashtype,status) \
    objshashtype::iterator it;\
    it = objects->begin(); \
    while (status && (it != objects->end()))\
    {\
      if (it->second->vars->find(name) == it->second->vars->end()) \
      {\
        it->second->vars->insert(varshashtype::value_type(name,new std::vector<double>)); \
      }\
      ++it; \
    }\


// =====================================================================
// =====================================================================




namespace mhydasdk { namespace base {



Function::Function(mhydasdk::core::CoreRepository* CoreData)
        : ComputationBlock(CoreData)
{

  m_SUVarsToCreate.Clear();
  m_SUVarsToUpdate.Clear();
  m_SUVarsToCheck.Clear();
  m_SUInicondsToCheck.Clear();
  m_SUPropsToCheck.Clear();

  m_RSVarsToCreate.Clear();
  m_SUVarsToUpdate.Clear();
  m_RSVarsToCheck.Clear();
  m_RSInicondsToCheck.Clear();
  m_RSPropsToCheck.Clear();

  m_GUVarsToCreate.Clear();
  m_GUVarsToUpdate.Clear();
  m_GUVarsToCheck.Clear();
  m_GUInicondsToCheck.Clear();
  m_GUPropsToCheck.Clear();

}


// =====================================================================
// =====================================================================


bool Function::prepareData()
{
  int i;
  bool IsOK = true;

  // ============ simulated vars to create ===============

  // create SUs vars
  i = 0;
  while (IsOK && i<m_SUVarsToCreate.GetCount())
  {
    CREATE_VAR(m_SUVarsToCreate[i],
               mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
               getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,IsOK);
    i++;
  }


  // create RSs vars
  i = 0;
  while (IsOK && i<m_RSVarsToCreate.GetCount())
  {
    CREATE_VAR(m_RSVarsToCreate[i],
               mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
               getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,IsOK);
    i++;
  }


  // create GUs vars
  i = 0;
  while (IsOK && i<m_GUVarsToCreate.GetCount())
  {
    CREATE_VAR(m_GUVarsToCreate[i],
               mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
               getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,IsOK);
    i++;
  }


  // ============ simulated vars to update ===============

  // update SUs vars
  i = 0;
  while (IsOK && i<m_SUVarsToUpdate.GetCount())
  {
    UPDATE_VAR(m_SUVarsToUpdate[i],
               mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
               getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,IsOK);
    i++;
  }


  // update RSs vars
  i = 0;
  while (IsOK && i<m_RSVarsToUpdate.GetCount())
  {
    UPDATE_VAR(m_RSVarsToUpdate[i],
               mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
               getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,IsOK);
    i++;
  }


  // update GUs vars
  i = 0;
  while (IsOK && i<m_GUVarsToUpdate.GetCount())
  {
    UPDATE_VAR(m_GUVarsToUpdate[i],
               mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
               getSimulatedVars(),mhydasdk::core::SimulatedVarsMap,IsOK);
    i++;
  }


  return IsOK;
}




// =====================================================================
// =====================================================================

bool Function::checkConsistency()
{
  int i;
  bool IsOK = true;


  // ============= Simulated vars =============

  // check SUs vars
  i = 0;
  while (IsOK && i<m_SUVarsToCheck.GetCount())
  {
    CHECK_VAR(m_SUVarsToCheck[i],
              mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
              getSimulatedVars(),IsOK);
    i++;
  }

  // check RSs vars
  i = 0;
  while (IsOK && i<m_RSVarsToCheck.GetCount())
  {
    CHECK_VAR(m_RSVarsToCheck[i],
              mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
              getSimulatedVars(),IsOK);
    i++;
  }

  // check GUs vars
  i = 0;
  while (IsOK && i<m_GUVarsToCheck.GetCount())
  {
    CHECK_VAR(m_GUVarsToCheck[i],
              mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
              getSimulatedVars(),IsOK);
    i++;
  }


  // ============= Simulated properties =============

  // check SUs vars
  i = 0;
  while (IsOK && i<m_SUPropsToCheck.GetCount())
  {
    CHECK_VAR(m_SUPropsToCheck[i],
              mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
              getProperties(),IsOK);
    i++;
  }

  // check RSs vars
  i = 0;
  while (IsOK && i<m_RSPropsToCheck.GetCount())
  {
    CHECK_VAR(m_RSPropsToCheck[i],
              mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
              getProperties(),IsOK);
    i++;
  }

  // check GUs vars
  i = 0;
  while (IsOK && i<m_GUPropsToCheck.GetCount())
  {
    CHECK_VAR(m_GUPropsToCheck[i],
              mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
              getProperties(),IsOK);
    i++;
  }

  // ============= initial conditions =============

  // check SUs vars
  i = 0;
  while (IsOK && i<m_SUInicondsToCheck.GetCount())
  {
    CHECK_VAR(m_SUInicondsToCheck[i],
              mp_CoreData->getSpatialData()->getSUsCollection(),mhydasdk::core::SUMap,
              getIniConditions(),IsOK);
    i++;
  }

  // check RSs vars
  i = 0;
  while (IsOK && i<m_RSInicondsToCheck.GetCount())
  {
    CHECK_VAR(m_RSInicondsToCheck[i],
              mp_CoreData->getSpatialData()->getRSsCollection(),mhydasdk::core::RSMap,
              getIniConditions(),IsOK);
    i++;
  }

  // check GUs vars
  i = 0;
  while (IsOK && i<m_GUInicondsToCheck.GetCount())
  {
    CHECK_VAR(m_GUInicondsToCheck[i],
              mp_CoreData->getSpatialData()->getGUsCollection(),mhydasdk::core::GUMap,
              getIniConditions(),IsOK);
    i++;
  }

  return IsOK;
}



} } // namespace mhydasdk::base




