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
  @file GeoRasterValue.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/core/GeoRasterValue.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace core {


GeoRasterValue::GeoRasterValue(const std::string& FilePath, const std::string& FileName) :
    GeoValue(FilePath, FileName), mp_Data(0)
{
  GDALAllRegister();
}


// =====================================================================
// =====================================================================


GeoRasterValue::~GeoRasterValue()
{
  if (mp_Data)
  {
    GDALClose(mp_Data);
  }
}


// =====================================================================
// =====================================================================


openfluid::core::UnstructuredValue::UnstructuredType GeoRasterValue::getType() const
{
  return openfluid::core::UnstructuredValue::GeoRasterValue;
}


// =====================================================================
// =====================================================================


GDALDataset* GeoRasterValue::data()
{
  if (!mp_Data)
  {
    tryToOpenSource();
  }

  return mp_Data;
}


// =====================================================================
// =====================================================================


void GeoRasterValue::tryToOpenSource()
{
  // GDALOpenShared to allow copy then close of this raster in virtual format (see http://www.gdal.org/gdal_vrttut.html)
  mp_Data = static_cast<GDALDataset*>(GDALOpenShared(m_AbsolutePath.c_str(),
                                               GA_ReadOnly));

  if (!mp_Data)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Error while trying to open file " + m_AbsolutePath +
                                              " (" + CPLGetLastErrorMsg() + ")");
  }

}


// =====================================================================
// =====================================================================


std::string GeoRasterValue::getAbsolutePath()
{
  return m_AbsolutePath;
}


// =====================================================================
// =====================================================================


}
} // namespaces
