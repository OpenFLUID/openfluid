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
 \file PolygonEdge.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef POLYGONEDGE_HPP_
#define POLYGONEDGE_HPP_

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
}
}

namespace openfluid {
namespace core {
class Value;

}
namespace landr {

class PolygonEntity;

/**
 * @brief A part of a PolygonEntity exterior ring, that may be share between to adjacent PolygoneEntities.
 * @details A PolygonEdge  has one or two Faces. The Faces are the PolygonEntities that share this PolygonEdge.
 */
class DLLEXPORT PolygonEdge: public geos::planargraph::Edge
{
  private:

    geos::geom::LineString& m_Line;

    /**
     * @details At most two elements vector.
     */
    std::vector<PolygonEntity*> m_Faces;


  public:

    PolygonEdge(geos::geom::LineString& Line);

    ~PolygonEdge();

    /**
     * @brief Map of Attributes which are carried by this PolygonEdge.
     */
    std::map<std::string, core::Value*> m_EdgeAttributes;


    /**
     * @brief Return the LineString representing this PolygonEdge.
     */
    geos::geom::LineString* getLine();

    /**
     * @brief Add a PolygonEntity as a Face to this PolygonEdge.
     * @throw base::OFException if this PolygonEdge is not in the boundary of the input PolygonEntity,
     * or if this PolygonEdge has already two Faces.
     */
    void addFace(PolygonEntity& NewFace);

    /**
     * @brief Check that this PolygonEdge is in the boundary of the input PolygonEntity.
     */
    bool isLineInFace(PolygonEntity& Face);

    /**
     * @brief Return the Faces of this PolygonEdge.
     */
    const std::vector<PolygonEntity*> getFaces();

    /**
     * @brief Remove a Face from the Faces of this PolygonEdge.
     * @details Does nothing if the input Face is not a part of this PolygonEdge Faces.
     */
    void removeFace(PolygonEntity* Face);

    /**
     * @brief Get the value of an attribute of this PolygonEdge.
     *
     * @param AttributeName The name of the attribute to get.
     * @param Value The core::Value to assign the attribute value.
     * @return True if the attribute exists, false otherwise.
     */
    bool getAttributeValue(const std::string& AttributeName, core::Value& Value) const;

    /**
     * @brief Set the value of an attribute of this PolygonEdge.
     * @details Takes the ownership of Value.
     *
     * @param AttributeName The name of the attribute to set.
     * @param Value The core::Value assign to the attribute value.
     * @return True if the attribute exists, false otherwise.
     */
    bool setAttributeValue(const std::string& AttributeName, const core::Value* Value);

    /**
     * @brief Remove an attribute of this PolygonEdge.
     *
     * @param AttributeName The name of the attribute to set.
     */
    void removeAttribute(const std::string& AttributeName);

    /**
     * @brief Test if this PolygonEdge is coincident with an other PolygonEdge
     *
     * @param Edge The PolygonEdge to test the coincidence
     * @return True if the two PolygonEdge are coincidents, false otherwise.
     */
    bool isCoincident(PolygonEdge *Edge);

};

} // namespace landr
} /* namespace openfluid */
#endif /* POLYGONEDGE_HPP_ */
