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
 \file AdvancedFluidXDescriptor.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef __ADVANCEDFLUIDXDESCRIPTOR_HPP__
#define __ADVANCEDFLUIDXDESCRIPTOR_HPP__

#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/fluidx/AdvancedDomainDescriptor.hpp>
#include <openfluid/fluidx/AdvancedModelDescriptor.hpp>
#include <openfluid/fluidx/AdvancedMonitoringDescriptor.hpp>
#include <openfluid/fluidx/AdvancedDatastoreDescriptor.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace fluidx {

class DLLEXPORT AdvancedFluidXDescriptor
{
  private:

    AdvancedDomainDescriptor* mp_Domain;

    AdvancedModelDescriptor* mp_Model;

    openfluid::fluidx::RunDescriptor* mp_RunDesc;

    openfluid::fluidx::DatastoreDescriptor* mp_DatastoreDesc;

    AdvancedDatastoreDescriptor* mp_Datastore;

    AdvancedMonitoringDescriptor* mp_Monitoring;

  public:

    AdvancedFluidXDescriptor(openfluid::fluidx::FluidXDescriptor& FluidXDesc);

    ~AdvancedFluidXDescriptor();

    AdvancedDomainDescriptor& getDomain();

    const AdvancedDomainDescriptor& getDomain() const;

    AdvancedModelDescriptor& getModel();

    const AdvancedModelDescriptor& getModel() const;

    openfluid::fluidx::RunDescriptor& getRunDescriptor();

    const openfluid::fluidx::RunDescriptor& getRunDescriptor() const;

    openfluid::fluidx::DatastoreDescriptor& getDatastoreDescriptor();

    const openfluid::fluidx::DatastoreDescriptor& getDatastoreDescriptor() const;

    AdvancedDatastoreDescriptor& getDatastore();

    AdvancedMonitoringDescriptor& getMonitoring();

    const AdvancedMonitoringDescriptor& getMonitoring() const;
};

} } // namespaces

#endif /* __ADVANCEDFLUIDXDESCRIPTOR_HPP__ */
