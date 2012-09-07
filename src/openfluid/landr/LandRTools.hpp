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
 \file LandRTools.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef LANDRTOOLS_HPP_
#define LANDRTOOLS_HPP_

#include <vector>

namespace geos {
namespace geom {
class Geometry;
class LineString;
class Polygon;
}
}

namespace openfluid {

namespace core {
class GeoVectorValue;
}

namespace landr {

class LandRTools
{
  public:

    enum Relationship
    {
      NONE, INTERSECTS, CONTAINS, EQUALS
    };

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
     * @brief Return all exterior rings of the polygon-typed GeoVectorValue
     *
     * @param Val A GeoVectorValue of polygons
     * @return A vector of new allocated Geometries representing exterior rings
     * @throw An OFException if the GeoVectorValue is not polygon-typed
     */
    static std::vector<geos::geom::Geometry*> getVectorOfExteriorRings(
        openfluid::core::GeoVectorValue& Val);

    /**
     * @brief Return all lines composing of the linestring-typed GeoVectorValue
     *
     * @param Val A GeoVectorValue of linestrings
     * @return A vector of new allocated Geometries representing lines
     * @throw An OFException if the GeoVectorValue is not linestring-typed
     */
    static std::vector<geos::geom::Geometry*> getVectorOfLines(
        openfluid::core::GeoVectorValue& Val);

    /**
     * @brief Get all full noded lines of a set of Geometries
     *
     * @param Geoms A vector of Geometries
     * @return A collection of lines, representing all input lines, cut at each node
     */
    static geos::geom::Geometry* getNodedLines(
        std::vector<geos::geom::Geometry*> Geoms);

    static std::vector<geos::geom::Polygon*>* getPolygonizedGeometry(
        geos::geom::Geometry* Geom);
};

}
} /* namespace openfluid */
#endif /* LANDRTOOLS_HPP_ */
