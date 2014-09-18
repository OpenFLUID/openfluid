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
  \file GDALHelpers.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __GDALHELPERS_HPP__
#define __GDALHELPERS_HPP__

#include <openfluid/dllexport.hpp>

#include <vector>
#include <map>
#include <set>
#include <QString>


namespace openfluid { namespace tools {

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
  Map of drivercode => openfluid::tools::GDALDriverInfos_t
*/
typedef std::map<std::string,GDALDriverInfos_t> GDALDriversFilesExts_t;


// =====================================================================
// =====================================================================

/**
  Returns a list of OGR drivers available in OpenFLUID for GIS vector files
  @return an openfluid::tools::GDALDriversFilesExts_t
*/
const GDALDriversFilesExts_t DLLEXPORT getOGRFilesDriversForOpenFLUID();


/**
  Returns a list of GDAL drivers available in OpenFLUID for GIS vector files
  @return an openfluid::tools::GDALDriversFilesExts_t
*/
const GDALDriversFilesExts_t DLLEXPORT getGDALFilesDriversForOpenFLUID();


/**
  Returns a set of GIS vector files extensions available in OpenFLUID
  @return a set of files extensions
*/
const std::set<std::string> DLLEXPORT getOGRFilesExtensionsForOpenFLUID();


} } // namespaces


#endif /* __GDALHELPERS_HPP__ */
