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
 \file BuilderModel.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef BUILDERMODEL_HPP_
#define BUILDERMODEL_HPP_

#include <openfluid/fluidx/CoupledModelDescriptor.hpp>

namespace openfluid {
namespace guicommon {

class BuilderModel
{
    openfluid::fluidx::CoupledModelDescriptor* mp_ModelDesc;

    void checkModel();

  public:

    BuilderModel(openfluid::fluidx::CoupledModelDescriptor& ModelDesc);

    ~BuilderModel();

    const std::list<openfluid::fluidx::ModelItemDescriptor*>& getItems();

    /**
     * @throw openfluid::base::OFException if Index is out of range
     * @param Index
     * @return
     */
    openfluid::fluidx::ModelItemDescriptor* getItemAt(unsigned int Index);

    /**
     * Return the position of the firts Item with ItemID found in the list, or -1 if not found
     * @param ItemID
     * @return
     */
    int getFirstItemIndex(std::string ItemID);

    int getFirstItemIndex(openfluid::fluidx::ModelItemDescriptor* Item);

    std::vector<std::string> getOrderedIDs();

    void appendItem(openfluid::fluidx::ModelItemDescriptor* Item);

    /**
     Insert an Item before the given position (positions starts at index 0)
     @param[in] Item the ModelItemDescriptor to insert
     @param[in] Position the position, should be between zero and list size - 1.
     To insert an Item at the end of the list, use appendItem instead.
     @throw openfluid::base::OFException if Position is out of range
     */
    void insertItem(openfluid::fluidx::ModelItemDescriptor* Item,
                    unsigned int Position);

    /**
     Remove from the list the Item located at the given Position (positions starts at index 0).
     This doesn't delete the ModelItemDescriptor pointer.
     @param[in] Position the position
     @throw openfluid::base::OFException if Position is out of range
     */
    void removeItem(unsigned int Position);

    /**
     * Move the Item located at the given From position to the To position (positions starts at index 0)
     * @param From
     * @param To
     * @throw openfluid::base::OFException a position is out of range
     */
    void moveItem(unsigned int From, unsigned int To);

    unsigned int getItemsCount() const;

    void setGlobalParameter(const openfluid::ware::WareParamKey_t& Key,
                            const openfluid::ware::WareParamKey_t& Value);

    void setGlobalParameters(const openfluid::ware::WareParams_t& Params);

    openfluid::ware::WareParams_t getGlobalParameters();

    void eraseGlobalParameter(const openfluid::ware::WareParamKey_t& Key);
};

}
} // namespaces
#endif /* BUILDERMODEL_HPP_ */
