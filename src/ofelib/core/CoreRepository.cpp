/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


#include "CoreRepository.h"


namespace openfluid { namespace core {



CoreRepository* CoreRepository::mp_Singleton = NULL;
unsigned int StepsReservation::Reserved = 0;
MemoryMonitor* MemoryMonitor::mp_Singleton = NULL;



// =====================================================================
// =====================================================================

CoreRepository::CoreRepository()
{
//  m_LastMemToDiskStep = -1;
}


// =====================================================================
// =====================================================================


CoreRepository* CoreRepository::getInstance()
{
  if (mp_Singleton == NULL) mp_Singleton = new CoreRepository();
  return mp_Singleton;
}




// =====================================================================
// =====================================================================


bool CoreRepository::addUnit(Unit aUnit)
{

  return m_PcsOrderedUnitsByClass[aUnit.getClass()].addUnit(aUnit);
}


// =====================================================================
// =====================================================================


bool CoreRepository::isUnitsClassExist(UnitClass_t UnitClass) const
{
  return m_PcsOrderedUnitsByClass.find(UnitClass) != m_PcsOrderedUnitsByClass.end();
}

// =====================================================================
// =====================================================================


Unit* CoreRepository::getUnit(UnitClass_t UnitClass, UnitID_t UnitID)
{
  UnitsListByClassMap_t::iterator it;

  it = m_PcsOrderedUnitsByClass.find(UnitClass);

  if (it != m_PcsOrderedUnitsByClass.end())
  {
    return (it->second.getUnit(UnitID));
  }

  return NULL;
}


// =====================================================================
// =====================================================================


UnitsCollection* CoreRepository::getUnits(UnitClass_t UnitClass)
{
  UnitsListByClassMap_t::iterator it;

  it = m_PcsOrderedUnitsByClass.find(UnitClass);

  if (it == m_PcsOrderedUnitsByClass.end()) return NULL;
  return  &(it->second);

}

// =====================================================================
// =====================================================================


const UnitsCollection* CoreRepository::getUnits(UnitClass_t UnitClass) const
{
  UnitsListByClassMap_t::const_iterator it;

  it = m_PcsOrderedUnitsByClass.find(UnitClass);

  if (it == m_PcsOrderedUnitsByClass.end()) return NULL;
  return  &(it->second);

}


// =====================================================================
// =====================================================================


bool CoreRepository::sortUnitsByProcessOrder()
{
  UnitsListByClassMap_t::iterator it;
  UnitsCollection* Units;


  for (it = m_PcsOrderedUnitsByClass.begin();it != m_PcsOrderedUnitsByClass.end();++it)
  {
    Units = &(it->second);
    Units->sortByProcessOrder();
  }

  return true;
}

// =====================================================================
// =====================================================================


void CoreRepository::printSTDOUT()
{
  UnitsListByClassMap_t::iterator ClassIt;

  UnitsList_t* Units;
  UnitsList_t::iterator UnitIt;


  if (m_PcsOrderedUnitsByClass.size() == 0)
  {
    std::cout << "No unit" << std::endl;
    return;
  }

  for (ClassIt = m_PcsOrderedUnitsByClass.begin();ClassIt != m_PcsOrderedUnitsByClass.end();++ClassIt)
  {
    std::cout << "** Units class : " << ClassIt->first << " **" << std::endl;


    Units = (ClassIt->second).getList();

    for (UnitIt = Units->begin();UnitIt != Units->end();++UnitIt)
    {
      UnitIt->printSTDOUT();
    }


  }
  std::cout << std::endl;
}


// =====================================================================
// =====================================================================



/*
bool CoreRepository::getMemReleaseRange(TimeStep_t LastStep, TimeStep_t* BeginStep, TimeStep_t* EndStep)
{
  *BeginStep = m_LastMemToDiskStep+1;
  *EndStep = m_LastMemToDiskStep+Packet;
  if (*EndStep > LastStep) *EndStep = LastStep;
  return true;
}
*/

// =====================================================================
// =====================================================================


bool CoreRepository::doMemRelease(TimeStep_t Step,bool WithoutKeep)
{
  if (mp_MemMonitor->isMemReleaseStep(Step))
  {
    releaseMemory(Step-mp_MemMonitor->getKeep());
//    m_LastMemToDiskStep = Step-Keep;
    return true;
  }
  else
  {
    if (WithoutKeep)
    {
      releaseMemory(Step);
//      m_LastMemToDiskStep = Step;
      return true;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


bool CoreRepository::releaseMemory(TimeStep_t Step)
{
//  std::cout << "Releasing memory from " << (mp_MemMonitor->getLastMemoryRelease() +1) << " to " << Step << std::endl;

  UnitsListByClassMap_t::iterator ClassIt;

  UnitsList_t* Units;
  UnitsList_t::iterator UnitIt;


  for (ClassIt = m_PcsOrderedUnitsByClass.begin();ClassIt != m_PcsOrderedUnitsByClass.end();++ClassIt)
  {

    Units = (ClassIt->second).getList();

    for (UnitIt = Units->begin();UnitIt != Units->end();++UnitIt)
    {
//      std::cout << UnitIt->getClass() << UnitIt->getID() << std::endl;std::cout.flush();
      UnitIt->getScalarVariables()->releaseMemory(Step);
      UnitIt->getVectorVariables()->releaseMemory(Step);
    }


  }


  return true;
}


} } // namespaces

