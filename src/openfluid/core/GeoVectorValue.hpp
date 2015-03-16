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
  @file GeoVectorValue.hpp

  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
 */

#ifndef __OPENFLUID_CORE_GEOVECTORVALUE_HPP__
#define __OPENFLUID_CORE_GEOVECTORVALUE_HPP__

#include <openfluid/core/GeoValue.hpp>
#include <openfluid/dllexport.hpp>

#include <ogrsf_frmts.h>

namespace openfluid { namespace core {

/**
 * @brief Container class for geospatial vector data,
 * represented by an OGR datasource.
 */
class OPENFLUID_API GeoVectorValue: public openfluid::core::GeoValue
{
  protected:

    /**
     @brief The OGRDataSource associated to this GeoVectorValue.
     */
    OGRDataSource* mp_Data;

    /**
     @brief Open the OGRDataSource of this GeoVectorValue.
     @throw openfluid::base::FrameworkException if OGR doesn't succeed to open the datasource.
    */
    void tryToOpenSource();

    /**
     @brief Destroy the OGRDataSource.
    */
    void destroyDataSource();

  public:

    /**
     @brief Creates a new value.

     For ESRI Shapefile, the <tt>FileName</tt> may be the name of a .shp, .shx or .dbf file,
     or a path to a directory containing proper shape files.
     It doesn't open the associated OGR datasource.
     @param FilePath The path of the file(s).
     @param FileName The name or the relative path of the file to open.
     */
    GeoVectorValue(const std::string& FilePath, const std::string& FileName);

    /**
     @brief Closes the opened OGR datasource.
     */
    ~GeoVectorValue();

    /**
     @brief Returns the type of this GeoVectorValue.
     */
    openfluid::core::UnstructuredValue::UnstructuredType getType() const;

    /**
     @brief Gets the associated opened OGR datasource in read-only access.
     If the datasource is not already opened, tries to open it first.
     @return The opened OGR datasource.
     @throw openfluid::base::FrameworkException if OGR doesn't succeed to open the datasource.
     */
    OGRDataSource* data();

    /**
     @brief Gets a layer of the shape.
     @param LayerIndex The index of the asked layer, default 0.
     @return The layer indexed LayerIndex.
     @throw openfluid::base::FrameworkException if OGR doesn't succeed to open the datasource.
     */
    OGRLayer* layer(unsigned int LayerIndex = 0);

    /**
     @brief Gets the Feature definition of a layer.
     @param LayerIndex The index of the asked layer definition, default 0.
     @return The OGR Feature definition of the LayerIndex layer.
     @throw openfluid::base::FrameworkException if OGR doesn't succeed to open the datasource.
     */
    OGRFeatureDefn* layerDef(unsigned int LayerIndex = 0);

    /**
     @brief Returns true if the GeoVectorValue is line type, false otherwise.
     @param LayerIndex The index of the layer to compare the type, default 0.
     @return True if the type of the layer LayerIndex is wkbLineString, false otherwise.
     @throw openfluid::base::FrameworkException if OGR doesn't succeed to open the datasource.
      */
     bool isLineType(unsigned int LayerIndex = 0);

     /**
      @brief Returns true if the GeoVectorValue is polygon type, false otherwise.
      @param LayerIndex The index of the layer to compare the type, default 0.
      @return True if the type of the layer LayerIndex is wkbPolygon, false otherwise.
      @throw openfluid::base::FrameworkException if OGR doesn't succeed to open the datasource.
      */
     bool isPolygonType(unsigned int LayerIndex = 0);

     /**
      @brief Returns true if a field exists in the LayerIndex layer.
      @param FieldName The name of the field to query.
      @param LayerIndex The index of the layer to query, default 0.
      @return True if the field FieldName exists, False otherwise.
      @throw openfluid::base::FrameworkException if OGR doesn't succeed to open the datasource.
      */
     bool containsField(const std::string& FieldName, unsigned int LayerIndex = 0);

     /**
      @brief Gets the index of a field in the LayerIndex layer.
      @param LayerIndex The index of the layer to query, default 0.
      @param FieldName The name of the field to query.
      @return The index of FieldName or -1 if field FieldName doesn't exist.
      @throw openfluid::base::FrameworkException if OGR doesn't succeed to open the datasource.
      */
     int getFieldIndex(const std::string& FieldName, unsigned int LayerIndex = 0);

     /**
      @brief Returns true if a field is of the type FieldType in the LayerIndex layer.
      @param FieldName The name of the field to query.
      @param FieldType The type of the field to query.
      @param LayerIndex The index of the layer to query, default 0.
      @return True if the field FieldName is type FieldType.
      @throw openfluid::base::FrameworkException if the field doesn't exist.
      @throw openfluid::base::FrameworkException if OGR doesn't succeed to open the datasource.
      */
     bool isFieldOfType(const std::string& FieldName, OGRFieldType FieldType,
                        unsigned int LayerIndex = 0);

     /**
      @brief Returns true if the GeoVectorValue is point type, false otherwise.
      @param LayerIndex The index of the layer to compare the type, default 0.
      @return True if the type of the layer LayerIndex is wkbPoint, false otherwise.
      @throw openfluid::base::FrameworkException if OGR doesn't succeed to open the datasource.
      */
     bool isPointType(unsigned int LayerIndex = 0);

     /**
      @brief Returns true if the GeoVectorValue is MultiPolygon type, false otherwise.
      @param LayerIndex The index of the layer to compare the type, default 0.
      @return True if the type of the layer LayerIndex is wkbMultiPolygon, false otherwise.
      @throw openfluid::base::FrameworkException if OGR doesn't succeed to open the datasource.
      */
     bool isMultiPolygonType(unsigned int LayerIndex = 0);

     /**
      @brief Returns true if the GeoVectorValue is MultiLine type, false otherwise.
      @param LayerIndex The index of the layer to compare the type, default 0.
      @return True if the type of the layer LayerIndex is wkbMultiLineString, false otherwise.
      @throw openfluid::base::FrameworkException if OGR doesn't succeed to open the datasource.
      */
     bool isMultiLineType(unsigned int LayerIndex = 0);

     /**
      @brief Returns true if the GeoVectorValue is MultiPoint type, false otherwise.
      @param LayerIndex The index of the layer to compare the type, default 0.
      @return True if the type of the layer LayerIndex is wkbMultiPoint, false otherwise.
      @throw openfluid::base::FrameworkException if OGR doesn't succeed to open the datasource.
      */
     bool isMultiPointType(unsigned int LayerIndex = 0);




};

} } // namespaces


#endif /* __OPENFLUID_CORE_GEOVECTORVALUE_HPP__ */
