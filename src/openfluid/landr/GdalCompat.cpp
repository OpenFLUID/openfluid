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
  @file GdalCompat.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/

#include <gdal_version.h>
#include <openfluid/landr/GdalCompat.hpp>


namespace openfluid { namespace landr {


GEOSGeom convertOGRGeometryToGEOS(const OGRGeometry* Geometry)
{
#if GDAL_VERSION_MAJOR > 1 || (GDAL_VERSION_MAJOR == 1 && (GDAL_VERSION_MINOR > 11 || (GDAL_VERSION_MINOR == 11 )))
  GEOSContextHandle_t ContextHandle = OGRGeometry::createGEOSContext();
  GEOSGeom TmpGeom = Geometry->exportToGEOS(ContextHandle);
  OGRGeometry::freeGEOSContext(ContextHandle);
  return TmpGeom;
#else
  return Geometry->exportToGEOS();
#endif
}


// =====================================================================
// =====================================================================


OGRGeometry* convertGEOSGeometryToOGR(const GEOSGeom Geometry)
{
#if GDAL_VERSION_MAJOR > 1 || (GDAL_VERSION_MAJOR == 1 && (GDAL_VERSION_MINOR > 11 || (GDAL_VERSION_MINOR == 11 )))
  GEOSContextHandle_t ContextHandle = OGRGeometry::createGEOSContext();
  OGRGeometry* TmpGeom = OGRGeometryFactory::createFromGEOS(ContextHandle,Geometry);
  OGRGeometry::freeGEOSContext(ContextHandle);
  return TmpGeom;
#else
    return OGRGeometryFactory::createFromGEOS(Geometry);
#endif
}


} }  // namespaces

