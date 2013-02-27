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

    OGRDataSource* mp_Data;

    void tryToOpenSource(bool UpdateMode);

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
     * @param FileName The name or the relative path of the file to open
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

};

}
} // namespaces

#endif /* __GEOVECTORVALUE_HPP__ */
