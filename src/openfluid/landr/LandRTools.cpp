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
  @file LandRTools.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
 */

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
#include <geos/geom/PrecisionModel.h>
#include <geos/precision/SimpleGeometryPrecisionReducer.h>

#include <openfluid/landr/GeosCompat.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/landr/LandRTools.hpp>


namespace openfluid { namespace landr {


geos::geom::LineString* LandRTools::computeMergedLineStringFromGeometry(geos::geom::Geometry* Geom)
{
  geos::geom::LineString* LS = nullptr;

  std::vector<geos::geom::LineString*>* Lines =  computeMergedLineStringsFromGeometry(Geom);

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


std::vector<geos::geom::LineString*>* LandRTools::computeMergedLineStringsFromGeometry(geos::geom::Geometry* Geom)
{
  std::vector<geos::geom::LineString*>* LS = nullptr;

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


std::vector<geos::geom::LineString*> LandRTools::computeVectorOfExteriorRings(openfluid::landr::VectorDataset& Val)
{
  if (!Val.isPolygonType())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "The GeoVectorValue is not polygon-typed");

  std::vector<geos::geom::LineString*> Lines;

  geos::geom::Geometry* Geom = Val.geometries();
  unsigned int iEnd=Geom->getNumGeometries();

  for (unsigned int i = 0; i < iEnd; i++)
  {
    Lines.push_back(const_cast<geos::geom::LineString*>
    (dynamic_cast<geos::geom::Polygon*>(const_cast<geos::geom::Geometry*>(Geom->getGeometryN(i)))->getExteriorRing()));
  }

  return Lines;
}


// =====================================================================
// =====================================================================


std::vector<geos::geom::LineString*> LandRTools::computeVectorOfLines(openfluid::landr::VectorDataset& Val)
{
  if (!Val.isLineType())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"The GeoVectorValue is not linestring-typed");

  std::vector<geos::geom::LineString*> Lines;

  geos::geom::Geometry* Geom = Val.geometries();

  unsigned int iEnd=Geom->getNumGeometries();
  for (unsigned int i = 0; i < iEnd; i++)
    Lines.push_back(
        dynamic_cast<geos::geom::LineString*>(const_cast<geos::geom::Geometry*>(Geom->getGeometryN(
            i))));

  return Lines;
}


// =====================================================================
// =====================================================================


std::vector<geos::geom::LineString*>* LandRTools::computeNodedLines(geos::geom::Geometry* Geom1,
                                                                geos::geom::Geometry* Geom2,
                                                                double SnapTolerance,
                                                                double PrecisionReducer)
{
  try
  {
    geos::geom::PrecisionModel *precision=new geos::geom::PrecisionModel(PrecisionReducer);
    geos::precision::SimpleGeometryPrecisionReducer reducer(precision);
    std::unique_ptr<geos::geom::Geometry> reducerGeom1(reducer.reduce(Geom1) );
    geos::geom::Geometry* Geom1Simple = reducerGeom1.release();
    std::unique_ptr<geos::geom::Geometry> reducerGeom2(reducer.reduce(Geom2) );
    geos::geom::Geometry* Geom2Simple = reducerGeom2.release();

    geos::geom::Geometry* UnionGeom = computeSnapOverlayUnion(*Geom1Simple, *Geom2Simple,SnapTolerance);

    std::list<geos::geom::LineString*> ExistingLines;
    geos::operation::linemerge::LineMerger lm;

    unsigned int iEnd=UnionGeom->getNumGeometries();

    for (unsigned int i = 0; i < iEnd; i++)
    {
      geos::geom::LineString* Line =
          dynamic_cast<geos::geom::LineString*>(const_cast<geos::geom::Geometry*>(UnionGeom->getGeometryN(i)));

      Line->normalize();

      if (!exists(Line, ExistingLines, SnapTolerance))
      {
        ExistingLines.push_back(Line);
        lm.add(Line);
      }
    }

    delete reducerGeom1.release();
    delete reducerGeom2.release();
    delete precision;
    delete Geom1Simple;
    delete Geom2Simple;

    return lm.getMergedLineStrings();
  }
  catch (geos::util::GEOSException& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Error while noding lines, try with a greater snap tolerance value "
                                              "(" + std::string(e.what()) + ")");
  }

  return nullptr;
}


// =====================================================================
// =====================================================================


geos::geom::Geometry* LandRTools::computeSnapOverlayUnion(geos::geom::Geometry& Geom1,
                                                          geos::geom::Geometry& Geom2,
                                                          double SnapTolerance)
{
  GEOM_PTR_PAIR PrepGeom;


  geos::operation::overlay::snap::GeometrySnapper::snap(Geom1,Geom2,SnapTolerance,PrepGeom);

  std::unique_ptr<geos::geom::Geometry> Result(geos::operation::overlay::OverlayOp::overlayOp(
                                                 PrepGeom.first.get(), PrepGeom.second.get(),
                                                 geos::operation::overlay::OverlayOp::opUNION));

  delete PrepGeom.first.release();
  delete PrepGeom.second.release();

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


void LandRTools::polygonizeGeometry(std::vector<geos::geom::Geometry*>& Lines,
                                    std::vector<geos::geom::Polygon*>& Polygons,
                                    std::vector<const geos::geom::LineString*>& Dangles)
{
  geos::operation::polygonize::Polygonizer* P = new geos::operation::polygonize::Polygonizer();

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
      geos::planargraph::Node * theNextNode=
          static_cast<openfluid::landr::LineStringEntity*>((*it)->getEdge())->startNode();

      if (Node->getCoordinate().equals(theNextNode->getCoordinate()))
        theNextNode=static_cast<openfluid::landr::LineStringEntity*>((*it)->getEdge())->endNode();

      // set Edge visited as true
      (*it)->getEdge()->setVisited(true);

      if (!theNextNode->isVisited())
        markVisitedNodesUsingDFS(theNextNode);
    }
  }

}


// =====================================================================
// =====================================================================


std::vector<geos::geom::Polygon*> LandRTools::computeIntersectPolygons(geos::geom::Geometry* Geom1,
                                                                       geos::geom::Geometry* Geom2)
{
  try
  {
    std::vector<geos::geom::Polygon*> Polygons;

    unsigned int iEnd=Geom1->getNumGeometries();
    for (unsigned int i = 0; i < iEnd; i++)
    {
      if (Geom1->getGeometryN(i)->getGeometryTypeId()!=geos::geom::GEOS_POLYGON)
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"The Geometry is not Polygon-typed");

      unsigned int jEnd=Geom2->getNumGeometries();

      for (unsigned int j = 0; j < jEnd; j++)
      {
        if (Geom2->getGeometryN(j)->getGeometryTypeId() != geos::geom::GEOS_POLYGON)
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"The Geometry is not Polygon-typed.");

        if ((Geom1->getGeometryN(i))->intersects(const_cast<geos::geom::Geometry*>(Geom2->getGeometryN(j))))
        {
          geos::geom::Geometry *Intersect =
              (Geom1->getGeometryN(i))->intersection(const_cast<geos::geom::Geometry*>(Geom2->getGeometryN(j)));

          if (Intersect->getGeometryTypeId() == geos::geom::GEOS_POLYGON)
            Polygons.push_back(dynamic_cast<geos::geom::Polygon*>(Intersect));

          else if (Intersect->getGeometryTypeId() == geos::geom::GEOS_MULTIPOLYGON)
          {
            unsigned int hEnd=Intersect->getNumGeometries();

            for (unsigned int h = 0; h < hEnd; h++)
            {
              Polygons.push_back(dynamic_cast<geos::geom::Polygon*>
                (const_cast<geos::geom::Geometry*>(Intersect->getGeometryN(h))));
            }
          }
        }
      }
    }
    return Polygons;
  }
  catch (geos::util::GEOSException& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Error while Intersecting Polygons ("  + std::string(e.what()) + ")");
  }
}


// =====================================================================
// =====================================================================


std::vector<geos::geom::LineString*> LandRTools::splitLineStringByPoint(geos::geom::LineString& Entity,
                                                                         geos::geom::Point& Point,
                                                                         double SnapTolerance)

{
  if (SnapTolerance<=0.0)
    throw  openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"SnapTolerance must be superior to 0.0");

  std::vector<geos::geom::LineString*> vEntities;
  geos::geom::Point * StartPoint=Entity.getStartPoint();
  geos::geom::Point * EndPoint=Entity.getEndPoint();

  double endDistance=Point.getCoordinate()->distance(*(EndPoint->getCoordinate()));
  double startDistance=Point.getCoordinate()->distance(*(StartPoint->getCoordinate()));
  delete EndPoint;
  delete StartPoint;

  if (endDistance<=SnapTolerance||startDistance<=SnapTolerance)
    return vEntities;

  geos::geom::Geometry *Geom=openfluid::landr::LandRTools::computeSnapOverlayUnion(Point, Entity,SnapTolerance);

  if (Geom->getGeometryTypeId()!=1)
  {
    delete Geom;
    return vEntities;
  }
  delete Geom;

  unsigned int numVertices = Entity.getNumPoints() - 1;
  unsigned int i=0;
  bool split=false;
  const geos::geom::CoordinateSequenceFactory *CoordSeqFactory=
      geos::geom::GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory();


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
    throw  openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Splitting operation can not be performed");

  unsigned int j=0;
  std::vector<geos::geom::Coordinate>* vFirstCoorLine = new std::vector<geos::geom::Coordinate>;
  for (;j<=i;j++)
    vFirstCoorLine->push_back(Entity.getCoordinateN(j));

  vFirstCoorLine->push_back(newCoorPoint);

  geos::geom::CoordinateSequence* FirstCoordSeq = CoordSeqFactory->create(vFirstCoorLine);

  FirstCoordSeq->removeRepeatedPoints();
  geos::geom::LineString * NewFirstLine=
      geos::geom::GeometryFactory::getDefaultInstance()->createLineString(FirstCoordSeq);

  std::vector<geos::geom::Coordinate>* vSecondCoorLine = new std::vector<geos::geom::Coordinate>;
  vSecondCoorLine->push_back(newCoorPoint);

  numVertices = Entity.getNumPoints();

  for (;j<numVertices;j++)
    vSecondCoorLine->push_back(Entity.getCoordinateN(j));

  geos::geom::CoordinateSequence* SecondCoordSeq = CoordSeqFactory->create(vSecondCoorLine);

  SecondCoordSeq->removeRepeatedPoints();

  geos::geom::LineString* NewSecondLine=
      geos::geom::GeometryFactory::getDefaultInstance()->createLineString(SecondCoordSeq);

  vEntities.push_back(NewFirstLine);
  vEntities.push_back(NewSecondLine);

  return vEntities;
}


// =====================================================================
// =====================================================================


void LandRTools::splitLineStringByPoints(geos::geom::LineString& Entity,
                                             std::vector<geos::geom::Point*>&Points,
                                             double SnapTolerance,std::vector<geos::geom::LineString*>&vLines,
                                             unsigned int step)
{

  if (SnapTolerance<=0.0)
    throw  openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"SnapTolerance must be greater or equal to 0.0");


  if (step>=Points.size())
    vLines.push_back(&Entity);
  else
  {
    std::vector<geos::geom::LineString*> vLinesSplitted =
      openfluid::landr::LandRTools::splitLineStringByPoint(Entity,*Points.at(step),SnapTolerance);

    step++;

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


std::vector<geos::geom::LineString*>* LandRTools::cleanLineStrings(std::vector<geos::geom::LineString*> vLines,
                                                                   double SnapTolerance)
{
  try
  {
    std::list<geos::geom::LineString*> ExistingLines;
    geos::operation::linemerge::LineMerger Merger;

    std::vector<geos::geom::LineString*>::iterator it = vLines.begin();
    std::vector<geos::geom::LineString*>::iterator ite = vLines.end();
    const geos::geom::CoordinateSequenceFactory *CoordSeqFactory =
        geos::geom::GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory();

    for(;it!=ite;++it)
    {
      unsigned int numVertices = (*it)->getNumPoints()-1;
      unsigned int i = 0;

      while(i<numVertices)
      {
        geos::geom::Coordinate FirstCoord = (*it)->getCoordinateN(i);
        geos::geom::Coordinate SecondCoord = (*it)->getCoordinateN(i+1);
        std::vector<geos::geom::Coordinate>* vCoor = new std::vector<geos::geom::Coordinate>;;
        vCoor->push_back(FirstCoord);
        vCoor->push_back(SecondCoord);

        geos::geom::CoordinateSequence* CoordSeq = CoordSeqFactory->create(vCoor);
        geos::geom::LineString * NewLine =
          geos::geom::GeometryFactory::getDefaultInstance()->createLineString(CoordSeq);
        NewLine->normalize();

        if (!exists(NewLine,ExistingLines,SnapTolerance))
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
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Error while cleaning lines, try with an other snap tolerance value "
                                              "( " + std::string(e.what()) + ")");
  }

  return nullptr;
}


// =====================================================================
// =====================================================================


std::vector<geos::geom::Point*> LandRTools::computeNodesFromVectorOfLines(
                                    std::vector<geos::geom::LineString*>& NodedLines)
{
  std::vector<geos::geom::Point*> vPoints;
  std::vector<geos::geom::LineString*>::iterator it = NodedLines.begin();
  std::vector<geos::geom::LineString*>::iterator ite = NodedLines.end();

  for (;it!=ite;++it)
  {
    geos::geom::Point* StartPoint = (*it)->getStartPoint();
    geos::geom::Point* EndPoint = (*it)->getEndPoint();
    std::vector<geos::geom::Point*>::iterator jt = vPoints.begin();
    std::vector<geos::geom::Point*>::iterator jte = vPoints.end();

    bool equalGeom = false;

    while (jt<jte&&!equalGeom)
    {
      if ((*jt)->equals(StartPoint))
        equalGeom=true;
      else
        ++jt;
    }

    if (!equalGeom)
      vPoints.push_back(StartPoint);
    else
      delete StartPoint;


    jt = vPoints.begin();
    jte = vPoints.end();
    equalGeom = false;

    while (jt<jte&&!equalGeom)
    {
      if ((*jt)->equals(EndPoint))
        equalGeom=true;
      else
        ++jt;
    }

    if (!equalGeom)
      vPoints.push_back(EndPoint);
    else
      delete EndPoint;
  }

  return vPoints;
}


// =====================================================================
// =====================================================================


void LandRTools::markInvertedLineStringEntityUsingDFS(geos::planargraph::Node* Node,
                                                      std::vector<int>& vectIdent )
{
  Node->setVisited(true);

  geos::planargraph::DirectedEdgeStar *DEdge = Node->getOutEdges();

  std::vector<geos::planargraph::DirectedEdge*>::iterator it = DEdge->begin();
  std::vector<geos::planargraph::DirectedEdge*>::iterator ite = DEdge->end();
  for (; it != ite; ++it)
  {
    if (!(*it)->getEdge()->isVisited())
    {
      geos::planargraph::Node * theNextNode=
          static_cast<openfluid::landr::LineStringEntity*>((*it)->getEdge())->startNode();

      if ((Node->getCoordinate()).equals(theNextNode->getCoordinate()))
      {
        theNextNode=static_cast<openfluid::landr::LineStringEntity*>((*it)->getEdge())->endNode();
        int OfldId = static_cast<openfluid::landr::LineStringEntity*>((*it)->getEdge())->getOfldId();
        vectIdent.push_back(OfldId);
      }

      (*it)->getEdge()->setVisited(true);

      if (!theNextNode->isVisited())
        markInvertedLineStringEntityUsingDFS(theNextNode,vectIdent);
    }
  }
}


// =====================================================================
// =====================================================================


bool LandRTools::isExtentsIntersect(std::vector<OGREnvelope> vEnvelope)
{
  if (vEnvelope.size() > 1)
  {
    for (unsigned int i = 0; i < vEnvelope.size(); i++)
    {
      for (unsigned int j = i + 1; j < vEnvelope.size(); j++)
      {
        if (vEnvelope[i].Intersects(vEnvelope[j]) != 1)
        return false;
      }
    }
  }

  return true;
}


} } // namespaces
