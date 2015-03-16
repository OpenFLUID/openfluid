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
  @file LandRGraph.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
 */

#ifndef __OPENFLUID_LANDR_LANDRGRAPH_HPP__
#define __OPENFLUID_LANDR_LANDRGRAPH_HPP__

#include <geos/planargraph/PlanarGraph.h>
#include <openfluid/dllexport.hpp>
#include <ogrsf_frmts.h>
#include <list>

namespace geos { namespace geom {
class Geometry;
class GeometryFactory;
class LineString;
class Polygon;
class Coordinate;
} }

namespace planargraph {
class Node;
}

namespace openfluid {

namespace core {
class GeoVectorValue;
class GeoRasterValue;
}

/**
  @brief Classes for landscape representation management.
*/
namespace landr {

class LandREntity;
class VectorDataset;
class RasterDataset;

/**
  @brief Interface for a graph composed of LandREntity.
*/
class OPENFLUID_API LandRGraph: public geos::planargraph::PlanarGraph
{
  public:

    enum GraphType
    {
      POLYGON, LINESTRING
    };

    typedef std::list<LandREntity*> Entities_t;


  protected:
    /**
      @brief The VectorDataset associated to this LandRGraph.
    */
    openfluid::landr::VectorDataset* mp_Vector;

    /**
      @brief The geos::geom::GeometryFactory used to build this LandRGraph.
    */
    const geos::geom::GeometryFactory* mp_Factory;

    /**
      @brief A map of the LandREntity of this LandRGraph and sorted by identifier.
    */
    std::map<int, LandREntity*> m_EntitiesByOfldId;

    /**
      @brief A list of the LandREntity of this LandRGraph.
    */
    Entities_t m_Entities;

    /**
      @brief The RasterDataset associated to this LandRGraph.
    */
    openfluid::landr::RasterDataset* mp_Raster;

    /**
      @brief The VectorDataset representation of the RasterDataset associated to this LandRGraph.
    */
    openfluid::landr::VectorDataset* mp_RasterPolygonized;

    /**
      @brief A vector of geos::geom::Polygon representation of the RasterDataset associated to this LandRGraph.
    */
    std::vector<geos::geom::Polygon*>* mp_RasterPolygonizedPolys;

    static int FileNum;

    LandRGraph();

    /**
      @brief Creates a new LandRGraph from a core::GeoVectorValue.
    */
    LandRGraph(openfluid::core::GeoVectorValue& Val);

    /**
      @brief Creates a new LandRGraph from a VectorDataset.
    */
    LandRGraph(const openfluid::landr::VectorDataset& Vect);

    /**
      @brief Adds LandREntity from the associated VectorDataset of this LandRGraph.
    */
    void addEntitiesFromGeoVector();

    /**
      @brief Adds LandREntity from a LandREntity list to this LandRGraph.
    */
    void addEntitiesFromEntityList(const LandRGraph::Entities_t& Entities);

    /**
      @brief Adds a LandREntity to this LandRGraph.
      @param Entity The LandREntity to add.
    */
    virtual void addEntity(LandREntity* Entity) = 0;

    /**
      @brief Creates a new LandREntity.
      @param Geom A geos::geom::Geometry.
      @param OfldId The identifier of the new LandREntity.
    */
    virtual LandREntity* createNewEntity(const geos::geom::Geometry* Geom,
                                      unsigned int OfldId) = 0;

    /**
      @brief Returns a geos::planagraph::Node of this LandRGraph from a geos::geom::Coordinate.
      @param Coordinate A geos::geom::Coordinate.
      @return A geos::planargraph::Node.
    */
    geos::planargraph::Node* node(const geos::geom::Coordinate& Coordinate);

  public:

    /**
      @attention Delete also associated RasterPolygonized if present.
    */
    virtual ~LandRGraph();

    /**
      @brief Returns the type of graph.
    */
    virtual GraphType getType() = 0;

    /**
      @brief Returns the LandREntity with OfldId, or 0 if it doesn't exist.
    */
    virtual LandREntity* entity(int OfldId);

    /**
      @brief Returns a list of the LandREntity of this LandRGraph.
    */
    Entities_t getEntities();

    /**
      @brief Returns a list of the LandREntity of this LandRGraph and sorted by identifier.
    */
    Entities_t getOfldIdOrderedEntities();

    /**
      @brief Returns a map of the LandREntity of this LandRGraph and their identifiers.
    */
    std::map<int, LandREntity*> getEntitiesByOfldId();

    /**
      @brief Gets the number of LandREntity in the LandRGraph.
    */
    unsigned int getSize() const;

    /**
      @brief Removes from this LandRGraph the nodes of degree 0.
    */
    void removeUnusedNodes();

    /**
      @brief Adds an attribute to this LandRGraph.
      @details Doesn't reset if the AttributeName already exists.
      @param AttributeName The name of the attribute.
    */
    void addAttribute(const std::string& AttributeName);

    /**
      @brief Removes an attribute to this LandRGraph.
      @details Does nothing if AttributeName doesn't exist.
      @param AttributeName The name of the attribute.
    */
    void removeAttribute(const std::string& AttributeName);

    /**
      @brief Returns a vector of the names of the attributes of this LandRGraph.
    */
    std::vector<std::string> getAttributeNames();

    /**
      @brief Associates a core::GeoRasterValue to this LandRGraph.
      @param Raster A core::GeoRasterValue.
      @details Replace associated raster if exists.
    */
    void addAGeoRasterValue(openfluid::core::GeoRasterValue& Raster);

    /**
      @brief Associates a RasterDataset to this LandRGraph.
      @param Raster A RasterDataset.
      @details Replace associated raster if exists.
    */
    void addAGeoRasterValue(const openfluid::landr::RasterDataset& Raster);

    /**
      @brief Returns true if this LandRGraph has an associated raster, false otherwise.
    */
    bool hasAnAssociatedRaster();

    /**
      @brief Transforms the associated raster value into an openfluid::landr::VectorDataset of polygons.
      @return An openfluid::landr::VectorDataset of the created polygons.
    */
    openfluid::landr::VectorDataset* rasterPolygonized();

    /**
      @brief Transforms the associated raster value into a vector of geos::geom::Polygon.
      @return A vector of the created geos::geom::Polygon.
    */
    std::vector<geos::geom::Polygon*>* rasterPolygonizedPolys();

    /**
      @brief Fetchs the associated raster value corresponding to the LandREntity centroid coordinate.
      @param Entity The LandREntity to get the centroid coordinate from.
      @return The raster value corresponding to the LandREntity centroid coordinate.
    */
    virtual double getRasterValueForEntityCentroid(const LandREntity& Entity);

    /**
      @brief Creates a new attribute for all the LandREntity of this LandRGraph, and set for each LandREntity
      this attribute value as the raster value corresponding to the LandREntity centroid coordinate.
      @param AttributeName The name of the attribute to create.
    */
    void setAttributeFromRasterValueAtCentroid(const std::string& AttributeName);

    /**
      @brief Creates a new attribute for all the LandREntity of this LandRGraph, and set for each LandREntity
      this attribute value as a mean of the raster value.
      @param AttributeName The name of the attribute to create.
    */
    virtual void setAttributeFromMeanRasterValues(const std::string& AttributeName)=0;

    /**
      @brief Computes the LandREntity neighbours of each LandREntity of this LandRGraph, according to its type.
    */
    void computeNeighbours();

    /**
      @brief Creates on disk a shapefile representing this LandRGraph.
      @param FilePath The path where to create the out file.
      @param FileName A name for the out file to create, with a .shp extension.
    */
    void exportToShp(const std::string& FilePath, const std::string& FileName);

    /**
      @brief Creates a new attribute for all the LandREntity of this LandRGraph, and set for each LandREntity
      this attribute value as the vector value corresponding to the entity OFLD_ID.
      @param AttributeName The name of the attribute to create.
      @param Vector The Name of the core::GeoVectorValue.
      @param Column The column of the core::GeoVectorValue to upload.
    */
    void setAttributeFromVectorId(const std::string& AttributeName,
                                  openfluid::core::GeoVectorValue& Vector,
                                  const std::string& Column);

    /**
      @brief Creates a new attribute for all the LandREntity of this LandRGraph, and set for each LandREntity
      this attribute value as the vector value corresponding to the entity OFLD_ID.
      @param AttributeName The name of the attribute to create.
      @param Vector The Name of the VectorDataset.
      @param Column The column of the core::GeoVectorValue to upload.
    */
    void setAttributeFromVectorId(const std::string& AttributeName,
                                  openfluid::landr::VectorDataset& Vector,
                                  const std::string& Column);

    /**
      @brief Creates a new attribute for all the LandREntity of this LandRGraph, and set for each LandREntity
      this attribute value as the vector value corresponding to the Vector Entity Geometry
      @param AttributeName The name of the attribute to create.
      @param Vector The Name of the core::GeoVectorValue.
      @param Column The column of the core::GeoVectorValue to upload.
      @param Thresh The threshold of minimum distance between
      the core::GeoVectorValue geometry and the LandRGraph geometry.
    */
    virtual void setAttributeFromVectorLocation(const std::string& AttributeName,
                                                openfluid::core::GeoVectorValue& Vector,
                                                const std::string& Column,double Thresh=0.0001);

    /**
      @brief Creates a new attribute for all the LandREntity of this LandRGraph, and set for each LandREntity
      this attribute value as the vector value corresponding to the Vector Entity Geometry
      @param AttributeName The name of the attribute to create.
      @param Vector The Name of the VectorDataset.
      @param Column The column of the VectorDataset to upload.
      @param Thresh The threshold of minimum distance between the VectorDataset geometry and the LandRGraph geometry.
    */
    virtual void setAttributeFromVectorLocation(const std::string& AttributeName,
                                                openfluid::landr::VectorDataset& Vector,
                                                const std::string& Column,double Thresh=0.0001);

    /**
      @brief Removes a LandREntity with identifier from this LandRGraph.
      @details The associated nodes of the LandREntity are also removed.
      @param OfldId The identifier.
    */
     virtual void removeEntity(int OfldId)=0;

    /**
      @brief Snap the vertices of this LandRGraph under a threshold.
      @param snapTolerance The threshold.
    */
     void snapVertices(double snapTolerance);

};

} } // namespace landr, openfluid

#endif /* __OPENFLUID_LANDR_LANDRGRAPH_HPP__ */
