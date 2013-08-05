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
 \file LandREntity.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "LandREntity.hpp"

#include <openfluid/core/Value.hpp>
#include <geos/geom/Geometry.h>
#include <geos/geom/Point.h>
#include <algorithm>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

LandREntity::LandREntity(const geos::geom::Geometry* Geom, unsigned int SelfId) :
    mp_Geom(Geom), m_SelfId(SelfId), mp_Neighbours(0)
{
  mp_Centroid = mp_Geom->getCentroid();
  m_Area = mp_Geom->getArea();
  m_Lenght = mp_Geom->getLength();
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

const geos::geom::Geometry* LandREntity::getGeometry()
{
  return mp_Geom;
}

// =====================================================================
// =====================================================================

unsigned int LandREntity::getSelfId() const
{
  return m_SelfId;
}

// =====================================================================
// =====================================================================

geos::geom::Point* LandREntity::getCentroid() const
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
  return m_Lenght;
}

// =====================================================================
// =====================================================================

std::set<LandREntity*>* LandREntity::getNeighbours()
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
  return mp_Centroid->distance(Other.getCentroid());
}

// =====================================================================
// =====================================================================

LandREntity* LandREntity::getNeighbour_MinDistCentroCentro()
{
  std::set<LandREntity*> Neigh = *getNeighbours();

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

}// namespace landr
} /* namespace openfluid */
