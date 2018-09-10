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
  @file GeoValue.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/core/GeoValue.hpp>


namespace openfluid { namespace core {


// =====================================================================
// =====================================================================


GeoValue::GeoValue(const std::string& FilePath, const std::string& FileName) :
    m_FilePath(FilePath), m_FileName(FileName)
{
  m_AbsolutePath = computeAbsolutePath(m_FilePath, m_FileName);
}


// =====================================================================
// =====================================================================


GeoValue::~GeoValue()
{

}


// =====================================================================
// =====================================================================


std::string GeoValue::computeAbsolutePath(const std::string& FilePath,
                                          const std::string& FileName)
{
  return FilePath + "/" + FileName;
}


// =====================================================================
// =====================================================================


std::string GeoValue::getFilePath()
{
  return m_FilePath;
}


} } // namesaces
