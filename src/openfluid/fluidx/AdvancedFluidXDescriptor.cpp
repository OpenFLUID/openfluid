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
  @file AdvancedFluidXDescriptor.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>

namespace openfluid { namespace fluidx {


// =====================================================================
// =====================================================================


AdvancedFluidXDescriptor::AdvancedFluidXDescriptor(openfluid::fluidx::FluidXDescriptor& FluidXDesc)
{
  mp_Domain = new AdvancedDomainDescriptor(FluidXDesc.spatialDomainDescriptor());
  mp_Model = new AdvancedModelDescriptor(FluidXDesc.modelDescriptor());
  mp_RunDesc = &(FluidXDesc.runDescriptor());
  mp_DatastoreDesc = &(FluidXDesc.datastoreDescriptor());
  mp_Datastore = new AdvancedDatastoreDescriptor(
      FluidXDesc.datastoreDescriptor());
  mp_Monitoring = new AdvancedMonitoringDescriptor(
      FluidXDesc.monitoringDescriptor());
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


AdvancedDomainDescriptor& AdvancedFluidXDescriptor::spatialDomain()
{
  return *mp_Domain;
}


// =====================================================================
// =====================================================================


const AdvancedDomainDescriptor& AdvancedFluidXDescriptor::spatialDomain() const
{
  return *mp_Domain;
}


// =====================================================================
// =====================================================================


AdvancedModelDescriptor& AdvancedFluidXDescriptor::model()
{
  return *mp_Model;
}


// =====================================================================
// =====================================================================


const AdvancedModelDescriptor& AdvancedFluidXDescriptor::model() const
{
  return *mp_Model;
}


// =====================================================================
// =====================================================================


openfluid::fluidx::RunDescriptor& AdvancedFluidXDescriptor::runDescriptor()
{
  return *mp_RunDesc;
}


// =====================================================================
// =====================================================================


const openfluid::fluidx::RunDescriptor& AdvancedFluidXDescriptor::runDescriptor() const
{
  return *mp_RunDesc;
}


// =====================================================================
// =====================================================================


openfluid::fluidx::DatastoreDescriptor& AdvancedFluidXDescriptor::datastoreDescriptor()
{
  return *mp_DatastoreDesc;
}


// =====================================================================
// =====================================================================


const openfluid::fluidx::DatastoreDescriptor& AdvancedFluidXDescriptor::datastoreDescriptor() const
{
  return *mp_DatastoreDesc;
}


// =====================================================================
// =====================================================================


AdvancedDatastoreDescriptor& AdvancedFluidXDescriptor::datastore()
{
  return *mp_Datastore;
}


// =====================================================================
// =====================================================================


AdvancedMonitoringDescriptor& AdvancedFluidXDescriptor::monitoring()
{
  return *mp_Monitoring;
}


// =====================================================================
// =====================================================================


const AdvancedMonitoringDescriptor& AdvancedFluidXDescriptor::monitoring() const
{
  return *mp_Monitoring;
}


} } // namespaces

