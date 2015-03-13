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
  @file PolygonEdge.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef __OPENFLUID_LANDR_POLYGONEDGE_HPP__
#define __OPENFLUID_LANDR_POLYGONEDGE_HPP__

#include <geos/planargraph/Edge.h>
#include <openfluid/dllexport.hpp>

#include <map>

namespace geos {
namespace geom {
class LineString;
class CoordinateSequence;
}
namespace planargraph {
class Edge;
} }

namespace openfluid {
namespace core {
class Value;
}

namespace landr {

class PolygonEntity;

/**
 @brief A part of a PolygonEntity exterior ring, that may be share between to adjacent PolygonEntity.
 @details A PolygonEdge  has one or two Faces. The Faces are the PolygonEntity that share this PolygonEdge.
 */
class OPENFLUID_API PolygonEdge: public geos::planargraph::Edge
{
  private:

    /**
     @brief The geos::geom::LineString associated to this PolygonEdge.
     */
    geos::geom::LineString& m_Line;

    /**
     @brief A vector of the PolygonEntity associated to this PolygonEdge.
     @details At most two elements vector.
     */
    std::vector<PolygonEntity*> m_Faces;


  public:

    PolygonEdge(geos::geom::LineString& Line);

    ~PolygonEdge();

    /**
     @brief Map of Attributes which are carried by this PolygonEdge.
     */
    std::map<std::string, core::Value*> m_EdgeAttributes;


    /**
     @brief Returns the geos::geom::LineString representing this PolygonEdge.
     */
    geos::geom::LineString* line();

    /**
     @brief Add a PolygonEntity as a Face to this PolygonEdge.
     @param NewFace A PolygonEntity.
     @throw base::FrameworkException if this PolygonEdge is not in the boundary of the input PolygonEntity,
     or if this PolygonEdge has already two Faces.
     */
    void addFace(PolygonEntity& NewFace);

    /**
     @brief Returns true if this PolygonEdge is in the boundary of the input PolygonEntity.
     @param Face A PolygonEntity.
     @return True if this PolygonEdge is in the boundary of the input PolygonEntity, false otherwise.
     */
    bool isLineInFace(PolygonEntity& Face);

    /**
     @brief Returns a vector of PolygonEntity which represent the Faces of this PolygonEdge.
     */
    const std::vector<PolygonEntity*> getFaces();

    /**
     @brief Removes a Face from the Faces of this PolygonEdge.
     @details Does nothing if the input Face is not a part of this PolygonEdge Faces.
     @param Face A PolygonEntity.
     */
    void removeFace(PolygonEntity* Face);

    /**
     @brief Gets the value of an attribute of this PolygonEdge.
     @param AttributeName The name of the attribute to get.
     @param Value The core::Value to assign the attribute value.
     @return True if the attribute exists, false otherwise.
     */
    bool getAttributeValue(const std::string& AttributeName, core::Value& Value) const;

    /**
     @brief Sets the value of an attribute of this PolygonEdge.
     @details Takes the ownership of Value.
     @param AttributeName The name of the attribute to set.
     @param Value The core::Value assign to the attribute value.
     @return True if the attribute exists, false otherwise.
     */
    bool setAttributeValue(const std::string& AttributeName, const core::Value* Value);

    /**
     @brief Removes an attribute of this PolygonEdge.
     @param AttributeName The name of the attribute to set.
     */
    void removeAttribute(const std::string& AttributeName);

    /**
     @brief Returns true if this PolygonEdge is coincident with an other PolygonEdge.
     @param Edge The PolygonEdge to test the coincidence.
     @return True if the two PolygonEdge are coincidents, false otherwise.
     */
    bool isCoincident(PolygonEdge *Edge);

};

} } // namespace landr, openfluid
#endif /* __OPENFLUID_LANDR_POLYGONEDGE_HPP__ */
