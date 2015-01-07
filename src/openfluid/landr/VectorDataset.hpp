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
 \file VectorDataset.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef __OPENFLUID_LANDR_VECTORDATASET_HPP__
#define __OPENFLUID_LANDR_VECTORDATASET_HPP__

#include <string>
#include <map>
#include <list>
#include <ogrsf_frmts.h>
#include <openfluid/dllexport.hpp>


namespace geos { namespace geom {
class Geometry;
} }

namespace openfluid {

namespace core {
class GeoVectorValue;
}

namespace landr {
/**
 @brief Interface for managing Vector Data format.
 *
 */

class OPENFLUID_API VectorDataset
{
  public:

    /**
     @brief A list of pair of OGRFeature and geos::geom::Geometry related to this VectorDataset entities.
     */
    typedef std::list<std::pair<OGRFeature*, geos::geom::Geometry*> > FeaturesList_t;

  private:

    /**
     @brief The OGRDataSource related to this VectorDataset.
     */
    OGRDataSource* mp_DataSource;

    /**
     @brief A list of all features of layers of this VectorDataset, indexed by layer index.
     */
    std::map<unsigned int, FeaturesList_t> m_Features;

    /**
     @brief A map of geos::geom::Geometry representing a collection of all the geometries of the layers of this VectorDataset, indexed by layer index.
     */
    std::map<unsigned int, geos::geom::Geometry*> m_Geometries;

    /**
     @brief Returns the path of this VectorDataset associated with time.
     */
    std::string getTimestampedPath(const std::string& OriginalFileName);

    /**
     @brief Returns the path of this VectorDataset.
     */
    std::string getInitializedTmpPath();

    /**
     @brief Returns true if this VectorDataset exists.
     @param Path The pathname to this VectorDataset.
     @return True if already exists, false otherwise.
     */
    bool isAlreadyExisting(const std::string& Path);

    /**
     @brief Parse the geometry of this VectorDataset.
     @param LayerIndex The index layer.
     */
    void parse(unsigned int LayerIndex);

  public:

    /**
     @brief Creates a new empty OGRDatasource in the openfluid temp directory, with filename suffixes with timestamp.
     @param FileName The name of the file to create.
     @throw openfluid::base::OFException if fails.
     */
    VectorDataset(const std::string& FileName);

    /**
     @brief Creates in the openfluid temp directory a copy of Value OGRDatasource,
     using Value filename suffixed with timestamp as filename.
     @param Value The GeoVectorValue to copy
     @throw openfluid::base::OFException if fails.
     */
    VectorDataset(openfluid::core::GeoVectorValue& Value);

    /**
     @brief Copy constructor.
     @throw openfluid::base::OFException if fails.
     */
    VectorDataset(const VectorDataset& Other);

    /**
     @brief Delete the OGRDatasource and relative files in openfluid temp directory.
     */
    ~VectorDataset();

    /**
     @brief Returns the OGRDataSource associated to this VectorDataset.
     */
    OGRDataSource* source();

    /**
     @brief Returns the const OGRDataSource associated to this VectorDataset.
     */
    OGRDataSource* source() const;

    /**
     @brief Write to disk a copy of the OGRDataSource.
     @param FilePath The path to the directory where writing, will be created if needed.
     @param FileName The name of the file to write.
     @param ReplaceIfExists If true and the file FilePath/FileName already exists, overwrite it.
     */
    void copyToDisk(const std::string& FilePath,
                    const std::string& FileName,
                    bool ReplaceIfExists);

    /**
     @brief Add to DataSource an empty new layer.
     *
     @param LayerName The name of the layer to create.
     @param LayerType The type of the layer to create, default wkbUnknown.
     @param SpatialRef The coordinate system to use for the new layer, or NULL (default) if no coordinate system is available.
     @throw openfluid::base::OFException if the creation of layer failed.
     */
    void addALayer(std::string LayerName = "",
                   OGRwkbGeometryType LayerType = wkbUnknown,
                   OGRSpatialReference* SpatialRef = NULL);

    /**
     @brief Get a layer of the shape.
     @param LayerIndex The index of the asked layer, default 0.
     @return The layer indexed LayerIndex.
     */
    OGRLayer* layer(unsigned int LayerIndex = 0);

    /**
     @brief Get the Feature definition of a layer.
     @param LayerIndex The index of the asked layer definition, default 0.
     @return The OGR Feature definition of the LayerIndex layer.
     */
    OGRFeatureDefn* layerDef(unsigned int LayerIndex = 0);

    /**
     @brief Add a field to a layer.
     @param FieldName The name of the field to add.
     @param FieldType The type of the field to add (default OFTString).
     @param LayerIndex The index of the layer to add the field, default 0.
     @throw openfluid::base::OFException if creating field failed.
     */
    void addAField(const std::string& FieldName,
                   OGRFieldType FieldType = OFTString,
                   unsigned int LayerIndex = 0);

    /**
     @brief Returns true if the VectorDataset is line type.
     @param LayerIndex The index of the layer to compare the type, default 0.
     @return True if the type of the layer LayerIndex is wkbLineString, false otherwise.
     */
    bool isLineType(unsigned int LayerIndex = 0);

    /**
     @brief Returns true if the VectorDataset is polygon type.
     @param LayerIndex The index of the layer to compare the type, default 0.
     @return True if the type of the layer LayerIndex is wkbPolygon, false otherwise.
     */
    bool isPolygonType(unsigned int LayerIndex = 0);

    /**
     @brief Returns true if a field exists in the LayerIndex layer.
     @param FieldName The name of the field to query.
     @param LayerIndex The index of the layer to query, default 0.
     @return True if the field FieldName exists, False otherwise.
     */
    bool containsField(const std::string& FieldName,
                       unsigned int LayerIndex = 0);

    /**
     @brief Get the index of a field in the LayerIndex layer.
     @param LayerIndex The index of the layer to query, default 0.
     @param FieldName The name of the field to query.
     @return The index of FieldName or -1 if field FieldName doesn't exist.
     */
    int getFieldIndex(const std::string& FieldName,
                      unsigned int LayerIndex = 0);

    /**
     @brief Returns true if a field is of the type FieldType in the LayerIndex layer.
     @param FieldName The name of the field to query.
     @param FieldType The type of the field to query.
     @param LayerIndex The index of the layer to query, default 0.
     @return True if the field FieldName is type FieldType.
     @throw openfluid::base::OFException if the field doesn't exist.
     */
    bool isFieldOfType(const std::string& FieldName,
                       OGRFieldType FieldType,
                       unsigned int LayerIndex = 0);

    /**
     @brief Returns true if a field has the value Value in the LayerIndex layer.
     @param FieldName The name of the field to query.
     @param Value The value to query.
     @param LayerIndex The index of the layer to query, default 0.
     @return True if the field has at least a feature containing the value Value, False otherwise.
     */
    bool isIntValueSet(const std::string& FieldName,
                       int Value,
                       unsigned int LayerIndex = 0);

    /**
     @brief Gets the list of all features of a layer of this GeoVectorValue.
     @param LayerIndex The index of the layer to query, default 0.
     @return A list of OGRFeature and geos::geom::Geometry of this VectorDataset.
     */
    FeaturesList_t getFeatures(unsigned int LayerIndex = 0);

    /**
     @brief Gets a geos::geom::Geometry representing a collection of all the geometries of the layer LayerIndex of this GeoVectorValue.
     @param LayerIndex The index of the layer to query, default 0.
     @return A geos::geom::Geometry.
     */
    geos::geom::Geometry* geometries(unsigned int LayerIndex = 0);

    /**
     @brief Returns true if the VectorDataset is point type.
     @param LayerIndex The index of the layer to compare the type, default 0.
     @return True if the type of the layer LayerIndex is wkbPoint, false otherwise.
     */
    bool isPointType(unsigned int LayerIndex = 0);



};

} } // namespaces openfluid, landr


#endif /* __OPENFLUID_LANDR_VECTORDATASET_HPP__ */
