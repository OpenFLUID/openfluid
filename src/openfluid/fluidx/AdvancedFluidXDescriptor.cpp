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
 \file AdvancedFluidXDescriptor.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>

namespace openfluid { namespace fluidx {


// =====================================================================
// =====================================================================


AdvancedFluidXDescriptor::AdvancedFluidXDescriptor(openfluid::fluidx::FluidXDescriptor& FluidXDesc)
{
  mp_Domain = new AdvancedDomainDescriptor(FluidXDesc.getDomainDescriptor());
  mp_Model = new AdvancedModelDescriptor(FluidXDesc.getModelDescriptor());
  mp_RunDesc = &(FluidXDesc.getRunDescriptor());
  mp_DatastoreDesc = &(FluidXDesc.getDatastoreDescriptor());
  mp_Datastore = new AdvancedDatastoreDescriptor(
      FluidXDesc.getDatastoreDescriptor());
  mp_Monitoring = new AdvancedMonitoringDescriptor(
      FluidXDesc.getMonitoringDescriptor());
}


// =====================================================================
// =====================================================================


AdvancedFluidXDescriptor::~AdvancedFluidXDescriptor()
{
  delete mp_Domain;
  delete mp_Model;
  delete mp_Datastore;
  delete mp_Monitoring;
}


// =====================================================================
// =====================================================================


AdvancedDomainDescriptor& AdvancedFluidXDescriptor::getDomain()
{
  return *mp_Domain;
}


// =====================================================================
// =====================================================================


const AdvancedDomainDescriptor& AdvancedFluidXDescriptor::getDomain() const
{
  return *mp_Domain;
}


// =====================================================================
// =====================================================================


AdvancedModelDescriptor& AdvancedFluidXDescriptor::getModel()
{
  return *mp_Model;
}


// =====================================================================
// =====================================================================


const AdvancedModelDescriptor& AdvancedFluidXDescriptor::getModel() const
{
  return *mp_Model;
}


// =====================================================================
// =====================================================================


openfluid::fluidx::RunDescriptor& AdvancedFluidXDescriptor::getRunDescriptor()
{
  return *mp_RunDesc;
}


// =====================================================================
// =====================================================================


const openfluid::fluidx::RunDescriptor& AdvancedFluidXDescriptor::getRunDescriptor() const
{
  return *mp_RunDesc;
}


// =====================================================================
// =====================================================================


openfluid::fluidx::DatastoreDescriptor& AdvancedFluidXDescriptor::getDatastoreDescriptor()
{
  return *mp_DatastoreDesc;
}


// =====================================================================
// =====================================================================


const openfluid::fluidx::DatastoreDescriptor& AdvancedFluidXDescriptor::getDatastoreDescriptor() const
{
  return *mp_DatastoreDesc;
}


// =====================================================================
// =====================================================================


AdvancedDatastoreDescriptor& AdvancedFluidXDescriptor::getDatastore()
{
  return *mp_Datastore;
}


// =====================================================================
// =====================================================================


AdvancedMonitoringDescriptor& AdvancedFluidXDescriptor::getMonitoring()
{
  return *mp_Monitoring;
}


// =====================================================================
// =====================================================================


const AdvancedMonitoringDescriptor& AdvancedFluidXDescriptor::getMonitoring() const
{
  return *mp_Monitoring;
}


} } // namespaces

