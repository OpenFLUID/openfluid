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

// for covariant return type
#include <openfluid/landr/PolygonEntity.hpp>

namespace openfluid {
namespace landr {

class PolygonEdge;

class PolygonGraph: public LandRGraph
{
  public:

    typedef std::map<geos::geom::Polygon*, double> RastValByRastPoly_t;

  private:

    /**
     * @brief Creates a new PolygonEdge, with its two DirectedEdges and add them to this graph.
     *
     * @param LineString The LineString representing the Edge to create.
     * @return The newly created Edge, or 0 if fails.
     */
    PolygonEdge* createEdge(geos::geom::LineString& LineString);

    /**
     * @brief Removes a segment of the exterior boundary of the input Entity.
     * @param Entity The entity to removes the segment to.
     * @param Segment The LineString to remove.
     */
    void removeSegment(PolygonEntity* Entity, geos::geom::LineString* Segment);

    void doRemoveEntity(LandREntity* Entity);

    void doDeleteAll();

  public:

    PolygonGraph();

    PolygonGraph(const openfluid::core::GeoVectorValue& Val);

    /**
     * Do not copy associated raster.
     */
    PolygonGraph(PolygonGraph& Other);

    PolygonGraph(const std::vector<PolygonEntity*>& Entities);

    virtual ~PolygonGraph();

    /**
     * @brief Returns a LineString representing the linearized input Geometry.
     *
     * @param Geom The Geometry to linearize.
     * @return A new allocated LineString representing the linearized input Geometry,
     * or 0 if the Geometry cannot be linearized into a single LineString.
     */
    static geos::geom::LineString* getMergedLineStringFromGeometry(
        geos::geom::Geometry* Geom);

    /**
     * @brief Returns a vector of LineStrings representing the linearized input Geometry.
     *
     * @param Geom The Geometry to linearize.
     * @return A new allocated vector of LineStrings representing the maximal linearized input Geometry,
     * or 0 if the Geometry is not \"Line\" typed.
     */
    static std::vector<geos::geom::LineString*>* getMergedLineStringsFromGeometry(
        geos::geom::Geometry* Geom);

    /**
     * Takes ownership of Polygon and Feature
     */
    PolygonEntity* addPolygon(const geos::geom::Polygon* Polygon,
                              OGRFeature* Feat);

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
};

} // namespace landr
} /* namespace openfluid */
#endif /* POLYGONGRAPH_HPP_ */
