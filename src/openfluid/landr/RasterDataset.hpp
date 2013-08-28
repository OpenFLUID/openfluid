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
 \file RasterDataset.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef RASTERDATASET_HPP_
#define RASTERDATASET_HPP_

#include <map>
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include <openfluid/dllexport.hpp>

namespace geos {
namespace geom {
class Coordinate;
}
}
namespace openfluid {

namespace core {
class GeoRasterValue;
}

namespace landr {

class VectorDataset;

class DLLEXPORT RasterDataset
{
  private:

    GDALDataset* mp_Dataset;

    double* mp_GeoTransform;

    std::map<unsigned int, openfluid::landr::VectorDataset*> mp_PolygonizedByRasterBandIndex;

    void computeGeoTransform();

  public:

    /**
     * @brief Create a virtual (in memory) copy of Value GDALDataset
     * @param Value The GeoRasterValue to copy
     * @throw openfluid::base::OFException if fails
     */
    RasterDataset(openfluid::core::GeoRasterValue& Value);

    /**
     * @brief Copy constructor
     * @throw openfluid::base::OFException if fails
     */
    RasterDataset(const RasterDataset& Other);

    /**
     * @brief Delete the virtual GDALDataset
     */
    ~RasterDataset();

    GDALDataset* getDataset();

    GDALDataset* getDataset() const;

    /**
     * @brief Get the RasterBand indexed with RasterBandIndex of the dataset
     * Is owned by its dataset, should never be destroyed with the C++ delete operator
     *
     * @param RasterBandIndex The rasterBand to get, default 1
     */
    GDALRasterBand* getRasterBand(unsigned int RasterBandIndex = 1);

    std::pair<int, int> getPixelFromCoordinate(geos::geom::Coordinate Coo);

    geos::geom::Coordinate* getOrigin();

    double getPixelWidth();

    double getPixelHeight();

    std::vector<float> getValuesOfLine(int LineIndex,
                                       unsigned int RasterBandIndex = 1);

    std::vector<float> getValuesOfColumn(int ColIndex,
                                         unsigned int RasterBandIndex = 1);

    float getValueOfPixel(int ColIndex, int LineIndex,
                          unsigned int RasterBandIndex = 1);

    float getValueOfCoordinate(geos::geom::Coordinate Coo,
                               unsigned int RasterBandIndex = 1);

    /**
     * @brief Create a new VectorDataset with polygons for all connected regions of pixels in the raster sharing a common pixel value.
     * Use openfluid::landr::VectorDataset::copyToDisk() to keep this vectorDataset on disk
     *
     * @param FileName The name of the new VectorDataset.
     * @param FieldName The name of the field to be created for storing the pixel value, limited to 10 characters (or will be truncated).
     * Default is set to "PixelVal". Type of field is OFTReal .
     *
     * @return The newly created VectorDataset.
     */
    openfluid::landr::VectorDataset* polygonize(
        const std::string& FileName, std::string FieldName = "",
        unsigned int RasterBandIndex = 1);

    static std::string getDefaultPolygonizedFieldName();
};

}
} // namespaces

#endif /* RASTERDATASET_HPP_ */
