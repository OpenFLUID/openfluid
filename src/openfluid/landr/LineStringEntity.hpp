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
  @file LineStringEntity.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Michael RABOTIN <michael.rabotin@supagro.inra.fr>
 */

#ifndef __OPENFLUID_LANDR_LINESTRINGENTITY_HPP__
#define __OPENFLUID_LANDR_LINESTRINGENTITY_HPP__

#include <openfluid/landr/LandREntity.hpp>
#include <openfluid/dllexport.hpp>
#include <geos/planargraph/Edge.h>


namespace geos { namespace geom {
class LineString;
} }

namespace openfluid { namespace landr {

/**
  @brief A LandREntity representing a geos::geom::LineString.
  @details A LineStringEntity has a StartNode and an EndNode, relatives to its LineString orientation.
*/
class OPENFLUID_API LineStringEntity: public LandREntity, public geos::planargraph::Edge
{
  private:

    const geos::geom::LineString* mp_Line;

    /**
      @brief Up neighbours of LineStringEntity type, according to the LineString orientation.
      @details An up neighbour is another LineStringEntity whose EndNode is this LineStringEntity StartNode.
    */
    std::vector<LineStringEntity*>* mp_LOUpNeighbours;

    /**
      @brief Down neighbours of LineStringEntity type, according to the LineString orientation.
      @details A down neighbour is another LineStringEntity whose StartNode is this LineStringEntity EndNode.
    */
    std::vector<LineStringEntity*>* mp_LODownNeighbours;

    LineStringEntity();
    LineStringEntity(const LineStringEntity&);

    /**
      @brief Computes the down neighbour of this LineStringEntity.
    */
    void computeLineOrientUpNeighbours();

    /**
      @brief Computes the up neighbour of this LineStringEntity.
    */
    void computeLineOrientDownNeighbours();

  public:

    /**
      @brief Creates a new LineStringEntity.
      @details Takes ownership of NewLine.
      @throw base::FrameworkException if NewLine is not a geos::geom::LineString or is an empty geometry.
    */
    LineStringEntity(const geos::geom::Geometry* NewLine, unsigned int OfldId);

    virtual ~LineStringEntity();

    /**
      @brief Clones a LineStringEntity into a new LineStringEntity.
    */
    LineStringEntity* clone();

    /**
      @brief Returns the geos::geom::LineString of this LineStringEntity.
    */
    const geos::geom::LineString* line() const;

    /**
      @brief Returns the start geos::planargraph::Node of this LineStringEntity.
    */
    geos::planargraph::Node* startNode();

    /**
      @brief Returns the end geos::planargraph::Node of this LineStringEntity.
    */
    geos::planargraph::Node* endNode();

    /**
      @brief Return a vector of up-neighbours LineStringEntity using the line orientation of this LineStringEntity.
    */
    std::vector<LineStringEntity*> getLineOrientUpNeighbours();

    /**
      @brief Return a vector of down-neighbours LineStringEntity using the line orientation of this LineStringEntity.
    */
    std::vector<LineStringEntity*> getLineOrientDownNeighbours();

    /**
      @brief Compute the neighbours using line orientation of this LineStringEntity.
    */
    void computeNeighbours();

    /**
      @brief Returns a vector of LineStringEntity down and upneighbours with NodeDegree=2.
      @return A vector of LineStringEntity.
    */
    std::vector<LineStringEntity*>  getLineNeighboursDegree2();

};

} } // namespace landr, openfluid

#endif /* __OPENFLUID_LANDR_LINESTRINGENTITY_HPP__ */
