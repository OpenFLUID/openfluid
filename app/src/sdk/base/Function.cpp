/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <iostream>

#include "Function.h"


// =====================================================================
// =====================================================================

#define CHECK_SIMULATION_VAR(name,collection,hashtype) \
    hashtype::iterator it;\
    for(it = collection->begin(); it != collection->end(); ++it )\
    {\
      if (it->second->getSimulatedVars()->find(name) == it->second->getSimulatedVars()->end())\
      return false;\
    }\
    return true;


#define ADD_SIMULATION_VAR(name,collection,hashtype) \
    if (checkSpatialSimulationVar(name,collection)) return false; \
    else \
    { \
      hashtype::iterator it; \
      for(it = collection->begin(); it != collection->end(); ++it ) \
      { \
        it->second->getSimulatedVars()->insert(mhydasdk::core::SimulatedVarsMap::value_type(name,new std::vector<double>)); \
      } \
    } \
    return true;


// =====================================================================
// =====================================================================




namespace mhydasdk { namespace base {



Function::Function(mhydasdk::core::CoreRepository* CoreData)
        : ComputationBlock(CoreData)
{

  m_SUVarsToCheck.Clear();
  m_SUVarsToAdd.Clear();

  m_RSVarsToCheck.Clear();
  m_RSVarsToAdd.Clear();

  m_GUVarsToCheck.Clear();
  m_GUVarsToAdd.Clear();


}

// =====================================================================
// =====================================================================

Function::~Function()
{

}




// =====================================================================
// =====================================================================

bool Function::addSpatialSimulationVar(wxString Name, mhydasdk::core::SUMap* SUsCollection)
{
  /*

  if (checkSpatialSimulationVar(Name,SUsCollection)) return false;
  else
  {
    mhydasdk::core::SUMap::iterator SUit;

    mhydasdk::core::SimulatedVarsMap VarsMap;

    for(SUit = SUsCollection->begin(); SUit != SUsCollection->end(); ++SUit )
    {

      SUit->second->getSimulatedVars()->insert(mhydasdk::core::SimulatedVarsMap::value_type(Name,new std::vector<double>));

    }

  }
  return true;

  */
  ADD_SIMULATION_VAR(Name,SUsCollection,mhydasdk::core::SUMap)

}


// =====================================================================
// =====================================================================


bool Function::addSpatialSimulationVar(wxString Name, mhydasdk::core::RSMap* RSsCollection)
{
  ADD_SIMULATION_VAR(Name,RSsCollection,mhydasdk::core::RSMap)
}



// =====================================================================
// =====================================================================


bool Function::addSpatialSimulationVar(wxString Name, mhydasdk::core::GUMap* GUsCollection)
{
  ADD_SIMULATION_VAR(Name,GUsCollection,mhydasdk::core::GUMap)
}




// =====================================================================
// =====================================================================


bool Function::checkSpatialSimulationVar(wxString Name, mhydasdk::core::SUMap* SUsCollection)
{

/*
  mhydasdk::core::SUMap::iterator SUit;

  for(SUit = SUsCollection->begin(); SUit != SUsCollection->end(); ++SUit )
  {
    if (SUit->second->getSimulatedVars()->find(Name) == SUit->second->getSimulatedVars()->end())
      return false;
  }

  return true;
  */

  CHECK_SIMULATION_VAR(Name,SUsCollection,mhydasdk::core::SUMap)
}


// =====================================================================
// =====================================================================


bool Function::checkSpatialSimulationVar(wxString Name, mhydasdk::core::RSMap* RSsCollection)
{

  CHECK_SIMULATION_VAR(Name,RSsCollection,mhydasdk::core::RSMap)
}



// =====================================================================
// =====================================================================



bool Function::checkSpatialSimulationVar(wxString Name, mhydasdk::core::GUMap* GUsCollection)
{
  CHECK_SIMULATION_VAR(Name,GUsCollection,mhydasdk::core::GUMap)
}




// =====================================================================
// =====================================================================




bool Function::initialize()
{
  int i;

  // check SUs vars
  for (i=0;i<m_SUVarsToCheck.GetCount();i++)
  {
    if (!checkSpatialSimulationVar(m_SUVarsToCheck[i],mp_CoreData->getSpatialData()->getSUsCollection())) return false;
  }

  // add SUs vars
  for (i=0;i<m_SUVarsToAdd.GetCount();i++)
  {
    if (!addSpatialSimulationVar(m_SUVarsToAdd[i],mp_CoreData->getSpatialData()->getSUsCollection())) return false;
  }


  // check RSs vars
  for (i=0;i<m_RSVarsToCheck.GetCount();i++)
  {
    if (!checkSpatialSimulationVar(m_RSVarsToCheck[i],mp_CoreData->getSpatialData()->getRSsCollection())) return false;
  }

  // add RSs vars
  for (i=0;i<m_RSVarsToAdd.GetCount();i++)
  {
    if (!addSpatialSimulationVar(m_RSVarsToAdd[i],mp_CoreData->getSpatialData()->getRSsCollection())) return false;
  }



  // check GUs vars
  for (i=0;i<m_GUVarsToCheck.GetCount();i++)
  {
    if (!checkSpatialSimulationVar(m_GUVarsToCheck[i],mp_CoreData->getSpatialData()->getGUsCollection())) return false;
  }

  // add GUs vars
  for (i=0;i<m_GUVarsToAdd.GetCount();i++)
  {
    if (!addSpatialSimulationVar(m_GUVarsToAdd[i],mp_CoreData->getSpatialData()->getGUsCollection())) return false;
  }



  return true;
}




} } // namespace mhydasdk::base




