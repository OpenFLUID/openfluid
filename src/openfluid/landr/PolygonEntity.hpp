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

    const geos::geom::Polygon* mp_Polygon;

    PolygonEntity();
    PolygonEntity(const PolygonEntity&);

  public:

    typedef std::map<PolygonEntity*, std::vector<PolygonEdge*> > NeighboursMap_t;
    typedef std::map<LineStringEntity*, PolygonEdge*> LineStringNeighboursMap_t;

    /**
     * @brief Map of neighbours of PolygonEntity type and the related vector of edges that are between this Polygon and the neighbour.
     */
    NeighboursMap_t* mp_NeighboursMap;

    /**
     * @brief Map of neighbours of LineStringEntity type and the related edge that is between this Polygon and the neighbour, if exists.
     */
    LineStringNeighboursMap_t* mp_LineStringNeighboursMap;

    /**
     * @brief PolygonEdges of this PolygonEntity.
     */
    std::vector<PolygonEdge*> m_PolyEdges;

    /**
     * @brief Create a new PolygonEntity.
     * @details Takes ownership of NewPolygon.
     *
     *  @throw base::OFException if NewPolygon is not a geos::geom::Polygon or is not a valid geometry.
     */
    PolygonEntity(const geos::geom::Geometry* NewPolygon, unsigned int SelfId);

    virtual ~PolygonEntity();

    /**
     * @attention Doesn't deep-copy m_PolyEdges nor neighbours.
     */
    PolygonEntity* clone();

    const geos::geom::Polygon* getPolygon() const;

    void addEdge(PolygonEdge& Edge);

    /**
     * @attention Also delete input parameter Edge.
     */
    void removeEdge(PolygonEdge* Edge);

    /**
     * @brief Returns a vector of linear intersections between two Polygons.
     *
     * @param Other The Polygon Entity to compare to.
     * @return A vector of new allocated LineStrings representing the linear intersections (eventually merged) between this Polygon Entity and Other.
     */
    std::vector<geos::geom::LineString*> getLineIntersectionsWith(
        PolygonEntity& Other);

    /**
     * @brief Returns the Edge containing Segment
     *
     * @param Segment The LineString to find.
     * @return The PolygonEdge of this PolygonEntity containing the input LineString,
     * or 0 if not found.
     */
    PolygonEdge* findEdgeLineIntersectingWith(geos::geom::LineString& Segment);

    /**
     * @brief Return a map of this PolygonEntity neighbours with for each a vector of the shared PolygoneEdges.
     * @return
     */
    const NeighboursMap_t* getNeighboursAndEdges();

    /**
     * @brief Return the SELF_IDs of this PolygonEntity neighbours, ascending ordered.
     */
    std::vector<int> getOrderedNeighbourSelfIds();

    /**
     * @brief Check if this Entity is complete, that is if all edges of this Entity,
     * merged in a LineString, equals this Entity polygon exterior ring.
     *
     * @return True if complete, false otherwise.
     */
    bool isComplete();

    /**
     * @brief Get the PolygonEdges of this PolygonEntity that are shared with Other.
     */
    std::vector<PolygonEdge*> getCommonEdgesWith(PolygonEntity& Other);

    /**
     * @brief Get the boundary of this PolygonEntity polygon, with a buffer of BufferDistance.
     */
    geos::geom::Geometry* getBufferedBoundary(double BufferDistance);

    /**
     * @brief Compute neighbours of this PolygonEntity.
     * @details A neighbour is another PolygonEntity that shares at least a PolygonEdge with this PolygonEntity.
     */
    void computeNeighbours();

    /**
     * @brief Compute the relations between this PolygonEntity and LineStringEntities of input LineStringGraph.
     * @details A LineString is considered as a neighbour if it lies within the buffer of this PolygonEntitys polygon boundary.
     *
     * @param Graph The LineStringGraph to compare to.
     * @param Relation The Relationship to use for comparison.
     * @param BufferDistance The distance below which we consider that two elements are related.
     */
    void computeLineStringNeighbours(LineStringGraph& Graph,
                                     LandRTools::Relationship Relation,
                                     double BufferDistance);

    /**
     * @brief Return the LineStringEntities neighbours of this PolygonEntity.
     */
    LineStringNeighboursMap_t* getLineStringNeighbours();


    /**
     * @brief Merge a PolygonEdge into an other one
     *
     * @param Edge An existent PolygonEdge.
     * @param EdgeToMerge Another PolygonEdge to merge.
     * @return A geos::geom:LineString which have the geometry of the merged PolygonEdges.
     */
    geos::geom::LineString*  mergeEdges(PolygonEdge* Edge, PolygonEdge* EdgeToMerge);




};

} // namespace landr
} /* namespace openfluid */
#endif /* POLYGONENTITY_HPP_ */
