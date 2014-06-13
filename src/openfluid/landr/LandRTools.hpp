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
 \file LandRTools.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef LANDRTOOLS_HPP_
#define LANDRTOOLS_HPP_

#include <vector>
#include <list>
#include <openfluid/landr/LineStringGraph.hpp>
#include <openfluid/dllexport.hpp>
#include <geos/geom/CoordinateArraySequenceFactory.h>

namespace geos {
namespace geom {
class Geometry;
class LineString;
class Polygon;
}
}

namespace openfluid {
namespace landr {

class VectorDataset;

/**
 * @brief Set of tools to manage LandR elements.
 */
class DLLEXPORT LandRTools
{
  public:

    enum Relationship
    {
      NONE, INTERSECTS, CONTAINS, EQUALS, TOUCHES
    };

    /**
     * @brief Returns a geos::geom::LineString representing the linearized input geos::geom::Geometry.
     *
     * @param Geom The geos::geom::Geometry to linearize.
     * @return A new allocated geos::geom::LineString representing the linearized input geos::geom::Geometry,
     * or 0 if the geos::geom::Geometry cannot be linearized into a single geos::geom::LineString.
     */
    static geos::geom::LineString* getMergedLineStringFromGeometry(
        geos::geom::Geometry* Geom);

    /**
     * @brief Returns a vector of geos::geom::LineString representing the linearized input geos::geom::Geometry.
     *
     * @param Geom The geos::geom::Geometry to linearize.
     * @return A new allocated vector of geos::geom::LineString representing the maximal linearized input geos::geom::Geometry,
     * or 0 if the geos::geom::Geometry is not \"Line\" typed.
     */
    static std::vector<geos::geom::LineString*>* getMergedLineStringsFromGeometry(
        geos::geom::Geometry* Geom);

    /**
     * @brief Returns all exterior rings of the polygon-typed VectorDataset.
     *
     * @param Val A VectorDataset of polygons.
     * @return A vector of new allocated geos::geom::LineString representing exterior rings.
     * @throw base::OFException if the VectorDataset is not polygon-typed.
     */
    static std::vector<geos::geom::LineString*> getVectorOfExteriorRings(
        openfluid::landr::VectorDataset& Val);

    /**
     * @brief Returns all geos::geom::LineString composing of the linestring-typed VectorDataset.
     *
     * @param Val A VectorDataset of linestrings.
     * @return A vector of new allocated geos::geom::Geometry representing lines.
     * @throw base::OFException if the VectorDataset is not linestring-typed.
     */
    static std::vector<geos::geom::LineString*> getVectorOfLines(
        openfluid::landr::VectorDataset& Val);

    /**
     * @brief Gets all full noded geos::geom::LineString from intersection between geom1 and geom2, with snap tolerance.
     *
     * @param Geom1 The geos::geom::Geometry to node with Geom2.
     * @param Geom2 The other geos::geom::Geometry.
     * @param SnapTolerance The tolerance to use while computing intersections and equality of lines.
     * @param PrecisionReducer The PrecisionModel value for reducing coordinates precision, default is 10000000 (7 digits).
     * @return A vector of geos::geom::LineString, representing all input lines, cut at each node.
     */
    static std::vector<geos::geom::LineString*>* getNodedLines(
        geos::geom::Geometry* Geom1, geos::geom::Geometry* Geom2,
        double SnapTolerance = 0,double PrecisionReducer=10000000);

    /**
     * @brief Same as from geos::operation::overlay::snap::SnapOverlayOp::Union(),
     * but with ability to use the wished snap tolerance value.
     *
     * @param Geom1 The geos::geom::Geometry to join with Geom2.
     * @param Geom2 The other geos::geom::Geometry.
     * @param SnapTolerance The tolerance to use, default is 0.
     * @return A new geos::geom::Geometry representing the union of Geom1 and Geom2 according to SnapTolerance value.
     */
    static geos::geom::Geometry* computeSnapOverlayUnion(
        geos::geom::Geometry& Geom1, geos::geom::Geometry& Geom2,
        double SnapTolerance = 0);


    /**
     * @brief Returns true if a geos::geom::LineString is exactly equals of an element of a list of geos::geom::LineString, up to a specified tolerance.
     * @param Line The geos::geom::LineString to compare.
     * @param RefLines The list of geos::geom::LineString to compare to.
     * @param Tolerance The tolerance to use.
     */
    static bool exists(geos::geom::LineString* Line,
                       std::list<geos::geom::LineString*> RefLines, double Tolerance = 0);

    /**
     * @brief Creates all possible geos::geom::Polygon from a geos::geom::Geometry.
     *
     * @param Lines The input vector of geos::geom::Geometry to polygonize.
     * @param Polygons The output vector of newly created geos::geom::Polygon.
     * @param Dangles The output vector of dangle geos::geom::LineString.
     */
    static void polygonizeGeometry(
        std::vector<geos::geom::Geometry*>& Lines,
        std::vector<geos::geom::Polygon*>& Polygons,
        std::vector<const geos::geom::LineString*>& Dangles);

    /**
     * @brief Recursive depth first search algorithm in a LineStringGraph and mark visited Nodes
     *
     * @param Node the begin geos::planargraph::Node of LineStringGraph
     */
    static void markVisitedNodesUsingDFS(geos::planargraph::Node* Node);

    /**
     * @brief Intersection of two geos::geom::Geometry of Polygons.
     *
     * @param Geom1 The geos::geom::Geometry to join with Geom2.
     * @param Geom2 The other geos::geom::Geometry.
     * @return A vector of geos::geom::Polygon representing the intersection of Geom1 and Geom2.
     */
    static std::vector<geos::geom::Polygon*> computeIntersectPolygons(
        geos::geom::Geometry* Geom1, geos::geom::Geometry* Geom2);


    /**
     * @brief Splits a geos::geom::LineString by a geos::geom::Point.
     *
     * @param Entity An existent geos::geom::LineString.
     * @param Point A geos::geom::Point.
     * @param SnapTolerance The threshold distance used to find Point on Line.
     * @return a vector of geos::geom::LineString,
     *  or empty vector if splitting operation is null.
     */
    static std::vector<geos::geom::LineString*> splitLineStringByPoint(geos::geom::LineString& Entity, geos::geom::Point& Point,double SnapTolerance);


    /**
     * @brief Recursively split operation on a geos::geom::LineString by a vector of geos::geom::Point.
     *
     * @param Entity An existent geos::geom::LineString.
     * @param Point A vector of geos::geom::Point.
     * @param SnapTolerance The threshold distance used to find Point on Line.
     * @param vLines the resulting vector of geos::geom::LineString splitted.
     * @param step iterator of the vector of Point; default is 0.
     */
    static void splitLineStringByPoints(geos::geom::LineString& Entity,std::vector<geos::geom::Point*>&Points,
                                             double SnapTolerance,std::vector<geos::geom::LineString*>&vLines,unsigned int step=0);

    /**
     * @brief Clean a vector of geos::geom::LineString (break at each intersection and remove duplicate geometry)
     *
     * @param vLines An existent vector of geos::geom::LineString.
     * @param SnapTolerance The tolerance to use.
     * @return a vector of geos::geom::LineString.
     */
    static std::vector<geos::geom::LineString*>* cleanLineStrings(
        std::vector<geos::geom::LineString*> vLines,double SnapTolerance);

};

}
} /* namespace openfluid */
#endif /* LANDRTOOLS_HPP_ */
