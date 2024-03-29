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
  @file DatastoreItem.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
*/


#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/core/DatastoreItem.hpp>


namespace openfluid { namespace core {


DatastoreItem::DatastoreItem(const std::string& ID,
                             const std::string& PrefixPath, const std::string& RelativePath,
                             UnstructuredValue::UnstructuredType Type, const std::string& UnitsClass) :
  m_ID(ID), m_PrefixPath(PrefixPath), m_RelativePath(RelativePath), m_UnitsClass(UnitsClass)
{
  if (Type == UnstructuredValue::UnstructuredType::VECTOR)
  {
    m_Value = std::make_shared<openfluid::core::GeoVectorValue>(m_PrefixPath,m_RelativePath);
  }
  else if (Type == UnstructuredValue::UnstructuredType::RASTER)
  {
    m_Value = std::make_shared<openfluid::core::GeoRasterValue>(m_PrefixPath,m_RelativePath);
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"No value to instanciate for item type "
                                                + UnstructuredValue::getStringFromValueType(Type) + " in " + ID);
  }
}


// =====================================================================
// =====================================================================


std::string DatastoreItem::getID() const
{
  return m_ID;
}


// =====================================================================
// =====================================================================


std::string DatastoreItem::getPrefixPath() const
{
  return m_PrefixPath;
}


// =====================================================================
// =====================================================================


std::string DatastoreItem::getRelativePath() const
{
  return m_RelativePath;
}


// =====================================================================
// =====================================================================


std::string DatastoreItem::getUnitsClass() const
{
  return m_UnitsClass;
}


// =====================================================================
// =====================================================================


UnstructuredValue* DatastoreItem::value()
{
  return m_Value.get();
}


// =====================================================================
// =====================================================================


const UnstructuredValue* DatastoreItem::value() const
{
  return m_Value.get();
}


} } // namespaces
