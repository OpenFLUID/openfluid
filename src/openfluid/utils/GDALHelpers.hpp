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
  @file GDALHelpers.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_UTILS_GDALHELPERS_HPP__
#define __OPENFLUID_UTILS_GDALHELPERS_HPP__


#include <vector>
#include <map>
#include <set>

#include <QString>

#include <ogr_api.h>

#include <openfluid/dllexport.hpp>
#include <openfluid/utils/GDALCompatibility.hpp>


namespace openfluid { namespace utils {

/**
  Structure associating a driver label with a list of corresponding files extensions
*/
struct GDALDriverInfos_t {
	  /** Label of the driver */
    std::string Label;

    /** Vector of files extensions */
    std::vector<std::string> FilesExts;
};


/**
  Map of drivercode => openfluid::utils::GDALDriverInfos_t
*/
typedef std::map<std::string,GDALDriverInfos_t> GDALDriversFilesExts_t;


// =====================================================================
// =====================================================================


/**
  Returns a list of OGR drivers available in OpenFLUID for GIS vector files
  @return an openfluid::utils::GDALDriversFilesExts_t
*/
const GDALDriversFilesExts_t OPENFLUID_API getOGRFilesDriversForOpenFLUID();


/**
  Returns a list of GDAL drivers available in OpenFLUID for GIS vector files
  @return an openfluid::utils::GDALDriversFilesExts_t
*/
const GDALDriversFilesExts_t OPENFLUID_API getGDALFilesDriversForOpenFLUID();


/**
  Returns a set of GIS vector files extensions available in OpenFLUID
  @return a set of files extensions
*/
const std::set<std::string> OPENFLUID_API getOGRFilesExtensionsForOpenFLUID();


/**
  Returns the available GDAL/OGR formats as a QFileDialog formats string.
  The string is built according to GDAL/OGR available drivers.
  The OGRRegisterAll() and/or GDALAllRegister() instruction must be called before using this function.
  @param[in] Drivers The available extensions list
  @param[in] AllFormatsLabel the label displayed for all formats
  @return the format string
  (e.g. "All vector files (*.shp *.mif *.tab);;ESRI Shapefiles (*.shp);;Mapinfo (*.mif *.tab)")
*/
QString OPENFLUID_API getOGRGDALFormatsForQFileDialogs(const GDALDriversFilesExts_t& Drivers,
                                                       const QString& AllFormatsLabel);


/**
  Returns true if the given field type is an OGR integer or an OGR 64bits integer,
  taking into account the compatibility with both GDAL 1.xx and 2.xx
  @param[in] FieldType the OGR field type
  @return true if the field type is integer
*/
inline bool OPENFLUID_API isOGRInteger(OGRFieldType FieldType)
{
  return (FieldType == OFTInteger) || (FieldType == GDALOFTInteger64_COMPAT);
}


} } // namespaces


#endif /* __OPENFLUID_UTILS_GDALHELPERS_HPP__ */
