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
  @file GeoRasterValue.hpp

  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CORE_GEORASTERVALUE_HPP__
#define __OPENFLUID_CORE_GEORASTERVALUE_HPP__


#include <gdal_priv.h>
#include <cpl_conv.h> // for CPLMalloc()

#include <openfluid/core/GeoValue.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {


/**
 Container class for geospatial raster data represented by a GDAL dataset.
*/
class OPENFLUID_API GeoRasterValue: public openfluid::core::GeoValue
{

  protected:

    /**
      The GDALDataset associated to this GeoRasterValue.
    */
    GDALDataset* mp_Data;

    /**
      Open the GDALDataset of this GeoRasterValue.
      @throw openfluid::base::FrameworkException if GDAL doesn't succeed to open the datasource.
    */
    void tryToOpenSource();

  public:

    /**
      Creates a new value.
      The <tt>FileName</tt> may be the name of a .jpeg, .tiff, .img or .asc file...
      It doesn't open the associated GDAL dataset.
      @param[in] FilePath The path of the file(s).
      @param[in] FileName The name or the relative path of the file to open.
    */
    GeoRasterValue(const std::string& FilePath, const std::string& FileName);

    /**
      Closes the opened GDAL dataset.
    */
    ~GeoRasterValue();

    /**
      Returns the type of this GeoRasterValue.
      @return An openfluid::core::UnstructuredValue::UnstructuredType.
    */
    openfluid::core::UnstructuredValue::UnstructuredType getType() const;

    /**
      Gets the associated opened GDAL dataset in read-only access.
      If the dataset is not already opened, tries to open it first.
      @return The opened GDAL dataset.
      @throw openfluid::base::FrameworkException if GDAL doesn't succeed to open the dataset.
    */
    GDALDataset* data();

    /**
      Returns the absolute Path of this GeoRasterValue.
    */
    std::string getAbsolutePath();
};


} } // namespaces


#endif /* __OPENFLUID_CORE_GEORASTERVALUE_HPP__ */
