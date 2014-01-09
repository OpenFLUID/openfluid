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
 \file GeoVectorValue.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <openfluid/core/GeoVectorValue.hpp>

#include <openfluid/base/FrameworkException.hpp>

namespace openfluid {
namespace core {

// =====================================================================
// =====================================================================

GeoVectorValue::GeoVectorValue(std::string FilePath, std::string FileName) :
    GeoValue(FilePath, FileName), mp_Data(0)
{
  OGRRegisterAll();
}

// =====================================================================
// =====================================================================

GeoVectorValue::~GeoVectorValue()
{
  destroyDataSource();
}

// =====================================================================
// =====================================================================

void GeoVectorValue::destroyDataSource()
{
  if (mp_Data)
  {
    OGRDataSource::DestroyDataSource(mp_Data);
    mp_Data = 0;
  }
}

// =====================================================================
// =====================================================================

openfluid::core::UnstructuredValue::UnstructuredType GeoVectorValue::getType() const
{
  return openfluid::core::UnstructuredValue::GeoVectorValue;
}

// =====================================================================
// =====================================================================

OGRDataSource* GeoVectorValue::get()
{
  if (!mp_Data)
    tryToOpenSource();

  return mp_Data;
}

// =====================================================================
// =====================================================================

void GeoVectorValue::tryToOpenSource()
{
  mp_Data = OGRSFDriverRegistrar::Open(m_AbsolutePath.c_str(), false);

  if (!mp_Data)
    throw openfluid::base::FrameworkException(
        "GeoVectorValue::tryToOpenSource",
        "Error while trying to open file " + m_AbsolutePath);
}

// =====================================================================
// =====================================================================

OGRLayer* GeoVectorValue::getLayer(unsigned int LayerIndex)
{
  return get()->GetLayer(LayerIndex);
}

// =====================================================================
// =====================================================================

OGRFeatureDefn* GeoVectorValue::getLayerDef(unsigned int LayerIndex)
{
  return getLayer(LayerIndex)->GetLayerDefn();
}

// =====================================================================
// =====================================================================

bool GeoVectorValue::isLineType(unsigned int LayerIndex)
{
  return getLayerDef(LayerIndex)->GetGeomType() == wkbLineString;
}

// =====================================================================
// =====================================================================

bool GeoVectorValue::isPolygonType(unsigned int LayerIndex)
{
  return getLayerDef(LayerIndex)->GetGeomType() == wkbPolygon;
}

// =====================================================================
// =====================================================================

bool GeoVectorValue::containsField(std::string FieldName,
                                  unsigned int LayerIndex)
{
  return getLayerDef(LayerIndex)->GetFieldIndex(FieldName.c_str()) != -1;
}

// =====================================================================
// =====================================================================

int GeoVectorValue::getFieldIndex(std::string FieldName, unsigned int LayerIndex)
{
  return getLayerDef(LayerIndex)->GetFieldIndex(FieldName.c_str());
}

// =====================================================================
// =====================================================================

bool GeoVectorValue::isFieldOfType(std::string FieldName, OGRFieldType FieldType,
                                  unsigned int LayerIndex)
{
  if (!containsField(FieldName))
    throw openfluid::base::FrameworkException(
        "VectorDataset::isFieldOfType",
        "Field \"" + FieldName + "\" is not set.");

  return getLayerDef(LayerIndex)->GetFieldDefn(getFieldIndex(FieldName))->GetType()
      == FieldType;
}

// =====================================================================
// =====================================================================

}
} // namespaces
