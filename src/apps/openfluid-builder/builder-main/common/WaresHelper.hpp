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
 \file WaresHelper.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef WARESHELPER_HPP_
#define WARESHELPER_HPP_

#include <vector>
#include <list>
#include <string>

namespace openfluid {
namespace machine {
class ObserverSignatureInstance;
}
namespace fluidx {
class AdvancedMonitoringDescriptor;
class AdvancedModelDescriptor;
class ObserverDescriptor;
class ModelItemDescriptor;
}
}

class WaresHelper
{
  public:

    /**
     * @brief Checks that each Function of the Model descriptor is available
     * @details Update the list of all available Functions before checking
     * @param Desc the Model descriptor to check
     * @param MissingFunctions a list of IDs of Functions that are in the Model but whose plugin files are not available
     * @return A copy of the list of Functions that are in the Model, whithout that are not available
     */
    static std::list<openfluid::fluidx::ModelItemDescriptor*> checkAndGetModifiedModel(
        const openfluid::fluidx::AdvancedModelDescriptor& Desc,
        std::list<std::string>& MissingFunctions);

    /**
     * @brief Checks that each Observer of the Monitoring descriptor is available
     * @details Update the list of all available Observers before checking
     * @param Desc the Monitoring descriptor to check
     * @param MissingObservers a list of IDs of Observers that are in the Monitoring but whose plugin files are not available
     * @return A copy of the list of Observers that are in the Monitoring, whithout that are not available
     */
    static std::list<openfluid::fluidx::ObserverDescriptor*> checkAndGetModifiedMonitoring(
        const openfluid::fluidx::AdvancedMonitoringDescriptor& Desc,
        std::list<std::string>& MissingObservers);

    static std::vector<openfluid::machine::ObserverSignatureInstance*> getUnusedAvailableObserverSignatures(
        const openfluid::fluidx::AdvancedMonitoringDescriptor& Desc);
};

#endif /* WARESHELPER_HPP_ */
