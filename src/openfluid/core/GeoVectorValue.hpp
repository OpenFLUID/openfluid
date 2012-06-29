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
 \file GeoVectorValue.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __GEOVECTORVALUE_HPP__
#define __GEOVECTORVALUE_HPP__

#include <openfluid/core/GeoValue.hpp>

#include <ogrsf_frmts.h>

namespace openfluid {
namespace core {

/**
 * @brief Container class for geospatial vector data,
 * represented by an OGR datasource.
 */
class GeoVectorValue: public openfluid::core::GeoValue
{
  protected:

    std::string mp_ShpDriverName;

    /**
     * Don't use it directly, use tryToGetShpDriver() instead.
     */
    OGRSFDriver* mp_ShpDriver;

    OGRDataSource* mp_Data;

    /**
     * Don't use it directly, use getLayer() instead.
     */
    OGRLayer* mp_Layer;

    /**
     * Don't use it directly, use getLayerDef() instead.
     */
    OGRFeatureDefn* mp_LayerDef;

    void tryToOpenSource(bool UpdateMode);

    OGRSFDriver* tryToGetShpDriver();

    void destroyDataSource();

  public:

    /**
     * @brief Creates a new value.
     *
     * The <tt>FileName</tt> may be the name of a .shp, .shx or .dbf file,
     * or a path to a directory containing proper shape files.
     *
     * It doesn't open the associated OGR datasource.
     *
     * @param FilePath The path of the file(s)
     * @param FileName The name or the relative path of the file to open, or the name of the file to create
     */
    GeoVectorValue(std::string FilePath, std::string FileName);

    /**
     * @brief Closes the opened OGR datasource.
     */
    ~GeoVectorValue();

    openfluid::core::UnstructuredValue::UnstructuredType getType() const;

    /**
     * @brief Gets the associated opened OGR datasource.
     *
     * If the datasource is not already opened, tries to open it first.
     *
     * @param UpdateMode False for read-only access (the default) or True for read-write access.
     * @return The opened OGR datasource.
     * @throw openfluid::base::OFException if OGR doesn't succeed to open the datasource.
     * @throw openfluid::base::OFException if OGR doesn't succeed to open the first layer of datasource.
     */
    OGRDataSource* get(bool UpdateMode = false);

    /**
     * @brief Creates an empty new shapefile with a layer of type LayerType, with Read-Write permission.
     *
     * @param LayerType The type of the layer (default wkbUnknown).
     * @param SpatialRef The coordinate system to use for the new layer, or NULL (default) if no coordinate system is available.
     * @param ReplaceIfExists True if an existing file has to be deleted before the new one creation (default).
     * @throw openfluid::base::OFException if ReplaceIfExists set to False and a shape file with the same name already exists.
     * @throw openfluid::base::OFException if the creation of output file failed.
     * @throw openfluid::base::OFException if the creation of first layer failed.
     */
    void createShp(OGRwkbGeometryType LayerType = wkbUnknown,
                   OGRSpatialReference* SpatialRef = NULL,
                   bool ReplaceIfExists = true);

    /**
     * @brief Creates a new shapefile with a layer which is a copy of the Sources layer, with Read-Write permission..
     *
     * @param Source The GeoVectorValue containing the layer to copy (may come from another dataset).
     * @param ReplaceIfExists True if an existing file has to be deleted before the new one creation (default).
     * @throw openfluid::base::OFException if ReplaceIfExists set to False and a shape file with the same name already exists.
     * @throw openfluid::base::OFException if the creation of output file failed.
     * @throw openfluid::base::OFException if the creation of first layer failed.
     */
    void copyShp(GeoVectorValue& Source, bool ReplaceIfExists = true);

    /**
     * @brief Returns if the file already exists.
     *
     * @return True if the file already exists, false otherwise.
     */
    bool isAlreadyExisting();

    /**
     * @brief Delete the shape file(s) on disk, if exists. Else does nothing.
     */
    void deleteShpOnDisk();

    /**
     * @brief Add a field to the first (default) layer.
     *
     * @param FieldName The name of the field to add.
     * @param FieldType The type of the field to add (default OFTString).
     * @throw openfluid::base::OFException if creating field failed.
     */
    void addAField(std::string FieldName, OGRFieldType FieldType = OFTString);

    /**
     * @brief Get the first layer of the shape.
     *
     * @return The layer index 0.
     */
    OGRLayer* getLayer0();

    /**
     * @brief Get the Feature definition of the first (default) layer.
     *
     * @return The OGR Feature definition of the first (default) layer.
     */
    OGRFeatureDefn* getLayerDef();

    bool isLineType();

    bool isPolygonType();

    /**
     * @brief Returns if a field exists in the first (default) layer.
     *
     * @param FieldName The name of the field to query
     * @return True if the field FieldName exists, False otherwise
     */
    bool containsField(std::string FieldName);

    /**
     * @brief Get the index of a field.
     *
     * @param FieldName The name of the field to query
     * @return -1 if field FieldName doesn't exist
     */
    int getFieldIndex(std::string FieldName);

    /**
     * @brief Returns if a field is of the type FieldType.
     *
     * @param FieldName The name of the field to query
     * @param FieldType The type of the field to query
     * @return True if the field FieldName is type FieldType.
     * @throw openfluid::base::OFException if the field doesn't exist.
     */
    bool isFieldOfType(std::string FieldName, OGRFieldType FieldType);

    /**
     * @brief Returns if a field has the value Value.
     *
     * @param FieldName The name of the field to query
     * @param Value The value to query
     * @return True if the field has at least a feature containing the value Value, False otherwise.
     */
    bool isIntValueSet(std::string FieldName, int Value);

};

}
} // namespaces

#endif /* __GEOVECTORVALUE_HPP__ */
