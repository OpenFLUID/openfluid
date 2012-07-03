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
 \file LineStringGraph.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef LINESTRINGGRAPH_HPP_
#define LINESTRINGGRAPH_HPP_

#include <geos/planargraph/PlanarGraph.h>

#include <openfluid/landr/LineStringEntity.hpp>

#include <vector>

namespace geos {
namespace geom {
class LineString;
class Coordinate;
}
namespace planargraph {
class Node;
class Edge;
class DirectedEdge;
}
}

namespace openfluid {
namespace core {
class GeoVectorValue;
}
}

/**
 Macro for declaration of a loop processing all entities of a graph
 @param[in] loopid ID of the loop
 */
#define DECLARE_ENTITIES_GRAPH_LOOP(loopid) \
  std::vector<openfluid::landr::LineStringEntity*>::iterator _M_##loopid##_it;\
  std::vector<openfluid::landr::LineStringEntity*> _M_##loopid##_uvect; \


/**
 Macro for declaration of a loop processing all entities of a graph, following their selfid
 @param[in] loopid ID of the loop
 */
#define DECLARE_ENTITIES_ORDERED_LOOP(loopid) \
  std::vector<openfluid::landr::LineStringEntity*>::iterator _M_##loopid##_it;\
  std::vector<openfluid::landr::LineStringEntity*> _M_##loopid##_uvect; \

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
    entity = *_M_##loopid##_it; \

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
      entity = *_M_##loopid##_it; \

/**
 Macro for the ending of a loop
 */
#define END_LOOP \
    } \
  }

namespace openfluid {
namespace landr {

class LineStringGraph: public geos::planargraph::PlanarGraph
{
  private:

    geos::planargraph::Node* getNode(const geos::geom::Coordinate& Coordinate);

    std::vector<geos::planargraph::Node*> m_NewNodes;

    std::map<int, openfluid::landr::LineStringEntity*> m_EntitiesBySelfId;

    std::vector<openfluid::landr::LineStringEntity*> m_Entities;

    std::vector<geos::planargraph::DirectedEdge*> m_NewDirEdges;

    void addAttribute(std::string AttributeName, LineStringEntity& Entity);

    void removeAttribute(std::string AttributeName, LineStringEntity& Entity);

  public:

    LineStringGraph();

    LineStringGraph(const openfluid::core::GeoVectorValue& Val);

    LineStringGraph(openfluid::landr::LineStringGraph& Other);

    LineStringGraph(
        const std::vector<openfluid::landr::LineStringEntity*>& Entities);

    /**
     * Takes ownership of LineString and Feature
     */
    openfluid::landr::LineStringEntity* addEdge(
        const geos::geom::LineString* LineString, OGRFeature* Feat);

    virtual ~LineStringGraph();

    openfluid::landr::LineStringEntity* getLastLineStringEntity();

    std::vector<openfluid::landr::LineStringEntity*> getEndLineStringEntities();

    std::vector<openfluid::landr::LineStringEntity*> getStartLineStringEntities();

    unsigned int getSize();

    openfluid::landr::LineStringEntity* getEntity(int SelfId);

    std::vector<openfluid::landr::LineStringEntity*> getEntities();

    std::vector<openfluid::landr::LineStringEntity*> getSelfIdOrderedEntities();

    /**
     * Doesn't reset if the AttributeName already exists.
     */
    void addAttribute(std::string AttributeName);

    /**
     * Does nothing if AttributeName doesn't exist.
     */
    void removeAttribute(std::string AttributeName);

};

} // namespace landr
} /* namespace openfluid */

#endif /* LINESTRINGGRAPH_HPP_ */
