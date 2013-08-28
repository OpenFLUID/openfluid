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
 \file Tools.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "LandRTools.hpp"

#include <openfluid/landr/GeosCompat.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/operation/linemerge/LineMerger.h>
#include <geos/operation/polygonize/Polygonizer.h>
#include <geos/operation/overlay/OverlayOp.h>
#include <geos/operation/overlay/snap/GeometrySnapper.h>
#include <geos/precision/CommonBitsRemover.h>
#include <geos/util/TopologyException.h>
#include <geos/planargraph/Node.h>
#include <geos/planargraph/DirectedEdge.h>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

geos::geom::LineString* LandRTools::getMergedLineStringFromGeometry(
    geos::geom::Geometry* Geom)
{
  geos::geom::LineString* LS = 0;

  std::vector<geos::geom::LineString*>* Lines =
      getMergedLineStringsFromGeometry(Geom);

  if (!Lines || Lines->size() != 1)
  {
    unsigned int iEnd=Lines->size();
    for (unsigned int i = 0; i < iEnd; i++)
      delete Lines->at(i);
  }
  else
    LS = *Lines->begin();

  delete Lines;

  return LS;
}

// =====================================================================
// =====================================================================

std::vector<geos::geom::LineString*>* LandRTools::getMergedLineStringsFromGeometry(
    geos::geom::Geometry* Geom)
{
  std::vector<geos::geom::LineString*>* LS = 0;

  geos::operation::linemerge::LineMerger Merger;

  switch (Geom->getGeometryTypeId())
  {
    case geos::geom::GEOS_LINESTRING:
      LS = new std::vector<geos::geom::LineString*>();
      LS->push_back(dynamic_cast<geos::geom::LineString*>(Geom->clone()));
      break;
    case geos::geom::GEOS_MULTILINESTRING:
    case geos::geom::GEOS_LINEARRING:
    case geos::geom::GEOS_GEOMETRYCOLLECTION:
      Merger.add(Geom);
      LS = Merger.getMergedLineStrings();
      break;
    default:
      break;
  }

  return LS;
}

// =====================================================================
// =====================================================================

std::vector<geos::geom::LineString*> LandRTools::getVectorOfExteriorRings(
    openfluid::landr::VectorDataset& Val)
{
  if (!Val.isPolygonType())
    throw openfluid::base::FrameworkException(
        "LandRTools::getVectorOfExteriorRings",
        " The GeoVectorValue is not polygon-typed.");

  std::vector<geos::geom::LineString*> Lines;

  geos::geom::Geometry* Geom = Val.getGeometries();

  unsigned int iEnd=Geom->getNumGeometries();
  for (unsigned int i = 0; i < iEnd; i++)
    Lines.push_back(
        const_cast<geos::geom::LineString*>(dynamic_cast<geos::geom::Polygon*>(const_cast<geos::geom::Geometry*>(Geom->getGeometryN(
            i)))->getExteriorRing()));

  return Lines;
}

// =====================================================================
// =====================================================================

std::vector<geos::geom::LineString*> LandRTools::getVectorOfLines(
    openfluid::landr::VectorDataset& Val)
{
  if (!Val.isLineType())
    throw openfluid::base::FrameworkException(
        "LandRTools::getVectorOfLines",
        " The GeoVectorValue is not linestring-typed.");

  std::vector<geos::geom::LineString*> Lines;

  geos::geom::Geometry* Geom = Val.getGeometries();

  unsigned int iEnd=Geom->getNumGeometries();
  for (unsigned int i = 0; i < iEnd; i++)
    Lines.push_back(
        dynamic_cast<geos::geom::LineString*>(const_cast<geos::geom::Geometry*>(Geom->getGeometryN(
            i))));

  return Lines;
}

// =====================================================================
// =====================================================================

std::vector<geos::geom::LineString*>* LandRTools::getNodedLines(
    geos::geom::Geometry* Geom1, geos::geom::Geometry* Geom2,
    double SnapTolerance)
{
  try
  {
    geos::geom::Geometry* UnionGeom = computeSnapOverlayUnion(*Geom1, *Geom2,
                                                              SnapTolerance);

    std::list<geos::geom::LineString*> ExistingLines;

    geos::operation::linemerge::LineMerger lm;

    unsigned int iEnd=UnionGeom->getNumGeometries();
    for (unsigned int i = 0; i < iEnd; i++)
    {
      geos::geom::LineString* Line =
          dynamic_cast<geos::geom::LineString*>(const_cast<geos::geom::Geometry*>(UnionGeom->getGeometryN(
              i)));

      Line->normalize();

      if (!exists(Line, ExistingLines, SnapTolerance))
      {
        ExistingLines.push_back(Line);
        lm.add(Line);
      }
    }

    return lm.getMergedLineStrings();

  }
  catch (geos::util::GEOSException& e)
  {
    throw openfluid::base::FrameworkException(
        "LandRTools::getNodedLines",
        "Error while noding lines, you can try again with a greater snap tolerance value.\n"
        "(Details: "
        + std::string(e.what()) + ")");
  }

  return (std::vector<geos::geom::LineString*>*) 0;
}

// =====================================================================
// =====================================================================

geos::geom::Geometry* LandRTools::computeSnapOverlayUnion(
    geos::geom::Geometry& Geom1, geos::geom::Geometry& Geom2,
    double SnapTolerance)
{
  GEOM_PTR_PAIR PrepGeom;
  GEOM_PTR_PAIR RemGeom;

  std::auto_ptr<geos::precision::CommonBitsRemover> cbr;
  cbr.reset(new geos::precision::CommonBitsRemover());
  cbr->add(&Geom1);
  cbr->add(&Geom2);
  RemGeom.first.reset(cbr->removeCommonBits(Geom1.clone()));
  RemGeom.second.reset(cbr->removeCommonBits(Geom2.clone()));

  geos::operation::overlay::snap::GeometrySnapper::snap(*RemGeom.first,
                                                        *RemGeom.second,
                                                        SnapTolerance,
                                                        PrepGeom);

  std::auto_ptr<geos::geom::Geometry> Result(
      geos::operation::overlay::OverlayOp::overlayOp(
          PrepGeom.first.get(), PrepGeom.second.get(),
          geos::operation::overlay::OverlayOp::opUNION));

  cbr->addCommonBits(&(*Result));

  return Result.release();
}

// =====================================================================
// =====================================================================

bool LandRTools::exists(geos::geom::LineString* Line,
                        std::list<geos::geom::LineString*> RefLines,
                        double Tolerance)
{
  std::list<geos::geom::LineString*>::iterator it = RefLines.begin();
  std::list<geos::geom::LineString*>::iterator ite = RefLines.end();
  for (; it != ite; ++it)
  {
    if ((*it)->equalsExact(Line, Tolerance))
      return true;
  }

  return false;
}

// =====================================================================
// =====================================================================

void LandRTools::polygonizeGeometry(
    std::vector<geos::geom::Geometry*>& Lines,
    std::vector<geos::geom::Polygon*>& Polygons,
    std::vector<const geos::geom::LineString*>& Dangles)
{
  geos::operation::polygonize::Polygonizer* P =
      new geos::operation::polygonize::Polygonizer();

  P->add(&Lines);

  // ! ask for Dangles BEFORE asking for polys (cf. Polygonizer code...)
  GET_DANGLES(P, Dangles);

  std::vector<geos::geom::Polygon*>* ThePolygons = P->getPolygons();
  if (ThePolygons)
    Polygons = *ThePolygons;

  delete P;
}

// =====================================================================
// =====================================================================

void LandRTools::markVisitedNodesUsingDFS(geos::planargraph::Node* Node )
{
  Node->setVisited(true);

  geos::planargraph::DirectedEdgeStar *DEdge=Node->getOutEdges();

  std::vector<geos::planargraph::DirectedEdge*>::iterator it=DEdge->begin();
  std::vector<geos::planargraph::DirectedEdge*>::iterator ite=DEdge->end();

  for (; it != ite; ++it)
  {
    if(!(*it)->getEdge()->isVisited())
    {
      geos::planargraph::Node * theNextNode=static_cast<openfluid::landr::LineStringEntity*>((*it)->getEdge())->getStartNode();

      if(Node->getCoordinate()==theNextNode->getCoordinate())
        theNextNode=static_cast<openfluid::landr::LineStringEntity*>((*it)->getEdge())->getEndNode();

      // set Edge visited as true
      (*it)->getEdge()->setVisited(true);

      if(!theNextNode->isVisited())
        markVisitedNodesUsingDFS(theNextNode);
    }
  }

}

// =====================================================================
// =====================================================================

std::vector<geos::geom::Polygon*> LandRTools::computeIntersectPolygons(
    geos::geom::Geometry* Geom1, geos::geom::Geometry* Geom2)
{
  try
  {
    std::vector<geos::geom::Polygon*> Polygons;

    unsigned int iEnd=Geom1->getNumGeometries();
    for (unsigned int i = 0; i < iEnd; i++)
    {
      if(Geom1->getGeometryN(i)->getGeometryTypeId()!=geos::geom::GEOS_POLYGON)
        throw openfluid::base::FrameworkException(
            "LandRTools::computeIntersectPolygons",
            " The Geometry is not Polygon-typed.");

      unsigned int jEnd=Geom2->getNumGeometries();
      for (unsigned int j = 0; j < jEnd; j++)
      {
        if(Geom2->getGeometryN(j)->getGeometryTypeId()!=geos::geom::GEOS_POLYGON)
          throw openfluid::base::FrameworkException(
              "LandRTools::computeIntersectPolygons",
              " The Geometry is not Polygon-typed.");
        if((Geom1->getGeometryN(i))->intersects(const_cast<geos::geom::Geometry*>(Geom2->getGeometryN(j))))
        {
          geos::geom::Geometry *Intersect=(Geom1->getGeometryN(i))->intersection(const_cast<geos::geom::Geometry*>(Geom2->getGeometryN(j)));
          Polygons.push_back(dynamic_cast<geos::geom::Polygon*>(Intersect));

        }
      }
    }
    return Polygons;
  }
  catch (geos::util::GEOSException& e)
  {
    throw openfluid::base::FrameworkException(
        "LandRTools::computeIntersectPolygons",
        "Error while Intersecting Polygons.\n"
        "(Details: "
        + std::string(e.what()) + ")");
  }
}


// =====================================================================
// =====================================================================


}
} /* namespace openfluid */
