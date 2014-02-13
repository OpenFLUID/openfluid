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
 \file LineStringGraph.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "LineStringGraph.hpp"

#include <openfluid/landr/LineStringEntity.hpp>
#include <openfluid/landr/RasterDataset.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/landr/LandRTools.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/StringValue.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <geos/planargraph/DirectedEdge.h>
#include <geos/planargraph/Node.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/LineString.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineSegment.h>
#include <geos/geom/Point.h>
#include <algorithm>


namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

LineStringGraph::LineStringGraph() :
            LandRGraph()
{

}

// =====================================================================
// =====================================================================

LineStringGraph::LineStringGraph(openfluid::core::GeoVectorValue& Val) :
            LandRGraph(Val)
{

}

// =====================================================================
// =====================================================================

LineStringGraph::LineStringGraph(openfluid::landr::VectorDataset& Vect) :
            LandRGraph(Vect)
{

}

// =====================================================================
// =====================================================================

LineStringGraph::~LineStringGraph()
{

}

// =====================================================================
// =====================================================================

LineStringGraph* LineStringGraph::create(openfluid::core::GeoVectorValue& Val)
{
  LineStringGraph* Graph = new LineStringGraph(Val);
  Graph->addEntitiesFromGeoVector();

  return Graph;
}

// =====================================================================
// =====================================================================

LineStringGraph* LineStringGraph::create(openfluid::landr::VectorDataset& Vect)
{
  LineStringGraph* Graph = new LineStringGraph(Vect);
  Graph->addEntitiesFromGeoVector();

  return Graph;
}

// =====================================================================
// =====================================================================

LineStringGraph* LineStringGraph::create(const LandRGraph::Entities_t& Entities)
{
  LineStringGraph* Graph = new LineStringGraph();
  Graph->addEntitiesFromEntityList(Entities);

  return Graph;
}

// =====================================================================
// =====================================================================

//LineStringGraph* LineStringGraph::clone()
//{
//  if (mp_Vector)
//    return LineStringGraph::create(*mp_Vector);
//  else
//    return LineStringGraph::create(getEntities());
//}

// =====================================================================
// =====================================================================

LandRGraph::GraphType LineStringGraph::getType()
{
  return LINESTRING;
}

// =====================================================================
// =====================================================================

void LineStringGraph::addEntity(LandREntity* Entity)
{
  LineStringEntity* Edge = dynamic_cast<LineStringEntity*>(Entity);

  const geos::geom::LineString* LineString = Edge->getLine();

  geos::geom::CoordinateSequence* Coordinates =
      geos::geom::CoordinateSequence::removeRepeatedPoints(
          LineString->getCoordinatesRO());

  const geos::geom::Coordinate& StartCoordinate = Coordinates->getAt(0);
  const geos::geom::Coordinate& EndCoordinate = Coordinates->getAt(
      Coordinates->getSize() - 1);

  geos::planargraph::Node* StartNode = getNode(StartCoordinate);
  geos::planargraph::Node* EndNode = getNode(EndCoordinate);

  geos::planargraph::DirectedEdge* DirectedEdge0 =
      new geos::planargraph::DirectedEdge(StartNode, EndNode,
                                          Coordinates->getAt(1), true);

  geos::planargraph::DirectedEdge* DirectedEdge1 =
      new geos::planargraph::DirectedEdge(
          EndNode, StartNode, Coordinates->getAt(Coordinates->getSize() - 2),
          false);

  Edge->setDirectedEdges(DirectedEdge0, DirectedEdge1);

  add(Edge);

  m_EntitiesByOfldId[Edge->getOfldId()] = Edge;
  m_Entities.push_back(Edge);

  delete Coordinates;
}

// =====================================================================
// =====================================================================

LandREntity* LineStringGraph::getNewEntity(const geos::geom::Geometry* Geom,
                                           unsigned int OfldId)
{
  return new LineStringEntity(Geom, OfldId);
}

// =====================================================================
// =====================================================================

void LineStringGraph::removeEntity(int OfldId)
{
  LineStringEntity* Ent = getEntity(OfldId);

  if (!Ent)
  {
    std::ostringstream s;
    s << "No entity with id " << OfldId;
    throw openfluid::base::FrameworkException(
                                       "LineStringGraph::removeEntity",
                                       s.str());
    return;
  }

  remove(dynamic_cast<geos::planargraph::Edge*>(Ent));

  m_Entities.erase(std::find(m_Entities.begin(), m_Entities.end(), Ent));
  m_EntitiesByOfldId.erase(OfldId);

  delete Ent;

  removeUnusedNodes();
}

// =====================================================================
// =====================================================================

LineStringEntity* LineStringGraph::getEntity(int OfldId)
{
  return dynamic_cast<LineStringEntity*>(LandRGraph::getEntity(OfldId));
}

// =====================================================================
// =====================================================================

LineStringEntity* LineStringGraph::getLastLineStringEntity()
{
  std::vector<LineStringEntity*> EndEntities = getEndLineStringEntities();

  if (EndEntities.size() != 1)
    return (LineStringEntity*) 0;

  return *EndEntities.begin();
}

// =====================================================================
// =====================================================================

std::vector<LineStringEntity*> LineStringGraph::getEndLineStringEntities()
{
  std::vector<LineStringEntity*> EndEntities;

  LineStringEntity* CurrentEntity = 0;

  unsigned int iEnd=getEdges()->size();
  for (unsigned int i = 0; i < iEnd; i++)
  {
    CurrentEntity = dynamic_cast<LineStringEntity*>(getEdges()->at(i));

    if (CurrentEntity->getEndNode()->getDegree() == 1)
    {
      EndEntities.push_back(CurrentEntity);
    }
  }

  return EndEntities;
}

// =====================================================================
// =====================================================================

std::vector<LineStringEntity*> LineStringGraph::getStartLineStringEntities()
{
  std::vector<LineStringEntity*> StartEntities;

  LineStringEntity* CurrentEntity = 0;

  unsigned int iEnd=getEdges()->size();
  for (unsigned int i = 0; i < iEnd; i++)
  {
    CurrentEntity = dynamic_cast<LineStringEntity*>(getEdges()->at(i));

    if (CurrentEntity->getStartNode()->getDegree() == 1)
    {
      StartEntities.push_back(CurrentEntity);
    }
  }

  return StartEntities;
}

// =====================================================================
// =====================================================================

float* LineStringGraph::getRasterValueForEntityStartNode(
    LineStringEntity& Entity)
{
  float* Val = 0;

  if (!mp_Raster)
    throw openfluid::base::FrameworkException(
        "LineStringGraph::getRasterValueForEntityStartNode",
        "No raster associated to the LineStringGraph");
  else
  {
    Val = new float(
        mp_Raster->getValueOfCoordinate(
            Entity.getStartNode()->getCoordinate()));
  }

  return Val;
}

// =====================================================================
// =====================================================================

float* LineStringGraph::getRasterValueForEntityEndNode(LineStringEntity& Entity)
{
  float* Val = 0;

  if (!mp_Raster)
    throw openfluid::base::FrameworkException(
        "LineStringGraph::getRasterValueForEntityEndNode",
        "No raster associated to the LineStringGraph");
  else
  {
    Val = new float(
        mp_Raster->getValueOfCoordinate(Entity.getEndNode()->getCoordinate()));
  }

  return Val;
}

// =====================================================================
// =====================================================================

void LineStringGraph::setAttributeFromRasterValueAtStartNode(
    const std::string& AttributeName)
{
  addAttribute(AttributeName);
  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  for (; it != ite; ++it)
  {
    float* Val = getRasterValueForEntityStartNode(
        *dynamic_cast<LineStringEntity*>(*it));

    if (!Val)
    {
      std::ostringstream s;
      s << "No raster value for entity " << (*it)->getOfldId() << " StartNode.";

      throw openfluid::base::FrameworkException(
          "LineStringGraph::setAttributeFromRasterValueAtStartNode", s.str());
      return;
    }

    (*it)->setAttributeValue(AttributeName, new core::DoubleValue(*Val));
  }

}

// =====================================================================
// =====================================================================

void LineStringGraph::setAttributeFromRasterValueAtEndNode(
    const std::string& AttributeName)
{
  addAttribute(AttributeName);

  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  for (; it != ite; ++it)
  {
    float* Val = getRasterValueForEntityEndNode(
        *dynamic_cast<LineStringEntity*>(*it));

    if (!Val)
    {
      std::ostringstream s;
      s << "No raster value for entity " << (*it)->getOfldId() << " EndNode.";

      throw openfluid::base::FrameworkException(
          "LineStringGraph::setAttributeFromRasterValueAtEndNode", s.str());
      return;
    }

    (*it)->setAttributeValue(AttributeName, new core::DoubleValue(*Val));
  }

}

// =====================================================================
// =====================================================================

void LineStringGraph::reverseLineStringEntity(
    LineStringEntity& Entity)
{

  const geos::geom::LineString* Ent=Entity.getLine();
  geos::geom::Geometry* ReverseEnt=Ent->reverse();
  LandREntity* LandEnt = dynamic_cast<LandREntity*>(&Entity);
  int OfldId=LandEnt->getOfldId();
  removeEntity(OfldId);
  try {
    addEntity(new LineStringEntity(ReverseEnt,OfldId));
  } catch (openfluid::base::FrameworkException& e) {
    std::ostringstream s;
    s << "Reverse orientation impossible for entity" << OfldId<<" : "<<e.what() ;
    throw openfluid::base::FrameworkException(
        "LineStringGraph::reverseLineStringEntity",s.str());
  }




}

// =====================================================================
// =====================================================================

bool LineStringGraph::isLineStringGraphArborescence( )
{
  int nEntities=this->getSize();
  int nNodes=std::distance(this->nodeBegin(), this->nodeEnd());

  if(nNodes!=(nEntities+1))
    return false;

  // use DFS method to travel in the nodes, if all nodes are visited, LineStringGraph is a corrected arborescence
  // first, mark all nodes as no visited

  std::vector<geos::planargraph::Node *> vNode;

  this->getNodes(vNode);

  std::vector<geos::planargraph::Node*>::iterator it=vNode.begin();
  std::vector<geos::planargraph::Node*>::iterator ite=vNode.end();
  for (; it!=ite;it++)
      (*it)->setVisited(false);

  // mark all edges as non marked
   std::vector<geos::planargraph::Edge *> *vEdge= this->getEdges();

   std::vector<geos::planargraph::Edge*>::iterator itE=vEdge->begin();
   std::vector<geos::planargraph::Edge*>::iterator itEe=vEdge->end();
   for (; itE!=itEe;itE++)
     (*itE)->setVisited(false);



  //call DFS method on an arbitrary node
  openfluid::landr::LandRTools::markVisitedNodesUsingDFS(vNode[0]);


  //check if all node have been visited
  vNode.clear();
  this->getNodes(vNode);

  it=vNode.begin();
  ite=vNode.end();
  for (; it!=ite;it++)
  {
    if(!(*it)->isVisited())
      return false;
  }


  return true;

}

// =====================================================================
// =====================================================================

void LineStringGraph::setAttributeFromMeanRasterValues(const std::string& AttributeName)
{
  addAttribute(AttributeName);

  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  for (; it != ite; ++it)
  {
    float* EndVal = getRasterValueForEntityEndNode(
        *dynamic_cast<LineStringEntity*>(*it));

    if (!EndVal)
    {
      std::ostringstream s;
      s << "No raster value for entity " << (*it)->getOfldId() << " EndNode.";

      throw openfluid::base::FrameworkException(
          "LineStringGraph::setAttributeFromMeanRasterValues", s.str());
      return;
    }

    float* StartVal = getRasterValueForEntityStartNode(
        *dynamic_cast<LineStringEntity*>(*it));

    if (!StartVal)
    {
      std::ostringstream s;
      s << "No raster value for entity " << (*it)->getOfldId() << " StartNode.";

      throw openfluid::base::FrameworkException(
          "LineStringGraph::setAttributeFromMeanRasterValues", s.str());
      return;
    }

    float Val=(*StartVal+*EndVal)/2;
    (*it)->setAttributeValue(AttributeName, new core::DoubleValue(Val));
  }
}

// =====================================================================
// =====================================================================

void LineStringGraph::setAttributeFromVectorLocation(const std::string& AttributeName, openfluid::core::GeoVectorValue& Vector,
                                                     const std::string& Column,double Thresh)
{
  if(!Vector.isLineType())
    throw openfluid::base::FrameworkException(
        "LineStringGraph::setAttributeFromVectorLocation",
        "Vector is not a Line type");


  if(!Vector.containsField(Column))
  {
    std::ostringstream s;
    s << "Unable to find the column " << Column << " in GeoVector.";
    throw openfluid::base::FrameworkException(
        "LandRGraph::setAttributeFromVectorId", s.str());
  }

  addAttribute(AttributeName);

  setlocale(LC_NUMERIC, "C");

  OGRLayer* Layer0 = Vector.getLayer(0);
  Layer0->ResetReading();

  int columnIndex=Vector.getFieldIndex(Column);


  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();

  for (; it != ite; ++it)
  {

    OGRFeature* Feat;
    while ((Feat = Layer0->GetNextFeature()) != NULL)
    {
      OGRGeometry* OGRGeom = Feat->GetGeometryRef();

      // c++ cast doesn't work (have to use the C API instead)
      geos::geom::Geometry* GeosGeom =
          (geos::geom::Geometry*) OGRGeom->exportToGEOS();

      const  geos::geom::LineString*  Line=dynamic_cast<openfluid::landr::LineStringEntity*>(*it)->getLine();
      const geos::geom::Coordinate FirstCoord=Line->getCoordinateN(0);
      const geos::geom::Coordinate SecondCoord=Line->getCoordinateN(1);
      geos::geom::LineSegment LineSegment(FirstCoord,SecondCoord);
      geos::geom::Coordinate CoordInteriorPoint;
      LineSegment.midPoint(CoordInteriorPoint);
      geos::geom::Point* CentroLine=mp_Factory->createPoint(CoordInteriorPoint);

      if(CentroLine->isWithinDistance(GeosGeom,Thresh))
      {
        if(Vector.isFieldOfType(Column, OFTInteger))
        {
          int value=Feat->GetFieldAsInteger(columnIndex);
          (*it)->setAttributeValue(AttributeName, new openfluid::core::IntegerValue(value));
          break;
        }
        else if(Vector.isFieldOfType(Column, OFTReal))
        {
          double value=Feat->GetFieldAsDouble(columnIndex);
          (*it)->setAttributeValue(AttributeName, new openfluid::core::DoubleValue(value));
          break;
        }
        else
        {
          std::string value=Feat->GetFieldAsString(columnIndex);
          (*it)->setAttributeValue(AttributeName, new openfluid::core::StringValue(value));
          break;
        }

      }
      // destroying the feature destroys also the associated OGRGeom
      OGRFeature::DestroyFeature(Feat);
      delete GeosGeom;

    }
    Layer0->ResetReading();
  }

}

// =====================================================================
// =====================================================================

void LineStringGraph::setAttributeFromVectorLocation(const std::string& AttributeName, openfluid::landr::VectorDataset& Vector,
                                                     const std::string& Column,double Thresh)
{
  if(!Vector.isLineType())
    throw openfluid::base::FrameworkException(
        "LineStringGraph::setAttributeFromVectorLocation",
        "Vector is not a Line type");


  if(!Vector.containsField(Column))
  {
    std::ostringstream s;
    s << "Unable to find the column " << Column << " in GeoVector.";
    throw openfluid::base::FrameworkException(
        "LandRGraph::setAttributeFromVectorId", s.str());
  }

  addAttribute(AttributeName);

  setlocale(LC_NUMERIC, "C");

  OGRLayer* Layer0 = Vector.getLayer(0);
  Layer0->ResetReading();

  int columnIndex=Vector.getFieldIndex(Column);


  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();

  for (; it != ite; ++it)
  {

    OGRFeature* Feat;
    while ((Feat = Layer0->GetNextFeature()) != NULL)
    {
      OGRGeometry* OGRGeom = Feat->GetGeometryRef();

      // c++ cast doesn't work (have to use the C API instead)
      geos::geom::Geometry* GeosGeom =
          (geos::geom::Geometry*) OGRGeom->exportToGEOS();

      const  geos::geom::LineString*  Line=dynamic_cast<openfluid::landr::LineStringEntity*>(*it)->getLine();
      const geos::geom::Coordinate FirstCoord=Line->getCoordinateN(0);
      const geos::geom::Coordinate SecondCoord=Line->getCoordinateN(1);
      geos::geom::LineSegment LineSegment(FirstCoord,SecondCoord);
      geos::geom::Coordinate CoordInteriorPoint;
      LineSegment.midPoint(CoordInteriorPoint);
      geos::geom::Point* CentroLine=mp_Factory->createPoint(CoordInteriorPoint);

      if(CentroLine->isWithinDistance(GeosGeom,Thresh))
      {
        if(Vector.isFieldOfType(Column, OFTInteger))
        {
          int value=Feat->GetFieldAsInteger(columnIndex);
          (*it)->setAttributeValue(AttributeName, new openfluid::core::IntegerValue(value));
          break;
        }
        else if(Vector.isFieldOfType(Column, OFTReal))
        {
          double value=Feat->GetFieldAsDouble(columnIndex);
          (*it)->setAttributeValue(AttributeName, new openfluid::core::DoubleValue(value));
          break;
        }
        else
        {
          std::string value=Feat->GetFieldAsString(columnIndex);
          (*it)->setAttributeValue(AttributeName, new openfluid::core::StringValue(value));
          break;
        }

      }
      // destroying the feature destroys also the associated OGRGeom
      OGRFeature::DestroyFeature(Feat);
      delete GeosGeom;

    }
    Layer0->ResetReading();
  }

}

// =====================================================================
// =====================================================================

void LineStringGraph::mergeLineStringEntities(LineStringEntity& Entity, LineStringEntity& EntityToMerge)
{

  //ensure that the two LineStrings are coincident
  bool Coincident=false;
  geos::planargraph::Node* StartNode=Entity.getStartNode();
  geos::planargraph::Node* EndNode=Entity.getEndNode();

  geos::planargraph::Node* StartNode2=EntityToMerge.getStartNode();
  geos::planargraph::Node* EndNode2=EntityToMerge.getEndNode();

  if((StartNode->getCoordinate()).equals(StartNode2->getCoordinate())||
      (StartNode->getCoordinate()).equals(EndNode2->getCoordinate())||
      (EndNode->getCoordinate()).equals(StartNode2->getCoordinate())||
      (EndNode->getCoordinate()).equals(EndNode2->getCoordinate()))
    Coincident=true;

  if(!Coincident)
    throw openfluid::base::FrameworkException(
        "LineStringGraph::mergeLineStringEntities",
        "The LineStringEntities are not coincident");


  // Four possibility of coincidence
  geos::geom::CoordinateSequence *CoordsOne=0;
  geos::geom::CoordinateSequence *CoordsTwo=0;

  if((EndNode->getCoordinate()).equals(StartNode2->getCoordinate()))
  {
    CoordsOne=(Entity.getLine())->getCoordinates();
    CoordsTwo=(EntityToMerge.getLine())->getCoordinates();
    CoordsOne->add(CoordsTwo,false,true);
  }
  else if((StartNode->getCoordinate()).equals(EndNode2->getCoordinate()))
  {
    CoordsOne=(EntityToMerge.getLine())->getCoordinates();
    CoordsTwo=(Entity.getLine())->getCoordinates();
    CoordsOne->add(CoordsTwo,false,true);
  }
  else if((EndNode->getCoordinate()).equals(EndNode2->getCoordinate()))
  {
    CoordsOne=(Entity.getLine())->getCoordinates();
    CoordsTwo=(EntityToMerge.getLine())->getCoordinates();
    CoordsOne->add(CoordsTwo,false,false);
  }
  else if((StartNode->getCoordinate()).equals(StartNode2->getCoordinate()))
  {
    reverseLineStringEntity(EntityToMerge);

    CoordsOne=(EntityToMerge.getLine())->getCoordinates();
    CoordsTwo=(Entity.getLine())->getCoordinates();
    CoordsOne->add(CoordsTwo,false,true);
  }

  geos::geom::LineString * NewLine=mp_Factory->createLineString(CoordsOne);


  int OfldId=Entity.getOfldId();
  int OfldIdToMerge=EntityToMerge.getOfldId();

  try {

    openfluid::landr::LineStringEntity* Entity2 =
        new openfluid::landr::LineStringEntity(NewLine,
                                               OfldId);
    removeEntity(OfldId);
    removeEntity(OfldIdToMerge);
    addEntity(Entity2);

  } catch (openfluid::base::FrameworkException& e) {
    std::ostringstream s;
    s << "Merge operation impossible for entity" << OfldId<<" : "<<e.what() ;
    throw openfluid::base::FrameworkException(
        "LineStringGraph::mergeLineStringEntities",s.str());
  }

}

// =====================================================================
// =====================================================================

std::multimap<double,  LineStringEntity*> LineStringGraph::getLineStringEntitiesByMinLength(double MinLength,bool rmDangle)
{
  if (MinLength<=0.0)
    throw  openfluid::base::FrameworkException(
        "LineStringGraph::getLineStringEntitiesByMinLength : "
        "Threshold must be superior to 0.0");

  std::list<LandREntity*> lEntities=getOfldIdOrderedEntities();
  std::list<LandREntity*>::iterator it = lEntities.begin();
  std::list<LandREntity*>::iterator ite = lEntities.end();
  std::multimap<double, LineStringEntity*> mOrderedLength;
  for(;it!=ite;++it)
  {

    if((*it)->getLength()<MinLength)
    {

      int StartDegree=dynamic_cast<openfluid::landr::LineStringEntity*>(*it)->getStartNode()->getDegree();
      int EndDegree=dynamic_cast<openfluid::landr::LineStringEntity*>(*it)->getEndNode()->getDegree();

      //is Line between two confluences ? StartNode and EndNode are in contact with three or more Edges
      if(!(StartDegree>=3 && EndDegree>=3))
      {
        // is Line a dangle ? postulate : LineStringGraph  is not well-oriented.
        //A dangle has StartNode in contact with one Edge and EndNode with three or more Edges
        // or has EndNode in contact with one Edge and StartNode with three or more Edges
        if((StartDegree==1 && EndDegree>=3 && rmDangle==false)||(EndDegree==1 && StartDegree>=3 && rmDangle==false))
          break;
        else
          mOrderedLength.insert ( std::pair<double, LineStringEntity*>((*it)->getLength(),dynamic_cast<openfluid::landr::LineStringEntity*>(*it)) );

      }

    }
  }

  return mOrderedLength;

}

// =====================================================================
// =====================================================================

} // namespace landr
} /* namespace openfluid */

