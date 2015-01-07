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
 @file GeoValue.hpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
 */

#ifndef GEOVALUE_HPP_
#define GEOVALUE_HPP_

#include <openfluid/core/UnstructuredValue.hpp>
#include <openfluid/dllexport.hpp>

namespace openfluid {
namespace core {

/**
  @brief Abstract class for geospatial data.
*/
class OPENFLUID_API GeoValue: public openfluid::core::UnstructuredValue
{
  protected:

    std::string m_FilePath;

    std::string m_FileName;

    std::string m_AbsolutePath;

    virtual void tryToOpenSource() = 0;

  public:

    GeoValue(const std::string& FilePath, const std::string& FileName);

    virtual ~GeoValue() = 0;

    std::string getFilePath();

    static std::string computeAbsolutePath(const std::string& FilePath, const std::string& FileName);
};

} } // namespaces

#endif /* GEOVALUE_HPP_ */
