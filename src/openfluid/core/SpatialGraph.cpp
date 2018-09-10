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
  @file SpatialGraph.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/core/SpatialGraph.hpp>


namespace openfluid { namespace core {


// =====================================================================
// =====================================================================


struct SortUnitsPtrByProcessOrder
{
  bool operator ()(SpatialUnit*& U1,SpatialUnit*& U2) const
  {
    return (U1->getProcessOrder() <= U2->getProcessOrder());
  }

};


// =====================================================================
// =====================================================================


SpatialGraph::SpatialGraph()
{

}


// =====================================================================
// =====================================================================


bool SpatialGraph::removeUnitFromList(UnitsPtrList_t* UnitsList,
                                        const UnitID_t& UnitID)
{

  UnitsPtrList_t::iterator UnitsIt;
  bool Found = false;

  UnitsIt = UnitsList->begin();
  while (!Found && UnitsIt!=UnitsList->end())
  {
    Found = ((*UnitsIt)->getID() == UnitID);

    if (Found)
      UnitsList->erase(UnitsIt);
    else
      ++UnitsIt;
  }

  return Found;
}


// =====================================================================
// =====================================================================


bool SpatialGraph::addUnit(const SpatialUnit& aUnit)
{
  SpatialUnit* TheUnit = m_PcsOrderedUnitsByClass[aUnit.getClass()].addSpatialUnit(aUnit);

  if (TheUnit != nullptr)
  {
    m_PcsOrderedUnitsGlobal.push_back(TheUnit);
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool SpatialGraph::deleteUnit(SpatialUnit* aUnit)
{

  std::vector<openfluid::core::UnitsClass_t> ClassVector;


  openfluid::core::UnitsListByClassMap_t::const_iterator itUnitsClass;

  for (itUnitsClass=m_PcsOrderedUnitsByClass.begin();itUnitsClass!=m_PcsOrderedUnitsByClass.end();++itUnitsClass)
    ClassVector.push_back((*itUnitsClass).first);


  // remove all connections

  UnitsPtrList_t* UnitsList;

  for (unsigned int i = 0; i < ClassVector.size(); i++)
  {
    // from
    UnitsList = aUnit->fromSpatialUnits(ClassVector[i]);
    while (UnitsList !=nullptr && !UnitsList->empty())
      removeFromToConnection(UnitsList->front(),aUnit);

    // to
    UnitsList = aUnit->toSpatialUnits(ClassVector[i]);
    while (UnitsList !=nullptr && !UnitsList->empty())
      removeFromToConnection(aUnit, UnitsList->front());

    // children
    UnitsList = aUnit->childSpatialUnits(ClassVector[i]);
    while (UnitsList !=nullptr && !UnitsList->empty())
      removeChildParentConnection(UnitsList->front(),aUnit);

    // parent
    UnitsList = aUnit->parentSpatialUnits(ClassVector[i]);
    while (UnitsList !=nullptr && !UnitsList->empty())
      removeChildParentConnection(aUnit, UnitsList->front());

  }


  // remove unit pointer from the list global list

  UnitsPtrList_t::iterator UnitsIt;
  bool Found = false;

  UnitsIt = m_PcsOrderedUnitsGlobal.begin();
  while (!Found && UnitsIt!=m_PcsOrderedUnitsGlobal.end())
  {
    Found = ((*UnitsIt)->getID() == aUnit->getID() && (*UnitsIt)->getClass() == aUnit->getClass());

    if (Found)
      m_PcsOrderedUnitsGlobal.erase(UnitsIt);
    else
      ++UnitsIt;
  }


  // remove unit object from the "by class" list

  UnitsListByClassMap_t::iterator it;
  Found = false;

  it = m_PcsOrderedUnitsByClass.find(aUnit->getClass());

  if (it != m_PcsOrderedUnitsByClass.end())
  {
    UnitsList_t* UnitsBase = it->second.list();
    UnitsList_t::iterator UnitsBaseIt;

    UnitsBaseIt = UnitsBase->begin();
    while (!Found && UnitsBaseIt!=UnitsBase->end())
    {
      Found = ((*UnitsBaseIt).getID() == aUnit->getID() && (*UnitsBaseIt).getClass() == aUnit->getClass());

      if (Found)
        UnitsBase->erase(UnitsBaseIt);
      else
        ++UnitsBaseIt;
    }
  }


  return Found;
}


// =====================================================================
// =====================================================================


bool SpatialGraph::removeFromToConnection(SpatialUnit* FromUnit,
                                          SpatialUnit* ToUnit)
{
  if (FromUnit != nullptr && ToUnit != nullptr)
  {
    return (removeUnitFromList(FromUnit->toSpatialUnits(ToUnit->getClass()),ToUnit->getID()) &&
            removeUnitFromList(ToUnit->fromSpatialUnits(FromUnit->getClass()),FromUnit->getID()));
  }
  else
    return false;
}


// =====================================================================
// =====================================================================


bool SpatialGraph::removeChildParentConnection(SpatialUnit* ChildUnit,
                                               SpatialUnit* ParentUnit)
{
  if (ChildUnit != nullptr && ParentUnit != nullptr)
  {
    return (removeUnitFromList(ChildUnit->parentSpatialUnits(ParentUnit->getClass()),ParentUnit->getID()) &&
            removeUnitFromList(ParentUnit->childSpatialUnits(ChildUnit->getClass()),ChildUnit->getID()));
  }
  else
    return false;
}


// =====================================================================
// =====================================================================


bool SpatialGraph::isUnitsClassExist(const UnitsClass_t& UnitsClass) const
{
  return m_PcsOrderedUnitsByClass.find(UnitsClass) != m_PcsOrderedUnitsByClass.end();
}


// =====================================================================
// =====================================================================


SpatialUnit* SpatialGraph::spatialUnit(const UnitsClass_t& UnitsClass, UnitID_t UnitID)
{
  UnitsListByClassMap_t::iterator it;

  it = m_PcsOrderedUnitsByClass.find(UnitsClass);

  if (it != m_PcsOrderedUnitsByClass.end())
  {
    return (it->second.spatialUnit(UnitID));
  }

  return nullptr;
}


// =====================================================================
// =====================================================================


const SpatialUnit* SpatialGraph::spatialUnit(const UnitsClass_t& UnitsClass, UnitID_t UnitID) const
{
  UnitsListByClassMap_t::const_iterator it;

  it = m_PcsOrderedUnitsByClass.find(UnitsClass);

  if (it != m_PcsOrderedUnitsByClass.end())
  {
    return (it->second.spatialUnit(UnitID));
  }

  return nullptr;
}


// =====================================================================
// =====================================================================


UnitsCollection* SpatialGraph::spatialUnits(const UnitsClass_t& UnitsClass)
{
  UnitsListByClassMap_t::iterator it;

  it = m_PcsOrderedUnitsByClass.find(UnitsClass);

  if (it == m_PcsOrderedUnitsByClass.end())
    return nullptr;

  return &(it->second);

}


// =====================================================================
// =====================================================================


const UnitsCollection* SpatialGraph::spatialUnits(const UnitsClass_t& UnitsClass) const
{
  UnitsListByClassMap_t::const_iterator it;

  it = m_PcsOrderedUnitsByClass.find(UnitsClass);

  if (it == m_PcsOrderedUnitsByClass.end())
    return nullptr;

  return  &(it->second);

}


// =====================================================================
// =====================================================================


bool SpatialGraph::sortUnitsByProcessOrder()
{
  UnitsListByClassMap_t::iterator it;
  UnitsCollection* Units;

  // sort primary units structures
  for (it = m_PcsOrderedUnitsByClass.begin();it != m_PcsOrderedUnitsByClass.end();++it)
  {
    Units = &(it->second);
    Units->sortByProcessOrder();
  }

  // sort global units structure
  m_PcsOrderedUnitsGlobal.sort(SortUnitsPtrByProcessOrder());

  return true;
}


// =====================================================================
// =====================================================================


void SpatialGraph::streamContents(std::ostream& OStream)
{
  UnitsListByClassMap_t::iterator ClassIt;

  UnitsList_t* Units;
  UnitsList_t::iterator UnitIt;


  if (m_PcsOrderedUnitsByClass.empty())
  {
    OStream << "No unit" << std::endl;
    return;
  }

  for (ClassIt = m_PcsOrderedUnitsByClass.begin();ClassIt != m_PcsOrderedUnitsByClass.end();++ClassIt)
  {
    OStream << "** Units class : " << ClassIt->first << " **" << std::endl;


    Units = (ClassIt->second).list();

    for (UnitIt = Units->begin();UnitIt != Units->end();++UnitIt)
    {
      UnitIt->streamContents(OStream);
    }


  }
  OStream << std::endl;
}


// =====================================================================
// =====================================================================


void SpatialGraph::clearAllVariables()
{
  for (openfluid::core::SpatialUnit* CurrentUnit : m_PcsOrderedUnitsGlobal)
    CurrentUnit->variables()->clear();
}


// =====================================================================
// =====================================================================


void SpatialGraph::clearAllAttributes()
{
  for (openfluid::core::SpatialUnit* CurrentUnit : m_PcsOrderedUnitsGlobal)
    CurrentUnit->attributes()->clear();
}


// =====================================================================
// =====================================================================


void SpatialGraph::clearAllEvents()
{
  for (openfluid::core::SpatialUnit* CurrentUnit : m_PcsOrderedUnitsGlobal)
    CurrentUnit->events()->clear();
}


// =====================================================================
// =====================================================================


void SpatialGraph::clearAllData()
{
  for (openfluid::core::SpatialUnit* CurrentUnit : m_PcsOrderedUnitsGlobal)
  {
    CurrentUnit->variables()->clear();
    CurrentUnit->attributes()->clear();
    CurrentUnit->events()->clear();
  }
}


// =====================================================================
// =====================================================================


void SpatialGraph::clearUnits()
{
  UnitsPtrList_t::iterator UnitPtrIt = m_PcsOrderedUnitsGlobal.begin();

  while (UnitPtrIt != m_PcsOrderedUnitsGlobal.end())
  {
    deleteUnit(*(UnitPtrIt++));
  }
}


} } // namespaces

