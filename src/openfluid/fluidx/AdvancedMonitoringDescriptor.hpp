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
 @file AdvancedMonitoringDescriptor.hpp

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef ADVANCEDMONITORINGDESCRIPTOR_HPP_
#define ADVANCEDMONITORINGDESCRIPTOR_HPP_

#include  <openfluid/fluidx/MonitoringDescriptor.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid {
namespace fluidx {


// TODO refactor this for enable the possibility to have the same observer twice in the monitoring list
class OPENFLUID_API AdvancedMonitoringDescriptor
{
  private:

    openfluid::fluidx::MonitoringDescriptor* mp_MonitoringDesc;

  public:

    AdvancedMonitoringDescriptor(
        openfluid::fluidx::MonitoringDescriptor& MonitoringDesc);

    ~AdvancedMonitoringDescriptor();

    void checkMonitoring() const;

    const std::list<openfluid::fluidx::ObserverDescriptor*>& items() const;

    /**
     * @brief Returns the Descriptor of the Observer with ObserverID if is in the Monitoring descriptor
     * @throw openfluid::base::OFException if this Observer is not in the Monitoring descriptor
     */
    openfluid::fluidx::ObserverDescriptor& descriptor(std::string ObserverID) const;

    /**
     * @brief Adds the Observer with ObserverID to the Monitoring descriptor
     */
    void addToObserverList(std::string ObserverID);

    /**
     * @brief Removes the Observer with ObserverID from the Monitoring descriptor
     * @throw openfluid::base::OFException if this Observer is not in the Monitoring descriptor
     */
    void removeFromObserverList(std::string ObserverID);

    /**
     * @brief Replace existing observers with ObserversList
     */
    void setItems(
        std::list<openfluid::fluidx::ObserverDescriptor*> ObserversList);

    // TODO to rename to match equivalent in AdvancedModelDescriptor
    void moveItemTowardsTheBeginning(std::string ObserverID);

    // TODO to rename to match equivalent in AdvancedModelDescriptor
    void moveItemTowardsTheEnd(std::string ObserverID);

    int getFirstIndex(const std::string& ID) const;

    std::vector<openfluid::ware::WareID_t> getOrderedIDs() const;



};

}
} //namespaces
#endif /* ADVANCEDMONITORINGDESCRIPTOR_HPP_ */
