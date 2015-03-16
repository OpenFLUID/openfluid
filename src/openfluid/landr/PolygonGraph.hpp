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
  @file PolygonGraph.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
 */

#ifndef __OPENFLUID_LANDR_POLYGONGRAPH_HPP__
#define __OPENFLUID_LANDR_POLYGONGRAPH_HPP__

#include <openfluid/core/Value.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/landr/LandRGraph.hpp>


// for covariant return type of getEntity
#include <openfluid/landr/PolygonEntity.hpp>
#include <openfluid/dllexport.hpp>

namespace openfluid { namespace landr {

class VectorDataset;

/**
  @brief A LandRGraph composed of PolygonEntities.
*/
class OPENFLUID_API PolygonGraph: public LandRGraph
{
  public:

    /**
      @brief A a map of polygonized Raster geos::geom::Polygon and its area intersecting PolygonEntity.
    */
    typedef std::map<geos::geom::Polygon*, double> RastValByRastPoly_t;

  private:

    /**
      @brief Creates a new PolygonGraph from an other PolygonGraph.
    */
    PolygonGraph(PolygonGraph& Other);

  protected:

    PolygonGraph();

    /**
      @brief Creates a new PolygonGraph initialized from a core::GeoVectorValue.
    */
    PolygonGraph(openfluid::core::GeoVectorValue& Val);

    /**
      @brief Creates a new PolygonGraph initialized from a VectorDataset.
    */
    PolygonGraph(openfluid::landr::VectorDataset& Vect);

    /**
      @brief Adds a LandREntity into this PolygonGraph.
    */
    virtual void addEntity(LandREntity* Entity);

    /**
      @brief Creates a new PolygonEntity.
      @param Geom The geos::geom::Geometry of the new PolygonEntity to create.
      @param OfldId The identifier of the new PolygonEntity.
      @return A new LandREntity.
    */
    virtual LandREntity* createNewEntity(const geos::geom::Geometry* Geom,
                                         unsigned int OfldId);

    /**
      @brief Creates a new PolygonEdge, with its two DirectedEdges and add them to this graph.
      @param LineString The geos::geom::LineString representing the PolygonEdge to create.
      @return The newly created PolygonEdge, or 0 if fails.
    */
    PolygonEdge* createEdge(geos::geom::LineString& LineString);

    /**
      @brief Removes a segment of the exterior boundary of the input PolygonEntity.
      @param Entity The PolygonEntity to removes the segment to.
      @param Segment The geos::geom::LineString to remove.
    */
    void removeSegment(PolygonEntity* Entity,
                       geos::geom::LineString* Segment);

    /**
      @brief Adds an attribute to the PolygonEdge of a PolygonEntity.
      @param AttributeName The name of the attribute to add.
      @param Entity The LandREntity to add the PolygonEdge attribute.
    */
    void addEdgeAttribute(std::string AttributeName,
                          LandREntity& Entity);

    /**
      @brief Removes an attribute to the PolygonEdge of a PolygonEntity.
      @param AttributeName The name of the attribute to remove.
      @param Entity The LandREntity to remove the PolygonEdge attribute.
    */
    void removeEdgeAttribute(std::string AttributeName,
                             LandREntity& Entity);

  public:

    /**
      @brief Creates a new PolygonGraph initialized from a core::GeoVectorValue.
      @details Val must be composed of one or many Polygons, and each of them must contain a "OFLD_ID" attribute.
    */
    static PolygonGraph* create(openfluid::core::GeoVectorValue& Val);

    /**
      @brief Create a new PolygonGraph initialized from a VectorDataset.
      @details Vect must be composed of one or many Polygons, and each of them must contain a "OFLD_ID" attribute.
    */
    static PolygonGraph* create(openfluid::landr::VectorDataset& Vect);

    /**
      @brief Create a new PolygonGraph initialized with a list of LandREntity.
      @details Entities must be PolygonEntity.
    */
    static PolygonGraph* create(const LandRGraph::Entities_t& Entities);

    virtual ~PolygonGraph();

    /**
      @brief Returns the type of graph.
    */
    LandRGraph::GraphType getType();

    /**
      @brief Returns a PolygonEntity with OfldId, or 0 if it doesn't exist.
    */
    PolygonEntity* entity(int OfldId);

    /**
      @brief Returns true if each PolygonEntity is complete.
      @return True if all PolygonEntity of this PolygonGraph are complete, false otherwise.
    */
    bool isComplete();

    /**
      @brief Returns true if this PolygonGraph has one or more islands.
      @return True if one or more islands are present, false otherwise.
    */
    bool hasIsland();

    /**
      @brief Gets a map of polygonized Raster polygons and its area intersecting Entity.
      @param Entity The PolygonEntity to compare with the associated Raster.
      @return A map of polygonized Raster Polygons, from associated polygonized raster,
      with for each one the intersection area.
    */
    RastValByRastPoly_t computeRasterPolyOverlapping(PolygonEntity& Entity);

    /**
      @brief Creates a new attribute for this PolygonGraph entities, and set for each PolygonEntity
      this attribute value as the mean of the overlapping raster values, relative to overlapping areas.
      @param AttributeName The name of the attribute to create
    */
    virtual void setAttributeFromMeanRasterValues(const std::string& AttributeName);

    /**
      @brief Creates on disk a shapefile representing the PolygonEdges of this PolygonGraph.
      @param FilePath The path where to create the out file.
      @param FileName A name for the out file to create, with a .shp extension.
    */
    void createVectorRepresentation(std::string FilePath,
                                    std::string FileName);

    /**
      @brief Computes the neighbours between the PolygonEntity elements of this PolygonGraph
      and the LineStringEntity of a LineStringGraph.
      @param Graph The LineStringGraph to compare to.
      @param Relation The Relationship to use for comparison.
      @param BufferDistance The distance below which we consider that two elements are related.
      @param ContactLength Min Length of the LineString in intersection with polygon Buffered Boundaries
      to be taking acccount (only for LandRTools::TOUCHES RelationShip)
    */
    void computeLineStringNeighbours(LineStringGraph& Graph,
                                     openfluid::landr::LandRTools::Relationship Relation,
                                     double BufferDistance,
                                     double ContactLength=0);

    /**
      @brief Computes the neighbours between the PolygonEntity elements of this PolygonGraph by using
      the LineStringEntity of an input LineStringGraph which are considered as barriers.
      @details A barrier between two PolygonEntity will avoid to considered them as neighbours.
      @details A LineStringEntity is considered as a barrier if it lies within the buffer of this
      PolygonEntity polygon boundary.
      @param Graph The LineStringGraph to compare to.
      @param Relation The Relationship to use for comparison, the LandRTools::Relationship INTERSECTS is not allowed.
      @param BufferDistance The distance below which we consider that two elements are related.
      @param ContactLength Min Length of the LineString in intersection with polygon Buffered Boundaries
      to be taking acccount (only for LandRTools::TOUCHES RelationShip)
    */
    void computeNeighboursWithBarriers(LineStringGraph& Graph,
                                       openfluid::landr::LandRTools::Relationship Relation,
                                       double BufferDistance,
                                       double ContactLength=0);

    /**
      @brief Creates attribute for the PolygonEdge of this PolygonGraph.
      @details Doesn't reset if the AttributeName already exists.
      @param AttributeName The name of the PolygonEdge attribute.
      @param Value The core::Value to associate to this attribute.
    */
    void createEdgeAttribute(std::string AttributeName,
                             openfluid::core::Value &Value);

    /**
      @brief Removes the attribute of the PolygonEdge of this PolygonGraph.
      @param AttributeName The name of the PolygonEdge attribute to delete.
      @details Does nothing if AttributeName doesn't exist.
    */
    void removeEdgeAttribute(std::string AttributeName);

    /**
      @brief Returns a vector of the name of the PolygonEdge attributes.
      @return A vector of the name of the PolygonEdge attributes.
    */
    std::vector<std::string> getEdgeAttributeNames();

    /**
      @brief Removes from this PolygonGraph the PolygonEntity with OfldId and its associated nodes.
      @param OfldId
    */
    virtual void removeEntity(int OfldId);

    /**
      @brief Clean the PolygonEdge of a PolygonEntity.
      @param Entity The PolygonEntity to clean.
    */
    void cleanEdges(PolygonEntity & Entity);

    /**
      @brief Gets a map of small PolygonEntity which area are under a threshold.
      @param MinArea The area threshold (in map units).
      @return a multimap of PolygonEntity with key is the area of each PolygonEntity.
    */
    std::multimap<double,  PolygonEntity*> getPolygonEntitiesByMinArea(double MinArea);

    /**
      @brief Gets a map of sliver PolygonEntity which compactness value are superior to a
      compactness threshold (Gravelius Index)
      @param Compactness The compactness threshold (perimeter/2 x sqrt (Pi x area))
      @return a multimap of PolygonEntity with key is the compactness of each PolygonEntity.
    */
    std::multimap<double,  PolygonEntity*> getPolygonEntitiesByCompactness(double Compactness);

    /**
      @brief Merge a PolygonEntity into an other one.
      @details The PolygonEntity to merge is deleted.
      @param Entity An existent PolygonEntity.
      @param EntityToMerge The PolygonEntity which will be merged into Entity and will be deleted.
    */
    void mergePolygonEntities(PolygonEntity& Entity,
                              PolygonEntity& EntityToMerge);

    /**
      @brief Merge the entities of this PolygonGraph which area is under threshold
      @details The small PolygonEntity is merged into the one which share the longest boundary.
      @param MinArea The minimum area threshold.
    */
    void mergePolygonEntitiesByMinArea(double MinArea);

    /**
      @brief Merge the entities of this PolygonGraph which compactness value are superior
      to a compactness threshold (Gravelius Index).
      @details The small PolygonEntity is merged into the one which share the longest boundary.
      @param Compactness The compactness threshold (perimeter/2 x sqrt (Pi x area)).
    */
    void mergePolygonEntitiesByCompactness(double Compactness);



};

} } // namespace landr, openfluid
#endif /* __OPENFLUID_LANDR_POLYGONGRAPH_HPP__ */
