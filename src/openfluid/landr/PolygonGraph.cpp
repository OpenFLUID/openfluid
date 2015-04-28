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
  @file PolygonGraph.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
 */

#include "PolygonGraph.hpp"

#include <openfluid/landr/PolygonEntity.hpp>
#include <openfluid/landr/PolygonEdge.hpp>
#include <openfluid/landr/LandRTools.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/StringValue.hpp>
#include <geos/geom/Polygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/LineString.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/planargraph/DirectedEdge.h>
#include <algorithm>
#include <complex>

namespace openfluid { namespace landr {


// =====================================================================
// =====================================================================


PolygonGraph::PolygonGraph() :
                LandRGraph()
{

}


// =====================================================================
// =====================================================================


PolygonGraph::PolygonGraph(openfluid::core::GeoVectorValue& Val) :
                LandRGraph(Val)
{

}


// =====================================================================
// =====================================================================


PolygonGraph::PolygonGraph(openfluid::landr::VectorDataset& Vect) :
                LandRGraph(Vect)
{

}


// =====================================================================
// =====================================================================


PolygonGraph* PolygonGraph::create(openfluid::core::GeoVectorValue& Val)
{
  if (!Val.isPolygonType())
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "GeoVectorValue is not Polygon type");

  PolygonGraph* Graph = new PolygonGraph(Val);
  try{

    Graph->addEntitiesFromGeoVector();

  }catch (openfluid::base::FrameworkException& e)
  {
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "Unable to create the PolygonGraph");
  }

  return Graph;
}


// =====================================================================
// =====================================================================


PolygonGraph* PolygonGraph::create(openfluid::landr::VectorDataset& Vect)
{
  if (!Vect.isPolygonType())
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "VectorDataset is not Polygon type");

  PolygonGraph* Graph = new PolygonGraph(Vect);
  try{

    Graph->addEntitiesFromGeoVector();

  }catch (openfluid::base::FrameworkException& e)
  {
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "Unable to create the PolygonGraph");
  }

  return Graph;
}


// =====================================================================
// =====================================================================


PolygonGraph* PolygonGraph::create(const LandRGraph::Entities_t& Entities)
{
  PolygonGraph* Graph = new PolygonGraph();
  Graph->addEntitiesFromEntityList(Entities);

  return Graph;
}


// =====================================================================
// =====================================================================


PolygonGraph::~PolygonGraph()
{
  unsigned int iEnd=edges.size();
  for (unsigned int i = 0; i < iEnd; i++)
    delete edges[i];
}


// =====================================================================
// =====================================================================


LandRGraph::GraphType PolygonGraph::getType()
{
  return POLYGON;
}


// =====================================================================
// =====================================================================


void PolygonGraph::addEntity(LandREntity* Entity)
{
  PolygonEntity* NewEntity = dynamic_cast<PolygonEntity*>(Entity);

  const geos::geom::Polygon* Polygon = NewEntity->polygon();

  std::vector<geos::geom::Geometry*> SharedGeoms;

  try
  {
    LandRGraph::Entities_t::iterator it = m_Entities.begin();
    LandRGraph::Entities_t::iterator ite = m_Entities.end();
    for (; it != ite; ++it)
    {
      PolygonEntity* Poly = dynamic_cast<PolygonEntity*>(*it);
      std::vector<geos::geom::LineString*> SharedLines =
          NewEntity->computeLineIntersectionsWith(*Poly);

      unsigned int iEnd=SharedLines.size();
      for (unsigned int i = 0; i < iEnd; i++)
      {
        geos::geom::LineString* SharedLine = SharedLines[i];

        PolygonEdge* SharedEdge = createEdge(*SharedLine);

        NewEntity->addEdge(*SharedEdge);
        Poly->addEdge(*SharedEdge);

        removeSegment(Poly, SharedLine);

        SharedGeoms.push_back(SharedLine);
      }

    }

    geos::geom::MultiLineString* NewMultiShared =
        mp_Factory->createMultiLineString(SharedGeoms);

    geos::geom::Geometry* DiffGeom = Polygon->getExteriorRing()->difference(
        NewMultiShared);

    if (!DiffGeom->isEmpty())
    {
      std::vector<geos::geom::LineString*>* NewLines =
          LandRTools::computeMergedLineStringsFromGeometry(DiffGeom);

      unsigned int iEnd=NewLines->size();
      for (unsigned int i = 0; i < iEnd; i++)
      {
        PolygonEdge* NewEdge = createEdge(*NewLines->at(i));

        if (NewEdge)
          NewEntity->addEdge(*NewEdge);
      }

    }

    m_EntitiesByOfldId[NewEntity->getOfldId()] = NewEntity;
    m_Entities.push_back(NewEntity);
    delete DiffGeom;
    delete NewMultiShared;
  }
  catch (openfluid::base::FrameworkException& e)
  {
    std::cerr << e.what() << std::endl;
    throw;
  }

}


// =====================================================================
// =====================================================================


LandREntity* PolygonGraph::createNewEntity(const geos::geom::Geometry* Geom,
                                           unsigned int OfldId)
{
  return new PolygonEntity(Geom, OfldId);
}


// =====================================================================
// =====================================================================


PolygonEdge* PolygonGraph::createEdge(geos::geom::LineString& LineString)
{
  if (LineString.isEmpty())
    return (PolygonEdge*) 0;

  geos::geom::CoordinateSequence* Coordinates =
      geos::geom::CoordinateSequence::removeRepeatedPoints(
          LineString.getCoordinatesRO());

  const geos::geom::Coordinate& StartCoordinate = Coordinates->getAt(0);
  const geos::geom::Coordinate& EndCoordinate = Coordinates->getAt(
      Coordinates->getSize() - 1);

  geos::planargraph::Node* StartNode = node(StartCoordinate);
  geos::planargraph::Node* EndNode = node(EndCoordinate);

  geos::planargraph::DirectedEdge* DirectedEdge0 =
      new geos::planargraph::DirectedEdge(StartNode, EndNode,
                                          Coordinates->getAt(1), true);

  geos::planargraph::DirectedEdge* DirectedEdge1 =
      new geos::planargraph::DirectedEdge(
          EndNode, StartNode, Coordinates->getAt(Coordinates->getSize() - 2),
          false);

  PolygonEdge* NewEdge = new PolygonEdge(LineString);

  NewEdge->setDirectedEdges(DirectedEdge0, DirectedEdge1);

  add(NewEdge);

  delete Coordinates;

  return NewEdge;
}


// =====================================================================
// =====================================================================


void PolygonGraph::removeSegment(PolygonEntity* Entity,
                                 geos::geom::LineString* Segment)
{
  PolygonEdge* OldEdge = Entity->findEdgeLineIntersectingWith(*Segment);

  if (!OldEdge)
  {
    delete OldEdge;

    std::ostringstream s;
    s << "Error when removing segment (" << Segment->toString()
                  << ") from Polygon " << Entity->getOfldId()
                  << ": doesn't find edge intersection.";

    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION, s.str());
  }

  geos::geom::Geometry* DiffGeom = OldEdge->line()->difference(Segment);

  if (!DiffGeom->isEmpty())
  {
    if (DiffGeom->getDimension() != geos::geom::Dimension::L)
    {
      delete DiffGeom;
      delete OldEdge;

      std::ostringstream s;
      s << "Error when removing segment (" << Segment->toString()
                    << ") from Polygon " << Entity->getOfldId()
                    << ": difference geometry is not \"Line\" typed.";

      throw openfluid::base::FrameworkException(
          OPENFLUID_CODE_LOCATION,
          s.str());
    }

    std::vector<geos::geom::LineString*>* DiffGeoms =
        LandRTools::computeMergedLineStringsFromGeometry(DiffGeom);

    unsigned int iEnd=DiffGeoms->size();
    for (unsigned int i = 0; i < iEnd; i++)
    {
      PolygonEdge* NewEdge = createEdge(
          dynamic_cast<geos::geom::LineString&>(*DiffGeoms->at(i)));

      if (NewEdge)
        Entity->addEdge(*NewEdge);
    }
  }

  remove(OldEdge);
  delete DiffGeom;
  Entity->removeEdge(OldEdge);
}


// =====================================================================
// =====================================================================


PolygonEntity* PolygonGraph::entity(int OfldId)
{
  return dynamic_cast<PolygonEntity*>(LandRGraph::entity(OfldId));
}


// =====================================================================
// =====================================================================


bool PolygonGraph::isComplete()
{
  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  for (; it != ite; ++it)
  {
    if (!(dynamic_cast<PolygonEntity*>(*it))->isComplete())
      return false;
  }

  return true;
}


// =====================================================================
// =====================================================================


bool PolygonGraph::hasIsland()
{
  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  for (; it != ite; ++it)
  {
    PolygonEntity*  Entity=dynamic_cast<PolygonEntity*>(*it);
    const geos::geom::Polygon* RefPoly = Entity->polygon();

    if (RefPoly->getNumInteriorRing ()!=0)
    {
      int NumInteriorRing=RefPoly->getNumInteriorRing ();

      for (int i=0;i<NumInteriorRing;i++)
      {
        const geos::geom::LineString *InnerRing=RefPoly->getInteriorRingN(i);

        LandRGraph::Entities_t::iterator it2 = m_Entities.begin();
        LandRGraph::Entities_t::iterator itE2 = m_Entities.end();
        for (; it2 != itE2; ++it2)
        {
          PolygonEntity*  IslandEntity=dynamic_cast<PolygonEntity*>(*it2);
          if (Entity->getOfldId()!=IslandEntity->getOfldId())
          {
            const geos::geom::Polygon* IslandPoly = IslandEntity->polygon();
            const geos::geom::LineString *OuterRing=IslandPoly->getExteriorRing();
            if (InnerRing->within(OuterRing))
              return true;

          }
        }
      }
    }
  }
  return false;
}


// =====================================================================
// =====================================================================


PolygonGraph::RastValByRastPoly_t PolygonGraph::computeRasterPolyOverlapping(PolygonEntity& Entity)
{
  const geos::geom::Polygon* RefPoly = Entity.polygon();

  RastValByRastPoly_t IntersectPolys;

  std::vector<geos::geom::Polygon*>* RasterPolys = rasterPolygonizedPolys();

  if (!RasterPolys)
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "No RasterPolygonizedMultiPolygon associated to the PolygonGraph");

  std::vector<geos::geom::Polygon*>::iterator it = RasterPolys->begin();
  std::vector<geos::geom::Polygon*>::iterator ite = RasterPolys->end();
  for (; it != ite; ++it)
  {
    if (RefPoly->relate(*it, "21*******"))
    {
      geos::geom::Geometry* Inter = RefPoly->intersection(*it);

      unsigned int iEnd=Inter->getNumGeometries();
      for (unsigned int i = 0; i < iEnd; i++)
      {
        geos::geom::Polygon* Poly =
            dynamic_cast<geos::geom::Polygon*>(const_cast<geos::geom::Geometry*>(Inter->getGeometryN(
                i)));
        if (Poly)
        {
          // !! copy doesn't keep UserData !
          Poly->setUserData((*it)->getUserData());
          IntersectPolys[Poly] = Poly->getArea();
        }
      }
    }
  }

  return IntersectPolys;
}


// =====================================================================
// =====================================================================


void PolygonGraph::setAttributeFromMeanRasterValues(const std::string& AttributeName)
{
  addAttribute(AttributeName);

  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  for (; it != ite; ++it)
  {
    PolygonGraph::RastValByRastPoly_t RastPolys = computeRasterPolyOverlapping(
        *dynamic_cast<PolygonEntity*>(*it));

    double PolyArea = (double)(*it)->getArea();

    if (!PolyArea)
      continue;

    double Mean = 0;

    PolygonGraph::RastValByRastPoly_t::iterator itPix = RastPolys.begin();
    PolygonGraph::RastValByRastPoly_t::iterator itPixE = RastPolys.end();
    for (; itPix != itPixE; ++itPix)
    {
      double* PixelVal = ((double*) itPix->first->getUserData());

      if (std::isnan(*PixelVal))
        *PixelVal = 1;

      double OverlappingArea = (double)itPix->second;

      Mean += *PixelVal * (OverlappingArea / PolyArea);
    }

    (*it)->setAttributeValue(AttributeName, new core::DoubleValue(Mean));
  }
}


// =====================================================================
// =====================================================================


void PolygonGraph::createVectorRepresentation(std::string FilePath,
                                              std::string FileName)
{
  openfluid::landr::VectorDataset* OutVector =
      new openfluid::landr::VectorDataset("tmp_" + FileName);

  OutVector->addALayer("", wkbLineString);

  std::vector<geos::planargraph::Edge*>* Edges = getEdges();

  if (!Edges)
  {
    delete OutVector;

    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "No edges for this graph.");
  }

  std::vector<geos::planargraph::Edge*>::iterator it = Edges->begin();
  std::vector<geos::planargraph::Edge*>::iterator ite = Edges->end();
  for (; it != ite; ++it)
  {
    OGRFeature* Feat = OGRFeature::CreateFeature(OutVector->layerDef());

    geos::geom::Geometry* Geom =
        dynamic_cast<geos::geom::Geometry*>((dynamic_cast<PolygonEdge*>(*it))->line());

    OGRGeometry* OGRGeom = OGRGeometryFactory::createFromGEOS((GEOSGeom) Geom);

    if (!OGRGeom)
    {
      delete OutVector;
      delete Geom;
      throw openfluid::base::FrameworkException(
          OPENFLUID_CODE_LOCATION,
          "Failed to transform geometry from GEOS to OGR.");
    }

    Feat->SetGeometry(OGRGeom);

    if (OutVector->layer(0)->CreateFeature(Feat) != OGRERR_NONE)
    {
      delete OutVector;

      throw openfluid::base::FrameworkException(
          OPENFLUID_CODE_LOCATION,
          "Failed to create feature in shapefile.");
    }
    delete Geom;
    OGRFeature::DestroyFeature(Feat);
  }

  OutVector->copyToDisk(FilePath, FileName, true);

  delete OutVector;
}


// =====================================================================
// =====================================================================


void PolygonGraph::computeLineStringNeighbours(LineStringGraph& Graph,
                                               openfluid::landr::LandRTools::Relationship Relation,
                                               double BufferDistance,
                                               double ContactLength)
{
  if (Relation == LandRTools::TOUCHES && ContactLength==0)
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
           "ContactLength must be superior to 0 for LandRTools::TOUCHES RelationShip");




  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  for (; it != ite; ++it)
    dynamic_cast<PolygonEntity*>(*it)->computeLineStringNeighbours(
        Graph, Relation, BufferDistance, ContactLength);
}


// =====================================================================
// =====================================================================


void PolygonGraph::createEdgeAttribute(std::string AttributeName,
                                       core::Value &Value)
{
  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  for (; it != ite; ++it)
  {
    std::vector<openfluid::landr::PolygonEdge*> vPolygonEdges=
        (dynamic_cast<openfluid::landr::PolygonEntity*>(*it))->m_PolyEdges;

    std::vector<openfluid::landr::PolygonEdge*>::iterator it2 = vPolygonEdges.begin();
    std::vector<openfluid::landr::PolygonEdge*>::iterator it2E = vPolygonEdges.end();
    for (; it2 != it2E; ++it2)
    {
      if (!(*it2)->m_EdgeAttributes.count(AttributeName))
        (*it2)->m_EdgeAttributes[AttributeName] = Value.clone();
    }
  }
}


// =====================================================================
// =====================================================================


void PolygonGraph::removeEdgeAttribute(std::string AttributeName)
{
  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  for (; it != ite; ++it)
  {
    std::vector<openfluid::landr::PolygonEdge*> vPolygonEdges=
        (dynamic_cast<openfluid::landr::PolygonEntity*>(*it))->m_PolyEdges;

    std::vector<openfluid::landr::PolygonEdge*>::iterator it2 = vPolygonEdges.begin();
    std::vector<openfluid::landr::PolygonEdge*>::iterator it2E = vPolygonEdges.end();
    for (; it2 != it2E; ++it2)
      (*it2)->removeAttribute(AttributeName);
  }

}


// =====================================================================
// =====================================================================


std::vector<std::string> PolygonGraph::getEdgeAttributeNames()
{
  std::vector<std::string> Names;
  std::vector<PolygonEdge*> vPolyEdge=
      (dynamic_cast<openfluid::landr::PolygonEntity*>(*m_Entities.begin()))->m_PolyEdges;

  std::map<std::string, core::Value*> Attr =(*vPolyEdge.begin())->m_EdgeAttributes;

  std::map<std::string, core::Value*>::iterator it = Attr.begin();
  std::map<std::string, core::Value*>::iterator ite = Attr.end();
  for (; it != ite; ++it)
    Names.push_back(it->first);

  return Names;
}


// =====================================================================
// =====================================================================


void PolygonGraph::removeEntity(int OfldId)
{

  PolygonEntity* Ent = entity(OfldId);

  if (!Ent)
  {
    std::ostringstream s;
    s << "No entity with id " << OfldId;
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        s.str());
    return;
  }
  computeNeighbours();

  std::vector<PolygonEdge*> vEdges=Ent->m_PolyEdges;
  std::vector<PolygonEdge*>::iterator it=vEdges.begin();
  std::vector<PolygonEdge*>::iterator ite=vEdges.end();

  std::list<PolygonEdge*>lEdges;
  for ( ; it != ite; ++it)
  {
    if ((*it)->getFaces().size()==1)
      lEdges.push_back(*it);
  }

  // for each neighbour of Ent

  openfluid::landr::PolygonEntity::NeighboursMap_t::iterator jt = Ent->mp_NeighboursMap->begin();
  openfluid::landr::PolygonEntity::NeighboursMap_t::iterator jte = Ent->mp_NeighboursMap->end();
  std::list<PolygonEntity*> lNeighbours;
  for (;jt!=jte;++jt)
  {

    jt->first->mp_NeighboursMap->erase(Ent);
    lNeighbours.push_back(jt->first);
    std::vector<PolygonEdge*> vNeighbourEdges=jt->first->m_PolyEdges;
    std::vector<PolygonEdge*>::iterator ht=vNeighbourEdges.begin();
    std::vector<PolygonEdge*>::iterator hte=vNeighbourEdges.end();

    for (;ht!=hte;++ht)
    {
      if ((*ht)->isLineInFace(*Ent))
        (*ht)->removeFace(Ent);
    }
  }

  std::list<PolygonEdge*>::iterator lt=lEdges.begin();
  std::list<PolygonEdge*>::iterator lte=lEdges.end();
  for (;lt!=lte;++lt)
    removeSegment(Ent,(*lt)->line());


  m_Entities.erase(std::find(m_Entities.begin(), m_Entities.end(), Ent));
  m_EntitiesByOfldId.erase(OfldId);
  delete Ent;
  removeUnusedNodes();

  //rebuild the Edges of Neighbours of Ent
  std::list<PolygonEntity*>::iterator mt=lNeighbours.begin();
  std::list<PolygonEntity*>::iterator mte=lNeighbours.end();
  for (;mt!=mte;++mt)
    cleanEdges(**mt);


  removeUnusedNodes();
}


// =====================================================================
// =====================================================================


void PolygonGraph::cleanEdges(PolygonEntity & Entity)
{

  Entity.computeNeighbours();

  std::vector<PolygonEdge*> vNeighbourEdges=Entity.m_PolyEdges;
  std::vector<PolygonEdge*>::iterator nt=vNeighbourEdges.begin();
  std::vector<PolygonEdge*>::iterator nte=vNeighbourEdges.end();
  std::list<PolygonEdge*> lEdgesWithOneFace;
  for (;nt!=nte;++nt)
  {
    if ((*nt)->getFaces().size()==1)
      lEdgesWithOneFace.push_back(*nt);
  }
  if (!lEdgesWithOneFace.empty())
  {

    std::list<PolygonEdge*>::iterator ot=lEdgesWithOneFace.begin();
    std::list<PolygonEdge*>::iterator ote=lEdgesWithOneFace.end();

    while (ot!=ote)
    {

      std::list<PolygonEdge*>::iterator ot2=ot;
      ++ot2;

      while (ot2 != ote)
      {
        if ((*ot)->isCoincident(*ot2))
        {
          geos::geom::LineString * NewLine=Entity.mergeEdges((*ot), (*ot2));
          remove(*ot2);
          Entity.removeEdge(*ot2);
          remove(*ot);
          Entity.removeEdge(*ot);
          PolygonEdge* NewEdge=createEdge(*NewLine);
          Entity.addEdge(*NewEdge);

          lEdgesWithOneFace.clear();
          vNeighbourEdges=Entity.m_PolyEdges;
          std::vector<PolygonEdge*>::iterator pt=vNeighbourEdges.begin();
          std::vector<PolygonEdge*>::iterator pte=vNeighbourEdges.end();

          for (;pt!=pte;++pt)
          {
            if ((*pt)->getFaces().size()==1)
              lEdgesWithOneFace.push_back(*pt);
          }
          ot=lEdgesWithOneFace.begin();
          ote=lEdgesWithOneFace.end();
          ot2=ot;
          ++ot2;
        }
        else
          ++ot2;

      }
      ++ot;
    }

  }

  removeUnusedNodes();

}


// =====================================================================
// =====================================================================


std::multimap<double,  PolygonEntity*> PolygonGraph::getPolygonEntitiesByMinArea(double MinArea, bool Neighbour)
{
  if (MinArea<=0.0)
    throw  openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "Threshold must be superior to 0.0");

  std::list<LandREntity*> lEntities=getOfldIdOrderedEntities();
  std::list<LandREntity*>::iterator it = lEntities.begin();
  std::list<LandREntity*>::iterator ite = lEntities.end();
  std::multimap<double, PolygonEntity*> mOrderedArea;
  for (;it!=ite;++it)
  {
    bool NeighbourExist=true;
    if(Neighbour)
    {
      dynamic_cast<openfluid::landr::PolygonEntity*>(*it)->computeNeighbours();
      if (dynamic_cast<openfluid::landr::PolygonEntity*>(*it)->getOrderedNeighbourOfldIds().empty())
        NeighbourExist=false;
    }

    if ((*it)->getArea()<MinArea && NeighbourExist)
      mOrderedArea.insert ( std::pair<double, PolygonEntity*>
      ((*it)->getArea(),dynamic_cast<openfluid::landr::PolygonEntity*>(*it)) );
  }
  return mOrderedArea;

}


// =====================================================================
// =====================================================================


void PolygonGraph::mergePolygonEntities(PolygonEntity& Entity,
                                        PolygonEntity& EntityToMerge)
{

  //ensure that the two PolygonEntities are neighbours
  std::vector<PolygonEdge*> vEdge=Entity.getCommonEdgesWith(EntityToMerge);
  if (vEdge.empty())
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "The PolygonEntities are not neighbours");

  geos::geom::Geometry *  NewPoly=Entity.geometry()->Union(EntityToMerge.geometry());


  int OfldId=Entity.getOfldId();
  int OfldIdToMerge=EntityToMerge.getOfldId();

  try {

    openfluid::landr::PolygonEntity* Entity2 =
        new openfluid::landr::PolygonEntity(dynamic_cast<geos::geom::Polygon*>(NewPoly),
                                            OfldId);
    removeEntity(OfldId);
    removeEntity(OfldIdToMerge);
    addEntity(Entity2);

  } catch (openfluid::base::FrameworkException& e) {
    std::ostringstream s;
    s << "Merge operation impossible for entity" << OfldId<<" : "<<e.what() ;
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,s.str());
  }

}


// =====================================================================
// =====================================================================


std::multimap<double,  PolygonEntity*> PolygonGraph::getPolygonEntitiesByCompactness(double Compactness, bool Neighbour)
{
  if (Compactness<=0.0)
    throw  openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "Threshold must be superior to 0.0");

  std::list<LandREntity*> lEntities=getOfldIdOrderedEntities();
  std::list<LandREntity*>::iterator it = lEntities.begin();
  std::list<LandREntity*>::iterator ite = lEntities.end();
  std::multimap<double, PolygonEntity*> mOrderedCompact;
  for (;it!=ite;++it)
  {
    bool NeighbourExist=true;
    if(Neighbour)
    {
      dynamic_cast<openfluid::landr::PolygonEntity*>(*it)->computeNeighbours();
      if (dynamic_cast<openfluid::landr::PolygonEntity*>(*it)->getOrderedNeighbourOfldIds().empty())
        NeighbourExist=false;
    }
    double valCompact=(*it)->getLength()/(2*std::sqrt(4 * std::atan(1.0)*(*it)->getArea()));
    if (valCompact>Compactness && NeighbourExist)
      mOrderedCompact.insert ( std::pair<double, PolygonEntity*>
      (valCompact,dynamic_cast<openfluid::landr::PolygonEntity*>(*it)) );
  }
  return mOrderedCompact;

}


// =====================================================================
// =====================================================================


void PolygonGraph::computeNeighboursWithBarriers(LineStringGraph& Graph,
                                                 openfluid::landr::LandRTools::Relationship Relation,
                                                 double BufferDistance,
                                                 double ContactLength)
{
  if (Relation == LandRTools::TOUCHES && ContactLength==0)
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "ContactLength must be superior to 0 for LandRTools::TOUCHES RelationShip");

  if (Relation == LandRTools::INTERSECTS)
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "LandRTools::INTERSECTS RelationShip not allowed");




  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  for (; it != ite; ++it)
    dynamic_cast<PolygonEntity*>(*it)->computeNeighboursWithBarriers(
        Graph, Relation, BufferDistance, ContactLength);
}


// =====================================================================
// =====================================================================


void PolygonGraph::mergePolygonEntitiesByMinArea(double MinArea)
{
  if (MinArea<=0.0)
    throw  openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "Threshold must be superior to 0.0");

  if (getEntities().size()==1)
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "PolygonGraph have just one PolygonEntity");


  std::multimap<double, openfluid::landr::PolygonEntity*> mOrderedArea;
  mOrderedArea=getPolygonEntitiesByMinArea(MinArea);

  if (mOrderedArea.empty())
    return ;


  while (!mOrderedArea.empty())
  {
    std::map<double, PolygonEntity*>::iterator jt=mOrderedArea.begin();

    try
    {
      PolygonEntity* EntityToMerge=(*jt).second;
      std::multimap<double, PolygonEntity*> mNeighbours;
      mNeighbours=EntityToMerge->getOrderedNeighboursByLengthBoundary();
      std::multimap<double, PolygonEntity*> ::reverse_iterator rit;
      rit=mNeighbours.rbegin();
      mergePolygonEntities(*(*rit).second,*EntityToMerge);
    }
    catch (std::exception& e)
    {
      throw openfluid::base::FrameworkException(
          OPENFLUID_CODE_LOCATION,
          "Unable to merge PolygonEntity.");
    }
    //TODO change this part to improve speed : just remove from the list the Entity and the Entity to Merge if necessary
    mOrderedArea.clear();
    mOrderedArea=getPolygonEntitiesByMinArea(MinArea);

  }

}


// =====================================================================
// =====================================================================


void PolygonGraph::mergePolygonEntitiesByCompactness(double Compactness)
{
  if (Compactness<=0.0)
    throw  openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "Threshold must be superior to 0.0");

  if (getEntities().size()==1)
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "PolygonGraph have just one PolygonEntity");


  std::multimap<double, PolygonEntity*> mOrderedCompactness;
  mOrderedCompactness=getPolygonEntitiesByCompactness(Compactness);

  if (mOrderedCompactness.empty())
    return ;

  while (!mOrderedCompactness.empty())
  {
    std::map<double, PolygonEntity*>::reverse_iterator jt=mOrderedCompactness.rbegin();

    try
    {
      PolygonEntity* EntityToMerge=(*jt).second;
      std::multimap<double, PolygonEntity*> mNeighbours;
      mNeighbours=EntityToMerge->getOrderedNeighboursByLengthBoundary();
      std::multimap<double, PolygonEntity*> ::reverse_iterator rit;
      rit=mNeighbours.rbegin();
      mergePolygonEntities(*(*rit).second,*EntityToMerge);
    }
    catch (std::exception& e)
    {
      throw openfluid::base::FrameworkException(
          OPENFLUID_CODE_LOCATION,
          "Unable to merge PolygonEntity.");
    }
    //TODO change this part to improve speed : just remove from the list the Entity and the Entity to Merge if necessary
    mOrderedCompactness.clear();
    mOrderedCompactness=getPolygonEntitiesByCompactness(Compactness);

  }

}


// =====================================================================
// =====================================================================


} }// namespace landr, openfluid
