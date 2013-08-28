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

    const geos::geom::Geometry* mp_Geom;

    unsigned int m_SelfId;

    geos::geom::Point* mp_Centroid;

    double m_Area;

    double m_Lenght;

    std::set<LandREntity*>* mp_Neighbours;

    std::map<std::string, core::Value*> m_Attributes;

    // for limiting access to m_Attributes creation/deletion to LandRGraph class
    friend class LandRGraph;

    virtual void computeNeighbours() = 0;

  public:

    LandREntity(const geos::geom::Geometry* Geom, unsigned int SelfId);

    virtual ~LandREntity();

    virtual LandREntity* clone() = 0;

    const geos::geom::Geometry* getGeometry();

    unsigned int getSelfId() const;

    geos::geom::Point* getCentroid() const;

    double getArea() const;

    double getLength() const;

    std::set<LandREntity*>* getNeighbours();

    /**
     * @brief Get the value of an attribute.
     *
     * @param AttributeName The name of the attribute to get.
     * @param Value The core::Value to assign the attribute value.
     * @return True if the attribute exists, false otherwise.
     */
    bool getAttributeValue(const std::string& AttributeName, core::Value& Value) const;

    /**
     * @brief Set the value of an attribute.
     * @details Takes the ownership of Value.
     *
     * @param AttributeName The name of the attribute to set.
     * @param Value The core::Value assign to the attribute value.
     * @return True if the attribute exists, false otherwise.
     */
    bool setAttributeValue(const std::string& AttributeName, const core::Value* Value);

    /**
     * @brief Get the distance between this entity centroid and Other entity centroid.
     */
    double getDistCentroCentro(LandREntity& Other);

    /**
     * @brief Get the neighbour that has the minimum centroid-to-centroid distance.
     */
    LandREntity* getNeighbour_MinDistCentroCentro();

};

} // namespace landr
} /* namespace openfluid */
#endif /* LANDRENTITY_HPP_ */
