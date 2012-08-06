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
 \file LandRGraph.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef LANDRGRAPH_HPP_
#define LANDRGRAPH_HPP_

#include <geos/planargraph/PlanarGraph.h>

#include <ogrsf_frmts.h>

namespace geos {
namespace geom {
class Geometry;
class GeometryFactory;
class LineString;
class Polygon;
class Coordinate;
}
}

namespace planargraph {
class Node;
}

namespace openfluid {

namespace core {
class GeoVectorValue;
class GeoRasterValue;
}

namespace landr {

class LandREntity;

class LandRGraph: public geos::planargraph::PlanarGraph
{
  protected:

    openfluid::core::GeoVectorValue* mp_Vector;

    const geos::geom::GeometryFactory* mp_Factory;

    std::map<int, LandREntity*> m_EntitiesBySelfId;

    //TODO change for a list
    std::vector<LandREntity*> m_Entities;

    openfluid::core::GeoRasterValue* mp_Raster;

    openfluid::core::GeoVectorValue* mp_RasterPolygonized;

    std::vector<geos::geom::Polygon*>* mp_RasterPolygonizedPolys;

    static int FileNum;

    LandRGraph();

    LandRGraph(openfluid::core::GeoVectorValue&);

    virtual LandRGraph* clone() = 0;

    void addEntitiesFromGeoVector();

    void addEntitiesFromEntityList(const std::vector<LandREntity*>& Entities);

    virtual void addEntity(LandREntity* Entity) = 0;

    virtual LandREntity* getNewEntity(const geos::geom::Geometry* Geom,
                                      unsigned int SelfId) = 0;

    geos::planargraph::Node* getNode(const geos::geom::Coordinate& Coordinate);

    void addAttribute(std::string AttributeName, LandREntity& Entity);

    void removeAttribute(std::string AttributeName, LandREntity& Entity);

  public:

    /**
     * Delete also associated RasterPolygonized if present.
     */
    virtual ~LandRGraph();

    virtual LandREntity* getEntity(int SelfId);

    std::vector<LandREntity*> getEntities();

    std::vector<LandREntity*> getSelfIdOrderedEntities();

    std::map<int, LandREntity*> getEntitiesBySelfId();

    unsigned int getSize();

    /**
     * @brief Removes from this Graph the nodes of degree 0.
     */
    void removeUnusedNodes();

    /**
     * Doesn't reset if the AttributeName already exists.
     */
    void addAttribute(std::string AttributeName);

    /**
     * Does nothing if AttributeName doesn't exist.
     */
    void removeAttribute(std::string AttributeName);

    std::vector<std::string> getAttributeNames();

    /**
     * Replace associated raster if exists.
     */
    void addAGeoRasterValue(openfluid::core::GeoRasterValue& Raster);

    openfluid::core::GeoVectorValue* getRasterPolygonized();

    std::vector<geos::geom::Polygon*>* getRasterPolygonizedPolys();

    /**
     * @brief Fetch the raster value corresponding to the entity centroid coordinate.
     *
     * @param Entity The LandREntity to get the centroid coordinate from.
     * @return The raster value corresponding to the Entity centroid coordinate.
     */
    virtual float* getRasterValueForEntityCentroid(const LandREntity& Entity);

    /**
     * @brief Create a new attribute for this Graph entities, and set for each entity
     * this attribute value as the raster value corresponding to the entity centroid coordinate.
     *
     * @param AttributeName The name of the attribute to create
     */
    void setAttributeFromRasterValueAtCentroid(std::string AttributeName);

};

} // namespace landr
} /* namespace openfluid */
#endif /* LANDRGRAPH_HPP_ */
