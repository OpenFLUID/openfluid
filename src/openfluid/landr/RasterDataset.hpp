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
/**
 * @brief Interface for managing Raster Data format.
 *
 */
class DLLEXPORT RasterDataset
{
  private:

    /**
     * @brief The GDALDataset associated to this RasterDataset.
     */
    GDALDataset* mp_Dataset;

    /**
     * @brief The affine transformation coefficients of this RasterDataset.
     */
    double* mp_GeoTransform;

    /**
     * @brief A map of the related VectorDataset to this RasterDataset.
     */
    std::map<unsigned int, openfluid::landr::VectorDataset*> mp_PolygonizedByRasterBandIndex;

    /**
     * @brief Computes the affine transformation coefficients of this RasterDataset.
     */
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

    /**
     * @brief Returns the GDALDataset related to this RasterDataset.
     */
    GDALDataset* getDataset();

    /**
     * @brief Returns the const GDALDataset related to this RasterDataset.
     */
    GDALDataset* getDataset() const;

    /**
     * @brief Gets the RasterBand indexed with RasterBandIndex of the dataset.
     * @details Is owned by its dataset, should never be destroyed with the C++ delete operator.
     *
     * @param RasterBandIndex The rasterBand to get, default 1
     */
    GDALRasterBand* getRasterBand(unsigned int RasterBandIndex = 1);

    /**
     * @brief Returns the column and line index of a pixel from the coordinate of the pixel.
     * @param Coo A geos::geom::Coordinate.
     * @return A pair of the column and line index of the pixel in this RasterDataset.
     */
    std::pair<int, int> getPixelFromCoordinate(geos::geom::Coordinate Coo);

    /**
     * @brief Returns the geos::geom::Coordinate origin of this RasterDataset.
     */
    geos::geom::Coordinate* getOrigin();

    /**
     * @brief Returns the pixel width of this RasterDataset.
     */
    double getPixelWidth();

    /**
     * @brief Returns the pixel height of this RasterDataset.
     */
    double getPixelHeight();

    /**
     * @brief Returns a vector of the pixel values of a line of this RasterDataset.
     * @param LineIndex The line index to get the pixel values.
     * @param RasterBandIndex The raster band index (default is 1).
     * @return A vector of pixel values.
     */
    std::vector<float> getValuesOfLine(int LineIndex,
                                       unsigned int RasterBandIndex = 1);
    /**
     * @brief Returns a vector of the pixel values of a column of this RasterDataset.
     * @param LineIndex The column index to get the pixel values.
     * @param RasterBandIndex The raster band index (default is 1).
     * @return A vector of pixel values.
     */
    std::vector<float> getValuesOfColumn(int ColIndex,
                                         unsigned int RasterBandIndex = 1);

    /**
     * @brief Returns the pixel value with column and line index.
     * @param ColIndex The column index.
     * @param LineIndex The line index.
     * @param RasterBandIndex The raster band index (default is 1).
     * @return The pixel value.
     */
    float getValueOfPixel(int ColIndex, int LineIndex,
                          unsigned int RasterBandIndex = 1);

    /**
     * @brief Returns the pixel value with coordinate.
     * @param Coo The geos::geom::Coordinate.
     * @param RasterBandIndex The raster band index (default is 1).
     * @return The pixel value.
     */
    float getValueOfCoordinate(geos::geom::Coordinate Coo,
                               unsigned int RasterBandIndex = 1);

    /**
     * @brief Creates a new VectorDataset with polygons for all connected regions of pixels in the raster sharing a common pixel value.
     * @details Use openfluid::landr::VectorDataset::copyToDisk() to keep this vectorDataset on disk
     *
     * @param FileName The name of the new VectorDataset.
     * @param FieldName The name of the field to be created for storing the pixel value, limited to 10 characters (or will be truncated).
     * Default is set to "PixelVal". Type of field is OFTReal .
     * @param RasterBandIndex The raster band index (default is 1).
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
