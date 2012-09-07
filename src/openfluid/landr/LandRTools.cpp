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
 \file Tools.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "LandRTools.hpp"

#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/base/OFException.hpp>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/operation/linemerge/LineMerger.h>
#include <geos/operation/polygonize/Polygonizer.h>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

geos::geom::LineString* LandRTools::getMergedLineStringFromGeometry(
    geos::geom::Geometry* Geom)
{
  geos::geom::LineString* LS = 0;

  std::vector<geos::geom::LineString*>* Lines =
      getMergedLineStringsFromGeometry(Geom);

  if (!Lines || Lines->size() != 1)
  {
    for (unsigned int i = 0; i < Lines->size(); i++)
      delete Lines->at(i);
  } else
    LS = *Lines->begin();

  delete Lines;

  return LS;
}

// =====================================================================
// =====================================================================

std::vector<geos::geom::LineString*>* LandRTools::getMergedLineStringsFromGeometry(
    geos::geom::Geometry* Geom)
{
  std::vector<geos::geom::LineString*>* LS = 0;

  geos::operation::linemerge::LineMerger Merger;

  switch (Geom->getGeometryTypeId())
  {
    case geos::geom::GEOS_LINESTRING:
      LS = new std::vector<geos::geom::LineString*>();
      LS->push_back(dynamic_cast<geos::geom::LineString*>(Geom->clone()));
      break;
    case geos::geom::GEOS_MULTILINESTRING:
    case geos::geom::GEOS_LINEARRING:
    case geos::geom::GEOS_GEOMETRYCOLLECTION:
      Merger.add(Geom);
      LS = Merger.getMergedLineStrings();
      break;
    default:
      break;
  }

  return LS;
}

// =====================================================================
// =====================================================================

std::vector<geos::geom::Geometry*> LandRTools::getVectorOfExteriorRings(
    openfluid::core::GeoVectorValue& Val)
{
  if (!Val.isPolygonType())
    throw openfluid::base::OFException(
        "OpenFLUID Framework", "LandRTools::getVectorOfExteriorRings",
        " The GeoVectorValue is not polygon-typed.");

  // TODO move to... ?
  setlocale(LC_NUMERIC, "C");

  std::vector<geos::geom::Geometry*> Geoms;

  openfluid::core::GeoVectorValue::FeaturesList_t Features = Val.getFeatures();

  for (openfluid::core::GeoVectorValue::FeaturesList_t::iterator it =
      Features.begin(); it != Features.end(); ++it)
    Geoms.push_back(
        const_cast<geos::geom::LineString*>(dynamic_cast<geos::geom::Polygon*>(it->second)->getExteriorRing()));

  return Geoms;
}

// =====================================================================
// =====================================================================

std::vector<geos::geom::Geometry*> LandRTools::getVectorOfLines(
    openfluid::core::GeoVectorValue& Val)
{
  if (!Val.isLineType())
    throw openfluid::base::OFException(
        "OpenFLUID Framework", "LandRTools::getVectorOfLines",
        " The GeoVectorValue is not linestring-typed.");

  std::vector<geos::geom::Geometry*> Geoms;

  openfluid::core::GeoVectorValue::FeaturesList_t Features = Val.getFeatures();

  for (openfluid::core::GeoVectorValue::FeaturesList_t::iterator it =
      Features.begin(); it != Features.end(); ++it)
    Geoms.push_back(it->second);

  return Geoms;
}

// =====================================================================
// =====================================================================

geos::geom::Geometry* LandRTools::getNodedLines(
    std::vector<geos::geom::Geometry*> Geoms)
{
  const geos::geom::GeometryFactory* Factory =
      geos::geom::GeometryFactory::getDefaultInstance();

  geos::geom::Geometry* MultiLS = Factory->buildGeometry(Geoms);

  geos::geom::Point* MlsPt = Factory->createPoint(*MultiLS->getCoordinate());

  geos::geom::Geometry* NodedLines = MultiLS->Union(
      dynamic_cast<geos::geom::Geometry*>(MlsPt));

  return NodedLines;
}

// =====================================================================
// =====================================================================

std::vector<geos::geom::Polygon*>* LandRTools::getPolygonizedGeometry(
    geos::geom::Geometry* Geom)
{
  geos::operation::polygonize::Polygonizer* P =
      new geos::operation::polygonize::Polygonizer();

  P->add(Geom);

  std::vector<geos::geom::Polygon*>* Polys = P->getPolygons();

  delete P;

  return Polys;
}

// =====================================================================
// =====================================================================

}
} /* namespace openfluid */
