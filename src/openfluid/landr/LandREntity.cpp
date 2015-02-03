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
  @file LandREntity.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
 */

#include "LandREntity.hpp"

#include <openfluid/core/Value.hpp>
#include <geos/geom/Geometry.h>
#include <geos/geom/Point.h>
#include <algorithm>

namespace openfluid { namespace landr {


// =====================================================================
// =====================================================================


LandREntity::LandREntity(const geos::geom::Geometry* Geom, unsigned int OfldId) :
    mp_Geom(Geom), m_OfldId(OfldId), mp_Neighbours(0)
{
  mp_Centroid = mp_Geom->getCentroid();
  m_Area = mp_Geom->getArea();
  m_Length = mp_Geom->getLength();
}


// =====================================================================
// =====================================================================


LandREntity::~LandREntity()
{
  delete mp_Centroid;
  delete mp_Geom;
  delete mp_Neighbours;

  std::map<std::string, core::Value*>::iterator it = m_Attributes.begin();
  std::map<std::string, core::Value*>::iterator ite = m_Attributes.end();
  for (; it != ite; ++it)
    delete it->second;
}


// =====================================================================
// =====================================================================


const geos::geom::Geometry* LandREntity::geometry()
{
  return mp_Geom;
}


// =====================================================================
// =====================================================================


unsigned int LandREntity::getOfldId() const
{
  return m_OfldId;
}


// =====================================================================
// =====================================================================


geos::geom::Point* LandREntity::centroid() const
{
  return mp_Centroid;
}


// =====================================================================
// =====================================================================


double LandREntity::getArea() const
{
  return m_Area;
}


// =====================================================================
// =====================================================================


double LandREntity::getLength() const
{
  return m_Length;
}


// =====================================================================
// =====================================================================


std::set<LandREntity*>* LandREntity::neighbours()
{
  if (!mp_Neighbours)
    computeNeighbours();

  return mp_Neighbours;
}


// =====================================================================
// =====================================================================


bool LandREntity::getAttributeValue(const std::string& AttributeName,
                                    core::Value& Value) const
{
  std::map<std::string, core::Value*>::const_iterator it = m_Attributes.find(
      AttributeName);

  if (it != m_Attributes.end() && it->second)
  {
    Value = *it->second;
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool LandREntity::setAttributeValue(const std::string& AttributeName,
                                    const core::Value* Value)
{
  std::map<std::string, core::Value*>::const_iterator it = m_Attributes.find(
      AttributeName);

  if (it != m_Attributes.end())
  {
    if (it->second)
      delete it->second;
    m_Attributes[AttributeName] = const_cast<core::Value*>(Value);
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


double LandREntity::getDistCentroCentro(LandREntity& Other)
{
  return mp_Centroid->distance(Other.centroid());
}


// =====================================================================
// =====================================================================


LandREntity* LandREntity::neighbour_MinDistCentroCentro()
{
  std::set<LandREntity*> Neigh = *neighbours();

  std::map<double, LandREntity*> NeighByDist;

  std::set<LandREntity*>::iterator it = Neigh.begin();
  std::set<LandREntity*>::iterator ite = Neigh.end();
  for (; it != ite; ++it)
  {
    LandREntity* Neigh = *it;

    double Dist = getDistCentroCentro(*Neigh);

    NeighByDist[Dist] = Neigh;
  }

  std::map<double, LandREntity*>::iterator itMin = std::min_element(
      NeighByDist.begin(), NeighByDist.end());

  return
      (itMin != NeighByDist.end() && itMin->first > 0) ? itMin->second :
                                                         (LandREntity*) 0;
}


// =====================================================================
// =====================================================================


} }// namespaces landr, openfluid
