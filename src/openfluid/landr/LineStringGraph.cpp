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
  @file LineStringGraph.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
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
#include <sstream>


namespace openfluid { namespace landr {


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
  if (!Val.isLineType())
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "GeoVectorValue is not Line type");

  LineStringGraph* Graph = new LineStringGraph(Val);
  Graph->addEntitiesFromGeoVector();

  return Graph;
}


// =====================================================================
// =====================================================================


LineStringGraph* LineStringGraph::create(openfluid::landr::VectorDataset& Vect)
{

  if (!Vect.isLineType())
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "VectorDataset is not Line type");

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


LandRGraph::GraphType LineStringGraph::getType()
{
  return LINESTRING;
}


// =====================================================================
// =====================================================================


void LineStringGraph::addEntity(LandREntity* Entity)
{
  LineStringEntity* Edge = dynamic_cast<LineStringEntity*>(Entity);

  const geos::geom::LineString* LineString = Edge->line();

  geos::geom::CoordinateSequence* Coordinates =
      geos::geom::CoordinateSequence::removeRepeatedPoints(
          LineString->getCoordinatesRO());

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

  Edge->setDirectedEdges(DirectedEdge0, DirectedEdge1);

  add(Edge);

  m_EntitiesByOfldId[Edge->getOfldId()] = Edge;
  m_Entities.push_back(Edge);

  delete Coordinates;
}


// =====================================================================
// =====================================================================


LandREntity* LineStringGraph::createNewEntity(const geos::geom::Geometry* Geom,
                                              unsigned int OfldId)
{
  return new LineStringEntity(Geom, OfldId);
}


// =====================================================================
// =====================================================================


void LineStringGraph::removeEntity(int OfldId)
{
  LineStringEntity* Ent = entity(OfldId);

  if (!Ent)
  {
    std::ostringstream s;
    s << "No entity with id " << OfldId;
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,s.str());
  }

  remove(dynamic_cast<geos::planargraph::Edge*>(Ent));

  m_Entities.erase(std::find(m_Entities.begin(), m_Entities.end(), Ent));
  m_EntitiesByOfldId.erase(OfldId);

  delete Ent;

  removeUnusedNodes();
}


// =====================================================================
// =====================================================================


LineStringEntity* LineStringGraph::entity(int OfldId)
{
  return dynamic_cast<LineStringEntity*>(LandRGraph::entity(OfldId));
}


// =====================================================================
// =====================================================================


LineStringEntity* LineStringGraph::lastLineStringEntity()
{
  std::vector<LineStringEntity*> EndEntities = getEndLineStringEntities();

  if (EndEntities.size() != 1)
    return nullptr;

  return *EndEntities.begin();
}


// =====================================================================
// =====================================================================


std::vector<LineStringEntity*> LineStringGraph::getEndLineStringEntities()
{
  std::vector<LineStringEntity*> EndEntities;

  LineStringEntity* CurrentEntity = nullptr;

  unsigned int iEnd=getEdges()->size();
  for (unsigned int i = 0; i < iEnd; i++)
  {
    CurrentEntity = dynamic_cast<LineStringEntity*>(getEdges()->at(i));

    if (CurrentEntity->endNode()->getDegree() == 1)
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

  LineStringEntity* CurrentEntity = nullptr;

  unsigned int iEnd=getEdges()->size();
  for (unsigned int i = 0; i < iEnd; i++)
  {
    CurrentEntity = dynamic_cast<LineStringEntity*>(getEdges()->at(i));

    if (CurrentEntity->startNode()->getDegree() == 1)
    {
      StartEntities.push_back(CurrentEntity);
    }
  }

  return StartEntities;
}


// =====================================================================
// =====================================================================


double LineStringGraph::getRasterValueForEntityStartNode(LineStringEntity& Entity)
{


  if (!mp_Raster)
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "No raster associated to the LineStringGraph");

  double	Val = (double)
      (mp_Raster->getValueOfCoordinate(
          Entity.startNode()->getCoordinate()));


  return Val;
}


// =====================================================================
// =====================================================================


double LineStringGraph::getRasterValueForEntityEndNode(LineStringEntity& Entity)
{


  if (!mp_Raster)
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "No raster associated to the LineStringGraph");

  double	Val = (double)
      (mp_Raster->getValueOfCoordinate(Entity.endNode()->getCoordinate()));


  return Val;
}


// =====================================================================
// =====================================================================


void LineStringGraph::setAttributeFromRasterValueAtStartNode(const std::string& AttributeName)
{
  addAttribute(AttributeName);
  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  for (; it != ite; ++it)
  {
    float* Val = new float(getRasterValueForEntityStartNode(
        *dynamic_cast<LineStringEntity*>(*it)));

    if (!Val)
    {
      std::ostringstream s;
      s << "No raster value for entity " << (*it)->getOfldId() << " StartNode.";

      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());
    }

    (*it)->setAttributeValue(AttributeName, new core::DoubleValue(*Val));
    delete Val;
  }

}


// =====================================================================
// =====================================================================


void LineStringGraph::setAttributeFromRasterValueAtEndNode(const std::string& AttributeName)
{
  addAttribute(AttributeName);

  LandRGraph::Entities_t::iterator it = m_Entities.begin();
  LandRGraph::Entities_t::iterator ite = m_Entities.end();
  for (; it != ite; ++it)
  {
    float* Val = new float (getRasterValueForEntityEndNode(
        *dynamic_cast<LineStringEntity*>(*it)));

    if (!Val)
    {
      std::ostringstream s;
      s << "No raster value for entity " << (*it)->getOfldId() << " EndNode.";

      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());
    }

    (*it)->setAttributeValue(AttributeName, new core::DoubleValue(*Val));
    delete Val;
  }

}


// =====================================================================
// =====================================================================


void LineStringGraph::reverseLineStringEntity(LineStringEntity& Entity)
{

  const geos::geom::LineString* Ent=Entity.line();
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
        OPENFLUID_CODE_LOCATION,s.str());
  }

}


// =====================================================================
// =====================================================================


bool LineStringGraph::isLineStringGraphArborescence( )
{
  int nEntities=this->getSize();
  int nNodes=std::distance(this->nodeBegin(), this->nodeEnd());

  if (nNodes!=(nEntities+1))
    return false;

  // use DFS method to travel in the nodes, if all nodes are visited, LineStringGraph is a corrected arborescence
  // first, mark all nodes as no visited

  std::vector<geos::planargraph::Node *> vNode;

  this->getNodes(vNode);

  std::vector<geos::planargraph::Node*>::iterator it=vNode.begin();
  std::vector<geos::planargraph::Node*>::iterator ite=vNode.end();
  for (; it!=ite;++it)
      (*it)->setVisited(false);

  // mark all edges as non marked
   std::vector<geos::planargraph::Edge *> *vEdge= this->getEdges();

   std::vector<geos::planargraph::Edge*>::iterator itE=vEdge->begin();
   std::vector<geos::planargraph::Edge*>::iterator itEe=vEdge->end();
   for (; itE!=itEe;++itE)
     (*itE)->setVisited(false);



  //call DFS method on an arbitrary node
  openfluid::landr::LandRTools::markVisitedNodesUsingDFS(vNode[0]);


  //check if all node have been visited
  vNode.clear();
  this->getNodes(vNode);

  it=vNode.begin();
  ite=vNode.end();
  for (; it!=ite;++it)
  {
    if (!(*it)->isVisited())
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
    float* EndVal = new float(getRasterValueForEntityEndNode(
        *dynamic_cast<LineStringEntity*>(*it)));

    if (!EndVal)
    {
      std::ostringstream s;
      s << "No raster value for entity " << (*it)->getOfldId() << " EndNode.";

      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());
    }

    float* StartVal = new float( getRasterValueForEntityStartNode(
        *dynamic_cast<LineStringEntity*>(*it)));

    if (!StartVal)
    {
      std::ostringstream s;
      s << "No raster value for entity " << (*it)->getOfldId() << " StartNode.";

      if (EndVal)
        delete EndVal;

      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s.str());

      return;
    }

    float Val=(*StartVal+*EndVal)/2;
    (*it)->setAttributeValue(AttributeName, new core::DoubleValue(Val));

    delete EndVal;
    delete StartVal;

  }
}


// =====================================================================
// =====================================================================


void LineStringGraph::mergeLineStringEntities(LineStringEntity& Entity,
                                              LineStringEntity& EntityToMerge)
{

  //ensure that the two LineStrings are coincident
  bool Coincident=false;
  geos::planargraph::Node* StartNode=Entity.startNode();
  geos::planargraph::Node* EndNode=Entity.endNode();

  geos::planargraph::Node* StartNode2=EntityToMerge.startNode();
  geos::planargraph::Node* EndNode2=EntityToMerge.endNode();

  if ((StartNode->getCoordinate()).equals(StartNode2->getCoordinate())||
      (StartNode->getCoordinate()).equals(EndNode2->getCoordinate())||
      (EndNode->getCoordinate()).equals(StartNode2->getCoordinate())||
      (EndNode->getCoordinate()).equals(EndNode2->getCoordinate()))
    Coincident=true;

  if (!Coincident)
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "The LineStringEntities are not coincident");


  // Four possibility of coincidence
  geos::geom::CoordinateSequence *CoordsOne = nullptr;
  geos::geom::CoordinateSequence *CoordsTwo = nullptr;

  if ((EndNode->getCoordinate()).equals(StartNode2->getCoordinate()))
  {
    CoordsOne=(Entity.line())->getCoordinates();
    CoordsTwo=(EntityToMerge.line())->getCoordinates();
    CoordsOne->add(CoordsTwo,false,true);
  }
  else if ((StartNode->getCoordinate()).equals(EndNode2->getCoordinate()))
  {
    CoordsOne=(EntityToMerge.line())->getCoordinates();
    CoordsTwo=(Entity.line())->getCoordinates();
    CoordsOne->add(CoordsTwo,false,true);
  }
  else if ((EndNode->getCoordinate()).equals(EndNode2->getCoordinate()))
  {
    CoordsOne=(Entity.line())->getCoordinates();
    CoordsTwo=(EntityToMerge.line())->getCoordinates();
    CoordsOne->add(CoordsTwo,false,false);
  }
  else if ((StartNode->getCoordinate()).equals(StartNode2->getCoordinate()))
  {
    reverseLineStringEntity(EntityToMerge);

    CoordsOne=(EntityToMerge.line())->getCoordinates();
    CoordsTwo=(Entity.line())->getCoordinates();
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
        OPENFLUID_CODE_LOCATION,s.str());
  }

}


// =====================================================================
// =====================================================================


std::multimap<double,  LineStringEntity*> LineStringGraph::getLineStringEntitiesByMinLength(double MinLength,
                                                                                            bool rmDangle,
                                                                                            bool HighDegree)
{
  if (MinLength<=0.0)
    throw  openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "Threshold must be superior to 0.0");

  std::list<LandREntity*> lEntities=getOfldIdOrderedEntities();
  std::list<LandREntity*>::iterator it = lEntities.begin();
  std::list<LandREntity*>::iterator ite = lEntities.end();
  std::multimap<double, LineStringEntity*> mOrderedLength;
  for (;it!=ite;++it)
  {

    if ((*it)->getLength()<MinLength)
    {

      int StartDegree=dynamic_cast<openfluid::landr::LineStringEntity*>(*it)->startNode()->getDegree();
      int EndDegree=dynamic_cast<openfluid::landr::LineStringEntity*>(*it)->endNode()->getDegree();
      bool LineCounted=true;
      //is Line between two confluences ? StartNode and EndNode are in contact with three or more Edges
      if(HighDegree &&(StartDegree>=3 && EndDegree>=3))
        LineCounted=false;
      if(LineCounted)
      {
        // is Line a dangle ? postulate : LineStringGraph  is not well-oriented.
        //A dangle has StartNode in contact with one Edge and EndNode with three or more Edges
        // or has EndNode in contact with one Edge and StartNode with three or more Edges
        if (!((StartDegree==1 && EndDegree>=3 && rmDangle==false)||(EndDegree==1 && StartDegree>=3 && rmDangle==false)))
          mOrderedLength.insert ( std::pair<double, LineStringEntity*>((*it)->getLength(),
                                  dynamic_cast<openfluid::landr::LineStringEntity*>(*it)) );

      }

    }
  }

  return mOrderedLength;

}


// =====================================================================
// =====================================================================


void LineStringGraph::setOrientationByOfldId(int OfldId)
{

  if (!this->isLineStringGraphArborescence())
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "The LineStringGraph is not a correct arborescence.");

  // get the node of this edge
  openfluid::landr::LineStringEntity* lineEntity=this->entity(OfldId);

  if(!lineEntity)
  {
    std::ostringstream s;
    s << "Entity " << OfldId<< " is not a correct Line  entity.";

    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION
        +s.str());
  }

  if (lineEntity->startNode()->getDegree()==1)
    this->reverseLineStringEntity(*lineEntity);    // reverse the outlet if necessary

  // mark all nodes as non marked

  geos::planargraph::PlanarGraph *planGraph=dynamic_cast<geos::planargraph::PlanarGraph*>(this);;
  std::vector<geos::planargraph::Node *> vNode;
  planGraph->getNodes(vNode);

  std::vector<geos::planargraph::Node*>::iterator it=vNode.begin();
  std::vector<geos::planargraph::Node*>::iterator ite=vNode.end();
  for (; it!=ite;++it)
    (*it)->setVisited(false);

  // mark all edges as non marked
  std::vector<geos::planargraph::Edge *> *vEdge= planGraph->getEdges();

  std::vector<geos::planargraph::Edge*>::iterator itEdge=vEdge->begin();
  std::vector<geos::planargraph::Edge*>::iterator itEdgeE=vEdge->end();
  for (; itEdge!=itEdgeE;++itEdge)
    (*itEdge)->setVisited(false);



  lineEntity=this->entity(OfldId);
  geos::planargraph::Node * firstNode=lineEntity->endNode();
  std::vector<int> vectIdent;

  openfluid::landr::LandRTools::markInvertedLineStringEntityUsingDFS(firstNode,vectIdent);

  // mark all node as non marked
  planGraph=dynamic_cast<geos::planargraph::PlanarGraph*>(this);;
  vNode.clear();
  planGraph->getNodes(vNode);

  it=vNode.begin();
  ite=vNode.end();
  for (; it!=ite;++it)
    (*it)->setVisited(false);

  // mark all edges as non marked
  vEdge->clear();
  vEdge= planGraph->getEdges();

  itEdge=vEdge->begin();
  itEdgeE=vEdge->end();
  for (; itEdge!=itEdgeE;++itEdge)
    (*itEdge)->setVisited(false);


  // reverse the lineStringEntity
  std::vector<int>::iterator itV=vectIdent.begin();
  std::vector<int>::iterator itVe=vectIdent.end();
  for (; itV!=itVe;++itV)
    this->reverseLineStringEntity(*(this->entity(*itV)));

}


// =====================================================================
// =====================================================================


void LineStringGraph::mergeLineStringEntitiesByMinLength(double MinLength,bool rmDangle)
{

  if (MinLength<=0.0)
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "Threshold must be superior to 0.0");

  if (getEntities().size()==1)
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        "RSGRaph have just one RS Entity");

  std::multimap<double, LineStringEntity*>  mOrderedLength;
  mOrderedLength=getLineStringEntitiesByMinLength(MinLength,rmDangle);

  if (mOrderedLength.empty())
    return ;


  while (!mOrderedLength.empty())
  {
    std::multimap<double, LineStringEntity*>::iterator jt=mOrderedLength.begin();
    LineStringEntity* EntityToMerge=(*jt).second;
    std::vector<LineStringEntity*> vNeighbours=EntityToMerge->getLineNeighboursDegree2();

    int StartDegree=EntityToMerge->startNode()->getDegree();
    int EndDegree=EntityToMerge->endNode()->getDegree();

    if (StartDegree==1 && EndDegree>=3 && rmDangle==true)
      removeEntity(EntityToMerge->getOfldId());

    else if (!vNeighbours.empty())
    {
      std::vector<LineStringEntity*>::iterator it2 = vNeighbours.begin();
      std::vector<LineStringEntity*>::iterator it2e = vNeighbours.end();
      std::multimap<double, LandREntity*> mLengthNeighbours;

      for (;it2!=it2e;++it2)
      {
        LandREntity* Line =
            dynamic_cast<LineStringEntity*>(*it2);
        mLengthNeighbours.insert ( std::pair<double, LandREntity*>(Line->getLength(),Line) );
      }
      std::multimap<double, LandREntity*>::iterator itMapEnd=mLengthNeighbours.end();
      --itMapEnd;

      LineStringEntity* Entity;
      Entity=dynamic_cast<LineStringEntity*>((*itMapEnd).second);

      try
      {
        mergeLineStringEntities(*Entity,*EntityToMerge);
      }
      catch (std::exception& e)
      {
        throw openfluid::base::FrameworkException(
            OPENFLUID_CODE_LOCATION,
            "Unable to merge LineStringEntity.");
      }

    }
    //TODO change this part to improve speed : just remove from the list the Entity and the Entity to Merge if necessary
    mOrderedLength.clear();
    mOrderedLength=getLineStringEntitiesByMinLength(MinLength,rmDangle);

  }

}


// =====================================================================
// =====================================================================


} } // namespace landr, openfluid

