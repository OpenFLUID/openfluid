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
  @file SpatialUnit.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <ogr_geometry.h>

#include <openfluid/core/SpatialUnit.hpp>


namespace openfluid { namespace core {


SpatialUnit::SpatialUnit(const UnitsClass_t& aClass, const UnitID_t anID,
                         const PcsOrd_t aPcsOrder) :
  m_ID(anID), m_Class(aClass), m_PcsOrder(aPcsOrder), m_Geometry(nullptr)
{

}


// =====================================================================
// =====================================================================


SpatialUnit::~SpatialUnit()
{

}


// =====================================================================
// =====================================================================


bool SpatialUnit::addToUnit(SpatialUnit* aUnit)
{
  m_ToUnits[aUnit->getClass()].push_back(aUnit);
  return true;
}


// =====================================================================
// =====================================================================


bool SpatialUnit::addFromUnit(SpatialUnit* aUnit)
{
  m_FromUnits[aUnit->getClass()].push_back(aUnit);
  return true;

}


// =====================================================================
// =====================================================================


bool SpatialUnit::addParentUnit(SpatialUnit* aUnit)
{
  m_ParentUnits[aUnit->getClass()].push_back(aUnit);
  return true;
}


// =====================================================================
// =====================================================================


bool SpatialUnit::addChildUnit(SpatialUnit* aUnit)
{
  m_ChildrenUnits[aUnit->getClass()].push_back(aUnit);
  return true;

}


// =====================================================================
// =====================================================================


const UnitsPtrList_t* SpatialUnit::toSpatialUnits(const UnitsClass_t& aClass) const
{
  return const_cast<UnitsPtrList_t*>(toSpatialUnits(aClass));

}


// =====================================================================
// =====================================================================


const UnitsPtrList_t* SpatialUnit::childSpatialUnits(const UnitsClass_t& aClass) const
{
  return const_cast<UnitsPtrList_t*>(childSpatialUnits(aClass));
}


// =====================================================================
// =====================================================================


const UnitsPtrList_t* SpatialUnit::parentSpatialUnits(const UnitsClass_t& aClass) const
{
  return const_cast<UnitsPtrList_t*>(parentSpatialUnits(aClass));

}


// =====================================================================
// =====================================================================


const UnitsPtrList_t* SpatialUnit::fromSpatialUnits(const UnitsClass_t& aClass) const
{
  return const_cast<UnitsPtrList_t*>(fromSpatialUnits(aClass));
}


// =====================================================================
// =====================================================================


UnitsPtrList_t* SpatialUnit::toSpatialUnits(const UnitsClass_t& aClass)
{
  LinkedUnitsListByClassMap_t::iterator it = m_ToUnits.find(aClass);

  if (it != m_ToUnits.end())
    return &(it->second);
  else
    return nullptr;
}


// =====================================================================
// =====================================================================


UnitsPtrList_t* SpatialUnit::fromSpatialUnits(const UnitsClass_t& aClass)
{
  LinkedUnitsListByClassMap_t::iterator it = m_FromUnits.find(aClass);

  if (it != m_FromUnits.end())
    return &(it->second);
  else
    return nullptr;
}


// =====================================================================
// =====================================================================


UnitsPtrList_t* SpatialUnit::parentSpatialUnits(const UnitsClass_t& aClass)
{
  LinkedUnitsListByClassMap_t::iterator it = m_ParentUnits.find(aClass);

  if (it != m_ParentUnits.end())
    return &(it->second);
  else
    return nullptr;
}


// =====================================================================
// =====================================================================


UnitsPtrList_t* SpatialUnit::childSpatialUnits(const UnitsClass_t& aClass)
{
  LinkedUnitsListByClassMap_t::iterator it = m_ChildrenUnits.find(aClass);

  if (it != m_ChildrenUnits.end())
    return &(it->second);
  else
    return nullptr;
}


// =====================================================================
// =====================================================================


void SpatialUnit::streamContents(std::ostream& OStream)
{
  UnitsPtrList_t::iterator IDIt;
  LinkedUnitsListByClassMap_t::iterator ClassIt;
  UnitsPtrList_t UnitsList;

  OStream << m_Class << " #"<< m_ID << " (order: " << m_PcsOrder << ")";

  if (!m_ToUnits.empty())
  {
    OStream << " To[";

    for (ClassIt=m_ToUnits.begin();ClassIt!=m_ToUnits.end();++ClassIt)
    {

      UnitsList = ClassIt->second;

     for (IDIt=UnitsList.begin();IDIt!=UnitsList.end();++IDIt)
      {
       OStream << "(" << (*IDIt)->getClass() << "," <<  (*IDIt)->getID() << ") ";
      }

    }
    OStream << "] ";
  }

  if (!m_FromUnits.empty())
  {
    OStream << " From[";

    for (ClassIt=m_FromUnits.begin();ClassIt!=m_FromUnits.end();++ClassIt)
    {

      UnitsList = ClassIt->second;

     for (IDIt=UnitsList.begin();IDIt!=UnitsList.end();++IDIt)
      {
       OStream << "(" << (*IDIt)->getClass() << "," <<  (*IDIt)->getID() << ") ";
      }

    }
    OStream << "] ";
  }

  OStream << std::endl;
}


// =====================================================================
// =====================================================================


bool SpatialUnit::importGeometryFromWkt(const std::string& WKT)
{
  if (m_Geometry)
    deleteGeometry();

  char* WktBuffer = (char*)(WKT.c_str());

  // TODO set spatial reference (from name given as method argument?)
  OGRErr Error = OGRGeometryFactory::createFromWkt(&WktBuffer,nullptr,&m_Geometry);

  if (Error != OGRERR_NONE)
  {
    deleteGeometry();
    return false;
  }

  return true;
}


// =====================================================================
// =====================================================================


std::string SpatialUnit::exportGeometryToWkt() const
{
  if (!m_Geometry)
    return "";

  char* WktBuffer = nullptr;

  m_Geometry->exportToWkt(&WktBuffer);
  std::string WKT(WktBuffer);

  OGRFree(WktBuffer);

  return WKT;
}


// =====================================================================
// =====================================================================


void SpatialUnit::deleteGeometry()
{
  if (m_Geometry)
  {
    OGRGeometryFactory::destroyGeometry(m_Geometry);
    m_Geometry = nullptr;
  }
}


} } // namespaces
