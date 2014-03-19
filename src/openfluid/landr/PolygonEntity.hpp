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
 \file PolygonEntity.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef POLYGONENTITY_HPP_
#define POLYGONENTITY_HPP_

#include <openfluid/landr/LandREntity.hpp>
#include <openfluid/landr/LandRTools.hpp>
#include <openfluid/dllexport.hpp>
#include <vector>

namespace geos {
namespace geom {
class Polygon;
class LineString;
}
}

namespace openfluid {
namespace landr {

class PolygonEdge;
class LineStringGraph;
class LineStringEntity;

/**
 * @brief A LandREntity representing a geos::geom::Polygon.
 * @details A PolygonEntity has at least a PolygonEdge, all edges representing the Polygon exterior ring.
 */
class DLLEXPORT PolygonEntity: public LandREntity
{
  private:

    /**
     * @brief The geos::geom::Polygon associated to this PolygonEntity.
     */
    const geos::geom::Polygon* mp_Polygon;

    PolygonEntity();
    PolygonEntity(const PolygonEntity&);

  public:

    /**
     * @brief A map of the PolygonEntity neighbours and their shared PolygonEdge to this PolygonEntity.
     */
    typedef std::map<PolygonEntity*, std::vector<PolygonEdge*> > NeighboursMap_t;

    /**
     * @brief A map of the LineStringEntity neighbours and the PolygonEdge in contact with this PolygonEntity.
     */
    typedef std::map<LineStringEntity*, PolygonEdge*> LineStringNeighboursMap_t;

    /**
     * @brief A Map of neighbours of PolygonEntity type and the related vector of PolygonEdge that are between this PolygonEntity and his neighbours.
     */
    NeighboursMap_t* mp_NeighboursMap;

    /**
     * @brief A Map of neighbours of LineStringEntity type and the related PolygonEdge that is between this PolygonEntity and his neighbours, if exist.
     */
    LineStringNeighboursMap_t* mp_LineStringNeighboursMap;

    /**
     * @brief A vector of the PolygonEdge of this PolygonEntity.
     */
    std::vector<PolygonEdge*> m_PolyEdges;

    /**
     * @brief Create a new PolygonEntity.
     * @details Takes ownership of NewPolygon.
     * @param NewPolygon The geos::geom::Geometry of this new PolygonEntity.
     * @param OfldId The identifier of this new PolygonEntity.
     *
     *  @throw base::OFException if NewPolygon is not a geos::geom::Polygon or is not a valid geometry.
     */
    PolygonEntity(const geos::geom::Geometry* NewPolygon, unsigned int OfldId);

    virtual ~PolygonEntity();

    /**
     * @brief Clone a new PolygonEntity from this PolygonEntity.
     * @attention Doesn't deep-copy m_PolyEdges nor neighbours.
     */
    PolygonEntity* clone();

    /**
     * @brief Returns the geos::geom::Polygon associated to this PolygonEntity.
     */
    const geos::geom::Polygon* getPolygon() const;

    /**
     * @brief Adds a PolygonEdge to this PolygonEntity.
     */
    void addEdge(PolygonEdge& Edge);

    /**
     * @brief Removes a PolygonEdge to this PolygonEntity.
     * @attention Also delete input parameter Edge.
     */
    void removeEdge(PolygonEdge* Edge);

    /**
     * @brief Returns a vector of geos::geom::LineString representing the linear intersections between two PolygonEntity.
     *
     * @param Other The PolygonEntity to compare to.
     * @return A vector of new allocated geos::geom::LineString representing the linear intersections (eventually merged) between this PolygonEntity and Other.
     */
    std::vector<geos::geom::LineString*> getLineIntersectionsWith(
        PolygonEntity& Other);

    /**
     * @brief Returns the PolygonEdge containing Segment.
     *
     * @param Segment The geos::geom::LineString to find.
     * @return The PolygonEdge of this PolygonEntity containing the input geos::geom::LineString,
     * or 0 if not found.
     */
    PolygonEdge* findEdgeLineIntersectingWith(geos::geom::LineString& Segment);

    /**
     * @brief Returns a map of this PolygonEntity neighbours with for each a vector of the shared PolygonEdge.
     */
    const NeighboursMap_t* getNeighboursAndEdges();

    /**
     * @brief Returns a vector of the OFLD_ID of this PolygonEntity neighbours, ascending ordered.
     */
    std::vector<int> getOrderedNeighbourOfldIds();

    /**
     * @brief Check if this PolygonEntity is complete, that is if all PolygonEdge of this PolygonEntity,
     * merged in a LineString, equals this PolygonEntity polygon exterior ring.
     *
     * @return True if complete, false otherwise.
     */
    bool isComplete();

    /**
     * @brief Gets the PolygonEdge of this PolygonEntity that are shared with Other.
     * @param Other A PolygonEntity.
     * @return A vector of PolygonEdge.
     */
    std::vector<PolygonEdge*> getCommonEdgesWith(PolygonEntity& Other);

    /**
     * @brief Gets the boundary of this PolygonEntity polygon, with a buffer of BufferDistance.
     * @param BufferDistance The buffer distance.
     * @return A geos::geom::Geometry representing the buffered boundaries of this PolygonEntity.
     */
    geos::geom::Geometry* getBufferedBoundary(double BufferDistance);

    /**
     * @brief Computes the neighbours of this PolygonEntity.
     * @details A neighbour is another PolygonEntity that shares at least a PolygonEdge with this PolygonEntity.
     */
    void computeNeighbours();

    /**
     * @brief Computes the relations between this PolygonEntity and the LineStringEntity of an input LineStringGraph.
     * @details A LineStringEntity is considered as a neighbour if it lies within the buffer of this PolygonEntity polygon boundary.
     *
     * @param Graph The LineStringGraph to compare to.
     * @param Relation The Relationship to use for comparison.
     * @param BufferDistance The distance below which we consider that two elements are related.
     * @param ContactLength Min Length of the LineString in intersection with polygon Buffered Boundaries to be taking acccount (only for LandRTools::TOUCHES RelationShip)
     */
    void computeLineStringNeighbours(LineStringGraph& Graph,
                                     LandRTools::Relationship Relation,
                                     double BufferDistance,double ContactLength=0);

    /**
     * @brief Return the a map of the LineStringEntity neighbours of this PolygonEntity.
     */
    LineStringNeighboursMap_t* getLineStringNeighbours();


    /**
     * @brief Merge a PolygonEdge into an other one.
     *
     * @param Edge An existent PolygonEdge.
     * @param EdgeToMerge Another PolygonEdge to merge.
     * @return A geos::geom:LineString which have the geometry of the merged PolygonEdge.
     */
    geos::geom::LineString*  mergeEdges(PolygonEdge* Edge, PolygonEdge* EdgeToMerge);




};

} // namespace landr
} /* namespace openfluid */
#endif /* POLYGONENTITY_HPP_ */
