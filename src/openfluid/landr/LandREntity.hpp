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
 \file LandREntity.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef LANDRENTITY_HPP_
#define LANDRENTITY_HPP_

#include <geos/planargraph/GraphComponent.h>
#include <map>
#include <set>
#include <string>
#include <openfluid/dllexport.hpp>

namespace geos {
namespace geom {
class Geometry;
class Point;
}
}

namespace openfluid {

namespace core {
class Value;

}
namespace landr {

/**
 * @brief Interface for a landscape representation element.
 */
class DLLEXPORT LandREntity: public geos::planargraph::GraphComponent
{
  private:

    LandREntity();

    LandREntity(const LandREntity&);

  protected:
    /**
     * @brief The geos::geom::Geometry of this LandREntity.
     */
    const geos::geom::Geometry* mp_Geom;

    /**
     * @brief The identifier of this LandREntity.
     */
    unsigned int m_OfldId;

    /**
     * @brief The centroid of this LandREntity.
     */
    geos::geom::Point* mp_Centroid;

    /**
     * @brief The area of this LandREntity.
     */
    double m_Area;

    /**
     * @brief The length of this LandREntity.
     */
    double m_Lenght;

    /**
     * @brief A set of LandREntity neighbours of this LandREntity.
     */
    std::set<LandREntity*>* mp_Neighbours;

    /**
     * @brief A map of attributes of this LandREntity.
     */

    std::map<std::string, core::Value*> m_Attributes;

    // for limiting access to m_Attributes creation/deletion to LandRGraph class
    friend class LandRGraph;

    /**
     * @brief Computes the neighbours of this LandREntity.
     */
    virtual void computeNeighbours() = 0;

  public:

    LandREntity(const geos::geom::Geometry* Geom, unsigned int OfldId);

    virtual ~LandREntity();

    virtual LandREntity* clone() = 0;

    /**
     * @brief Returns the geos::geom::Geometry of this LandREntity.
     */
    const geos::geom::Geometry* getGeometry();

    /**
     * @brief Returns the identifier of this LandREntity.
     */
    unsigned int getOfldId() const;

    /**
     * @brief Returns the centroid of this LandREntity.
     */
    geos::geom::Point* getCentroid() const;

    /**
     * @brief Returns the area of this LandREntity.
     */
    double getArea() const;

    /**
     * @brief Returns the length of this LandREntity.
     */
    double getLength() const;

    /**
     * @brief Returns a set of LandREntity neighbours of this LandREntity.
     */
    std::set<LandREntity*>* getNeighbours();

    /**
     * @brief Gets the value of an attribute.
     *
     * @param AttributeName The name of the attribute to get.
     * @param Value The core::Value to assign the attribute value.
     * @return True if the attribute exists, false otherwise.
     */
    bool getAttributeValue(const std::string& AttributeName, core::Value& Value) const;

    /**
     * @brief Sets the value of an attribute.
     * @details Takes the ownership of Value.
     *
     * @param AttributeName The name of the attribute to set.
     * @param Value The core::Value assign to the attribute value.
     * @return True if the attribute exists, false otherwise.
     */
    bool setAttributeValue(const std::string& AttributeName, const core::Value* Value);

    /**
     * @brief Gets the distance between this LandREntity centroid and Other LandREntity centroid.
     */
    double getDistCentroCentro(LandREntity& Other);

    /**
     * @brief Gets the LandREntity neighbour that has the minimum centroid-to-centroid distance.
     */
    LandREntity* getNeighbour_MinDistCentroCentro();

};

} // namespace landr
} /* namespace openfluid */
#endif /* LANDRENTITY_HPP_ */
