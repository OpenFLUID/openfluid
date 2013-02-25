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
 \file GeoRasterValue.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __GEORASTERVALUE_HPP__
#define __GEORASTERVALUE_HPP__

#include <openfluid/core/GeoValue.hpp>

#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
namespace geos {
namespace geom {
class Coordinate;
}
}

namespace openfluid {

//namespace landr {
//class VectorDataset;
//}

namespace core {

/**
 * @brief Container class for geospatial raster data,
 * represented by a GDAL dataset.
 */
class GeoRasterValue: public openfluid::core::GeoValue
{

  protected:

    GDALDataset* mp_Data;

    /**
     * Owned by its dataset, should never be destroyed with the C++ delete operator.
     */
    GDALRasterBand* mp_RasterBand1;

    double* mp_GeoTransform;

//    openfluid::landr::VectorDataset* mp_Polygonized;

    void tryToOpenSource(bool UpdateMode);

    void computeGeoTransform();

  public:

    /**
     * @brief Creates a new value.
     *
     * The <tt>RelativePath</tt> may be path to a .jpeg, .tiff, .img or .asc file...
     *
     * It doesn't open the associated GDAL dataset.
     *
     * @param RelativePath The path of the data, relative to the PrefixPath.
     */
    GeoRasterValue(std::string FilePath, std::string FileName);

    /**
     * @brief Closes the opened GDAL dataset.
     */
    ~GeoRasterValue();

    openfluid::core::UnstructuredValue::UnstructuredType getType() const;

    /**
     * @brief Gets the associated opened GDAL dataset.
     *
     * If the dataset is not already opened, tries to open it first.
     *
     * @param UpdateMode False for read-only access (the default) or True for read-write access.
     * @return The opened GDAL dataset.
     * @throw openfluid::base::OFException if GDAL doesn't succeed to open the dataset.
     */
    GDALDataset* get(bool UpdateMode = false);

    /**
     * Get the first RasterBand (indexed with 1) of the dataset.
     * Is owned by its dataset, should never be destroyed with the C++ delete operator.
     */
    GDALRasterBand* getRasterBand1();

    std::pair<int, int> getPixelFromCoordinate(geos::geom::Coordinate Coo);

    geos::geom::Coordinate* getOrigin();

    double getPixelWidth();

    double getPixelHeight();

    std::vector<float> getValuesOfLine(int LineIndex);

    std::vector<float> getValuesOfColumn(int ColIndex);

    float getValueOfPixel(int ColIndex, int LineIndex);

    float getValueOfCoordinate(geos::geom::Coordinate Coo);

    /**
     * @brief Create a new VectorDataset with polygons for all connected regions of pixels in the raster sharing a common pixel value.
     * Use openfluid::landr::VectorDataset::copyToDisk() to keep this vectorDataset on disk
     *
     * @param FileName The name of the new VectorDataset.
     * @param FieldName The name of the field to be created for storing the pixel value, limited to 10 characters (or will be truncated).
     * Default is set to "PixelVal". Type of field is OFTInteger (float pixel values are rounded).
     *
     * @return The newly created VectorDataset.
     */
//    openfluid::landr::VectorDataset* polygonize(std::string FileName,
//                                                std::string FieldName = "");

    static std::string getDefaultPolygonizedFieldName();

};

}
} // namespaces
#endif /* __GEORASTERVALUE_HPP__ */
