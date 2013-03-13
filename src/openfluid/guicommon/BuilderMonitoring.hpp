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
 \file BuilderMonitoring.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef BUILDERMONITORING_HPP_
#define BUILDERMONITORING_HPP_

#include  <openfluid/fluidx/MonitoringDescriptor.hpp>

namespace openfluid {

namespace machine {
class ObserverPluginsManager;
class ObserverSignatureInstance;
}

namespace guicommon {

class BuilderMonitoring
{
  private:

    openfluid::fluidx::MonitoringDescriptor* mp_MonitoringDesc;

    openfluid::machine::ObserverPluginsManager* mp_Manager;

    std::vector<openfluid::machine::ObserverSignatureInstance*> m_AvailableSignatures;

  public:

    BuilderMonitoring(openfluid::fluidx::MonitoringDescriptor& MonitoringDesc);

    ~BuilderMonitoring();

    /**
     * @brief Checks that each Observer of the Monitoring descriptor is available
     * @details Update the list of all available Observers before checking
     * @param MissingObservers a textual list of Observers that are in the Monitoring but whose plugin files are not available,
     * or an empty string if all Observers of the Monitoring are available
     * @return A copy of the list of Observers that are in the Monitoring, whithout that are not available
     */
    std::list<openfluid::fluidx::ObserverDescriptor*> checkAndGetModifiedMonitoring(
        std::string& MissingObservers);

    const std::list<openfluid::fluidx::ObserverDescriptor*>& getItems();

    /**
     * @brief Returns the Signature of the Observer with ObserverID if available
     * @throw openfluid::base::OFException if this Observer plugin is not available
     */
    const openfluid::machine::ObserverSignatureInstance& getSignature(
        std::string ObserverID);

    /**
     * @brief Returns the Descriptor of the Observer with ObserverID if is in the Monitoring descriptor
     * @throw openfluid::base::OFException if this Observer is not in the Monitoring descriptor
     */
    openfluid::fluidx::ObserverDescriptor& getDescriptor(
        std::string ObserverID);

    /**
     * @brief Adds the Observer with ObserverID to the Monitoring descriptor if it's Signature is available
     * @throw openfluid::base::OFException if this Observer plugin is not available
     */
    void addToObserverList(std::string ObserverID);

    /**
     * @brief Removes the Observer with ObserverID from the Monitoring descriptor
     * @throw openfluid::base::OFException if this Observer is not in the Monitoring descriptor
     */
    void removeFromObserverList(std::string ObserverID);

    std::vector<openfluid::machine::ObserverSignatureInstance*> getUnusedAvailableSignatures();

    /**
     * @brief Replace existing observers with ObserversList
     */
    void setItems(
        std::list<openfluid::fluidx::ObserverDescriptor*> ObserversList);

    /**
     * @brief Updates the list of available signatures, according to Runtime environment paths
     */
    void update();

};

}
} //namespaces
#endif /* BUILDERMONITORING_HPP_ */
