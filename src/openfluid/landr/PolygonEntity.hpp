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
  @file PolygonEntity.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
 */

#ifndef __OPENFLUID_LANDR_POLYGONENTITY_HPP__
#define __OPENFLUID_LANDR_POLYGONENTITY_HPP__

#include <openfluid/landr/LandREntity.hpp>
#include <openfluid/landr/LandRTools.hpp>
#include <openfluid/landr/VectorDataset.hpp>
#include <openfluid/dllexport.hpp>
#include <vector>

namespace geos { namespace geom {
class Polygon;
class LineString;
} }

namespace openfluid { namespace landr {

class PolygonEdge;
class LineStringGraph;
class LineStringEntity;


/**
  @brief A LandREntity representing a geos::geom::Polygon.
  @details A PolygonEntity has at least a PolygonEdge, all edges representing the Polygon exterior ring.
*/
class OPENFLUID_API PolygonEntity: public LandREntity
{
  private:

    /**
      @brief The geos::geom::Polygon associated to this PolygonEntity.
    */
    const geos::geom::Polygon* mp_Polygon;

    PolygonEntity();
    PolygonEntity(const PolygonEntity&);

  public:

    /**
      @brief A map of the PolygonEntity neighbours and their shared PolygonEdge to this PolygonEntity.
    */
    typedef std::map<PolygonEntity*, std::vector<PolygonEdge*> > NeighboursMap_t;

    /**
      @brief A map of the LineStringEntity neighbours and the PolygonEdge in contact with this PolygonEntity.
    */
    typedef std::map<LineStringEntity*, PolygonEdge*> LineStringNeighboursMap_t;

    /**
      @brief A Map of neighbours of PolygonEntity type and the related vector of PolygonEdge
      that are between this PolygonEntity and his neighbours.
    */
    NeighboursMap_t* mp_NeighboursMap;

    /**
      @brief A Map of neighbours of LineStringEntity type and the related PolygonEdge
      that is between this PolygonEntity and his neighbours, if exist.
    */
    LineStringNeighboursMap_t* mp_LineStringNeighboursMap;

    /**
      @brief A vector of the PolygonEdge of this PolygonEntity.
    */
    std::vector<PolygonEdge*> m_PolyEdges;

    /**
      @brief Create a new PolygonEntity.
      @details Takes ownership of NewPolygon.
      @param NewPolygon The geos::geom::Geometry of this new PolygonEntity.
      @param OfldId The identifier of this new PolygonEntity.
      @throw base::FrameworkException if NewPolygon is not a geos::geom::Polygon or is not a valid geometry.
    */
    PolygonEntity(const geos::geom::Geometry* NewPolygon, unsigned int OfldId);

    virtual ~PolygonEntity();

    /**
      @brief Clone a new PolygonEntity from this PolygonEntity.
      @attention Doesn't deep-copy m_PolyEdges nor neighbours.
    */
    PolygonEntity* clone();

    /**
      @brief Returns the geos::geom::Polygon associated to this PolygonEntity.
    */
    const geos::geom::Polygon* polygon() const;

    /**
      @brief Adds a PolygonEdge to this PolygonEntity.
    */
    void addEdge(PolygonEdge& Edge);

    /**
      @brief Removes a PolygonEdge to this PolygonEntity.
      @attention Also delete input parameter Edge.
    */
    void removeEdge(PolygonEdge* Edge);

    /**
      @brief Returns a vector of geos::geom::LineString representing the linear intersections between two PolygonEntity.
      @param Other The PolygonEntity to compare to.
      @return A vector of new allocated geos::geom::LineString representing the linear
      intersections (eventually merged) between this PolygonEntity and Other.
    */
    std::vector<geos::geom::LineString*> computeLineIntersectionsWith(PolygonEntity& Other);

    /**
      @brief Returns the PolygonEdge containing Segment.
      @param Segment The geos::geom::LineString to find.
      @return The PolygonEdge of this PolygonEntity containing the input geos::geom::LineString,
      or 0 if not found.
    */
    PolygonEdge* findEdgeLineIntersectingWith(geos::geom::LineString& Segment);

    /**
      @brief Returns a map of this PolygonEntity neighbours with for each a vector of the shared PolygonEdge.
    */
    const NeighboursMap_t* neighboursAndEdges();

    /**
      @brief Returns a vector of the OFLD_ID of this PolygonEntity neighbours, ascending ordered.
    */
    std::vector<int> getOrderedNeighbourOfldIds();

    /**
      @brief Returns a multimap of the length of the shared boundary of each
      neighbour of this PolygonEntity and each PolygonEntity neighbour,
      ascending ordered by length shared boundary (shortest to longest boundary).
    */
    std::multimap<double,PolygonEntity*> getOrderedNeighboursByLengthBoundary();

    /**
      @brief Check if this PolygonEntity is complete, that is if all PolygonEdge of this PolygonEntity,
      merged in a LineString, equals this PolygonEntity polygon exterior ring.
      @return True if complete, false otherwise.
    */
    bool isComplete();

    /**
      @brief Gets the PolygonEdge of this PolygonEntity that are shared with Other.
      @param Other A PolygonEntity.
      @return A vector of PolygonEdge.
    */
    std::vector<PolygonEdge*> getCommonEdgesWith(PolygonEntity& Other);

    /**
      @brief Gets the PolygonEntity which share the same Edge with the current PolygonEntity.
      @param Edge A PolygonEdge.
      @return A vector a PolygonEntity.
    */
    PolygonEntity * neighbourWithCommonEdge(PolygonEdge * Edge);

    /**
      @brief Gets the boundary of this PolygonEntity polygon, with a buffer of BufferDistance.
      @param BufferDistance The buffer distance.
      @return A geos::geom::Geometry representing the buffered boundaries of this PolygonEntity.
    */
    geos::geom::Geometry* getBufferedBoundary(double BufferDistance);

    /**
      @brief Computes the neighbours of this PolygonEntity.
      @details A neighbour is another PolygonEntity that shares at least a PolygonEdge with this PolygonEntity.
    */
    void computeNeighbours();

    /**
      @brief Computes the relations between this PolygonEntity and the LineStringEntity of an input LineStringGraph.
      @details A LineStringEntity is considered as a neighbour if it lies within the buffer
      of this PolygonEntity polygon boundary.
      @param Graph The LineStringGraph to compare to.
      @param Relation The Relationship to use for comparison.
      @param BufferDistance The distance below which we consider that two elements are related.
      @param ContactLength Min Length of the LineString in intersection with polygon
      Buffered Boundaries to be taking acccount (only for LandRTools::TOUCHES RelationShip)
    */
    void computeLineStringNeighbours(LineStringGraph& Graph,
                                     LandRTools::Relationship Relation,
                                     double BufferDistance,
                                     double ContactLength=0);

    /**
      @brief Computes the relations between this PolygonEntity and its PolygonEntities Neighbours by using
      the LineStringEntity of an input LineStringGraph which are considered as barriers.
      @details A barrier between two PolygonEntity will avoid to considered them as neighbours.
      @details A LineStringEntity is considered as a barrier if it lies within the buffer
      of this PolygonEntity polygon boundary.
      @param Graph The LineStringGraph to compare to.
      @param Relation The Relationship to use for comparison, the LandRTools::Relationship INTERSECTS is not allowed.
      @param BufferDistance The distance below which we consider that two elements are related.
      @param ContactLength Min Length of the LineString in intersection with polygon
      Buffered Boundaries to be taking acccount (only for LandRTools::TOUCHES RelationShip)
    */
    void computeNeighboursWithBarriers(LineStringGraph& Graph,
                                       LandRTools::Relationship Relation,
                                       double BufferDistance,
                                       double ContactLength=0);

    /**
      @brief Return the a map of the LineStringEntity neighbours of this PolygonEntity.
    */
    LineStringNeighboursMap_t* lineStringNeighbours();

    /**
      @brief Merge a PolygonEdge into an other one.
      @param Edge An existent PolygonEdge.
      @param EdgeToMerge Another PolygonEdge to merge.
      @return A geos::geom:LineString which have the geometry of the merged PolygonEdge.
    */
    geos::geom::LineString*  mergeEdges(PolygonEdge* Edge,
                                        PolygonEdge* EdgeToMerge);

    /**
      @brief Find the LandREntity neighbour of this PolygonEntity by using
      a line VectorDataset which indicates the neighbour relationship
      \verbatim
         **************    Following the directions of the
         *   |-*-->   *    lines of the VectorDataset,
         *  1  *   3  *    the neighbour of PolygonEntity 2 is
         *     *      *    the PolygonEntity 1 and the neighbour
         *  ^  ********    of PolygonEntity 1 is the PolygonEntity
         *  |  *           3. If a line of the VectorDataset crosses
         *******           a LineStringNeighbour of the PolygonEntity,
         *  |  *           it becomes the neighbour :e.g, if LineStringNeighbour
         *  -  *           exists between PolygonEntity 1 and 2, the neighbour
         *  2  *           of PolygonEntity becomes this LineStringNeighbour.
         *     *
         *******
      \endverbatim
      @param LineTopology A line VectorDataset
      @return A pair of openfluid::landr:landREntity and the length of the line of the VectorDataset
      or an empty pair if not found.
    */
   std::pair< LandREntity*, double> computeNeighbourByLineTopology(VectorDataset LineTopology);



};

} } // namespace landr, openfluid
#endif /* __OPENFLUID_LANDR_POLYGONENTITY_HPP__ */
