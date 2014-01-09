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
 \file LineStringGraph.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef LINESTRINGGRAPH_HPP_
#define LINESTRINGGRAPH_HPP_

#include <openfluid/landr/LandRGraph.hpp>
#include <openfluid/dllexport.hpp>

// for covariant return type of getEntity
#include <openfluid/landr/LineStringEntity.hpp>

/**
 Macro for declaration of a loop processing all entities of a graph
 @param[in] loopid ID of the loop
 */
#define DECLARE_ENTITIES_GRAPH_LOOP(loopid) \
		std::list<openfluid::landr::LandREntity*>::iterator _M_##loopid##_it;\
		std::list<openfluid::landr::LandREntity*> _M_##loopid##_uvect; \


/**
 Macro for declaration of a loop processing all entities of a graph, following their selfid
 @param[in] loopid ID of the loop
 */
#define DECLARE_ENTITIES_ORDERED_LOOP(loopid) \
		std::list<openfluid::landr::LandREntity*>::iterator _M_##loopid##_it;\
		std::list<openfluid::landr::LandREntity*> _M_##loopid##_uvect; \

/**
 Macro for the beginning of a loop processing all entities of a graph
 @param[in] loopid ID of the loop, must match declaration
 @param[in] graph pointer to a openfluid::landr::LineStringGraph
 @param[out] entity pointer to a openfluid::landr::LineStringEntity object, pointing to the current processed entity
 */
#define BEGIN_ENTITIES_GRAPH_LOOP(loopid,graph,entity) \
		if (graph) \
		{ \
			_M_##loopid##_uvect = graph->getEntities();\
			for(_M_##loopid##_it=_M_##loopid##_uvect.begin(); _M_##loopid##_it != _M_##loopid##_uvect.end(); ++_M_##loopid##_it) \
			{ \
				entity = dynamic_cast<openfluid::landr::LineStringEntity*>(*_M_##loopid##_it); \

/**
 Macro for the beginning of a loop processing all entities of a graph, following their selfid
 @param[in] loopid ID of the loop, must match declaration
 @param[in] graph pointer to a openfluid::landr::LineStringGraph
 @param[out] entity pointer to a openfluid::landr::LineStringEntity object, pointing to the current processed entity
 */
#define BEGIN_ENTITIES_ORDERED_LOOP(loopid,graph,entity) \
		if (graph) \
		{ \
			_M_##loopid##_uvect = graph->getSelfIdOrderedEntities();\
			for(_M_##loopid##_it=_M_##loopid##_uvect.begin(); _M_##loopid##_it != _M_##loopid##_uvect.end(); ++_M_##loopid##_it) \
			{ \
				entity = dynamic_cast<openfluid::landr::LineStringEntity*>(*_M_##loopid##_it); \

/**
 Macro for the ending of a loop
 */
#define END_LOOP \
		} \
		}

namespace openfluid {
namespace landr {

class VectorDataset;

/**
 * @brief A LandRGraph composed of LineStringEntities.
 */
class DLLEXPORT LineStringGraph: public LandRGraph
{
private:

	LineStringGraph(LineStringGraph& Other);

protected:

	LineStringGraph();

	/**
	 * @brief Creates a new LineStringGraph initialized from a core::GeoVectorValue.
	 */
	LineStringGraph(openfluid::core::GeoVectorValue& Val);

	/**
	 * @brief Creates a new LineStringGraph initialized from a VectorDataset.
	 */
	LineStringGraph(openfluid::landr::VectorDataset& Vect);

	/**
	 * @brief Adds a LandREntity into this LineStringGraph.
	 */
	virtual void addEntity(LandREntity* Entity);

	/**
	 * @brief Creates a new LineStringEntity.
	 *
	 * @param Geom The geos::geom::Geometry of the new LineStringEntity to create.
	 * @param SelfId The identifier of the new LineStringEntity.
	 *
	 * @return A new LandREntity.
	 */
	virtual LandREntity* getNewEntity(const geos::geom::Geometry* Geom,
			unsigned int SelfId);




public:

	/**
	 * @brief Creates a new LineStringGraph initialized from a core::GeoVectorValue.
	 * @param Val A core::GeoVectorValue which must be composed of one or many LineStrings, and each of them must contain a "SELF_ID" attribute.
	 */
	static LineStringGraph* create(openfluid::core::GeoVectorValue& Val);

	/**
	 * @brief Creates a new LineStringGraph initialized from a VectorDataset.
	 * @param Vect A VectorDataset which must be composed of one or many LineStrings, and each of them must contain a "SELF_ID" attribute.
	 */
	static LineStringGraph* create(openfluid::landr::VectorDataset& Vect);

	/**
	 * @brief Creates a new LineStringGraph initialized with a list of LandREntity.
	 * @param Entities A list of LandREntity which must be LineStringEntity.
	 */
	static LineStringGraph* create(const LandRGraph::Entities_t& Entities);

	virtual ~LineStringGraph();

	/**
	 * @brief Returns the type of graph.
	 */
	LandRGraph::GraphType getType();

	/**
	 * @brief Returns a LineStringEntity with SelfId, or 0 if it doesn't exist.
	 */
	LineStringEntity* getEntity(int SelfId);

	/**
	 * @brief Removes from this LineStringGraph the LineStringEntity with SelfId and its associated nodes.
	 * @param SelfId The identifier.
	 */
	virtual void removeEntity(int SelfId);

	/**
	 * @brief Returns the last LineStringEntity of this LineStringGraph, according to the LineStringEntity orientations,
	 * ie the one that has no down neighbour.
	 *
	 * @return The last LineStringEntity or 0 if there is zero or more than one LineStringEntity whith no down neighbour.
	 */
	LineStringEntity* getLastLineStringEntity();

	/**
	 * @brief Returns a vector of LineStringEntity that have no down neighbour, according to the LineStringEntity orientations.
	 */
	std::vector<LineStringEntity*> getEndLineStringEntities();

	/**
	 * @brief Returns a vector of LineStringEntity that have no up neighbour, according to the LineStringEntity orientations.
	 */
	std::vector<LineStringEntity*> getStartLineStringEntities();

	/**
	 * @brief Fetch the associated raster value corresponding to the LineStringEntity StartNode coordinate.
	 *
	 * @param Entity The LineStringEntity to get the StartNode coordinate from.
	 * @return The raster value corresponding to the LineStringEntity StartNode coordinate.
	 */
	float* getRasterValueForEntityStartNode(LineStringEntity& Entity);

	/**
	 * @brief Fetch the associated raster value corresponding to the LineStringEntity EndNode coordinate.
	 *
	 * @param Entity The LineStringEntity to get the EndNode coordinate from.
	 * @return The raster value corresponding to the LineStringEntity EndNode coordinate.
	 */
	float* getRasterValueForEntityEndNode(LineStringEntity& Entity);

	/**
	 * @brief Creates a new attribute for these LineStringGraph entities, and set for each LineStringEntity
	 * this attribute value as the associated raster values corresponding to the StartNode LineStringEntity coordinates.
	 *
	 * @param AttributeName The name of the attribute to create for the StartNode
	 */
	void setAttributeFromRasterValueAtStartNode(const std::string& AttributeName);

	/**
	 * @brief Creates a new attribute for these LineStringGraph entities, and set for each LineStringEntity
	 * this attribute value as the associated raster values corresponding to the EndNode LineStringEntity coordinates.
	 *
	 * @param AttributeName The name of the attribute to create for the EndNode
	 */
	void setAttributeFromRasterValueAtEndNode(const std::string& AttributeName);

	/**
	 * @brief Reverse a LineStringEntity orientation.
	 *
	 * @param Entity The LineStringEntity to reverse.
	 */
	void reverseLineStringEntity(LineStringEntity& Entity);

	/**
	 * @brief Returns true if this LineStringGraph is an arborescence, false otherwise.
	 * @details An arborescence is a graph with no loop; edges can be well directed or not.
	 */
	bool isLineStringGraphArborescence();

	/**
	 * @brief Creates a new attribute for this LineStringGraph entities, and set for each LineStringEntity
	 * this attribute value as the mean of the StartNode altitude and the EndNode altitude.
	 * @param AttributeName The name of the attribute to create.
	 */
	virtual void setAttributeFromMeanRasterValues(const std::string& AttributeName);

	/**
	 * @brief Creates a new attribute for this LineStringGraph entities, and set for each LineStringEntity
	 * this attribute value as the vector value corresponding to the Vector Entity Geometry.
	 *
	 * @param AttributeName The name of the attribute to create.
	 * @param Vector The Name of the core::GeoVectorValue.
	 * @param Column The column of the core::GeoVectorValue to upload.
	 * @param Thresh The threshold of minimum distance between the core::GeoVectorValue geometry and the LineStringGraph geometry.
	 */
	virtual void setAttributeFromVectorLocation(const std::string& AttributeName, openfluid::core::GeoVectorValue& Vector,
	                                            const std::string& Column,double Thresh=0.0001);

	/**
	 * @brief Creates a new attribute for this LineStringGraph entities, and set for each LineStringEntity.
	 * this attribute value as the vector value corresponding to the Vector Entity Geometry
	 *
	 * @param AttributeName The name of the attribute to create.
	 * @param Vector The Name of the VectorDataset.
	 * @param Column The column of the VectorDataset to upload.
	 * @param Thresh The threshold of minimum distance between the VectorDataset geometry and the LineStringGraph geometry.
	 */
	virtual void setAttributeFromVectorLocation(const std::string& AttributeName, openfluid::landr::VectorDataset& Vector,
	                                            const std::string& Column,double Thresh=0.0001);

	/**
	 * @brief Merges a LineStringEntity into an other one.
	 * @details The LineStringEntity to merge is deleted.
	 *
	 * @param Entity An existent LineStringEntity.
	 * @param EntityToMerge The LineStringEntity which will be merged into Entity and will be deleted.
	 */
	void mergeLineStringEntities(LineStringEntity& Entity, LineStringEntity& EntityToMerge);


	/**
	 * @brief Gets a map of small LineStringEntity under length threshold and only if the LineStringEntity is
	 * not between two confluences.
	 *
	 * @param MinLength The length threshold (in map units).
	 * @param rmDangle : if true, get also dangles under the threshold.
	 * @return a multimap of LineStringEntity with key is the length of each LineStringEntity.
	 */
	std::multimap<double,  LineStringEntity*> getLineStringEntitiesByMinLength(double MinLength,bool rmDangle=true);





};

} // namespace landr
} /* namespace openfluid */

#endif /* LINESTRINGGRAPH_HPP_ */
