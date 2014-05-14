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
#include <geos/geom/LineSegment.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
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
  delete cbr.release();
  delete PrepGeom.first.release();
  delete PrepGeom.second.release();
  delete RemGeom.first.release();
  delete RemGeom.second.release();

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

  delete ThePolygons;
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
    if (!(*it)->getEdge()->isVisited())
    {
      geos::planargraph::Node * theNextNode=static_cast<openfluid::landr::LineStringEntity*>((*it)->getEdge())->getStartNode();

      if (Node->getCoordinate().equals(theNextNode->getCoordinate()))
        theNextNode=static_cast<openfluid::landr::LineStringEntity*>((*it)->getEdge())->getEndNode();

      // set Edge visited as true
      (*it)->getEdge()->setVisited(true);

      if (!theNextNode->isVisited())
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
      if (Geom1->getGeometryN(i)->getGeometryTypeId()!=geos::geom::GEOS_POLYGON)
        throw openfluid::base::FrameworkException(
            "LandRTools::computeIntersectPolygons",
            " The Geometry is not Polygon-typed.");

      unsigned int jEnd=Geom2->getNumGeometries();
      for (unsigned int j = 0; j < jEnd; j++)
      {
        if (Geom2->getGeometryN(j)->getGeometryTypeId()!=geos::geom::GEOS_POLYGON)
          throw openfluid::base::FrameworkException(
              "LandRTools::computeIntersectPolygons",
              " The Geometry is not Polygon-typed.");
        if ((Geom1->getGeometryN(i))->intersects(const_cast<geos::geom::Geometry*>(Geom2->getGeometryN(j))))
        {
          geos::geom::Geometry *Intersect=(Geom1->getGeometryN(i))->intersection(const_cast<geos::geom::Geometry*>(Geom2->getGeometryN(j)));
          if (Intersect->getGeometryTypeId()==geos::geom::GEOS_POLYGON)
            Polygons.push_back(dynamic_cast<geos::geom::Polygon*>(Intersect));
          else if (Intersect->getGeometryTypeId()==geos::geom::GEOS_MULTIPOLYGON)
          {
            unsigned int hEnd=Intersect->getNumGeometries();
            for (unsigned int h = 0; h < hEnd; h++)
              Polygons.push_back(dynamic_cast<geos::geom::Polygon*>(const_cast<geos::geom::Geometry*>(Intersect->getGeometryN(h))));

          }

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

std::vector<geos::geom::LineString*> LandRTools::splitLineStringByPoint( geos::geom::LineString& Entity,  geos::geom::Point& Point,double SnapTolerance)

{
  if (SnapTolerance<=0.0)
    throw  openfluid::base::FrameworkException(
        "LandRTools::splitLineStringByPoint : "
        "SnapTolerance must be superior to 0.0");


  std::vector<geos::geom::LineString*> vEntities;
  geos::geom::Point * StartPoint=Entity.getStartPoint();
  geos::geom::Point * EndPoint=Entity.getEndPoint();

  double endDistance=Point.getCoordinate()->distance(*(EndPoint->getCoordinate()));
  double startDistance=Point.getCoordinate()->distance(*(StartPoint->getCoordinate()));
  delete EndPoint;
  delete StartPoint;

  if (endDistance<=SnapTolerance||startDistance<=SnapTolerance)
    return vEntities;

  geos::geom::Geometry *Geom=openfluid::landr::LandRTools::computeSnapOverlayUnion(
      Point, Entity,SnapTolerance);
  if (Geom->getGeometryTypeId()!=1)
  {
    delete Geom;
    return vEntities;
  }
  delete Geom;

  unsigned int numVertices=Entity.getNumPoints()-1;
  unsigned int i=0;
  bool split=false;
  const geos::geom::CoordinateSequenceFactory *CoordSeqFactory=geos::geom::GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory();


  geos::geom::Coordinate newCoorPoint;
  while (i<numVertices&&!split)
  {
    geos::geom::Coordinate FirstCoord=Entity.getCoordinateN(i);
    geos::geom::Coordinate SecondCoord=Entity.getCoordinateN(i+1);

    std::vector<geos::geom::Coordinate> vCoor;
    vCoor.push_back(FirstCoord);
    vCoor.push_back(SecondCoord);

    geos::geom::CoordinateSequence* CoordSeq=CoordSeqFactory->create(&vCoor);
    geos::geom::LineString * NewLine=geos::geom::GeometryFactory::getDefaultInstance()->createLineString(CoordSeq);


    geos::geom::Geometry *Geom=openfluid::landr::LandRTools::computeSnapOverlayUnion(
        Point, *dynamic_cast<geos::geom::Geometry*>(NewLine),SnapTolerance);

    if (Geom->getGeometryTypeId()==1)
    {
      split=true;
      geos::geom::LineSegment LineSegment(FirstCoord,SecondCoord);
      LineSegment.project(*(Point.getCoordinate()),newCoorPoint);
    }
    else
      i++;

    delete Geom;

  }

  if (!split)
    throw  openfluid::base::FrameworkException(
        "LandRTools::splitLineString : "
        "Splitting operation impossible");

  unsigned int j=0;
  std::vector<geos::geom::Coordinate>* vFirstCoorLine =new std::vector<geos::geom::Coordinate>;
  for (;j<=i;j++)
    vFirstCoorLine->push_back(Entity.getCoordinateN(j));

  vFirstCoorLine->push_back(newCoorPoint);

  geos::geom::CoordinateSequence* FirstCoordSeq=CoordSeqFactory->create(vFirstCoorLine);
  FirstCoordSeq->removeRepeatedPoints();
  geos::geom::LineString * NewFirstLine=geos::geom::GeometryFactory::getDefaultInstance()->createLineString(FirstCoordSeq);

  std::vector<geos::geom::Coordinate>* vSecondCoorLine= new std::vector<geos::geom::Coordinate>;
  vSecondCoorLine->push_back(newCoorPoint);

  numVertices=Entity.getNumPoints();
  for (;j<numVertices;j++)
    vSecondCoorLine->push_back(Entity.getCoordinateN(j));
  geos::geom::CoordinateSequence* SecondCoordSeq=CoordSeqFactory->create(vSecondCoorLine);
  SecondCoordSeq->removeRepeatedPoints();
  geos::geom::LineString * NewSecondLine=geos::geom::GeometryFactory::getDefaultInstance()->createLineString(SecondCoordSeq);

  vEntities.push_back(NewFirstLine);
  vEntities.push_back(NewSecondLine);


  return vEntities;
}

// =====================================================================
// =====================================================================


void LandRTools::splitLineStringByPoints(geos::geom::LineString& Entity,std::vector<geos::geom::Point*>&Points,
                                         double SnapTolerance,std::vector<geos::geom::LineString*>&vLines,unsigned int step)
{

  if (SnapTolerance<=0.0)
    throw  openfluid::base::FrameworkException(
        "LandRTools::splitLineStringByPoints : "
        "SnapTolerance must be superior to 0.0");



  if (step>=Points.size())
    vLines.push_back(&Entity);

  else
  {
    std::vector<geos::geom::LineString*> vLinesSplitted=openfluid::landr::LandRTools::
        splitLineStringByPoint(Entity,*Points.at(step),SnapTolerance);
    step=step+1;
    if (vLinesSplitted.empty())
      splitLineStringByPoints(Entity,Points,SnapTolerance,vLines,step);

    else
    {
      splitLineStringByPoints(*vLinesSplitted[0],Points,SnapTolerance,vLines,step);
      splitLineStringByPoints(*vLinesSplitted[1],Points,SnapTolerance,vLines,step);

    }

  }

}

// =====================================================================
// =====================================================================


std::vector<geos::geom::LineString*>* LandRTools::cleanLineStrings(
    std::vector<geos::geom::LineString*> vLines,double SnapTolerance)
{
  try
  {
    std::list<geos::geom::LineString*> ExistingLines;
    geos::operation::linemerge::LineMerger Merger;

    std::vector<geos::geom::LineString*>::iterator it=vLines.begin();
    std::vector<geos::geom::LineString*>::iterator ite=vLines.end();
    const geos::geom::CoordinateSequenceFactory *CoordSeqFactory=geos::geom::GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory();

    for(;it!=ite;++it)
    {
      unsigned int numVertices=(*it)->getNumPoints()-1;
      unsigned int i=0;

      while(i<numVertices)
      {

        geos::geom::Coordinate FirstCoord=(*it)->getCoordinateN(i);
        geos::geom::Coordinate SecondCoord=(*it)->getCoordinateN(i+1);
        std::vector<geos::geom::Coordinate>* vCoor=new std::vector<geos::geom::Coordinate>;;
        vCoor->push_back(FirstCoord);
        vCoor->push_back(SecondCoord);

        geos::geom::CoordinateSequence* CoordSeq=CoordSeqFactory->create(vCoor);
        geos::geom::LineString * NewLine=geos::geom::GeometryFactory::getDefaultInstance()->createLineString(CoordSeq);
        NewLine->normalize();

        if (!exists(NewLine, ExistingLines,SnapTolerance))
        {
          ExistingLines.push_back(NewLine);
          Merger.add(NewLine);
        }
        i++;

      }

    }

    return Merger.getMergedLineStrings();
  }
  catch (geos::util::GEOSException& e)
  {
    throw openfluid::base::FrameworkException(
        "LandRTools::cleanLineStrings",
        "Error while cleaning lines, you can try again with an other snap tolerance value.\n"
        "(Details: "
        + std::string(e.what()) + ")");
  }

  return (std::vector<geos::geom::LineString*>*) 0;

}

// =====================================================================
// =====================================================================


}
} /* namespace openfluid */


