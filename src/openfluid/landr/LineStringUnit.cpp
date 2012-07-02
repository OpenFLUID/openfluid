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
 \file LineStringUnit.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "LineStringUnit.hpp"

#include <geos/geom/LineString.h>
#include <geos/planargraph/DirectedEdge.h>
#include <geos/planargraph/Node.h>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

LineStringUnit::LineStringUnit(const geos::geom::LineString* NewLine,
                               OGRFeature* Feat) :
    geos::planargraph::Edge(), mp_Line(NewLine), mp_Feature(Feat)
{

}

// =====================================================================
// =====================================================================

LineStringUnit::LineStringUnit(const openfluid::landr::LineStringUnit& Other) :
    geos::planargraph::Edge()
{
  mp_Line = dynamic_cast<geos::geom::LineString*>(Other.getLine()->clone());
  mp_Feature =
      (const_cast<openfluid::landr::LineStringUnit&>(Other).getFeature())->Clone();
}

// =====================================================================
// =====================================================================

LineStringUnit::~LineStringUnit()
{
  OGRFeature::DestroyFeature(mp_Feature);
  delete mp_Line;
}

// =====================================================================
// =====================================================================

const geos::geom::LineString* LineStringUnit::getLine() const
{
  return mp_Line;
}

// =====================================================================
// =====================================================================

OGRFeature* LineStringUnit::getFeature()
{
  return mp_Feature;
}

// =====================================================================
// =====================================================================

int LineStringUnit::getSelfId()
{
  return mp_Feature->GetFieldAsInteger("SELF_ID");
}

// =====================================================================
// =====================================================================

geos::planargraph::Node* LineStringUnit::getStartNode()
{
  return getDirEdge(0)->getFromNode();
}

// =====================================================================
// =====================================================================

geos::planargraph::Node* LineStringUnit::getEndNode()
{
  return getDirEdge(0)->getToNode();
}

// =====================================================================
// =====================================================================

std::vector<openfluid::landr::LineStringUnit*> LineStringUnit::getUpNeighbours()
{
  std::vector<openfluid::landr::LineStringUnit*> UpNeighbours;

  geos::planargraph::DirectedEdgeStar* UpStar = getStartNode()->getOutEdges();

  geos::geom::Coordinate UpNodeCoo = getStartNode()->getCoordinate();

  for (std::vector<geos::planargraph::DirectedEdge*>::iterator it =
      UpStar->iterator(); it != UpStar->end(); ++it)
  {
    openfluid::landr::LineStringUnit* Unit =
        dynamic_cast<openfluid::landr::LineStringUnit*>((*it)->getEdge());

    if (Unit->getEndNode()->getCoordinate().equals(UpNodeCoo))
      UpNeighbours.push_back(Unit);
  }

  return UpNeighbours;
}

// =====================================================================
// =====================================================================

std::vector<openfluid::landr::LineStringUnit*> LineStringUnit::getDownNeighbours()
{
  std::vector<openfluid::landr::LineStringUnit*> DownNeighbours;

  geos::planargraph::DirectedEdgeStar* DownStar = getEndNode()->getOutEdges();

  geos::geom::Coordinate DownNodeCoo = getEndNode()->getCoordinate();

  for (std::vector<geos::planargraph::DirectedEdge*>::iterator it =
      DownStar->iterator(); it != DownStar->end(); ++it)
  {
    openfluid::landr::LineStringUnit* Unit =
        dynamic_cast<openfluid::landr::LineStringUnit*>((*it)->getEdge());

    if (Unit->getStartNode()->getCoordinate().equals(DownNodeCoo))
      DownNeighbours.push_back(Unit);
  }

  return DownNeighbours;
}

// =====================================================================
// =====================================================================

}// namespace landr
} /* namespace openfluid */
