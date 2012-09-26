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
 \file PolygonGraph.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef POLYGONGRAPH_HPP_
#define POLYGONGRAPH_HPP_

#include <openfluid/landr/LandRGraph.hpp>

// for covariant return type of getEntity
#include <openfluid/landr/PolygonEntity.hpp>

namespace openfluid {
namespace landr {

/**
 * @brief A LandRGraph composed of PolygonEntities.
 */
class PolygonGraph: public LandRGraph
{
  public:

    typedef std::map<geos::geom::Polygon*, double> RastValByRastPoly_t;

  private:

    PolygonGraph(PolygonGraph& Other);

  protected:

    PolygonGraph();

    PolygonGraph(openfluid::core::GeoVectorValue& Val);

    virtual void addEntity(LandREntity* Entity);

    virtual LandREntity* getNewEntity(const geos::geom::Geometry* Geom,
                                      unsigned int SelfId);

    /**
     * @brief Creates a new PolygonEdge, with its two DirectedEdges and add them to this graph.
     *
     * @param LineString The LineString representing the Edge to create.
     * @return The newly created Edge, or 0 if fails.
     */
    PolygonEdge* createEdge(geos::geom::LineString& LineString);

    /**
     * @brief Removes a segment of the exterior boundary of the input Entity.
     *
     * @param Entity The entity to removes the segment to.
     * @param Segment The LineString to remove.
     */
    void removeSegment(PolygonEntity* Entity, geos::geom::LineString* Segment);

  public:

    /**
     * @brief Create a new graph initialized with Val elements.
     * @details Val must be composed of one or many Polygons, and each of them must contain a "SELF_ID" attribute.
     */
    static PolygonGraph* create(openfluid::core::GeoVectorValue& Val);

    /**
     * @brief Create a new graph initialized with Entities.
     * @details Entities must be PolygonEntities.
     */
    static PolygonGraph* create(const LandRGraph::Entities_t& Entities);

    virtual ~PolygonGraph();

    /**
     * @attention Do not copy associated raster.
     */
    PolygonGraph* clone();

    LandRGraph::GraphType getType();

    /**
     * @see LandRGraph::getEntity;
     */
    PolygonEntity* getEntity(int SelfId);

    /**
     * @brief Check if each entity is complete.
     *
     * @return True if all entities of this graph are complete, false otherwise.
     */
    bool isComplete();

    /**
     * Get a map of polygonized Raster polygons and its area intersecting Entity.
     *
     * @param Entity The Entity to compare with the associated Raster.
     *
     * @return A map of polygonized Raster Polygons, from associated polygonized raster,
     * with for each one the intersection area.
     */
    RastValByRastPoly_t getRasterPolyOverlapping(PolygonEntity& Entity);

    /**
     * @brief Create a new attribute for this Graph entities, and set for each entity
     * this attribute value as the mean of the overlapping raster values, relative to overlapping areas.
     * Beware that this uses GeoRasterValue::polygonize function, which currently only deal with integer values.
     * So raster float pixel values are rounded before computing.
     *
     * @param AttributeName The name of the attribute to create
     */
    void setAttributeFromMeanRasterValues(std::string AttributeName);

    /**
     * @brief Create on disk, in the project out directory, a shapefile representing this Graph.
     *
     * @param FilePath The path where to create the out file.
     * @param FileName A Name for the out file to create, with a .shp extension.
     */
    void createVectorRepresentation(std::string FilePath, std::string FileName);

    /**
     * @brief Compute neighbours between PolygonEntities elements of this graph and LineStringEntities of Graph.
     *
     * @param Graph The LineStringGraph to compare to.
     * @param Relation The Relationship to use for comparison.
     * @param BufferDistance The distance below which we consider that two elements are related.
     */
    void computeLineStringNeighbours(
        LineStringGraph& Graph,
        openfluid::landr::LandRTools::Relationship Relation,
        double BufferDistance);
};

} // namespace landr
} /* namespace openfluid */
#endif /* POLYGONGRAPH_HPP_ */
